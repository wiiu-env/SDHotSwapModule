#include "exports.h"
#include "logger.h"
#include <coreinit/cache.h>
#include <coreinit/filesystem.h>
#include <mutex>

static FSClient sClient;
std::mutex *mutex;

bool sFSClientAdded = false;

int InitialSDCheck() {
    mutex = new (std::nothrow) std::mutex;
    if (!mutex) {
        return -1;
    }
    FSStatus result = FSAddClient(&sClient, FS_ERROR_FLAG_ALL);
    if (result != FS_STATUS_OK) {
        return -2;
    } else {
        sFSClientAdded = true;
        OSMemoryBarrier();
        FSCmdBlock fsCmd;
        FSInitCmdBlock(&fsCmd);
        FSMountSource mountSource;
        return FSGetMountSource(&sClient, &fsCmd, FS_MOUNT_SOURCE_SD, &mountSource, FS_ERROR_FLAG_ALL) == FS_STATUS_OK;
    }
}

void DeInitSDCheck() {
    if (mutex) {
        delete mutex;
        mutex = nullptr;
    }

    if (sFSClientAdded) {
        FSDelClient(&sClient, FS_ERROR_FLAG_ALL);
        memset(&sClient, 0, sizeof(sClient));
        sFSClientAdded = false;
    }

    cleanUpAttachCallbacks();

    OSMemoryBarrier();
}

int IsSDCardInserted() {
    if (!sFSClientAdded || !mutex) {
        return -1;
    }
    std::lock_guard<std::mutex> lk(*mutex);
    FSCmdBlock fsCmd;
    FSMountSource mountSource;
    FSInitCmdBlock(&fsCmd);
    memset(&mountSource, 0, sizeof(mountSource));
    return FSGetMountSource(&sClient, &fsCmd, FS_MOUNT_SOURCE_SD, &mountSource, FS_ERROR_FLAG_ALL) == FS_STATUS_OK;
}

int MountSDCard() {
    if (!sFSClientAdded || !mutex) {
        return -1;
    }
    std::lock_guard<std::mutex> lk(*mutex);
    FSCmdBlock fsCmd;
    FSInitCmdBlock(&fsCmd);
    FSMountSource mountSource;
    char mountPath[0x80];

    if (FSGetMountSource(&sClient, &fsCmd, FS_MOUNT_SOURCE_SD, &mountSource, FS_ERROR_FLAG_ALL) != FS_STATUS_OK) {
        DEBUG_FUNCTION_LINE("No SD Card found");
        return 0;
    }

    FSStatus res;
    if ((res = FSMount(&sClient, &fsCmd, &mountSource, mountPath, sizeof(mountPath), FS_ERROR_FLAG_ALL)) != FS_STATUS_OK) {
        DEBUG_FUNCTION_LINE("FSMount failed %d", res);
        return 0;
    }
    DEBUG_FUNCTION_LINE("Mounted SD Card");
    return 1;
}

int UnmountSDCard() {
    if (!sFSClientAdded || !mutex) {
        return -1;
    }
    std::lock_guard<std::mutex> lk(*mutex);
    FSCmdBlock fsCmd;
    FSInitCmdBlock(&fsCmd);

    FSStatus res = FS_STATUS_OK;
    while (res == FS_STATUS_OK) {
        res = FSUnmount(&sClient, &fsCmd, "/vol/external01", FS_ERROR_FLAG_ALL);
        if (res != FS_STATUS_OK && res != FS_STATUS_NOT_FOUND) {
            return 0;
        }
    }
    DEBUG_FUNCTION_LINE("Unmounted SD Card");

    return 1;
}

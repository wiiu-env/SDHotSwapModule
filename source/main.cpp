#include "exports.h"
#include "logger.h"
#include "sdcard.h"
#include "version.h"
#include <condition_variable>
#include <coreinit/cache.h>
#include <coreinit/filesystem.h>
#include <coreinit/thread.h>
#include <mutex>
#include <sdutils/sdutils.h>
#include <thread>
#include <wums.h>

WUMS_MODULE_EXPORT_NAME("homebrew_sdhotswap");
WUMS_MODULE_SKIP_INIT_FINI();

// This NEEDS to be on the heap. Global DTOR are never called for modules
// but we need a fresh instance of this condition_variable on each app change.
std::condition_variable *cv = nullptr;
std::mutex *cv_m            = nullptr;

std::thread *mountThread     = nullptr;
bool sStopThread             = false;
bool sIsSDInsertedAndMounted = false;

#define MODULE_VERSION      "v0.1"
#define MODULE_VERSION_FULL MODULE_VERSION MODULE_VERSION_EXTRA

int mount_thread() {
    std::unique_lock<std::mutex> lk(*cv_m);
    // Wait until the main thread has checked the sd status once.
    cv->wait(lk);
    while (!sStopThread) {
        auto newStatus = IsSDCardInserted();
        if (newStatus != sIsSDInsertedAndMounted) {
            if (newStatus) {
                if (MountSDCard()) {
                    callAttachCallbacks(SDUTILS_ATTACH_MOUNTED);
                    sIsSDInsertedAndMounted = true;
                }
            } else {
                if (UnmountSDCard()) {
                    callAttachCallbacks(SDUTILS_ATTACH_UNMOUNTED);
                    sIsSDInsertedAndMounted = false;
                }
            }
            OSMemoryBarrier();
        }

        OSSleepTicks(OSMillisecondsToTicks(100));
    }
    return 0;
}

WUMS_APPLICATION_STARTS() {
    OSReport("Running SDHotSwapModule " MODULE_VERSION_FULL "\n");
    initLogging();

    sStopThread = false;

    cv   = new std::condition_variable;
    cv_m = new std::mutex;

    OSMemoryBarrier();

    mountThread       = new std::thread(mount_thread);
    auto nativeHandle = (OSThread *) mountThread->native_handle();
    OSSetThreadName(nativeHandle, "SDHotSwapModuleThread");
    while (!OSSetThreadAffinity(nativeHandle, OS_THREAD_ATTRIB_AFFINITY_CPU2)) {
        OSSleepTicks(OSMillisecondsToTicks(16));
    }

    auto check = InitialSDCheck();
    if (check < 0) {
        // On error stop thread.
        sStopThread = true;
    } else {
        // Otherwise it retuns the current status.
        sIsSDInsertedAndMounted = check == 1;
    }

    OSMemoryBarrier();

    DEBUG_FUNCTION_LINE_VERBOSE("Wake up the thread");
    // Now we can wake up the thread!
    cv->notify_all();
}

WUMS_APPLICATION_ENDS() {
    sStopThread = true;
    OSMemoryBarrier();
    cv->notify_all();

    if (mountThread != nullptr) {
        mountThread->join();

        delete mountThread;
        mountThread = nullptr;
    }

    DeInitSDCheck();

    delete cv;
    delete cv_m;
    cv   = nullptr;
    cv_m = nullptr;

    OSMemoryBarrier();
    deinitLogging();
}

#include "exports.h"
#include <cstring>
#include <sdutils/sdutils.h>
#include <wums.h>

#define MAX_HANDLERS 16
static SDAttachHandlerFn sHandlers[MAX_HANDLERS] = {nullptr};

void callAttachCallbacks(SDUtilsAttachStatus status) {
    int i;
    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sHandlers[i]) {
            sHandlers[i](status);
        }
    }
}

void cleanUpAttachCallbacks() {
    memset(sHandlers, 0, sizeof(sHandlers));
}

bool SDUtilsAddAttachHandler(SDAttachHandlerFn fn) {
    int i;

    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sHandlers[i] == fn) {
            return true;
        }
        if (!sHandlers[i]) {
            sHandlers[i] = fn;
            return true;
        }
    }

    return false;
}

bool SDUtilsRemoveAttachHandler(SDAttachHandlerFn fn) {
    int i;

    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sHandlers[i] == fn) {
            sHandlers[i] = nullptr;
            return true;
        }
    }

    return false;
}

WUMS_EXPORT_FUNCTION(SDUtilsAddAttachHandler);
WUMS_EXPORT_FUNCTION(SDUtilsRemoveAttachHandler);
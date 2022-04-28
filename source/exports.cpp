#include "exports.h"
#include <cstring>
#include <sdutils/sdutils.h>
#include <wums.h>

#define MAX_HANDLERS 16
static SDAttachHandlerFn sHandlers[MAX_HANDLERS] = {nullptr};

static SDCleanUpHandlesHandlerFn sCleanupHandlesHandlers[MAX_HANDLERS] = {nullptr};

void callAttachCallbacks(SDUtilsAttachStatus status) {
    int i;
    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sHandlers[i]) {
            sHandlers[i](status);
        }
    }
}

void callCleanUpHandlesCallbacks() {
    int i;
    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sCleanupHandlesHandlers[i]) {
            sCleanupHandlesHandlers[i]();
        }
    }
}

void cleanUpAttachCallbacks() {
    memset(sHandlers, 0, sizeof(sHandlers));
}

void cleanUpCleanUpHandlesCallbacks() {
    memset(sCleanupHandlesHandlers, 0, sizeof(sCleanupHandlesHandlers));
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

bool SDUtilsAddCleanUpHandlesHandler(SDCleanUpHandlesHandlerFn fn) {
    int i;

    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sCleanupHandlesHandlers[i] == fn) {
            return true;
        }
        if (!sCleanupHandlesHandlers[i]) {
            sCleanupHandlesHandlers[i] = fn;
            return true;
        }
    }

    return false;
}

bool SDUtilsRemoveCleanUpHandlesHandler(SDCleanUpHandlesHandlerFn fn) {
    int i;

    for (i = 0; i < MAX_HANDLERS; ++i) {
        if (sCleanupHandlesHandlers[i] == fn) {
            sCleanupHandlesHandlers[i] = nullptr;
            return true;
        }
    }

    return false;
}

SDUtilsVersion SDUtilsGetVersion() {
    return SDUTILS_MODULE_VERSION;
}

WUMS_EXPORT_FUNCTION(SDUtilsGetVersion);

WUMS_EXPORT_FUNCTION(SDUtilsAddAttachHandler);
WUMS_EXPORT_FUNCTION(SDUtilsRemoveAttachHandler);

WUMS_EXPORT_FUNCTION(SDUtilsAddCleanUpHandlesHandler);
WUMS_EXPORT_FUNCTION(SDUtilsRemoveCleanUpHandlesHandler);
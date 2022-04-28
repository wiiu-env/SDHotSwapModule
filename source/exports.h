#pragma once
#include <sdutils/sdutils.h>

void callAttachCallbacks(SDUtilsAttachStatus status);
void cleanUpAttachCallbacks();

void callCleanUpHandlesCallbacks();
void cleanUpCleanUpHandlesCallbacks();
#pragma once

int InitialSDCheck();

void DeInitSDCheck();

/**
 * @return < 0 on error.
 *         0 when no FAT32 formatted sd card is inserted
 *         1 when a FAT32 formatted sd card is inserted
 */

int IsSDCardInserted();

/**
 * @return < 0 on error.
 *         0 when mounting failed,
 *         1 when mounting was successful.
 */
int MountSDCard();

/**
 * @return < 0 on error.
 *         0 when unmounting failed,
 *         1 when unmounting was successful.
 */
int UnmountSDCard();
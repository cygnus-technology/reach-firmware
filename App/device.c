/*
 * Copyright (c) 2023-2024 i3 Product Development
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/********************************************************************************************
 *    _ ____  ___             _         _     ___              _                        _
 *   (_)__ / | _ \_ _ ___  __| |_  _ __| |_  |   \ _____ _____| |___ _ __ _ __  ___ _ _| |_
 *   | ||_ \ |  _/ '_/ _ \/ _` | || / _|  _| | |) / -_) V / -_) / _ \ '_ \ '  \/ -_) ' \  _|
 *   |_|___/ |_| |_| \___/\__,_|\_,_\__|\__| |___/\___|\_/\___|_\___/ .__/_|_|_\___|_||_\__|
 *                                                                  |_|
 *                           -----------------------------------
 *                       Copyright i3 Product Development 2023-2024
 *
 * \brief Provides the device info service to a Reach-enabled application.
 *
 ********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cr_stack.h"
#include "i3_log.h"
#include "version.h"
#include "reach_silabs.h"
#include "reach-server.h"  // determines configuration


// This is intended to live in flash
const cr_DeviceInfoResponse test1_di = 
{
    .protocol_version       = cr_ReachProtoVersion_CURRENT_VERSION,
    .device_name            = "Jack Reacher",
    .manufacturer           = "i3 Product Development",
    .device_description     = "This is a test of Jack Reacher's System.",
    .firmware_version       = "-1.-1.-1", // to be overwritten
  #ifdef ENABLE_REMOTE_CLI
    .services = cr_ServiceIds_PARAMETER_REPO + cr_ServiceIds_FILES + cr_ServiceIds_COMMANDS + cr_ServiceIds_CLI,
  #else
    .services = cr_ServiceIds_PARAMETER_REPO + cr_ServiceIds_FILES + cr_ServiceIds_COMMANDS,
  #endif
    .parameter_metadata_hash    = 22, // this will be overwritten
    .has_application_identifier = 0,  // false
    .application_identifier     = {0},
    .endpoints                  = 0,
};


// The stack will call this function.
// The const copy of the basis in flash is copied to RAM so that the device
// can overwrite varying data like SN and hash.
int crcb_device_get_info(cr_DeviceInfoResponse *pDi)
{
    // The app owns the memory here.
    // The address is returned so that the data can come from flash.
    // use memcpy as the structure copy imposes an unnecessary address alignment requirement.
    memcpy(pDi, &test1_di, sizeof(cr_DeviceInfoResponse));
    i3_log(LOG_MASK_ALWAYS, "%s: %s\n", __FUNCTION__, test1_di.device_name);

    sprintf(pDi->firmware_version, "%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);

    snprintf(pDi->device_name, REACH_DEVICE_NAME_LEN, "%s", rsl_get_advertised_name());
    return 0;
}

int crcb_ping_get_signal_strength(int8_t *rssi)
{
    sl_bt_connection_get_median_rssi(rsl_get_connection(), rssi);
    return 0;
}


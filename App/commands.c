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
 *                        Copyright i3 Product Development 2023-2024
 * 
 * \brief Provides the device info service to a Reach-enabled application.
 *
 ********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cr_stack.h"
#include "i3_log.h"
#include "reach_silabs.h"

#include "nvm3_default.h"

//*************************************************************************
//  Command Service
//*************************************************************************

#define NUM_COMMANDS    5

#define COMMAND_ENABLE_RCLI     1
#define COMMAND_DISABLE_RCLI    2
#define COMMAND_MIN_LOG         3
#define COMMAND_MAX_LOG         4
#define COMMAND_FACTORY         10

uint8_t sCommandIndex = 0;
const cr_CommandInfo sCommands[NUM_COMMANDS] = 
{
    {COMMAND_ENABLE_RCLI,   "Enable remote CLI"},
    {COMMAND_DISABLE_RCLI,  "Disable remote CLI"},
    {COMMAND_MIN_LOG,       "Minimize Logging (lm 0)"},
    {COMMAND_MAX_LOG,       "Much Logging (lm 7C0)"},
    {COMMAND_FACTORY,       "Factory Reset"}     // erases NVM, restores serial number

    //    {47, "Trigger OTA"},        // example not implemented here.
    //    {99, "Board Reset"},        // example not implemented here.

};

int crcb_file_get_command_count()
{
    return NUM_COMMANDS;
}

int crcb_command_discover_next(cr_CommandInfo *cmd_desc)
{
    if (sCommandIndex >= NUM_COMMANDS)
    {
        I3_LOG(LOG_MASK_REACH, "%s: Command index %d indicates discovery complete.",
               __FUNCTION__, sCommandIndex);
        return cr_ErrorCodes_NO_DATA;
    }
    *cmd_desc = sCommands[sCommandIndex++];
    return 0;
}

int crcb_command_discover_reset(const uint32_t cid)
{
    if (cid >= NUM_COMMANDS)
    {
        i3_log(LOG_MASK_ERROR, "%s: Command ID %d does not exist.",
               __FUNCTION__, cid);
        return cr_ErrorCodes_INVALID_PARAMETER;
    }
    sCommandIndex = cid;
    return 0;
}

// actually execute the command
int crcb_command_execute(const uint8_t cid)
{
    for (int i=0; i< NUM_COMMANDS; i++)
    {
        if (cid == sCommands[i].id)
        {
            i3_log(LOG_MASK_ALWAYS, "Execute command %d, '%s'", 
                   sCommands[i].id, sCommands[i].name);
            switch (cid) {
            case COMMAND_ENABLE_RCLI:  // 1
                i3_log_set_remote_cli_enable(true);
                break;
            case COMMAND_DISABLE_RCLI:  // 2
                i3_log_set_remote_cli_enable(false);
                break;
            case COMMAND_MIN_LOG:  // 3
                i3_log_set_mask(0);
                break;
            case COMMAND_MAX_LOG:  // 4
                i3_log_set_mask(0x7C0);
                break;
            case COMMAND_FACTORY:
            {
                unsigned int sn;
                int has_sn = !rsl_read_serial_number(&sn);
                Ecode_t eCode = nvm3_eraseAll(nvm3_defaultHandle);
                i3_log(LOG_MASK_ALWAYS, "nvm3_eraseAll() returned 0x%x", eCode);
                extern void init_param_repo();
                eCode = nvm3_eraseAll(nvm3_defaultHandle);
                i3_log(LOG_MASK_ALWAYS, "nvm3_eraseAll() returned 0x%x", eCode);
                init_param_repo();
                if (has_sn) {
                    nvm3_writeData(nvm3_defaultHandle, REACH_SN_KEY, (uint8_t *)&sn, sizeof(unsigned int));
                    i3_log(LOG_MASK_ALWAYS, "Serial number restored as %d.", sn);
                }
                break;
            }
            default:
                i3_log(LOG_MASK_ALWAYS, "Command not implemented.");
                break;
            }
            return 0;
        }
    }
    i3_log(LOG_MASK_ERROR, "%s: Command ID %d not found.",
           __FUNCTION__, cid);
    return cr_ErrorCodes_INVALID_PARAMETER;
}


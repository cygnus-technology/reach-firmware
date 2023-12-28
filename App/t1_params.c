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
 *                          Copyright i3 Product Development 2023
 *
 * \brief "t1_params.c" provides a trivial 'fake' parameter repository.
 *
 * Original Author: Chuck.Peplinski
 *
 ********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "nvm3_default.h"

#include "cr_stack.h"
#include "i3_log.h"
#include "version.h"
#include "const_params.h"
#include "reach_silabs.h"

#define MSG_BUFFER_SIZE	256

// This is effectively a 'fake' parameter repository.
// The init function makes it valid.
static cr_ParameterValue sCr_param_val[NUM_PARAMS];

static int read_param_from_nvm(const uint32_t pid, cr_ParameterValue *param);
static int write_param_to_nvm(const uint32_t pid, const cr_ParameterValue *param);

void init_param_repo()
{
    int rval = 0;
    for (int i=0; i<NUM_PARAMS; i++)
    {
        sCr_param_val[i].parameter_id = param_desc[i].id;

        // the PID directly maps to the parameter type, just to make it easy.
        switch (param_desc[i].data_type)
        {
        case cr_ParameterDataType_UINT32: // pid 1, 23
            sCr_param_val[i].value.uint32_value = 1984;
            sCr_param_val[i].which_value = cr_ParameterValue_uint32_value_tag;
            break;
        case cr_ParameterDataType_INT32: // pid 3, 25
            sCr_param_val[i].value.sint32_value = -1999;
            sCr_param_val[i].which_value = cr_ParameterValue_sint32_value_tag;
            break;
        case cr_ParameterDataType_FLOAT32: // pid 5, 27
            sCr_param_val[i].value.float32_value = 0.993;
            sCr_param_val[i].which_value = cr_ParameterValue_float32_value_tag;
            break;
        case cr_ParameterDataType_UINT64:  // pid 7, 29
            sCr_param_val[i].value.uint64_value = 441;
            sCr_param_val[i].which_value = cr_ParameterValue_uint64_value_tag;
            break;
        case cr_ParameterDataType_INT64:  // pid 9
            sCr_param_val[i].value.sint64_value = -10853;
            sCr_param_val[i].which_value = cr_ParameterValue_sint64_value_tag;
            break;
        case cr_ParameterDataType_FLOAT64:  // pid 11
            sCr_param_val[i].value.float64_value = 0.51111111111;
            sCr_param_val[i].which_value = cr_ParameterValue_float64_value_tag;
            break;
        case cr_ParameterDataType_BOOL:  // pid 13
            sCr_param_val[i].value.bool_value = true;
            sCr_param_val[i].which_value = cr_ParameterValue_bool_value_tag;
            break;
        case cr_ParameterDataType_STRING:  // pid 15
            sprintf(sCr_param_val[i].value.string_value, "Flea bag");
            sCr_param_val[i].which_value = cr_ParameterValue_string_value_tag;
            break;
        case cr_ParameterDataType_ENUMERATION:  // 17
            sCr_param_val[i].value.enum_value = 3;
            sCr_param_val[i].which_value = cr_ParameterValue_enum_value_tag;
            break;
        case cr_ParameterDataType_BIT_FIELD:  // 19
            sCr_param_val[i].value.bitfield_value = 0xDEAD;
            sCr_param_val[i].which_value = cr_ParameterValue_bitfield_value_tag;
            break;
        case cr_ParameterDataType_BYTE_ARRAY:  // 21
            sprintf((char*)sCr_param_val[i].value.bytes_value.bytes, "a byte array");
            sCr_param_val[i].value.bytes_value.size  = 13;
            sCr_param_val[i].which_value = cr_ParameterValue_bytes_value_tag;
            break;
        default:
            affirm(0);  // should not happen.
            break;
        }  // end switch

        // read any stored values from NVM
        switch (param_desc[i].storage_location) {
        default:
        case cr_StorageLocation_STORAGE_LOCATION_INVALID:
            i3_log(LOG_MASK_ERROR, "At param index %d, invalid storage location %d.",
                   i, param_desc[i].storage_location);
            break;
        case cr_StorageLocation_RAM:
        case cr_StorageLocation_RAM_EXTENDED:
            break;  // no need to read
        case cr_StorageLocation_NONVOLATILE_EXTENDED:
            // cr_StorageLocation_NONVOLATILE_EXTENDED is intended to 
            // support a system with more than one NVM region. 
            i3_log(LOG_MASK_ERROR, "At param index %d, NVM-EX not supported.", i);
            break;
        case cr_StorageLocation_NONVOLATILE:
            rval = read_param_from_nvm(param_desc[i].id, &sCr_param_val[i]);
            if (rval != 0) {
                // first time?
                write_param_to_nvm(param_desc[i].id, &sCr_param_val[i]);
                i3_log(LOG_MASK_ALWAYS, "Initialized pid %d in NVM.", param_desc[i].id);
            }
            break;
        }

    } // end for
}

// Populate a parameter value structure
int crcb_parameter_read(const uint32_t pid, cr_ParameterValue *data)
{
    affirm(data != NULL);

    for (int i=0; i<NUM_PARAMS; i++) {
        if (sCr_param_val[i].parameter_id == pid) {
            *data = sCr_param_val[i];
            // to do: write timestamp.
            return 0;
        }
    }
    return cr_ErrorCodes_INVALID_PARAMETER;
}

int crcb_parameter_write(const uint32_t pid, const cr_ParameterValue *data)
{
    int rval = 0;

    for (int i=0; i<NUM_PARAMS; i++) {
        if (sCr_param_val[i].parameter_id == pid) 
        {
            I3_LOG(LOG_MASK_PARAMS, "Write param[%d], pid %d (%d)", 
                   i, pid, data->parameter_id);
            I3_LOG(LOG_MASK_PARAMS, "  timestamp %d", data->timestamp);
            I3_LOG(LOG_MASK_PARAMS, "  which %d", data->which_value);
            sCr_param_val[i].timestamp = data->timestamp;
            sCr_param_val[i].which_value = data->which_value;

            switch (data->which_value)
            {
            case cr_ParameterValue_uint32_value_tag:
                sCr_param_val[i].value.uint32_value = data->value.uint32_value;
                break;
            case cr_ParameterValue_sint32_value_tag:
                sCr_param_val[i].value.sint32_value = data->value.sint32_value;
                break;
            case cr_ParameterValue_float32_value_tag:
                sCr_param_val[i].value.float32_value = data->value.float32_value;
                break;
            case cr_ParameterValue_uint64_value_tag:
                sCr_param_val[i].value.uint64_value = data->value.uint64_value;
                break;
            case cr_ParameterValue_sint64_value_tag:
                sCr_param_val[i].value.sint64_value = data->value.sint64_value;
                break;
            case cr_ParameterValue_float64_value_tag:
                sCr_param_val[i].value.float64_value = data->value.float64_value;
                break;
            case cr_ParameterValue_bool_value_tag:
                sCr_param_val[i].value.bool_value = data->value.bool_value;
                break;
            case cr_ParameterValue_string_value_tag:
                memcpy(sCr_param_val[i].value.string_value,
                       data->value.string_value, REACH_PVAL_STRING_LEN);
                sCr_param_val[i].value.string_value[REACH_PVAL_STRING_LEN-1] = 0;
                I3_LOG(LOG_MASK_PARAMS, "String value: %s",
                       sCr_param_val[i].value.string_value);
                break;
            case cr_ParameterValue_bitfield_value_tag:
                sCr_param_val[i].value.bitfield_value = data->value.bitfield_value;
                break;
            case cr_ParameterValue_enum_value_tag:
                sCr_param_val[i].value.enum_value = data->value.enum_value;
                break;
            case cr_ParameterValue_bytes_value_tag:
                memcpy(sCr_param_val[i].value.bytes_value.bytes, 
                       data->value.bytes_value.bytes, 
                       REACH_PVAL_BYTES_LEN);
                if (data->value.bytes_value.size > REACH_PVAL_BYTES_LEN)
                {
                    LOG_ERROR("Parameter write of bytes has invalide size %d > %d", 
                              data->value.bytes_value.size, REACH_PVAL_BYTES_LEN);
                    sCr_param_val[i].value.bytes_value.size = REACH_PVAL_BYTES_LEN;
                }
                else
                {
                    sCr_param_val[i].value.bytes_value.size = data->value.bytes_value.size;
                }
                LOG_DUMP_MASK(LOG_MASK_PARAMS, "bytes value",
                              sCr_param_val[i].value.bytes_value.bytes,
                              sCr_param_val[i].value.bytes_value.size);
                break;
            default:
                LOG_ERROR("Parameter write which_value %d not recognized.", 
                              data->which_value);
                rval = 1;
                break;
            }  // end switch

            if (rval == 0) {

                // Store to NVM if appropriate
                switch (param_desc[i].storage_location) {
                default:
                case cr_StorageLocation_STORAGE_LOCATION_INVALID:
                    i3_log(LOG_MASK_ERROR, "%s: At param index %d, invalid storage location %d.",
                           __FUNCTION__, i, param_desc[i].storage_location);
                    break;
                case cr_StorageLocation_RAM:
                case cr_StorageLocation_RAM_EXTENDED:
                    break;  // no need to read
                case cr_StorageLocation_NONVOLATILE_EXTENDED:
                    // cr_StorageLocation_NONVOLATILE_EXTENDED is intended to 
                    // support a system with more than one NVM region. 
                    i3_log(LOG_MASK_ERROR, "%s: At param index %d, NVM-EX not supported.", __FUNCTION__, i);
                    break;
                case cr_StorageLocation_NONVOLATILE:
                    write_param_to_nvm(param_desc[i].id, &sCr_param_val[i]);
                    break;
                }
            }
            return 0;
        } // end if PID matches.
    }
    // exited the for loop without finding a match.
    return cr_ErrorCodes_INVALID_PARAMETER;
}

// return a number that changes if the parameter descriptions have changed.
uint32_t crcb_compute_parameter_hash(void)
{
    // Note that the layout of the structure param_desc differs by compiler.
    // The hash computed on windows won't match that computed on SiLabs.
    uint32_t *ptr = (uint32_t*)param_desc;
    // char *cptr = (char*)param_desc;
    size_t sz = sizeof(param_desc)/(sizeof(uint32_t));
    // LOG_DUMP_MASK(LOG_MASK_PARAMS, "Raw Params", cptr, sizeof(param_desc));

    uint32_t hash = ptr[0];
    for (size_t i= 1; i<sz; i++)
        hash ^= ptr[i];

    ptr = (uint32_t*)param_ex_desc;
    size_t sz1 = sizeof(param_ex_desc)/(sizeof(uint32_t));

    for (size_t i= 0; i<sz1; i++)
        hash ^= ptr[i];

    I3_LOG(LOG_MASK_PARAMS, "%s: hash 0x%x over %d+%d = %d words.\n", 
           __FUNCTION__, hash, sz, sz1, sz+sz1);
    return hash;
}


// overriding the weak implemetation, this reports on our local repo.
// Gets a pointer to this parameter description.
static int parameter_get_description(const uint32_t pid, cr_ParameterInfo **ppParam)
{
    affirm(ppParam != NULL);

    for (int i=0; i<NUM_PARAMS; i++) {
        if (param_desc[i].id == pid) {
            *ppParam = &param_desc[i];
            return 0;
        }
    }
    return cr_ErrorCodes_INVALID_PARAMETER;
}

static int sCurrentParameter = 0;


// Gets the parameter description for the next parameter.
// Allows the stack to iterate through the parameter list.
// Implies an order in the parameter list that is known by the application,
// but not directly by the stack.  Parameter ID's need not be 
// continuous or in order.
// The caller provides a cr_ParameterInfo containing string pointers that will be overwritten.
// The app owns the string pointers which must not be on the stack.
int crcb_parameter_discover_next(cr_ParameterInfo **ppDesc)
{
    if (sCurrentParameter >= NUM_PARAMS)
    {
        I3_LOG(LOG_MASK_PARAMS, "%s: sCurrentParameter (%d) >= NUM_PARAMS (%d)", 
               __FUNCTION__, sCurrentParameter, NUM_PARAMS);
        return cr_ErrorCodes_INVALID_PARAMETER;
    }
    int rval = parameter_get_description(param_desc[sCurrentParameter].id, ppDesc);
    sCurrentParameter++;
    return rval;
}


int crcb_parameter_get_count()
{
    return NUM_PARAMS;
}
// Resets the application's pointer into the parameter table such that
// the next call to crcb_parameter_discover_next() will return the
// description of this parameter.
int crcb_parameter_discover_reset(const uint32_t pid)
{
    int i;
    sCurrentParameter = 0;  // in case none match
    for (i = 0; i < NUM_PARAMS; i++)
    {
        if (param_desc[i].id == pid) {
            sCurrentParameter = i;
            I3_LOG(LOG_MASK_PARAMS, "dp reset(%d) reset to %d", pid, sCurrentParameter);
            return 0;
        }
    }
    I3_LOG(LOG_MASK_PARAMS, "dp reset(%d) reset defaults to %d", pid, sCurrentParameter);
    return cr_ErrorCodes_INVALID_PARAMETER;
}

// In parallel to the parameter discovery, use this to find out 
// about enumerations and bitfields
static int sCurrentExParam = 0;
static int sRequestedParamPid = -1; // negative means all

int crcb_parameter_ex_get_count(const int32_t pid)
{
    if (pid < 0)  // all 
        return NUM_EX_PARAMS;

    int num_ex_msgs = 0;

    for (int i=0; i<NUM_EX_PARAMS; i++) {
        if ((int32_t)param_ex_desc[i].associated_pid == pid) {
            num_ex_msgs++;
        }
    }
    return num_ex_msgs;
}

int crcb_parameter_ex_discover_reset(const int32_t pid)
{
    // unlike the full params, reset of param_ex always goes to zero.
    sCurrentExParam = 0;
    sRequestedParamPid = pid;
    return 0;
}

int crcb_parameter_ex_discover_next(cr_ParamExInfoResponse *pDesc)
{
    affirm(pDesc);
    pDesc->enumerations_count = 0;
    if (sCurrentExParam>=NUM_EX_PARAMS)
    {
        I3_LOG(LOG_MASK_PARAMS, "%s: No more ex params.", __FUNCTION__);
        return cr_ErrorCodes_INVALID_PARAMETER;
    }

    if (sRequestedParamPid < 0)
    {
        I3_LOG(LOG_MASK_PARAMS, "%s: For all, return param_ex %d.", __FUNCTION__, sCurrentExParam);
        *pDesc = param_ex_desc[sCurrentExParam];
        sCurrentExParam++;
        return 0;
    }

    for (int i=sCurrentExParam; i<NUM_EX_PARAMS; i++)
    {
        if ((int32_t)param_ex_desc[i].associated_pid == sRequestedParamPid)
        {
            I3_LOG(LOG_MASK_PARAMS, "%s: For pid %d, return param_ex %d.", 
                   __FUNCTION__, sRequestedParamPid, sCurrentExParam);
            *pDesc = param_ex_desc[i];
            sCurrentExParam = i+1;;
            return 0;
        }
    }
    // should not get here.
    I3_LOG(LOG_MASK_PARAMS, "%s: No more ex params 2.", __FUNCTION__);
    return cr_ErrorCodes_INVALID_PARAMETER;
}

static uint32_t sLastChanged = 0;
void generate_data_for_notify(uint32_t timestamp)
{
    uint32_t delta = timestamp - sLastChanged;
    if (delta < 1000)
        return;   // this parameter changes once per second.
    sCr_param_val[11].value.sint32_value++;
    sCr_param_val[11].timestamp = timestamp;
    sLastChanged = timestamp;

}

static int read_param_from_nvm(const uint32_t pid, cr_ParameterValue *param)
{
    int key = -1;
    size_t dataLen;
    uint32_t objectType;

    for (int i=0; i<NUM_PARAMS; i++) {
        if (sCr_param_val[i].parameter_id == pid) 
        {
            key = pid;
            break;
        }
    }
    if (key <0) {
        i3_log(LOG_MASK_ERROR, "%s: PID %d not found.", __FUNCTION__, pid);
        return cr_ErrorCodes_INVALID_PARAMETER;
    }

    nvm3_getObjectInfo(nvm3_defaultHandle, key, &objectType, &dataLen);
    if (objectType != NVM3_OBJECTTYPE_DATA) {
        i3_log(LOG_MASK_ERROR, "%s: NVM object type of PID %d failed.", 
               __FUNCTION__, pid);
        return cr_ErrorCodes_READ_FAILED;
    }

    Ecode_t eCode = nvm3_readData(nvm3_defaultHandle, key, (uint8_t*)param, dataLen);
    if (ECODE_NVM3_OK != eCode) {
        i3_log(LOG_MASK_ERROR, "%s: NVM Read of PID %d failed with 0x%x.", 
               __FUNCTION__, pid, eCode);
        return cr_ErrorCodes_READ_FAILED;
    }
    return cr_ErrorCodes_NO_ERROR;
}

static int write_param_to_nvm(const uint32_t pid, const cr_ParameterValue *param)
{
    int key = -1;

    for (int i=0; i<NUM_PARAMS; i++) {
        if (sCr_param_val[i].parameter_id == pid) 
        {
            key = i;
            break;
        }
    }
    if (key <0) {
        i3_log(LOG_MASK_ERROR, "%s: PID %d not found.", __FUNCTION__, pid);
        return cr_ErrorCodes_INVALID_PARAMETER;
    }

    Ecode_t eCode = nvm3_writeData(nvm3_defaultHandle, pid,
                                   (uint8_t*)param, sizeof(cr_ParameterValue));
    if (ECODE_NVM3_OK != eCode) {
        i3_log(LOG_MASK_ERROR, "%s: NVM Write of PID %d failed with 0x%x.", 
               __FUNCTION__, pid, eCode);
        return cr_ErrorCodes_READ_FAILED;
    }

    // Do repacking if needed
    if (nvm3_repackNeeded(nvm3_defaultHandle)) {
        i3_log(LOG_MASK_ALWAYS, "Repacking NVM");
        eCode = nvm3_repack(nvm3_defaultHandle);
        if (eCode != ECODE_NVM3_OK) {
            i3_log(LOG_MASK_ERROR, "%s: Error 0x%x repacking", __FUNCTION__, eCode);
        }
    }
    I3_LOG(LOG_MASK_REACH, "Wrote PID %d (index %d) to NVM", pid, key);
    return cr_ErrorCodes_NO_ERROR;
}



int crcb_ping_get_signal_strength(int8_t *rssi)
{
    sl_bt_connection_get_median_rssi(rsl_get_connection(), rssi);
    return 0;
}

/*
 * File Support
 */
#define NUM_FILES   2
static const cr_FileInfo sFiles[NUM_FILES] =
{
    {
        0,                      // int32_t file_id; /* ID */
        "log_file.csv",         // char file_name[24]; /* Name */
        cr_AccessLevel_READ,    // cr_AccessLevel access; /* Access Level (Read / Write) */
        4000,                   /* size in bytes */
        cr_StorageLocation_RAM  // cr_StorageLocation storage_location;
    },
    {
        1,                              // int32_t file_id; /* ID */
        "ota.bin",                      // char file_name[24]; /* Name */
        cr_AccessLevel_READ_WRITE,      // cr_AccessLevel access; /* Access Level (Read / Write) */
        50000,                          /* size in bytes */
        cr_StorageLocation_NONVOLATILE  // cr_StorageLocation storage_location;
    }
};

int crcb_file_get_description(uint32_t fid, cr_FileInfo *file_desc)
{
    if (fid == sFiles[0].file_id)
    {
        *file_desc = sFiles[0];
        return 0;
    }
    if (fid == sFiles[1].file_id)
    {
        *file_desc = sFiles[1];
        return 0;
    }
    return cr_ErrorCodes_BAD_FILE;
}

// Could override the requested ack rate using this.
// 0 does not override.
int crcb_file_get_preferred_ack_rate(int is_write)
{
    if (0 != i3_log_get_mask()) {
        i3_log(LOG_MASK_WARN, "Logging can interfere with file write.");
    }
    (void)is_write;
    return 0;
}

int crcb_file_get_file_count()
{
    return NUM_FILES;
}

static uint8_t sFid_index = 0;
int crcb_file_discover_reset(const uint8_t fid)
{
    if (fid == sFiles[0].file_id)
    {
        sFid_index = 0;
        return 0;
    }
    if (fid == sFiles[1].file_id)
    {
        sFid_index = 1;
        return 0;
    }
    i3_log(LOG_MASK_ERROR, "crcb_file_discover_reset(%d): invalid FID, using 0.", fid);
    sFid_index = 0;
    return 0;
}

int crcb_file_discover_next(cr_FileInfo *file_desc)
{
    if (sFid_index >= NUM_FILES)
    {
        i3_log(LOG_MASK_WARN, "%s: sFid_index (%d) >= NUM_FILES (%d)",
               __FUNCTION__, sFid_index, NUM_FILES);
        return cr_ErrorCodes_BAD_FILE;
    }
    *file_desc = sFiles[sFid_index];
    sFid_index++;
    return 0;
}

// Attempts to read the specified file.
// returns zero or an error code
int crcb_read_file(const uint32_t fid,            // which file
                 const int offset,              // offset, negative value specifies current location.
                 const size_t bytes_requested,  // how many bytes to read
                 uint8_t *pData,                // where the data goes
                 int *bytes_read)               // bytes actually read, negative for errors.
{
    if (fid > 1)
    {
        i3_log(LOG_MASK_ERROR, "%s: File ID %d does not exist.", __FUNCTION__, fid);
        return cr_ErrorCodes_BAD_FILE;
    }
    if (bytes_requested > REACH_BYTES_IN_A_FILE_PACKET)
    {
        i3_log(LOG_MASK_ERROR, "%s: %d is more than the buffer for a file read (%d).",
               __FUNCTION__, fid, REACH_BYTES_IN_A_FILE_PACKET);
        return cr_ErrorCodes_BUFFER_TOO_SMALL;
    }
    affirm(REACH_BYTES_IN_A_FILE_PACKET < 256);  // assumed

    uint32_t *ptr = (uint32_t *)pData;
    if (bytes_requested<4)
    {
        for (size_t i = 0; i<bytes_requested; i++)
            pData[i] = i;
        *bytes_read = bytes_requested;
    }
    else
    {
        for (size_t i = 0; i<(bytes_requested >> 2); i++)
            ptr[i] = offset+i;
        *bytes_read = bytes_requested & 0xFC;
    }
    return 0;
}

// returns zero or an error code
int crcb_write_file(const uint32_t fid,   // which file
                 const int offset,      // offset, negative value specifies current location.
                 const size_t bytes,    // how many bytes to write
                 const uint8_t *pData)   // where to get the data from
{
    (void)offset;
    (void)bytes;
    (void)pData;

    if (fid > 1)
    {
        i3_log(LOG_MASK_ERROR, "%s: File ID %d does not exist.",
               __FUNCTION__, fid);
        return cr_ErrorCodes_BAD_FILE;
    }
    if (fid != 1)
    {
        i3_log(LOG_MASK_ERROR, "%s: File ID %d has no write permission.",
               __FUNCTION__, fid);
        return cr_ErrorCodes_PERMISSION_DENIED;
    }
    // LOG_DUMP_MASK(LOG_MASK_FILES, "Received File Data", pData, bytes);
    return 0;
}

// returns zero or an error code
int crcb_erase_file(const uint32_t fid)
{
    if (fid > 1)
    {
        i3_log(LOG_MASK_ERROR, "%s: File ID %d does not exist.",
               __FUNCTION__, fid);
        return cr_ErrorCodes_BAD_FILE;
    }
    if (fid != 1)
    {
        i3_log(LOG_MASK_ERROR, "%s: File ID %d has no write permission.",
               __FUNCTION__, fid);
        return cr_ErrorCodes_PERMISSION_DENIED;
    }
    I3_LOG(LOG_MASK_FILES, "Fake file erase of file %d.", fid);
    return 0;
}

//*************************************************************************
//  Command Service
//*************************************************************************

#define NUM_COMMANDS    7

uint8_t sCommandIndex = 0;
const cr_CommandInfo sCommands[NUM_COMMANDS] = 
{
    {1,  "Enable remote CLI"},
    {2,  "Disable remote CLI"},
    {3,  "Minimize Logging (lm 0)"},
    {4,  "Much Logging (lm 7C0)"},
    {47, "Trigger OTA"},
    {99, "Board Reset"},
    {100,  "Factory Reset"}
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
            case 1:
                i3_log_set_remote_cli_enable(true);
                break;
            case 2:
                i3_log_set_remote_cli_enable(false);
                break;
            case 3:
                i3_log_set_mask(0);
                break;
            case 4:
                i3_log_set_mask(0x7C0);
                break;
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

// This is intended to live in flash
const cr_DeviceInfoResponse test1_di = 
{
    .protocol_version       = cr_ReachProtoVersion_CURRENT_VERSION,
    .device_name            = "Jack Reacher",
    .manufacturer           = "i3 Product Development",
    .device_description     = "This is a test of Jack Reacher's System.",
    .firmware_version       = "-1.-1.-1", // to be overwritten
    .max_message_size       = 244,
  #ifdef ENABLE_REMOTE_CLI
    .services = cr_ServiceIds_PARAMETER_REPO + cr_ServiceIds_FILES + cr_ServiceIds_COMMANDS + cr_ServiceIds_CLI,
  #else
    .services = cr_ServiceIds_PARAMETER_REPO + cr_ServiceIds_FILES + cr_ServiceIds_COMMANDS,
  #endif
    .parameter_metadata_hash    = 22, // this will be overwritten
    .has_application_identifier = 0,  // false
    .application_identifier     = {0},
    .endpoints                  = 0,
    .parameter_buffer_count     = REACH_PARAM_BUFFER_COUNT,
    .num_medium_structs_in_msg  = REACH_NUM_MEDIUM_STRUCTS_IN_MESSAGE,
    .big_data_buffer_size       = REACH_BIG_DATA_BUFFER_LEN
};


// The stack will call this function.
// The const copy of the basis in flash is copied to RAM so that the device
// can overwrite varying data like SN and hash.
int crcb_device_get_info(cr_DeviceInfoResponse *pDi)
{
    // The app owns the memory here.
    // The address is returned so that the data can come from flash.
    // memcpy as the structure copy imposes a further address alignment requirement.
    // *pDi = test1_di;
    memcpy(pDi, &test1_di, sizeof(cr_DeviceInfoResponse));
    i3_log(LOG_MASK_ALWAYS, "%s: %s\n", __FUNCTION__, test1_di.device_name);

    sprintf(pDi->firmware_version, "%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);

    snprintf(pDi->device_name, REACH_DEVICE_NAME_LEN, "%s", rsl_get_advertised_name());
    return 0;
}

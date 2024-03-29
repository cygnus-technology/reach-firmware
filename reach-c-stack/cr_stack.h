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
 * \brief "cr_stack.h" defines the API to the Cygnus Reach device stack
 *
 * Original Author: Chuck.Peplinski
 *
 ********************************************************************************************/

/**
 * @file      cr_stack.h
 * @brief     This file exposes the core of the public Reach stack API. It 
 *            depends on reach-server.h which defines what parts of the stack
 *            are being used by an applicaiton.  crcb_weak.h is incluced. That
 *            is the part of the API that must be implemented by a server
 *            application.
 * @author    Chuck Peplinski
 * @date      2024-01-24
 * @copyright (c) Copyright 2023 i3 Product Development. All Rights Reserved.
 * The Cygngus Reach firmware stack is shared under an MIT license.
 */

#ifndef _CR_STACK_H
#define _CR_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// reach-server.h defines the build configuration of the Reach stack.
// This includes which services, and the basic size of structures appropriate for the 
// current transport means.  For example, BLE.
#include "reach-server.h"

#include "crcb_weak.h"
// reach.pb.h is generated by nanopb based on the protobuf file reach.proto.
#include "reach.pb.h"
#include "reach_ble_proto_sizes.h"

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Conventions:
//----------------------------------------------------------------------------
// Naming convention:  
//  cr_ prefix (for Cygnus Reach), then service name, then verb.
// crcb_ prefix for the "weak" callback functions required.
// 
// All API functions return zero on success or an identifiable error code.
//
// There are a small number of API functions that are to be called by
// the hosting application.  Most of the functions in the API are to be 
// provided by the application.  The reach stack provides "weak" declared
// versions so the application provide only what is required.
// The stack can be built to report errors if required weak functions are 
// not overridden.
//
// The configuration structures that must be passed in to the stack to 
// describe  the system are designed to be constant and stored in flash 
// if the application desires.  This allows for a minimum use of RAM.
//
// Lists of items such as parameters, command, files, etc are handled using
// an iterator pattern.  A function gets the next item and a reset function
// is available.
// In all cases, resetting to zero will select the lowest available object.
//

#define MAX_NUM_PARAM_ID                32767 // 16 bits

//----------------------------------------------------------------------------
// API
//----------------------------------------------------------------------------

// Error codes are defined in the proto file so that the client can see them.
    
/*
//----------------------------------------------------------------------------
// static buffers used and reused by the reach stack.
//----------------------------------------------------------------------------

// Terminology
//   A transaction is a series of messages.
//   A message has a header and a payload.
//   The prompt is a received payload.
//   The response is a generated payload.
 
*/

#define ALIGN_TO_WORD   __attribute__((aligned(4)))

//----------------------------------------------------------------------------
// To be called once before cr_process is called.
int cr_init();

// The application should set the name of the device to be advertised.
// If the length exceeds the maximum, the max length will be returned.
int cr_set_advertised_name(char *name, int length);

// to be used by the transport code to get the name.
const char *cr_get_advertised_name();

// The application must call cr_process() regularly.  
// ticks tells it approximately how many  milliseconds have passed since 
// the system started.  This allows it to perform timing related tasks.  
// Most  
int cr_process(uint32_t ticks);

// allows the app to store the coded prompt in the memory held by the stack.
int cr_store_coded_prompt(uint8_t *data, size_t len);

int cr_get_coded_response_buffer(uint8_t **pResponse, size_t *len);

// you can get more useful error reports if you provide ~128 bytes here.
void cr_provide_debug_buffer(char *buf, size_t len);

void cr_get_reply_buffer_and_size(char **ptr, size_t *sz);

// error handling is always present
void cr_report_error(int error_code, const char *fmt, ...);

void cr_set_comm_link_connected(bool connected);
bool cr_get_comm_link_connected(void);

uint32_t cr_get_current_ticks();

void cr_test_sizes();


/** The reach_sizes_t is used to communicate the sizes of device structures to
 *  clients.  These sizes can vary from one server to another and the client
 *   is required to respect these sizes.  Many of them are determined by the
 *   chosen communication link, such as BLE. It uses smaller member sizes that
 *   the uint32 favored by protobufs. Hence it communicates these sizes without
 *   blowing up the size of the
 *  Refer also to enum SizesOffsets reach.proto */
typedef struct {
    uint16_t  max_message_size;             /**< The largest message that can be communicated */
    uint16_t  big_data_buffer_size;         /**< The size of the buffer used for the longest strings. */
    /// The number of parameter buffers kept by the device.
    /// This determines the number of parameters that can be
    /// handled in a single message.
    uint8_t   parameter_buffer_count;
    /// The number of parameter values that 
    /// fit in one message.
    uint8_t   num_params_in_response;
    /// The length of the description field in the 
    /// device info structure
    uint8_t   device_description_len;
    /// The number of bytes in the largest parameter types
    /// eg, strings and byte array 
    uint8_t   max_param_bytes;
    /// Length of the text string for parameter info description. 
    uint8_t   param_info_description_len;
    /// The length of longer strings.
    uint8_t   medium_string_len;
    /// The number of bytes in short strings like the units label.
    uint8_t   short_string_len;
    /// padding : no longer used
    uint8_t   param_info_enum_count;
    /// number of descriptors (stream, file) that fit in one message. 
    uint8_t   num_descriptors_in_response;
    /// Number of parameter notifications supported
    uint8_t   num_param_notifications;
    /// number of commands that can be in one info packet
    uint8_t   num_commands_in_response;
    /// number of parameter descriptions that 
    ///  can be in one info packet.
    uint8_t   num_param_desc_in_response;
} reach_sizes_t;

// The size of this structure (REACH_SIZE_STRUCT_SIZE) must be defined in 
// reach_ble_proto_sizes.h, or the analogous file used to set structure sizes.
// #define REACH_SIZE_STRUCT_SIZE      16

const char *cr_get_reach_version();

#ifdef __cplusplus
}
#endif


#endif  // ndef _CR_STACK_H


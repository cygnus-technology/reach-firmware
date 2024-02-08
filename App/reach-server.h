/*
 * reach-server.h
 *
 *  This file specifies aspects of the Reach server that are controlled by the app.
 *
 */

/**
 * @file      reach-server.c
 * @brief     This file specifies aspects of the Reach server that are 
 *            controlled by the app.
 * @copyright (c) Copyright 2023 i3 Product Development. All Rights Reserved.
 */


#ifndef _REACH_SERVER_H_
#define _REACH_SERVER_H_

/// A range of parameters are driven by the 244 byte packet size imposed by 
/// BLE.See reach_ble_proto_sizes.h

/// As the app is using BLE, the largest encoded buffer cannot be larger than 
/// 244 bytes.  The Reach stack will statically allocate two buffers of this size, 
/// for encoding and decoding
#define CR_CODED_BUFFER_SIZE    244

/// The raw data that encodes to BLE might be slightly larger.
/// The Reach stack will allocate one buffer of this size, for decoding the prompt.
/// The app is to provide the raw memory to be encoded.
#define CR_DECODED_BUFFER_SIZE   256 

/// Number of ticks per second passed to cr_process()
#define SYS_TICK_RATE   1000

/// #define NO_REACH_LOGGING to completely exclude logging to save space.
/// #define NO_REACH_LOGGING

/// Determines the storage space for the advertised name.  See 
/// cr_set_advertised_name().  Allows an app to adjust this depending on its 
/// needs. 
#define APP_ADVERTISED_NAME_LENGTH  16


#define INCLUDE_PARAMETER_SERVICE
/// Define this to be a 32 bit number that must be provided to access the device.
/// #define APP_REQUIRED_CHALLENGE_KEY  0x1020304

/// Define this to be a 32 bit number that must be provided to access a longer
/// list of parameters.
/// #define APP_REQUIRED_PARAMETER_KEY  0x9080706

/// Setting this to zero removes support for unpolled parameter change notification
/// Defines the size of the array holding param notification specifications.
#define NUM_SUPPORTED_PARAM_NOTIFY  8

/// Define this to include support for the file service.
#define INCLUDE_FILE_SERVICE

/// Define this to support the remote CLI service.
#define INCLUDE_CLI_SERVICE
#ifdef INCLUDE_CLI_SERVICE
  /// If false use a command to enable it.
  #define REMOTE_CLI_ECHO_ON_DEFAULT    false
#endif

#define INCLUDE_COMMAND_SERVICE

#define INCLUDE_TIME_SERVICE

/// #define INCLUDE_STREAM_SERVICE

#include "reach.pb.h"

/// Ideally all of the buffer sizes flow from here.
#include "reach_ble_proto_sizes.h"

/// This is the number of buffers allocated.
/// It's coded here to be the same as the number allowed in a transfer.
/// With code changes it could be less.
#define REACH_PARAM_BUFFER_COUNT REACH_COUNT_PARAM_IDS

// To test the case in which there are no extended parameter descriptions:
// #define SKIP_ENUMS  // test

#endif  // ndef _REACH_SERVER_H_


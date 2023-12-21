/*
 * reach-server.h
 *
 *  This file specifies aspects of the Reach server that are controlled by the app.
 *
 */

#ifndef _REACH_SERVER_H_
#define _REACH_SERVER_H_

// A range of parameters are driven by the 244 byte packet size imposed by BLE.


// As the app is using BLE, the largest encoded buffer cannot be larger than 
// 244 bytes.  The Reach stack will statically allocate two buffers of this size, 
// for encoding and decoding
#define CR_CODED_BUFFER_SIZE    244

// The raw data that encodes to BLE might be slightly larger.
// The Reach stack will allocate one buffer of this size, for decoding the prompt.
// The app is to provide the raw memory to be encoded.
#define CR_DECODED_BUFFER_SIZE   256 

// Define this to enable remote CLI via a buffer.
#define ENABLE_REMOTE_CLI

#include "reach.pb.h"

// Ideally all of the buffer sizes flow from here.
#include "reach_ble_proto_sizes.h"

// This is the number of buffers allocated.
// It's coded here to be the same as the number allowed in a transfer.
// With code changes it could be less.
#define REACH_PARAM_BUFFER_COUNT REACH_COUNT_PARAM_IDS

#endif  // ndef _REACH_SERVER_H_


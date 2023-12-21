/********************************************************************************************
 *    _ ____  ___             _         _     ___              _                        _
 *   (_)__ / | _ \_ _ ___  __| |_  _ __| |_  |   \ _____ _____| |___ _ __ _ __  ___ _ _| |_
 *   | ||_ \ |  _/ '_/ _ \/ _` | || / _|  _| | |) / -_) V / -_) / _ \ '_ \ '  \/ -_) ' \  _|
 *   |_|___/ |_| |_| \___/\__,_|\_,_\__|\__| |___/\___|\_/\___|_\___/ .__/_|_|_\___|_||_\__|
 *                                                                  |_|
 *                           -----------------------------------
 *                          Copyright i3 Product Development 2023
 *
 * @author  i3 Product Development
 *          Wei Zhou wei.zhou@i3pd.com
 *
 * @version 0.0.1
 * @date    2023-09-26
 * @brief   This file provides support for the socket based notification
 *          server running on the other end.  It primarily supports
 *          asyncrhonous CLI feedback via crcb_cli_respond().
 *          It duplicates the memory and functions used with the normal
 *          reach server.  A BLE link would be more efficient with memory.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Include this header for close function

#include "message_util.h"
#include "reach_client.h"
#include "reach-server.h"
#include "cr_stack.h"
#include "i3_log.h"
#include "reach_silabs.h"  // for rsl_notify_client.  Improper dependency.


#include "pb_encode.h"


// These buffers are used to ensure no conflict with the sCr_ buffers of the reach stack.
// These may not be needed in a BLE system.
// The functions here similarly copy those in cr_stack.c as a quick way to avoid 
// conflict
#define UNCODED_PAYLOAD_SIZE  (CR_CODED_BUFFER_SIZE-4)
static uint8_t sRs_uncoded_response_buffer[UNCODED_PAYLOAD_SIZE];
static cr_ReachMessage sRs_uncoded_message_structure;
static uint8_t sRs_encoded_payload_buffer[UNCODED_PAYLOAD_SIZE]; 
static size_t sRs_encoded_payload_size; 
static uint8_t sRs_uncoded_response_buffer[UNCODED_PAYLOAD_SIZE];

static uint8_t sRs_encoded_response_buffer[CR_CODED_BUFFER_SIZE];
static size_t  sRs_encoded_response_size = 0;


static
bool encode_notification_payload(cr_ReachMessageTypes message_type,    // in
                          const void *data,                     // in:  data to be encoded
                          uint8_t *buffer,                      // out: Encode to here.
                          pb_size_t buffer_size,                // in:  max size of encoded data
                          size_t *encode_size)                  // out: encoded data size
{

  /* Create a stream that writes to the buffer. */
  pb_ostream_t os_stream = pb_ostream_from_buffer(buffer, buffer_size);
  bool status = false;

  /* Now we are ready to encode the message. */
  switch (message_type) 
  {
  case cr_ReachMessageTypes_ERROR_REPORT:
      status = pb_encode(&os_stream, cr_ErrorReport_fields, data);
      if (status) {
        *encode_size = os_stream.bytes_written;
        // cr_ErrorReport *er = (cr_ErrorReport *)data;
        // i3_log(LOG_MASK_REACH, "Error Report: %s", er->result_string);
      }
      break;
  case cr_ReachMessageTypes_DISCOVER_STREAMS:
    // if (request) {
    //   status = pb_encode(&os_stream, cr_StreamsRequest_fields, data);
    //   if (status) {
    //     *encode_size = os_stream.bytes_written;
    //     LOG_REACH(
    //         "Discover streams request: \n%s\n",
    //         message_util_discover_streams_json((cr_StreamsRequest *)data));
    //   }
    // } else {
    //   status = pb_encode(&os_stream, cr_StreamsResponse_fields, data);
    //   if (status) {
    //     *encode_size = os_stream.bytes_written;
    //     LOG_REACH("Discover streams response: \n%s\n",
    //               message_util_discover_streams_response_json(
    //                   (cr_StreamsResponse *)data));
    //   }
    // }
    break;
  case cr_ReachMessageTypes_CLI_NOTIFICATION:
      status = pb_encode(&os_stream, cr_CLIData_fields, data);
      if (status) {
        *encode_size = os_stream.bytes_written;
      }
      break;
  default:
      status = cr_ErrorCodes_NO_DATA;
      break;
  }

  if (status)
      return true;

  printf("%s: payload encoding failed: %s\n", __FUNCTION__, PB_GET_ERROR(&os_stream));
  return false;
}

static
bool encode_notification_message(const cr_ReachMessage *message,   // in:  message to be encoded
                          uint8_t *buffer,                  // out: Buffer to encode into
                          size_t buffer_size,               // in:  max size of encoded message
                          size_t *encode_size)              // out: actual size of encoded message.
{
  pb_ostream_t os_stream = pb_ostream_from_buffer(buffer, buffer_size);

  bool status = pb_encode(&os_stream, cr_ReachMessage_fields, (void *)message);
  if (status) {
    *encode_size = os_stream.bytes_written;
  } else {
    printf("%s: Message encoding failed: %s\n", __FUNCTION__, PB_GET_ERROR(&os_stream));
  }
  return status;
}

// encodes message to sRs_encoded_response_buffer.
// The caller must populate the header
static int 
rs_notification_message(cr_ReachMessageTypes message_type,    // in
                        const void *payload,                  // in:  to be encoded
                        cr_ReachMessageHeader *hdr)           // in
{
    // i3_log(LOG_MASK_REACH, "%s(): hdr: type %d, num_obj %d, remain %d, trans_id %d.", __FUNCTION__,
    //        hdr->message_type, hdr->number_of_objects, hdr->remaining_objects, hdr->transaction_id);

    if (!encode_notification_payload(message_type, payload,
                              sRs_encoded_payload_buffer,
                              sizeof(sRs_encoded_payload_buffer),
                              &sRs_encoded_payload_size))
    {
        cr_report_error(cr_ErrorCodes_ENCODING_FAILED, "encode payload %d failed.", message_type);
        return cr_ErrorCodes_ENCODING_FAILED;
    }
    // i3_log_dump_buffer(1, "payload encoded", sRs_encoded_payload_buffer, sRs_encoded_payload_size); 

    // build the message envelope
    sRs_uncoded_message_structure.header     = *hdr;
    sRs_uncoded_message_structure.has_header = true;
    memcpy(sRs_uncoded_message_structure.payload.bytes, 
           sRs_encoded_payload_buffer, 
           sRs_encoded_payload_size);
    sRs_uncoded_message_structure.payload.size = sRs_encoded_payload_size;  

    /*i3_log(LOG_MASK_REACH, "%s(): type %d, num_obj %d, remain %d, trans_id %d.", __FUNCTION__,
           sRs_uncoded_message_structure.header.message_type, 
           sRs_uncoded_message_structure.header.number_of_objects, 
           sRs_uncoded_message_structure.header.remaining_objects, 
           sRs_uncoded_message_structure.header.transaction_id);
     */
    // encode the wrapped message
    if (!encode_notification_message(&sRs_uncoded_message_structure,
                                     sRs_encoded_response_buffer,
                                     sizeof(sRs_encoded_response_buffer),
                                     &sRs_encoded_response_size))
    {
        printf("%s: encode message %d failed.", __FUNCTION__, message_type);
        return cr_ErrorCodes_ENCODING_FAILED;
    }
    return 0;
}


// When the device supports a CLI it is expected to share anything printed 
// to the CLI back to the stack for remote display using crcb_cli_respond()
int crcb_cli_respond(char *cli)
{
    i3_log(LOG_MASK_WIRE, TEXT_GREEN "%s for CLI" TEXT_RESET, __FUNCTION__);
    cr_CLIData *cld = (cr_CLIData*)sRs_uncoded_response_buffer;
    strncpy(cld->message_data, cli, REACH_ERROR_BUFFER_LEN);
    cld->is_complete = true;

    cr_ReachMessageHeader hdr;
    hdr.message_type      = cr_ReachMessageTypes_CLI_NOTIFICATION;
    hdr.number_of_objects = 0;
    hdr.remaining_objects = 0;
    hdr.transaction_id    = 0;  // The transaction ID should always be 0 as the app 
                               // will never request a CLI notification

    rs_notification_message(cr_ReachMessageTypes_CLI_NOTIFICATION,
                             sRs_uncoded_response_buffer,  // in:  to be encoded
                             &hdr);

    i3_log_dump_buffer(LOG_MASK_WIRE, "CLI", sRs_encoded_response_buffer, sRs_encoded_response_size);
    rsl_notify_client(sRs_encoded_response_buffer, sRs_encoded_response_size);

    return 0;
}


// When the device supports a CLI it is expected to share anything printed 
// to the CLI back to the stack for remote display using crcb_cli_respond()
int crcb_notify_error(cr_ErrorReport *err)
{
    cr_ErrorReport *errRep = (cr_ErrorReport*)sRs_uncoded_response_buffer;
    memcpy(errRep, err, sizeof(cr_ErrorReport));

    // i3_log(LOG_MASK_WIRE, TEXT_RED "%s for error" TEXT_RESET, __FUNCTION__);
    cr_ReachMessageHeader hdr;
    hdr.message_type      = cr_ReachMessageTypes_ERROR_REPORT;
    hdr.number_of_objects = 0;
    hdr.remaining_objects = 0;
    hdr.transaction_id    = 0;
    rs_notification_message(cr_ReachMessageTypes_ERROR_REPORT,
                             sRs_uncoded_response_buffer,  // in: to be encoded
                             &hdr);

    i3_log_dump_buffer(LOG_MASK_WIRE, "error report", sRs_encoded_response_buffer, sRs_encoded_response_size);
    rsl_notify_client(sRs_encoded_response_buffer, sRs_encoded_response_size);

    return 0;
}



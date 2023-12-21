/********************************************************************************************
 *    _ ____  ___             _         _     ___              _                        _
 *   (_)__ / | _ \_ _ ___  __| |_  _ __| |_  |   \ _____ _____| |___ _ __ _ __  ___ _ _| |_
 *   | ||_ \ |  _/ '_/ _ \/ _` | || / _|  _| | |) / -_) V / -_) / _ \ '_ \ '  \/ -_) ' \  _|
 *   |_|___/ |_| |_| \___/\__,_|\_,_\__|\__| |___/\___|\_/\___|_\___/ .__/_|_|_\___|_||_\__|
 *                                                                  |_|
 *                           -----------------------------------
 *                        (c) Copyright 2023, i3 Product Development
 *
 * \brief reach_silabs.h/.c provides a SiLabs specific implementation of the required
 *      Reach functionality.
 *
 * Original Author: Chuck.Peplinski
 *
 ********************************************************************************************/

#ifndef _REACH_H_
#define _REACH_H_

#include <stddef.h>

#include "gatt_db.h"
#include "sl_bluetooth.h"
#include "sl_cli_command.h"

#define REACH_MAX_PROMPT_SIZE 256

// should come from GATT DB.
#define REACH_BLE_CHARICTERISTIC_ID   gattdb_REACH

void rsl_init();
int rsl_stats();

void rsl_inform_connection(uint8_t connection, uint16_t characteristic);
void rsl_inform_subscribed(bool subscribed);

int rsl_notify_client(uint8_t *data, size_t len);

/**************************************************************************//**
 * Reach version of the SiLabs Application Init.
 *****************************************************************************/
void rsl_app_init(void);

/**************************************************************************//**
 * Reach version of the SiLabs Application Process Action. 
 *****************************************************************************/
void rsl_app_process_action(void);

/**************************************************************************//**
 * Reach version of the SiLabs event handler.
 *****************************************************************************/
void rsl_bt_on_event(sl_bt_msg_t *evt);

const char *rsl_get_advertised_name();

/**************************************************************************//**
 * Command line handlers:
 *****************************************************************************/
extern void print_versions(void);
extern void slash();
extern void cli_lm(sl_cli_command_arg_t *args);
extern void cli_rcli(sl_cli_command_arg_t *args);
extern void cli_phy(sl_cli_command_arg_t *args);
extern void cli_nvm(sl_cli_command_arg_t *args);


#endif // _REACH_H_

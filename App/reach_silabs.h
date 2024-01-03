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
#define REACH_SN_KEY    0x10000

// should come from GATT DB.
#define REACH_BLE_CHARICTERISTIC_ID   gattdb_REACH

// Cyan is OK on both white and black backgrounds
#define TEXT_CLI   TEXT_CYAN

void rsl_init();
int rsl_stats();

// stores the connection info so others can use it.
void rsl_inform_connection(uint8_t connection, uint16_t characteristic);
uint8_t rsl_get_connection(void);
void rsl_inform_subscribed(bool subscribed);
int rsl_get_rssi(void);

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

int rsl_read_serial_number(unsigned int *sn);
int rsl_write_serial_number(unsigned int sn);

/**************************************************************************//**
 * Command line handlers:
 *****************************************************************************/
extern void print_versions(void);
extern void slash();
extern void cli_lm(sl_cli_command_arg_t *args);
extern void cli_rcli(sl_cli_command_arg_t *args);
extern void cli_phy(sl_cli_command_arg_t *args);
extern void cli_nvm(sl_cli_command_arg_t *args);
extern void cli_sn(sl_cli_command_arg_t *args);


#endif // _REACH_H_

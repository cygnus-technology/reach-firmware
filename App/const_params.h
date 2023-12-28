/********************************************************************************************
 *    _ ____  ___             _         _     ___              _                        _
 *   (_)__ / | _ \_ _ ___  __| |_  _ __| |_  |   \ _____ _____| |___ _ __ _ __  ___ _ _| |_
 *   | ||_ \ |  _/ '_/ _ \/ _` | || / _|  _| | |) / -_) V / -_) / _ \ '_ \ '  \/ -_) ' \  _|
 *   |_|___/ |_| |_| \___/\__,_|\_,_\__|\__| |___/\___|\_/\___|_\___/ .__/_|_|_\___|_||_\__|
 *                                                                  |_|
 *                           -----------------------------------
 *                          Copyright i3 Product Development 2023
 *
 * \brief "const_params.h" description of trivial 'fake' parameter repository.
 *
 * Original Author: Chuck.Peplinski
 *
 ********************************************************************************************/

#ifndef _CONST_PARAMS_H
#define _CONST_PARAMS_H

// There are 35 params defined in the .c file.
// One is write only so it is not transmitted.
// Set this to 34 to go over the 32 param request size. 
#define NUM_PARAMS      33  
#define NUM_EX_PARAMS   4

extern const cr_ParameterInfo  param_desc[NUM_PARAMS];
extern const cr_ParamExInfoResponse param_ex_desc[NUM_EX_PARAMS];

#endif  // _CONST_PARAMS_H

 
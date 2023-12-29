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
 * \brief "const_params.c" description of trivial 'fake' parameter repository.
 *
 * Original Author: Chuck.Peplinski
 *
 ********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cr_stack.h"
#include "i3_log.h"
#include "const_params.h"


// This is effectively a 'fake' parameter repository.
// The init function must be called to connect it to the system.
// This particular set of parameters is chosen to test functionality.

// Notice that parameter ID's are odd numbers.  This was done
// to prove that they can be discontinuous.

const cr_ParameterInfo  param_desc[NUM_PARAMS] = {
    // A uint32 type with a description and a limited range.
    { // [0]
        .id =               1,
        .data_type =        cr_ParameterDataType_UINT32,
        .size_in_bytes  =   0,
        .name =             "first param (1)",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "This parameter comes first",
        .units =            "unsigned int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        32767,
        .default_value =    1970,
        .storage_location = cr_StorageLocation_RAM


    },
    // A signed int with a description and a limited range.
    // units test a UTF-8 symbol
    { // [1]
        .id =               3,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "param #two",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Eleven bits signed",
        .units =            "\xC2\xB0",  // degres in UTF-8
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM_EXTENDED
    },
    // A float with a limited range and a description.
    // Stored in non-volatile
    { // [2]
        .id =               5,
        .data_type =        cr_ParameterDataType_FLOAT32,
        .size_in_bytes  =   0,
        .name =             "NV p5 %",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "float32",
        .units =            "%",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        100,
        .default_value =    66.66666666666667,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // A uint64 with a large linmited range.
    // By ifdef can test the "too long string" error.
    { // [3]
        .id =               7,
        .data_type =        cr_ParameterDataType_UINT64,
        .size_in_bytes  =   0,
        .name =             "0 to 68719476736",
        .access =           cr_AccessLevel_READ_WRITE,
       // #define TOO_LONG_STRING
       #ifdef TOO_LONG_STRING
        // exercises an error case that we like to know works!
        .description =      "This parameter doesn't come first",
      #else
        .description =      "Parmenter no come foist!",
      #endif  // def TOO_LONG_STRING
        .units =            "unsigned long",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        68719476736,
        .default_value =    68719476736,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // An int64 with a limited range
    { // [4]
        .id =               9,
        .data_type =        cr_ParameterDataType_INT64,
        .size_in_bytes  =   0,
        .name =             "+/- 68719476736",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "large int",
        .units =            "signed long",
        .has_description =  true,
        .has_range_min =    true,  // demo no max/min
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -68719476736,
        .range_max =        68719476735,
        .default_value =    -68719476736,
        .storage_location = cr_StorageLocation_RAM 
    },
    // double with limited range
    { // [5]
        .id =               11,
        .data_type =        cr_ParameterDataType_FLOAT64,
        .size_in_bytes  =   0,
        .name =             "double 0-100",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "float64",
        .units =            "accurate",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        100,
        .default_value =    66.6666666666666666667,
        .storage_location = cr_StorageLocation_RAM 
    },
    // bool controls LED
    { // [6]
        .id =               13,
        .data_type =        cr_ParameterDataType_BOOL,
        .size_in_bytes  =   0,
        .name =             "LED switch",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "On or off",
        .units =            "truth",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        1,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM 
    },
    // Test editing strings, stored in NVM.
    { // [7]
        .id =               15,
        .data_type =        cr_ParameterDataType_STRING,
        .size_in_bytes  =   0,
        .name =             "String in NVM",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "string type",
        .units =            "words",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = false,
        .range_min =        1,
        .range_max =        0,
        .default_value =    0,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // Test enumeration, stored in NVM
    { // [8]
        .id =               17,
        .data_type =        cr_ParameterDataType_ENUMERATION,
        .size_in_bytes  =   0,
        .name =             "enum in NVM",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "enum",
        .units =            "nums",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = true,
        .range_min =        1,
        .range_max =        0,
        .default_value =    3,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // Test bit field, stored in NVM
    { // [9]
        .id =               19,
        .data_type =        cr_ParameterDataType_BIT_FIELD,
        .size_in_bytes  =   0,
        .name =             "bitfield in NVM",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Turn me on deadman",
        .units =            "bits",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = true,
        .range_min =        1,
        .range_max =        0,
        .default_value =    5,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // Test byte array                      
    { // [10]               
        .id =               21,
        .data_type =        cr_ParameterDataType_BYTE_ARRAY,
        .size_in_bytes  =   0,
        .name =             "bytes in NVM",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "array of bytes",
        .units =            "data",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = false,
        .range_min =        1,
        .range_max =        0,
        .default_value =    0,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // Read only with no default value.
    { // [11]
        .id =               23,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "read only",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM-EX",
        .units =            "+/- 1024",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM_EXTENDED
    },
    // write only
    { // [12]
        .id =               25,
        .data_type =        cr_ParameterDataType_FLOAT32,
        .size_in_bytes  =   0,
        .name =             "write only",
        .access =           cr_AccessLevel_WRITE,
        .description =      "write only",
        .units =            "percentage",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        100,
        .default_value =    66.66666666666667,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    // filler, NVM
    { // [13]
        .id =               27,
        .data_type =        cr_ParameterDataType_UINT64,
        .size_in_bytes  =   0,
        .name =             "uint64 NVM",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "This parameter is 13th",
        .units =            "37 bits",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        68719476736,
        .default_value =    68719476736,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    { // [14]
        .id =               29,
        .data_type =        cr_ParameterDataType_INT64,
        .size_in_bytes  =   0,
        .name =             "param #29",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "38 bits signed",
        .units =            "38 bits signed",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -68719476736,
        .range_max =        68719476735,
        .default_value =    -68719476736,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [15]
        .id =               31,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "param #31",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM-EX",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM_EXTENDED
    },
    // specifies max 1023 but no min.  Should be -32 bits
    { // [16]
        .id =               33,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "33 no min",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "no min",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    // specifies min -1024 but no max.  Should be 32 bits
    { // [17]
        .id =               35,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "35 no max",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "No max",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    false,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [18]
        .id =               37,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p37",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "+/- 1024",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [19]
        .id =               39,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p39 no desc",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  false,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [20]
        .id =               41,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p41",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [21]
        .id =               43,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p43",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    // no max no min.  Should be +/- 32 bits.
    { // [22]
        .id =               45,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "no max no min",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "??",
        .units =            "signed int",
        .has_description =  false,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [23]
        .id =               47,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p47 no default",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [24]
        .id =               49,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p49 no default",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM-EX",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = false,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    1,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [25]
        .id =               51,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p51",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    51,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [26]
        .id =               53,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p53",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    53,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [27]
        .id =               55,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p55",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    55,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [28]
        .id =               57,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p57",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    57,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [29]
        .id =               59,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p59",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    59,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [30]
        .id =               61,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p61",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Read write,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    61,
        .storage_location = cr_StorageLocation_RAM
    },
    { // [31]
        .id =               63,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p63",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Read write,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    63,
        .storage_location = cr_StorageLocation_RAM
    },
    // If there were no write only params this would overflow one read request.
    { // [32]
        .id =               65,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p65",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Read write,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    65,
        .storage_location = cr_StorageLocation_RAM
    },
    // When a write only param is not displayed this is still in one read request
    { // [33]
        .id =               67,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p67",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    67,
        .storage_location = cr_StorageLocation_RAM
    },
    // Even with one write only this overflows one read request.
    { // [34]
        .id =               69,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "p69",
        .access =           cr_AccessLevel_READ,
        .description =      "Read only,RAM",
        .units =            "signed int",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -1024,
        .range_max =        1023,
        .default_value =    69,
        .storage_location = cr_StorageLocation_RAM
    },
};


const cr_ParamExInfoResponse param_ex_desc[NUM_EX_PARAMS] = {
    {
        17, // associated_pid
        cr_ParameterDataType_ENUMERATION, // data_type
        8,
        {
            {1, "one"},
            {2, "two"},
            {3, "three"},
            {4, "four"},
            {5, "five"},
            {6, "six"},
            {7, "seven"},
            {8, "eight"},

        }
    },
    {
        17, // associated_pid
        cr_ParameterDataType_ENUMERATION, // data_type
        6,
        {
            {9, "nine"},
            {10, "ten"},
            {11, "eleven"},
            {12, "twelve"},
            {13, "thirteen"},
            {14, "fourteen"}
        }
    },
    {
        19, // associated_pid
        cr_ParameterDataType_BIT_FIELD, // data_type
        8,
        {
            {1, "one"},
            {2, "two"},
            {4, "four"},
            {8, "eight"},
            {16, "sixteen"},
            {32, "thirty two"},
            {64, "sixty four"},
            {128, "onetwentyeight"},

        }
    },
    {
        19, // associated_pid
        cr_ParameterDataType_BIT_FIELD, // data_type
        6,
        {
            {256, "1<<8"},
            {512, "1<<9"},
            {1024, "1<<10"},
            {2048, "1<<11"},
            {4096, "1<12"},
            {8182, "1<13"}
        }
    }
};



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
// The init function makes it valid.
#define NUM_PARAMS      15
#define NUM_EX_PARAMS   4

const cr_ParameterInfo  param_desc[NUM_PARAMS] = {
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
    { // [2]
        .id =               5,
        .data_type =        cr_ParameterDataType_FLOAT32,
        .size_in_bytes  =   0,
        .name =             "fifth param",
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
    { // [3]
        .id =               7,
        .data_type =        cr_ParameterDataType_UINT64,
        .size_in_bytes  =   0,
        .name =             "first uint64",
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
    { // [4]
        .id =               9,
        .data_type =        cr_ParameterDataType_INT64,
        .size_in_bytes  =   0,
        .name =             "param #nine",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "38 bits signed",
        .units =            "signed long",
        .has_description =  true,
        .has_range_min =    false,  // demo no max/min
        .has_range_max =    false,
        .has_default_value = true,
        .range_min =        -68719476736,
        .range_max =        68719476735,
        .default_value =    -68719476736,
        .storage_location = cr_StorageLocation_RAM 
    },
    { // [5]
        .id =               11,
        .data_type =        cr_ParameterDataType_FLOAT64,
        .size_in_bytes  =   0,
        .name =             "eleventh param",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "float64",
        .units =            "bigs",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        0,
        .range_max =        100,
        .default_value =    66.6666666666666666667,
        .storage_location = cr_StorageLocation_RAM 
    },
    { // [6]
        .id =               13,
        .data_type =        cr_ParameterDataType_BOOL,
        .size_in_bytes  =   0,
        .name =             "first bool",
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
    { // [7]
        .id =               15,
        .data_type =        cr_ParameterDataType_STRING,
        .size_in_bytes  =   0,
        .name =             "param #nine",
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
    { // [8]
        .id =               17,
        .data_type =        cr_ParameterDataType_ENUMERATION,
        .size_in_bytes  =   0,
        .name =             "eleventh param",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "float64",
        .units =            "bigs",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = false,
        .range_min =        1,
        .range_max =        0,
        .default_value =    0,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },
    { // [9]
        .id =               19,
        .data_type =        cr_ParameterDataType_BIT_FIELD,
        .size_in_bytes  =   0,
        .name =             "enable bits",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "Turn me on deadman",
        .units =            "bits",
        .has_description =  true,
        .has_range_min =    false,
        .has_range_max =    false,
        .has_default_value = true,
        .range_min =        1,
        .range_max =        0,
        .default_value =    0,
        .storage_location = cr_StorageLocation_NONVOLATILE
    },                      
    { // [10]               
        .id =               21,
        .data_type =        cr_ParameterDataType_BYTE_ARRAY,
        .size_in_bytes  =   0,
        .name =             "param 21",
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
    { // [11]
        .id =               23,
        .data_type =        cr_ParameterDataType_INT32,
        .size_in_bytes  =   0,
        .name =             "param #23",
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
        .storage_location = cr_StorageLocation_RAM_EXTENDED
    },
    { // [12]
        .id =               25,
        .data_type =        cr_ParameterDataType_FLOAT32,
        .size_in_bytes  =   0,
        .name =             "25th param",
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
    { // [13]
        .id =               27,
        .data_type =        cr_ParameterDataType_UINT64,
        .size_in_bytes  =   0,
        .name =             "p27 uint64",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "This parameter is 13th",
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
    { // [14]
        .id =               29,
        .data_type =        cr_ParameterDataType_INT64,
        .size_in_bytes  =   0,
        .name =             "param #29",
        .access =           cr_AccessLevel_READ_WRITE,
        .description =      "38 bits signed",
        .units =            "signed long",
        .has_description =  true,
        .has_range_min =    true,
        .has_range_max =    true,
        .has_default_value = true,
        .range_min =        -68719476736,
        .range_max =        68719476735,
        .default_value =    -68719476736,
        .storage_location = cr_StorageLocation_RAM
    }
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



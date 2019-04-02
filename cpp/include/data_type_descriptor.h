/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK Basic Components RedhawkDevUtils.
 *
 * REDHAWK Basic Components RedhawkDevUtils is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK Basic Components RedhawkDevUtils is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.

 * This is the component code. This file contains the child class where
 * custom functionality can be added to the component. Custom
 * functionality to the base class can be extended here. Access to
 * the ports can also be done from this class
 */
#ifndef FR_DATA_TYPE_H
#define FR_DATA_TYPE_H

#include <vector>
#include <queue>
#include <complex>
#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <typeinfo>
#include <string>
#include <map>


namespace SUPPORTED_DATA_TYPE {

    ///////////////////////////////////
    //    DATA FORMAT IDENTIFIERS    //
    ///////////////////////////////////

    // Scalar
    const std::string UNKNOWN("UNKNOWN");
    const std::string XML("XML");
    const std::string ALAW("ALAW");
    const std::string MULAW("MULAW");

    // Byte Order N/A
    const std::string CHAR("CHAR");
    const std::string OCTET("OCTET");

    // Little Endian
    const std::string SHORT_LITTLE_ENDIAN("SHORT_LITTLE_ENDIAN");
    const std::string USHORT_LITTLE_ENDIAN("USHORT_LITTLE_ENDIAN");
    const std::string LONG_LITTLE_ENDIAN("LONG_LITTLE_ENDIAN");
    const std::string ULONG_LITTLE_ENDIAN("ULONG_LITTLE_ENDIAN");
    const std::string FLOAT_LITTLE_ENDIAN("FLOAT_LITTLE_ENDIAN");
    const std::string DOUBLE_LITTLE_ENDIAN("DOUBLE_LITTLE_ENDIAN");
    const std::string LONGLONG_LITTLE_ENDIAN("LONGLONG_LITTLE_ENDIAN");

    // Big Endian
    const std::string SHORT_BIG_ENDIAN("SHORT_BIG_ENDIAN");
    const std::string USHORT_BIG_ENDIAN("USHORT_BIG_ENDIAN");
    const std::string LONG_BIG_ENDIAN("LONG_BIG_ENDIAN");
    const std::string ULONG_BIG_ENDIAN("ULONG_BIG_ENDIAN");
    const std::string FLOAT_BIG_ENDIAN("FLOAT_BIG_ENDIAN");
    const std::string DOUBLE_BIG_ENDIAN("DOUBLE_BIG_ENDIAN");
    const std::string LONGLONG_BIG_ENDIAN("LONGLONG_BIG_ENDIAN");

    // Complex
    // Byte Order N/A
    const std::string COMPLEX_CHAR("COMPLEX_CHAR");
    const std::string COMPLEX_UCHAR("COMPLEX_UCHAR");
    const std::string COMPLEX_OCTET(COMPLEX_UCHAR);

    // Little Endian
    const std::string COMPLEX_SHORT_LITTLE_ENDIAN("COMPLEX_SHORT_LITTLE_ENDIAN");
    const std::string COMPLEX_USHORT_LITTLE_ENDIAN("COMPLEX_USHORT_LITTLE_ENDIAN");
    const std::string COMPLEX_LONG_LITTLE_ENDIAN("COMPLEX_LONG_LITTLE_ENDIAN");
    const std::string COMPLEX_ULONG_LITTLE_ENDIAN("COMPLEX_ULONG_LITTLE_ENDIAN");
    const std::string COMPLEX_FLOAT_LITTLE_ENDIAN("COMPLEX_FLOAT_LITTLE_ENDIAN");
    const std::string COMPLEX_DOUBLE_LITTLE_ENDIAN("COMPLEX_DOUBLE_LITTLE_ENDIAN");
    const std::string COMPLEX_LONGLONG_LITTLE_ENDIAN("COMPLEX_LONGLONG_LITTLE_ENDIAN");

    // Big Endian
    const std::string COMPLEX_SHORT_BIG_ENDIAN("COMPLEX_SHORT_BIG_ENDIAN");
    const std::string COMPLEX_USHORT_BIG_ENDIAN("COMPLEX_USHORT_BIG_ENDIAN");
    const std::string COMPLEX_LONG_BIG_ENDIAN("COMPLEX_LONG_BIG_ENDIAN");
    const std::string COMPLEX_ULONG_BIG_ENDIAN("COMPLEX_ULONG_BIG_ENDIAN");
    const std::string COMPLEX_FLOAT_BIG_ENDIAN("COMPLEX_FLOAT_BIG_ENDIAN");
    const std::string COMPLEX_DOUBLE_BIG_ENDIAN("COMPLEX_DOUBLE_BIG_ENDIAN");
    const std::string COMPLEX_LONGLONG_BIG_ENDIAN("COMPLEX_LONGLONG_BIG_ENDIAN");


    //////////////////////
    //    DATA TYPES    //
    //////////////////////

    enum data_type_enum {
        _8a_, //8-bit A-law companded (ITU G.711). 
        _8u_, //8-bit mu-law companded (ITU G.711). 
        _8o_, //8-bit Offset Binary (uchar). 
        _8t_, //8-bit Two's Complement (char). 
        _16o_, //16-bit Offset Binary (ushort). 
        _16t_, //16-bit Two's Complement (short). 
        _32o_, //32-bit Offset Binary (ulong). 
        _32t_, //32-bit Two's Complement (long). 
        _32f_, //32-bit IEEE754 floating point format (float). 
        _64t_, //64-bit Two's Complement (long long). 
        _64f_, //64-bit floating point format (double). 

    };


    //////////////////////
    //       MODE       //
    //////////////////////

    enum mode_enum {
        _scalar_ = 0, // scalar/real data
        _complex_ = 1, // complex data

    };


    //////////////////////
    //    ENDIANNESS    //
    //////////////////////

    enum endian_enum {
        _big_endian_,
        _little_endian_,
        _byte_swap_,
        _keep_endianess_
    };


    //////////////////////
    //     ENCODING     //
    //////////////////////

    enum encoding_enum {
        _alaw_,
        _ulaw_,
        _none_
    };

    struct data_description {

        data_description(std::string _id, data_type_enum _dt, mode_enum _mode, endian_enum _endian, encoding_enum _encoding, const std::type_info * _type) {
            id = _id;
            data_type = _dt;
            mode = _mode;
            endian = _endian;
            encoding = _encoding;
            type = _type;

            bytes_per_element = 1;
            if (data_type == _16o_ || data_type == _16t_)
                bytes_per_element = 2;
            else if (data_type == _32o_ || data_type == _32t_ || data_type == _32f_)
                bytes_per_element = 4;
            else if (data_type == _64t_ || data_type == _64f_)
                bytes_per_element = 8;
            
            bytes_per_sample = bytes_per_element;
            if (mode == _complex_)
                bytes_per_sample *= 2;
        }

        std::string id;
        data_type_enum data_type;
        mode_enum mode;
        endian_enum endian;
        encoding_enum encoding;
        size_t bytes_per_sample;
        size_t bytes_per_element;

        const std::type_info *type;
    };


    //////////////////////////////
    //     CLASS DEFINITION     //
    //////////////////////////////

    class data_type_helper {
    public:
        data_type_helper();

        ~data_type_helper() {
        }

        const std::string get_identifier(data_type_enum data_type, mode_enum mode, endian_enum endian, encoding_enum encoding = _none_);
        data_description* get_dt_descriptor(const std::string &identifier);
        inline bool is_data_identifier_valid(const std::string &identifier){ return (get_dt_descriptor(identifier) != NULL);}
        
    private:


        std::map<std::string, data_description> dt_map;





    };


};

#endif

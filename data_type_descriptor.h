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

    // No byteswapping
    const std::string CHAR("CHAR");
    const std::string OCTET("OCTET");
    const std::string SHORT("SHORT");
    const std::string USHORT("USHORT");
    const std::string LONG("LONG");
    const std::string ULONG("ULONG");
    const std::string FLOAT("FLOAT");
    const std::string DOUBLE("DOUBLE");
    const std::string LONGLONG("LONGLONG");

    // With byteswapping
    const std::string SHORT_BS("SHORT_BYTESWAPPED");
    const std::string USHORT_BS("USHORT_BYTESWAPPED");
    const std::string LONG_BS("LONG_BYTESWAPPED");
    const std::string ULONG_BS("ULONG_BYTESWAPPED");
    const std::string FLOAT_BS("FLOAT_BYTESWAPPED");
    const std::string DOUBLE_BS("DOUBLE_BYTESWAPPED");
    const std::string LONGLONG_BS("LONGLONG_BYTESWAPPED");

    // Complex
    // No byteswapping
    const std::string COMPLEX_CHAR("COMPLEX_CHAR");
    const std::string COMPLEX_UCHAR("COMPLEX_UCHAR");
    const std::string COMPLEX_OCTET(COMPLEX_UCHAR);
    
    const std::string COMPLEX_SHORT("COMPLEX_SHORT");
    const std::string COMPLEX_USHORT("COMPLEX_USHORT");
    const std::string COMPLEX_LONG("COMPLEX_LONG");
    const std::string COMPLEX_ULONG("COMPLEX_ULONG");
    const std::string COMPLEX_FLOAT("COMPLEX_FLOAT");
    const std::string COMPLEX_DOUBLE("COMPLEX_DOUBLE");
    const std::string COMPLEX_LONGLONG("COMPLEX_LONGLONG");
    // With byteswapping
    const std::string COMPLEX_SHORT_BS("COMPLEX_SHORT_BYTESWAPPED");
    const std::string COMPLEX_USHORT_BS("COMPLEX_USHORT_BYTESWAPPED");
    const std::string COMPLEX_LONG_BS("COMPLEX_LONG_BYTESWAPPED");
    const std::string COMPLEX_ULONG_BS("COMPLEX_ULONG_BYTESWAPPED");
    const std::string COMPLEX_FLOAT_BS("COMPLEX_FLOAT_BYTESWAPPED");
    const std::string COMPLEX_DOUBLE_BS("COMPLEX_DOUBLE_BYTESWAPPED");
    const std::string COMPLEX_LONGLONG_BS("COMPLEX_LONGLONG_BYTESWAPPED");


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
        //        _big_endian_,
        //        _little_endian_,
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

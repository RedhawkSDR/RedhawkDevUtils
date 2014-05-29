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
#include "data_type_descriptor.h"


namespace SUPPORTED_DATA_TYPE {

    data_type_helper::data_type_helper() {

        dt_map.insert(std::make_pair(UNKNOWN, data_description(UNKNOWN, _8t_, _scalar_, _keep_endianess_, _none_, &typeid (char))));
        dt_map.insert(std::make_pair(XML, data_description(XML, _8t_, _scalar_, _keep_endianess_, _none_, &typeid (char))));
        dt_map.insert(std::make_pair(ALAW, data_description(ALAW, _8a_, _scalar_, _keep_endianess_, _alaw_, &typeid (char))));
        dt_map.insert(std::make_pair(MULAW, data_description(MULAW, _8u_, _scalar_, _keep_endianess_, _ulaw_, &typeid (char))));

        dt_map.insert(std::make_pair(CHAR, data_description(CHAR, _8t_, _scalar_, _keep_endianess_, _none_, &typeid (char))));
        dt_map.insert(std::make_pair(OCTET, data_description(OCTET, _8o_, _scalar_, _keep_endianess_, _none_, &typeid (unsigned char))));
        dt_map.insert(std::make_pair(SHORT, data_description(SHORT, _16t_, _scalar_, _keep_endianess_, _none_, &typeid (short))));
        dt_map.insert(std::make_pair(USHORT, data_description(USHORT, _16o_, _scalar_, _keep_endianess_, _none_, &typeid (unsigned short))));
        dt_map.insert(std::make_pair(LONG, data_description(LONG, _32t_, _scalar_, _keep_endianess_, _none_, &typeid (long))));
        dt_map.insert(std::make_pair(ULONG, data_description(ULONG, _32o_, _scalar_, _keep_endianess_, _none_, &typeid (unsigned long))));
        dt_map.insert(std::make_pair(FLOAT, data_description(FLOAT, _32f_, _scalar_, _keep_endianess_, _none_, &typeid (float))));
        dt_map.insert(std::make_pair(DOUBLE, data_description(DOUBLE, _64f_, _scalar_, _keep_endianess_, _none_, &typeid (double))));
        dt_map.insert(std::make_pair(LONGLONG, data_description(LONGLONG, _64t_, _scalar_, _keep_endianess_, _none_, &typeid (long long))));
        dt_map.insert(std::make_pair(SHORT_BS, data_description(SHORT_BS, _16t_, _scalar_, _byte_swap_, _none_, &typeid (short))));
        dt_map.insert(std::make_pair(USHORT_BS, data_description(USHORT_BS, _16o_, _scalar_, _byte_swap_, _none_, &typeid (unsigned short))));
        dt_map.insert(std::make_pair(LONG_BS, data_description(LONG_BS, _32t_, _scalar_, _byte_swap_, _none_, &typeid (long))));
        dt_map.insert(std::make_pair(ULONG_BS, data_description(ULONG_BS, _32o_, _scalar_, _byte_swap_, _none_, &typeid (unsigned long))));
        dt_map.insert(std::make_pair(FLOAT_BS, data_description(FLOAT_BS, _32f_, _scalar_, _byte_swap_, _none_, &typeid (float))));
        dt_map.insert(std::make_pair(DOUBLE_BS, data_description(DOUBLE_BS, _64f_, _scalar_, _byte_swap_, _none_, &typeid (double))));
        dt_map.insert(std::make_pair(LONGLONG_BS, data_description(LONGLONG_BS, _64t_, _scalar_, _byte_swap_, _none_, &typeid (long long))));

        dt_map.insert(std::make_pair(COMPLEX_CHAR, data_description(COMPLEX_CHAR, _8t_, _complex_, _keep_endianess_, _none_, &typeid (char))));
        dt_map.insert(std::make_pair(COMPLEX_OCTET, data_description(COMPLEX_OCTET, _8o_, _complex_, _keep_endianess_, _none_, &typeid (unsigned char))));
        dt_map.insert(std::make_pair(COMPLEX_SHORT, data_description(COMPLEX_SHORT, _16t_, _complex_, _keep_endianess_, _none_, &typeid (short))));
        dt_map.insert(std::make_pair(COMPLEX_USHORT, data_description(COMPLEX_USHORT, _16o_, _complex_, _keep_endianess_, _none_, &typeid (unsigned short))));
        dt_map.insert(std::make_pair(COMPLEX_LONG, data_description(COMPLEX_LONG, _32t_, _complex_, _keep_endianess_, _none_, &typeid (long))));
        dt_map.insert(std::make_pair(COMPLEX_ULONG, data_description(COMPLEX_ULONG, _32o_, _complex_, _keep_endianess_, _none_, &typeid (unsigned long))));
        dt_map.insert(std::make_pair(COMPLEX_FLOAT, data_description(COMPLEX_FLOAT, _32f_, _complex_, _keep_endianess_, _none_, &typeid (float))));
        dt_map.insert(std::make_pair(COMPLEX_DOUBLE, data_description(COMPLEX_DOUBLE, _64f_, _complex_, _keep_endianess_, _none_, &typeid (double))));
        dt_map.insert(std::make_pair(COMPLEX_LONGLONG, data_description(COMPLEX_LONGLONG, _64t_, _complex_, _keep_endianess_, _none_, &typeid (long long))));
        dt_map.insert(std::make_pair(COMPLEX_SHORT_BS, data_description(COMPLEX_SHORT_BS, _16t_, _complex_, _byte_swap_, _none_, &typeid (short))));
        dt_map.insert(std::make_pair(COMPLEX_USHORT_BS, data_description(COMPLEX_USHORT_BS, _16o_, _complex_, _byte_swap_, _none_, &typeid (unsigned short))));
        dt_map.insert(std::make_pair(COMPLEX_LONG_BS, data_description(COMPLEX_LONG_BS, _32t_, _complex_, _byte_swap_, _none_, &typeid (long))));
        dt_map.insert(std::make_pair(COMPLEX_ULONG_BS, data_description(COMPLEX_ULONG_BS, _32o_, _complex_, _byte_swap_, _none_, &typeid (unsigned long))));
        dt_map.insert(std::make_pair(COMPLEX_FLOAT_BS, data_description(COMPLEX_FLOAT_BS, _32f_, _complex_, _byte_swap_, _none_, &typeid (float))));
        dt_map.insert(std::make_pair(COMPLEX_DOUBLE_BS, data_description(COMPLEX_DOUBLE_BS, _64f_, _complex_, _byte_swap_, _none_, &typeid (double))));
        dt_map.insert(std::make_pair(COMPLEX_LONGLONG_BS, data_description(COMPLEX_LONGLONG_BS, _64t_, _complex_, _byte_swap_, _none_, &typeid (long long))));

    }

    const std::string data_type_helper::get_identifier(data_type_enum data_type, mode_enum mode, endian_enum endian, encoding_enum encoding) {
        for (std::map<std::string, data_description>::iterator iter = dt_map.begin(); iter != dt_map.end(); iter++) {
            if (data_type == iter->second.data_type && mode == iter->second.mode && endian == iter->second.endian && encoding == iter->second.encoding)
                return iter->first;
        }
        return UNKNOWN;
    }
    
     data_description* data_type_helper::get_dt_descriptor(const std::string &identifier){
         std::map<std::string, data_description>::iterator iter = dt_map.find(identifier);
         if(iter == dt_map.end()){
        	 iter = dt_map.find(UNKNOWN);
         }
         return &(iter->second);
     }


}





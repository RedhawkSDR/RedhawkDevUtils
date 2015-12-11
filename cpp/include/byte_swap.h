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
#ifndef _BYTE_SWAP_H
#define _BYTE_SWAP_H

#include <stdint.h>

#ifdef __GCC__
#include <byteswap.h>

template <typename DT> DT Byte_Swap16(DT i) {
    return __bswap_16(i);
};

template <typename DT> DT Byte_Swap32(DT i) {
    return __bswap_32(i);
};

template <typename DT> DT Byte_Swap64(DT i) {
    return __bswap_64(i);
};



inline void Byte_Swap16_inline(void* i) {
    *((uint16_t*) i) = __bswap_16(*((uint16_t*) i));
};

inline void Byte_Swap32_inline(void* i) {
    *((uint32_t*) i) = __bswap_32(*((uint32_t*) i));
};

inline void Byte_Swap64_inline(void* i) {
    *((uint64_t*) i) = __bswap_64(*((uint64_t*) i));
};

#else

template <typename DT> DT Byte_Swap16(DT i) {
    return (DT(i & 255) << 8) +
            (DT((i >> 8) & 255));
};

template <typename DT> DT Byte_Swap32(DT i) {
    return (DT(i & 255) << 24) +
            (DT((i >> 8) & 255) << 16) +
            (DT((i >> 16) & 255) << 8) +
            (DT((i >> 24) & 255));

};

template <typename DT> DT Byte_Swap64(DT i) {
    return (DT(i & 255) << 56) +
            (DT((i >> 8) & 255) << 48) +
            (DT((i >> 16) & 255) << 40) +
            (DT((i >> 24) & 255) << 32) +
            (DT((i >> 32) & 255) << 24) +
            (DT((i >> 40) & 255) << 16) +
            (DT((i >> 48) & 255) << 8) +
            (DT((i >> 56) & 255) << 0);

};


inline void Byte_Swap16_inline(void* i) {
    *((uint16_t*) i) =
            (uint16_t(*((uint16_t*) i) & 255) << 8) +
            (uint16_t((*((uint16_t*) i) >> 8) & 255));
};

inline void Byte_Swap32_inline(void* i) {
    *((uint32_t*) i) =
            (uint32_t(*((uint32_t*) i) & 255) << 24) +
            (uint32_t((*((uint32_t*) i) >> 8) & 255) << 16) +
            (uint32_t((*((uint32_t*) i) >> 16) & 255) << 8) +
            (uint32_t((*((uint32_t*) i) >> 24) & 255));
};

inline void Byte_Swap64_inline(void* i) {
    *((uint64_t*) i) =
            (uint64_t(*((uint64_t*) i) & 255) << 56) +
            (uint64_t((*((uint64_t*) i) >> 8) & 255) << 48) +
            (uint64_t((*((uint64_t*) i) >> 16) & 255) << 40) +
            (uint64_t((*((uint64_t*) i) >> 24) & 255) << 32) +
            (uint64_t((*((uint64_t*) i) >> 32) & 255) << 24) +
            (uint64_t((*((uint64_t*) i) >> 40) & 255) << 16) +
            (uint64_t((*((uint64_t*) i) >> 48) & 255) << 8) +
            (uint64_t((*((uint64_t*) i) >> 56) & 255) << 0);
};

#endif


// By default there are no swap functions that take in float or doubles

inline float Byte_Swap32(const float i) {

	float retVal;
	char *floatToConvert = ( char* ) & i;
	char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
};

inline double Byte_Swap64(const double i) {
	double retVal;
	char *doubleToConvert = ( char* ) & i;
	char *returnDouble = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnDouble[0] = doubleToConvert[7];
   returnDouble[1] = doubleToConvert[6];
   returnDouble[2] = doubleToConvert[5];
   returnDouble[3] = doubleToConvert[4];
   returnDouble[4] = doubleToConvert[3];
   returnDouble[5] = doubleToConvert[2];
   returnDouble[6] = doubleToConvert[1];
   returnDouble[7] = doubleToConvert[0];

   return retVal;
};



#endif  /* _BYTE_SWAP_H */


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
/* 
 * File:   vectorMagic.h
 * Author: 
 *
 * Created on June 1, 2011, 2:20 PM
 */

#ifndef _VECTORMAGIC_H
#define	_VECTORMAGIC_H

#include<vector>
#include<typeinfo>
#include <iostream>
#include <cstdlib>
#include <cstdio>

template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class vectorMagic : public std::vector<_Tp, _Alloc> {
    typedef std::vector<_Tp, _Alloc> _Base;
    typedef vectorMagic<_Tp, _Alloc> vector_type;

public:
    typedef _Tp value_type;
    typedef size_t size_type;
    typedef _Alloc allocator_type;

    vectorMagic(const allocator_type& __a = allocator_type()) : _Base(__a) {
    };

    vectorMagic(size_type __n, const value_type& __value = value_type(), const allocator_type& __a = allocator_type()) : _Base(__n, __a) {
    };

    template<typename _InputIterator> vectorMagic(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type()) : _Base(__a) {
    };

    vectorMagic(const _Base& __x) : _Base(__x.get_allocator()) {
        copyAtPos(0, __x.begin(), __x.size());
    };

    vectorMagic(value_type* memStart, size_t numValues, const allocator_type& __a = allocator_type()) : _Base(__a) {
        this->useExistingMemory(memStart, numValues);
    }

    void resize(size_type __new_size, value_type __x = value_type()) {
        if (__new_size > this->capacity())
            this->reserve(__new_size);
        this->_M_impl._M_finish = this->_M_impl._M_start + __new_size;
    }

    void useExistingMemory(value_type* memStart, size_t numValues) {
        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
        this->_M_impl._M_start = memStart;
        this->_M_impl._M_end_of_storage = memStart + numValues;
        this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
    }

    
    void copyEnd(value_type* memStart, size_t numValues) {
        size_t curSize = this->size();
        this->resize(curSize + numValues);
        memcpy(&this->at(curSize), memStart, sizeof (value_type) * numValues);
    }

    void copyAtPos(size_t curPosition, value_type* memStart, size_t numValues, bool alwaysResize = false) {
        if (this->size() < curPosition + numValues || alwaysResize)
            this->resize(curPosition + numValues);
        memcpy(&this->at(curPosition), memStart, sizeof (value_type) * numValues);
    }

    void memcpyAtPos(size_t curPosition, void* memStart, size_t numBytes, bool alwaysResize = false) {
        if (this->size() < curPosition + numBytes/sizeof (value_type)  || alwaysResize)
            this->resize(curPosition +  numBytes/sizeof (value_type));
        memcpy(&this->at(curPosition), memStart, numBytes);
    }

    void zeroFillAtPos(size_t curPosition, size_t numValues, bool alwaysResize = false) {
        if (numValues <= 0)
            return;
        if (this->size() < curPosition + numValues || alwaysResize)
            this->resize(curPosition + numValues);
        fill(&this->at(curPosition), &this->at(curPosition + numValues - 1), 0);
    }

    template <class SOURCE_TYPE> void genericCopyAtPos(size_t curPosition, SOURCE_TYPE* memStart, size_t numValues, double scale = 1, bool alwaysResize = false) {
        if (numValues <= 0)
            return;
        if (this->size() < curPosition + numValues || alwaysResize)
            this->resize(curPosition + numValues);

        //If converting the same types (ie - float to float), a memcpy would be quicker
        if (typeid (value_type) == typeid (SOURCE_TYPE) && scale == 1) {
            this->copyAtPos(curPosition, (value_type*) memStart, numValues);
            return;
        }

        //Now will have to iterate through the buffer and convert. 
        for (size_t elementNum = 0, outIter = curPosition; elementNum < numValues; elementNum++, outIter++) {
            this->at(outIter) = (*memStart);
            this->at(outIter) *= scale;
            memStart++;
        }

        return;
    }

private:

};

namespace VECTOR_MAGIC_HELPERS {
    template< typename VECTOR_DATA_TYPE> void std_vector_instant_resize(std::vector<VECTOR_DATA_TYPE> * vect_ptr, size_t new_size) {
        ((vectorMagic<VECTOR_DATA_TYPE>*) vect_ptr)->resize(new_size);
    };
};

namespace VECTOR_MAGIC_ALLOCATORS{

	template <typename _Tp, unsigned long ALIGNMENT_B=64>
	class aligned_alloc : public std::allocator<_Tp>
	{
		public:
			typedef size_t size_type;
			typedef _Tp* pointer;

			template<typename U>
			struct rebind {
				typedef aligned_alloc<U> other;
			};

			// constructor
			aligned_alloc() : std::allocator<_Tp>() { }

			// copy constructor
			aligned_alloc(const aligned_alloc&) { }


			pointer allocate(size_type __n, const void* = 0) {
				if (__builtin_expect(__n > this->max_size(), false))
					std::__throw_bad_alloc();

				void* mem = NULL;
				if (posix_memalign(&mem, ALIGNMENT_B, sizeof(_Tp)* __n) != 0)
				{
					std::__throw_bad_alloc();
				}
				return reinterpret_cast<pointer>(mem);
			}
		private:
			typedef std::allocator<_Tp> Base;

	};


	template <typename _Tp>
	class array_alloc : public std::allocator<_Tp>
	{
		public:
			typedef size_t size_type;
			typedef _Tp* pointer;

			template<typename U>
			struct rebind {
				typedef aligned_alloc<U> other;
			};

			// constructor
			array_alloc() : std::allocator<_Tp>() { }

			// copy constructor
			array_alloc(const array_alloc&) { }

			pointer allocate(size_type __n, const void* = 0) {
				if (__builtin_expect(__n > this->max_size(), false))
					std::__throw_bad_alloc();
				return (_Tp*) new _Tp[__n];
			}

			void deallocate(pointer __p, size_type) {
				::operator delete[](__p);
			}
		private:
			typedef std::allocator<_Tp> Base;
	};


}



#endif	/* _VECTORMAGIC_H */


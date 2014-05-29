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
 * File:   DATATYPETRANSFORM.H
 * Author: 
 *
 * Created on June 1, 2011, 1:19 PM
 */

#ifndef _DATATYPETRANSFORM_H
#define	_DATATYPETRANSFORM_H

#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <limits>
#include <numeric>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
namespace dataTypeTransform {

	/**********
	 Data Limits
	 **********/

	// Return the largest value for the max or abs(min)

	template <typename TYPE> double getNumericalLimits(TYPE val) {
		return std::max((double) -1*std::numeric_limits<TYPE>::min(), (double) (std::numeric_limits<TYPE>::max)());
	}

	template <typename TYPE> double getNumericalLimits(std::complex<TYPE> val) {
		return std::max((double) -1*std::numeric_limits<TYPE>::min(), (double) (std::numeric_limits<TYPE>::max)());
	}

	// Quick tests to see if data is unsigned
	template <typename TYPE> bool isUnsigned(TYPE val) {
		return ((std::numeric_limits<TYPE>::min)() == 0);
	}

	/**********
	 Conversions
	 **********/


	// Performs Max Scale conversions with float/normalization between [-1,1]

	template <typename IN_TYPE, typename OUT_TYPE> OUT_TYPE convertTypeScale(IN_TYPE ret, bool normalize = true) {
		double dRange = double(std::numeric_limits<OUT_TYPE>::max()) - double(std::numeric_limits<OUT_TYPE>::min());
		double dMin = double(std::numeric_limits<OUT_TYPE>::min());
		double sRange = double(std::numeric_limits<IN_TYPE>::max()) - double(std::numeric_limits<IN_TYPE>::min());
		double sMin = double(std::numeric_limits<IN_TYPE>::min());

		// Normalize float/double output b/t -1 and 1
		if (normalize && (typeid (OUT_TYPE) == typeid (float) || typeid (OUT_TYPE) == typeid (double))) {
			dRange = 2;
			dMin = -1;

		}

		// Normalize float/double input b/t -1 and 1
		if (normalize && (typeid (IN_TYPE) == typeid (float) || typeid (IN_TYPE) == typeid (double))) {
			sRange = 2;
			sMin = -1;
		}
	
		//Casting loses precision
		double tmpFloat = (ret - sMin)* (dRange / sRange) + dMin;
		return OUT_TYPE(tmpFloat);
	}


	
	// Converts Entire Vector
	template <typename IN_TYPE, typename IN_TYPE_ALLOC, typename OUT_TYPE> void convertVectorDataType(std::vector<IN_TYPE, IN_TYPE_ALLOC> *source, std::vector<OUT_TYPE> *dest, bool normalize = true) {
		double dRange = double(std::numeric_limits<OUT_TYPE>::max()) - double(std::numeric_limits<OUT_TYPE>::min());
		double dMin = double(std::numeric_limits<OUT_TYPE>::min());
		double sRange = double(std::numeric_limits<IN_TYPE>::max()) - double(std::numeric_limits<IN_TYPE>::min());
		double sMin = double(std::numeric_limits<IN_TYPE>::min());
	
		// Normalize float/double output b/t -1 and 1
		if (normalize && (typeid (OUT_TYPE) == typeid (float) || typeid (OUT_TYPE) == typeid (double))) {
			dRange = 2;
			dMin = -1;
	
		}

		// Normalize float/double input b/t -1 and 1
		if (normalize && (typeid (IN_TYPE) == typeid (float) || typeid (IN_TYPE) == typeid (double))) {
			sRange = 2;
			sMin = -1;
		}

		//Clear Output Vector
		dest->clear();
		double tmpFloat;

		for (size_t i = 0; i < source->size(); i++) {
			tmpFloat = (source->at(i) - sMin)* (dRange / sRange) + dMin;
			dest->push_back(OUT_TYPE(tmpFloat));
		}
	}


	// Bounds given by user
	template <typename IN_TYPE, typename IN_TYPE_ALLOC, typename OUT_TYPE> void convertVectorDataType(std::vector<IN_TYPE, IN_TYPE_ALLOC> *source, std::vector<OUT_TYPE> *dest, double sMin, double sMax, double dMin, double dMax) {
		double dRange = dMax - dMin;
		double sRange = sMax - sMin;
	
		//Clear Output Vector
		dest->clear();
		double tmpFloat;
		for (size_t i = 0; i < source->size(); i++) {
			if (source->at(i) <= sMin)
				tmpFloat = dMin;
			else if (source->at(i) >= sMax)
				tmpFloat = dMax;
			else
				tmpFloat = (source->at(i) - sMin)* (dRange / sRange) + dMin;
			dest->push_back(OUT_TYPE(tmpFloat));
		}
	}
	

}



#endif	/* _DATATYPETRANSFORM_H */

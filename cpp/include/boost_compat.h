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
#ifndef RHDU_BOOST_COMPAT_H
#define RHDU_BOOST_COMPAT_H

#if BOOST_FILESYSTEM_VERSION == 2
#define BOOST_FILESYSTEM_PATH(x) boost::filesystem::path((x), boost::filesystem::native)
#else
#define BOOST_FILESYSTEM_PATH(x) boost::filesystem::path((x))
#endif

#if BOOST_FILESYSTEM_VERSION < 3
#define BOOST_PATH_STRING(x) (x)
#else
#define BOOST_PATH_STRING(x) (x).string()
#endif

#endif // RHDU_BOOST_COMPAT_H

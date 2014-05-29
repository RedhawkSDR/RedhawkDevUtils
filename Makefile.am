#!/bin/sh
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK Basic Components RedhawkDevUtils.
#
# REDHAWK Basic Components RedhawkDevUtils is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK Basic Components RedhawkDevUtils is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
 
AM_CXXFLAGS = -Wall -g $(PROJECTDEPS_CFLAGS) $(BOOST_CPPFLAGS)

ACLOCAL_AMFLAGS = -I ../aclocal.d

lib_LTLIBRARIES = libVectorMagic_v1.la librh_utils_v1.la libDataTypeTransform_v1.la  libbyte_swap_v1.la libabstracted_file_io_v1.la libdata_type_descriptor_v1.la
libdir = $(spkg_libdir)

pkgconfigdir = $(spkg_pkgconfig_dir)/
pkgconfig_DATA = RedhawkDevUtils_v1.pc

xmldir = $(spkg_dir)/
dist_xml_DATA = RedhawkDevUtils_v1.spd.xml


datadir = $(spkg_impldir)

incdir = $(spkg_includedir)



# Library source go here
vectorMagic = vectorMagic.cpp vectorMagic.h
dataTypeTransform = dataTypeTransform.cpp dataTypeTransform.h 
byte_swap = byte_swap.h
abstracted_file_io = abstracted_file_io.h abstracted_file_io.cpp
data_type_descriptor = data_type_descriptor.h data_type_descriptor.cpp
rh_utils = rh_utils.h rh_utils.cpp

# Library headers must also be listed here
dist_inc_DATA = \
vectorMagic.h \
rh_utils.h \
dataTypeTransform.h\
data_type_descriptor.h\
byte_swap.h \
abstracted_file_io.h 


libVectorMagic_v1_la_LDFLAGS = -version-info 1:0:0
libVectorMagic_v1_la_SOURCES = \
	$(vectorMagic) 


libDataTypeTransform_v1_la_LDFLAGS = -version-info 1:0:0
libDataTypeTransform_v1_la_SOURCES = \
	$(dataTypeTransform) 

	
libbyte_swap_v1_la_LDFLAGS = -version-info 1:0:0
libbyte_swap_v1_la_SOURCES = \
	$(byte_swap) 
	
libabstracted_file_io_v1_la_LDFLAGS = -version-info 1:0:0
libabstracted_file_io_v1_la_SOURCES = \
	$(abstracted_file_io) 
	
libdata_type_descriptor_v1_la_LDFLAGS = -version-info 1:0:0
libdata_type_descriptor_v1_la_SOURCES = \
	$(data_type_descriptor) 

librh_utils_v1_la_LDFLAGS = -version-info 1:0:0 $(BOOST_LDFLAGS) $(BOOST_REGEX_LIB)
librh_utils_v1_la_SOURCES = \
	$(rh_utils)

	
distclean-local:
	rm -f config.*
	rm -rf autom4te.cache
	rm -f acinclude.m4
	rm -f aclocal.m4
	rm -f configure
	rm -f depcomp
	rm -f install-sh
	rm -f ltmain.sh
	rm -f Makefile.in
	rm -f missing
	rm -rf .deps
	rm -rf *.la
	rm -rf .libs


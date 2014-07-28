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
# By default, the RPM will install to the standard REDHAWK SDR root location (/var/redhawk/sdr)
# You can override this at install time using --prefix /new/sdr/root when invoking rpm (preferred method, if you must)

# Define default SDRROOT
%define _sdrroot /var/redhawk/sdr
%define _prefix %{_sdrroot}
#reflects the name in the configure.ac file
Name: 		RedhawkDevUtils_v1
#must match the version number in the configure.ac file
Version: 	3.0.0
Release:	10%{?dist}
Summary: 	Dependency RedhawkDevUtils
Prefix:		%{_sdrroot}

Group: 		REDHAWK/Dependencies
License: 	None
URL:		http://redhawksdr.org/	
Source0: 	%{name}-%{version}.tar.gz

AutoReqProv: yes

Requires: redhawk >= 1.10
BuildRequires: redhawk-devel >= 1.10 
BuildRequires: autoconf automake libtool
BuildRequires: omniORB bulkioInterfaces

# C++ requirements
%if "%{?rhel}" == "6"
Requires: libuuid
%else
Requires: e2fsprogs
%endif


%description
(U) REDHAWK Basic Components RedhawkDevUtils's development utility library.

%package devel
Requires: %{name} = %{version}
Group: Development/REDHAWK
Summary: Development files for %{name}

%if "%{?rhel}" == "6"
BuildRequires: libuuid-devel
%else
BuildRequires: e2fsprogs-devel
%endif

BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot

%description devel
(U) Development files for %{name} that provide useful c++ extension for interacting with the framework

%prep
%setup -q

%build
./reconf
SDRROOT=%{_sdrroot} %configure
make 


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}


%files
%defattr(-, redhawk, redhawk)
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/pkgconfig
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/pkgconfig
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/pkgconfig/RedhawkDevUtils_v1.pc
%{_prefix}/dom/deps/RedhawkDevUtils_v1/RedhawkDevUtils_v1.spd.xml
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libabstracted_file_io_v1.so*
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libbyte_swap_v1.so*
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libdata_type_descriptor_v1.so*
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libDataTypeTransform_v1.so*
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/librh_utils_v1.so*
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libVectorMagic_v1.so*

%files devel
%defattr(-, redhawk, redhawk)
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1
%{_prefix}/dom/deps/RedhawkDevUtils_v1/RedhawkDevUtils_v1.spd.xml
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/pkgconfig
%dir %{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/pkgconfig/RedhawkDevUtils_v1.pc
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/abstracted_file_io.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/byte_swap.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/data_type_descriptor.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/dataTypeTransform.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/rh_utils.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/include/vectorMagic.h
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libabstracted_file_io_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libbyte_swap_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libdata_type_descriptor_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libDataTypeTransform_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/librh_utils_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libVectorMagic_v1.a
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libabstracted_file_io_v1.la
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libbyte_swap_v1.la
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libdata_type_descriptor_v1.la
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libDataTypeTransform_v1.la
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/librh_utils_v1.la
%{_prefix}/dom/deps/RedhawkDevUtils_v1/cpp/lib/libVectorMagic_v1.la



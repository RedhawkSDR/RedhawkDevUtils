#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK Basic Components rh.RedhawkDevUtils shared library.
#
# REDHAWK Basic Components rh.RedhawkDevUtils shared library is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK Basic Components rh.RedhawkDevUtils shared library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
# By default, the RPM will install to the standard REDHAWK SDR root location (/var/redhawk/sdr)
%{!?_sdrroot: %define _sdrroot /var/redhawk/sdr}
%define _prefix %{_sdrroot}/dom/deps/rh/RedhawkDevUtils

# Point install paths to locations within our target SDR root
%define _libdir        %{_prefix}/cpp/lib
%define _sysconfdir    %{_prefix}/etc
%define _localstatedir %{_prefix}/var
%define _mandir        %{_prefix}/man
%define _infodir       %{_prefix}/info

Name:           rh.RedhawkDevUtils
Version:        4.0.1
Release:        2%{?dist}
Summary:        REDHAWK shared library %{name}

Group:          REDHAWK/Shared Libraries
License:        LGPLv3+
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  redhawk-devel >= 2.0
BuildRequires:  bulkioInterfaces >= 2.0

# Allow upgrades from previous package name
Obsoletes:      RedhawkDevUtils_v1 < 4.0.0



%description
REDHAWK shared library %{name}

%package devel
Summary:        REDHAWK shared library %{name}
Group:          REDHAWK/Shared Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       libuuid-devel
Obsoletes:      RedhawkDevUtils_v1-devel < 4.0.0


%description devel
Libraries and header files for REDHAWK shared library %{name}

%prep
%setup -q


%build
# Implementation cpp
pushd cpp
./reconf
%configure --with-sdr=%{_sdrroot}
make %{?_smp_mflags}
popd


%install
rm -rf $RPM_BUILD_ROOT
# Implementation cpp
pushd cpp
make install DESTDIR=$RPM_BUILD_ROOT
popd


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,redhawk,redhawk,-)
%dir %{_sdrroot}/dom/deps/rh
%dir %{_sdrroot}/dom/deps/rh/RedhawkDevUtils
%{_prefix}/RedhawkDevUtils.spd.xml
%{_prefix}/cpp
%exclude %{_libdir}/libRedhawkDevUtils.la
%exclude %{_libdir}/libRedhawkDevUtils.so
%exclude %{_libdir}/pkgconfig

%files devel
%defattr(-,redhawk,redhawk,-)
%{_libdir}/libRedhawkDevUtils.la
%{_libdir}/libRedhawkDevUtils.so
%{_libdir}/pkgconfig
%{_prefix}/include


#
# spec file for package libhllapi
#
# Copyright (c) 2015 SUSE LINUX GmbH, Nuernberg, Germany.
# Copyright (C) <2008> <Banco do Brasil S.A.>
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

#---[ Package header ]------------------------------------------------------------------------------------------------

Summary:		HLLAPI client library for lib3270/pw3270
Name:			libhllapi
Version:		5.3
Release:		0
License:		LGPL-3.0
Source:			%{name}-%{version}.tar.xz

Url:			https://github.com/PerryWerneck/libhllapi.git

Group:			Development/Libraries/C and C++

BuildRequires:	autoconf >= 2.61
BuildRequires:	automake
BuildRequires:	binutils
BuildRequires:	coreutils
BuildRequires:	gcc-c++
BuildRequires:	gettext-devel
BuildRequires:	m4
BuildRequires:	pkgconfig(ipc3270)

%if 0%{?fedora} ||  0%{?suse_version} > 1200

BuildRequires:	gcc-c++
BuildRequires:	pkgconfig(ipc3270)

%else

BuildRequires:	gcc-c++ >= 4.8
BuildRequires:	libipc3270-devel

%endif


%description

HLLAPI client library for pw3270/lib3270

See more details at https://softwarepublico.gov.br/social/pw3270/

#---[ Library ]-------------------------------------------------------------------------------------------------------

%define MAJOR_VERSION %(echo %{version} | cut -d. -f1)
%define MINOR_VERSION %(echo %{version} | cut -d. -f2 | cut -d+ -f1)
%define _libvrs %{MAJOR_VERSION}_%{MINOR_VERSION}

%package -n %{name}%{_libvrs}
Summary:	HLLAPI client library for lib3270/pw3270
Group:		Development/Libraries/C and C++

%description -n %{name}%{_libvrs}

HLLAPI client library for pw3270/lib3270

See more details at https://softwarepublico.gov.br/social/pw3270/

#---[ Development ]---------------------------------------------------------------------------------------------------

%package devel
Summary:	HLLAPI Development files.
Requires:	%{name}%{_libvrs} = %{version}

%description devel

Development files for lib3270/pw3270 HLLAPI client library.


#---[ Build & Install ]-----------------------------------------------------------------------------------------------

%prep
%setup

NOCONFIGURE=1 \
	./autogen.sh

%configure

%build
make clean
make all

%install
rm -rf $RPM_BUILD_ROOT

%makeinstall

%files -n %{name}%{_libvrs}
%defattr(-,root,root)

# https://en.opensuse.org/openSUSE:Packaging_for_Leap#RPM_Distro_Version_Macros
%if 0%{?sle_version} > 120200
%doc AUTHORS README.md
%license LICENSE
%else
%doc AUTHORS README.md LICENSE
%endif

%{_libdir}/%{name}.so.%{MAJOR_VERSION}.%{MINOR_VERSION}

%files devel
%defattr(-,root,root)

%{_libdir}/%{name}.so
%{_includedir}/lib3270/hllapi.h
%{_libdir}/pkgconfig/hllapi.pc

%pre -n %{name}%{_libvrs} -p /sbin/ldconfig

%post -n %{name}%{_libvrs} -p /sbin/ldconfig

%postun -n %{name}%{_libvrs} -p /sbin/ldconfig

%changelog


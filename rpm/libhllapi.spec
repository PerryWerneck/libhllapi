#
# spec file for packages pw3270-plugin-ipc
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

#---[ Versions ]------------------------------------------------------------------------------------------------------

%define MAJOR_VERSION 5
%define MINOR_VERSION 2

#---[ Main package ]--------------------------------------------------------------------------------------------------

Summary:	HLLAPI client library for lib3270/pw3270
Name:		libhllapi5_2
Version:	5.2
Release:	0
License:	LGPL-3.0
Source:		lib3270-hllapi-bindings-%{version}.tar.xz

Url:		https://github.com/PerryWerneck/lib3270-hllapi-bindings.git

Group:		System/X11/Terminals
BuildRoot:	/var/tmp/%{name}-%{version}

BuildRequires:  autoconf >= 2.61
BuildRequires:  automake
BuildRequires:  binutils
BuildRequires:  coreutils
BuildRequires:  gcc-c++
BuildRequires:  gettext-devel
BuildRequires:  m4
BuildRequires:  pkgconfig(ipc3270)

%if 0%{?fedora} ||  0%{?suse_version} > 1200

BuildRequires:	gcc-c++
BuildRequires:  pkgconfig(ipc3270)

%else

BuildRequires:  gcc-c++ >= 4.8
BuildRequires:  libipc3270-devel

%endif


%description

HLLAPI client library for pw3270/lib3270

See more details at https://softwarepublico.gov.br/social/pw3270/

%package devel
Summary: HLLAPI Development files.

%description devel

Development files for lib3270/pw3270 HLLAPI client library.


#---[ Build & Install ]-----------------------------------------------------------------------------------------------

%prep
%setup -n lib3270-hllapi-bindings-%{version}

NOCONFIGURE=1 ./autogen.sh

%configure

%build
make clean
make all

%install
rm -rf $RPM_BUILD_ROOT

make \
	DESTDIR=%{?buildroot:%{buildroot}} \
	install

%files
%defattr(-,root,root)
%doc AUTHORS LICENSE README.md

%{_libdir}/libhllapi.so.%{MAJOR_VERSION}
%{_libdir}/libhllapi.so.%{MAJOR_VERSION}.%{MINOR_VERSION}

%files devel
%defattr(-,root,root)
%{_libdir}/libhllapi.so
%{_includedir}/lib3270/hllapi.h

%pre
/sbin/ldconfig
exit 0

%post
/sbin/ldconfig
exit 0

%postun
/sbin/ldconfig
exit 0

%changelog


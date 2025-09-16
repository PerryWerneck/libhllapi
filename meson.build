# Maintainer: Perry Werneck <perry.werneck@gmail.com>

# SPDX-License-Identifier: LGPL-3.0-or-later 

# Copyright (C) 2025 Perry Werneck <perry.werneck@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

project(
	'libhllapi', 
	['c', 'cpp'],
  default_options : ['c_std=c11', 'cpp_std=c++17'],
	version: '5.6.0',
	license: 'GPL-3.0-or-later',
)

project_description = 'HLLAPI client library for lib3270/pw3270'

#
# Versioning
#
pkg_version = meson.project_version()
version_array = pkg_version.split('.')
pkg_major_version = version_array[0].to_int()
pkg_minor_version = version_array[1].to_int()
pkg_micro_version = version_array[2].to_int()

#libversion = '@0@.@1@'.format(pkg_major_version,pkg_minor_version)

#
# Compiler flags
#
cxx = meson.get_compiler('cpp')
enable_debug = get_option('debug') or get_option('buildtype').contains('debug')

compiler_flags_common = [
  '-fvisibility=hidden',
  '-DHAVE_CONFIG_H=1',
  '-ggdb3',
  '-fPIC',
  get_option('buildtype').contains('debug') ? '-DDEBUG=1' : '-DNDEBUG=1',
]

add_project_arguments(cxx.get_supported_arguments(compiler_flags_common), language: 'cpp')

#
# Dependencies
#
lib3270 = dependency(
  'lib3270', 
  required: true,
  fallback: [ 
    'lib3270', 
    'static_library'
  ]
)

lib_deps = [
  dependency(
    'libipc3270', 
    required: true,
    fallback: [ 
      'libipc3270', 
      'static_library'
    ]
  )
]

#
# Configuration
#
app_conf = configuration_data()
app_conf.set_quoted('PACKAGE_NAME', meson.project_name())
app_conf.set_quoted('PACKAGE_VERSION', meson.project_version())
app_conf.set_quoted('PACKAGE_DESCRIPTION', project_description)
app_conf.set('PACKAGE_VERSION_MAJOR', pkg_major_version)
app_conf.set('PACKAGE_VERSION_MINOR', pkg_minor_version)
app_conf.set('PACKAGE_VERSION_MICRO', pkg_micro_version)

app_conf.set('PRODUCT_NAME', lib3270.get_variable('product_name'))
app_conf.set('PRODUCT_VERSION',lib3270.get_variable('sdk_version'))

config_src = [
  configure_file(
      output : 'config.h', 
      configuration : app_conf
  )
]

#
# Source files
#
sources = [
  'src/core/actions.cc',
  'src/core/calls.cc',
  'src/core/controller.cc',
  'src/core/cursor.cc',
  'src/core/get.cc',
  'src/core/hllapi.cc',
  'src/core/keyboard.cc',
  'src/core/set.cc',
  'src/core/tools.cc',
]

includes_dir = include_directories('src/include')

#
# SDK
# https://mesonbuild.com/Pkgconfig-module.html
#
pkg = import('pkgconfig')

if host_machine.system() == 'windows'

  # https://mesonbuild.com/Windows-module.html
  windows = import('windows')
  resources = windows.compile_resources(
    configure_file(
      input : 'src/core/windows/resources.rc.in',
      output : 'resources.rc',
      configuration : app_conf
    )
  )

  pkg.generate(
    name: meson.project_name(),
    description: project_description,
    libraries: [ '-lhllapi.dll' ]
  )

  pkg.generate(
    name: meson.project_name() + '-static',
    description: project_description,
    libraries: [ '-lhllapi' ]
  )

  sources += [
    'src/core/windows/init.cc',
    'src/core/windows/tools.cc',
    resources,
  ]  
 
else

  pkg.generate(
    name: meson.project_name(),
    description: project_description,
    libraries: [ '-ldmiget' ]
  )

  pkg.generate(
    name: meson.project_name() + '-static',
    description: project_description,
    libraries: [ '-l:libdmiget.a' ]
  )

endif

#
# Targets
#
if cxx.get_id() == 'msvc'

  static_library(
    meson.project_name(), 
    sources,
    install: true,
    dependencies: lib_deps,
    include_directories: includes_dir
  )

  shared_library(
    meson.project_name(), 
    sources,
    install: true,
    dependencies: lib_deps,
    include_directories: includes_dir
  )

else

  dynamic = shared_library(
    meson.project_name(), 
    sources,
    install: true,
    cpp_args: [ '-ffat-lto-objects' ],
    gnu_symbol_visibility: 'hidden',
    dependencies: lib_deps,
    soversion: pkg_major_version,
    include_directories: includes_dir
  )

  static_library(
    meson.project_name(), 
    sources,
    install: true,
    cpp_args: [ '-fno-lto' ],
    dependencies: lib_deps,
    include_directories: includes_dir
  )

  executable(
    meson.project_name(),
    [ 'src/testprogram/testprogram.cc' ],
    install: true,
    link_with : dynamic,
    install_dir: get_option('bindir'),
    include_directories: includes_dir
  )
  
endif

# Install headers
install_headers(
  'src/include/lib3270/hllapi.h', 
  subdir: 'lib3270'  
)



# Copyright (c) 2006, 2017, Oracle and/or its affiliates.
# Copyright (c) 2008, 2018, MariaDB Corporation
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

project(SIPTOOLS)

cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_BUILD_TYPE "Debug")

include(ExternalProject)

set(OSIP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/libosip2-5.1.0 CACHE FILEPATH "Where to find sources for osip (libosip).")
set(osip_configure_opts --with-pic --enable-static)
list(APPEND osip_configure_opts "CC=${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1}")

file(GLOB OSIP_ALL_FILES ${OSIP_SOURCE_DIR}/*)
ExternalProject_Add(libosip
    PREFIX
        build_osip
    DOWNLOAD_COMMAND
        cp -a "${OSIP_ALL_FILES}" "<SOURCE_DIR>/"
    CONFIGURE_COMMAND
        "<SOURCE_DIR>/configure" ${osip_configure_opts}
        "--prefix=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr"
        "--libdir=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib"
    BUILD_COMMAND
        make
    INSTALL_COMMAND
        make install
)

add_library(osip2 STATIC IMPORTED)
set_target_properties(osip2 PROPERTIES IMPORTED_LOCATION
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib/libosip2.a")

add_library(osipparser2 STATIC IMPORTED)
set_target_properties(osipparser2 PROPERTIES IMPORTED_LOCATION
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib/libosipparser2.a")

add_dependencies(osip2 libosip)
add_dependencies(osipparser2 libosip)

set(OSIP2_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib/libosip2.a)
set(OSIPPARSER2_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib/libosipparser2.a)

include_directories(${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/include)

find_package(Threads)
if(CMAKE_THREAD_LIBS_INIT)
    set(THREAD_LIBRARY ${CMAKE_THREAD_LIBS_INIT})
else(CMAKE_THREAD_LIBS_INIT)
    message(FATAL_ERROR "pthread lib could not be found")
endif(CMAKE_THREAD_LIBS_INIT)

find_package(LibXml2)
if(LIBXML2_LIBRARY)
else(LIBXML2_LIBRARY)
    message(FATAL_ERROR "xml2 lib could not be found")
endif(LIBXML2_LIBRARY)

link_libraries(-ldl -lm)

add_executable(siptools
    ${PROJECT_SOURCE_DIR}/main/logger.cpp
    ${PROJECT_SOURCE_DIR}/main/thread.cpp
    ${PROJECT_SOURCE_DIR}/tport/transport.cpp
    ${PROJECT_SOURCE_DIR}/tport/udp_transport.cpp
    ${PROJECT_SOURCE_DIR}/sip/sip.cpp
    ${PROJECT_SOURCE_DIR}/main/main.cpp
    ${PROJECT_SOURCE_DIR}/main/errcode.cpp)

add_dependencies(siptools
    osip2
    osipparser2
)

add_definitions(
    -g
    -std=c++11
)

target_link_libraries(siptools
    ${THREAD_LIBRARY}
    ${OSIP2_LIBRARY}
    ${OSIPPARSER2_LIBRARY})

install(PROGRAMS
    ${CMAKE_BINARY_DIR}/siptools
    DESTINATION
    ${CMAKE_INSTALL_PREFIX}/sbin)

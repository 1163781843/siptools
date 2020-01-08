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

PROJECT(SIPTOOLS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_BUILD_TYPE "Debug")

include(ExternalProject)

SET(OSIP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/libs/libosip2-5.1.0 CACHE FILEPATH "Where to find sources for osip (libosip).")
SET(osip_configure_opts --with-pic --enable-static)
LIST(APPEND osip_configure_opts "CC=${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1}")

FILE(GLOB OSIP_ALL_FILES ${OSIP_SOURCE_DIR}/*)
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

SET(OSIP2_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/lib/libosip2.a)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/usr/include)

FIND_PACKAGE(Threads)
IF(CMAKE_THREAD_LIBS_INIT)
    SET(THREAD_LIBRARY ${CMAKE_THREAD_LIBS_INIT})
ELSE(CMAKE_THREAD_LIBS_INIT)
    MESSAGE(FATAL_ERROR "pthread lib could not be found")
ENDIF(CMAKE_THREAD_LIBS_INIT)

FIND_PACKAGE(LibXml2)
IF(LIBXML2_LIBRARY)
ELSE(LIBXML2_LIBRARY)
    MESSAGE(FATAL_ERROR "xml2 lib could not be found")
ENDIF(LIBXML2_LIBRARY)

LINK_LIBRARIES(-ldl -lm)

ADD_EXECUTABLE(siptools
    ${PROJECT_SOURCE_DIR}/tport/transport.cpp
    ${PROJECT_SOURCE_DIR}/tport/udp_transport.cpp
    ${PROJECT_SOURCE_DIR}/main/main.cpp
    ${PROJECT_SOURCE_DIR}/main/errcode.cpp)

ADD_DEFINITIONS(
    -g
    -std=c++11
)

TARGET_LINK_LIBRARIES(siptools
    ${THREAD_LIBRARY}
    ${OSIP2_LIBRARY})

INSTALL(PROGRAMS
    ${CMAKE_BINARY_DIR}/siptools
    DESTINATION
    ${CMAKE_INSTALL_PREFIX}/sbin)

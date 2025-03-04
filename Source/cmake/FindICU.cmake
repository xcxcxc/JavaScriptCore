# Finds the International Components for Unicode (ICU) Library
#
#  ICU_FOUND          - True if ICU found.
#  ICU_I18N_FOUND     - True if ICU's internationalization library found.
#  ICU_INCLUDE_DIRS   - Directory to include to get ICU headers
#                       Note: always include ICU headers as, e.g.,
#                       unicode/utypes.h
#  ICU_LIBRARIES      - Libraries to link against for the common ICU
#  ICU_I18N_LIBRARIES - Libraries to link against for ICU internationaliation
#                       (note: in addition to ICU_LIBRARIES)
#  ICU_DATA_LIBRARIES - Needed as far as I know, but only noticed starting with 52.1 and Android.

find_package(PkgConfig)
pkg_check_modules(PC_ICU icu-uc)

# Look for the header file.
find_path(
    ICU_INCLUDE_DIR
    NAMES unicode/utypes.h
    HINTS ${PC_ICU_INCLUDE_DIRS}
          ${PC_ICU_INCLUDEDIR}
    DOC "Include directory for the ICU library")
mark_as_advanced(ICU_INCLUDE_DIR)

# Look for the library.
find_library(
    ICU_LIBRARY
    NAMES icuuc cygicuuc cygicuuc32
    HINTS ${PC_ICU_LIBRARY_DIRS}
          ${PC_ICU_LIBDIR}
    DOC "Libraries to link against for the common parts of ICU")
mark_as_advanced(ICU_LIBRARY)

# Copy the results to the output variables.
if (ICU_INCLUDE_DIR AND ICU_LIBRARY)
    set(ICU_FOUND 1)
    set(ICU_LIBRARIES ${ICU_LIBRARY})
    set(ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIR})

    set(ICU_VERSION 0)
    set(ICU_MAJOR_VERSION 0)
    set(ICU_MINOR_VERSION 0)
    file(READ "${ICU_INCLUDE_DIR}/unicode/uversion.h" _ICU_VERSION_CONENTS)
    string(REGEX REPLACE ".*#define U_ICU_VERSION_MAJOR_NUM ([0-9]+).*" "\\1" ICU_MAJOR_VERSION "${_ICU_VERSION_CONENTS}")
    string(REGEX REPLACE ".*#define U_ICU_VERSION_MINOR_NUM ([0-9]+).*" "\\1" ICU_MINOR_VERSION "${_ICU_VERSION_CONENTS}")

    set(ICU_VERSION "${ICU_MAJOR_VERSION}.${ICU_MINOR_VERSION}")

    # Look for the ICU internationalization libraries
    find_library(
        ICU_I18N_LIBRARY
        NAMES icuin icui18n cygicuin cygicuin32
        DOC "Libraries to link against for ICU internationalization")
    mark_as_advanced(ICU_I18N_LIBRARY)
    if (ICU_I18N_LIBRARY)
        set(ICU_I18N_FOUND 1)
        set(ICU_I18N_LIBRARIES ${ICU_I18N_LIBRARY})
    else ()
        set(ICU_I18N_FOUND 0)
        set(ICU_I18N_LIBRARIES)
    endif ()
    
    # Look for the ICU udata libraries. The main icu library seems to depend on this via openCommonData
	# in source/common/udata.cpp in 52.1. I don't know if this is a new development or just unnoticed
	# by those dynamically linking everything and already have all the libraries.
	# But its absence is noticed in static linking scenarios.
    find_library(
		ICU_DATA_LIBRARY
        NAMES icudata cygicudata cygicudata32
        DOC "Libraries to link against for ICU data")
    mark_as_advanced(ICU_DATA_LIBRARY)
    if (ICU_DATA_LIBRARY)
		set(ICU_DATA_LIBRARY_FOUND 1)
		set(ICU_DATA_LIBRARIES ${ICU_DATA_LIBRARY})
    else ()
		set(ICU_DATA_LIBRARY_FOUND 0)
        set(ICU_DATA_LIBRARIES)
    endif ()
else ()
    set(ICU_FOUND 0)
    set(ICU_I18N_FOUND 0)
    set(ICU_LIBRARIES)
    set(ICU_I18N_LIBRARIES)
    set(ICU_DATA_LIBRARIES)
    set(ICU_INCLUDE_DIRS)
    set(ICU_VERSION)
    set(ICU_MAJOR_VERSION)
    set(ICU_MINOR_VERSION)
endif ()

if (ICU_FOUND)
    if (NOT ICU_FIND_QUIETLY)
        message(STATUS "Found ICU header files in ${ICU_INCLUDE_DIRS}")
        message(STATUS "Found ICU libraries: ${ICU_LIBRARIES}")
    endif ()
else ()
    if (ICU_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find ICU")
    else ()
        message(STATUS "Optional package ICU was not found")
    endif ()
endif ()

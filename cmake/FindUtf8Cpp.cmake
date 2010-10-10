# - Find Utf8Cpp
# Find the native Utf8Cpp includes
#
#   UTF8CPP_FOUND       - True if Utf8Cpp found.
#   UTF8CPP_INCLUDE_DIR - where to find utf8.h, etc.
#

IF( UTF8CPP_INCLUDE_DIR )
    # Already in cache, be silent
    SET( Utf8Cpp_FIND_QUIETLY TRUE )
ENDIF( UTF8CPP_INCLUDE_DIR )

FIND_PATH( UTF8CPP_INCLUDE_DIR "utf8.h"
           PATH_SUFFIXES "utf8cpp" )

# handle the QUIETLY and REQUIRED arguments and set UTF8CPP_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "Utf8Cpp" DEFAULT_MSG UTF8CPP_INCLUDE_DIR )

MARK_AS_ADVANCED( UTF8CPP_INCLUDE_DIR )

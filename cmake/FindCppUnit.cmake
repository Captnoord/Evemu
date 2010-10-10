# - Find CppUnit
# Find the native CppUnit includes and library
#
#   CPPUNIT_FOUND       - True if CppUnit found.
#   CPPUNIT_INCLUDE_DIR - where to find Test.h, etc.
#   CPPUNIT_LIBRARIES   - List of libraries when using CppUnit.
#

IF( CPPUNIT_INCLUDE_DIR )
    # Already in cache, be silent
    SET( CppUnit_FIND_QUIETLY TRUE )
ENDIF( CPPUNIT_INCLUDE_DIR )

FIND_PATH( CPPUNIT_INCLUDE_DIR "cppunit/Test.h" )

FIND_LIBRARY( CPPUNIT_LIBRARIES
              NAMES "cppunit"
              PATH_SUFFIXES "cppunit" )

# handle the QUIETLY and REQUIRED arguments and set CPPUNIT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "CppUnit" DEFAULT_MSG CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARIES )

MARK_AS_ADVANCED( CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARIES )

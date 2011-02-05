#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-zlib" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

FIND_PACKAGE( "ZLIB" REQUIRED )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonZlib.h" )
SET( SOURCE
     "" )

SET( zlib_INCLUDE
     "${TARGET_INCLUDE_DIR}/zlib/Utils.h" )
SET( zlib_SOURCE
     "${TARGET_SOURCE_DIR}/zlib/Utils.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${ZLIB_INCLUDE_DIR}"
                     "${common-std_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"       FILES ${INCLUDE} )
SOURCE_GROUP( "include\\zlib" FILES ${zlib_INCLUDE} )

SOURCE_GROUP( "src"       FILES ${SOURCE} )
SOURCE_GROUP( "src\\zlib" FILES ${zlib_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}      ${SOURCE}
             ${zlib_INCLUDE} ${zlib_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonZlib.h"
                  "zlib/Utils.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-std"
                       ${ZLIB_LIBRARIES} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_CATEGORY    "common" )
SET( TARGET_NAME        "common-std" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_CATEGORY}/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/${TARGET_CATEGORY}/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonStd.h" )
SET( SOURCE
     "" )

SET( stdx_INCLUDE
     "${TARGET_INCLUDE_DIR}/stdx/File.h"
     "${TARGET_INCLUDE_DIR}/stdx/Types.h"
     "${TARGET_INCLUDE_DIR}/stdx/Utils.h" )
SET( stdx_SOURCE
     "${TARGET_SOURCE_DIR}/stdx/File.cpp"
     "${TARGET_SOURCE_DIR}/stdx/Utils.cpp" )

SET( stream_INCLUDE
     "${TARGET_INCLUDE_DIR}/stream/Error.h"
     "${TARGET_INCLUDE_DIR}/stream/Hub.h"
     "${TARGET_INCLUDE_DIR}/stream/Hub.inl"
     "${TARGET_INCLUDE_DIR}/stream/Input.h"
     "${TARGET_INCLUDE_DIR}/stream/Output.h" )
SET( stream_SOURCE
     "" )

SET( util_INCLUDE
     "${TARGET_INCLUDE_DIR}/util/Buffer.h"
     "${TARGET_INCLUDE_DIR}/util/Buffer.inl"
     "${TARGET_INCLUDE_DIR}/util/Crc32.h"
     "${TARGET_INCLUDE_DIR}/util/Data.h"
     "${TARGET_INCLUDE_DIR}/util/Data.inl"
     "${TARGET_INCLUDE_DIR}/util/Lock.h"
     "${TARGET_INCLUDE_DIR}/util/Misc.h"
     "${TARGET_INCLUDE_DIR}/util/RefPtr.h"
     "${TARGET_INCLUDE_DIR}/util/RefPtr.inl"
     "${TARGET_INCLUDE_DIR}/util/SafeDelete.h"
     "${TARGET_INCLUDE_DIR}/util/Singleton.h"
     "${TARGET_INCLUDE_DIR}/util/String.h" )
SET( util_SOURCE
     "${TARGET_SOURCE_DIR}/util/Crc32.cpp"
     "${TARGET_SOURCE_DIR}/util/Misc.cpp"
     "${TARGET_SOURCE_DIR}/util/String.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"         FILES ${INCLUDE} )
SOURCE_GROUP( "include\\stdx"   FILES ${stdx_INCLUDE} )
SOURCE_GROUP( "include\\stream" FILES ${stream_INCLUDE} )
SOURCE_GROUP( "include\\util"   FILES ${util_INCLUDE} )

SOURCE_GROUP( "src"         FILES ${SOURCE} )
SOURCE_GROUP( "src\\stdx"   FILES ${stdx_SOURCE} )
SOURCE_GROUP( "src\\stream" FILES ${stream_SOURCE} )
SOURCE_GROUP( "src\\util"   FILES ${util_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}        ${SOURCE}
             ${stdx_INCLUDE}   ${stdx_SOURCE}
             ${stream_INCLUDE} ${stream_SOURCE}
             ${util_INCLUDE}   ${util_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonStd.h"
                  "stdx/Utils.cpp" )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

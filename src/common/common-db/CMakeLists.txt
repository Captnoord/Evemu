#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-db" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

FIND_PACKAGE( "MySQL" 5.0 REQUIRED )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonDb.h" )
SET( SOURCE
     "" )

SET( db_INCLUDE
     "${TARGET_INCLUDE_DIR}/db/Core.h"
     "${TARGET_INCLUDE_DIR}/db/Type.h" )
SET( db_SOURCE
     "${TARGET_SOURCE_DIR}/db/Core.cpp"
     "${TARGET_SOURCE_DIR}/db/Type.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${MYSQL_INCLUDE_DIR}"
                     "${common-std_INCLUDE_DIR}"
                     "${common-os_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"     FILES ${INCLUDE} )
SOURCE_GROUP( "include\\db" FILES ${db_INCLUDE} )

SOURCE_GROUP( "src"     FILES ${SOURCE} )
SOURCE_GROUP( "src\\db" FILES ${db_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}    ${SOURCE}
             ${db_INCLUDE} ${db_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonDb.h"
                  "db/Core.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-os"
                       ${MYSQL_LIBRARIES} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

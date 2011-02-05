#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_CATEGORY    "common" )
SET( TARGET_NAME        "common-xml" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_CATEGORY}/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/${TARGET_CATEGORY}/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonXml.h" )
SET( SOURCE
     "" )

SET( xml_INCLUDE
     "${TARGET_INCLUDE_DIR}/xml/Parser.h"
     "${TARGET_INCLUDE_DIR}/xml/ParserEx.h" )
SET( xml_SOURCE
     "${TARGET_SOURCE_DIR}/xml/Parser.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${TINYXML_INCLUDE_DIR}"
                     "${common-std_INCLUDE_DIR}"
                     "${common-os_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"      FILES ${INCLUDE} )
SOURCE_GROUP( "include\\xml" FILES ${xml_INCLUDE} )

SOURCE_GROUP( "src"      FILES ${SOURCE} )
SOURCE_GROUP( "src\\xml" FILES ${xml_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}     ${SOURCE}
             ${xml_INCLUDE} ${xml_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonXml.h"
                  "xml/Parser.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-os"
                       ${TINYXML_LIBRARIES} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

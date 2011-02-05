#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_CATEGORY    "common" )
SET( TARGET_NAME        "common-ga" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_CATEGORY}/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/${TARGET_CATEGORY}/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonGa.h" )
SET( SOURCE
     "" )

SET( ga_INCLUDE
     "${TARGET_INCLUDE_DIR}/ga/Utils.h" )
SET( ga_SOURCE
     "" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${GANGSTA_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"     FILES ${INCLUDE} )
SOURCE_GROUP( "include\\ga" FILES ${ga_INCLUDE} )

SOURCE_GROUP( "src"     FILES ${SOURCE} )
SOURCE_GROUP( "src\\ga" FILES ${ga_SOURCE} )

# This is header-only project, so ADD_LIBRARY doesn't work.
ADD_CUSTOM_TARGET( "${TARGET_NAME}"
                   SOURCES ${INCLUDE}    ${SOURCE}
                           ${ga_INCLUDE} ${ga_SOURCE} )

TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       ${GANGSTA_LIBRARIES} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

#
# CMake build system file for EVEmu
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "test-common" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/test/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/test/${TARGET_NAME}" )

SET( EVEMU_TEST_COMMON_XML
     CACHE PATH "XML output file of test-common (none if empty)." )
MARK_AS_ADVANCED( EVEMU_TEST_COMMON_XML )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/TestCommon.h"
     "${TARGET_INCLUDE_DIR}/TestFsDirectory.h"
     "${TARGET_INCLUDE_DIR}/TestFsFile.h"
     "${TARGET_INCLUDE_DIR}/TestNetStreamSocket.h"
     "${TARGET_INCLUDE_DIR}/TestStdFile.h"
     "${TARGET_INCLUDE_DIR}/TestUtilBuffer.h"
     "${TARGET_INCLUDE_DIR}/TestUtilData.h" )
SET( SOURCE
     "${TARGET_SOURCE_DIR}/TestCommon.cpp"
     "${TARGET_SOURCE_DIR}/TestFsDirectory.cpp"
     "${TARGET_SOURCE_DIR}/TestFsFile.cpp"
     "${TARGET_SOURCE_DIR}/TestStdFile.cpp"
     "${TARGET_SOURCE_DIR}/TestUtilBuffer.cpp"
     "${TARGET_SOURCE_DIR}/TestUtilData.cpp" )

########################
# Setup the executable #
########################
INCLUDE_DIRECTORIES( "${CPPUNIT_INCLUDE_DIR}"
                     "${common-std_INCLUDE_DIR}" "${common-os_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include" FILES ${INCLUDE} )
SOURCE_GROUP( "src"     FILES ${SOURCE} )

ADD_EXECUTABLE( "${TARGET_NAME}"
                ${INCLUDE} ${SOURCE} )
ADD_TEST( NAME "${TARGET_NAME}"
          COMMAND "${TARGET_NAME}" "${EVEMU_TEST_COMMON_XML}" )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "TestCommon.h"
                  "TestCommon.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-os"
                       ${CPPUNIT_LIBRARIES} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )

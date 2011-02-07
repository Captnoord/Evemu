#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

###############
# CMake setup #
###############
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )

PROJECT( "evemu" )
SET( PROJECT_DESCRIPTION_SUMMARY "A server emulator for EVE Online" )
SET( PROJECT_VENDOR              "The EVEmu development team" )
SET( PROJECT_VERSION             "0.7-common-rewrite" )

SET( CMAKE_MODULE_PATH
     "${PROJECT_SOURCE_DIR}/cmake"
     "${CMAKE_MODULE_PATH}" )

#################
# Build options #
#################
# For now it's this easy, add a proper build option/tests
# once we actually need it.
IF( WIN32 )
    SET( WITH_IPV6 FALSE )
ELSE( WIN32 )
    SET( WITH_IPV6 TRUE )
ENDIF( WIN32 )

SET( EVEMU_ROOT_DIR ".."
     CACHE PATH "The root of EVEmu workspace." )
MARK_AS_ADVANCED( EVEMU_ROOT_DIR )

SET( TINYXML_USE_STL ON
     CACHE BOOL "tinyxml will use native STL." )
MARK_AS_ADVANCED( TINYXML_USE_STL )

##################
# Platform tests #
##################
INCLUDE( "CheckIncludeFileCXX" )

CHECK_INCLUDE_FILE_CXX( "inttypes.h" HAVE_INTTYPES_H )
CHECK_INCLUDE_FILE_CXX( "crtdbg.h"   HAVE_CRTDBG_H )
CHECK_INCLUDE_FILE_CXX( "tr1/tuple"  HAVE_TR1_PREFIX )

INCLUDE( "CheckCXXSourceCompiles" )

# cfloat
CHECK_CXX_SOURCE_COMPILES( "#include <cfloat>
int main() { return *(int*)::_finite; }" HAVE__FINITE )
CHECK_CXX_SOURCE_COMPILES( "#include <cfloat>
int main() { return *(int*)::_isnan; }" HAVE__ISNAN )

# cmath
CHECK_CXX_SOURCE_COMPILES( "#include <cmath>
int main() { return std::finite( 0.0 ); }" HAVE_STD_FINITE )
CHECK_CXX_SOURCE_COMPILES( "#include <cmath>
int main() { return std::isfinite( 0.0 ); }" HAVE_STD_ISFINITE )
CHECK_CXX_SOURCE_COMPILES( "#include <cmath>
int main() { return std::isnan( 0.0 ); }" HAVE_STD_ISNAN )

# cstdarg
CHECK_CXX_SOURCE_COMPILES( "#include <cstdarg>
int main() { va_list ap[2]; va_copy( ap[0], ap[1] ); return 0; }" HAVE_VA_COPY )

# cstdio
CHECK_CXX_SOURCE_COMPILES( "#include <cstdio>
int main() { return *(int*)::asprintf; }" HAVE_ASPRINTF )
CHECK_CXX_SOURCE_COMPILES( "#include <cstdio>
int main() { return *(int*)::vasprintf; }" HAVE_VASPRINTF )
CHECK_CXX_SOURCE_COMPILES( "#include <cstdio>
int main() { return *(int*)::snprintf; }" HAVE_SNPRINTF )
CHECK_CXX_SOURCE_COMPILES( "#include <cstdio>
int main() { return *(int*)::vsnprintf; }" HAVE_VSNPRINTF )

# cstdlib
CHECK_CXX_SOURCE_COMPILES( "#include <cstdlib>
int main() { return *(int*)::random; }" HAVE_RANDOM )
CHECK_CXX_SOURCE_COMPILES( "#include <cstdlib>
int main() { return *(int*)::srandom; }" HAVE_SRANDOM )

# cstring
CHECK_CXX_SOURCE_COMPILES( "#include <cstring>
int main() { return *(int*)::strcasecmp; }" HAVE_STRCASECMP )
CHECK_CXX_SOURCE_COMPILES( "#include <cstring>
int main() { return *(int*)::strncasecmp; }" HAVE_STRNCASECMP )
CHECK_CXX_SOURCE_COMPILES( "#include <cstring>
int main() { return *(int*)::strdup; }" HAVE_STRDUP )

# ctime
CHECK_CXX_SOURCE_COMPILES( "#include <ctime>
int main() { timespec ts; return ts.tv_nsec; }" HAVE_TIMESPEC )
CHECK_CXX_SOURCE_COMPILES( "#include <ctime>
int main() { return *(int*)::localtime_r; }" HAVE_LOCALTIME_R )
CHECK_CXX_SOURCE_COMPILES( "#include <ctime>
int main() { return *(int*)::localtime_s; }" HAVE_LOCALTIME_S )

# sys/socket.h
CHECK_CXX_SOURCE_COMPILES( "#include <sys/socket.h>
int main() { return MSG_NOSIGNAL; }" HAVE_MSG_NOSIGNAL )
CHECK_CXX_SOURCE_COMPILES( "#include <sys/socket.h>
int main() { return SO_NOSIGPIPE; }" HAVE_SO_NOSIGPIPE )

# sys/types.h
CHECK_CXX_SOURCE_COMPILES( "#include <sys/types.h>
int main() { useconds_t us; return us; }" HAVE_USECONDS_T )

##################
# Compiler flags #
##################
IF( MSVC )
    #
    # This is here as we usually use pre-built MySQL binaries which are
    # compiled with /MT (use static libc); building with /MD (use DLL libc)
    # results in linker warning during the linking stage, however the resultant
    # binary works and is even a little bit smaller due to the dynamic linking.
    #
    # For now we supress the linker warning by forcing static libc.
    #
    STRING( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS}" )
    STRING( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}" )
    STRING( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL}" )
    STRING( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}" )
    STRING( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )
ENDIF( MSVC )

###################
# Source revision #
###################
IF( NOT EVEMU_SOURCE_REVISION )
    SET( EVEMU_SOURCE_SCM ""
         CACHE STRING "Source SCM in use; valid values: '', 'svn', 'git'" )

    IF( EVEMU_SOURCE_SCM STREQUAL "svn" )
        FIND_PACKAGE( "Subversion" REQUIRED )

        Subversion_WC_INFO( "${PROJECT_SOURCE_DIR}" "${PROJECT_NAME}" )

        SET( EVEMU_SOURCE_REVISION "r${${PROJECT_NAME}_WC_LAST_CHANGED_REV}"
             CACHE STRING "Revision of the source." )
    ELSEIF( EVEMU_SOURCE_SCM STREQUAL "git" )
        FIND_PACKAGE( "Git" REQUIRED )

        GIT_TREE_INFO( "${PROJECT_SOURCE_DIR}" "${PROJECT_NAME}" )

        SET( EVEMU_SOURCE_REVISION "${${PROJECT_NAME}_HASH_SHORT}"
             CACHE STRING "Revision of the source." )
    ENDIF( EVEMU_SOURCE_SCM STREQUAL "svn" )
ENDIF( NOT EVEMU_SOURCE_REVISION )

IF( EVEMU_SOURCE_REVISION )
    MARK_AS_ADVANCED( EVEMU_SOURCE_REVISION )
    SET( PROJECT_VERSION "${PROJECT_VERSION}-${EVEMU_SOURCE_REVISION}" )
ENDIF( EVEMU_SOURCE_REVISION )

#####################
# Generate config.h #
#####################
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/config.h.in"
                "${PROJECT_BINARY_DIR}/config.h"
                ESCAPE_QUOTES @ONLY )

ADD_DEFINITIONS( "-DHAVE_CONFIG_H" )
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" )

##################
# Targets import #
##################
IF( CMAKE_CROSSCOMPILING )
    SET( EVEMU_TARGETS_IMPORT ""
         CACHE FILEPATH "Path to an import script generated during a native build." )
    MARK_AS_ADVANCED( EVEMU_TARGETS_IMPORT )

    IF( EVEMU_TARGETS_IMPORT )
        INCLUDE( "${EVEMU_TARGETS_IMPORT}" )
    ENDIF( EVEMU_TARGETS_IMPORT )
ENDIF( CMAKE_CROSSCOMPILING )

############################
# Distributed dependencies #
############################
FIND_PACKAGE( "Gangsta" QUIET )
FIND_PACKAGE( "TinyXML" QUIET )
FIND_PACKAGE( "Utf8Cpp" QUIET )

IF( NOT GANGSTA_FOUND )
    ADD_SUBDIRECTORY( "src/dep/gangsta" )

    SET( GANGSTA_FOUND       TRUE )
    SET( GANGSTA_INCLUDE_DIR "${gangsta_INCLUDE_DIR}" )
    SET( GANGSTA_LIBRARIES   "gangsta" )
ENDIF( NOT GANGSTA_FOUND )

IF( NOT TINYXML_FOUND )
    ADD_SUBDIRECTORY( "src/dep/tinyxml" )

    SET( TINYXML_FOUND       TRUE )
    SET( TINYXML_INCLUDE_DIR "${tinyxml_INCLUDE_DIR}" )
    SET( TINYXML_LIBRARIES   "tinyxml" )
ENDIF( NOT TINYXML_FOUND )

IF( NOT UTF8CPP_FOUND )
    ADD_SUBDIRECTORY( "src/dep/utf8cpp" )

    SET( UTF8CPP_FOUND       TRUE )
    SET( UTF8CPP_INCLUDE_DIR "${utf8cpp_INCLUDE_DIR}" )
    SET( UTF8CPP_LIBRARIES   "utf8cpp" )
ENDIF( NOT UTF8CPP_FOUND )

ADD_SUBDIRECTORY( "src/dep/utils" )

###########################
# Generated documentation #
###########################
SET( EVEMU_DOC_ENABLE OFF
     CACHE BOOL "Generate HTML documentation (requires Doxygen)." )

IF( EVEMU_DOC_ENABLE )
    ADD_SUBDIRECTORY( "doc" )
ENDIF( EVEMU_DOC_ENABLE )

################
# Common stuff #
################
INCLUDE( "TargetBuildPCH" )

ADD_SUBDIRECTORY( "src/common/common-std" )
ADD_SUBDIRECTORY( "src/common/common-os" )
ADD_SUBDIRECTORY( "src/common/common-db" )
ADD_SUBDIRECTORY( "src/common/common-ga" )
ADD_SUBDIRECTORY( "src/common/common-xml" )
ADD_SUBDIRECTORY( "src/common/common-zlib" )

################
# EVEmu source #
################
SET( EVEMU_COLLECTOR_ENABLE OFF
     CACHE BOOL "Build eve-collector." )
SET( EVEMU_TOOL_ENABLE OFF
     CACHE BOOL "Build eve-tool." )

IF( NOT CMAKE_CROSSCOMPILING )
    ADD_SUBDIRECTORY( "src/eve/eve-xmlpktgen" )
ENDIF( NOT CMAKE_CROSSCOMPILING )

ADD_SUBDIRECTORY( "src/eve/eve-common" )
ADD_SUBDIRECTORY( "src/eve/eve-server" )

IF( EVEMU_COLLECTOR_ENABLE )
    ADD_SUBDIRECTORY( "src/eve/eve-collector" )
ENDIF( EVEMU_COLLECTOR_ENABLE )

IF( EVEMU_TOOL_ENABLE )
    ADD_SUBDIRECTORY( "src/eve/eve-tool" )
ENDIF( EVEMU_TOOL_ENABLE )

###############
# EVEmu tests #
###############
SET( EVEMU_TEST_ENABLE OFF
     CACHE BOOL "Build tests (requires CppUnit)." )

IF( EVEMU_TEST_ENABLE )
    FIND_PACKAGE( "CppUnit" REQUIRED )
    ENABLE_TESTING()

    ADD_SUBDIRECTORY( "src/test/test-common" )
ENDIF( EVEMU_TEST_ENABLE )

##################
# Targets export #
##################
IF( NOT CMAKE_CROSSCOMPILING )
    SET( EVEMU_TARGETS_EXPORT ""
         CACHE FILEPATH "Path to an import script to generate (for cross-compiling)." )
    MARK_AS_ADVANCED( EVEMU_TARGETS_EXPORT )

    IF( EVEMU_TARGETS_EXPORT )
        EXPORT( TARGETS "eve-xmlpktgen"
                FILE "${EVEMU_TARGETS_EXPORT}" )
    ENDIF( EVEMU_TARGETS_EXPORT )
ENDIF( NOT CMAKE_CROSSCOMPILING )

###############
# CPack setup #
###############
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION_SUMMARY}" )
SET( CPACK_PACKAGE_VENDOR              "${PROJECT_VENDOR}" )
SET( CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}" )

INCLUDE( "CPack" )

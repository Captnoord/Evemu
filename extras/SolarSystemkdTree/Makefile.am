INCLUDES += -I$(srcdir)/../../include/ascent-shared/
INCLUDES += -I$(srcdir)/../../include/Authorization/
INCLUDES += -I$(srcdir)/../../include/Communication/
INCLUDES += -I$(srcdir)/../../include/PCH/
INCLUDES += -I$(srcdir)/../../include/PythonObjects/
INCLUDES += -I$(srcdir)/../../include/server/
INCLUDES += -I$(srcdir)/../../include/server/Session/
INCLUDES += -I$(srcdir)/../../include/server/Space/
INCLUDES += -I$(srcdir)/../../include/TestCases/
INCLUDES += -I$(srcdir)/../../include/Utilities/

bin_PROGRAMS = kdtree

kdtree_SOURCES = \
	../../src/Communication/DBRowModule.cpp \
	../../src/Communication/MarshalStream.cpp \
	../../src/Communication/ReadStream.cpp \
	../../src/Communication/WriteStream.cpp \
    ../../src/PythonObjects/PyChameleon.cpp \
	../../src/PythonObjects/PyObjects.cpp \
	../../src/PythonObjects/PyStringTable.cpp \
	../../src/PythonObjects/PyUnicodeUCS2.cpp \
	../../src/server/ConfigMgr.cpp \
	../../src/server/ObjectCaching.cpp \
	../../src/Utilities/RleModule.cpp \
	camera.cpp \
	kdtree.cpp \
	main.cpp

kdtree_LDADD = \
  -lascent \
  -lglut \
  -ldl

kdtree_LDFLAGS = \
	-L$(srcdir)/../../src/ascent-shared \
	@MYSQL_LDFLAGS@

AM_CXXFLAGS+= \
	-pipe \
	-fno-inline \
	-Wall \
	-ansi

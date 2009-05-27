INCLUDES += -I$(srcdir)/../3rdparty/
INCLUDES += -I$(srcdir)/../include/ascent-shared/
INCLUDES += -I$(srcdir)/../include/Authorization/
INCLUDES += -I$(srcdir)/../include/Communication/
INCLUDES += -I$(srcdir)/../include/PCH/
INCLUDES += -I$(srcdir)/../include/PythonObjects/
INCLUDES += -I$(srcdir)/../include/server/
INCLUDES += -I$(srcdir)/../include/server/Session/
INCLUDES += -I$(srcdir)/../include/server/Space/
INCLUDES += -I$(srcdir)/../include/TestCases/
INCLUDES += -I$(srcdir)/../include/Utilities/



bin_PROGRAMS = evemuserver

evemuserver_SOURCES = \
    Authorization/PasswordModule.cpp \
    Communication/DBRowModule.cpp \
	Communication/MarshalStream.cpp \
	Communication/ReadStream.cpp \
	Communication/WriteStream.cpp \
	PythonObjects/PyChameleon.cpp \
	PythonObjects/PyClass.cpp \
	PythonObjects/PyDict.cpp \
	PythonObjects/PyInstance.cpp \
	PythonObjects/PyList.cpp \
	PythonObjects/PyModule.cpp \
	PythonObjects/PyObjects.cpp \
	PythonObjects/PyPackedRow.cpp \
	PythonObjects/PyString.cpp \
	PythonObjects/PySubStream.cpp \
	PythonObjects/PySubStruct.cpp \
	PythonObjects/PyTuple.cpp \
	PythonObjects/PyStringTable.cpp \
	PythonObjects/PyUnicodeUCS2.cpp \
	server/Session/EveSession.cpp \
	server/Space/RegionMapManager.cpp \
	server/Space/Space.cpp \
	server/Space/SpaceRunnable.cpp \
	server/AccountMgr.cpp \
	server/ConfigMgr.cpp \
	server/EveClientSession.cpp \
	server/EveClientSocket.cpp \
	server/ObjectCaching.cpp \
	Utilities/RleModule.cpp \
	main.cpp

evemuserver_LDADD = \
  -lascent \
  -ltinyxml \
  -lz \
  -lpthread \
  -ldl

evemuserver_LDFLAGS = \
	-L$(srcdir)/../3rdparty/tinyxml \
	-L$(srcdir)/ascent-shared \
	@MYSQL_LDFLAGS@

AM_CXXFLAGS+= \
	-pipe \
	-fno-inline \
	-Wall \
	-ansi

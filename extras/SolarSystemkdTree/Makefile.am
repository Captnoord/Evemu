INCLUDES += -I$(srcdir)/../../include/ascent-shared/
INCLUDES += -I$(srcdir)/../../include/ascent-shared/Config
INCLUDES += -I$(srcdir)/../../include/ascent-shared/Database
INCLUDES += -I$(srcdir)/../../include/server/Space/

bin_PROGRAMS = kdtree

kdtree_SOURCES = \
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

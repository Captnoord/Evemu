ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = configure

SUBDIRS = 3rdparty src/ascent-shared src/common src/rewrite src/server

if COLLECTOR
SUBDIRS+= extras/collector
endif

if EVETOOL
SUBDIRS+= extras/evetool
endif


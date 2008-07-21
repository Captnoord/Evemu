INCLUDES += -I$(srcdir)/../common/

BUILT_DISTS = \
	ObjectCaching.xmlp \
	AccountPkts.xmlp \
	Authentication.xmlp \
	Character.xmlp \
	CorporationPkts.xmlp \
	Crypto.xmlp \
	Destiny.xmlp \
	DogmaIM.xmlp \
	General.xmlp \
	Inventory.xmlp \
	Language.xmlp \
	LSCPkts.xmlp \
	Manufacturing.xmlp \
	Market.xmlp \
	Missions.xmlp \
	Standing2.xmlp \
	Tutorial.xmlp \
	Wallet.xmlp 
#  SkillPkts.xmlp 

noinst_LIBRARIES = \
	libauth.a \
	libpackets.a

libauth_a_SOURCES = \
	$(BUILT_DISTS) \
	Authentication.cpp 

libpackets_a_SOURCES = \
	$(BUILT_DISTS) \
  ObjectCaching.cpp \
  AccountPkts.cpp \
  Character.cpp \
  CorporationPkts.cpp \
  Crypto.cpp \
  Destiny.cpp \
  DogmaIM.cpp \
  General.cpp \
  Inventory.cpp \
  Language.cpp \
  LSCPkts.cpp \
  Manufacturing.cpp \
  Market.cpp \
  Missions.cpp \
  Standing2.cpp \
  Tutorial.cpp \
  Wallet.cpp
#	SkillPkts.cpp 

%.cpp: %.xmlp
	@echo Generating $^ -\> $@
	../xmlpktgen/xmlpktgen $^


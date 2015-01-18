#---------
#
# tlv Makefile
# modified from cpputest Makefile template
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = tlv
CPPUTEST_HOME = ../../cpputest-3.7dev

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc
#disable this warning because cpputest use old style cast
CPPUTEST_CXXFLAGS += -Wno-old-style-cast

SRC_DIRS = \
	applib

TEST_SRC_DIRS = \
	AllTests

INCLUDE_DIRS =\
  .\
  applib\
  $(CPPUTEST_HOME)/include\

include $(CPPUTEST_HOME)/build/MakefileWorker.mk



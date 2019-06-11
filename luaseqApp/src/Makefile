TOP=../..

include $(TOP)/configure/CONFIG

LIBRARY_IOC += luaseq

DBD += luaseq.dbd

luaseq_SRCS += lseqlib.cpp

luaseq_LIBS += lua
luaseq_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_IOC = luaseqTest

DBD += luaseqTestIOC.dbd
luaseqTestIOC_DBD += base.dbd
luaseqTestIOC_DBD += luaSupport.dbd

luaseqTest_LIBS += lua
luaseqTest_LIBS += $(EPICS_BASE_IOC_LIBS)

luaseqTest_SRCS += luaseqTestIOC_registerRecordDeviceDriver.cpp

luaseqTest_SRCS_DEFAULT += luaseqMain.cpp


include $(TOP)/configure/RULES

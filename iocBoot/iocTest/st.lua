iocsh.dbLoadDatabase("../../dbd/luaseqTestIOC.dbd")
iocsh.luaseqTestIOC_registerRecordDeviceDriver(pdbbase)

---------------
iocsh.iocInit()
---------------

iocsh.luaSpawn("./seqTest.lua")

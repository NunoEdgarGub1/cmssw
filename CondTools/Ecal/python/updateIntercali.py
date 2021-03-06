import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("EmptyIOVSource",
    lastValue = cms.uint64(100000000000),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(100000000000),
    interval = cms.uint64(1)
)

process.load("CondCore.DBCommon.CondDBCommon_cfi")

#process.CondDBCommon.connect = 'sqlite_file:DB.db'
#process.CondDBCommon.connect = 'oracle://cms_orcoff_prep/CMS_COND_ECAL'
process.CondDBCommon.connect = 'oracle://cms_orcon_prod/CMS_COND_31X_ECAL'
process.CondDBCommon.DBParameters.authenticationPath = '/nfshome0/popcondev/conddb'

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon, 
    logconnect = cms.untracked.string('sqlite_file:log.db'),   
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('EcalIntercalibConstantsRcd'),
        tag = cms.string('EcalIntercalibConstants_v9_offline')
    ))
)

process.Test1 = cms.EDAnalyzer("ExTestEcalIntercalibAnalyzer",
    record = cms.string('EcalIntercalibConstantsRcd'),
    loggingOn= cms.untracked.bool(True),
    IsDestDbCheckedInQueryLog=cms.untracked.bool(True),
    SinceAppendMode=cms.bool(True),
    Source=cms.PSet(
     FileLowField = cms.string('/nfshome0/popcondev/EcalTPGPopCon/CMSSW_3_11_0_ONLINE/src/CondTools/Ecal/python/interCalib_Boff.xml'),
     FileHighField = cms.string('/nfshome0/popcondev/EcalTPGPopCon/CMSSW_3_11_0_ONLINE/src/CondTools/Ecal/python/interCalib_Bon.xml'),
     Value_Bon = cms.untracked.double(0.75585),
     firstRun = cms.string('98273'),
     lastRun = cms.string('10000000'),
     OnlineDBSID = cms.string('cms_omds_lb'),
     OnlineDBUser = cms.string('cms_ecal_r'),
     OnlineDBPassword = cms.string('*******'),
     LocationSource = cms.string('P5'),
     Location = cms.string('P5_Co'),
     GenTag = cms.string('GLOBAL'),
     RunType = cms.string('COSMICS')
    )                            
)

process.p = cms.Path(process.Test1)

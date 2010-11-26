# Import configurations
import FWCore.ParameterSet.Config as cms

process = cms.Process("ChPartTree")

# initialize MessageLogger and output report ----------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#Geometry --------------------------------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

# configure modules via Global Tag -------------------------------------------------
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'MC_31X_V3::All'
#process.GlobalTag.globaltag = 'STARTUP3X_V8O::All'
#process.GlobalTag.globaltag = 'GR_R_35X_V6::All'
#process.GlobalTag.globaltag = 'GR_R_37X_V6A::All'
process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'

# Data source -----------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
            'dcap://pnfs/iihe/cms/store/user/xjanssen/data/CMSSW_3_6_2/DataCopy_36x/__MinimumBias__Commissioning10-Jun14thReReco_v1__RECO/DataCopy_36x__CMSSW_3_6_2__MinimumBias__Commissioning10-Jun14thReReco_v1__RECO_1_1_xom.root'
#           'file:///user/xjanssen/data/CMSSW_3_7_0_patch4/DataCopy_37x/__MinimumBias__Commissioning10-Jun9thReReco_v1__RECO/DataCopy_37x__CMSSW_3_7_0_patch4__MinimumBias__Commissioning10-Jun9thReReco_v1__RECO_1_2_ihT.root'
#            'rfio:///castor/cern.ch/cms/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0129/58274A60-523E-DF11-A311-0030486792F0.root'
#          'file:///user/xjanssen/MBdata/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/DataCopy_mb__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_1.root'
#          'file:///user/rougny/TESTFILES/Summer09-MC_31X_V3-v1_GEN-SIM-RECO_900GeV.root'
#          'file:///user/xjanssen/MBdata/D6T2360GeV_test01/simrecofile_103.root'
     )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# gen particles printouts -----------------------------------------------------------

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.GenPartDecay = cms.EDAnalyzer("ParticleDecayDrawer",
    src = cms.InputTag('genParticles'),
    printP4 = cms.untracked.bool(False),
    printPtEtaPhi =  cms.untracked.bool(False),
    printVertex = cms.untracked.bool(False)
)

process.GenPartTree = cms.EDAnalyzer("ParticleTreeDrawer",
    src = cms.InputTag('genParticles'),
    printP4 = cms.untracked.bool(False),
    printPtEtaPhi =  cms.untracked.bool(True),
    printVertex = cms.untracked.bool(False),
    printStatus = cms.untracked.bool(False),
    printIndex = cms.untracked.bool(False),
    status = cms.untracked.vint32( 3 )
)

process.GenPartList = cms.EDAnalyzer("ParticleListDrawer",
    maxEventsToPrint = cms.untracked.int32(10),
    printVertex = cms.untracked.bool(False),
    src = cms.InputTag("genParticles")
)

# STANDARD RECO ----------------------------------------------------------------------

process.load("Configuration.StandardSequences.Reconstruction_cff")

# Bamspot ----------------------------------------------------------------------------

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")

# Ferenc Tracking --------------------------------------------------------------------
process.load('RecoPixelVertexing.PixelLowPtUtilities.MinBiasTracking_cff')
#     This is only for MC, if Rechits are not present
#process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi")
#process.myTTRHBuilderWithoutAngle4PixelTriplets.ComputeCoarseLocalPositionFromDisk = True



# Ferenc vertex on Tracks ------------------------------------------------------------

import UserCode.FerencSiklerVertexing.NewVertexProducer_cfi

process.generalVertices = UserCode.FerencSiklerVertexing.NewVertexProducer_cfi.newVertices.clone()
process.generalVertices.TrackCollection = 'generalTracks'
process.generalVertices.PtMin = cms.double(0.0)

process.allVertices = UserCode.FerencSiklerVertexing.NewVertexProducer_cfi.newVertices.clone()
process.allVertices.TrackCollection = 'allTracks'
process.allVertices.PtMin = cms.double(0.0)


# MIT Big Events Cuts -------------------------------------------------------------------
process.load("MitEdm.Producers.evtSelData_cfi"); 
# MIT Data Filters -----------------------------------------------------------------------
process.load("MitEdm.Filters.FilterLumi_cfi")
process.load("MitEdm.Filters.FilterBX_cff")
process.load("MitEdm.Filters.FilterEvtSel_cff")

# L1 Filters -------------------------------------------------------------------------
#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND ( 34 OR 40 OR 41 ) AND NOT 36 AND NOT 37 AND NOT 38 AND NOT 39')


# ----- Trigger Selection --------------------

process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'
#process.mypath=cms.Path(process.hltPhysicsDeclared+<my_other_things_here>)

#---------------Beam scraping -------------------------


process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(False),
                                numtrack = cms.untracked.uint32(10),
                                thresh = cms.untracked.double(0.25)
                                )

# gen particles Tree -----------------------------------------------------------------

process.GenPartAna = cms.EDAnalyzer('ChPartTree'

  , fileName = cms.untracked.string('ChPartTree.root')

# Modules to execute
  , StoreGenPart = cms.bool(False)
  , StoreGenKine = cms.bool(False)

# Define DATA Collections
  , genPartColl   = cms.InputTag("genParticles")
  , hepMCColl     = cms.InputTag("generator")
  , requested_hlt_bits = cms.vstring( 'HLT_L1_BscMinBiasOR_BptxPlusORMinus' ,
                                      'HLT_PixelTracks_Multiplicity40' ,
                                      'HLT_PixelTracks_Multiplicity70' ,
                                      'HLT_PixelTracks_Multiplicity85' 
                                    )

# Fwd Gap Stuff
  , CaloTowerTag = cms.InputTag("towerMaker")
  ,  EnergyThresholdHB = cms.double(1.5)
  ,  EnergyThresholdHE = cms.double(2.0)
  ,  EnergyThresholdHF = cms.double(4.0)
  ,  EnergyThresholdEB = cms.double(1.5)
  ,  EnergyThresholdEE = cms.double(2.5)

)


# Data output ----------------------------------------------------------------------- 
process.out = cms.OutputModule("PoolOutputModule",
    verbose = cms.untracked.bool(False),
    fileName = cms.untracked.string('cmsdata.root')
)


# PAth (what to do) ------------------------------------------------------------------


process.freco = cms.Path(
                           process.hltPhysicsDeclared
                         * process.noscraping
#                        *   process.goodLumiBlocks
#                        * process.collisionBunchCrossings
#                         * process.hltLevel1GTSeed
                        )

process.simu  = cms.Path(process.offlineBeamSpot)

process.lreco = cms.Path(
                         process.siPixelRecHits *
                         process.siStripMatchedRecHits
                       )

process.greco = cms.Path(
                         process.evtSelData * 
#                        process.looseEvtSelFilter *
                         process.minBiasTracking *
                         process.allVertices *
#                        process.generalVertices *
#                        process.GenPartDecay *
#                        process.GenPartTree *
#                        process.GenPartList *  
                         process.GenPartAna
                       )

#process.outpath = cms.EndPath(process.out)

process.schedule = cms.Schedule(
                                  process.freco
                                , process.simu
                                , process.lreco
                                , process.greco
#                                , process.outpath
                               )





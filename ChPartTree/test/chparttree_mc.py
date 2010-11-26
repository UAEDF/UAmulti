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
#process.GlobalTag.globaltag = 'STARTUP3X_V8K::All'
process.GlobalTag.globaltag = 'START36_V10::All'

# Data source -----------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#           ' '
           'file:///user/xjanssen/MBdata/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/DataCopy_mb__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root'
#          'file:///user/rougny/TESTFILES/Summer09-MC_31X_V3-v1_GEN-SIM-RECO_900GeV.root'
#          'file:///user/xjanssen/MBdata/D6T2360GeV_test01/simrecofile_103.root'
     )
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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
    maxEventsToPrint = cms.untracked.int32(100),
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
process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi")
process.myTTRHBuilderWithoutAngle4PixelTriplets.ComputeCoarseLocalPositionFromDisk = True

# Ferenc vertex on Tracks ------------------------------------------------------------

import UserCode.FerencSiklerVertexing.NewVertexProducer_cfi

process.generalVertices = UserCode.FerencSiklerVertexing.NewVertexProducer_cfi.newVertices.clone()
process.generalVertices.TrackCollection = 'generalTracks'
process.generalVertices.PtMin = cms.double(0.0)

process.allVertices = UserCode.FerencSiklerVertexing.NewVertexProducer_cfi.newVertices.clone()
process.allVertices.TrackCollection = 'allTracks'
process.allVertices.PtMin = cms.double(0.0)


#MIT dndeta selection ----------------------------------------------------------------
process.load("MitEdm.Producers.evtSelData_cfi"); 


# gen particles Tree -----------------------------------------------------------------

process.GenPartAna = cms.EDAnalyzer('ChPartTree'

  , fileName = cms.untracked.string('ChPartTree.root')

# Modules to execute
  , StoreGenPart = cms.bool(True)
  , StoreGenKine = cms.bool(True)

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

process.simu  = cms.Path(process.offlineBeamSpot)

process.lreco = cms.Path(
                         process.siPixelRecHits *
                         process.siStripMatchedRecHits
                       )

process.greco = cms.Path(
			 process.minBiasTracking *
                         process.allVertices *
                         process.generalVertices * 
                         process.evtSelData *
#                        process.GenPartDecay *
#                        process.GenPartTree *
#                        process.GenPartList *  
                         process.GenPartAna
                       )

#process.outpath = cms.EndPath(process.out)

process.schedule = cms.Schedule(
                                  process.simu 
                                , process.lreco  
                                , process.greco 
#                               , process.outpath
                               )







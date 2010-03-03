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
process.GlobalTag.globaltag = 'STARTUP3X_V8O::All'

# Data source -----------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#          'file:///user/rougny/TESTFILES/Summer09-MC_31X_V3-v1_GEN-SIM-RECO_900GeV.root'
           'file:///user/xjanssen/MBdata/D6T2360GeV_test01/simrecofile_103.root'
     )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
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
    maxEventsToPrint = cms.untracked.int32(100),
    printVertex = cms.untracked.bool(False),
    src = cms.InputTag("genParticles")
)

# STANDARD RECO ----------------------------------------------------------------------

process.load("Configuration.StandardSequences.Reconstruction_cff")

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

  , fileName = cms.untracked.string('ChPartTree_2.36_simrecfile103.root')

# Modules to execute
  , StoreGenPart = cms.bool(True)
  , StoreGenKine = cms.bool(True)

# Define DATA Collections
  , genPartColl   = cms.InputTag("genParticles")
  , hepMCColl     = cms.InputTag("generator")


)


# Data output ----------------------------------------------------------------------- 
process.out = cms.OutputModule("PoolOutputModule",
    verbose = cms.untracked.bool(False),
    fileName = cms.untracked.string('cmsdata.root')
)


# PAth (what to do) ------------------------------------------------------------------
process.path = cms.Path(
			 process.minBiasTracking *
                         process.allVertices *
                         process.generalVertices * 
                         process.siPixelRecHits *
                         process.evtSelData *
#                        process.GenPartDecay *
#                        process.GenPartTree *
#                        process.GenPartList *  
                         process.GenPartAna
                       )

# EndPath (what to store) ------------------------------------------------------------
#process.outpath = cms.EndPath(process.out)





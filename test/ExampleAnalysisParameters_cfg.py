import FWCore.ParameterSet.Config as cms
from UserCode.diall.storeTools_cff import *
import FWCore.ParameterSet.VarParsing as VarParsing

## parse some command line arguments
#options = VarParsing.VarParsing ('analysis')

#options.register ('anaFile',
#                  1, # default value
#                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                  VarParsing.VarParsing.varType.int,       # string, int, or float
#                  "Number of files to process (-1 for all)")

#options.anaFile = 1

#options.parseArguments()

#print('anaFile: ',options.anaFile)

tt_PbPb=("ZMMpp_v0.root","/store/group/phys_heavyions/velicanu/forest/Run2015E/SingleMuHighPt/RAW-RECO/ZMM-PromptReco-v1/Merged/")
DY_PbPb=("HiForest_*.root","/store/group/phys_heavyions/mverweij/pp5TeV/SingleHighPtMu/forest_SingleMuHighPt_v2_azsimon/partial/")
QCDEM_PbPb=("1.root","/store/user/velicanu/Merged/PromptReco-AOD-DimuonSkim-Mass40-ppFOREST_v10/")
QCDDJ_PbPb=("QCDDJ_PbPb.root","/store/cmst3/user/mverweij/jetsPbPb/Run2Prep/Dijet80CMSSW753p1/v6/PyquenUnquenched_Dijet_NcollFilt_pthat80_740pre8_MCHI1_74_V4_GEN-SIM_v3/crab_HiForestDijet80Run2Fullv6/151020_135458")
HCALMeth2=("HCALM2.root","/store/user/istaslis/PyquenUnquenched_Dijet_NcollFilt_pthat80_753patch1_RECODEBUG_FOREST/")

centralityRequirements={"inc":[0,200],
                        "0to20":[0,40],
                        "20to50":[40,100],
                        "50to80":[100,160],
                        "80to100":[160,200]}


#sample=tt_PbPb
#sample=DY_PbPb
sample=QCDEM_PbPb
#sample=HCALMeth2 
#sample = QCDDJ_PbPb

centralityBins=centralityRequirements["inc"]
#centralityBins=centralityRequirements["0to20"]
#centralityBins=centralityRequirements["20to50"]
#centralityBins=centralityRequirements["50to80"]
#centralityBins=centralityRequirements["80to100"]

config = cms.PSet(
    output = cms.string('cen_%dto%d_%s'%(centralityBins[0],centralityBins[1],sample[0])),
    input  = cms.vstring( 
        #fillFromStore(sample[1])
        'root://cmsxrootd.fnal.gov//store/user/velicanu/Merged/Pythia8_Z30mumuJet_pthat30Norm_TuneCUETP8M1_5020GeV_cff_ppFOREST_PrivMC_v10/0.root'
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_merged.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_2.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_3.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_4.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_5.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_6.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_7.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_8.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_9.root',
        #'root://cmsxrootd.fnal.gov//store/user/gkrintir/TopHI/HighPtLowerPhotons_Run2015E-PromptReco-v1_v1/HighPtLowerPhotons/crab_TopHI/160111_200108/0000/HiForestAOD_10.root'
        ), #fillFromStore(sample[1]) 
    maxEvents = cms.int32(100),#-1),
    minCentrality = cms.int32(centralityBins[0]),
    maxCentrality = cms.int32(centralityBins[1])#,
   # anaFile = cms.int32(options.anaFile)
)

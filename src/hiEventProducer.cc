//
// producer of container with heavy-ion event properties
//

#include "UserCode/diall/interface/hiEventProducer.h"

#include <ostream>
using namespace std;

ClassImp(hiEventProducer)

//__________________________________________________________
hiEventProducer::hiEventProducer() :
inputBase("hiEventProducer"),
  fhiEventContName("hiEventContainer"),
  fhiEventContainer(0),
  fRun(-1),
  fEvt(-1),
  fLumi(-1),
  fWeight(-1),
  fVx(0.),
  fVy(0.),
  fVz(0.),
  fHiBin(0),
  fHiHF(0),
  fNPV(0),
  fHLT_AK4PFJet80_Eta5p1_v1(0),
  fHLT_HISinglePhoton30_Eta3p5_v1(0),
  fHLT_FullTrack24ForPPRef_v1(0),
  fHLT_FullTrack45ForPPRef_v1(0),
  fcollisionEvtSel(1),
  fHBHENoise(1),
  fHBHENoiseLoose(1),
  fPrimaryVertexFilter(1),
  fClusterCompatibilityFilter(1),
  fHFCoincFilter(1),
  fForestFJRho(),
  fRhoMap(0)
{
  //default constructor
}

//__________________________________________________________
hiEventProducer::hiEventProducer(const char *name) :
  inputBase(name),
  fhiEventContName("hiEventContainer"),
  fhiEventContainer(0),
  fRun(-1),
  fEvt(-1),
  fLumi(-1),
  fWeight(-1),
  fVx(0.),
  fVy(0.),
  fVz(0.),
  fHiBin(0),
  fHiHF(0),
  fNPV(0),
  fHLT_AK4PFJet80_Eta5p1_v1(0),
  fHLT_HISinglePhoton30_Eta3p5_v1(0),
  fHLT_FullTrack24ForPPRef_v1(0),
  fHLT_FullTrack45ForPPRef_v1(0),
  fcollisionEvtSel(1),
  fHBHENoise(1),
  fHBHENoiseLoose(1),
  fPrimaryVertexFilter(1),
  fClusterCompatibilityFilter(1),
  fHFCoincFilter(1),
  fForestFJRho(),
  fRhoMap(0)
{
  //standard constructor
}

//__________________________________________________________
void hiEventProducer::SetInput(TChain *chain) {

  inputBase::SetInput(chain);
  //Init();
  
}

//__________________________________________________________
Bool_t hiEventProducer::Init() {

  if(!inputBase::Init()) return kFALSE;

  if(fInputMode==hiForest) {
    if(fChain->GetBranch("run"))
      fChain->SetBranchAddress("run",   &fRun,   &b_run);
    if(fChain->GetBranch("evt"))
      fChain->SetBranchAddress("evt",   &fEvt,   &b_evt);
    if(fChain->GetBranch("lumi"))
      fChain->SetBranchAddress("lumi",  &fLumi,  &b_lumi);
    if(fChain->GetBranch("weight"))
      fChain->SetBranchAddress("weight",&fWeight,&b_weight);
    if(fChain->GetBranch("vx"))
      fChain->SetBranchAddress("vx",    &fVx,    &b_vx);
    if(fChain->GetBranch("vy"))
      fChain->SetBranchAddress("vy",    &fVy,    &b_vy);
    if(fChain->GetBranch("vz"))
      fChain->SetBranchAddress("vz",    &fVz,    &b_vz);
    if(fChain->GetBranch("hiBin"))
      fChain->SetBranchAddress("hiBin", &fHiBin, &b_hiBin);
    else Printf("cannot find hiBin branch");
    if(fChain->GetBranch("hiHF"))
      fChain->SetBranchAddress("hiHF",  &fHiHF,  &b_hiHF);
    if(fChain->GetBranch("nPU"))
      fChain->SetBranchAddress("nPU",&fNPV,&b_nPU);
    if(fChain->GetBranch("HLT_AK4PFJet80_Eta5p1_v1"))
      fChain->SetBranchAddress("HLT_AK4PFJet80_Eta5p1_v1",&fHLT_AK4PFJet80_Eta5p1_v1,&b_HLT_AK4PFJet80_Eta5p1_v1);
    if(fChain->GetBranch("HLT_HISinglePhoton30_Eta3p5_v1"))
      fChain->SetBranchAddress("HLT_HISinglePhoton30_Eta3p5_v1",&fHLT_HISinglePhoton30_Eta3p5_v1,&b_HLT_HISinglePhoton30_Eta3p5_v1);
    if(fChain->GetBranch("HLT_FullTrack24ForPPRef_v1"))
      fChain->SetBranchAddress("HLT_FullTrack24ForPPRef_v1",&fHLT_FullTrack24ForPPRef_v1,&b_HLT_FullTrack24ForPPRef_v1);
    if(fChain->GetBranch("HLT_FullTrack45ForPPRef_v1"))
      fChain->SetBranchAddress("HLT_FullTrack45ForPPRef_v1",&fHLT_FullTrack45ForPPRef_v1,&b_HLT_FullTrack45ForPPRef_v1);
    if(fChain->GetBranch("PAcollisionEventSelection"))
      fChain->SetBranchAddress("PAcollisionEventSelection",&fcollisionEvtSel,&b_collisionEvtSel);
    else if(fChain->GetBranch("pcollisionEventSelection"))
      fChain->SetBranchAddress("pcollisionEventSelection",&fcollisionEvtSel,&b_collisionEvtSel);
    if(fChain->GetBranch("pHBHENoiseFilterResult"))
      fChain->SetBranchAddress("pHBHENoiseFilterResult",&fHBHENoise,&b_HBHENoise);
    if(fChain->GetBranch("pHBHENoiseFilterResultRun2Loose"))
      fChain->SetBranchAddress("pHBHENoiseFilterResultRun2Loose",&fHBHENoiseLoose,&b_HBHENoiseLoose);
    if(fChain->GetBranch("pprimaryVertexFilter"))
      fChain->SetBranchAddress("pprimaryVertexFilter",&fPrimaryVertexFilter,&b_PrimaryVertexFilter);
    if(fChain->GetBranch("pclusterCompatibilityFilter"))
      fChain->SetBranchAddress("pclusterCompatibilityFilter",&fClusterCompatibilityFilter,&b_ClusterCompatibilityFilter);
    if(fChain->GetBranch("phfCoincFilter3"))
      fChain->SetBranchAddress("phfCoincFilter3",&fHFCoincFilter,&b_phfCoincFilter);

    
    if (fChain->GetBranch("etaMin"))
      fChain->SetBranchStatus("eta*",1);
    if (fChain->GetBranch("etaMin"))
      fChain->SetBranchAddress("etaMin", &fForestFJRho.etaMin, &fForestFJRho.b_etaMin);
    if (fChain->GetBranch("etaMax"))
    fChain->SetBranchAddress("etaMax", &fForestFJRho.etaMax, &fForestFJRho.b_etaMax);
    if (fChain->GetBranch("rho"))
      fChain->SetBranchStatus("rho*",1);
    if (fChain->GetBranch("rho"))
      fChain->SetBranchAddress("rho", &fForestFJRho.rho, &fForestFJRho.b_rho);
    if (fChain->GetBranch("rhom"))
      fChain->SetBranchAddress("rhom", &fForestFJRho.rhom, &fForestFJRho.b_rhom);
    
    fInit = kTRUE;
  }
  return kTRUE;
}
//__________________________________________________________
Bool_t hiEventProducer::InitEventObjects() {
  
  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(fhiEventContName)) {
      fhiEventContainer = new hiEventContainer(fhiEventContName);
      fEventObjects->Add(fhiEventContainer);
      Printf("Created hiEventContainer");
    }
  }

  if(!fRhoMap) {
    TString rhoName = Form("rho_%s",fhiEventContName.Data());
    fRhoMap = new rhoMap(rhoName.Data());
    fEventObjects->Add(fRhoMap);
    fhiEventContainer->SetRhoMap(fRhoMap);
  }
  
  return kTRUE;
}

//__________________________________________________________
Bool_t hiEventProducer::Run(Long64_t entry) {

  //run analysis
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;
 
  fhiEventContainer->SetRun(fRun);
  fhiEventContainer->SetEvent(fEvt);
  fhiEventContainer->SetLumi(fLumi);
  fhiEventContainer->SetWeight(fWeight);
  fhiEventContainer->SetVx(fVx);
  fhiEventContainer->SetVy(fVy);
  fhiEventContainer->SetVz(fVz);
  //Printf("fHiBin: %d",fHiBin);
  fhiEventContainer->SetHiBin(fHiBin);
  fhiEventContainer->SetHiHF(fHiHF);
  fhiEventContainer->SetNPV(fNPV);
  fhiEventContainer->SetPFJet80(fHLT_AK4PFJet80_Eta5p1_v1);
  fhiEventContainer->SetPhoton30(fHLT_HISinglePhoton30_Eta3p5_v1);
  fhiEventContainer->SetTrk24(fHLT_FullTrack24ForPPRef_v1);
  fhiEventContainer->SetTrk45(fHLT_FullTrack45ForPPRef_v1);
  fhiEventContainer->SetColl(fcollisionEvtSel);
  fhiEventContainer->SetHBHENoise(fHBHENoise);
  fhiEventContainer->SetHBHENoiseLoose(fHBHENoiseLoose);
  fhiEventContainer->SetPrimaryVertexFilter(fPrimaryVertexFilter);
  fhiEventContainer->SetClusterCompatibilityFilter(fClusterCompatibilityFilter);
  fhiEventContainer->SetHFCoincFilter(fHFCoincFilter);

  if(fRhoMap && fForestFJRho.etaMin) {
  fRhoMap->ClearMap();
  //Printf("create rho map. netaBins: %d",(int)fForestFJRho.etaMin->size());
  for(unsigned int ieta = 0; ieta<fForestFJRho.etaMin->size(); ++ieta) {
    //Printf("ieta: %d %f-%f rho: %f",(int)ieta,fForestFJRho.etaMin->at(ieta),fForestFJRho.etaMax->at(ieta),fForestFJRho.rho->at(ieta));
    fRhoMap->AddEtaRange(fForestFJRho.etaMin->at(ieta),fForestFJRho.etaMax->at(ieta),ieta,fForestFJRho.rho->at(ieta));
  }
  }

  return kTRUE; 
}

//__________________________________________________________
Long64_t hiEventProducer::LoadTree(Long64_t entry) {

  //overloaded LoadTree function
  //
  //Printf("hiEventProducer::LoadTree %d",(int)entry);
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }

  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
    //Printf("%lld fCurrent: %d",entry,fCurrent);
  }
 
  // fChain->SetMakeClass(1);
 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("hiEventProducer: centry smaller than 0");
    return centry;  
  }
 
  //Printf("get entry %d",(int)entry); 
  fChain->GetEntry(entry);
  
  return centry;  
}

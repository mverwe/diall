//
// producer of particle flow candidates
//

#include "UserCode/diall/interface/pfParticleProducerVector.h"

ClassImp(pfParticleProducerVector)

//__________________________________________________________
pfParticleProducerVector::pfParticleProducerVector() :
inputBase("pfParticleProducerVector"),
  fpfParticlesName("pfParticles"),
  fpfParticles(0x0),
  fPFs()
{
  //default constructor
}

//__________________________________________________________
pfParticleProducerVector::pfParticleProducerVector(const char *name) :
  inputBase(name),
  fpfParticlesName("pfParticles"),
  fpfParticles(0x0),
  fPFs()
{
  //standard constructor
}

//__________________________________________________________
Bool_t pfParticleProducerVector::Init() {

  if(!inputBase::Init()) return kFALSE;
  
  if(fInputMode==hiForest) {
    fChain->SetBranchStatus("*", 0);
    fChain->SetBranchStatus("nPFpart", 1);
    fChain->SetBranchStatus("pf*", 1);

    if (fChain->GetBranch("nPFpart"))
      fChain->SetBranchAddress("nPFpart", &fPFs.nPFpart, &fPFs.b_nPFpart);
    if (fChain->GetBranch("pfId"))
      fChain->SetBranchAddress("pfId", &fPFs.pfId, &fPFs.b_pfId);
    if (fChain->GetBranch("pfPt"))
      fChain->SetBranchAddress("pfPt", &fPFs.pfPt, &fPFs.b_pfPt);
    if (fChain->GetBranch("pfVsPtInitial"))
      fChain->SetBranchAddress("pfVsPtInitial", &fPFs.pfVsPt, &fPFs.b_pfVsPt);
    if (fChain->GetBranch("pfEta"))
      fChain->SetBranchAddress("pfEta", &fPFs.pfEta, &fPFs.b_pfEta);
    if (fChain->GetBranch("pfPhi"))
      fChain->SetBranchAddress("pfPhi", &fPFs.pfPhi, &fPFs.b_pfPhi);
    fInit = kTRUE;
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t pfParticleProducerVector::InitEventObjects() {
  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(fpfParticlesName)) {
      fpfParticles = new TClonesArray("pfParticle");
      fpfParticles->SetName(fpfParticlesName);
      fEventObjects->Add(fpfParticles);
    }
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t pfParticleProducerVector::Run(Long64_t entry) {

  //run analysis
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;
 
  //clear array
  fpfParticles->Delete();
  
  //put particles of this event in array
  Int_t pfCount = 0;
  for(Int_t i = 0; i<fPFs.nPFpart; i++) {
    Double_t mass = 0.;
    Int_t charge = 0;
    if(fPFs.pfId->at(i)==1) { //charged hadron, assume pion mass
      mass   = 0.13957;
      charge = 1;
    }
    //Printf("%s: i %d mass %f charge %d",GetName(),i,mass,charge);
    //float pt = fPFs.pfPt->at(i);
    //float eta = fPFs.pfEta->at(i);
    //float phi = fPFs.pfPhi->at(i);
    //Printf("%s: pt = %f eta = %f phi = %f",GetName(),pt,eta,phi);
    pfParticle *pPart = new ((*fpfParticles)[pfCount])
      pfParticle(fPFs.pfPt->at(i),
                 fPFs.pfEta->at(i),
                 fPFs.pfPhi->at(i),
                 mass,
                 fPFs.pfId->at(i));
    pPart->SetCharge(charge);
    pPart->SetPtVS(fPFs.pfVsPt->at(i));
    ++pfCount;
  }
  fpfParticles->Sort();

  return kTRUE;
}

//__________________________________________________________
Long64_t pfParticleProducerVector::LoadTree(Long64_t entry) {

  //overloaded LoadTree function 
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }
  
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
    //Printf("%s: %lld fCurrent: %d",GetName(),entry,fCurrent);
  }

  // fChain->SetMakeClass(1);
 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("pfParticleProducerVector: centry smaller than 0");
    return centry;  
  } 

  fChain->GetEntry(entry);
  
  return centry;
}

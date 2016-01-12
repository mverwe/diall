//
// producer of muon candidates
//

#include "UserCode/diall/interface/lwElectronProducer.h"
#include "UserCode/diall/interface/genParticle.h"
#include "UserCode/diall/interface/lwElectron.h"


ClassImp(lwElectronProducer)

//__________________________________________________________
lwElectronProducer::lwElectronProducer() :
inputBase("lwElectronProducer"),
  flwElectronsRecoName("lwElectronsReco"),
  flwElectronsReco(0x0),
  flwElectronsGeneName("lwElectronsGene"),
  flwElectronsGene(0x0),
  fElectrons(),
  fPtMin(10.),
  fdEtaAtVtxB(0.0094),
  fdEtaAtVtxE(0.00773),
  fdPhiAtVtxB(0.0296),
  fdPhiAtVtxE(0.148),
  fHoverEB(0.0372),
  fHoverEE(0.0546),
  fD0B(0.0151), 
  fD0E(0.0535),
  fDZB(0.238),
  fDZE(0.572),
  fSigmaIEtaIEtaB(0.0101),
  fSigmaIEtaIEtaE(0.0287),
  fEoverPInvB(0.118),
  fEoverPInvE(0.104)
{
  //default constructor
}

//__________________________________________________________
lwElectronProducer::lwElectronProducer(const char *name) :
  inputBase(name),
  flwElectronsRecoName("lwElectronsReco"),
  flwElectronsReco(0x0),
  flwElectronsGeneName("lwElectronsGene"),
  flwElectronsGene(0x0),
  fElectrons(),
  fPtMin(16.)
{
  //standard constructor
}

//__________________________________________________________
Bool_t lwElectronProducer::Init() {

  if(!inputBase::Init()) return kFALSE;
  
  if(fInputMode==hiForest) {
    // Reco Info
    fChain->SetBranchStatus("*", 0);
    fChain->SetBranchStatus("nEle", 1);
    fChain->SetBranchStatus("ele*", 1);
    if (fChain->GetBranch("nEle"))
      fChain->SetBranchAddress("nEle", &fElectrons.nEle, &fElectrons.b_nEle);
    if (fChain->GetBranch("eleCharge"))
      fChain->SetBranchAddress("eleCharge", &fElectrons.eleCharge, &fElectrons.b_eleCharge);
    if (fChain->GetBranch("elePt"))
      fChain->SetBranchAddress("elePt", &fElectrons.elePt, &fElectrons.b_elePt);
    if (fChain->GetBranch("eleEta"))
      fChain->SetBranchAddress("eleEta", &fElectrons.eleEta, &fElectrons.b_eleEta);
    if (fChain->GetBranch("elePhi"))
      fChain->SetBranchAddress("elePhi", &fElectrons.elePhi, &fElectrons.b_elePhi);
   if (fChain->GetBranch("eledEtaAtVtx"))
      fChain->SetBranchAddress("eledEtaAtVtx", &fElectrons.eledEtaAtVtx, &fElectrons.b_eledEtaAtVtx);
    if (fChain->GetBranch("eledPhiAtVtx"))
      fChain->SetBranchAddress("eledPhiAtVtx", &fElectrons.eledPhiAtVtx, &fElectrons.b_eledPhiAtVtx);
    if (fChain->GetBranch("eleD0"))
      fChain->SetBranchAddress("eleD0", &fElectrons.eleD0, &fElectrons.b_eleD0);
    if (fChain->GetBranch("eleDz"))
      fChain->SetBranchAddress("eleDz", &fElectrons.eleDz, &fElectrons.b_eleDz);
    if (fChain->GetBranch("eleSigmaIEtaIEta"))
      fChain->SetBranchAddress("eleSigmaIEtaIEta", &fElectrons.eleSigmaIEtaIEta, &fElectrons.b_eleSigmaIEtaIEta);
    if (fChain->GetBranch("eleHoverE"))
      fChain->SetBranchAddress("eleHoverE", &fElectrons.eleHoverE, &fElectrons.b_eleHoverE);
    if (fChain->GetBranch("eleEoverP"))
      fChain->SetBranchAddress("eleEoverP", &fElectrons.eleEoverP, &fElectrons.b_eleEoverP);
    if (fChain->GetBranch("eleEoverPInv"))
      fChain->SetBranchAddress("eleEoverPInv", &fElectrons.eleEoverPInv, &fElectrons.b_eleEoverPInv);
    if (fChain->GetBranch("eleMissHits"))
      fChain->SetBranchAddress("eleMissHits", &fElectrons.eleMissHits, &fElectrons.b_eleMissHits);
 
    fInit = kTRUE;
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t lwElectronProducer::InitEventObjects() {

  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(flwElectronsRecoName)) {
      flwElectronsReco = new TClonesArray("lwElectron");
      flwElectronsReco->SetName(flwElectronsRecoName);
      fEventObjects->Add(flwElectronsReco);
    }
    if(!fEventObjects->FindObject(flwElectronsGeneName) && !flwElectronsGeneName.IsNull()) {
      flwElectronsGene = new TClonesArray("genParticle");
      flwElectronsGene->SetName(flwElectronsGeneName);
      fEventObjects->Add(flwElectronsGene);
    }
  }
  
  return kTRUE;
}

//__________________________________________________________
Bool_t lwElectronProducer::Run(Long64_t entry) {

  //overloaded run funtion
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;
  
  //clear arrays
  flwElectronsReco->Delete();
  if(flwElectronsGene) flwElectronsGene->Delete();

  //reconstructed electrons
  Int_t eleCount = 0;
  for(Int_t i = 0; i<fElectrons.nEle; i++) {
    if(!AcceptElectron(i)) continue;
    lwElectron *el = new lwElectron(fElectrons.elePt->at(i),
                            fElectrons.eleEta->at(i),
                            fElectrons.elePhi->at(i),
                            0,
                            i);
    el->SetCharge(fElectrons.eleCharge->at(i));
    (*flwElectronsReco)[eleCount] = el;
    ++eleCount;
  }
  flwElectronsReco->Sort();

  return kTRUE;
}

//__________________________________________________________
Bool_t lwElectronProducer::AcceptElectron(Int_t i) {

  //electron quality selection 
  //https://github.com/CmsHI/quickZMacros/blob/master/ggHistos.C#L4
 
  if((fElectrons.elePt->at(i))<fPtMin) return false;
  if(fabs(fElectrons.eleEta->at(i))>2.4) return false;
  if(fabs(fElectrons.eleEta->at(i))<1.479) {
   if(  fabs(fElectrons.eledEtaAtVtx->at(i))<fdEtaAtVtxB //0.0094
	&& fabs(fElectrons.eledPhiAtVtx->at(i))<fdPhiAtVtxB //0.0296
	&& fElectrons.eleSigmaIEtaIEta->at(i)<fSigmaIEtaIEtaB //0.0101
	&& fElectrons.eleHoverE->at(i)<fHoverEB //0.0372
	&& fabs(fElectrons.eleD0->at(i))<fD0B //0.0151
	&& fabs(fElectrons.eleDz->at(i))<fDZB //0.238
	&& fabs(fElectrons.eleEoverPInv->at(i))<fEoverPInvB //0.118
	&& fElectrons.eleMissHits->at(i) <= 2
	) return true;
   else return false;
  }
  if(fabs(fElectrons.eleSCEta->at(i))>1.479) {
   if(  fabs(fElectrons.eledEtaAtVtx->at(i))<fdEtaAtVtxE //0.00773
	&& fabs(fElectrons.eledPhiAtVtx->at(i))<fdPhiAtVtxE //0.148
	&& fElectrons.eleSigmaIEtaIEta->at(i)<fSigmaIEtaIEtaE //0.0287
	&& fElectrons.eleHoverE->at(i)<fHoverEE //0.0546
	&& fabs(fElectrons.eleD0->at(i))<fD0E //0.0535
	&& fabs(fElectrons.eleDz->at(i))<fDZE //0.572
	&& fabs(fElectrons.eleEoverPInv->at(i))<fEoverPInvE //0.104
	&& fElectrons.eleMissHits->at(i) <= 1
	) return true;
   else return false;
  }
  return false;
}

//__________________________________________________________
Long64_t lwElectronProducer::LoadTree(Long64_t entry) {

  //overloaded LoadTree function 
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }
  
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
    Printf("%lld fCurrent: %d",entry,fCurrent);
  }

  //  fChain->SetMakeClass(1);
 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("lwElectronProducer: centry smaller than 0");
    return centry;  
  }
  
  fChain->GetEntry(entry);

  return centry;
}

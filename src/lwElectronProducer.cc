//
// producer of electron candidates
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
  fPtMin(18.), 
  fMaxEtaAbs(2.4),
  fMaxdEtaAtVtxBarrel(0.0094),  
  fMaxdPhiAtVtxBarrel(0.0296),
  fMaxSigmaIEtaIEtaBarrel(0.0101),
  fMaxHoverEBarrel(0.0372),
  fMaxDxyBarrel(0.0151),
  fMaxDzBarrel(0.238),
  fMaxEoverPInvBarrel(0.118),
  fMaxMissHitsBarrel(2),
  fPassConversionVetoBarrel(kTRUE),
  fMaxdEtaAtVtxEndcap(0.00773),  
  fMaxdPhiAtVtxEndcap(0.148),
  fMaxSigmaIEtaIEtaEndcap(0.0287),
  fMaxHoverEEndcap(0.0546),
  fMaxDxyEndcap(0.0535),
  fMaxDzEndcap(0.572),
  fMaxEoverPInvEndcap(0.104),
  fMaxMissHitsEndcap(1),
  fPassConversionVetoEndcap(kTRUE)

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
  fPtMin(16.),
  fMaxEtaAbs(2.1),
  fMaxdEtaAtVtxBarrel(0.0094),  
  fMaxdPhiAtVtxBarrel(0.0296),
  fMaxSigmaIEtaIEtaBarrel(0.0101),
  fMaxHoverEBarrel(0.0372),
  fMaxDxyBarrel(0.0151),
  fMaxDzBarrel(0.238),
  fMaxEoverPInvBarrel(0.118),
  fMaxMissHitsBarrel(2),
  fPassConversionVetoBarrel(kTRUE),
  fMaxdEtaAtVtxEndcap(0.00773),  
  fMaxdPhiAtVtxEndcap(0.148),
  fMaxSigmaIEtaIEtaEndcap(0.0287),
  fMaxHoverEEndcap(0.0546),
  fMaxDxyEndcap(0.0535),
  fMaxDzEndcap(0.572),
  fMaxEoverPInvEndcap(0.104),
  fMaxMissHitsEndcap(1),
  fPassConversionVetoEndcap(kTRUE)
{
  //standard constructor
}

//__________________________________________________________
Bool_t lwElectronProducer::Init() {

  if(!inputBase::Init()) return kFALSE;
  
  if(fInputMode==hiForest) {
    // Gen Info
    /*
    if (fChain->GetBranch("mult"))
      fChain->SetBranchAddress("mult", &fElectrons.Gen_nptl, &fElectrons.b_Gen_nptl);
    if (fChain->GetBranch("pdg"))
      fChain->SetBranchAddress("pdg", &fElectrons.Gen_pid, &fElectrons.b_Gen_pid);
    //if (fChain->GetBranch("Gen_mom"))
    // fChain->SetBranchAddress("Gen_mom", &fElectrons.Gen_mom, &fElectrons.b_Gen_mom);
    if (fChain->GetBranch("pt"))
      fChain->SetBranchAddress("pt", &fElectrons.Gen_pt, &fElectrons.b_Gen_pt);
    if (fChain->GetBranch("eta"))
      fChain->SetBranchAddress("eta", &fElectrons.Gen_eta, &fElectrons.b_Gen_eta);
    if (fChain->GetBranch("phi"))
      fChain->SetBranchAddress("phi", &fElectrons.Gen_phi, &fElectrons.b_Gen_phi);
    */
    // Reco Info
    //fChain->SetBranchStatus("*", 0);
    fChain->SetBranchStatus("nEle",1);     // enable electron branches
    fChain->SetBranchStatus("ele*",1);     // enable electron branches
    if (fChain->GetBranch("nEle")){    printf("mpika!!!!! ");
      fChain->SetBranchAddress("nEle", &fElectrons.nEle, &fElectrons.b_nEle); }
    if (fChain->GetBranch("eleEn"))
      fChain->SetBranchAddress("eleEn", &fElectrons.eleEn, &fElectrons.b_eleEn);
    if (fChain->GetBranch("eleCharge"))
      fChain->SetBranchAddress("eleCharge", &fElectrons.eleCharge, &fElectrons.b_eleCharge);
    if (fChain->GetBranch("elePt"))
      fChain->SetBranchAddress("elePt", &fElectrons.elePt, &fElectrons.b_elePt);
    if (fChain->GetBranch("eleEta"))
      fChain->SetBranchAddress("eleEta", &fElectrons.eleEta, &fElectrons.b_eleEta);
    if (fChain->GetBranch("elePhi"))
      fChain->SetBranchAddress("elePhi", &fElectrons.elePhi, &fElectrons.b_elePhi);
    if (fChain->GetBranch("eleSCEn"))
      fChain->SetBranchAddress("eleSCEn", &fElectrons.eleSCEn, &fElectrons.b_eleSCEn);
    if (fChain->GetBranch("eleESEn"))
      fChain->SetBranchAddress("eleESEn", &fElectrons.eleESEn, &fElectrons.b_eleESEn);
    if (fChain->GetBranch("eleSCEta"))
      fChain->SetBranchAddress("eleSCEta", &fElectrons.eleSCEta, &fElectrons.b_eleSCEta);
    if (fChain->GetBranch("eleSCPhi"))
      fChain->SetBranchAddress("eleSCPhi", &fElectrons.eleSCPhi, &fElectrons.b_eleSCPhi);
    if (fChain->GetBranch("eleSCRawEn"))
      fChain->SetBranchAddress("eleSCRawEn", &fElectrons.eleSCRawEn, &fElectrons.b_eleSCRawEn);
    if (fChain->GetBranch("eleSCEtaWidth"))
      fChain->SetBranchAddress("eleSCEtaWidth", &fElectrons.eleSCEtaWidth, &fElectrons.b_eleSCEtaWidth);
    if (fChain->GetBranch("eleSCPhiWidth"))
      fChain->SetBranchAddress("eleSCPhiWidth", &fElectrons.eleSCPhiWidth, &fElectrons.b_eleSCPhiWidth);
    if (fChain->GetBranch("eleHoverE"))
      fChain->SetBranchAddress("eleHoverE", &fElectrons.eleHoverE, &fElectrons.b_eleHoverE);
    if (fChain->GetBranch("eleSigmaIEtaIEta"))
      fChain->SetBranchAddress("eleSigmaIEtaIEta", &fElectrons.eleSigmaIEtaIEta, &fElectrons.b_eleSigmaIEtaIEta);
    if (fChain->GetBranch("eleSigmaIEtaIEta_2012"))
      fChain->SetBranchAddress("eleSigmaIEtaIEta_2012", &fElectrons.eleSigmaIEtaIEta_2012, &fElectrons.b_eleSigmaIEtaIEta_2012);
    if (fChain->GetBranch("eleSigmaIPhiIPhi"))
      fChain->SetBranchAddress("eleSigmaIPhiIPhi", &fElectrons.eleSigmaIPhiIPhi, &fElectrons.b_eleSigmaIPhiIPhi);
    if (fChain->GetBranch("eleEoverPInv"))
      fChain->SetBranchAddress("eleEoverPInv", &fElectrons.eleEoverPInv, &fElectrons.b_eleEoverPInv);
    if (fChain->GetBranch("eledEtaAtVtx"))
      fChain->SetBranchAddress("eledEtaAtVtx", &fElectrons.eledEtaAtVtx, &fElectrons.b_eledEtaAtVtx);
    if (fChain->GetBranch("eledPhiAtVtx"))
      fChain->SetBranchAddress("eledPhiAtVtx", &fElectrons.eledPhiAtVtx, &fElectrons.b_eledPhiAtVtx);
    if (fChain->GetBranch("eleD0"))
      fChain->SetBranchAddress("eleD0", &fElectrons.eleD0, &fElectrons.b_eleD0);
    if (fChain->GetBranch("eleDz"))
      fChain->SetBranchAddress("eleDz", &fElectrons.eleDz, &fElectrons.b_eleDz);
    if (fChain->GetBranch("eleD0Err"))
      fChain->SetBranchAddress("eleD0Err", &fElectrons.eleD0Err, &fElectrons.b_eleD0Err);
    if (fChain->GetBranch("eleDzErr"))
      fChain->SetBranchAddress("eleDzErr", &fElectrons.eleDzErr, &fElectrons.b_eleDzErr);
    if (fChain->GetBranch("eleMissHits"))
      fChain->SetBranchAddress("eleMissHits", &fElectrons.eleMissHits, &fElectrons.b_eleMissHits);
    if (fChain->GetBranch("elePFChIso"))
      fChain->SetBranchAddress("elePFChIso", &fElectrons.elePFChIso, &fElectrons.b_elePFChIso);
    if (fChain->GetBranch("elePFPhoIso"))
      fChain->SetBranchAddress("elePFPhoIso", &fElectrons.elePFPhoIso, &fElectrons.b_elePFPhoIso);
    if (fChain->GetBranch("elePFNeuIso"))
      fChain->SetBranchAddress("elePFNeuIso", &fElectrons.elePFNeuIso, &fElectrons.b_elePFNeuIso);
    if (fChain->GetBranch("elePFPUIso"))
      fChain->SetBranchAddress("elePFPUIso", &fElectrons.elePFPUIso, &fElectrons.b_elePFPUIso);
        
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
    lwElectron *ele = new lwElectron(fElectrons.elePt->at(i),
				     fElectrons.eleEta->at(i),
				     fElectrons.elePhi->at(i),
				     0,
				     i);
    ele->SetCharge(fElectrons.eleCharge->at(i));
    (*flwElectronsReco)[eleCount] = ele;
    ++eleCount;
  }
  flwElectronsReco->Sort();
  

  //generated electrons
  if(flwElectronsGene) {
    eleCount = 0;
    for(Int_t i = 0; i<fElectrons.Gen_nptl; i++) {
      genParticle *ele = new genParticle(fElectrons.Gen_pt[i],
					 fElectrons.Gen_eta[i],
					 fElectrons.Gen_phi[i],
					 0,
					 i);
      ele->SetCharge(fElectrons.Gen_pid[i]/abs(fElectrons.Gen_pid[i]));
      ele->SetPID(fElectrons.Gen_pid[i]);
      ele->SetPIDMom(fElectrons.Gen_mom[i]);
      (*flwElectronsGene)[eleCount] = ele;
      ++eleCount;
    }
    flwElectronsGene->Sort();
    //Printf("%d generated electrons",eleCount);
  }
  
  return kTRUE;
}

//__________________________________________________________
Bool_t lwElectronProducer::AcceptElectron(Int_t i) {

  //electron quality selection 
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns (medium WP)
  //no isolation for the moment!!!

  if(fabs(fElectrons.eleSCEta->at(i))<1.479) {
    if(  fabs(fElectrons.eledEtaAtVtx->at(i))<fMaxdEtaAtVtxBarrel
	 && fabs(fElectrons.eledPhiAtVtx->at(i))<fMaxdPhiAtVtxBarrel
	 && fElectrons.eleSigmaIEtaIEta->at(i)<fMaxSigmaIEtaIEtaBarrel
	 && fElectrons.eleHoverE->at(i)< fMaxHoverEBarrel
	 && fabs(fElectrons.eleD0->at(i))<fMaxDxyBarrel
	 && fabs(fElectrons.eleDz->at(i))<fMaxDzBarrel
	 && fabs(fElectrons.eleEoverPInv->at(i))<fMaxEoverPInvBarrel
	 && fElectrons.eleMissHits->at(i) <= fMaxMissHitsBarrel
	 ) return kTRUE;
    else return kFALSE;
  }
  if(fabs(fElectrons.eleSCEta->at(i))>1.479) {
    if(  fabs(fElectrons.eledEtaAtVtx->at(i))<fMaxdEtaAtVtxEndcap
	 && fabs(fElectrons.eledPhiAtVtx->at(i))<fMaxdPhiAtVtxEndcap
	 && fElectrons.eleSigmaIEtaIEta->at(i)<fMaxSigmaIEtaIEtaEndcap
	 && fElectrons.eleHoverE->at(i)<fMaxHoverEEndcap
	 && fabs(fElectrons.eleD0->at(i))<fMaxDxyEndcap
	 && fabs(fElectrons.eleDz->at(i))<fMaxDzEndcap
	 && fabs(fElectrons.eleEoverPInv->at(i))<fMaxEoverPInvEndcap
	 && fElectrons.eleMissHits->at(i) <= fMaxMissHitsEndcap
	 ) return kTRUE;
    else return kFALSE;
  }
  else return kFALSE;

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
    Printf("hiEventProducer: centry smaller than 0");
    return centry;  
  }
  //fChain->SetNotify(0); 
  fChain->GetEntry(entry); //Line that gives the brake!!

  
  return centry;
}


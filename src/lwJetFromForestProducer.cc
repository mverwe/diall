//
// producer of particle flow candidates
//

#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/lwJet.h"

#include <iostream>
ClassImp(lwJetFromForestProducer)

//__________________________________________________________
lwJetFromForestProducer::lwJetFromForestProducer() :
inputBase("lwJetFromForestProducer"),
  flwJetContName("flwJetCont"),
  flwJetContainer(0x0),
  flwGenJetContName("flwGenJetCont"),
  flwGenJetContainer(0x0),
  fForestJets(),
  fRadius(-1.),
  fMinJetPt(-1.),
  fDoPFJetID(false)
{
  //default constructor
}

//__________________________________________________________
lwJetFromForestProducer::lwJetFromForestProducer(const char *name) :
  inputBase(name),
  flwJetContName("flwJetCont"),
  flwJetContainer(0x0),
  flwGenJetContName("flwGenJetCont"),
  flwGenJetContainer(0x0),
  fForestJets(),
  fRadius(-1.),
  fMinJetPt(-1.),
  fDoPFJetID(false)
{
  //standard constructor
}

//__________________________________________________________
Bool_t lwJetFromForestProducer::Init() {

  if(!inputBase::Init()) return kFALSE;
  
  if(fInputMode==hiForest) {
    fChain->SetBranchStatus("nref",1); // enable jet branches
    if (fChain->GetBranch("nref"))
      fChain->SetBranchAddress("nref", &fForestJets.nref, &fForestJets.b_nref);
    if (fChain->GetBranch("rawpt")){
      fChain->SetBranchStatus("rawpt",1);
      fChain->SetBranchAddress("rawpt", &fForestJets.rawpt, &fForestJets.b_rawpt);}
    fChain->SetBranchStatus("jt*",1);
    if (fChain->GetBranch("jtpt")) {
      fChain->SetBranchAddress("jtpt", &fForestJets.jtpt, &fForestJets.b_jtpt); }
    if (fChain->GetBranch("jteta")){
      fChain->SetBranchAddress("jteta", &fForestJets.jteta, &fForestJets.b_jteta); }
    if (fChain->GetBranch("jty")){
      fChain->SetBranchAddress("jty", &fForestJets.jty, &fForestJets.b_jty);}
    if (fChain->GetBranch("jtphi")){
      fChain->SetBranchAddress("jtphi", &fForestJets.jtphi, &fForestJets.b_jtphi);}
    if (fChain->GetBranch("jtpu")){
      fChain->SetBranchAddress("jtpu", &fForestJets.jtpu, &fForestJets.b_jtpu);}
    if (fChain->GetBranch("jtm")){
      fChain->SetBranchAddress("jtm", &fForestJets.jtm, &fForestJets.b_jtm);}
    if (fChain->GetBranch("jtarea"))
      fChain->SetBranchAddress("jtarea", &fForestJets.jtarea, &fForestJets.b_jtarea);
    if (fChain->GetBranch("jtPfCHF")){
      fChain->SetBranchAddress("jtPfCHF", &fForestJets.jtPfCHF, &fForestJets.b_jtPfCHF);}
    if (fChain->GetBranch("jtPfNHF")){
      fChain->SetBranchAddress("jtPfNHF", &fForestJets.jtPfNHF, &fForestJets.b_jtPfNHF);}
    if (fChain->GetBranch("jtPfCEF")){
      fChain->SetBranchAddress("jtPfCEF", &fForestJets.jtPfCEF, &fForestJets.b_jtPfCEF);}
    if (fChain->GetBranch("jtPfNEF")){
      fChain->SetBranchAddress("jtPfNEF", &fForestJets.jtPfNEF, &fForestJets.b_jtPfNEF);}
    if (fChain->GetBranch("jtPfMUF")){
      fChain->SetBranchAddress("jtPfMUF", &fForestJets.jtPfMUF, &fForestJets.b_jtPfMUF);}
    if (fChain->GetBranch("jtPfCHM")){
      fChain->SetBranchAddress("jtPfCHM", &fForestJets.jtPfCHM, &fForestJets.b_jtPfCHM);}
    if (fChain->GetBranch("jtPfNHM")){
      fChain->SetBranchAddress("jtPfNHM", &fForestJets.jtPfNHM, &fForestJets.b_jtPfNHM);}
    if (fChain->GetBranch("jtPfCEM")){
      fChain->SetBranchAddress("jtPfCEM", &fForestJets.jtPfCEM, &fForestJets.b_jtPfCEM);}
    if (fChain->GetBranch("jtPfNEM")){
      fChain->SetBranchAddress("jtPfNEM", &fForestJets.jtPfNEM, &fForestJets.b_jtPfNEM);}
    if (fChain->GetBranch("jtPfMUM")){
      fChain->SetBranchAddress("jtPfMUM", &fForestJets.jtPfMUM, &fForestJets.b_jtPfMUM);}
    if (fChain->GetBranch("jttau1"))
      fChain->SetBranchAddress("jttau1", &fForestJets.jttau1, &fForestJets.b_jttau1);
    if (fChain->GetBranch("jttau2"))
      fChain->SetBranchAddress("jttau2", &fForestJets.jttau2, &fForestJets.b_jttau2);
    if (fChain->GetBranch("jttau3"))
      fChain->SetBranchAddress("jttau3", &fForestJets.jttau3, &fForestJets.b_jttau3);
    if (fChain->GetBranch("jtSubJetPt"))
      fChain->SetBranchAddress("jtSubJetPt", &fForestJets.jtSubJetPt, &fForestJets.b_jtSubJetPt);
    if (fChain->GetBranch("jtSubJetEta"))
      fChain->SetBranchAddress("jtSubJetEta", &fForestJets.jtSubJetEta, &fForestJets.b_jtSubJetEta);
    if (fChain->GetBranch("jtSubJetPhi"))
      fChain->SetBranchAddress("jtSubJetPhi", &fForestJets.jtSubJetPhi, &fForestJets.b_jtSubJetPhi);
    if (fChain->GetBranch("jtSubJetM"))
      fChain->SetBranchAddress("jtSubJetM", &fForestJets.jtSubJetM, &fForestJets.b_jtSubJetM);
    if (fChain->GetBranch("jtdroppedBranches"))
      fChain->SetBranchAddress("jtdroppedBranches",&fForestJets.jtdroppedBranches,&fForestJets.b_jtdroppedBranches);
        
    if (fChain->GetBranch("ref*")) fChain->SetBranchStatus("ref*",1);
    if (fChain->GetBranch("refpt")){
      fChain->SetBranchAddress("refpt", &fForestJets.refpt, &fForestJets.b_refpt);}
    if (fChain->GetBranch("refeta"))
      fChain->SetBranchAddress("refeta", &fForestJets.refeta, &fForestJets.b_refeta);
    if (fChain->GetBranch("refphi"))
      fChain->SetBranchAddress("refphi", &fForestJets.refphi, &fForestJets.b_refphi);
    if (fChain->GetBranch("refm"))
      fChain->SetBranchAddress("refm", &fForestJets.refm, &fForestJets.b_refm);
    if (fChain->GetBranch("reftau1"))
      fChain->SetBranchAddress("reftau1", &fForestJets.reftau1, &fForestJets.b_reftau1);
    if (fChain->GetBranch("reftau2"))
      fChain->SetBranchAddress("reftau2", &fForestJets.reftau2, &fForestJets.b_reftau2);
    if (fChain->GetBranch("reftau3"))
      fChain->SetBranchAddress("reftau3", &fForestJets.reftau3, &fForestJets.b_reftau3);
    if (fChain->GetBranch("refdrjt"))
      fChain->SetBranchAddress("refdrjt", &fForestJets.refdrjt, &fForestJets.b_refdrjt);
    if (fChain->GetBranch("subid"))
      fChain->SetBranchAddress("subid", &fForestJets.subid, &fForestJets.b_subid);
    fChain->SetBranchStatus("*Max",1);
    if (fChain->GetBranch("trackMax"))
      fChain->SetBranchAddress("trackMax", &fForestJets.trackMax, &fForestJets.b_trackMax);
    if (fChain->GetBranch("chargedMax"))
      fChain->SetBranchAddress("chargedMax", &fForestJets.chargedMax, &fForestJets.b_chargedMax);
    if (fChain->GetBranch("photonMax"))
      fChain->SetBranchAddress("photonMax", &fForestJets.photonMax, &fForestJets.b_photonMax);
    if (fChain->GetBranch("neutralMax"))
      fChain->SetBranchAddress("neutralMax", &fForestJets.neutralMax, &fForestJets.b_neutralMax);
    if (fChain->GetBranch("eMax"))
      fChain->SetBranchAddress("eMax", &fForestJets.eMax, &fForestJets.b_eMax);
    if (fChain->GetBranch("muMax"))
      fChain->SetBranchAddress("muMax", &fForestJets.muMax, &fForestJets.b_muMax);
    fChain->SetBranchStatus("*Sum",1);
    if (fChain->GetBranch("chargedSum"))
      fChain->SetBranchAddress("chargedSum", &fForestJets.chargedSum, &fForestJets.b_chargedSum);
    if (fChain->GetBranch("chargedHardSum"))
      fChain->SetBranchAddress("chargedHardSum", &fForestJets.chargedHardSum, &fForestJets.b_chargedHardSum);
    if (fChain->GetBranch("photonSum"))
      fChain->SetBranchAddress("photonSum", &fForestJets.photonSum, &fForestJets.b_photonSum);
    if (fChain->GetBranch("neutralSum"))
      fChain->SetBranchAddress("neutralSum", &fForestJets.neutralSum, &fForestJets.b_neutralSum);
    if (fChain->GetBranch("eSum"))
      fChain->SetBranchAddress("eSum", &fForestJets.eSum, &fForestJets.b_eSum);
    if (fChain->GetBranch("muSum"))
      fChain->SetBranchAddress("muSum", &fForestJets.muSum, &fForestJets.b_muSum);
    fChain->SetBranchStatus("*N",1);
    if (fChain->GetBranch("trackN"))
      fChain->SetBranchAddress("trackN", &fForestJets.trackN, &fForestJets.b_trackN);
    if (fChain->GetBranch("chargedN"))
      fChain->SetBranchAddress("chargedN", &fForestJets.chargedN, &fForestJets.b_chargedN);
    if (fChain->GetBranch("photonN"))
      fChain->SetBranchAddress("photonN", &fForestJets.photonN, &fForestJets.b_photonN);
    if (fChain->GetBranch("neutralN"))
      fChain->SetBranchAddress("neutralN", &fForestJets.neutralN, &fForestJets.b_neutralN);
    if (fChain->GetBranch("eN"))
      fChain->SetBranchAddress("eN", &fForestJets.eN, &fForestJets.b_eN);
    if (fChain->GetBranch("muN"))
      fChain->SetBranchAddress("muN", &fForestJets.muN, &fForestJets.b_muN);
    fChain->SetBranchStatus("discr_*",1);
    if (fChain->GetBranch("discr_ssvHighEff"))
      fChain->SetBranchAddress("discr_ssvHighEff", &fForestJets.discr_ssvHighEff, &fForestJets.b_discr_ssvHighEff);
    if (fChain->GetBranch("discr_ssvHighPur"))
      fChain->SetBranchAddress("discr_ssvHighPur", &fForestJets.discr_ssvHighPur, &fForestJets.b_discr_ssvHighPur);
    if (fChain->GetBranch("discr_csvMva"))
      fChain->SetBranchAddress("discr_csvMva", &fForestJets.discr_csvMva, &fForestJets.b_discr_csvMva);
    if (fChain->GetBranch("discr_csvSimple"))
      fChain->SetBranchAddress("discr_csvSimple", &fForestJets.discr_csvSimple, &fForestJets.b_discr_csvSimple);
    if (fChain->GetBranch("refparton_flavor"))
      fChain->SetBranchAddress("refparton_flavor", &fForestJets.refparton_flavor, &fForestJets.b_refparton_flavor);
    if (fChain->GetBranch("refparton_flavorForB"))
      fChain->SetBranchAddress("refparton_flavorForB", &fForestJets.refparton_flavorForB, &fForestJets.b_refparton_flavorForB);

    if (fChain->GetBranch("refptG"))
      fChain->SetBranchAddress("refptG", &fForestJets.refptG, &fForestJets.b_refptG);
    if (fChain->GetBranch("refetaG"))
      fChain->SetBranchAddress("refetaG", &fForestJets.refetaG, &fForestJets.b_refetaG);
    if (fChain->GetBranch("refphiG"))
      fChain->SetBranchAddress("refphiG", &fForestJets.refphiG, &fForestJets.b_refphiG);
    if (fChain->GetBranch("refmG"))
      fChain->SetBranchAddress("refmG", &fForestJets.refmG, &fForestJets.b_refmG);
    if (fChain->GetBranch("refSubJetPt"))
      fChain->SetBranchAddress("refSubJetPt", &fForestJets.refSubJetPt, &fForestJets.b_refSubJetPt);
    if (fChain->GetBranch("refSubJetEta"))
      fChain->SetBranchAddress("refSubJetEta", &fForestJets.refSubJetEta, &fForestJets.b_refSubJetEta);
    if (fChain->GetBranch("refSubJetPhi"))
      fChain->SetBranchAddress("refSubJetPhi", &fForestJets.refSubJetPhi, &fForestJets.b_refSubJetPhi);
    if (fChain->GetBranch("refSubJetM"))
      fChain->SetBranchAddress("refSubJetM", &fForestJets.refSubJetM, &fForestJets.b_refSubJetM);
    if (fChain->GetBranch("refdroppedBranches"))
      fChain->SetBranchAddress("refdroppedBranches",&fForestJets.refdroppedBranches,&fForestJets.b_refdroppedBranches);
    
    if (fChain->GetBranch("ngen")) fChain->SetBranchStatus("ngen",1);
    if (fChain->GetBranch("gen*")) fChain->SetBranchStatus("gen*",1);
    if (fChain->GetBranch("ngen"))
      fChain->SetBranchAddress("ngen", &fForestJets.ngen, &fForestJets.b_ngen);
    if (fChain->GetBranch("genpt"))
      fChain->SetBranchAddress("genpt", &fForestJets.genpt, &fForestJets.b_genpt);
    if (fChain->GetBranch("geneta"))
      fChain->SetBranchAddress("geneta", &fForestJets.geneta, &fForestJets.b_geneta);
    if (fChain->GetBranch("geny"))
      fChain->SetBranchAddress("geny", &fForestJets.geny, &fForestJets.b_geny);
    if (fChain->GetBranch("genphi"))
      fChain->SetBranchAddress("genphi", &fForestJets.genphi, &fForestJets.b_genphi);
    if (fChain->GetBranch("genm"))
      fChain->SetBranchAddress("genm", &fForestJets.genm, &fForestJets.b_genm);
    if (fChain->GetBranch("gentau1"))
      fChain->SetBranchAddress("gentau1", &fForestJets.gentau1, &fForestJets.b_gentau1);
    if (fChain->GetBranch("gentau2"))
      fChain->SetBranchAddress("gentau2", &fForestJets.gentau2, &fForestJets.b_gentau2);
    if (fChain->GetBranch("gentau3"))
      fChain->SetBranchAddress("gentau3", &fForestJets.gentau3, &fForestJets.b_gentau3);
    if (fChain->GetBranch("gensubid"))
      fChain->SetBranchAddress("gensubid", &fForestJets.gensubid, &fForestJets.b_gensubid);
    if (fChain->GetBranch("genmatchindex"))
      fChain->SetBranchAddress("genmatchindex", &fForestJets.genmatchindex, &fForestJets.b_genmatchindex);
    if (fChain->GetBranch("genptG"))
      fChain->SetBranchAddress("genptG", &fForestJets.genptG, &fForestJets.b_genptG);
    if (fChain->GetBranch("genetaG"))
      fChain->SetBranchAddress("genetaG", &fForestJets.genetaG, &fForestJets.b_genetaG);
    if (fChain->GetBranch("genphiG"))
      fChain->SetBranchAddress("genphiG", &fForestJets.genphiG, &fForestJets.b_genphiG);
    if (fChain->GetBranch("genmG"))
      fChain->SetBranchAddress("genmG", &fForestJets.genmG, &fForestJets.b_genmG);
    if (fChain->GetBranch("genSubJetPt"))
      fChain->SetBranchAddress("genSubJetPt", &fForestJets.genSubJetPt, &fForestJets.b_genSubJetPt);
    if (fChain->GetBranch("genSubJetEta"))
      fChain->SetBranchAddress("genSubJetEta", &fForestJets.genSubJetEta, &fForestJets.b_genSubJetEta);
    if (fChain->GetBranch("genSubJetPhi"))
      fChain->SetBranchAddress("genSubJetPhi", &fForestJets.genSubJetPhi, &fForestJets.b_genSubJetPhi);
    if (fChain->GetBranch("genSubJetM"))
      fChain->SetBranchAddress("genSubJetM", &fForestJets.genSubJetM, &fForestJets.b_genSubJetM);

    
    fInit = kTRUE;
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t lwJetFromForestProducer::InitEventObjects() {
  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(flwJetContName) && !flwJetContName.IsNull()) {
      flwJetContainer = new lwJetContainer(flwJetContName);
      flwJetContainer->Init();
      flwJetContainer->SetJetRadius(fRadius);
      fEventObjects->Add(flwJetContainer);
    }
    if(!fEventObjects->FindObject(flwGenJetContName) && !flwGenJetContName.IsNull()) {
      flwGenJetContainer = new lwJetContainer(flwGenJetContName);
      flwGenJetContainer->Init();
      flwGenJetContainer->SetJetRadius(fRadius);
      fEventObjects->Add(flwGenJetContainer);
    }
  }

  return kTRUE;
}

//__________________________________________________________
Bool_t lwJetFromForestProducer::Run(Long64_t entry) {

  //run analysis
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;
 
  //clear array
  flwJetContainer->ClearVec();
  
  //put jets of this event in array
  Int_t jetCount = 0;
  //Printf("%s: njets: %d",GetName(),fForestJets.nref);
  //int genSubJetCounter = 0;
  for(Int_t i = 0; i<fForestJets.nref; i++) {
    if( fForestJets.jtpt[i]<fMinJetPt) continue;
    if( fDoPFJetID && !IsGoodPFJet(i)) {
      //Printf("jet not accepted");
      continue;
    }
    //Printf("%s: jet pt: %f",GetName(),fForestJets.jtpt[i]);
    lwJet *jet = new lwJet(fForestJets.jtpt[i],
                           fForestJets.jteta[i],
                           fForestJets.jtphi[i],
                           fForestJets.jtm[i]);
    jet->SetCsvSimpleDiscr(fForestJets.discr_csvSimple[i]);
    jet->SetRawPt(fForestJets.rawpt[i]);
    jet->SetTau(1,fForestJets.jttau1[i]);
    jet->SetTau(2,fForestJets.jttau2[i]);
    jet->SetTau(3,fForestJets.jttau3[i]);
    if(fForestJets.jtSubJetPt)  jet->SetSubJetPt(fForestJets.jtSubJetPt->at(i));
    if(fForestJets.jtSubJetEta) jet->SetSubJetEta(fForestJets.jtSubJetEta->at(i));
    if(fForestJets.jtSubJetPhi) jet->SetSubJetPhi(fForestJets.jtSubJetPhi->at(i));
    if(fForestJets.jtSubJetM)   jet->SetSubJetM(fForestJets.jtSubJetM->at(i));
    if(fForestJets.jtdroppedBranches) jet->SetNDroppedBranches(fForestJets.jtdroppedBranches[i]);
    
    jet->SetRefToParton(fForestJets.refparton_flavor[i]);
    jet->SetRefToPartonForB(fForestJets.refparton_flavorForB[i]);
    jet->SetRefPt(fForestJets.refpt[i]);
    jet->SetRefEta(fForestJets.refeta[i]);
    jet->SetRefPhi(fForestJets.refphi[i]);
    jet->SetRefM(fForestJets.refm[i]);

    jet->SetRefPtG(fForestJets.refptG[i]);
    jet->SetRefEtaG(fForestJets.refetaG[i]);
    jet->SetRefPhiG(fForestJets.refphiG[i]);
    jet->SetRefMG(fForestJets.refmG[i]);
    
    jet->SetRefTau(1,fForestJets.reftau1[i]);
    jet->SetRefTau(2,fForestJets.reftau2[i]);
    jet->SetRefTau(3,fForestJets.reftau3[i]);
    jet->SetRefDr(fForestJets.refdrjt[i]);

    if(fForestJets.refSubJetPt && fForestJets.refptG[i]>0.) {
      if(i<(int)fForestJets.refSubJetPt->size()) {
        if(fForestJets.refSubJetPt)  jet->SetRefSubJetPt(fForestJets.refSubJetPt->at(i));
        if(fForestJets.refSubJetEta) jet->SetRefSubJetEta(fForestJets.refSubJetEta->at(i));
        if(fForestJets.refSubJetPhi) jet->SetRefSubJetPhi(fForestJets.refSubJetPhi->at(i));
        if(fForestJets.refSubJetM)   jet->SetRefSubJetM(fForestJets.refSubJetM->at(i));
      }
      if(fForestJets.refdroppedBranches) jet->SetRefNDroppedBranches(fForestJets.refdroppedBranches[i]);
      
    }

    // if(fForestJets.refSubJetPt && fForestJets.refptG[i]>0.) {
    //   if(i<(int)fForestJets.refSubJetPt->size()) {
    //     double subjetptsum = 0.;
    //     if(fForestJets.refSubJetPt->at(i).size()>1) {
    //       subjetptsum = fForestJets.refSubJetPt->at(i).at(0) + fForestJets.refSubJetPt->at(i).at(1);
    //     }
    //     Printf("i: %d i: %d refpt: %f refptG: %f subjetptsum: %f",i,i,fForestJets.refpt[i],fForestJets.refptG[i],subjetptsum);
    //     if(fForestJets.refSubJetPt->at(i).size()>1) {
    //       Printf("eta %f %f %f %f",fForestJets.refeta[i],fForestJets.refetaG[i],fForestJets.refSubJetEta->at(i).at(0),fForestJets.refSubJetEta->at(i).at(1));
    //       Printf("phi %f %f %f %f",fForestJets.refphi[i],fForestJets.refphiG[i],fForestJets.refSubJetPhi->at(i).at(0),fForestJets.refSubJetPhi->at(i).at(1));
    //     }
    //   }
    // }

    //with ordering bug
    // if(fForestJets.refSubJetPt && fForestJets.refptG[genSubJetCounter]>0.) {
    //   if(genSubJetCounter<(int)fForestJets.refSubJetPt->size()) {
    //     if(fForestJets.refSubJetPt)  jet->SetRefSubJetPt(fForestJets.refSubJetPt->at(genSubJetCounter));
    //     if(fForestJets.refSubJetEta) jet->SetRefSubJetEta(fForestJets.refSubJetEta->at(genSubJetCounter));
    //     if(fForestJets.refSubJetPhi) jet->SetRefSubJetPhi(fForestJets.refSubJetPhi->at(genSubJetCounter));
    //     if(fForestJets.refSubJetM)   jet->SetRefSubJetM(fForestJets.refSubJetM->at(genSubJetCounter));
    //   }
    // }
    
    //if(fForestJets.refpt[i]>=0.) genSubJetCounter++;    
    
    // if(fForestJets.refSubJetPt && fForestJets.refptG[genSubJetCounter]>0.) {
    //   if(genSubJetCounter<(int)fForestJets.refSubJetPt->size()) {
    //     double subjetptsum = 0.;
    //     if(fForestJets.refSubJetPt->at(genSubJetCounter).size()>1) {
    //       subjetptsum = fForestJets.refSubJetPt->at(genSubJetCounter).at(0) + fForestJets.refSubJetPt->at(genSubJetCounter).at(1);
    //     }
    //     Printf("i: %d genSubJetCounter: %d refpt: %f refptG: %f subjetptsum: %f",i,genSubJetCounter,fForestJets.refpt[i],fForestJets.refptG[genSubJetCounter],subjetptsum);
    //   }
    // }
    
    //    if(fForestJets.refptG[genSubJetCounter]>0.) genSubJetCounter++;

    jet->SetCsvSimpleDiscr(fForestJets.discr_csvSimple[i]);
    jet->SetSubEvent(fForestJets.subid[i]);
    jet->SetChargedProp(fForestJets.chargedMax[i],fForestJets.chargedSum[i],fForestJets.chargedN[i]);
    jet->SetChargedHardProp(fForestJets.chargedMax[i],fForestJets.chargedHardSum[i],fForestJets.chargedHardN[i]);
    jet->SetPhotonProp(fForestJets.photonMax[i],fForestJets.photonSum[i],fForestJets.photonN[i]);
    jet->SetNeutralProp(fForestJets.neutralMax[i],fForestJets.neutralSum[i],fForestJets.neutralN[i]);
    jet->SetEmProp(fForestJets.eMax[i],fForestJets.eSum[i],fForestJets.eN[i]);
    jet->SetMuProp(fForestJets.muMax[i],fForestJets.muSum[i],fForestJets.muN[i]);

    jet->SetCHF(fForestJets.jtPfCHF[i]);
    jet->SetNHF(fForestJets.jtPfNHF[i]);
    jet->SetCEF(fForestJets.jtPfCEF[i]);
    jet->SetNEF(fForestJets.jtPfNEF[i]);
    jet->SetMUF(fForestJets.jtPfMUF[i]);

    jet->SetCHM(fForestJets.jtPfCHM[i]);
    jet->SetNHM(fForestJets.jtPfNHM[i]);
    jet->SetCEM(fForestJets.jtPfCEM[i]);
    jet->SetNEM(fForestJets.jtPfNEM[i]);
    jet->SetMUM(fForestJets.jtPfMUM[i]);
    
    flwJetContainer->AddJet(jet,jetCount);
    ++jetCount;
  }
  //Printf("jetCount: %d",jetCount);
  flwJetContainer->SortJets();

  //only produce gen jets if requested and if they exist
  if(flwGenJetContainer && fChain->GetBranch("ngen")) {
    //Printf("%s: n genjets: %d",GetName(),fForestJets.ngen);
    //Printf("doing gen jets");
    if(fForestJets.ngen>0) {
      flwGenJetContainer->ClearVec();
      Int_t genJetCount = 0;
      for(Int_t i = 0; i<fForestJets.ngen; i++) {
        //if(fForestJets.genmatchindex[i]<0) continue;
        if(fForestJets.genpt[i]<10.) continue;
        lwJet *genjet = new lwJet(fForestJets.genpt[i],
                                  fForestJets.geneta[i],
                                  fForestJets.genphi[i],
                                  fForestJets.genm[i]);
        genjet->SetMatchId1(fForestJets.genmatchindex[i]);
        genjet->SetRefPt(fForestJets.jtpt[fForestJets.genmatchindex[i]]);

        if(fForestJets.genSubJetPt && fForestJets.genptG[i]>0.) {
          if(i<(int)fForestJets.genSubJetPt->size()) {
            if(fForestJets.genSubJetPt)  genjet->SetSubJetPt(fForestJets.genSubJetPt->at(i));
            if(fForestJets.genSubJetEta) genjet->SetSubJetEta(fForestJets.genSubJetEta->at(i));
            if(fForestJets.genSubJetPhi) genjet->SetSubJetPhi(fForestJets.genSubJetPhi->at(i));
            if(fForestJets.genSubJetM)   genjet->SetSubJetM(fForestJets.genSubJetM->at(i));
          }
        }
        
        flwGenJetContainer->AddJet(genjet,genJetCount);
        ++genJetCount;
      }
      flwGenJetContainer->SortJets();
    }
  }
  
  return kTRUE;
}

//__________________________________________________________
Long64_t lwJetFromForestProducer::LoadTree(Long64_t entry) {

  //overloaded LoadTree function 
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }
  
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
    //  Printf("%lld fCurrent: %d",entry,fCurrent);
  }

  // fChain->SetMakeClass(1);
 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("lwJetFromForestProducer: centry smaller than 0");
    return centry;  
  } 

  fChain->GetEntry(entry);
  
  return centry;
}

//__________________________________________________________
bool lwJetFromForestProducer::IsGoodPFJet(int i) const {
  //pf jet selection (https://twiki.cern.ch/twiki/bin/view/CMS/JetID)
  int nconst = fForestJets.chargedN[i]+fForestJets.photonN[i]+fForestJets.neutralN[i]+fForestJets.eN[i]+fForestJets.muN[i];
  if(abs(fForestJets.jteta[i])<=3.) {
    if(fForestJets.neutralSum[i]/fForestJets.rawpt[i] > 0.9) return false;
    if(fForestJets.eSum[i]/fForestJets.rawpt[i] > 0.9) return false;
    if(nconst < 2) return false;
    if(fForestJets.muSum[i]/fForestJets.rawpt[i] > 0.8) return false;

    if(abs(fForestJets.jteta[i])<=2.4) {
      if(fForestJets.chargedSum[i]/fForestJets.rawpt[i] < 0.01) return false;
      if(fForestJets.chargedN[i]<1) return false;
    }
  } else {
    if(fForestJets.eSum[i]/fForestJets.rawpt[i] > 0.9) return false;
    if(fForestJets.neutralN[i]<5) return false;
  }
  return true;
}

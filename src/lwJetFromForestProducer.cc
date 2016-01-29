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
    fChain->SetBranchStatus("ref*",1);
    if (fChain->GetBranch("refpt")){
      fChain->SetBranchAddress("refpt", &fForestJets.refpt, &fForestJets.b_refpt);}
    if (fChain->GetBranch("refeta"))
      fChain->SetBranchAddress("refeta", &fForestJets.refeta, &fForestJets.b_refeta);
    if (fChain->GetBranch("refm"))
      fChain->SetBranchAddress("refm", &fForestJets.refm, &fForestJets.b_refm);
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
    fChain->SetBranchStatus("ngen",1);
    fChain->SetBranchStatus("gen*",1);
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
    if (fChain->GetBranch("gensubid"))
      fChain->SetBranchAddress("gensubid", &fForestJets.gensubid, &fForestJets.b_gensubid);
    if (fChain->GetBranch("genmatchindex"))
      fChain->SetBranchAddress("genmatchindex", &fForestJets.genmatchindex, &fForestJets.b_genmatchindex);

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

  for(Int_t i = 0; i<fForestJets.nref; i++) {
    //std::cout<<fForestJets.chargedSum[i]<<std::endl;

    if( fDoPFJetID && !IsGoodPFJet(i)) {
      //Printf("jet not accepted");
      continue;
    }
    lwJet *jet = new lwJet(fForestJets.jtpt[i],
                           fForestJets.jteta[i],
                           fForestJets.jtphi[i],
                           fForestJets.jtm[i]);
    jet->SetRefToParton(fForestJets.refparton_flavor[i]);
    jet->SetRefToPartonForB(fForestJets.refparton_flavorForB[i]);
    jet->SetCsvSimpleDiscr(fForestJets.discr_csvSimple[i]);
    jet->SetRawPt(fForestJets.rawpt[i]);
    jet->SetRefPt(fForestJets.refpt[i]);
    jet->SetRefEta(fForestJets.refeta[i]);
    jet->SetRefM(fForestJets.refm[i]);
    jet->SetRefDr(fForestJets.refdrjt[i]);
    jet->SetCsvSimpleDiscr(fForestJets.discr_csvSimple[i]);
    jet->SetSubEvent(fForestJets.subid[i]);
    jet->SetChargedProp(fForestJets.chargedMax[i],fForestJets.chargedSum[i],fForestJets.chargedN[i]);
    jet->SetChargedHardProp(fForestJets.chargedMax[i],fForestJets.chargedHardSum[i],fForestJets.chargedHardN[i]);
    jet->SetPhotonProp(fForestJets.photonMax[i],fForestJets.photonSum[i],fForestJets.photonN[i]);
    jet->SetNeutralProp(fForestJets.neutralMax[i],fForestJets.neutralSum[i],fForestJets.neutralN[i]);
    jet->SetEmProp(fForestJets.eMax[i],fForestJets.eSum[i],fForestJets.eN[i]);
    jet->SetMuProp(fForestJets.muMax[i],fForestJets.muSum[i],fForestJets.muN[i]);
    flwJetContainer->AddJet(jet,jetCount);
    ++jetCount;
  }
  //Printf("jetCount: %d",jetCount);
  flwJetContainer->SortJets();

  //only produce gen jets if requested and if they exist
  if(flwGenJetContainer && fChain->GetBranch("ngen")) {
    //Printf("doing gen jets");
    if(fForestJets.ngen>0) {
      flwGenJetContainer->ClearVec();
      Int_t genJetCount = 0;
      for(Int_t i = 0; i<fForestJets.ngen; i++) {
        //if(fForestJets.genmatchindex[i]<0) continue;
        lwJet *genjet = new lwJet(fForestJets.genpt[i],
                                  fForestJets.geneta[i],
                                  fForestJets.genphi[i],
                                  fForestJets.genm[i]);
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

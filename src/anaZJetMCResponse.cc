#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/diParticle.h"

#include "TLorentzVector.h"

//#include "TClass.h"

ClassImp(anaZJetMCResponse)

//----------------------------------------------------------
anaZJetMCResponse::anaZJetMCResponse(const char *name, const char *title) 
 :anaBaseTask(name,title),
  fZsName(""),
  fZs(0x0),
  fJetsName(""),
  fJetsCont(0x0),
  fh1ZPt(),
  fh3ZJetPtRecGen(),
  fh3ZJetXJZRecGen()
{

}

//----------------------------------------------------------
void anaZJetMCResponse::Exec(Option_t * /*option*/)
{
   anaBaseTask::Exec();
   //printf("anaZJetMCResponse executing !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");

   if(!SelectEvent()) return;
  
   if(!fInitOutput) CreateOutputObjects();

   //Get objects from event

   //Get Z candidates
   if(!fZs && !fZsName.IsNull()) {
     fZs = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fZsName.Data()));
   }
   if(!fZs) return;

   //Get jets
   if(!fJetsCont && !fJetsName.IsNull())
    fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
  if(!fJetsCont) {
    Printf("%s: Cannot find %s",GetName(),fJetsName.Data());
    return;
  }

  // Double_t cent = 0.;
  float weight = 1.;
  if(fHiEvent) {
    if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
  //    cent = fHiEvent->GetCentrality();
  }
  
  Int_t nZs = fZs->GetEntriesFast();
  const Int_t nJets = fJetsCont->GetNJets();
  //Printf("nZs: %d nJets: %d",nZs,nJets);
  if(nJets==0 || nZs==0) return;
  
  for (int i = 0; i < fZs->GetEntriesFast(); i++) {
    diParticle *z = static_cast<diParticle*>(fZs->At(i));
    if(!z) {
      Printf("%s ERROR: couldn't get Z candidate",GetName());
      continue;
    }
    if(z->Pt()<20.) continue;
    fh1ZPt->Fill(z->Pt(),weight);
    
    for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ij++) {
      lwJet *jet = fJetsCont->GetJet(ij);
      if(!jet) continue;
      if(jet->GetRefPt()<10. || abs(jet->Eta())>2.) continue; //only select true jets
      
      double dphi = acos(cos(jet->Phi() - z->Phi()));
      double mindphi = 2*3.14159/3.;
      if(dphi<mindphi) continue;
      fh3ZJetPtRecGen->Fill(z->Pt(),jet->Pt(),jet->GetRefPt(),weight);
      fh3ZJetXJZRecGen->Fill(z->Pt(),jet->Pt()/z->Pt(),jet->GetRefPt()/z->Pt(),weight);      
      
    }//jet loop
  }//Z loop
  
}

//----------------------------------------------------------
void anaZJetMCResponse::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaZJetMCResponse: fOutput not present");
    return;
  }

  fh1ZPt = new TH1F("fh1ZPt","fh1ZPt;p_{T,Z}",200,0,200.);
  fOutput->Add(fh1ZPt);

  fh3ZJetPtRecGen = new TH3F("fh3ZJetPtRecGen","fh3JetPtRecGen;;p_{T,Z};p_{T,jet}^{det};p_{T,jet}^{part}",200,0,200,200,0,200,200,0,200);
  fOutput->Add(fh3ZJetPtRecGen);

  fh3ZJetXJZRecGen = new TH3F("fh3ZJetXJZRecGen","fh3JetPtRecGen;;p_{T,Z};p_{T,jet}^{det};p_{T,jet}^{part}",200,0,200,200,0,200,200,0,200);
  fOutput->Add(fh3ZJetXJZRecGen);

}

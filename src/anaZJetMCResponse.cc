#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/diParticle.h"

#include "TLorentzVector.h"

//#include "TClass.h"

ClassImp(anaZJetMCResponse)

//----------------------------------------------------------
anaZJetMCResponse::anaZJetMCResponse(const char *name, const char *title) 
 :anaBaseTask(name,title),
  fNcentBins(4),
  fZsName(""),
  fZs(0x0),
  fJetsName(""),
  fJetsCont(0x0),
  fGenJetsName(""),
  fGenJetsCont(0x0),
  fUseForestMatching(true),
  fh1ZPt(),
  fh3ZJetPtRecGen(),
  fh3ZJetXJZRecGen()
{

  fh1ZPt               = new TH1F*[fNcentBins];
  fh3ZJetPtRecGen      = new TH3F*[fNcentBins];
  fh3ZJetXJZRecGen     = new TH3F*[fNcentBins];

  for (Int_t i = 0; i < fNcentBins; i++) {
    fh1ZPt[i]           = 0;
    fh3ZJetPtRecGen[i]  = 0;
    fh3ZJetXJZRecGen[i] = 0;
  }

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

  //Get gen jets
  if(!fGenJetsCont && !fGenJetsName.IsNull())
    fGenJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fGenJetsName.Data()));
  if(!fGenJetsCont && !fUseForestMatching) {
    Printf("%s: Cannot find %s",GetName(),fJetsName.Data());
    return;
  }

  // Double_t cent = 0.;
  float weight = 1.;
  double cent = 0.;
  if(fHiEvent) {
    if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
    cent = fHiEvent->GetCentrality();
  }
  Int_t fCentBin = 0;
  if(fNcentBins==4) {
    if(cent>=0. && cent<10.)       fCentBin = 0;
    else if(cent>=10. && cent<30.) fCentBin = 1;
    else if(cent>=30. && cent<50.) fCentBin = 2;
    else if(cent>=50. && cent<80.) fCentBin = 3;
    else fCentBin = -1;
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
    if(fCentBin>-1 && fCentBin<fNcentBins)
      fh1ZPt[fCentBin]->Fill(z->Pt(),weight);
    
    for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ij++) {
      lwJet *jet = fJetsCont->GetJet(ij);
      if(!jet) continue;

      double refpt = -1.;
      if(fUseForestMatching)
        refpt = jet->GetRefPt();
      else {
       int id = jet->GetMatchId1();
       lwJet *jetGen = fGenJetsCont->GetJet(id);
       if(!jetGen) continue;
       refpt = jetGen->Pt();
      }
      
      if(refpt<25. || fabs(jet->Eta())>2.) continue; //only select true jets
      
      double dphi = acos(cos(jet->Phi() - z->Phi()));
      double mindphi = 2*3.14159/3.;
      if(dphi<mindphi) continue;
      if(fCentBin>-1 && fCentBin<fNcentBins) {
        fh3ZJetPtRecGen[fCentBin]->Fill(z->Pt(),jet->Pt(),refpt,weight);
        fh3ZJetXJZRecGen[fCentBin]->Fill(z->Pt(),jet->Pt()/z->Pt(),refpt/z->Pt(),weight);
      }
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

  TString histTitle = "";
  TString histName  = "";
  for (Int_t i = 0; i < fNcentBins; i++) {
    histName  = Form("fh1ZPt_%d",i);
    histTitle = Form("%s;#it{p}_{T,Z};",histName.Data());
    fh1ZPt[i] = new TH1F(histName.Data(),histTitle.Data(),200,0,200.);
    fOutput->Add(fh1ZPt[i]);

    histName  = Form("fh3ZJetPtRecGen_%d",i);
    histTitle = Form("%s;#it{p}_{T,Z};p_{T,jet}^{rec};p_{T,jet}^{gen}",histName.Data());
    fh3ZJetPtRecGen[i] = new TH3F(histName.Data(),histTitle.Data(),200,0,200,200,0,200,200,0,200);
    fOutput->Add(fh3ZJetPtRecGen[i]);

    histName  = Form("fh3ZJetZJZRecGen_%d",i);
    histTitle = Form("%s;#it{p}_{T,Z};p_{T,jet}^{rec}/p_{T,Z};p_{T,jet}^{gen}/p_{T,Z}",histName.Data());
    fh3ZJetXJZRecGen[i] = new TH3F(histName.Data(),histTitle.Data(),200,0,200,40,0,2,40,0,2);
    fOutput->Add(fh3ZJetXJZRecGen[i]);
  }
}

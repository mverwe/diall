#include "UserCode/diall/interface/anaDijetAj.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaDijetAj)
   
anaDijetAj::anaDijetAj(const char *name, const char *title) 
:anaBaseTask(name,title),
  fNcentBins(4),
  fJetsName(""),
  fJetsCont(),
  fJetEtaMin(-5.),
  fJetEtaMax(5.),
  fMinRefPt(-10000.),
  fDoDijets(true),
  fPtLeadingMin(),
  fPtLeadingMax(),
  fPtSubleadingMin(30.),
  fMinDPhi(2*3.14159/3.),
  fMinMassLeading(-1.),
  fhCentrality(0),
  fh3PtEtaPhi(),
  fh2PtMass(),
  fh2SLPtSubjetPtAj(),
  fh2SLPtSubjetPtRatio()
{
  
  fh3PtEtaPhi          = new TH3F*[fNcentBins];
  fh2PtMass            = new TH2F*[fNcentBins];
 
  for (int i = 0; i < fNcentBins; i++) {
    fh3PtEtaPhi[i]      = 0;
    fh2PtMass[i]        = 0;
  }

}

//----------------------------------------------------------
void anaDijetAj::Exec(Option_t * /*option*/)
{
  //printf("anaDijetAj executing\n");
  anaBaseTask::Exec();
  if(!SelectEvent()) return;
  
  if(!fInitOutput) CreateOutputObjects();

  if(!fJetsCont && !fJetsName.IsNull())
    fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
  if(!fJetsCont) return;

  //Get MC weight
  float weight = 1.;
  if(fHiEvent) {
    if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
  }

  //Determine centrality bin
  Double_t cent = 0.;
  if(fHiEvent) cent = fHiEvent->GetCentrality();
  fhCentrality->Fill(cent);
  Int_t fCentBin = 0;
  if(fNcentBins==4) {
    if(cent>=0. && cent<10.)       fCentBin = 0;
    else if(cent>=10. && cent<30.) fCentBin = 1;
    else if(cent>=30. && cent<50.) fCentBin = 2;
    else if(cent>=50. && cent<80.) fCentBin = 3;
    else fCentBin = -1;
  }

   for (int i = 0; i < fJetsCont->GetNJets(); i++) {
     lwJet *jet = fJetsCont->GetJet(i);
     Double_t pt = jet->Pt();
     Double_t phi = jet->Phi();
     Double_t eta = jet->Eta();
     Double_t m = jet->M();
     if(fabs(pt-0.)<1e-6) continue; //remove ghosts
     if(eta<fJetEtaMin || eta>fJetEtaMax) continue;
     if(fMinRefPt>-1. && jet->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    
 
     if(fCentBin>-1 && fCentBin<fNcentBins) {
       fh3PtEtaPhi[fCentBin]->Fill(pt,eta,phi,weight);
       fh2PtMass[fCentBin]->Fill(pt,m);
     }
       
     if(fDoDijets && fCentBin>-1 && fCentBin<fNcentBins) {
       if(m<fMinMassLeading) continue;
       
       int nBinsLJ = (int)fPtLeadingMin.size();
       for(int l = 0; l<nBinsLJ; ++l) {
         float ptmin = fPtLeadingMin.at(l);
         float ptmax = fPtLeadingMax.at(l);
         if(pt<ptmin || pt>ptmax) continue;

         bool foundSL = false;
         for (int j = i+1; j < fJetsCont->GetNJets(); j++) {
           if(i==j) continue;
           if(foundSL) continue;//only correlate with 2nd leading jet in acceptance
           lwJet *jet2 = fJetsCont->GetJet(j);
           if(fabs(jet2->Pt()-0.)<1e-6) continue; //remove ghosts
           if(jet2->Eta()<fJetEtaMin || jet2->Eta()>fJetEtaMax) continue;
           if(fMinRefPt>-1. && jet2->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    

           if(jet2->Pt()<fPtSubleadingMin) continue;

           foundSL = true;
           
           double dphi = acos(cos(jet->Phi() - jet2->Phi()));
           if(dphi<fMinDPhi) continue;
           
           double aj = (pt - jet2->Pt()) / (pt + jet2->Pt());
           double ptrat = jet2->Pt()/pt;

           TH2F *haj = (fh2SLPtSubjetPtAj.at(fCentBin)).at(l);
           haj->Fill(jet2->Pt(),aj);

           TH2F *hrat = (fh2SLPtSubjetPtRatio.at(fCentBin)).at(l);
           hrat->Fill(jet2->Pt(),ptrat);
         }
       }
     }
   }

}

//----------------------------------------------------------
void anaDijetAj::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaDijetAj: fOutput not present");
    return;
  }

  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  TString histTitle = "";
  TString histName  = "";
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = Form("fh2PtEtaPhi_%d",i);
    histTitle = Form("%s;pt;#eta;#phi;",histName.Data());
    fh3PtEtaPhi[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhi[i]);

    histName = Form("fh2PtMass_%d",i);
    histTitle = Form("%s;p_{T};M;",histName.Data());
    fh2PtMass[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,50);
    fOutput->Add(fh2PtMass[i]);
  }

  int nBinsLJ = (int)fPtLeadingMin.size();
  for (int i = 0; i < fNcentBins; i++) {
    std::vector<TH2F *> h2LJaj;
    std::vector<TH2F *> h2LJrat;
    for(int j = 0; j < nBinsLJ; j++) {
      histName = Form("fh2SLPtSubjetPtAj_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};A_{J}",histName.Data());
      TH2F *htmpaj = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJaj.push_back(htmpaj);
      fOutput->Add(htmpaj);

      histName = Form("fh2SLPtSubjetPtRatio_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,2}/p_{T,1}",histName.Data());
      TH2F *htmprat = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJrat.push_back(htmprat);
      fOutput->Add(htmprat);
    }
    fh2SLPtSubjetPtAj.push_back(h2LJaj);
    fh2SLPtSubjetPtRatio.push_back(h2LJrat);
  }
}

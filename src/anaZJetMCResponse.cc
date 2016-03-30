#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/diParticle.h"

#include "TLorentzVector.h"

//#include "TClass.h"

ClassImp(anaZJetMCResponse)

//----------------------------------------------------------
anaZJetMCResponse::anaZJetMCResponse(const char *name, const char *title) 
 :anaBaseTask(name,title),
  fNcentBins(3),
  fZsName(""),
  fZs(0x0),
  fJetsName(""),
  fJetsCont(0x0),
  fGenJetsName(""),
  fGenJetsCont(0x0),
  fUseForestMatching(true),
  fMaxJetEta(2.),
  fMinJetPt(30.),
  fMinZPt(40.),
  fh1ZPtInc(0),
  fh3ZJetPtRecGenInc(0),
  fh3ZJetXJZRecGenInc(0),
  fh3PtTrueEtaScalePtInc(0),
  fh1ZPt(),
  fh3ZJetPtRecGen(),
  fh3ZJetXJZRecGen(),
  fh3PtTrueEtaScalePt(0)
{

  fh1ZPt               = new TH1F*[fNcentBins];
  fh3ZJetPtRecGen      = new TH3F*[fNcentBins];
  fh3ZJetXJZRecGen     = new TH3F*[fNcentBins];
  fh3PtTrueEtaScalePt  = new TH3F*[fNcentBins];

  for (Int_t i = 0; i < fNcentBins; i++) {
    fh1ZPt[i]              = 0;
    fh3ZJetPtRecGen[i]     = 0;
    fh3ZJetXJZRecGen[i]    = 0;
    fh3PtTrueEtaScalePt[i] = 0;
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
  if(fNcentBins==3) {
    if(cent>=0. && cent<30.)        fCentBin = 0;
    else if(cent>=30. && cent<100.) fCentBin = 1;
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
    if(z->Pt()<fMinZPt) continue;
    if(fCentBin>-1 && fCentBin<fNcentBins)
      fh1ZPt[fCentBin]->Fill(z->Pt(),weight);
    fh1ZPtInc->Fill(z->Pt(),weight);
    
    for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ij++) {
      lwJet *jet = fJetsCont->GetJet(ij);
      if(!jet) continue;

      if(jet->Pt()<fMinJetPt) continue;

      double refpt = -1.;
      if(fUseForestMatching)
        refpt = jet->GetRefPt();
      else {
       int id = jet->GetMatchId1();
       lwJet *jetGen = fGenJetsCont->GetJet(id);
       if(!jetGen) continue;
       refpt = jetGen->Pt();
      }

      if(jet->GetRefDr()>0.3
         || jet->GetRefPt()<10.)//e-3)
        continue;
      
      if(refpt<10. || fabs(jet->Eta())>fMaxJetEta) continue; //only select true jets
      
      double dphi = acos(cos(jet->Phi() - z->Phi()));
      double mindphi = 2*3.14159/3.;
      if(dphi<mindphi) continue;
      if(fCentBin>-1 && fCentBin<fNcentBins) {
        fh3ZJetPtRecGen[fCentBin]->Fill(z->Pt(),jet->Pt(),refpt,weight);
        fh3ZJetXJZRecGen[fCentBin]->Fill(z->Pt(),jet->Pt()/z->Pt(),refpt/z->Pt(),weight);

        fh3PtTrueEtaScalePt[fCentBin]->Fill(jet->GetRefPt(),jet->GetRefEta(),jet->Pt()/jet->GetRefPt(),weight);
      }

      //integrated over all centralities
      fh3ZJetPtRecGenInc->Fill(z->Pt(),jet->Pt(),refpt,weight);
      fh3ZJetXJZRecGenInc->Fill(z->Pt(),jet->Pt()/z->Pt(),refpt/z->Pt(),weight);
      fh3PtTrueEtaScalePtInc->Fill(jet->GetRefPt(),jet->GetRefEta(),jet->Pt()/jet->GetRefPt(),weight);
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

  const Int_t nBinsPtDet  = 200;
  const Double_t minPtDet =  0.;
  const Double_t maxPtDet = 400.;
  double *binsPtDet = new double[nBinsPtDet+1];
  for(int i=0; i<=nBinsPtDet; ++i) binsPtDet[i]=(double)minPtDet + (maxPtDet-minPtDet)/nBinsPtDet*(double)i ;

  const Int_t nBinsPtPart  = 200;
  const Double_t minPtPart = 0.;
  const Double_t maxPtPart = 400.;
  double *binsPtPart = new double[nBinsPtPart+1];
  for(int i=0; i<=nBinsPtPart; ++i) binsPtPart[i]=(double)minPtPart + (maxPtPart-minPtPart)/nBinsPtPart*(double)i ;

  const int nBinsEta = 30;
  double etaEdge[nBinsEta+1] = {-5.191, -3.139, -2.964, -2.853, -2.650, -2.500, -2.322, -2.172, -1.930, -1.653, -1.305, -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783, 1.044, 1.305, 1.653, 1.930, 2.172, 2.322, 2.500, 2.650, 2.853, 2.964, 3.139, 5.191};
  double *binsEta = new double[nBinsEta];
  for(int i = 0; i<=nBinsEta; ++i)
    binsEta[i] = etaEdge[i];

  const Int_t nBinsScalePt  = 300;
  const Double_t minScalePt = 0.;
  const Double_t maxScalePt = 3.;
  double *binsScalePt = new double[nBinsScalePt+1];
  for(int i=0; i<=nBinsScalePt; ++i) binsScalePt[i]=(double)minScalePt + (maxScalePt-minScalePt)/nBinsScalePt*(double)i ;

  TString histTitle = "";
  TString histName  = "";
  
  histName  = Form("fh1ZPtInc");
  histTitle = Form("%s;#it{p}_{T,Z};",histName.Data());
  fh1ZPtInc = new TH1F(histName.Data(),histTitle.Data(),200,0,200.);
  fOutput->Add(fh1ZPtInc);

  histName  = Form("fh3ZJetPtRecGenInc");
  histTitle = Form("%s;#it{p}_{T,Z};p_{T,jet}^{rec};p_{T,jet}^{gen}",histName.Data());
  fh3ZJetPtRecGenInc = new TH3F(histName.Data(),histTitle.Data(),200,0,200,200,0,200,200,0,200);
  fOutput->Add(fh3ZJetPtRecGenInc);

  histName  = Form("fh3ZJetZJZRecGenInc");
  histTitle = Form("%s;#it{p}_{T,Z};p_{T,jet}^{rec}/p_{T,Z};p_{T,jet}^{gen}/p_{T,Z}",histName.Data());
  fh3ZJetXJZRecGenInc = new TH3F(histName.Data(),histTitle.Data(),200,0,200,32,0,2,32,0,2);
  fOutput->Add(fh3ZJetXJZRecGenInc);

  histName = Form("fh3PtTrueEtaScalePtInc");
  histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
  fh3PtTrueEtaScalePtInc = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScalePt,binsScalePt);
  fOutput->Add(fh3PtTrueEtaScalePtInc);
  
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
    fh3ZJetXJZRecGen[i] = new TH3F(histName.Data(),histTitle.Data(),200,0,200,32,0,2,32,0,2);
    fOutput->Add(fh3ZJetXJZRecGen[i]);

    histName = Form("fh3PtTrueEtaScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueEtaScalePt[i]);
  }

  if(binsPtPart)            delete [] binsPtPart;
  if(binsPtDet)             delete [] binsPtDet;
  if(binsEta)               delete [] binsEta;
  if(binsScalePt)           delete [] binsScalePt;
}

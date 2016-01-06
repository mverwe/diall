#include "UserCode/diall/interface/anaJetEnergyScale.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetEnergyScale)
   
anaJetEnergyScale::anaJetEnergyScale(const char *name, const char *title) 
:anaBaseTask(name,title),
  fNcentBins(4),
  fJetsName(""),
  fJetsCont(0x0),
  fJets2Name(""),
  fJets2Cont(0x0),
  fhEventSel(0),
  fhCentrality(0),
  fhNPV(0),
  fh2PtEtaNoMatching(0),
  fh3PtEtaPhiNotMatched(0),
  fh3PtEtaPhiMatched(0),
  fh3PtTrueNPVDeltaPt(0),
  fh3PtTrueNPVDeltaPtRel(0),
  fh3PtTrueNPVScalePt(0),
  fh3PtTruePtSubNPV(0),
  fh3PtTrueEtaDeltaPt(0),
  fh3PtTrueEtaDeltaPtRel(0),
  fh3PtTrueEtaScalePt(0),
  fh3PtTruePtSubEta(0)
{

  fh2PtEtaNoMatching           = new TH2F*[fNcentBins];
  fh3PtEtaPhiNotMatched        = new TH3F*[fNcentBins];
  fh3PtEtaPhiMatched           = new TH3F*[fNcentBins];
  fh3PtTrueNPVDeltaPt          = new TH3F*[fNcentBins];
  fh3PtTrueNPVDeltaPtRel       = new TH3F*[fNcentBins];
  fh3PtTrueNPVScalePt          = new TH3F*[fNcentBins];
  fh3PtTruePtSubNPV            = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaPt          = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaPtRel       = new TH3F*[fNcentBins];
  fh3PtTrueEtaScalePt          = new TH3F*[fNcentBins];
  fh3PtTruePtSubEta            = new TH3F*[fNcentBins];
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    fh2PtEtaNoMatching[i]           = 0;
    fh3PtEtaPhiNotMatched[i]        = 0;
    fh3PtEtaPhiMatched[i]           = 0;
    fh3PtTrueNPVDeltaPt[i]          = 0;
    fh3PtTrueNPVDeltaPtRel[i]       = 0;
    fh3PtTrueNPVScalePt[i]          = 0;
    fh3PtTruePtSubNPV[i]            = 0;
    fh3PtTrueEtaDeltaPt[i]          = 0;
    fh3PtTrueEtaDeltaPtRel[i]       = 0;
    fh3PtTrueEtaScalePt[i]          = 0;
    fh3PtTruePtSubEta[i]            = 0;
  }

}

//----------------------------------------------------------
void anaJetEnergyScale::Exec(Option_t * /*option*/)
{
 
  anaBaseTask::Exec();
  if(!SelectEvent()) return;

  //printf("anaJetEnergyScale executing\n");
  if(!fInitOutput) CreateOutputObjects();

  //Get event properties
  if(fHiEvent) {
    if(abs(fHiEvent->GetVz())>15.) {
      Printf("%s: Didn't pass vertex selection: %f",GetName(),fHiEvent->GetVz());
      return;
    }
  }
  
  //Get PF jets
  if(!fJetsCont && !fJetsName.IsNull())
    fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
  if(!fJetsCont) {
    Printf("%s: Cannot find %s",GetName(),fJetsName.Data());
    return;
  }
   //Get calo jets
   if(!fJets2Cont && !fJets2Name.IsNull())
     fJets2Cont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJets2Name.Data()));
   if(!fJets2Cont) {
     Printf("%s: Cannot find %s",GetName(),fJets2Name.Data());
     return;
   }
   const Int_t nJets1 = fJetsCont->GetNJets();
   const Int_t nJets2 = fJets2Cont->GetNJets();
   if(nJets1==0 || nJets2==0) {
     // Printf("%s: nJets1: %d  nJets2: %d",GetName(),nJets1,nJets2);
     return;
   }

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

   //number of generated pile-up vertices
   int npv = 1;
   if(fHiEvent) npv = fHiEvent->GetNPV();
   fhNPV->Fill(npv);

   fhEventSel->Fill(0.5);

   for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ij++) {
     lwJet *jet1 = fJetsCont->GetJet(ij);
     if(!jet1) continue;

     if(fCentBin>-1 && fCentBin<fNcentBins) 
       fh2PtEtaNoMatching[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),weight);
          
     int id = jet1->GetMatchId1();
     lwJet *jet2 = fJets2Cont->GetJet(id);
     if(!jet2) {
       if(fCentBin>-1 && fCentBin<fNcentBins)
         fh3PtEtaPhiNotMatched[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);
       continue;
     }

     if(fCentBin>-1 && fCentBin<fNcentBins)
       fh3PtEtaPhiMatched[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);
     
     double dpt = jet2->Pt()-jet1->Pt();

     if(fCentBin>-1 && fCentBin<fNcentBins) {
       fh3PtTrueEtaDeltaPt[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dpt,weight);
       if(jet1->Pt()>0.) {
         fh3PtTrueEtaDeltaPtRel[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dpt/jet1->Pt(),weight);
         fh3PtTrueEtaScalePt[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet2->Pt()/jet1->Pt(),weight);
       }
       fh3PtTruePtSubEta[fCentBin]->Fill(jet1->Pt(),jet2->Pt(),jet1->Eta(),weight);
     
       if(abs(jet1->Eta())<1.3) {
         
         fh3PtTrueNPVDeltaPt[fCentBin]->Fill(jet1->Pt(),npv,dpt,weight);
         if(jet1->Pt()>0.) {
           fh3PtTrueNPVDeltaPtRel[fCentBin]->Fill(jet1->Pt(),npv,dpt/jet1->Pt(),weight);
           fh3PtTrueNPVScalePt[fCentBin]->Fill(jet1->Pt(),npv,jet2->Pt()/jet1->Pt(),weight);
         }
         fh3PtTruePtSubNPV[fCentBin]->Fill(jet1->Pt(),jet2->Pt(),npv,weight);
       }
     }
   }
}
//----------------------------------------------------------
void anaJetEnergyScale::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaJetEnergyScale: fOutput not present");
    return;
  }

  TString histTitle = "";
  TString histName  = "";

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

  const Int_t nBinsDPt  = 200;
  const Double_t minDPt = -100.;
  const Double_t maxDPt = 100.;
  double *binsDPt = new double[nBinsDPt+1];
  for(int i=0; i<=nBinsDPt; ++i) binsDPt[i]=(double)minDPt + (maxDPt-minDPt)/nBinsDPt*(double)i ;

  const Int_t nBinsDPtRel  = 600;
  const Double_t minDPtRel = -3.;
  const Double_t maxDPtRel = 3.;
  double *binsDPtRel = new double[nBinsDPtRel+1];
  for(int i=0; i<=nBinsDPtRel; ++i) binsDPtRel[i]=(double)minDPtRel + (maxDPtRel-minDPtRel)/nBinsDPtRel*(double)i ;

  const Int_t nBinsScalePt  = 300;
  const Double_t minScalePt = 0.;
  const Double_t maxScalePt = 3.;
  double *binsScalePt = new double[nBinsScalePt+1];
  for(int i=0; i<=nBinsScalePt; ++i) binsScalePt[i]=(double)minScalePt + (maxScalePt-minScalePt)/nBinsScalePt*(double)i ;

  const Int_t nBinsNPV = 100;
  const Double_t minNPV = 0.;
  const Double_t maxNPV = 100.;
  double *binsNPV = new double[nBinsNPV+1];
  for(int i=0; i<=nBinsNPV; ++i) binsNPV[i]=(double)minNPV + (maxNPV-minNPV)/nBinsNPV*(double)i ;

  const Int_t nBinsPhi = 72;
  const Double_t minPhi = -TMath::Pi();
  const Double_t maxPhi = TMath::Pi();
  double *binsPhi = new double[nBinsPhi+1];
  for(int i=0; i<=nBinsPhi; ++i) binsPhi[i]=(double)minPhi + (maxPhi-minPhi)/nBinsPhi*(double)i ;
  
  const int nBinsEta = 30;
  double etaEdge[nBinsEta+1] = {-5.191, -3.139, -2.964, -2.853, -2.650, -2.500, -2.322, -2.172, -1.930, -1.653, -1.305, -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783, 1.044, 1.305, 1.653, 1.930, 2.172, 2.322, 2.500, 2.650, 2.853, 2.964, 3.139, 5.191};
  double *binsEta = new double[nBinsEta];
  for(int i = 0; i<=nBinsEta; ++i)
    binsEta[i] = etaEdge[i];

  fhEventSel = new TH1F("fhEventSel","fhEventSel",10,0,10);
  fOutput->Add(fhEventSel);

  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  fhNPV = new TH1F("fhNPV","fhNPV",nBinsNPV,minNPV,maxNPV);
  fOutput->Add(fhNPV);

  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = Form("fh2PtEtaNoMatching_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh2PtEtaNoMatching[i] = new TH2F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsEta,binsEta);
    fOutput->Add(fh2PtEtaNoMatching[i]);

    histName = Form("fh3PtEtaPhiNotMatched_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh3PtEtaPhiNotMatched[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
    fOutput->Add(fh3PtEtaPhiNotMatched[i]);

    histName = Form("fh3PtEtaPhiMatched_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh3PtEtaPhiMatched[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
    fOutput->Add(fh3PtEtaPhiMatched[i]);
  
    histName = Form("fh3PtTrueNPVDeltaPt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;#it{p}_{T,rec}-#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVDeltaPt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPt,binsDPt);
    fOutput->Add(fh3PtTrueNPVDeltaPt[i]);

    histName = Form("fh3PtTrueNPVDeltaPtRel_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;(#it{p}_{T,rec}-#it{p}_{T,gen})/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVDeltaPtRel[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPtRel,binsDPtRel);
    fOutput->Add(fh3PtTrueNPVDeltaPtRel[i]);

    histName = Form("fh3PtTrueNPVScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueNPVScalePt[i]);

    histName = Form("fh3PtTruePtSubNPV_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#it{p}_{T,rec};NPV",histName.Data());
    fh3PtTruePtSubNPV[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsNPV,binsNPV);
    fOutput->Add(fh3PtTruePtSubNPV[i]);

    histName = Form("fh3PtTrueEtaDeltaPt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}-#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaDeltaPt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPt,binsDPt);
    fOutput->Add(fh3PtTrueEtaDeltaPt[i]);

    histName = Form("fh3PtTrueEtaDeltaPtRel_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;(#it{p}_{T,rec}-#it{p}_{T,gen})/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaDeltaPtRel[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPtRel,binsDPtRel);
    fOutput->Add(fh3PtTrueEtaDeltaPtRel[i]);

    histName = Form("fh3PtTrueEtaScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueEtaScalePt[i]);

    histName = Form("fh3PtTruePtSubEta_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#it{p}_{T,rec};Eta",histName.Data());
    fh3PtTruePtSubEta[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsEta,binsEta);
    fOutput->Add(fh3PtTruePtSubEta[i]);
  }
  
  if(binsPhi)               delete [] binsPhi;
  if(binsEta)               delete [] binsEta;
  if(binsPtPart)            delete [] binsPtPart;
  if(binsPtDet)             delete [] binsPtDet;
  if(binsDPt)               delete [] binsDPt;
  if(binsDPtRel)            delete [] binsDPtRel;
  if(binsScalePt)           delete [] binsScalePt;
  if(binsNPV)               delete [] binsNPV;

}

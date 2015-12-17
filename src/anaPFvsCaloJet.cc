#include "UserCode/diall/interface/anaPFvsCaloJet.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaPFvsCaloJet)
   
anaPFvsCaloJet::anaPFvsCaloJet(const char *name, const char *title) 
:anaBaseTask(name,title),
  fJetsName(""),
  fJetsCont(0x0),
  fJets2Name(""),
  fJets2Cont(0x0),
  fNjet(0),
  // fJetPt(0.),
  // fJetEta(0.),
  // fJetPhi(0.),
  // fChargedSum(0.),
  // fChargedMax(0.),
  // fChargedN(0),
  // fPhotonSum(0.),
  // fPhotonMax(0.),
  // fPhotonN(0),
  // fNeutralSum(0.),
  // fNeutralMax(0.),
  // fNeutralN(0),
  // fNConst(0),
  // fJetPtMatch(0.),
  // fDeltaR(0.),
  fStoreTree(false),
  fTreeOut(0x0),
  fhEventSel(),
  fhCentrality(),
  fhNPV(),
  fh2PtEtaNoMatching(),
  fh3PtEtaPhiNotMatched(),
  fh3PtEtaPhiMatched(),
  fh3PtTrueNPVDeltaPt(),
  fh3PtTrueNPVDeltaPtRel(),
  fh3PtTrueNPVScalePt(),
  fh3PtTruePtSubNPV(),
  fh3PtTrueEtaDeltaPt(),
  fh3PtTrueEtaDeltaPtRel(),
  fh3PtTrueEtaScalePt(),
  fh3PtTruePtSubEta()
{


}

//----------------------------------------------------------
void anaPFvsCaloJet::Exec(Option_t * /*option*/)
{
 
  anaBaseTask::Exec();
  if(!SelectEvent()) return;


  //printf("anaPFvsCaloJet executing\n");
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

   //  Printf("event accepted");
   
   int npv = 1;
   if(fHiEvent) npv = fHiEvent->GetNPV();
   fhNPV->Fill(npv);

   fRun = fHiEvent->GetRun();
   fLumi = fHiEvent->GetLumi();
   fEvent = fHiEvent->GetEvent();

   fhEventSel->Fill(0.5);

   float weight = 1.;
   if(fHiEvent) {
     if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
   }

   for(int i = 0; i<200; ++i) {
     fJetPt[i]  = -1.;
     fJetRawPt[i]  = -1.;
     fJetEta[i] = -1.;
     fJetPhi[i] = -1;
     fChargedSum[i] = -1;
     fChargedMax[i] = -1;
     fChargedN[i] = -1;
     fChargedHardSum[i] = -1;
     fChargedHardMax[i] = -1;
     fChargedHardN[i] = -1;
     fPhotonSum[i] = -1;
     fPhotonMax[i] = -1;
     fPhotonN[i] = -1;
     fNeutralSum[i] = -1;
     fNeutralMax[i] = -1;
     fNeutralN[i] = -1;
     fEmSum[i] = -1;
     fEmMax[i] = -1;
     fEmN[i] = -1;
     fMuSum[i] = -1;
     fMuMax[i] = -1;
     fMuN[i] = -1;
     fNConst[i] = -1;
     fJetPtMatch[i] = -1;
     fDeltaR[i] = -1;
   }
   
   fNjet = 0;

   for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ij++) {
     lwJet *jet1 = fJetsCont->GetJet(ij);
     if(!jet1) continue;

     bool saveJet = false;
     if(jet1->Pt()>30.) saveJet = true;
     if(!saveJet) continue;
     if(saveJet) {
       fJetPt[fNjet]  = (float)jet1->Pt();
       fJetRawPt[fNjet]  = (float)jet1->GetRawPt();
       fJetEta[fNjet] = (float)jet1->Eta();
       fJetPhi[fNjet] = (float)jet1->Phi();
       fChargedSum[fNjet] = (float)jet1->GetChargedSum();
       fChargedMax[fNjet] = (float)jet1->GetChargedMax();
       fChargedN[fNjet] = (int)jet1->GetChargedN();
       fChargedHardSum[fNjet] = (float)jet1->GetChargedHardSum();
       fChargedHardMax[fNjet] = (float)jet1->GetChargedHardMax();
       fChargedHardN[fNjet] = (int)jet1->GetChargedHardN();
       fPhotonSum[fNjet] = (float)jet1->GetPhotonSum();
       fPhotonMax[fNjet] = (float)jet1->GetPhotonMax();
       fPhotonN[fNjet] = jet1->GetPhotonN();
       fNeutralSum[fNjet] = (float)jet1->GetNeutralSum();
       fNeutralMax[fNjet] = (float)jet1->GetNeutralMax();
       fNeutralN[fNjet] = jet1->GetNeutralN();
       fEmSum[fNjet] = (float)jet1->GetEmSum();
       fEmMax[fNjet] = (float)jet1->GetEmMax();
       fEmN[fNjet] = jet1->GetEmN();
       fMuSum[fNjet] = (float)jet1->GetMuSum();
       fMuMax[fNjet] = (float)jet1->GetMuMax();
       fMuN[fNjet] = jet1->GetMuN();
       int nconst = jet1->GetChargedN() + jet1->GetPhotonN() + jet1->GetNeutralN() + jet1->GetEmN() + jet1->GetMuN();
       fNConst[fNjet] = nconst;
     }
     
     
     fh2PtEtaNoMatching->Fill(jet1->Pt(),jet1->Eta(),weight);
          
     int id = jet1->GetMatchId1();
     lwJet *jet2 = fJets2Cont->GetJet(id);
     if(!jet2) {
       fh3PtEtaPhiNotMatched->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);
       if(saveJet) {
         fJetPtMatch[fNjet] = -1.;
         fDeltaR[fNjet] = -1.;
         fNjet++;
       }
       continue;
     }

     if(saveJet) {
       fJetPtMatch[fNjet] = (float)jet2->Pt();
       float dr = (float)jet1->DeltaR(jet2);
       fDeltaR[fNjet] = dr;
     }
     
     fh3PtEtaPhiMatched->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);
     
     double dpt = jet2->Pt()-jet1->Pt();

     fh3PtTrueEtaDeltaPt->Fill(jet1->Pt(),jet1->Eta(),dpt,weight);
     if(jet1->Pt()>0.) {
       fh3PtTrueEtaDeltaPtRel->Fill(jet1->Pt(),jet1->Eta(),dpt/jet1->Pt(),weight);
       fh3PtTrueEtaScalePt->Fill(jet1->Pt(),jet1->Eta(),jet2->Pt()/jet1->Pt(),weight);
     }
     fh3PtTruePtSubEta->Fill(jet1->Pt(),jet2->Pt(),jet1->Eta(),weight);
     
     if(abs(jet1->Eta())<1.3) {
       
       fh3PtTrueNPVDeltaPt->Fill(jet1->Pt(),npv,dpt,weight);
       if(jet1->Pt()>0.) {
         fh3PtTrueNPVDeltaPtRel->Fill(jet1->Pt(),npv,dpt/jet1->Pt(),weight);
         fh3PtTrueNPVScalePt->Fill(jet1->Pt(),npv,jet2->Pt()/jet1->Pt(),weight);
       }
       fh3PtTruePtSubNPV->Fill(jet1->Pt(),jet2->Pt(),npv,weight);
     }

     if(saveJet) fNjet++;
   }
   if(fStoreTree) fTreeOut->Fill();


}
//----------------------------------------------------------
void anaPFvsCaloJet::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaPFvsCaloJet: fOutput not present");
    return;
  }

  if(fStoreTree) {
    fTreeOut = new TTree(Form("%sTree",GetName()),"jet matching tree");
    fTreeOut->Branch("run",	&fRun,	"run/I");
    fTreeOut->Branch("lumi",	&fLumi,	"lumi/I");
    fTreeOut->Branch("event",	&fEvent, "event/I");
    fTreeOut->Branch("njet",	&fNjet,	"njet/I");
    fTreeOut->Branch("jetpt",	&fJetPt,	"jetpt[njet]/F");
    fTreeOut->Branch("jetrawpt",	&fJetRawPt,	"jetrawpt[njet]/F");
    fTreeOut->Branch("jeteta",	&fJetEta,	"jeteta[njet]/F");
    fTreeOut->Branch("jetphi",	&fJetPhi,	"jetphi[njet]/F");
    fTreeOut->Branch("chargedSum",&fChargedSum,	"chargedSum[njet]/F");
    fTreeOut->Branch("chargedMax",&fChargedMax,	"chargedMax[njet]/F");
    fTreeOut->Branch("chargedN",  &fChargedN,	"chargedN[njet]/I");
    fTreeOut->Branch("chargedHardSum",&fChargedHardSum,	"chargedHardSum[njet]/F");
    fTreeOut->Branch("chargedHardMax",&fChargedHardMax,	"chargedHardMax[njet]/F");
    fTreeOut->Branch("chargedHardN",  &fChargedHardN,	"chargedHardN[njet]/I");
    fTreeOut->Branch("photonSum", &fPhotonSum,	"photonSum[njet]/F");
    fTreeOut->Branch("photonMax", &fPhotonMax,	"photonMax[njet]/F");
    fTreeOut->Branch("photonN",   &fPhotonN,	"photonN[njet]/I");
    fTreeOut->Branch("neutralSum",&fNeutralSum,	"neutralSum[njet]/F");
    fTreeOut->Branch("neutralMax",&fNeutralMax,	"neutralMax[njet]/F");
    fTreeOut->Branch("neutralN",  &fNeutralN,	"neutralN[njet]/I");
    fTreeOut->Branch("emSum",     &fEmSum,	"emSum[njet]/F");
    fTreeOut->Branch("emMax",     &fEmMax,	"emMax[njet]/F");
    fTreeOut->Branch("emN",       &fEmN,	        "emN[njet]/I");
    fTreeOut->Branch("muSum",     &fMuSum,	"muSum[njet]/F");
    fTreeOut->Branch("muMax",     &fMuMax,	"muMax[njet]/F");
    fTreeOut->Branch("muN",       &fMuN,	        "muN[njet]/I");
    fTreeOut->Branch("nconst",   &fNConst,	"nconst[njet]/I");
    fTreeOut->Branch("jetptmatch",&fJetPtMatch,	"jetptmatch[njet]/F");
    fTreeOut->Branch("deltaR",	&fDeltaR,	"deltaR[njet]/F");
  
    fOutput->Add(fTreeOut);
  }

  TString histTitle;
  TString histName;

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

  histName = "fh2PtEtaNoMatching";
  histTitle = Form("%s;#it{p}_{T,PF};#eta;",histName.Data());
  fh2PtEtaNoMatching = new TH2F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsEta,binsEta);
  fOutput->Add(fh2PtEtaNoMatching);

  histName = "fh3PtEtaPhiNotMatched";
  histTitle = Form("%s;#it{p}_{T,PF};#eta;",histName.Data());
  fh3PtEtaPhiNotMatched = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
  fOutput->Add(fh3PtEtaPhiNotMatched);

  histName = "fh3PtEtaPhiMatched";
  histTitle = Form("%s;#it{p}_{T,PF};#eta;",histName.Data());
  fh3PtEtaPhiMatched = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
  fOutput->Add(fh3PtEtaPhiMatched);
  
  histName = Form("fh3PtTrueNPVDeltaPt");
  histTitle = Form("%s;#it{p}_{T,PF};NPV;#it{p}_{T,calo}-#it{p}_{T,PF}",histName.Data());
  fh3PtTrueNPVDeltaPt = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPt,binsDPt);
  fOutput->Add(fh3PtTrueNPVDeltaPt);

  histName = Form("fh3PtTrueNPVDeltaPtRel");
  histTitle = Form("%s;#it{p}_{T,PF};NPV;(#it{p}_{T,calo}-#it{p}_{T,PF})/#it{p}_{T,PF}",histName.Data());
  fh3PtTrueNPVDeltaPtRel = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPtRel,binsDPtRel);
  fOutput->Add(fh3PtTrueNPVDeltaPtRel);

  histName = Form("fh3PtTrueNPVScalePt");
  histTitle = Form("%s;#it{p}_{T,PF};NPV;#it{p}_{T,calo}/#it{p}_{T,PF}",histName.Data());
  fh3PtTrueNPVScalePt = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsScalePt,binsScalePt);
  fOutput->Add(fh3PtTrueNPVScalePt);

  histName = Form("fh3PtTruePtSubNPV");
  histTitle = Form("%s;#it{p}_{T,PF};#it{p}_{T,calo};NPV",histName.Data());
  fh3PtTruePtSubNPV = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsNPV,binsNPV);
  fOutput->Add(fh3PtTruePtSubNPV);

  histName = Form("fh3PtTrueEtaDeltaPt");
  histTitle = Form("%s;#it{p}_{T,PF};Eta;#it{p}_{T,calo}-#it{p}_{T,PF}",histName.Data());
  fh3PtTrueEtaDeltaPt = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPt,binsDPt);
  fOutput->Add(fh3PtTrueEtaDeltaPt);

  histName = Form("fh3PtTrueEtaDeltaPtRel");
  histTitle = Form("%s;#it{p}_{T,PF};Eta;(#it{p}_{T,calo}-#it{p}_{T,PF})/#it{p}_{T,PF}",histName.Data());
  fh3PtTrueEtaDeltaPtRel = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPtRel,binsDPtRel);
  fOutput->Add(fh3PtTrueEtaDeltaPtRel);

  histName = Form("fh3PtTrueEtaScalePt");
  histTitle = Form("%s;#it{p}_{T,PF};Eta;#it{p}_{T,calo}/#it{p}_{T,PF}",histName.Data());
  fh3PtTrueEtaScalePt = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScalePt,binsScalePt);
  fOutput->Add(fh3PtTrueEtaScalePt);

  histName = Form("fh3PtTruePtSubEta");
  histTitle = Form("%s;#it{p}_{T,PF};#it{p}_{T,calo};Eta",histName.Data());
  fh3PtTruePtSubEta = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsEta,binsEta);
  fOutput->Add(fh3PtTruePtSubEta);

  if(binsPhi)               delete [] binsPhi;
  if(binsEta)               delete [] binsEta;
  if(binsPtPart)            delete [] binsPtPart;
  if(binsPtDet)             delete [] binsPtDet;
  if(binsDPt)               delete [] binsDPt;
  if(binsDPtRel)            delete [] binsDPtRel;
  if(binsScalePt)           delete [] binsScalePt;
  if(binsNPV)               delete [] binsNPV;

}

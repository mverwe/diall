#include "UserCode/diall/interface/anaJetQA.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetQA)
   
anaJetQA::anaJetQA(const char *name, const char *title) 
:anaBaseTask(name,title),
  fJetsName(""),
  fJetsCont(),
  fJetIdType(0),
  fh3PtEtaPhi(),
  fh3PtEtaPhiJetID(),
  fh3PtEtaArea(),
  fh3PtEtaCHF(),
  fh3PtEtaNHF(),
  fh3PtEtaCEF(),
  fh3PtEtaNEF(),
  fh3PtEtaMUF(),
  fh3PtEtaCHM(),
  fh3PtEtaNHM(),
  fh3PtEtaCEM(),
  fh3PtEtaNEM(),
  fh3PtEtaMUM(),
  fh3PtEtaCHFCut(),
  fh3PtEtaNHFCut(),
  fh3PtEtaCEFCut(),
  fh3PtEtaNEFCut(),
  fh3PtEtaMUFCut(),
  fh3PtEtaCHMCut(),
  fh3PtEtaNHMCut(),
  fh3PtEtaCEMCut(),
  fh3PtEtaNEMCut(),
  fh3PtEtaMUMCut()
{

}

//----------------------------------------------------------
void anaJetQA::Exec(Option_t * /*option*/)
{
  //  printf("anaJetQA executing\n");
  anaBaseTask::Exec();
  if(!SelectEvent()) return;
  
  if(!fInitOutput) CreateOutputObjects();

   if(!fJetsCont && !fJetsName.IsNull())
     fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
   if(!fJetsCont) return;

   Double_t maxPt = 0.;
   Int_t njets = 0;
   for (int i = 0; i < fJetsCont->GetNJets(); i++) {
     lwJet *jet = fJetsCont->GetJet(i);
     Double_t pt = jet->Pt();
     Double_t phi = jet->Phi();
     Double_t eta = jet->Eta();
     //      Double_t m = jet->M();
     
     if(pt<1e-6) continue; //remove ghosts
     
     fh3PtEtaPhi->Fill(pt,eta,phi);
     fh3PtEtaArea->Fill(pt,eta,jet->GetArea());
     
     fh3PtEtaCHF->Fill(pt,eta,jet->GetCHF());
     fh3PtEtaNHF->Fill(pt,eta,jet->GetNHF());
     fh3PtEtaCEF->Fill(pt,eta,jet->GetCEF());
     fh3PtEtaNEF->Fill(pt,eta,jet->GetNEF());
     fh3PtEtaMUF->Fill(pt,eta,jet->GetMUF());
     
     fh3PtEtaCHM->Fill(pt,eta,(float)jet->GetCHM());
     fh3PtEtaNHM->Fill(pt,eta,jet->GetNHM());
     fh3PtEtaCEM->Fill(pt,eta,jet->GetCEM());
     fh3PtEtaNEM->Fill(pt,eta,jet->GetNEM());
     fh3PtEtaMUM->Fill(pt,eta,jet->GetMUM());

     //pp 13 TeV cuts Loose,Tight,TightLepVeto
     float maxnhf[3] = {0.99,0.9,0.9};
     float maxnef[3] = {0.99,0.9,0.9};
     int minconst[3] = {2,2,2};
     float maxmuf[3] = {1.1,1.1,0.8};
     float minchf[3] = {0.,0.,0.};
     int  minchm[3]  = {2,2,2};
     float maxcef[3] = {0.99,0.99,0.9};

     int id = fJetIdType;
     
     int nconst = jet->GetCHM() + jet->GetNHM() + jet->GetCEM() + jet->GetNEM() + jet->GetMUM();

     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaPhiJetID->Fill(pt,eta,phi);

     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaCHFCut->Fill(pt,eta,jet->GetCHF());
     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaCHMCut->Fill(pt,eta,jet->GetCHM());

     if(jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaNHFCut->Fill(pt,eta,jet->GetNHF());
     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaNHMCut->Fill(pt,eta,jet->GetNHM());

     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id])
       fh3PtEtaCEFCut->Fill(pt,eta,jet->GetCEF());
     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaCEMCut->Fill(pt,eta,jet->GetCEM());

     if(jet->GetNHF()<maxnhf[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaNEFCut->Fill(pt,eta,jet->GetNEF());
     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaNEMCut->Fill(pt,eta,jet->GetNEM());

     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaMUFCut->Fill(pt,eta,jet->GetMUF());
     if(jet->GetNHF()<maxnhf[id] && jet->GetNEF()<maxnef[id] && nconst>=minconst[id] && jet->GetMUF()<maxmuf[id] && jet->GetCHF()>minchf[id] && jet->GetCHM()>=minchm[id] && jet->GetCEF()<maxcef[id])
       fh3PtEtaMUMCut->Fill(pt,eta,jet->GetMUM());
     
     if(pt>maxPt) maxPt = pt;
     njets++;
   }
   fh1NJets->Fill(njets);
}

//----------------------------------------------------------
void anaJetQA::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaJetQA: fOutput not present");
    return;
  }

  TString histName;
  TString histTitle;

  fh1NJets = new TH1F("fh1NJets","fh1NJets;N_{jets}",500,0,500);
  fOutput->Add(fh1NJets);
  
  fh3PtEtaPhi = new TH3F("fh3PtEtaPhi","fh3PtEtaPhi;pt;eta;phi",500,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
  fOutput->Add(fh3PtEtaPhi);

  fh3PtEtaPhiJetID = new TH3F("fh3PtEtaPhiJetID","fh3PtEtaPhiJetID;pt;eta;phi",500,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
  fOutput->Add(fh3PtEtaPhiJetID);
  
  fh3PtEtaArea = new TH3F("fh3PtEtaArea","fh3PtEtaArea;pt;eta;A",500,0,500,100,-5,5,100,0,1);
  fOutput->Add(fh3PtEtaArea);

  histName = "fh3PtEtaCHF";
  histTitle = Form("%s;p_{T};#eta;CHF",histName.Data());
  fh3PtEtaCHF = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaCHF);

  histName = "fh3PtEtaNHF";
  histTitle = Form("%s;p_{T};#eta;NHF",histName.Data());
  fh3PtEtaNHF = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaNHF);

  histName = "fh3PtEtaCEF";
  histTitle = Form("%s;p_{T};#eta;CEF",histName.Data());
  fh3PtEtaCEF = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaCEF);

  histName = "fh3PtEtaNEF";
  histTitle = Form("%s;p_{T};#eta;NEF",histName.Data());
  fh3PtEtaNEF = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaNEF);

  histName = "fh3PtEtaMUF";
  histTitle = Form("%s;p_{T};#eta;MUF",histName.Data());
  fh3PtEtaMUF = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaMUF);

  histName = "fh3PtEtaCHM";
  histTitle = Form("%s;p_{T};#eta;CHF",histName.Data());
  fh3PtEtaCHM = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaCHM);

  histName = "fh3PtEtaNHM";
  histTitle = Form("%s;p_{T};#eta;NHF",histName.Data());
  fh3PtEtaNHM = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaNHM);

  histName = "fh3PtEtaCEM";
  histTitle = Form("%s;p_{T};#eta;CEF",histName.Data());
  fh3PtEtaCEM = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaCEM);

  histName = "fh3PtEtaNEM";
  histTitle = Form("%s;p_{T};#eta;NEF",histName.Data());
  fh3PtEtaNEM = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaNEM);

  histName = "fh3PtEtaMUM";
  histTitle = Form("%s;p_{T};#eta;MUF",histName.Data());
  fh3PtEtaMUM = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaMUM);
  
  //
  histName = "fh3PtEtaCHFCut";
  histTitle = Form("%s;p_{T};#eta;CHF",histName.Data());
  fh3PtEtaCHFCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaCHFCut);

  histName = "fh3PtEtaNHFCut";
  histTitle = Form("%s;p_{T};#eta;NHF",histName.Data());
  fh3PtEtaNHFCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaNHFCut);

  histName = "fh3PtEtaCEFCut";
  histTitle = Form("%s;p_{T};#eta;CEF",histName.Data());
  fh3PtEtaCEFCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaCEFCut);

  histName = "fh3PtEtaNEFCut";
  histTitle = Form("%s;p_{T};#eta;NEF",histName.Data());
  fh3PtEtaNEFCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaNEFCut);

  histName = "fh3PtEtaMUFCut";
  histTitle = Form("%s;p_{T};#eta;MUF",histName.Data());
  fh3PtEtaMUFCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,101,0,1.01);
  fOutput->Add(fh3PtEtaMUFCut);

  histName = "fh3PtEtaCHMCut";
  histTitle = Form("%s;p_{T};#eta;CHF",histName.Data());
  fh3PtEtaCHMCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaCHMCut);

  histName = "fh3PtEtaNHMCut";
  histTitle = Form("%s;p_{T};#eta;NHF",histName.Data());
  fh3PtEtaNHMCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaNHMCut);

  histName = "fh3PtEtaCEMCut";
  histTitle = Form("%s;p_{T};#eta;CEF",histName.Data());
  fh3PtEtaCEMCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaCEMCut);

  histName = "fh3PtEtaNEMCut";
  histTitle = Form("%s;p_{T};#eta;NEF",histName.Data());
  fh3PtEtaNEMCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaNEMCut);

  histName = "fh3PtEtaMUMCut";
  histTitle = Form("%s;p_{T};#eta;MUF",histName.Data());
  fh3PtEtaMUMCut = new TH3F(histName.Data(),histTitle.Data(),500,0,500,100,-5,5,100,0,100);
  fOutput->Add(fh3PtEtaMUMCut);
}

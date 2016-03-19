#include "UserCode/diall/interface/anaJetQA.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetQA)
   
anaJetQA::anaJetQA(const char *name, const char *title) 
:anaBaseTask(name,title),
  fJetsName(""),
  fJetsCont(),
  fh3PtEtaPhi(),
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
   //printf("anaJetQA executing\n");
  if(!fInitOutput) CreateOutputObjects();

   if(!fJetsCont && !fJetsName.IsNull())
     fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
   if(!fJetsCont) return;

   TClonesArray *jets = fJetsCont->GetJets();

   Double_t maxPt = 0.;
   Int_t njets = 0;
   for (int i = 0; i < fJetsCont->GetNJets(); i++) {
     lwJet *jet = static_cast<lwJet*>(jets->At(i));
     Double_t pt = jet->Pt();
     Double_t phi = jet->Phi();
     Double_t eta = jet->Eta();
     //      Double_t m = jet->M();
     if(fabs(pt-0.)<1e-6) continue; //remove ghosts
     fh3PtEtaPhi->Fill(pt,eta,phi);
     fh3PtEtaArea->Fill(pt,eta,jet->GetArea());

     fh3PtEtaCHF->Fill(pt,eta,jet->GetCHF());
     fh3PtEtaNHF->Fill(pt,eta,jet->GetNHF());
     fh3PtEtaCEF->Fill(pt,eta,jet->GetCEF());
     fh3PtEtaNEF->Fill(pt,eta,jet->GetNEF());
     fh3PtEtaMUF->Fill(pt,eta,jet->GetMUF());

     fh3PtEtaCHM->Fill(pt,eta,jet->GetCHM());
     fh3PtEtaNHM->Fill(pt,eta,jet->GetNHM());
     fh3PtEtaCEM->Fill(pt,eta,jet->GetCEM());
     fh3PtEtaNEM->Fill(pt,eta,jet->GetNEM());
     fh3PtEtaMUM->Fill(pt,eta,jet->GetMUM());

     //pp 13 TeV cuts TightLepVeto
     int nconst = jet->GetCHM() + jet->GetNHM() + jet->GetCEM() + jet->GetNEM() + jet->GetMUM();
     
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaCHFCut->Fill(pt,eta,jet->GetCHF());
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCEF()<0.9)
       fh3PtEtaCHMCut->Fill(pt,eta,jet->GetCHM());

     if(jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaNHFCut->Fill(pt,eta,jet->GetNHF());
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaNHMCut->Fill(pt,eta,jet->GetNHM());

     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0)
       fh3PtEtaCEFCut->Fill(pt,eta,jet->GetCEF());
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaCEMCut->Fill(pt,eta,jet->GetCEM());

     if(jet->GetNHF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaNEFCut->Fill(pt,eta,jet->GetNEF());
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaNEMCut->Fill(pt,eta,jet->GetNEM());

     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
       fh3PtEtaMUFCut->Fill(pt,eta,jet->GetMUF());
     if(jet->GetNHF()<0.9 && jet->GetNEF()<0.9 && nconst>1 && jet->GetMUF()<0.8 && jet->GetCHF()>0. && jet->GetCHM()>0 && jet->GetCEF()<0.9)
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

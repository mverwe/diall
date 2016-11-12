#include "UserCode/diall/interface/anaJetMass.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetMass)
   
anaJetMass::anaJetMass(const char *name, const char *title) 
:anaBaseTask(name,title),
  fJetsName(""),
  fJetsCont(0x0),
  fh3PtEtaPhi(),
  fh3PtEtaArea(),
  fh3PtEtaMass()
{

}

//----------------------------------------------------------
void anaJetMass::Exec(Option_t * /*option*/)
{
  //  printf("anaJetMass executing\n");
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
     if(!jet) continue;
     Double_t pt  = jet->Pt();
     Double_t phi = jet->Phi();
     Double_t eta = jet->Eta();
     Double_t m   = jet->M();
     
     if(pt<1e-6) continue; //remove ghosts
     
     fh3PtEtaPhi->Fill(pt,eta,phi);
     fh3PtEtaArea->Fill(pt,eta,jet->GetArea());
     fh3PtEtaMass->Fill(pt,eta,m);
   
     
     if(pt>maxPt) maxPt = pt;
     njets++;
   }
   fh1NJets->Fill(njets);
}

//----------------------------------------------------------
void anaJetMass::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();
  
  if(!fOutput) {
    Printf("anaJetMass: fOutput not present");
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

  fh3PtEtaMass = new TH3F("fh3PtEtaMass","fh3PtEtaMass;pt;eta;M",500,0,500,100,-5,5,100,0,100.);
  fOutput->Add(fh3PtEtaMass);

}

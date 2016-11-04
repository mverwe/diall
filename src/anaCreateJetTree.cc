#include "UserCode/diall/interface/anaCreateJetTree.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaCreateJetTree)
   
anaCreateJetTree::anaCreateJetTree(const char *name, const char *title) 
:anaBaseTask(name,title),
  fNcentBins(4),
  fJetsGenName(""),
  fJetsGenCont(0x0),
  fJetsRecName(""),
  fJetsRecCont(0x0),
  fUseForestMatching(false),
  fUseRawPt(false),
  fRhoMapName(""),
  fRhoMap(0x0),
  fMaxDist(0.4),
  fRefPartonFlavorMin(-1),
  fRefPartonFlavorMax(-1),
  fMinJetPtRec(0.),
  fOutJetTreeVars()
  //  fForestJets()
{

}

//----------------------------------------------------------
void anaCreateJetTree::Exec(Option_t * /*option*/)
{
 
  anaBaseTask::Exec();
  if(!SelectEvent()) return;

  //printf("anaCreateJetTree executing\n");
  if(!fInitOutput) CreateOutputObjects();

  //Get event properties
  if(fHiEvent) {
    if(abs(fHiEvent->GetVz())>15.) {
      //Printf("%s: Didn't pass vertex selection: %f",GetName(),fHiEvent->GetVz());
      return;
    }
  }
  
  //Get base jets
  if(!fJetsGenCont && !fJetsGenName.IsNull())
    fJetsGenCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsGenName.Data()));
  if(!fJetsGenCont && !fUseForestMatching) {
    Printf("%s: 1 Cannot find %s",GetName(),fJetsGenName.Data());
    return;
  }
   //Get tag jets
   if(!fJetsRecCont && !fJetsRecName.IsNull())
     fJetsRecCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsRecName.Data()));
   if(!fJetsRecCont) {
     Printf("%s: 2 Cannot find %s",GetName(),fJetsRecName.Data());
     return;
   }
   const Int_t nJetsRec = fJetsRecCont->GetNJets();
   if(nJetsRec==0) return;

   //get rho background density map
   if(!fRhoMap && !fRhoMapName.IsNull())
     fRhoMap = dynamic_cast<rhoMap*>(fEventObjects->FindObject(fRhoMapName.Data()));
 
   //Get MC weight
   // float weight = 1.;
   // if(fHiEvent) {
   //   if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
   // }

   //Determine centrality bin
   Double_t cent = 0.;
   if(fHiEvent) cent = fHiEvent->GetCentrality();

   //number of generated pile-up vertices
   // int npv = 1;
   // if(fHiEvent) npv = fHiEvent->GetNPV();
   
   //clear vectors in output tree  
   ClearOutJetTreeVars();

   fOutJetTreeVars.fRun = -999.;
   if(fHiEvent)
     fOutJetTreeVars.fRun = fHiEvent->GetRun();
   fOutJetTreeVars.fCent = cent;
   
   for(Int_t ij = 0; ij<fJetsRecCont->GetNJets(); ij++) {
     lwJet *jet = fJetsRecCont->GetJet(ij);
     if(!jet) continue;

     double rawpt = jet->GetRawPt();
     double jtpt = jet->Pt();
     double jteta = jet->Eta();
     double jtphi = jet->Phi();
     double jtm = jet->M();
     double refpt = -1.;

     if(fUseForestMatching) {
       //reject unmatched or badly matched jets
       if(jet->GetRefDr()>fMaxDist
          || jet->GetSubEvent()!=0)
         continue;
       refpt = jet->GetRefPt();
     } else {
       int id = jet->GetMatchId1();
       lwJet *jetGen = fJetsGenCont->GetJet(id);
       if(!jetGen) continue;
       double dR = jet->DeltaR(jetGen);
       if(dR>fMaxDist) continue;
       refpt = jetGen->Pt();
     }

     if(refpt<fMinJetPtRec) continue;

     fOutJetTreeVars.fPt.push_back(jtpt);
     fOutJetTreeVars.fPtRaw.push_back(rawpt);
     fOutJetTreeVars.fEta.push_back(jteta);
     fOutJetTreeVars.fPhi.push_back(jtphi);
     fOutJetTreeVars.fM.push_back(jtm);
     fOutJetTreeVars.fPtRef.push_back(refpt);
   }
   fTreeOut->Fill();
  
}
//----------------------------------------------------------
void anaCreateJetTree::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaCreateJetTree: fOutput not present");
    return;
  }

  fTreeOut = new TTree(Form("%sTree",GetName()),"jet tree");
  fTreeOut->Branch("fRun",&fOutJetTreeVars.fRun,"fRun/I");
  fTreeOut->Branch("fCent",&fOutJetTreeVars.fCent,"fCent/F");
  fTreeOut->Branch("fPt",&fOutJetTreeVars.fPt);
  fTreeOut->Branch("fPtRaw",&fOutJetTreeVars.fPtRaw);
  fTreeOut->Branch("fEta",&fOutJetTreeVars.fEta);
  fTreeOut->Branch("fPhi",&fOutJetTreeVars.fPhi);
  fTreeOut->Branch("fM",&fOutJetTreeVars.fM);
  fTreeOut->Branch("fPtRef",&fOutJetTreeVars.fPtRef);

  fOutput->Add(fTreeOut);
  
  // TString histTitle = "";
  // TString histName  = "";

}

//----------------------------------------------------------
void anaCreateJetTree::ClearOutJetTreeVars() {
  //clear vectors in fOutJetTreeVars
  fOutJetTreeVars.fPt.clear();
  fOutJetTreeVars.fPtRaw.clear();
  fOutJetTreeVars.fEta.clear();
  fOutJetTreeVars.fPhi.clear();
  fOutJetTreeVars.fM.clear();
  fOutJetTreeVars.fPtRef.clear();
}

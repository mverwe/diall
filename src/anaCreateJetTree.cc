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
  fMinJetPtRef(0.),
  fStoreSubjets(false),
  fStorePFIdInfo(false),
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
     double refeta = -999.;
     double matchpt = -1.;
     double matcheta = -999.;

     if(jtpt<fMinJetPtRec) continue;

     lwJet *jetGen;
     
     if(fUseForestMatching) {
       //reject unmatched or badly matched jets
       if(jet->GetRefDr()>fMaxDist
          || jet->GetSubEvent()!=0)
         continue;
       refpt = jet->GetRefPt();
       refeta = jet->GetRefEta();

       int id = jet->GetMatchId1();
       jetGen = fJetsGenCont->GetJet(id);
       if(!jetGen) continue;
       double dR = jet->DeltaR(jetGen);
       if(dR>fMaxDist) continue;
       matchpt = jetGen->Pt();
       matcheta = jetGen->Eta();       
     } else {
       int id = jet->GetMatchId1();
       jetGen = fJetsGenCont->GetJet(id);
       if(!jetGen) continue;
       double dR = jet->DeltaR(jetGen);
       if(dR>fMaxDist) continue;
       refpt = jetGen->Pt();
       refeta = jetGen->Eta();
       matchpt = refpt;
       matcheta = refeta;
     }

     if(refpt<fMinJetPtRef) continue;
     
     fOutJetTreeVars.fPt.push_back(jtpt);
     fOutJetTreeVars.fPtRaw.push_back(rawpt);
     fOutJetTreeVars.fEta.push_back(jteta);
     fOutJetTreeVars.fPhi.push_back(jtphi);
     fOutJetTreeVars.fM.push_back(jtm);
     if(fStoreSubjets) {
       fOutJetTreeVars.fSubJetPt.push_back(jet->GetSubJetPt());
       fOutJetTreeVars.fSubJetEta.push_back(jet->GetSubJetEta());
       fOutJetTreeVars.fSubJetPhi.push_back(jet->GetSubJetPhi());
       fOutJetTreeVars.fSubJetM.push_back(jet->GetSubJetM());
     }
     
     fOutJetTreeVars.fPtRef.push_back(refpt);
     fOutJetTreeVars.fEtaRef.push_back(refeta);
     fOutJetTreeVars.fPtMatch.push_back(matchpt);
     fOutJetTreeVars.fEtaMatch.push_back(matcheta);

     if(fStorePFIdInfo) {
       if(fUseForestMatching) {
         fOutJetTreeVars.fCHF.push_back(jet->GetCHF());
         fOutJetTreeVars.fNHF.push_back(jet->GetNHF());
         fOutJetTreeVars.fCEF.push_back(jet->GetCEF());
         fOutJetTreeVars.fNEF.push_back(jet->GetNEF());
         fOutJetTreeVars.fMUF.push_back(jet->GetMUF());
       
         fOutJetTreeVars.fCHM.push_back(jet->GetCHM());
         fOutJetTreeVars.fNHM.push_back(jet->GetNHM());
         fOutJetTreeVars.fCEM.push_back(jet->GetCEM());
         fOutJetTreeVars.fNEM.push_back(jet->GetNEM());
         fOutJetTreeVars.fMUM.push_back(jet->GetMUM());
       } else {
         fOutJetTreeVars.fCHF.push_back(jetGen->GetCHF());
         fOutJetTreeVars.fNHF.push_back(jetGen->GetNHF());
         fOutJetTreeVars.fCEF.push_back(jetGen->GetCEF());
         fOutJetTreeVars.fNEF.push_back(jetGen->GetNEF());
         fOutJetTreeVars.fMUF.push_back(jetGen->GetMUF());
       
         fOutJetTreeVars.fCHM.push_back(jetGen->GetCHM());
         fOutJetTreeVars.fNHM.push_back(jetGen->GetNHM());
         fOutJetTreeVars.fCEM.push_back(jetGen->GetCEM());
         fOutJetTreeVars.fNEM.push_back(jetGen->GetNEM());
         fOutJetTreeVars.fMUM.push_back(jetGen->GetMUM());
       }
     }
     
     if(fStoreSubjets) {
       if(fUseForestMatching) {
         fOutJetTreeVars.fSubJetPtRef.push_back(jet->GetRefSubJetPt());
         fOutJetTreeVars.fSubJetEtaRef.push_back(jet->GetRefSubJetEta());
         fOutJetTreeVars.fSubJetPhiRef.push_back(jet->GetRefSubJetPhi());
         fOutJetTreeVars.fSubJetMRef.push_back(jet->GetRefSubJetM());
       } else {
         int id = jet->GetMatchId1();
         lwJet *jetGen = fJetsGenCont->GetJet(id);
         fOutJetTreeVars.fSubJetPtRef.push_back(jetGen->GetSubJetPt());
         fOutJetTreeVars.fSubJetEtaRef.push_back(jetGen->GetSubJetEta());
         fOutJetTreeVars.fSubJetPhiRef.push_back(jetGen->GetSubJetPhi());
         fOutJetTreeVars.fSubJetMRef.push_back(jetGen->GetSubJetM());
       }
     }
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
  fTreeOut->Branch("fEtaRef",&fOutJetTreeVars.fEtaRef);
  fTreeOut->Branch("fPtMatch",&fOutJetTreeVars.fPtMatch);
  fTreeOut->Branch("fEtaMatch",&fOutJetTreeVars.fEtaMatch);
  if(fStoreSubjets) {
    fTreeOut->Branch("fSubJetPt",&fOutJetTreeVars.fSubJetPt);
    fTreeOut->Branch("fSubJetEta",&fOutJetTreeVars.fSubJetEta);
    fTreeOut->Branch("fSubJetPhi",&fOutJetTreeVars.fSubJetPhi);
    fTreeOut->Branch("fSubJetM",&fOutJetTreeVars.fSubJetM);

    fTreeOut->Branch("fSubJetPtRef",&fOutJetTreeVars.fSubJetPtRef);
    fTreeOut->Branch("fSubJetEtaRef",&fOutJetTreeVars.fSubJetEtaRef);
    fTreeOut->Branch("fSubJetPhiRef",&fOutJetTreeVars.fSubJetPhiRef);
    fTreeOut->Branch("fSubJetMRef",&fOutJetTreeVars.fSubJetMRef);
  }
  if(fStorePFIdInfo) {
    fTreeOut->Branch("fCHF",&fOutJetTreeVars.fCHF);
    fTreeOut->Branch("fNHF",&fOutJetTreeVars.fNHF);
    fTreeOut->Branch("fCEF",&fOutJetTreeVars.fCEF);
    fTreeOut->Branch("fNEF",&fOutJetTreeVars.fNEF);
    fTreeOut->Branch("fMUF",&fOutJetTreeVars.fMUF);
    fTreeOut->Branch("fCHM",&fOutJetTreeVars.fCHM);
    fTreeOut->Branch("fNHM",&fOutJetTreeVars.fNHM);
    fTreeOut->Branch("fCEM",&fOutJetTreeVars.fCEM);
    fTreeOut->Branch("fNEM",&fOutJetTreeVars.fNEM);
    fTreeOut->Branch("fMUM",&fOutJetTreeVars.fMUM);
  }
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
  fOutJetTreeVars.fEtaRef.clear();
  fOutJetTreeVars.fPtMatch.clear();
  fOutJetTreeVars.fEtaMatch.clear();
  fOutJetTreeVars.fSubJetPt.clear();
  fOutJetTreeVars.fSubJetEta.clear();
  fOutJetTreeVars.fSubJetPhi.clear();
  fOutJetTreeVars.fSubJetM.clear();
  fOutJetTreeVars.fSubJetPtRef.clear();
  fOutJetTreeVars.fSubJetEtaRef.clear();
  fOutJetTreeVars.fSubJetPhiRef.clear();
  fOutJetTreeVars.fSubJetMRef.clear();
  fOutJetTreeVars.fCHF.clear();
  fOutJetTreeVars.fNHF.clear();
  fOutJetTreeVars.fCEF.clear();
  fOutJetTreeVars.fNEF.clear();
  fOutJetTreeVars.fMUF.clear();
  fOutJetTreeVars.fCHM.clear();
  fOutJetTreeVars.fNHM.clear();
  fOutJetTreeVars.fCEM.clear();
  fOutJetTreeVars.fNEM.clear();
  fOutJetTreeVars.fMUM.clear();
}

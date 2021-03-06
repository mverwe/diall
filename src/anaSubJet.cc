#include "UserCode/diall/interface/anaSubJet.h"

#include "UserCode/diall/interface/lwJet.h"

#include <iostream>

ClassImp(anaSubJet)
   
anaSubJet::anaSubJet(const char *name, const char *title) 
:anaBaseTask(name,title),
  fNcentBins(4),
  fJetsName(""),
  fJetsCont(0x0),
  fJetsRefName(""),
  fJetsRefCont(0x0),
  fJets000Name(""),
  fJets000Cont(0x0),
  fJetEtaMin(-5.),
  fJetEtaMax(5.),
  fMinRefPt(-10000.),
  fDoDijets(false),
  fPtLeadingMin(),
  fPtLeadingMax(),
  fPtSubleadingMin(30.),
  fMinDPhi(2*3.14159/3.),
  fMinMassLeading(-1.),
  fhCentrality(0),
  fh3PtEtaPhi(),
  fh2PtNSubjets(),
  fh2PtGenNSubjets(),
  fh2PtMass(),
  fh2PtSubjetPtRatio21(),
  fh2PtSubjetPtRatio32(),
  fh2PtSubjetPtRatio43(),
  fh2PtSubjetPtRatio54(),
  fh2PtSubjetPtFrac1(),
  fh2PtSubjetPtFrac2(),
  fh2PtSubjetPtFrac3(),
  fh2PtSubjetPtFrac4(),
  fh2PtSubjetPtInvMass21(),
  fh2PtSubjetPtInvMass32(),
  fh2PtSubjetPtInvMass43(),
  fh2PtSubjetPtInvMass54(),
  fh2PtZg(),
  fh2PtZgTrue(),
  fh2PtZgNoRef(),
  fh3PtRecPtTrueZg(),
  fh3PtTrueDeltaPtDeltaZg(),
  fh3PtTruePtSJScalePtSJ(),
  fh3PtTruePtLSJScalePtLSJ(),
  fh3PtTruePtSLSJScalePtSLSJ(),
  fh3PtTruePtRecoSJScalePtSJ(),
  fh3PtTruePtRecoLSJScalePtLSJ(),
  fh3PtTruePtRecoSLSJScalePtSLSJ(),
  fh2PtThetag(),
  fh2PtThetagTrue(),
  fh2PtThetagNoRef(),
  fh3PtRecPtTrueThetag(),
  fhnZgResponse(),
  fh2SLPtSubjetPtRatio21(),
  fh2SLPtSubjetPtRatio32(),
  fh2SLPtSubjetPtRatio43(),
  fh2SLPtSubjetPtRatio54(),
  fh2SLPtSubjetPtInvMass21(),
  fh2SLPtSubjetPtInvMass32(),
  fh2SLPtSubjetPtInvMass43(),
  fh2SLPtSubjetPtInvMass54(),
  fh2SLPtZg(),
  fh2SLPtZgTrue(),
  fh2SLPtZgNoRef(),
  fh2SLPtThetag(),
  fh2SLPtThetagTrue(),
  fh2SLPtThetagNoRef(),
  fh2SLPtDeltaPhi(),
  fh2SLPtDeltaPhiTrue(),
  fh2SLPtDeltaPhiNoRef(),
  fh3SLPtZgDeltaPhi(),
  fStoreTree(false),
  fStoreTreeRef(true),
  fTreeOut(0x0),
  fMinPtJetTree(0.),
  fSubjetTreeVars()
{
  
  fh3PtEtaPhi          = new TH3F*[fNcentBins];
  fh2PtNSubjets        = new TH2F*[fNcentBins];
  fh2PtGenNSubjets     = new TH2F*[fNcentBins];
  fh2PtMass            = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio21 = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio32 = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio43 = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio54 = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac1   = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac2   = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac3   = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac4   = new TH2F*[fNcentBins];
  fh2PtZg              = new TH2F*[fNcentBins];
  fh2PtZgTrue          = new TH2F*[fNcentBins];
  fh2PtZgNoRef         = new TH2F*[fNcentBins];
  fh3PtRecPtTrueZg     = new TH3F*[fNcentBins];
  fh3PtTrueDeltaPtDeltaZg    = new TH3F*[fNcentBins];
  fh3PtTruePtSJScalePtSJ     = new TH3F*[fNcentBins];
  fh3PtTruePtLSJScalePtLSJ   = new TH3F*[fNcentBins];
  fh3PtTruePtSLSJScalePtSLSJ = new TH3F*[fNcentBins];
  fh3PtTruePtRecoSJScalePtSJ     = new TH3F*[fNcentBins];
  fh3PtTruePtRecoLSJScalePtLSJ   = new TH3F*[fNcentBins];
  fh3PtTruePtRecoSLSJScalePtSLSJ = new TH3F*[fNcentBins];
  fh2PtThetag          = new TH2F*[fNcentBins];
  fh2PtThetagTrue      = new TH2F*[fNcentBins];
  fh2PtThetagNoRef     = new TH2F*[fNcentBins];
  fh3PtRecPtTrueThetag = new TH3F*[fNcentBins];
  fh2PtSubjetPtInvMass21 = new TH2F*[fNcentBins];
  fh2PtSubjetPtInvMass32 = new TH2F*[fNcentBins];
  fh2PtSubjetPtInvMass43 = new TH2F*[fNcentBins];
  fh2PtSubjetPtInvMass54 = new TH2F*[fNcentBins];

  fhnZgResponse        = new THnSparse*[fNcentBins];

  for (int i = 0; i < fNcentBins; i++) {
    fh3PtEtaPhi[i]      = 0;
    fh2PtNSubjets[i]    = 0;
    fh2PtGenNSubjets[i] = 0;
    fh2PtMass[i]        = 0;
    fh2PtSubjetPtRatio21[i] = 0;
    fh2PtSubjetPtRatio32[i] = 0;
    fh2PtSubjetPtRatio43[i] = 0;
    fh2PtSubjetPtRatio54[i] = 0;
    fh2PtSubjetPtFrac1[i]   = 0;
    fh2PtSubjetPtFrac2[i]   = 0;
    fh2PtSubjetPtFrac3[i]   = 0;
    fh2PtSubjetPtFrac4[i]   = 0;
    fh2PtSubjetPtInvMass21[i]   = 0;
    fh2PtSubjetPtInvMass32[i]   = 0;
    fh2PtSubjetPtInvMass43[i]   = 0;
    fh2PtSubjetPtInvMass54[i]   = 0;
    fh2PtZg[i]                  = 0;
    fh2PtZgTrue[i]              = 0;
    fh2PtZgNoRef[i]             = 0;
    fh3PtRecPtTrueZg[i]         = 0;
    fh3PtTrueDeltaPtDeltaZg[i]  = 0;
    fh3PtTruePtSJScalePtSJ[i]    = 0;
    fh3PtTruePtLSJScalePtLSJ[i]  = 0;
    fh3PtTruePtSLSJScalePtSLSJ[i]= 0;
    fh3PtTruePtRecoSJScalePtSJ[i]    = 0;
    fh3PtTruePtRecoLSJScalePtLSJ[i]  = 0;
    fh3PtTruePtRecoSLSJScalePtSLSJ[i]= 0;
    fh2PtThetag[i]              = 0;
    fh2PtThetagTrue[i]          = 0;
    fh2PtThetagNoRef[i]         = 0;
    fh3PtRecPtTrueThetag[i]     = 0;
    fhnZgResponse[i]            = 0;
  }
}

//----------------------------------------------------------
void anaSubJet::Exec(Option_t * /*option*/)
{
  //printf("anaSubJet executing\n");
  anaBaseTask::Exec();
  if(!SelectEvent()) return;
  
  if(!fInitOutput) CreateOutputObjects();

  //Groomed jet. default zcut
  if(!fJetsCont && !fJetsName.IsNull())
    fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
  if(!fJetsCont) return;

  //Ungroomed jet
  if(!fJetsRefCont && !fJetsRefName.IsNull())
    fJetsRefCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsRefName.Data()));
  if(!fJetsRefName.IsNull() && !fJetsRefCont) {
    Printf("Cannot find fJetsRefCont %s. Returning",fJetsRefName.Data());
    return;
  }
  //Printf("%s: fJetsRefName: %s",GetName(),fJetsRefName.Data());
  
  //zcut=0
  if(!fJets000Cont && !fJets000Name.IsNull())
    fJets000Cont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJets000Name.Data()));
  //if(!fJets000Name.IsNull() && !fJets000Cont) return;

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

  ClearSubjetTreeVars();
  fSubjetTreeVars.fRun = -999;
  if(fHiEvent)
    fSubjetTreeVars.fRun = fHiEvent->GetRun();
  fSubjetTreeVars.fCent = cent;
  
  for (int i = 0; i < fJetsCont->GetNJets(); i++) {
    lwJet *jet = fJetsCont->GetJet(i);//jetSD000Name -> id1 jetName
    //Printf("jet %d pt %f",i,jet->Pt());
    lwJet *jetNotGroomed = 0x0;  //jetName -> id1 jetName -> id2 jetSDName
    if(fJetsRefCont) { //pick-up ungroomed jet from separate container which was previously matched
      int id = jet->GetMatchId1();
      if( id<0 || id>fJetsRefCont->GetNJets() ) continue;
      jetNotGroomed = fJetsRefCont->GetJet(id);
    } else { //assume main container is ungroomed (applies to gen-level)
      //Printf("%s: assume main container is ungroomed",GetName());
      jetNotGroomed = jet;
    }

    lwJet *jetZCut000 = 0x0; //jetSDName -> id2 jetName
    if(fJets000Cont && jetNotGroomed) { //pick-up zcut=0 jet from separate container which was previously matched
      int id = jetNotGroomed->GetMatchId2();
      if( id<0 || id>fJets000Cont->GetNJets() ) continue;
      jetZCut000 = fJets000Cont->GetJet(id);
      //Printf("%s: jet pt: %f  ptG: %f ptZp: %f",GetName(),jetNotGroomed->Pt(),jet->Pt(),jetZCut000->Pt());
    }
     
    Double_t pt  = jetNotGroomed->Pt();
    //Double_t ptg = jet->Pt();
    Double_t phi = jetNotGroomed->Phi();
    Double_t eta = jetNotGroomed->Eta();
    Double_t m = jetNotGroomed->M();
    if(fabs(pt-0.)<1e-6) continue; //remove ghosts
    if(pt<10.) continue;
    if(eta<fJetEtaMin || eta>fJetEtaMax) continue;
    if(fMinRefPt>-1. && jetNotGroomed->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    
 
    if(fCentBin>-1 && fCentBin<fNcentBins) {
      fh3PtEtaPhi[fCentBin]->Fill(pt,eta,phi,weight);
      fh2PtMass[fCentBin]->Fill(pt,m);
    }
       
    std::vector<float> sjpt  = jet->GetSubJetPt();
    std::vector<float> sjeta = jet->GetSubJetEta();
    std::vector<float> sjphi = jet->GetSubJetPhi();
    std::vector<float> sjm   = jet->GetSubJetM();
    int nsubjets = (int)sjpt.size();
    //Printf("%s: nsubjets: %d",GetName(),nsubjets);
    if(fCentBin>-1 && fCentBin<fNcentBins) {
      fh2PtNSubjets[fCentBin]->Fill(pt,nsubjets,weight);
      fh2PtGenNSubjets[fCentBin]->Fill(jetNotGroomed->GetRefPt(),nsubjets,weight);
    }

    float zg = 0.;
    double thetag = -1.;
    
    if(nsubjets>1) {
      float ptrat = -1.;
      for(int is = 1; is<nsubjets; ++is) {
        if(sjpt.at(is-1)>0.) {
          //Printf("jetpt: %f sjpt1: %f sjpt2: %f",pt,sjpt.at(is-1),sjpt.at(is));
          ptrat = sjpt.at(is)/sjpt.at(is-1);

          TLorentzVector v1;
          TLorentzVector v2;
          v1.SetPtEtaPhiM(sjpt.at(is-1),sjeta.at(is-1),sjphi.at(is-1),sjm.at(is-1));
          v2.SetPtEtaPhiM(sjpt.at(is),sjeta.at(is),sjphi.at(is),sjm.at(is));
          TLorentzVector disj = v1 + v2;
            
          if(fCentBin>-1 && fCentBin<fNcentBins) {
            if(is==1) fh2PtSubjetPtRatio21[fCentBin]->Fill(pt,ptrat);
            if(is==2) fh2PtSubjetPtRatio32[fCentBin]->Fill(pt,ptrat);
            if(is==3) fh2PtSubjetPtRatio43[fCentBin]->Fill(pt,ptrat);
            if(is==4) fh2PtSubjetPtRatio54[fCentBin]->Fill(pt,ptrat);

            if(is==1) fh2PtSubjetPtInvMass21[fCentBin]->Fill(pt,disj.M());
            if(is==2) fh2PtSubjetPtInvMass32[fCentBin]->Fill(pt,disj.M());
            if(is==3) fh2PtSubjetPtInvMass43[fCentBin]->Fill(pt,disj.M());
            if(is==4) fh2PtSubjetPtInvMass54[fCentBin]->Fill(pt,disj.M());
          }
        }
      }

      zg = std::min(sjpt.at(0),sjpt.at(1))/(sjpt.at(0)+sjpt.at(1));
      TLorentzVector v1;
      TLorentzVector v2;
      v1.SetPtEtaPhiM(sjpt.at(0),sjeta.at(0),sjphi.at(0),sjm.at(0));
      v2.SetPtEtaPhiM(sjpt.at(1),sjeta.at(1),sjphi.at(1),sjm.at(1));
      thetag = v1.Angle(v2.Vect());
      //Printf("ptjet: %f zg: %f thetag: %f",pt,zg,thetag);
    }
      
    if(fStoreTree && jetNotGroomed->Pt()>fMinPtJetTree) {
      //ungroomed jet
      fSubjetTreeVars.fPt.push_back(jetNotGroomed->Pt());
      fSubjetTreeVars.fPtRaw.push_back(jetNotGroomed->GetRawPt());
      fSubjetTreeVars.fEta.push_back(jetNotGroomed->Eta());
      fSubjetTreeVars.fPhi.push_back(jetNotGroomed->Phi());
      fSubjetTreeVars.fM.push_back(jetNotGroomed->M());
      
      fSubjetTreeVars.fCHF.push_back(jetNotGroomed->GetCHF());
      fSubjetTreeVars.fNHF.push_back(jetNotGroomed->GetNHF());
      fSubjetTreeVars.fCEF.push_back(jetNotGroomed->GetCEF());
      fSubjetTreeVars.fNEF.push_back(jetNotGroomed->GetNEF());
      fSubjetTreeVars.fMUF.push_back(jetNotGroomed->GetMUF());
      
      fSubjetTreeVars.fCHM.push_back(jetNotGroomed->GetCHM());
      fSubjetTreeVars.fNHM.push_back(jetNotGroomed->GetNHM());
      fSubjetTreeVars.fCEM.push_back(jetNotGroomed->GetCEM());
      fSubjetTreeVars.fNEM.push_back(jetNotGroomed->GetNEM());
      fSubjetTreeVars.fMUM.push_back(jetNotGroomed->GetMUM());
      
      //groomed jet
      //Printf("jet pt: %f groomed jet pt: %f",jetNotGroomed->Pt(),jet->Pt());
      fSubjetTreeVars.fPtG.push_back(jet->Pt());
      fSubjetTreeVars.fPtRawG.push_back(jet->GetRawPt());
      fSubjetTreeVars.fEtaG.push_back(jet->Eta());
      fSubjetTreeVars.fPhiG.push_back(jet->Phi());
      fSubjetTreeVars.fMG.push_back(jet->M());
      
      fSubjetTreeVars.fZg.push_back(zg);
      fSubjetTreeVars.fThetag.push_back(thetag);

      fSubjetTreeVars.fDropped.push_back(jet->GetNDroppedBranches());

      if(fStoreTreeRef) {
        //ref jet
        fSubjetTreeVars.fPtRef.push_back(jetNotGroomed->GetRefPt());
        fSubjetTreeVars.fEtaRef.push_back(jetNotGroomed->GetRefEta());
        fSubjetTreeVars.fPhiRef.push_back(jetNotGroomed->GetRefPhi());
        
        fSubjetTreeVars.fPtGRef.push_back(jet->GetRefPtG());
        fSubjetTreeVars.fEtaGRef.push_back(jet->GetRefEtaG());
        fSubjetTreeVars.fPhiGRef.push_back(jet->GetRefPhiG());
      }

      //zcut=0 jet
      if(jetZCut000) {
        fSubjetTreeVars.fZCut0PtG.push_back(jetZCut000->Pt());
        fSubjetTreeVars.fZCut0EtaG.push_back(jetZCut000->Eta());
        fSubjetTreeVars.fZCut0PhiG.push_back(jetZCut000->Phi());
        fSubjetTreeVars.fZCut0MG.push_back(jetZCut000->M());

        std::vector<float> sj000pt  = jetZCut000->GetSubJetPt();
        std::vector<float> sj000eta = jetZCut000->GetSubJetEta();
        std::vector<float> sj000phi = jetZCut000->GetSubJetPhi();
        std::vector<float> sj000m   = jetZCut000->GetSubJetM();
     
        if(sj000pt.size()>0) fSubjetTreeVars.fZCut0PtSJ1.push_back(sj000pt.at(0));
        else fSubjetTreeVars.fZCut0PtSJ1.push_back(-999.);
        if(sj000eta.size()>0) fSubjetTreeVars.fZCut0EtaSJ1.push_back(sj000eta.at(0));
        else fSubjetTreeVars.fZCut0EtaSJ1.push_back(-999.);
        if(sj000phi.size()>0) fSubjetTreeVars.fZCut0PhiSJ1.push_back(sj000phi.at(0));
        else fSubjetTreeVars.fZCut0PhiSJ1.push_back(-999.);
        if(sj000m.size()>0) fSubjetTreeVars.fZCut0MSJ1.push_back(sj000m.at(0));
        else fSubjetTreeVars.fZCut0MSJ1.push_back(-999.);

        if(sj000pt.size()>1) fSubjetTreeVars.fZCut0PtSJ2.push_back(sj000pt.at(1));
        else fSubjetTreeVars.fZCut0PtSJ2.push_back(-999.);
        if(sj000eta.size()>1) fSubjetTreeVars.fZCut0EtaSJ2.push_back(sj000eta.at(1));
        else fSubjetTreeVars.fZCut0EtaSJ2.push_back(-999.);
        if(sj000phi.size()>1) fSubjetTreeVars.fZCut0PhiSJ2.push_back(sj000phi.at(1));
        else fSubjetTreeVars.fZCut0PhiSJ2.push_back(-999.);
        if(sj000m.size()>1) fSubjetTreeVars.fZCut0MSJ2.push_back(sj000m.at(1));
        else fSubjetTreeVars.fZCut0MSJ2.push_back(-999.);

        if(sj000pt.size()>1) {
          fSubjetTreeVars.fZCut0Zg.push_back(std::min(sj000pt.at(0),sj000pt.at(1))/(sj000pt.at(0)+sj000pt.at(1)));
          TLorentzVector v1000;
          TLorentzVector v2000;
          v1000.SetPtEtaPhiM(sj000pt.at(0),sj000eta.at(0),sj000phi.at(0),sj000m.at(0));
          v2000.SetPtEtaPhiM(sj000pt.at(1),sj000eta.at(1),sj000phi.at(1),sj000m.at(1));
          double thetag000 = v1000.Angle(v2000.Vect());
          fSubjetTreeVars.fZCut0Thetag.push_back(thetag000);
        }
        if(fStoreTreeRef) {
          fSubjetTreeVars.fZCut0PtGRef.push_back(jetZCut000->GetRefPt());
          fSubjetTreeVars.fZCut0EtaGRef.push_back(jetZCut000->GetRefEta());
          fSubjetTreeVars.fZCut0PhiGRef.push_back(jetZCut000->GetRefPhi());
          fSubjetTreeVars.fZCut0MGRef.push_back(jetZCut000->GetRefM());

          std::vector<float> sjref000pt  = jetZCut000->GetRefSubJetPt();
          std::vector<float> sjref000eta = jetZCut000->GetRefSubJetEta();
          std::vector<float> sjref000phi = jetZCut000->GetRefSubJetPhi();
          std::vector<float> sjref000m   = jetZCut000->GetRefSubJetM();
             
          if(sjref000pt.size()>0) fSubjetTreeVars.fZCut0PtSJ1Ref.push_back(sjref000pt.at(0));
          else fSubjetTreeVars.fZCut0PtSJ1Ref.push_back(-999.);
          if(sjref000eta.size()>0) fSubjetTreeVars.fZCut0EtaSJ1Ref.push_back(sjref000eta.at(0));
          else fSubjetTreeVars.fZCut0EtaSJ1Ref.push_back(-999.);
          if(sjref000phi.size()>0) fSubjetTreeVars.fZCut0PhiSJ1Ref.push_back(sjref000phi.at(0));
          else fSubjetTreeVars.fZCut0PhiSJ1Ref.push_back(-999.);
          if(sjref000m.size()>0) fSubjetTreeVars.fZCut0MSJ1Ref.push_back(sjref000m.at(0));
          else fSubjetTreeVars.fZCut0MSJ1Ref.push_back(-999.);

          if(sjref000pt.size()>1) fSubjetTreeVars.fZCut0PtSJ2Ref.push_back(sjref000pt.at(1));
          else fSubjetTreeVars.fZCut0PtSJ2Ref.push_back(-999.);
          if(sjref000eta.size()>1) fSubjetTreeVars.fZCut0EtaSJ2Ref.push_back(sjref000eta.at(1));
          else fSubjetTreeVars.fZCut0EtaSJ2Ref.push_back(-999.);
          if(sjref000phi.size()>1) fSubjetTreeVars.fZCut0PhiSJ2Ref.push_back(sjref000phi.at(1));
          else fSubjetTreeVars.fZCut0PhiSJ2Ref.push_back(-999.);
          if(sjref000m.size()>1) fSubjetTreeVars.fZCut0MSJ2Ref.push_back(sjref000m.at(1));
          else fSubjetTreeVars.fZCut0MSJ2Ref.push_back(-999.);

          if(sjref000pt.size()>1) {
            fSubjetTreeVars.fZCut0ZgRef.push_back(std::min(sjref000pt.at(0),sjref000pt.at(1))/(sjref000pt.at(0)+sjref000pt.at(1)));
            TLorentzVector v1000ref;
            TLorentzVector v2000ref;
            v1000ref.SetPtEtaPhiM(sjref000pt.at(0),sjref000eta.at(0),sjref000phi.at(0),sjref000m.at(0));
            v2000ref.SetPtEtaPhiM(sjref000pt.at(1),sjref000eta.at(1),sjref000phi.at(1),sjref000m.at(1));
            double thetag000ref = v1000ref.Angle(v2000ref.Vect());
            fSubjetTreeVars.fZCut0ThetagRef.push_back(thetag000ref);
          }
          
        }//fStoreTreeRef
      }//jetZCut000
    }//fStoreTree

    //gen-level subjets
    std::vector<float> refsjpt   = jet->GetRefSubJetPt();
    std::vector<float> refsjeta  = jet->GetRefSubJetEta();
    std::vector<float> refsjphi  = jet->GetRefSubJetPhi();
    std::vector<float> refsjm    = jet->GetRefSubJetM();
    
    //just check if leading jet reco level is closest to leading jet gen level.
    bool swap = false;
    if(refsjpt.size()>1) {
      double dr11 = DeltaR(sjphi.at(0),refsjphi.at(0),sjeta.at(0),refsjeta.at(0));
      double dr12 = DeltaR(sjphi.at(0),refsjphi.at(1),sjeta.at(0),refsjeta.at(1));
      
      double dr21 = -1.;
      double dr22 = -1.;
      if(sjphi.size()>1) {
        dr21 = DeltaR(sjphi.at(1),refsjphi.at(0),sjeta.at(1),refsjeta.at(0));
        dr22 = DeltaR(sjphi.at(1),refsjphi.at(1),sjeta.at(1),refsjeta.at(1));
      }
      if(dr12<dr11 && dr21<dr22)
        swap = true;
    }
    //if(fStoreTreeRef)
    //fSubjetTreeVars.fSwap.push_back(swap);
      
    /*
    // ============== Begin complicated matching of subjets
    //if there is a ref jet order according to matching, otherwise pt-ordered
    std::vector<float> recsjpt;
    std::vector<float> recsjeta;
    std::vector<float> recsjphi;
    std::vector<int>   closestIndices;
    std::vector<float> closestDistances;
    for(uint refsj = 0; refsj<refsjpt.size(); ++refsj) {
    int closest = -1;
    double drmin = 999.;
    for(uint sj = 0; sj<sjpt.size(); ++sj) {
    double dr = DeltaR(sjphi.at(sj),refsjphi.at(refsj),sjeta.at(sj),refsjeta.at(refsj));
    if(dr<drmin) {
    drmin = dr;
    closest = (int)sj;
    }
    }
    //Printf("sj: %d closest: %d drmin: %f",sj,closest,drmin);
    closestIndices.push_back(closest);
    closestDistances.push_back(drmin);
    }

    //check uniqueness of matching. If double match, take closest one
    //Printf("check uniqueness");
    if(closestIndices.size()>1) {
    for(uint l = 0; l<closestIndices.size(); ++l) {
    int id = closestIndices.at(l);
    if(id<0) continue;
    for(uint m = l+1; m<closestIndices.size(); ++m) {
    if(id==closestIndices.at(m)) {
    //Printf("found double match %d %d %d %d %f %f",(int)l,(int)m,id,closestIndices.at(m),closestDistances.at(l),closestDistances.at(m));
    if(closestDistances.at(l)<closestDistances.at(m)) {
    closestIndices.at(m) = -1;
    closestDistances.at(m) = 999;
    }
    else {
    closestIndices.at(l) = -1;
    closestDistances.at(l) = 999;
    }
    }
    }
    }
    }
    //Printf("set subjet pt,eta,phi reco level");
    for(uint recsj = 0; recsj<closestIndices.size(); ++recsj) {
    int closest = closestIndices.at(recsj);
    if(closest>-1 && closest<(int)sjpt.size()) {
    recsjpt.push_back(sjpt.at(closest));
    recsjeta.push_back(sjeta.at(closest));
    recsjphi.push_back(sjphi.at(closest));
    } else {
    recsjpt.push_back(-999);
    recsjeta.push_back(-999);
    recsjphi.push_back(-999);
    }
    }

    if(fStoreTreeRef && jetNotGroomed->Pt()>fMinPtJetTree) {
    if(refsjpt.size()>0 && refsjpt.size()<2) { //only 1 subjet
    fSubjetTreeVars.fPtSJ1Ref.push_back(refsjpt.at(0));
    fSubjetTreeVars.fPtSJ1.push_back(recsjpt.at(0));
    fSubjetTreeVars.fPtSJ2Ref.push_back(-999);
    fSubjetTreeVars.fPtSJ2.push_back(-999);
    fSubjetTreeVars.fEtaSJ1Ref.push_back(refsjeta.at(0));
    fSubjetTreeVars.fEtaSJ1.push_back(recsjeta.at(0));
    fSubjetTreeVars.fEtaSJ2Ref.push_back(-999);
    fSubjetTreeVars.fEtaSJ2.push_back(-999);
    fSubjetTreeVars.fPhiSJ1Ref.push_back(refsjphi.at(0));
    fSubjetTreeVars.fPhiSJ1.push_back(recsjphi.at(0));
    fSubjetTreeVars.fPhiSJ2Ref.push_back(-999);
    fSubjetTreeVars.fPhiSJ2.push_back(-999);

    double dr = DeltaR(refsjphi.at(0),recsjphi.at(0),refsjeta.at(0),recsjeta.at(0));
    fSubjetTreeVars.fDeltaRSJ1.push_back(dr);
    fSubjetTreeVars.fDeltaRSJ1.push_back(999.);
    }
    else if(refsjpt.size()>1) { //2 subjets
    if(refsjpt.at(0)>(refsjpt.at(1))) {
    fSubjetTreeVars.fPtSJ1Ref.push_back(refsjpt.at(0));
    fSubjetTreeVars.fPtSJ2Ref.push_back(refsjpt.at(1));
    fSubjetTreeVars.fEtaSJ1Ref.push_back(refsjeta.at(0));
    fSubjetTreeVars.fEtaSJ2Ref.push_back(refsjeta.at(1));
    fSubjetTreeVars.fPhiSJ1Ref.push_back(refsjphi.at(0));
    fSubjetTreeVars.fPhiSJ2Ref.push_back(refsjphi.at(1));
              
    fSubjetTreeVars.fPtSJ1.push_back(recsjpt.at(0));
    fSubjetTreeVars.fPtSJ2.push_back(recsjpt.at(1));
    fSubjetTreeVars.fEtaSJ1.push_back(recsjeta.at(0));
    fSubjetTreeVars.fEtaSJ2.push_back(recsjeta.at(1));
    fSubjetTreeVars.fPhiSJ1.push_back(recsjphi.at(0));
    fSubjetTreeVars.fPhiSJ2.push_back(recsjphi.at(1));

    double drsj1 = DeltaR(refsjphi.at(0),recsjphi.at(0),refsjeta.at(0),recsjeta.at(0));
    double drsj2 = DeltaR(refsjphi.at(1),recsjphi.at(1),refsjeta.at(1),recsjeta.at(1));
    fSubjetTreeVars.fDeltaRSJ1.push_back(drsj1);
    fSubjetTreeVars.fDeltaRSJ2.push_back(drsj2);
    } else {
    fSubjetTreeVars.fPtSJ1Ref.push_back(refsjpt.at(1));
    fSubjetTreeVars.fPtSJ2Ref.push_back(refsjpt.at(0));
    fSubjetTreeVars.fEtaSJ1Ref.push_back(refsjeta.at(1));
    fSubjetTreeVars.fEtaSJ2Ref.push_back(refsjeta.at(0));
    fSubjetTreeVars.fPhiSJ1Ref.push_back(refsjphi.at(1));
    fSubjetTreeVars.fPhiSJ2Ref.push_back(refsjphi.at(0));
              
    fSubjetTreeVars.fPtSJ1.push_back(recsjpt.at(1));
    fSubjetTreeVars.fPtSJ2.push_back(recsjpt.at(0));
    fSubjetTreeVars.fEtaSJ1.push_back(recsjeta.at(1));
    fSubjetTreeVars.fEtaSJ2.push_back(recsjeta.at(0));
    fSubjetTreeVars.fPhiSJ1.push_back(recsjphi.at(1));
    fSubjetTreeVars.fPhiSJ2.push_back(recsjphi.at(0));

    double drsj1 = DeltaR(refsjphi.at(1),recsjphi.at(1),refsjeta.at(1),recsjeta.at(1));
    double drsj2 = DeltaR(refsjphi.at(0),recsjphi.at(0),refsjeta.at(0),recsjeta.at(0));
    fSubjetTreeVars.fDeltaRSJ1.push_back(drsj1);
    fSubjetTreeVars.fDeltaRSJ2.push_back(drsj2);
    }
    } else {
    fSubjetTreeVars.fPtSJ1Ref.push_back(-999);
    fSubjetTreeVars.fPtSJ2Ref.push_back(-999);
    fSubjetTreeVars.fEtaSJ1Ref.push_back(-999);
    fSubjetTreeVars.fEtaSJ2Ref.push_back(-999);
    fSubjetTreeVars.fPhiSJ1Ref.push_back(-999);
    fSubjetTreeVars.fPhiSJ2Ref.push_back(-999);

    fSubjetTreeVars.fPtSJ1.push_back(-999);
    fSubjetTreeVars.fPtSJ2.push_back(-999);
    fSubjetTreeVars.fEtaSJ1.push_back(-999);
    fSubjetTreeVars.fEtaSJ2.push_back(-999);
    fSubjetTreeVars.fPhiSJ1.push_back(-999);
    fSubjetTreeVars.fPhiSJ2.push_back(-999);

    fSubjetTreeVars.fDeltaRSJ1.push_back(999);
    fSubjetTreeVars.fDeltaRSJ2.push_back(999);
    }
    }//fStoreRefTree
    // ============== End complicated matching of subjets
    */

    //      if(!fStoreTreeRef && jetNotGroomed->Pt()>fMinPtJetTree) {
    if(jetNotGroomed->Pt()>fMinPtJetTree) {
      //if(sjpt.at(0)>sjpt.at(1)) {
      if(sjpt.size()>0) fSubjetTreeVars.fPtSJ1.push_back(sjpt.at(0));
      else fSubjetTreeVars.fPtSJ1.push_back(-999.);
      
      if(sjpt.size()>1) fSubjetTreeVars.fPtSJ2.push_back(sjpt.at(1));
      else fSubjetTreeVars.fPtSJ2.push_back(-999.);

      if(sjeta.size()>0) fSubjetTreeVars.fEtaSJ1.push_back(sjeta.at(0));
      else fSubjetTreeVars.fEtaSJ1.push_back(-999.);

      if(sjeta.size()>1) fSubjetTreeVars.fEtaSJ2.push_back(sjeta.at(1));
      else fSubjetTreeVars.fEtaSJ2.push_back(-999.);

      if(sjphi.size()>0) fSubjetTreeVars.fPhiSJ1.push_back(sjphi.at(0));
      else fSubjetTreeVars.fPhiSJ1.push_back(-999.);

      if(sjphi.size()>1) fSubjetTreeVars.fPhiSJ2.push_back(sjphi.at(1));
      else fSubjetTreeVars.fPhiSJ2.push_back(-999.);

      if(sjm.size()>0) fSubjetTreeVars.fMSJ1.push_back(sjm.at(0));
      else fSubjetTreeVars.fMSJ1.push_back(-999.);

      if(sjm.size()>1) fSubjetTreeVars.fMSJ2.push_back(sjm.at(1));
      else fSubjetTreeVars.fMSJ2.push_back(-999.);
      // } else {
      //   fSubjetTreeVars.fPtSJ1.push_back(sjpt.at(1));
      //   fSubjetTreeVars.fPtSJ2.push_back(sjpt.at(0));
      //   fSubjetTreeVars.fEtaSJ1.push_back(sjeta.at(1));
      //   fSubjetTreeVars.fEtaSJ2.push_back(sjeta.at(0));
      //   fSubjetTreeVars.fPhiSJ1.push_back(sjphi.at(1));
      //   fSubjetTreeVars.fPhiSJ2.push_back(sjphi.at(0));
      // }
      fSubjetTreeVars.fSwap.push_back(swap);
      if(fStoreTreeRef) {
        // if(refsjpt.at(0)>refsjpt.at(1)) {
        if(refsjpt.size()>0) {
            fSubjetTreeVars.fPtSJ1Ref.push_back(refsjpt.at(0));
            fSubjetTreeVars.fEtaSJ1Ref.push_back(refsjeta.at(0));
            fSubjetTreeVars.fPhiSJ1Ref.push_back(refsjphi.at(0));
            fSubjetTreeVars.fMSJ1Ref.push_back(refsjm.at(0));
            double dr11 = DeltaR(refsjphi.at(0),sjphi.at(0),refsjeta.at(0),sjeta.at(0));
            double dr12 = -1.;
            if(sjpt.size()>1) dr12 = DeltaR(refsjphi.at(0),sjphi.at(1),refsjeta.at(0),sjeta.at(1));
            fSubjetTreeVars.fDeltaRSJ11.push_back(dr11);
            fSubjetTreeVars.fDeltaRSJ12.push_back(dr12);
          } else {
            fSubjetTreeVars.fPtSJ1Ref.push_back(-999.);
            fSubjetTreeVars.fEtaSJ1Ref.push_back(-999.);
            fSubjetTreeVars.fPhiSJ1Ref.push_back(-999.);
            fSubjetTreeVars.fMSJ1Ref.push_back(-999.);
            fSubjetTreeVars.fDeltaRSJ11.push_back(-999.);
            fSubjetTreeVars.fDeltaRSJ12.push_back(-999.);
          }
        if(refsjpt.size()>1 && sjpt.size()>1)  {
            
            fSubjetTreeVars.fPtSJ2Ref.push_back(refsjpt.at(1));
            fSubjetTreeVars.fEtaSJ2Ref.push_back(refsjeta.at(1));
            fSubjetTreeVars.fPhiSJ2Ref.push_back(refsjphi.at(1));
            fSubjetTreeVars.fMSJ2Ref.push_back(refsjm.at(1));
            double dr22 = -1.;
            if(sjpt.size()>1) dr22 = DeltaR(refsjphi.at(1),sjphi.at(1),refsjeta.at(1),sjeta.at(1));
            double dr21 = DeltaR(refsjphi.at(1),sjphi.at(0),refsjeta.at(1),sjeta.at(0));
            fSubjetTreeVars.fDeltaRSJ22.push_back(dr22);
            fSubjetTreeVars.fDeltaRSJ21.push_back(dr21);
          } else {
            fSubjetTreeVars.fPtSJ2Ref.push_back(-999.);
            fSubjetTreeVars.fEtaSJ2Ref.push_back(-999.);
            fSubjetTreeVars.fPhiSJ2Ref.push_back(-999.);
            fSubjetTreeVars.fMSJ2Ref.push_back(-999.);
            fSubjetTreeVars.fDeltaRSJ22.push_back(-999.);
            fSubjetTreeVars.fDeltaRSJ21.push_back(-999.);
          }
          // } else {
          //   fSubjetTreeVars.fPtSJ1Ref.push_back(refsjpt.at(1));
          //   fSubjetTreeVars.fPtSJ2Ref.push_back(refsjpt.at(0));
          //   fSubjetTreeVars.fEtaSJ1Ref.push_back(refsjeta.at(1));
          //   fSubjetTreeVars.fEtaSJ2Ref.push_back(refsjeta.at(0));
          //   fSubjetTreeVars.fPhiSJ1Ref.push_back(refsjphi.at(1));
          //   fSubjetTreeVars.fPhiSJ2Ref.push_back(refsjphi.at(0));
          // }
        }
      }
      
      float refzg = -1.;
      float refthetag = -1.;
      if(refsjpt.size()>1) {
        refzg = std::min(refsjpt.at(0),refsjpt.at(1))/(refsjpt.at(0)+refsjpt.at(1));
        TLorentzVector refv1;
        TLorentzVector refv2;
        refv1.SetPtEtaPhiM(refsjpt.at(0),refsjeta.at(0),refsjphi.at(0),refsjm.at(0));
        refv2.SetPtEtaPhiM(refsjpt.at(1),refsjeta.at(1),refsjphi.at(1),refsjm.at(1));
        refthetag = refv1.Angle(refv2.Vect());
      }
      if(fStoreTreeRef && jetNotGroomed->Pt()>fMinPtJetTree) {
        fSubjetTreeVars.fZgRef.push_back(refzg);
        fSubjetTreeVars.fThetagRef.push_back(refthetag);
        fSubjetTreeVars.fDroppedRef.push_back(jet->GetRefNDroppedBranches());
      }
      if(fCentBin>-1 && fCentBin<fNcentBins) {
        fh2PtZg[fCentBin]->Fill(pt,zg);
        fh2PtThetag[fCentBin]->Fill(pt,thetag);
        if(jetNotGroomed->GetRefPt()<10.) {
          fh2PtZgNoRef[fCentBin]->Fill(pt,zg);
          fh2PtThetagNoRef[fCentBin]->Fill(pt,thetag);
        }
        else {
          fh2PtZgTrue[fCentBin]->Fill(pt,zg);
          fh2PtThetagTrue[fCentBin]->Fill(pt,thetag);
          fh3PtRecPtTrueZg[fCentBin]->Fill(pt,jetNotGroomed->GetRefPt(),zg);
          fh3PtRecPtTrueThetag[fCentBin]->Fill(pt,jetNotGroomed->GetRefPt(),thetag);
          std::vector<float> refsjpt   = jet->GetRefSubJetPt();
          // std::vector<float> refsjeta  = jet->GetRefSubJetEta();
          // std::vector<float> refsjphi  = jet->GetRefSubJetPhi();
          // std::vector<float> refsjm    = jet->GetRefSubJetM();
          float refzg = -1.;
          //float refthetag = -1.;
          double dzgrel = -999.;
          if(refsjpt.size()>1) {
            refzg = std::min(refsjpt.at(0),refsjpt.at(1))/(refsjpt.at(0)+refsjpt.at(1));
            dzgrel = (zg - refzg)/refzg;

            // TLorentzVector refv1;
            // TLorentzVector refv2;
            // refv1.SetPtEtaPhiM(refsjpt.at(0),refsjeta.at(0),refsjphi.at(0),refsjm.at(0));
            // refv2.SetPtEtaPhiM(refsjpt.at(1),refsjeta.at(1),refsjphi.at(1),refsjm.at(1));
            //refthetag = refv1.Angle(refv2.Vect());
          }
          double dptrel = (pt - jetNotGroomed->GetRefPt())/jetNotGroomed->GetRefPt();
          double var[6] = {(double)zg,(double)refzg,(double)pt,(double)jetNotGroomed->GetRefPt(),dptrel,dzgrel};
          fhnZgResponse[fCentBin]->Fill(var);
          fh3PtTrueDeltaPtDeltaZg[fCentBin]->Fill(jetNotGroomed->GetRefPt(),pt-jetNotGroomed->GetRefPt(),zg-refzg);
          
          for(uint sj = 0; sj<refsjpt.size(); ++sj) {
            if(sj>1) continue;
            if(sj>=sjpt.size()) continue;
            uint sjmatch = sj;
            if(swap && sj==0) sjmatch = 1;
            if(swap && sj==1) sjmatch = 0;
            double dptrel = (sjpt.at(sjmatch)-refsjpt.at(sj))/refsjpt.at(sj);
            fh3PtTruePtSJScalePtSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),refsjpt.at(sj),dptrel);
            fh3PtTruePtRecoSJScalePtSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),sjpt.at(sjmatch),dptrel+1.);
            if(sj==0) {
              fh3PtTruePtLSJScalePtLSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),refsjpt.at(sj),dptrel);
              fh3PtTruePtRecoLSJScalePtLSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),sjpt.at(sjmatch),dptrel+1.);
            }
            else if(sj==1) {
              fh3PtTruePtSLSJScalePtSLSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),refsjpt.at(sj),dptrel);
              fh3PtTruePtRecoSLSJScalePtSLSJ[fCentBin]->Fill(jetNotGroomed->GetRefPt(),sjpt.at(sjmatch),dptrel+1.);
            }
          }
        }//true jet matched
      }//centbins
      for(int is = 0; is<nsubjets; ++is) {
        float ptrat = sjpt.at(is)/pt;
        if(fCentBin>-1 && fCentBin<fNcentBins) {
          if(is==0) fh2PtSubjetPtFrac1[fCentBin]->Fill(pt,ptrat);
          if(is==1) fh2PtSubjetPtFrac2[fCentBin]->Fill(pt,ptrat);
          if(is==2) fh2PtSubjetPtFrac3[fCentBin]->Fill(pt,ptrat);
          if(is==3) fh2PtSubjetPtFrac4[fCentBin]->Fill(pt,ptrat);
        }
      }
      //}//nsubjets>1
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

          TH2F *hdphi = (fh2SLPtDeltaPhi.at(fCentBin)).at(l);
          hdphi->Fill(jet2->Pt(),dphi);
          if(jet2->GetRefPt()<20.) {
            TH2F *hdphiNoRef = (fh2SLPtDeltaPhiNoRef.at(fCentBin)).at(l);
            hdphiNoRef->Fill(jet2->Pt(),dphi);
          } else {
            TH2F *hdphiTrue = (fh2SLPtDeltaPhiTrue.at(fCentBin)).at(l);
            hdphiTrue->Fill(jet2->Pt(),dphi);
          }
           
          //if(dphi<fMinDPhi) continue;
           
          std::vector<float> sjptsl  = jet2->GetSubJetPt();
          std::vector<float> sjetasl = jet2->GetSubJetEta();
          std::vector<float> sjphisl = jet2->GetSubJetPhi();
          std::vector<float> sjmsl   = jet2->GetSubJetM();
          int nsubjets2 = (int)sjptsl.size();
          if(nsubjets2<2) continue;
           
          for(int is = 1; is<nsubjets2; ++is) {
            if(sjptsl.at(is-1)>0.) {
              float ptrat = sjptsl.at(is)/sjptsl.at(is-1);

              TLorentzVector v1;
              TLorentzVector v2;
              v1.SetPtEtaPhiM(sjptsl.at(is-1),sjetasl.at(is-1),sjphisl.at(is-1),sjmsl.at(is-1));
              v2.SetPtEtaPhiM(sjptsl.at(is),sjetasl.at(is),sjphisl.at(is),sjmsl.at(is));
              TLorentzVector disj = v1 + v2;
               
              if(is==1) {
                if(dphi>fMinDPhi) {
                  TH2F *h21 = (fh2SLPtSubjetPtRatio21.at(fCentBin)).at(l);
                  h21->Fill(jet2->Pt(),ptrat);
                  TH2F *h21m = (fh2SLPtSubjetPtInvMass21.at(fCentBin)).at(l);
                  h21m->Fill(jet2->Pt(),disj.M());
                }
              }
              if(is==2) {
                if(dphi>fMinDPhi) {
                  TH2F *h32 = (fh2SLPtSubjetPtRatio32.at(fCentBin)).at(l);
                  h32->Fill(jet2->Pt(),ptrat);
                  TH2F *h32m = (fh2SLPtSubjetPtInvMass32.at(fCentBin)).at(l);
                  h32m->Fill(jet2->Pt(),disj.M());
                }
              }
              if(is==3) {
                if(dphi>fMinDPhi) {
                  TH2F *h43 = (fh2SLPtSubjetPtRatio43.at(fCentBin)).at(l);
                  h43->Fill(jet2->Pt(),ptrat);
                  TH2F *h43m = (fh2SLPtSubjetPtInvMass43.at(fCentBin)).at(l);
                  h43m->Fill(jet2->Pt(),disj.M());
                }
              }
              if(is==4) {
                if(dphi>fMinDPhi) {
                  TH2F *h54 = (fh2SLPtSubjetPtRatio54.at(fCentBin)).at(l);
                  h54->Fill(jet2->Pt(),ptrat);
                  TH2F *h54m = (fh2SLPtSubjetPtInvMass54.at(fCentBin)).at(l);
                  h54m->Fill(jet2->Pt(),disj.M());
                }
              }
            }      
          }
          float zgsl = std::min(sjptsl.at(0),sjptsl.at(1))/(sjptsl.at(0)+sjptsl.at(1));
          TH3F *hsjzgdphi = (fh3SLPtZgDeltaPhi.at(fCentBin)).at(l);
          hsjzgdphi->Fill(jet2->Pt(),zgsl,dphi);
          if(dphi>fMinDPhi) {
            TH2F *hsjzg = (fh2SLPtZg.at(fCentBin)).at(l);
            hsjzg->Fill(jet2->Pt(),zgsl);
            if(jet2->GetRefPt()<20.) {
              TH2F *hsjzgNoRef = (fh2SLPtZgNoRef.at(fCentBin)).at(l);
              hsjzgNoRef->Fill(jet2->Pt(),zgsl);
            } else {
              TH2F *hsjzgTrue = (fh2SLPtZgTrue.at(fCentBin)).at(l);
              hsjzgTrue->Fill(jet2->Pt(),zgsl);
            }
          }
        }
      }
    }//fDoDijet
  }
  
  if(fStoreTree) {
    fTreeOut->Fill();
    ClearSubjetTreeVars();
  }
}

//----------------------------------------------------------
void anaSubJet::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaSubJet: fOutput not present");
    return;
  }

  if(fStoreTree) {
    fTreeOut = new TTree(Form("%sTree",GetName()),"subjet tree");
    fTreeOut->Branch("fRun",&fSubjetTreeVars.fRun,"fRun/I");
    fTreeOut->Branch("fCent",&fSubjetTreeVars.fCent,"fCent/F");
    fTreeOut->Branch("fPt",&fSubjetTreeVars.fPt);
    fTreeOut->Branch("fPtRaw",&fSubjetTreeVars.fPtRaw);
    fTreeOut->Branch("fEta",&fSubjetTreeVars.fEta);
    fTreeOut->Branch("fPhi",&fSubjetTreeVars.fPhi);
    fTreeOut->Branch("fM",&fSubjetTreeVars.fM);
    fTreeOut->Branch("fPtG",&fSubjetTreeVars.fPtG);
    fTreeOut->Branch("fPtRawG",&fSubjetTreeVars.fPtRawG);
    fTreeOut->Branch("fEtaG",&fSubjetTreeVars.fEtaG);
    fTreeOut->Branch("fPhiG",&fSubjetTreeVars.fPhiG);
    fTreeOut->Branch("fMG",&fSubjetTreeVars.fMG);
    fTreeOut->Branch("fPtSJ1",&fSubjetTreeVars.fPtSJ1);
    fTreeOut->Branch("fPtSJ2",&fSubjetTreeVars.fPtSJ2);
    fTreeOut->Branch("fEtaSJ1",&fSubjetTreeVars.fEtaSJ1);
    fTreeOut->Branch("fEtaSJ2",&fSubjetTreeVars.fEtaSJ2);
    fTreeOut->Branch("fPhiSJ1",&fSubjetTreeVars.fPhiSJ1);
    fTreeOut->Branch("fPhiSJ2",&fSubjetTreeVars.fPhiSJ2);
    fTreeOut->Branch("fMSJ1",&fSubjetTreeVars.fMSJ1);
    fTreeOut->Branch("fMSJ2",&fSubjetTreeVars.fMSJ2);
    fTreeOut->Branch("fZg",&fSubjetTreeVars.fZg);
    fTreeOut->Branch("fThetag",&fSubjetTreeVars.fThetag);
    fTreeOut->Branch("fDropped",&fSubjetTreeVars.fDropped);
    fTreeOut->Branch("fCHF",&fSubjetTreeVars.fCHF);
    fTreeOut->Branch("fNHF",&fSubjetTreeVars.fNHF);
    fTreeOut->Branch("fCEF",&fSubjetTreeVars.fCEF);
    fTreeOut->Branch("fNEF",&fSubjetTreeVars.fNEF);
    fTreeOut->Branch("fMUF",&fSubjetTreeVars.fMUF);
    fTreeOut->Branch("fCHM",&fSubjetTreeVars.fCHM);
    fTreeOut->Branch("fNHM",&fSubjetTreeVars.fNHM);
    fTreeOut->Branch("fCEM",&fSubjetTreeVars.fCEM);
    fTreeOut->Branch("fNEM",&fSubjetTreeVars.fNEM);
    fTreeOut->Branch("fMUM",&fSubjetTreeVars.fMUM);
    fTreeOut->Branch("fPtRef",&fSubjetTreeVars.fPtRef);
    fTreeOut->Branch("fEtaRef",&fSubjetTreeVars.fEtaRef);
    fTreeOut->Branch("fPhiRef",&fSubjetTreeVars.fPhiRef);
    fTreeOut->Branch("fPtGRef",&fSubjetTreeVars.fPtGRef);
    fTreeOut->Branch("fEtaGRef",&fSubjetTreeVars.fEtaGRef);
    fTreeOut->Branch("fPhiGRef",&fSubjetTreeVars.fPhiGRef);
    fTreeOut->Branch("fPtSJ1Ref",&fSubjetTreeVars.fPtSJ1Ref);
    fTreeOut->Branch("fPtSJ2Ref",&fSubjetTreeVars.fPtSJ2Ref);
    fTreeOut->Branch("fEtaSJ1Ref",&fSubjetTreeVars.fEtaSJ1Ref);
    fTreeOut->Branch("fEtaSJ2Ref",&fSubjetTreeVars.fEtaSJ2Ref);
    fTreeOut->Branch("fPhiSJ1Ref",&fSubjetTreeVars.fPhiSJ1Ref);
    fTreeOut->Branch("fPhiSJ2Ref",&fSubjetTreeVars.fPhiSJ2Ref);
    fTreeOut->Branch("fMSJ1Ref",&fSubjetTreeVars.fMSJ1Ref);
    fTreeOut->Branch("fMSJ2Ref",&fSubjetTreeVars.fMSJ2Ref);
    fTreeOut->Branch("fZgRef",&fSubjetTreeVars.fZgRef);
    fTreeOut->Branch("fDroppedRef",&fSubjetTreeVars.fDroppedRef);
    fTreeOut->Branch("fThetagRef",&fSubjetTreeVars.fThetagRef);
    fTreeOut->Branch("fSwap",&fSubjetTreeVars.fSwap);
    fTreeOut->Branch("fDeltaRSJ11",&fSubjetTreeVars.fDeltaRSJ11);
    fTreeOut->Branch("fDeltaRSJ12",&fSubjetTreeVars.fDeltaRSJ12);
    fTreeOut->Branch("fDeltaRSJ21",&fSubjetTreeVars.fDeltaRSJ21);
    fTreeOut->Branch("fDeltaRSJ22",&fSubjetTreeVars.fDeltaRSJ22);

    fTreeOut->Branch("fZCut0PtG",&fSubjetTreeVars.fZCut0PtG);
    fTreeOut->Branch("fZCut0EtaG",&fSubjetTreeVars.fZCut0EtaG);
    fTreeOut->Branch("fZCut0PhiG",&fSubjetTreeVars.fZCut0PhiG);
    fTreeOut->Branch("fZCut0MG",&fSubjetTreeVars.fZCut0MG);
    fTreeOut->Branch("fZCut0PtSJ1",&fSubjetTreeVars.fZCut0PtSJ1);
    fTreeOut->Branch("fZCut0PtSJ2",&fSubjetTreeVars.fZCut0PtSJ2);
    fTreeOut->Branch("fZCut0EtaSJ1",&fSubjetTreeVars.fZCut0EtaSJ1);
    fTreeOut->Branch("fZCut0EtaSJ2",&fSubjetTreeVars.fZCut0EtaSJ2);
    fTreeOut->Branch("fZCut0PhiSJ1",&fSubjetTreeVars.fZCut0PhiSJ1);
    fTreeOut->Branch("fZCut0PhiSJ2",&fSubjetTreeVars.fZCut0PhiSJ2);
    fTreeOut->Branch("fZCut0MSJ1",&fSubjetTreeVars.fZCut0MSJ1);
    fTreeOut->Branch("fZCut0MSJ2",&fSubjetTreeVars.fZCut0MSJ2);
    fTreeOut->Branch("fZCut0Zg",&fSubjetTreeVars.fZCut0Zg);
    fTreeOut->Branch("fZCut0Thetag",&fSubjetTreeVars.fZCut0Thetag);

    fTreeOut->Branch("fZCut0PtGRef",&fSubjetTreeVars.fZCut0PtGRef);
    fTreeOut->Branch("fZCut0EtaGRef",&fSubjetTreeVars.fZCut0EtaGRef);
    fTreeOut->Branch("fZCut0PhiGRef",&fSubjetTreeVars.fZCut0PhiGRef);
    fTreeOut->Branch("fZCut0MGRef",&fSubjetTreeVars.fZCut0MGRef);
    fTreeOut->Branch("fZCut0PtSJ1Ref",&fSubjetTreeVars.fZCut0PtSJ1Ref);
    fTreeOut->Branch("fZCut0PtSJ2Ref",&fSubjetTreeVars.fZCut0PtSJ2Ref);
    fTreeOut->Branch("fZCut0EtaSJ1Ref",&fSubjetTreeVars.fZCut0EtaSJ1Ref);
    fTreeOut->Branch("fZCut0EtaSJ2Ref",&fSubjetTreeVars.fZCut0EtaSJ2Ref);
    fTreeOut->Branch("fZCut0PhiSJ1Ref",&fSubjetTreeVars.fZCut0PhiSJ1Ref);
    fTreeOut->Branch("fZCut0PhiSJ2Ref",&fSubjetTreeVars.fZCut0PhiSJ2Ref);
    fTreeOut->Branch("fZCut0MSJ1Ref",&fSubjetTreeVars.fZCut0MSJ1Ref);
    fTreeOut->Branch("fZCut0MSJ2Ref",&fSubjetTreeVars.fZCut0MSJ2Ref);
    fTreeOut->Branch("fZCut0ZgRef",&fSubjetTreeVars.fZCut0ZgRef);
    fTreeOut->Branch("fZCut0ThetagRef",&fSubjetTreeVars.fZCut0ThetagRef);

    fOutput->Add(fTreeOut);
  }

  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  TString histTitle = "";
  TString histName  = "";

  //Binning for THnSparse
  const int nBinsPt  = 50;
  const double minPt = 0.;
  const double maxPt = 500.;

  const int nBinsZg = 50;
  const double minZg = 0.;
  const double maxZg = 0.5;

  const int nBinsDPtRel  =  60;
  const double minDPtRel = -3.;
  const double maxDPtRel =  3.;

  const int nBinsDZgRel  =  50;
  const double minDZgRel = -1.;
  const double maxDZgRel =  4.;
  
  const Int_t nBinsSparse0 = 6;
  const Int_t nBins0[nBinsSparse0] = {nBinsZg,nBinsZg,nBinsPt,nBinsPt,nBinsDPtRel,nBinsDZgRel};
  const Double_t xmin0[nBinsSparse0]  = { minZg, minZg, minPt, minPt, minDPtRel, minDZgRel};
  const Double_t xmax0[nBinsSparse0]  = { maxZg, maxZg, maxPt, maxPt, maxDPtRel, maxDZgRel};
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = Form("fh2PtEtaPhi_%d",i);
    histTitle = Form("%s;pt;#eta;#phi;",histName.Data());
    fh3PtEtaPhi[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhi[i]);

    histName = Form("fh2PtNSubjets_%d",i);
    histTitle = Form("%s;pt;N_{subjets};",histName.Data());
    fh2PtNSubjets[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,10,0,10);
    fOutput->Add(fh2PtNSubjets[i]);

    histName = Form("fh2PtGenNSubjets_%d",i);
    histTitle = Form("%s;pt;N_{subjets};",histName.Data());
    fh2PtGenNSubjets[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,10,0,10);
    fOutput->Add(fh2PtGenNSubjets[i]);

    histName = Form("fh2PtMass_%d",i);
    histTitle = Form("%s;p_{T};M;",histName.Data());
    fh2PtMass[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,50);
    fOutput->Add(fh2PtMass[i]);
    
    histName = Form("fh2PtSubjetPtRatio21_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
    fh2PtSubjetPtRatio21[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio21[i]);
    
    histName = Form("fh2PtSubjetPtRatio32_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ3}/p_{T,SJ2};",histName.Data());
    fh2PtSubjetPtRatio32[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio32[i]);

    histName = Form("fh2PtSubjetPtRatio43_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ4}/p_{T,SJ3};",histName.Data());
    fh2PtSubjetPtRatio43[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio43[i]);

    histName = Form("fh2PtSubjetPtRatio54_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ5}/p_{T,SJ4};",histName.Data());
    fh2PtSubjetPtRatio54[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio54[i]);

    histName = Form("fh2PtSubjetPtFrac1_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ1}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac1[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac1[i]);

    histName = Form("fh2PtSubjetPtFrac2_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac2[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac2[i]);

    histName = Form("fh2PtSubjetPtFrac3_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ3}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac3[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac3[i]);

    histName = Form("fh2PtSubjetPtFrac4_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ4}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac4[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac4[i]);

    histName = Form("fh2PtSubjetPtInvMass21_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
    fh2PtSubjetPtInvMass21[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
    fOutput->Add(fh2PtSubjetPtInvMass21[i]);
    
    histName = Form("fh2PtSubjetPtInvMass32_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ3}/p_{T,SJ2};",histName.Data());
    fh2PtSubjetPtInvMass32[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
    fOutput->Add(fh2PtSubjetPtInvMass32[i]);

    histName = Form("fh2PtSubjetPtInvMass43_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ4}/p_{T,SJ3};",histName.Data());
    fh2PtSubjetPtInvMass43[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
    fOutput->Add(fh2PtSubjetPtInvMass43[i]);

    histName = Form("fh2PtSubjetPtInvMass54_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ5}/p_{T,SJ4};",histName.Data());
    fh2PtSubjetPtInvMass54[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
    fOutput->Add(fh2PtSubjetPtInvMass54[i]);
    
    histName = Form("fh2PtZg_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZg[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZg[i]);

    histName = Form("fh2PtZgTrue_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgTrue[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgTrue[i]);

    histName = Form("fh2PtZgNoRef_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgNoRef[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgNoRef[i]);

    histName = Form("fh3PtRecPtTrueZg_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh3PtRecPtTrueZg[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,500,50,0,0.5);
    fOutput->Add(fh3PtRecPtTrueZg[i]);
    
    histName = Form("fh2PtThetag_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtThetag[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetag[i]);

    histName = Form("fh2PtThetagTrue_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtThetagTrue[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetagTrue[i]);

    histName = Form("fh2PtThetagNoRef_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtThetagNoRef[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetagNoRef[i]);

    histName = Form("fh3PtRecPtTrueThetag_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh3PtRecPtTrueThetag[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,500,50,0,0.5);
    fOutput->Add(fh3PtRecPtTrueThetag[i]);

    histName = Form("fhnZgResponse_%d",i);
    histTitle = Form("%s;#it{z}_{g,det};#it{z}_{g,part};#it{p}_{T,det};#it{p}_{T,part}",histName.Data());
    fhnZgResponse[i] = new THnSparseF(histName.Data(),histTitle.Data(),nBinsSparse0,nBins0,xmin0,xmax0);
    fOutput->Add(fhnZgResponse[i]); 

    histName = Form("fh3PtTrueDeltaPtDeltaZg_%d",i);
    histTitle = Form("%s;p_{T,true};#Delta p_{T};#Delta z_{g};",histName.Data());
    fh3PtTrueDeltaPtDeltaZg[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,-500,500,100,-0.5,0.5);
    fOutput->Add(fh3PtTrueDeltaPtDeltaZg[i]);

    histName = Form("fh3PtTruePtSJScalePtSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtSJScalePtSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,-1.,4.);
    fOutput->Add(fh3PtTruePtSJScalePtSJ[i]);
    
    histName = Form("fh3PtTruePtLSJScalePtLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtLSJScalePtLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,-1.,4.);
    fOutput->Add(fh3PtTruePtLSJScalePtLSJ[i]);

    histName = Form("fh3PtTruePtSLSJScalePtSLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtSLSJScalePtSLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,-1.,4.);
    fOutput->Add(fh3PtTruePtSLSJScalePtSLSJ[i]);

    //
    histName = Form("fh3PtTruePtRecoSJScalePtSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoSJScalePtSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoSJScalePtSJ[i]);
    
    histName = Form("fh3PtTruePtRecoLSJScalePtLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoLSJScalePtLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoLSJScalePtLSJ[i]);

    histName = Form("fh3PtTruePtRecoSLSJScalePtSLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoSLSJScalePtSLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,500,0,500,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoSLSJScalePtSLSJ[i]);
  }

  int nBinsLJ = (int)fPtLeadingMin.size();
  // fh2SLPtSubjetPtRatio21.reserve(nbins*fNcentBins);
  // fh2SLPtSubjetPtRatio32.reserve(nbins*fNcentBins);
  // fh2SLPtSubjetPtRatio43.reserve(nbins*fNcentBins);
  // fh2SLPtSubjetPtRatio54.reserve(nbins*fNcentBins);
  for (int i = 0; i < fNcentBins; i++) {
    std::vector<TH2F *> h2LJ21;
    std::vector<TH2F *> h2LJ32;
    std::vector<TH2F *> h2LJ43;
    std::vector<TH2F *> h2LJ54;
    std::vector<TH2F *> h2LJM21;
    std::vector<TH2F *> h2LJM32;
    std::vector<TH2F *> h2LJM43;
    std::vector<TH2F *> h2LJM54;
    std::vector<TH2F *> h2LJZg;
    std::vector<TH2F *> h2LJZgTrue;
    std::vector<TH2F *> h2LJZgNoRef;
    std::vector<TH2F *> h2LJThetag;
    std::vector<TH2F *> h2LJThetagTrue;
    std::vector<TH2F *> h2LJThetagNoRef;
    std::vector<TH2F *> h2LJDeltaPhi;
    std::vector<TH2F *> h2LJDeltaPhiTrue;
    std::vector<TH2F *> h2LJDeltaPhiNoRef;
    std::vector<TH3F *> h3LJZgDeltaPhi;
    
    for(int j = 0; j < nBinsLJ; j++) {
      histName = Form("fh2SLPtSubjetPtRatio21_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp21 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
      h2LJ21.push_back(htmp21);
      fOutput->Add(htmp21);

      histName = Form("fh2SLPtSubjetPtRatio32_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp32 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
      h2LJ32.push_back(htmp32);
      fOutput->Add(htmp32);

      histName = Form("fh2SLPtSubjetPtRatio43_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp43 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
      h2LJ43.push_back(htmp43);
      fOutput->Add(htmp43);

      histName = Form("fh2SLPtSubjetPtRatio54_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp54 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
      h2LJ54.push_back(htmp54);
      fOutput->Add(htmp54);

      histName = Form("fh2SLPtSubjetPtInvMass21_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;m_{SJ1,SJ2}",histName.Data());
      TH2F *htmpM21 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
      h2LJM21.push_back(htmpM21);
      fOutput->Add(htmpM21);

      histName = Form("fh2SLPtSubjetPtInvMass32_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;m_{SJ2,SJ3}",histName.Data());
      TH2F *htmpM32 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
      h2LJM32.push_back(htmpM32);
      fOutput->Add(htmpM32);

      histName = Form("fh2SLPtSubjetPtInvMass43_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;m_{SJ3,SJ4}",histName.Data());
      TH2F *htmpM43 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
      h2LJM43.push_back(htmpM43);
      fOutput->Add(htmpM43);

      histName = Form("fh2SLPtSubjetPtInvMass54_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;m_{SJ4,SJ5}",histName.Data());
      TH2F *htmpM54 = new TH2F(histName.Data(),histTitle.Data(),50,0,500,150,0,150);
      h2LJM54.push_back(htmpM54);
      fOutput->Add(htmpM54);

      //zg
      histName = Form("fh2SLPtZg_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzg = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
      h2LJZg.push_back(htmpzg);
      fOutput->Add(htmpzg);

      histName = Form("fh2SLPtZgTrue_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzgTrue = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
      h2LJZgTrue.push_back(htmpzgTrue);
      fOutput->Add(htmpzgTrue);

      histName = Form("fh2SLPtZgNoRef_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzgNoRef = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
      h2LJZgNoRef.push_back(htmpzgNoRef);
      fOutput->Add(htmpzgNoRef);

      //thetag
      histName = Form("fh2SLPtThetag_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
      TH2F *htmpthetag = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
      h2LJThetag.push_back(htmpthetag);
      fOutput->Add(htmpthetag);

      histName = Form("fh2SLPtThetagTrue_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
      TH2F *htmpthetagTrue = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
      h2LJThetagTrue.push_back(htmpthetagTrue);
      fOutput->Add(htmpthetagTrue);

      histName = Form("fh2SLPtThetagNoRef_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
      TH2F *htmpthetagNoRef = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
      h2LJThetagNoRef.push_back(htmpthetagNoRef);
      fOutput->Add(htmpthetagNoRef);
      
      //DeltaPhi
      histName = Form("fh2SLPtDeltaPhi_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhi = new TH2F(histName.Data(),histTitle.Data(),50,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhi.push_back(htmpDeltaPhi);
      fOutput->Add(htmpDeltaPhi);

      histName = Form("fh2SLPtDeltaPhiTrue_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhiTrue = new TH2F(histName.Data(),histTitle.Data(),50,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhiTrue.push_back(htmpDeltaPhiTrue);
      fOutput->Add(htmpDeltaPhiTrue);

      histName = Form("fh2SLPtDeltaPhiNoRef_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhiNoRef = new TH2F(histName.Data(),histTitle.Data(),50,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhiNoRef.push_back(htmpDeltaPhiNoRef);
      fOutput->Add(htmpDeltaPhiNoRef);

      histName = Form("fh3SLPtZgDeltaPhi_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};#Delta#phi",histName.Data());
      TH3F *htmpzgdphi = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5,36,0.,TMath::Pi());
      h3LJZgDeltaPhi.push_back(htmpzgdphi);
      fOutput->Add(htmpzgdphi);
    }
    fh2SLPtSubjetPtRatio21.push_back(h2LJ21);
    fh2SLPtSubjetPtRatio32.push_back(h2LJ32);
    fh2SLPtSubjetPtRatio43.push_back(h2LJ43);
    fh2SLPtSubjetPtRatio54.push_back(h2LJ54);
    fh2SLPtSubjetPtInvMass21.push_back(h2LJM21);
    fh2SLPtSubjetPtInvMass32.push_back(h2LJM32);
    fh2SLPtSubjetPtInvMass43.push_back(h2LJM43);
    fh2SLPtSubjetPtInvMass54.push_back(h2LJM54);
    fh2SLPtZg.push_back(h2LJZg);
    fh2SLPtZgTrue.push_back(h2LJZgTrue);
    fh2SLPtZgNoRef.push_back(h2LJZgNoRef);
    fh2SLPtThetag.push_back(h2LJThetag);
    fh2SLPtThetagTrue.push_back(h2LJThetagTrue);
    fh2SLPtThetagNoRef.push_back(h2LJThetagNoRef);
    fh2SLPtDeltaPhi.push_back(h2LJDeltaPhi);
    fh2SLPtDeltaPhiTrue.push_back(h2LJDeltaPhiTrue);
    fh2SLPtDeltaPhiNoRef.push_back(h2LJDeltaPhiNoRef);
    fh3SLPtZgDeltaPhi.push_back(h3LJZgDeltaPhi);
  }
}

//----------------------------------------------------------
void anaSubJet::ClearSubjetTreeVars() {
  //clear vectors in fSubjetTreeVars
  fSubjetTreeVars.fPt.clear();
  fSubjetTreeVars.fPtRaw.clear();
  fSubjetTreeVars.fEta.clear();
  fSubjetTreeVars.fPhi.clear();
  fSubjetTreeVars.fM.clear();
  fSubjetTreeVars.fPtG.clear();
  fSubjetTreeVars.fPtRawG.clear();
  fSubjetTreeVars.fEtaG.clear();
  fSubjetTreeVars.fPhiG.clear();
  fSubjetTreeVars.fMG.clear();
  fSubjetTreeVars.fPtSJ1.clear();
  fSubjetTreeVars.fPtSJ2.clear();
  fSubjetTreeVars.fEtaSJ1.clear();
  fSubjetTreeVars.fEtaSJ2.clear();
  fSubjetTreeVars.fPhiSJ1.clear();
  fSubjetTreeVars.fPhiSJ2.clear();
  fSubjetTreeVars.fMSJ1.clear();
  fSubjetTreeVars.fMSJ2.clear();
  fSubjetTreeVars.fZg.clear();
  fSubjetTreeVars.fThetag.clear();
  fSubjetTreeVars.fDropped.clear();
  fSubjetTreeVars.fCHF.clear();
  fSubjetTreeVars.fNHF.clear();
  fSubjetTreeVars.fCEF.clear();
  fSubjetTreeVars.fNEF.clear();
  fSubjetTreeVars.fMUF.clear();
  fSubjetTreeVars.fCHM.clear();
  fSubjetTreeVars.fNHM.clear();
  fSubjetTreeVars.fCEM.clear();
  fSubjetTreeVars.fNEM.clear();
  fSubjetTreeVars.fMUM.clear();
  fSubjetTreeVars.fPtRef.clear();
  fSubjetTreeVars.fEtaRef.clear();
  fSubjetTreeVars.fPhiRef.clear();
  fSubjetTreeVars.fPtGRef.clear();
  fSubjetTreeVars.fEtaGRef.clear();
  fSubjetTreeVars.fPhiGRef.clear();
  fSubjetTreeVars.fPtSJ1Ref.clear();
  fSubjetTreeVars.fPtSJ2Ref.clear();
  fSubjetTreeVars.fEtaSJ1Ref.clear();
  fSubjetTreeVars.fEtaSJ2Ref.clear();
  fSubjetTreeVars.fPhiSJ1Ref.clear();
  fSubjetTreeVars.fPhiSJ2Ref.clear();
  fSubjetTreeVars.fMSJ1Ref.clear();
  fSubjetTreeVars.fMSJ2Ref.clear();
  fSubjetTreeVars.fZgRef.clear();
  fSubjetTreeVars.fThetagRef.clear();
  fSubjetTreeVars.fDroppedRef.clear();
  fSubjetTreeVars.fSwap.clear();
  fSubjetTreeVars.fDeltaRSJ11.clear();
  fSubjetTreeVars.fDeltaRSJ12.clear();
  fSubjetTreeVars.fDeltaRSJ22.clear();
  fSubjetTreeVars.fDeltaRSJ21.clear();

  fSubjetTreeVars.fZCut0PtG.clear();
  fSubjetTreeVars.fZCut0EtaG.clear();
  fSubjetTreeVars.fZCut0PhiG.clear();
  fSubjetTreeVars.fZCut0MG.clear();
  fSubjetTreeVars.fZCut0PtSJ1.clear();
  fSubjetTreeVars.fZCut0PtSJ2.clear();
  fSubjetTreeVars.fZCut0EtaSJ1.clear();
  fSubjetTreeVars.fZCut0EtaSJ2.clear();
  fSubjetTreeVars.fZCut0PhiSJ1.clear();
  fSubjetTreeVars.fZCut0PhiSJ2.clear();
  fSubjetTreeVars.fZCut0MSJ1.clear();
  fSubjetTreeVars.fZCut0MSJ2.clear();
  fSubjetTreeVars.fZCut0Zg.clear();
  fSubjetTreeVars.fZCut0Thetag.clear();

  fSubjetTreeVars.fZCut0PtGRef.clear();
  fSubjetTreeVars.fZCut0EtaGRef.clear();
  fSubjetTreeVars.fZCut0PhiGRef.clear();
  fSubjetTreeVars.fZCut0MGRef.clear();
  fSubjetTreeVars.fZCut0PtSJ1Ref.clear();
  fSubjetTreeVars.fZCut0PtSJ2Ref.clear();
  fSubjetTreeVars.fZCut0EtaSJ1Ref.clear();
  fSubjetTreeVars.fZCut0EtaSJ2Ref.clear();
  fSubjetTreeVars.fZCut0PhiSJ1Ref.clear();
  fSubjetTreeVars.fZCut0PhiSJ2Ref.clear();
  fSubjetTreeVars.fZCut0MSJ1Ref.clear();
  fSubjetTreeVars.fZCut0MSJ2Ref.clear();
  fSubjetTreeVars.fZCut0ZgRef.clear();
  fSubjetTreeVars.fZCut0ThetagRef.clear();
}

//----------------------------------------------------------
double anaSubJet::DeltaR(double phi1, double phi2, double eta1, double eta2) {
  //calculate separation distance in eta,phi plane
  Double_t dPhi = phi1 - phi2;
  Double_t dEta = eta1 - eta2;
  dPhi = TVector2::Phi_mpi_pi(dPhi);
  double dr2 = dPhi * dPhi + dEta * dEta;
  if(dr2>0.) return sqrt(dr2);
  else       return -1.;
}

#include "UserCode/diall/interface/anaZgHistos.h"

#include "UserCode/diall/interface/lwJet.h"
#include "UserCode/diall/interface/rhoMap.h"
#include "UserCode/diall/interface/rhoMCWeights.h"

#include <iostream>

ClassImp(anaZgHistos)
   
anaZgHistos::anaZgHistos(const char *name, const char *title) 
:anaBaseTask(name,title),
  fTriggerObjName(""),
  fTriggerObj(0x0),
  fTrigEtaMin(-5.),
  fTrigEtaMax(5.),
  fVzOffset(0.),
  fNcentBins(4),
  fJetsName(""),
  fJetsCont(0x0),
  fJetsRefName(""),
  fJetsRefCont(0x0),
  fJetEtaMin(-5.),
  fJetEtaMax(5.),
  fMinDeltaR(0.1),
  fMaxDeltaR(999.),
  fMinRefPt(-10000.),
  fUseRhoMCWeight(false),
  fRhoMax(10000.),
  fNotUseRef(false),
  fDoSubjetSmearing(false),
  fSubjetSmearing(),
  fDoSubjetSmearingResolution(false),
  fSubjetSmearingResolution(),
  fDoJetShift(false),
  fJetShift(0.04),
  fDoZgReweight(false),
  f1ZgReweight(0x0),
  fDoJewelZgReweight(false),
  fJewelZgReweight(),
  fhCentrality(0),
  fh2RhoCent(),
  fh1Vz(),
  fh3PtEtaPhiTrigObj(),
  fh3PtEtaPhi(),
  fh2PtNSubjets(),
  fh2PtGenNSubjets(),
  fh2PtMass(),
  fh2PtPtGF(),
  fh2PtNDrop(),
  fh2PtSubjetPtRatio21(),
  fh2PtSubjetPtFrac1(),
  fh2PtSubjetPtFrac2(),
  fh2PtSubjetPtInvMass21(),
  fh2PtZg(),
  fh2PtGZg(),
  fh2PtZgChMax(),
  fh2PtZgDRNoPass(),
  fh2PtZgAll(),
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
  fh2PtChMax(),
  fh2PtThetag(),
  fh2PtThetagTrue(),
  fh2PtThetagNoRef(),
  fh3PtRecPtTrueThetag(),
  fh2PtDeltaR12(),
  fh2PtDeltaR12True(),
  fh2PtDeltaR12NoRef(),
  fh3PtRecPtTrueDeltaR12(),
  fh2PtMgOverPt(),
  fh2PtMgOverPtTrue(),
  fh2PtMgOverPtNoRef(),
  fh3PtRecPtTrueMgOverPt(),
  fh3PtZgThetag(),
  fh3PtTrueZgScaleZg(),
  fhnZgResponse()
{
  
  fh3PtEtaPhiTrigObj   = new TH3F*[fNcentBins];
  fh3PtEtaPhi          = new TH3F*[fNcentBins];
  fh2PtNSubjets        = new TH2F*[fNcentBins];
  fh2PtGenNSubjets     = new TH2F*[fNcentBins];
  fh2PtMass            = new TH2F*[fNcentBins];
  fh2PtPtGF            = new TH2F*[fNcentBins];
  fh2PtNDrop           = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio21 = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac1   = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac2   = new TH2F*[fNcentBins];
  fh2PtZg              = new TH2F*[fNcentBins];
  fh2PtGZg             = new TH2F*[fNcentBins];
  fh2PtZgChMax         = new TH2F*[fNcentBins];
  fh2PtZgDRNoPass      = new TH2F*[fNcentBins];
  fh2PtZgAll           = new TH2F*[fNcentBins];
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
  fh2PtChMax          = new TH2F*[fNcentBins];
  fh2PtThetag          = new TH2F*[fNcentBins];
  fh2PtThetagTrue      = new TH2F*[fNcentBins];
  fh2PtThetagNoRef     = new TH2F*[fNcentBins];
  fh3PtRecPtTrueThetag = new TH3F*[fNcentBins];
  fh2PtDeltaR12          = new TH2F*[fNcentBins];
  fh2PtDeltaR12True      = new TH2F*[fNcentBins];
  fh2PtDeltaR12NoRef     = new TH2F*[fNcentBins];
  fh3PtRecPtTrueDeltaR12 = new TH3F*[fNcentBins];
  fh2PtMgOverPt          = new TH2F*[fNcentBins];
  fh2PtMgOverPtTrue      = new TH2F*[fNcentBins];
  fh2PtMgOverPtNoRef     = new TH2F*[fNcentBins];
  fh3PtRecPtTrueMgOverPt = new TH3F*[fNcentBins];
  fh3PtZgThetag          = new TH3F*[fNcentBins];
  fh2PtSubjetPtInvMass21 = new TH2F*[fNcentBins];
  fh3PtTrueZgScaleZg   = new TH3F*[fNcentBins];

  fhnZgResponse        = new THnSparse*[fNcentBins];

  for (int i = 0; i < fNcentBins; i++) {
    fh3PtEtaPhiTrigObj[i] = 0;
    fh3PtEtaPhi[i]      = 0;
    fh2PtNSubjets[i]    = 0;
    fh2PtGenNSubjets[i] = 0;
    fh2PtMass[i]        = 0;
    fh2PtPtGF[i]        = 0;
    fh2PtNDrop[i]       = 0;
    fh2PtSubjetPtRatio21[i] = 0;
    fh2PtSubjetPtFrac1[i]   = 0;
    fh2PtSubjetPtFrac2[i]   = 0;
    fh2PtSubjetPtInvMass21[i]   = 0;
    fh2PtZg[i]                  = 0;
    fh2PtGZg[i]                 = 0;
    fh2PtZgChMax[i]             = 0;
    fh2PtZgDRNoPass[i]          = 0;
    fh2PtZgAll[i]               = 0;
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
    fh2PtChMax[i]               = 0;
    fh2PtThetag[i]              = 0;
    fh2PtThetagTrue[i]          = 0;
    fh2PtThetagNoRef[i]         = 0;
    fh3PtRecPtTrueThetag[i]     = 0;
    fh2PtDeltaR12[i]              = 0;
    fh2PtDeltaR12True[i]          = 0;
    fh2PtDeltaR12NoRef[i]         = 0;
    fh3PtRecPtTrueDeltaR12[i]     = 0;
    fh2PtMgOverPt[i]              = 0;
    fh2PtMgOverPtTrue[i]          = 0;
    fh2PtMgOverPtNoRef[i]         = 0;
    fh3PtRecPtTrueMgOverPt[i]     = 0;
    fh3PtZgThetag[i]              = 0;
    fh3PtTrueZgScaleZg[i]       = 0;
    fhnZgResponse[i]            = 0;
  }
}

//----------------------------------------------------------
void anaZgHistos::Exec(Option_t * /*option*/)
{
  //printf("anaZgHistos executing\n");
  anaBaseTask::Exec();
  if(!fInitOutput) CreateOutputObjects();

  //Get MC weight
  float weight = 1.;
  if(fHiEvent) {
    if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
  }
  fh1Vz->Fill(fHiEvent->GetVz()-fVzOffset,weight);

  //reject events not passing standard event selections
  if(!SelectEvent()) return;
  
  //Printf("event selected");

  //Get trigger object map
  if(!fTriggerObj && !fTriggerObjName.IsNull()) {
    fTriggerObj = dynamic_cast<triggerObjectMap*>(fEventObjects->FindObject(fTriggerObjName.Data()));
    if(!fTriggerObj) {
      Printf("%s: !!WARNING: Couldn't locate %s branch",GetName(),fTriggerObjName.Data());
      return;
    }
  }
  
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
  
  //Determine centrality bin
  Double_t cent = 0.;
  if(fHiEvent) cent = fHiEvent->GetCentrality();
  
  Int_t fCentBin = 0;
  if(fNcentBins==4) {
    double offset = 0.;
    if(fUseRhoMCWeight) offset = 2.;
    if(cent>=(0.+offset) && cent<(10.+offset))       fCentBin = 0;
    else if(cent>=(10.+offset) && cent<(30.+offset)) fCentBin = 1;
    else if(cent>=(30.+offset) && cent<(50.+offset)) fCentBin = 2;
    else if(cent>=(50.+offset) && cent<(80.+offset)) fCentBin = 3;
    else fCentBin = -1;
  }

  if(fTriggerObj) {
    if(fTriggerObj->GetTriggerEta()<fTrigEtaMin || fTriggerObj->GetTriggerEta()>fTrigEtaMax) return;
  }

  if(fCentBin>-1 && fCentBin<fNcentBins && fTriggerObj) {
    // TString strTrigObj = fTriggerList[0];
    // strTrigObj.ReplaceAll("v1","v");
    // strTrigObj.ReplaceAll("v2","v");
    // strTrigObj.ReplaceAll("v3","v");
    // strTrigObj.ReplaceAll("v4","v");
    // strTrigObj.ReplaceAll("v5","v");
    fh3PtEtaPhiTrigObj[fCentBin]->Fill(fTriggerObj->GetTriggerPt(),fTriggerObj->GetTriggerEta(),fTriggerObj->GetTriggerPhi()); 
  }
  
  float rhoCentral = -1.;
  if(fHiEvent) {
    rhoMap *rm = fHiEvent->GetRhoMap();
    // if(rm) {
    //   for(int ieta = 0; ieta<rm->GetNEtaBins(); ++ieta) {
    //     Printf("ieta: %d rho: %f",ieta,rm->GetValue(ieta));
    //   }
    // } else
    //   Printf("rhomap not found");
    if(rm) {
      int icentral = TMath::FloorNint( (double)rm->GetNEtaBins()/2.);
      rhoCentral = rm->GetValue(icentral);
    }
  }
  if(rhoCentral>fRhoMax) return;

  rhoMCWeights rhomcW;
  double weightRho = 1.;
  if(fUseRhoMCWeight)
    weightRho = rhomcW.getWeights(cent,rhoCentral);

  weight*=weightRho;

  fhCentrality->Fill(cent,weight);
  fh2RhoCent->Fill(cent,rhoCentral,weight);

  // Printf("NJets: %d cent: %f rhoCentral: %f",fJetsCont->GetNJets(),cent,rhoCentral);
  for (int i = 0; i < fJetsCont->GetNJets(); i++) {
    // Printf("get jet");
    lwJet *jet = fJetsCont->GetJet(i);
    // Printf("got jet");
    // Printf("pt: %f",jet->Pt());
    lwJet *jetNotGroomed = 0x0;
    if(fJetsRefCont) { //pick-up ungroomed jet from separate container which was previously matched
      // Printf("get ungroomed jet");
      int id = jet->GetMatchId1();
      // Printf("id: %d",id);
      if( id<0 || id>fJetsRefCont->GetNJets() ) continue;
      jetNotGroomed = fJetsRefCont->GetJet(id);
    } else { //assume main container is ungroomed (applies to gen-level)
      jetNotGroomed = jet;
    }
    if(!jetNotGroomed) continue;
    // Printf("got ungroomed jet");
    // if(!jetNotGroomed) Printf("ungroomed jet pointer empty");
     
    Double_t pt  = jetNotGroomed->Pt();
    // Printf("pt: %f",pt);
    Double_t ptg = jet->Pt();
    // Printf("ptg: %f",ptg);
    Double_t phi = jetNotGroomed->Phi();
    Double_t eta = jetNotGroomed->Eta();

    // Printf("pt: %f ptg: %f",pt,ptg);
    
    Double_t m   = jetNotGroomed->M();
    Double_t mg  = jet->M();
    Int_t    ndrop = jet->GetNDroppedBranches();
    // Printf("pt: %f ptg: %f mg: %f ndrop: %d",pt,ptg,mg,ndrop);
 
    if(pt<10.) continue;
    if(eta<fJetEtaMin || eta>fJetEtaMax) continue;
    //if(fMinRefPt>-1. && jetNotGroomed->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    
 
    if(fCentBin>-1 && fCentBin<fNcentBins) {
      fh3PtEtaPhi[fCentBin]->Fill(pt,eta,phi,weight);
      fh2PtMass[fCentBin]->Fill(pt,m,weight);
    }
       
    std::vector<float> sjpt  = jet->GetSubJetPt();
    std::vector<float> sjeta = jet->GetSubJetEta();
    std::vector<float> sjphi = jet->GetSubJetPhi();
    std::vector<float> sjm   = jet->GetSubJetM();
    int nsubjets = (int)sjpt.size();

    if(fCentBin>-1 && fCentBin<fNcentBins) {
      fh2PtNSubjets[fCentBin]->Fill(pt,nsubjets,weight);
      fh2PtGenNSubjets[fCentBin]->Fill(jetNotGroomed->GetRefPt(),nsubjets,weight);
    }

    double ptrecsj1 = 1.;
    double ptrecsj2 = 0.;

    double zg = 0.;
    double thetag = -1.;
    double deltaR12 = 999.;
    //Printf("keeping all jets now nsubjets: %d",nsubjets);
    if(nsubjets>1) {

      ptrecsj1 = sjpt.at(0);
      ptrecsj2 = sjpt.at(1);
      
      zg = std::min(sjpt.at(0),sjpt.at(1))/(sjpt.at(0)+sjpt.at(1));
    }
    //Printf("zg: %f",zg);
    //Fill zg histogram with all reconstructed jets including those with zg<zcut
    if(fCentBin>-1 && fCentBin<fNcentBins) fh2PtZgAll[fCentBin]->Fill(pt,zg,weight);

    if(nsubjets<2) continue;
    //Printf("passed nsubjets cut");
    //gen-level jet and subjets
    //double refpt = jetNotGroomed->GetRefPt();
    std::vector<float> refsjpt   = jet->GetRefSubJetPt();
    std::vector<float> refsjeta  = jet->GetRefSubJetEta();
    std::vector<float> refsjphi  = jet->GetRefSubJetPhi();
    std::vector<float> refsjm    = jet->GetRefSubJetM();

    if(fNotUseRef) {
      Printf("Gen level not found, used matched jet");
      refsjpt = jetNotGroomed->GetSubJetPt();
      refsjeta = jetNotGroomed->GetSubJetEta();
      refsjphi = jetNotGroomed->GetSubJetPhi();
      refsjm = jetNotGroomed->GetSubJetM();
    }

    double refpt = jetNotGroomed->GetRefPt();
    if(fNotUseRef) refpt = jetNotGroomed->Pt();
    
    float refzg = -1.;
    double dzgrel = -999.;
    if(refsjpt.size()>1) {
      refzg = std::min(refsjpt.at(0),refsjpt.at(1))/(refsjpt.at(0)+refsjpt.at(1));
      dzgrel = (zg - refzg)/refzg;
    }

    //Printf("n ref jets: %d refzg: %f",(int)refsjpt.size(),refzg);
    
    float weightzg = 1.;
    if(fDoZgReweight && refzg>0.) {
      weightzg = f1ZgReweight->Eval(refzg);
      weight*=weightzg;
    }
    if(fDoJewelZgReweight && refzg>0.) {
      weightzg = fJewelZgReweight.getWeight(refpt,refzg,cent);
      weight*=weightzg;
    }

    if(fDoSubjetSmearing) {
      if(fCentBin<0 || fCentBin>=fNcentBins)
        continue;
      //Printf("doing subjet smearing");
      fSubjetSmearing[fCentBin].smearJet(pt,sjpt.at(0),sjpt.at(1),pt,ptrecsj1,ptrecsj2);
      zg = std::min(ptrecsj1,ptrecsj2)/(ptrecsj1+ptrecsj2);
      //Printf("smearing done");
      sjpt.at(0) = ptrecsj1;
      sjpt.at(1) = ptrecsj2;
    }

    if(fDoSubjetSmearingResolution) {
      if(fCentBin<0 || fCentBin>=fNcentBins)
        continue;
      //Printf("doing subjet smearing");
      if(!fSubjetSmearingResolution[fCentBin].smearJet(pt,zg,pt,zg)) continue;
      //                  //Printf("smearing done");
      sjpt.at(0) = (1.-zg)*pt;
      sjpt.at(1) = zg*pt;
    }

    if(fDoJetShift) {
      pt += pt*fJetShift;
    }
    
    float ptrat = -1.;
    for(int is = 1; is<nsubjets; ++is) {
      if(sjpt.at(is-1)>0.) {
        //Printf("jetpt: %f sjpt1: %f sjpt2: %f",pt,sjpt.at(is-1),sjpt.at(is));
        ptrat = sjpt.at(is)/sjpt.at(is-1);
        
        TLorentzVector v1;
        TLorentzVector v2;
        v1.SetPtEtaPhiM(ptrecsj1,sjeta.at(is-1),sjphi.at(is-1),sjm.at(is-1));
        v2.SetPtEtaPhiM(ptrecsj2,sjeta.at(is),sjphi.at(is),sjm.at(is));
        //v1.SetPtEtaPhiM(sjpt.at(is-1),sjeta.at(is-1),sjphi.at(is-1),sjm.at(is-1));
        //v2.SetPtEtaPhiM(sjpt.at(is),sjeta.at(is),sjphi.at(is),sjm.at(is));
        TLorentzVector disj = v1 + v2;
        
        if(fCentBin>-1 && fCentBin<fNcentBins) {
          if(is==1) fh2PtSubjetPtRatio21[fCentBin]->Fill(pt,ptrat,weight);
          if(is==1) fh2PtSubjetPtInvMass21[fCentBin]->Fill(pt,disj.M(),weight);
        }
      }
    }
   
    TLorentzVector v1;
    TLorentzVector v2;
    v1.SetPtEtaPhiM(ptrecsj1,sjeta.at(0),sjphi.at(0),sjm.at(0));
    v2.SetPtEtaPhiM(ptrecsj2,sjeta.at(1),sjphi.at(1),sjm.at(1));
    // v1.SetPtEtaPhiM(sjpt.at(0),sjeta.at(0),sjphi.at(0),sjm.at(0));
    // v2.SetPtEtaPhiM(sjpt.at(1),sjeta.at(1),sjphi.at(1),sjm.at(1));
    // Printf("calculate thetag");
    thetag = v1.Angle(v2.Vect());
    // Printf("ptjet: %f zg: %f thetag: %f",pt,zg,thetag);
    deltaR12 = DeltaR(sjphi.at(0),sjphi.at(1),sjeta.at(0),sjeta.at(1));

    //reject jets outside requested DeltaR12 + bookkeep them
    if(deltaR12<fMinDeltaR || deltaR12>fMaxDeltaR) {
      if(fCentBin>-1 && fCentBin<fNcentBins)
        fh2PtZgDRNoPass[fCentBin]->Fill(pt,zg,weight);
      continue;
    }
    
    if(fCentBin>-1 && fCentBin<fNcentBins) {
      
      fh2PtPtGF[fCentBin]->Fill(pt,ptg/pt);
      fh2PtNDrop[fCentBin]->Fill(pt,ndrop);
      fh2PtZg[fCentBin]->Fill(pt,zg,weight);
      fh2PtGZg[fCentBin]->Fill(ptg,zg,weight);
      if(jetNotGroomed->GetChargedMax()>5.) fh2PtZgChMax[fCentBin]->Fill(pt,zg,weight);
      fh2PtChMax[fCentBin]->Fill(pt,jetNotGroomed->GetChargedMax(),weight);
      fh2PtThetag[fCentBin]->Fill(pt,thetag,weight);
      fh2PtDeltaR12[fCentBin]->Fill(pt,deltaR12,weight);
      if(ptg>0.) fh2PtMgOverPt[fCentBin]->Fill(pt,mg/ptg,weight);
      fh3PtZgThetag[fCentBin]->Fill(pt,zg,thetag,weight);
      
      if(refpt<fMinRefPt) { //fakes
        fh2PtZgNoRef[fCentBin]->Fill(pt,zg,weight);
        fh2PtThetagNoRef[fCentBin]->Fill(pt,thetag,weight);
        fh2PtDeltaR12NoRef[fCentBin]->Fill(pt,deltaR12,weight);
        if(ptg>0.) fh2PtMgOverPtNoRef[fCentBin]->Fill(pt,mg/ptg,weight);
      }
      else { //true matches: filling reco level info
        fh2PtZgTrue[fCentBin]->Fill(pt,zg,weight);
        fh2PtThetagTrue[fCentBin]->Fill(pt,thetag,weight);
        fh2PtDeltaR12True[fCentBin]->Fill(pt,deltaR12,weight);
        if(ptg>0.) fh2PtMgOverPtTrue[fCentBin]->Fill(pt,mg/ptg,weight);
        
        fh3PtRecPtTrueZg[fCentBin]->Fill(pt,refpt,zg,weight);
        fh3PtRecPtTrueThetag[fCentBin]->Fill(pt,refpt,thetag,weight);
        fh3PtRecPtTrueDeltaR12[fCentBin]->Fill(pt,refpt,deltaR12,weight);
        if(ptg>0.) fh3PtRecPtTrueMgOverPt[fCentBin]->Fill(pt,refpt,mg/ptg,weight);
        
        if(sjphi.size()>1 && refsjphi.size()>1) {
          if(sjphi[0]>-10. && refsjphi[0]>-10. && sjphi[1]>-10. && refsjphi[1]>-10) {
          
            double dr11 = DeltaR(sjphi.at(0),refsjphi.at(0),sjeta.at(0),refsjeta.at(0));
            double dr12 = DeltaR(sjphi.at(0),refsjphi.at(1),sjeta.at(0),refsjeta.at(1));

            double dr21 = DeltaR(sjphi.at(1),refsjphi.at(0),sjeta.at(1),refsjeta.at(0));
            double dr22 = DeltaR(sjphi.at(1),refsjphi.at(1),sjeta.at(1),refsjeta.at(1));

            if(dr12>dr11 && dr21>dr22) {
        
              double dptrel = (pt - refpt)/refpt;
              double var[6] = {(double)zg,(double)refzg,(double)pt,(double)refpt,dptrel,dzgrel};
              fhnZgResponse[fCentBin]->Fill(var,weight);
              fh3PtTrueDeltaPtDeltaZg[fCentBin]->Fill(refpt,pt-refpt,zg-refzg,weight);
              if(refzg>0.) fh3PtTrueZgScaleZg[fCentBin]->Fill(refpt,refzg,zg/refzg);
        
              for(uint sj = 0; sj<refsjpt.size(); ++sj) {
                if(sj>1) continue;
                if(sj>=sjpt.size()) continue;
                double dptrel = (sjpt.at(sj)-refsjpt.at(sj))/refsjpt.at(sj);
                fh3PtTruePtSJScalePtSJ[fCentBin]->Fill(refpt,refsjpt.at(sj),dptrel,weight);
                fh3PtTruePtRecoSJScalePtSJ[fCentBin]->Fill(refpt,sjpt.at(sj),dptrel+1.,weight);
                if(sj==0) {
                  fh3PtTruePtLSJScalePtLSJ[fCentBin]->Fill(refpt,refsjpt.at(sj),dptrel,weight);
                  fh3PtTruePtRecoLSJScalePtLSJ[fCentBin]->Fill(refpt,sjpt.at(sj),dptrel+1.,weight);
                }
                else if(sj==1) {
                  fh3PtTruePtSLSJScalePtSLSJ[fCentBin]->Fill(refpt,refsjpt.at(sj),dptrel,weight);
                  fh3PtTruePtRecoSLSJScalePtSLSJ[fCentBin]->Fill(refpt,sjpt.at(sj),dptrel+1.,weight);
                }
              }//subjet loop
            }
          }
        }
      }//true jet matched
    }//centbins
    
    for(int is = 0; is<nsubjets; ++is) {
      float ptrat = sjpt.at(is)/pt;
      if(fCentBin>-1 && fCentBin<fNcentBins) {
        if(is==0) fh2PtSubjetPtFrac1[fCentBin]->Fill(pt,ptrat,weight);
        if(is==1) fh2PtSubjetPtFrac2[fCentBin]->Fill(pt,ptrat,weight);
      }
    }
  }
}

//----------------------------------------------------------
void anaZgHistos::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaZgHistos: fOutput not present");
    return;
  }
  
  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  fh2RhoCent = new TH2F("fh2RhoCent","fh2RhoCent;centrality;#rho",100,0,100,500,0,500);
  fOutput->Add(fh2RhoCent);

  fh1Vz = new TH1F("fh1Vz","fh1Vz",120,-30.,30.);
  fOutput->Add(fh1Vz);
  
  TString histTitle = "";
  TString histName  = "";

  //Binning for THnSparse
  const int nBinsPt  = 50;
  const double minPt = 0.;
  const double maxPt = 500.;

  const int nBinsZg = 10;//50;
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
    histName = Form("fh3PtEtaPhiTrigObj_%d",i);
    histTitle = Form("%s;pt;#eta;#phi;",histName.Data());
    fh3PtEtaPhiTrigObj[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhiTrigObj[i]);

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

    histName = Form("fh2PtPtGF_%d",i);
    histTitle = Form("%s;p_{T,jet};p_{T,G}/p_{T,jet}",histName.Data());
    fh2PtPtGF[i] = new TH2F(histName.Data(),histTitle.Data(),50,0.,500.,101,0.,1.01);
    fOutput->Add(fh2PtPtGF[i]);  

    histName = Form("fh2PtNDrop_%d",i);
    histTitle = Form("%s;p_{T,jet};N_{drop}",histName.Data());
    fh2PtNDrop[i] = new TH2F(histName.Data(),histTitle.Data(),50,0.,500.,50,0.,50.);
    fOutput->Add(fh2PtNDrop[i]);
 
    histName = Form("fh2PtSubjetPtRatio21_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
    fh2PtSubjetPtRatio21[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio21[i]);
 
    histName = Form("fh2PtSubjetPtFrac1_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ1}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac1[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac1[i]);

    histName = Form("fh2PtSubjetPtFrac2_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac2[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac2[i]);

    histName = Form("fh2PtSubjetPtInvMass21_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
    fh2PtSubjetPtInvMass21[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,30,0,150);
    fOutput->Add(fh2PtSubjetPtInvMass21[i]);
    
    histName = Form("fh2PtZg_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZg[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZg[i]);

    histName = Form("fh2PtGZg_%d",i);
    histTitle = Form("%s;p_{T,g};z_{g};",histName.Data());
    fh2PtGZg[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtGZg[i]);

    histName = Form("fh2PtZgChMax_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgChMax[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgChMax[i]);

    histName = Form("fh2PtZgDRNoPass_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgDRNoPass[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgDRNoPass[i]);

    histName = Form("fh2PtZgAll_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgAll[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgAll[i]);

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

    histName = Form("fh2PtChMax_%d",i);
    histTitle = Form("%s;p_{T};p_{T,ch max};",histName.Data());
    fh2PtChMax[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,100.);
    fOutput->Add(fh2PtChMax[i]);
    
    histName = Form("fh2PtThetag_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtThetag[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetag[i]);

    histName = Form("fh2PtThetagTrue_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtThetagTrue[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetagTrue[i]);

    histName = Form("fh2PtThetagNoRef_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtThetagNoRef[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetagNoRef[i]);

    histName = Form("fh3PtRecPtTrueThetag_%d",i);
    histTitle = Form("%s;p_{T,rec};p_{T,true};#theta_{g};",histName.Data());
    fh3PtRecPtTrueThetag[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,500,50,0,1.);
    fOutput->Add(fh3PtRecPtTrueThetag[i]);

    histName = Form("fh2PtDeltaR12_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtDeltaR12[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtDeltaR12[i]);

    histName = Form("fh2PtDeltaR12True_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtDeltaR12True[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtDeltaR12True[i]);

    histName = Form("fh2PtDeltaR12NoRef_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtDeltaR12NoRef[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtDeltaR12NoRef[i]);

    //
    histName = Form("fh2PtMgOverPt_%d",i);
    histTitle = Form("%s;p_{T};M_{g}/p_{T,g};",histName.Data());
    fh2PtMgOverPt[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtMgOverPt[i]);

    histName = Form("fh2PtMgOverPtTrue_%d",i);
    histTitle = Form("%s;p_{T};M_{g}/p_{T,g};",histName.Data());
    fh2PtMgOverPtTrue[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtMgOverPtTrue[i]);

    histName = Form("fh2PtMgOverPtNoRef_%d",i);
    histTitle = Form("%s;p_{T};M_{g}/p_{T,g};",histName.Data());
    fh2PtMgOverPtNoRef[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtMgOverPtNoRef[i]);

    histName = Form("fh3PtRecPtTrueMgOverPt_%d",i);
    histTitle = Form("%s;p_{T,rec};p_{T,true};M_{g}/p_{T,g};",histName.Data());
    fh3PtRecPtTrueMgOverPt[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,500,50,0,1.);
    fOutput->Add(fh3PtRecPtTrueMgOverPt[i]);
    //
    
    histName = Form("fh3PtZgThetag_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh3PtZgThetag[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5,100,0.,1.);
    fOutput->Add(fh3PtZgThetag[i]);
    
    histName = Form("fh3PtRecPtTrueDeltaR12_%d",i);
    histTitle = Form("%s;p_{T,rec};p_{T,true};#theta_{g};",histName.Data());
    fh3PtRecPtTrueDeltaR12[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,500,50,0,1.);
    fOutput->Add(fh3PtRecPtTrueDeltaR12[i]);

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
    fh3PtTruePtSJScalePtSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,-1.,4.);
    fOutput->Add(fh3PtTruePtSJScalePtSJ[i]);
    
    histName = Form("fh3PtTruePtLSJScalePtLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtLSJScalePtLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,-1.,4.);
    fOutput->Add(fh3PtTruePtLSJScalePtLSJ[i]);

    histName = Form("fh3PtTruePtSLSJScalePtSLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtSLSJScalePtSLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,-1.,4.);
    fOutput->Add(fh3PtTruePtSLSJScalePtSLSJ[i]);

    //
    histName = Form("fh3PtTruePtRecoSJScalePtSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoSJScalePtSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoSJScalePtSJ[i]);
    
    histName = Form("fh3PtTruePtRecoLSJScalePtLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoLSJScalePtLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoLSJScalePtLSJ[i]);

    histName = Form("fh3PtTruePtRecoSLSJScalePtSLSJ_%d",i);
    histTitle = Form("%s;p_{T,jet,true};p_{T,SJ,true};#Delta p_{T}/p_{T,SJ,true};",histName.Data());
    fh3PtTruePtRecoSLSJScalePtSLSJ[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,200,200,0.,4.);
    fOutput->Add(fh3PtTruePtRecoSLSJScalePtSLSJ[i]);

    histName = Form("fh3PtTrueZgScaleZg_%d",i);
    histTitle = Form("%s;p_{T,jet,true};z_{g,true};z_{g,reco}/z_{g,true};",histName.Data());
    fh3PtTrueZgScaleZg[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5,200,0.,4.);
    fOutput->Add(fh3PtTrueZgScaleZg[i]);
  }

}

//----------------------------------------------------------
double anaZgHistos::DeltaR(double phi1, double phi2, double eta1, double eta2) {
  //calculate separation distance in eta,phi plane
  Double_t dPhi = phi1 - phi2;
  Double_t dEta = eta1 - eta2;
  dPhi = TVector2::Phi_mpi_pi(dPhi);
  double dr2 = dPhi * dPhi + dEta * dEta;
  if(dr2>0.) return sqrt(dr2);
  else       return -1.;
}

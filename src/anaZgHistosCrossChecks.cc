#include "UserCode/diall/interface/anaZgHistosCrossChecks.h"

#include "UserCode/diall/interface/lwJet.h"
#include "UserCode/diall/interface/rhoMap.h"
#include "UserCode/diall/interface/rhoMCWeights.h"

#include <iostream>

ClassImp(anaZgHistosCrossChecks)
   
anaZgHistosCrossChecks::anaZgHistosCrossChecks(const char *name, const char *title) 
:anaBaseTask(name,title),
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
  fDoSubjetSmearing(false),
  fSubjetSmearing(),
  fDoSubjetSmearingResolution(false),
  fSubjetSmearingResolution(),
  fDoJetShift(false),
  fJetShift(0.04),
  fDoZgReweight(false),
  f1ZgReweight(0x0),
  fhCentrality(0),
  fh2RhoCent(),
  fh3PtEtaPhi(),
  fh2PtNSubjets(),
  fh2PtGenNSubjets(),
  fh2PtMass(),
  fh2PtPtGF(),
  fh2PtSubjetPtRatio21(),
  fh2PtSubjetPtFrac1(),
  fh2PtSubjetPtFrac2(),
  fh2PtZg(),
  fh2PtThetag(),
  fh2ZgPtRatio()
{
  
  fh3PtEtaPhi          = new TH3F*[fNcentBins];
  fh2PtNSubjets        = new TH2F*[fNcentBins];
  fh2PtGenNSubjets     = new TH2F*[fNcentBins];
  fh2PtMass            = new TH2F*[fNcentBins];
  fh2PtPtGF            = new TH2F*[fNcentBins];
  fh2PtSubjetPtRatio21 = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac1   = new TH2F*[fNcentBins];
  fh2PtSubjetPtFrac2   = new TH2F*[fNcentBins];
  fh2PtZg              = new TH2F*[fNcentBins];
  fh2PtThetag          = new TH2F*[fNcentBins];
  fh2ZgPtRatio         = new TH2F*[fNcentBins];

  for (int i = 0; i < fNcentBins; i++) {
    fh3PtEtaPhi[i]      = 0;
    fh2PtNSubjets[i]    = 0;
    fh2PtGenNSubjets[i] = 0;
    fh2PtMass[i]        = 0;
    fh2PtPtGF[i]        = 0;
    fh2PtSubjetPtRatio21[i] = 0;
    fh2PtSubjetPtFrac1[i]   = 0;
    fh2PtSubjetPtFrac2[i]   = 0;
    fh2PtZg[i]                  = 0;
    fh2PtThetag[i]              = 0;
    fh2ZgPtRatio[i]             = 0;
  }
}

//----------------------------------------------------------
void anaZgHistosCrossChecks::Exec(Option_t * /*option*/)
{
  //printf("anaZgHistosCrossChecks executing\n");
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
  
  //Get MC weight
  float weight = 1.;
  if(fHiEvent) {
    if(fHiEvent->GetWeight()>0.) weight = fHiEvent->GetWeight();
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

  for (int i = 0; i < fJetsCont->GetNJets(); i++) {
    lwJet *jet = fJetsCont->GetJet(i);
    lwJet *jetNotGroomed = 0x0;
    if(fJetsRefCont) { //pick-up ungroomed jet from separate container which was previously matched
      int id = jet->GetMatchId1();
      if( id<0 || id>fJetsRefCont->GetNJets() ) continue;
      jetNotGroomed = fJetsRefCont->GetJet(id);
    } else { //assume main container is ungroomed (applies to gen-level)
      jetNotGroomed = jet;
    }
     
    Double_t pt  = jetNotGroomed->Pt();
    Double_t ptg = jet->Pt();
    Double_t phi = jetNotGroomed->Phi();
    Double_t eta = jetNotGroomed->Eta();
    Double_t m = jetNotGroomed->M();
    
    if(pt<10.) continue;
    if(eta<fJetEtaMin || eta>fJetEtaMax) continue;
    if(fMinRefPt>-1. && jetNotGroomed->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    
 
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

    if(nsubjets<2) continue;

    double ptrecsj1 = sjpt.at(0);
    double ptrecsj2 = sjpt.at(1);
    
    double zg = std::min(sjpt.at(0),sjpt.at(1))/(sjpt.at(0)+sjpt.at(1));
    double thetag = -1.;
    double deltaR12 = 999.;

    //gen-level jet and subjets
    //double refpt = jetNotGroomed->GetRefPt();
    std::vector<float> refsjpt   = jet->GetRefSubJetPt();
    std::vector<float> refsjeta  = jet->GetRefSubJetEta();
    std::vector<float> refsjphi  = jet->GetRefSubJetPhi();
    std::vector<float> refsjm    = jet->GetRefSubJetM();

    float refzg = -1.;
    //    double dzgrel = -999.;
    if(refsjpt.size()>1) {
      refzg = std::min(refsjpt.at(0),refsjpt.at(1))/(refsjpt.at(0)+refsjpt.at(1));
      //dzgrel = (zg - refzg)/refzg;
    }

    float weightzg = 1.;
    if(fDoZgReweight && refzg>0.) {
      weightzg = f1ZgReweight->Eval(refzg);
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
          //if(is==1) fh2PtSubjetPtInvMass21[fCentBin]->Fill(pt,disj.M(),weight);
        }
      }
    }
   
    TLorentzVector v1;
    TLorentzVector v2;
    v1.SetPtEtaPhiM(ptrecsj1,sjeta.at(0),sjphi.at(0),sjm.at(0));
    v2.SetPtEtaPhiM(ptrecsj2,sjeta.at(1),sjphi.at(1),sjm.at(1));
    // v1.SetPtEtaPhiM(sjpt.at(0),sjeta.at(0),sjphi.at(0),sjm.at(0));
    // v2.SetPtEtaPhiM(sjpt.at(1),sjeta.at(1),sjphi.at(1),sjm.at(1));
    thetag = v1.Angle(v2.Vect());
    //Printf("ptjet: %f zg: %f thetag: %f",pt,zg,thetag);
    deltaR12 = DeltaR(sjphi.at(0),sjphi.at(1),sjeta.at(0),sjeta.at(1));
    
    if(deltaR12<fMinDeltaR || deltaR12>fMaxDeltaR) continue;
 
    if(fCentBin>-1 && fCentBin<fNcentBins) {
      
      fh2PtPtGF[fCentBin]->Fill(pt,ptg/pt);
      fh2PtZg[fCentBin]->Fill(pt,zg,weight);
      fh2PtThetag[fCentBin]->Fill(pt,thetag,weight);

    }//centbins
    
    for(int is = 0; is<nsubjets; ++is) {
      float ptrat = sjpt.at(is)/pt;
      if(fCentBin>-1 && fCentBin<fNcentBins) {
        if(is==0) fh2PtSubjetPtFrac1[fCentBin]->Fill(pt,ptrat,weight);
        if(is==1) fh2PtSubjetPtFrac2[fCentBin]->Fill(pt,ptrat,weight);
      }
    }

    //Do dijet balance here as function of Zg. Use same binning as LLR analysis
    if(pt<100.) continue;
    
    bool foundSL = false;
    for (int j = i+1; j < fJetsCont->GetNJets(); j++) {
      if(foundSL) continue;//only correlate with 2nd leading jet in acceptance
      
      lwJet *jet2Groomed = fJetsCont->GetJet(j);

      lwJet *jet2 = 0x0;
      if(fJetsRefCont) { //pick-up ungroomed jet from separate container which was previously matched
        int id = jet2Groomed->GetMatchId1();
        if( id<0 || id>fJetsRefCont->GetNJets() ) continue;
        jet2 = fJetsRefCont->GetJet(id);
      } else { //assume main container is ungroomed (applies to gen-level)
        jet2 = jet2Groomed;
      }
      
      if(fabs(jet2->Pt()-0.)<1e-6) continue; //remove ghosts
      if(jet2->Eta()<fJetEtaMin || jet2->Eta()>fJetEtaMax) continue;
      if(fMinRefPt>-1. && jet2->GetRefPt()<fMinRefPt) continue; //remove fakes in MC

      if(jet2->Pt()<40.) continue;
      
      foundSL = true;

      double dphi = acos(cos(jet->Phi() - jet2->Phi()));

      //fill dijet asymmetry histos
      if(dphi>(2./3.*TMath::Pi())) {
        if(fCentBin>-1 && fCentBin<fNcentBins) {
          fh2ZgPtRatio[fCentBin]->Fill(zg,pt/jet2->Pt());
        }
      }
      
    }
    
  }//jet loop
}

//----------------------------------------------------------
void anaZgHistosCrossChecks::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaZgHistosCrossChecks: fOutput not present");
    return;
  }
  
  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  fh2RhoCent = new TH2F("fh2RhoCent","fh2RhoCent;centrality;#rho",100,0,100,500,0,500);
  fOutput->Add(fh2RhoCent);

  TString histTitle = "";
  TString histName  = "";

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

    histName = Form("fh2PtPtGF_%d",i);
    histTitle = Form("%s;p_{T,jet};p_{T,G}/p_{T,jet}",histName.Data());
    fh2PtPtGF[i] = new TH2F(histName.Data(),histTitle.Data(),50,0.,500.,101,0.,1.01);
    fOutput->Add(fh2PtPtGF[i]);  
 
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
    
    histName = Form("fh2PtZg_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZg[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,50,0,0.5);
    fOutput->Add(fh2PtZg[i]);
    
    histName = Form("fh2PtThetag_%d",i);
    histTitle = Form("%s;p_{T};#theta_{g};",histName.Data());
    fh2PtThetag[i] = new TH2F(histName.Data(),histTitle.Data(),50,0,500,100,0,1.);
    fOutput->Add(fh2PtThetag[i]);

    histName = Form("fh2ZgPtRatio_%d",i);
    histTitle = Form("%s;z_{g};p_{T,2}/p_{T,1}",histName.Data());
    fh2ZgPtRatio[i] = new TH2F(histName.Data(),histTitle.Data(),50,0.,0.5,20,0.,1.);
    fOutput->Add(fh2ZgPtRatio[i]);
    
  }

}

//----------------------------------------------------------
double anaZgHistosCrossChecks::DeltaR(double phi1, double phi2, double eta1, double eta2) {
  //calculate separation distance in eta,phi plane
  Double_t dPhi = phi1 - phi2;
  Double_t dEta = eta1 - eta2;
  dPhi = TVector2::Phi_mpi_pi(dPhi);
  double dr2 = dPhi * dPhi + dEta * dEta;
  if(dr2>0.) return sqrt(dr2);
  else       return -1.;
}

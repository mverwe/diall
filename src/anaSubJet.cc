#include "UserCode/diall/interface/anaSubJet.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaSubJet)
   
anaSubJet::anaSubJet(const char *name, const char *title) 
:anaBaseTask(name,title),
  fNcentBins(4),
  fJetsName(""),
  fJetsCont(),
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
  fh2PtMass(),
  fh2PtSubjetPtRatio21(),
  fh2PtSubjetPtRatio32(),
  fh2PtSubjetPtRatio43(),
  fh2PtSubjetPtRatio54(),
  fh2PtSubjetPtFrac1(),
  fh2PtSubjetPtFrac2(),
  fh2PtSubjetPtFrac3(),
  fh2PtSubjetPtFrac4(),
  fh2PtZg(),
  fh2PtZgTrue(),
  fh2PtZgNoRef(),
  fh2SLPtSubjetPtRatio21(),
  fh2SLPtSubjetPtRatio32(),
  fh2SLPtSubjetPtRatio43(),
  fh2SLPtSubjetPtRatio54(),
  fh2SLPtZg(),
  fh2SLPtZgTrue(),
  fh2SLPtZgNoRef(),
  fh2SLPtDeltaPhi(),
  fh2SLPtDeltaPhiTrue(),
  fh2SLPtDeltaPhiNoRef(),
  fStoreTree(false),
  fTreeOut(0x0),
  fSubjetTreeVars()
{
  
  fh3PtEtaPhi          = new TH3F*[fNcentBins];
  fh2PtNSubjets        = new TH2F*[fNcentBins];
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

  for (int i = 0; i < fNcentBins; i++) {
    fh3PtEtaPhi[i]      = 0;
    fh2PtNSubjets[i]    = 0;
    fh2PtMass[i]        = 0;
    fh2PtSubjetPtRatio21[i] = 0;
    fh2PtSubjetPtRatio32[i] = 0;
    fh2PtSubjetPtRatio43[i] = 0;
    fh2PtSubjetPtRatio54[i] = 0;
    fh2PtSubjetPtFrac1[i]   = 0;
    fh2PtSubjetPtFrac2[i]   = 0;
    fh2PtSubjetPtFrac3[i]   = 0;
    fh2PtSubjetPtFrac4[i]   = 0;
    fh2PtZg[i]              = 0;
    fh2PtZgTrue[i]          = 0;
    fh2PtZgNoRef[i]         = 0;
  }

}

//----------------------------------------------------------
void anaSubJet::Exec(Option_t * /*option*/)
{
  //printf("anaSubJet executing\n");
  anaBaseTask::Exec();
  if(!SelectEvent()) return;
  
  if(!fInitOutput) CreateOutputObjects();

  if(!fJetsCont && !fJetsName.IsNull())
    fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
  if(!fJetsCont) return;

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

   for (int i = 0; i < fJetsCont->GetNJets(); i++) {
     lwJet *jet = fJetsCont->GetJet(i);
     Double_t pt = jet->Pt();
     Double_t phi = jet->Phi();
     Double_t eta = jet->Eta();
     Double_t m = jet->M();
     if(fabs(pt-0.)<1e-6) continue; //remove ghosts
     if(eta<fJetEtaMin || eta>fJetEtaMax) continue;
     if(fMinRefPt>-1. && jet->GetRefPt()<fMinRefPt) continue; //remove fakes in MC    
 
     if(fCentBin>-1 && fCentBin<fNcentBins) {
       fh3PtEtaPhi[fCentBin]->Fill(pt,eta,phi,weight);
       fh2PtMass[fCentBin]->Fill(pt,m);
     }
       
     std::vector<float> sjpt = jet->GetSubJetPt();
     int nsubjets = (int)sjpt.size();
     //Printf("nsubjets: %d",nsubjets);
     if(fCentBin>-1 && fCentBin<fNcentBins) fh2PtNSubjets[fCentBin]->Fill(pt,nsubjets,weight);

     if(nsubjets>1) {
       float ptrat = -1.;
       for(int is = 1; is<nsubjets; ++is) {
         if(sjpt.at(is-1)>0.) {
           //Printf("jetpt: %f sjpt1: %f sjpt2: %f",pt,sjpt.at(is-1),sjpt.at(is));
           ptrat = sjpt.at(is)/sjpt.at(is-1);
           if(fCentBin>-1 && fCentBin<fNcentBins) {
             if(is==1) fh2PtSubjetPtRatio21[fCentBin]->Fill(pt,ptrat);
             if(is==2) fh2PtSubjetPtRatio32[fCentBin]->Fill(pt,ptrat);
             if(is==3) fh2PtSubjetPtRatio43[fCentBin]->Fill(pt,ptrat);
             if(is==4) fh2PtSubjetPtRatio54[fCentBin]->Fill(pt,ptrat);
           }
         }
       }
       float zg = std::min(sjpt.at(0),sjpt.at(1))/(sjpt.at(0)+sjpt.at(1));
       if(fCentBin>-1 && fCentBin<fNcentBins) {
         fh2PtZg[fCentBin]->Fill(pt,zg);
         if(jet->GetRefPt()<0.) fh2PtZgNoRef[fCentBin]->Fill(pt,zg);
         else                   fh2PtZgTrue[fCentBin]->Fill(pt,zg);
       }
     }
     for(int is = 0; is<nsubjets; ++is) {
       float ptrat = sjpt.at(is)/pt;
       if(fCentBin>-1 && fCentBin<fNcentBins) {
         if(is==0) fh2PtSubjetPtFrac1[fCentBin]->Fill(pt,ptrat);
         if(is==1) fh2PtSubjetPtFrac2[fCentBin]->Fill(pt,ptrat);
         if(is==2) fh2PtSubjetPtFrac3[fCentBin]->Fill(pt,ptrat);
         if(is==3) fh2PtSubjetPtFrac4[fCentBin]->Fill(pt,ptrat);
       }
     }

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
           if(jet2->GetRefPt()<0.) {
             TH2F *hdphiNoRef = (fh2SLPtDeltaPhiNoRef.at(fCentBin)).at(l);
             hdphiNoRef->Fill(jet2->Pt(),dphi);
           } else {
             TH2F *hdphiTrue = (fh2SLPtDeltaPhiTrue.at(fCentBin)).at(l);
             hdphiTrue->Fill(jet2->Pt(),dphi);
           }
           
           if(dphi<fMinDPhi) continue;
           
           std::vector<float> sjptsl = jet2->GetSubJetPt();
           int nsubjets2 = (int)sjptsl.size();
           if(nsubjets2<2) continue;
           
           for(int is = 1; is<nsubjets2; ++is) {
             if(sjptsl.at(is-1)>0.) {
               float ptrat = sjptsl.at(is)/sjptsl.at(is-1);
               if(is==1) {
                 TH2F *h21 = (fh2SLPtSubjetPtRatio21.at(fCentBin)).at(l);
                 h21->Fill(jet2->Pt(),ptrat);
               }
               if(is==2) {
                 TH2F *h32 = (fh2SLPtSubjetPtRatio32.at(fCentBin)).at(l);
                 h32->Fill(jet2->Pt(),ptrat);
               }
               if(is==3) {
                 TH2F *h43 = (fh2SLPtSubjetPtRatio43.at(fCentBin)).at(l);
                 h43->Fill(jet2->Pt(),ptrat);
               }
               if(is==4) {
                 TH2F *h54 = (fh2SLPtSubjetPtRatio54.at(fCentBin)).at(l);
                 h54->Fill(jet2->Pt(),ptrat);
               }
             }        
           }
           float zgsl = std::min(sjptsl.at(0),sjptsl.at(1))/(sjptsl.at(0)+sjptsl.at(1));
           TH2F *hsjzg = (fh2SLPtZg.at(fCentBin)).at(l);
           hsjzg->Fill(jet2->Pt(),zgsl);
           if(jet2->GetRefPt()<0.) {
             TH2F *hsjzgNoRef = (fh2SLPtZgNoRef.at(fCentBin)).at(l);
             hsjzgNoRef->Fill(jet2->Pt(),zgsl);
           } else {
             TH2F *hsjzgTrue = (fh2SLPtZgTrue.at(fCentBin)).at(l);
             hsjzgTrue->Fill(jet2->Pt(),zgsl);
           }
         }
       }
     }
   }

}

//----------------------------------------------------------
void anaSubJet::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaSubJet: fOutput not present");
    return;
  }

  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  TString histTitle = "";
  TString histName  = "";
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = Form("fh2PtEtaPhi_%d",i);
    histTitle = Form("%s;pt;#eta;#phi;",histName.Data());
    fh3PtEtaPhi[i] = new TH3F(histName.Data(),histTitle.Data(),50,0,500,100,-5,5,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhi[i]);

    histName = Form("fh2PtNSubjets_%d",i);
    histTitle = Form("%s;pt;N_{subjets};",histName.Data());
    fh2PtNSubjets[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,10,0,10);
    fOutput->Add(fh2PtNSubjets[i]);

    histName = Form("fh2PtMass_%d",i);
    histTitle = Form("%s;p_{T};M;",histName.Data());
    fh2PtMass[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,50);
    fOutput->Add(fh2PtMass[i]);
    
    histName = Form("fh2PtSubjetPtRatio21_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
    fh2PtSubjetPtRatio21[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio21[i]);
    
    histName = Form("fh2PtSubjetPtRatio32_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ3}/p_{T,SJ2};",histName.Data());
    fh2PtSubjetPtRatio32[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio32[i]);

    histName = Form("fh2PtSubjetPtRatio43_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ4}/p_{T,SJ3};",histName.Data());
    fh2PtSubjetPtRatio43[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio43[i]);

    histName = Form("fh2PtSubjetPtRatio54_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ5}/p_{T,SJ4};",histName.Data());
    fh2PtSubjetPtRatio54[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtRatio54[i]);

    histName = Form("fh2PtSubjetPtFrac1_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ1}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac1[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac1[i]);

    histName = Form("fh2PtSubjetPtFrac2_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac2[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac2[i]);

    histName = Form("fh2PtSubjetPtFrac3_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ3}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac3[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac3[i]);

    histName = Form("fh2PtSubjetPtFrac4_%d",i);
    histTitle = Form("%s;pt;p_{T,SJ4}/p_{T,jet};",histName.Data());
    fh2PtSubjetPtFrac4[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
    fOutput->Add(fh2PtSubjetPtFrac4[i]);
    
    histName = Form("fh2PtZg_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZg[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
    fOutput->Add(fh2PtZg[i]);

    histName = Form("fh2PtZgTrue_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgTrue[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgTrue[i]);

    histName = Form("fh2PtZgNoRef_%d",i);
    histTitle = Form("%s;p_{T};z_{g};",histName.Data());
    fh2PtZgNoRef[i] = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
    fOutput->Add(fh2PtZgNoRef[i]);
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
    std::vector<TH2F *> h2LJZg;
    std::vector<TH2F *> h2LJZgTrue;
    std::vector<TH2F *> h2LJZgNoRef;
    std::vector<TH2F *> h2LJDeltaPhi;
    std::vector<TH2F *> h2LJDeltaPhiTrue;
    std::vector<TH2F *> h2LJDeltaPhiNoRef;
    
    for(int j = 0; j < nBinsLJ; j++) {
      histName = Form("fh2SLPtSubjetPtRatio21_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp21 = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJ21.push_back(htmp21);
      fOutput->Add(htmp21);

      histName = Form("fh2SLPtSubjetPtRatio32_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp32 = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJ32.push_back(htmp32);
      fOutput->Add(htmp32);

      histName = Form("fh2SLPtSubjetPtRatio43_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp43 = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJ43.push_back(htmp43);
      fOutput->Add(htmp43);

      histName = Form("fh2SLPtSubjetPtRatio54_%d_LJ%d",i,j);
      histTitle = Form("%s;pt;p_{T,SJ2}/p_{T,SJ1};",histName.Data());
      TH2F *htmp54 = new TH2F(histName.Data(),histTitle.Data(),500,0,500,100,0,1);
      h2LJ54.push_back(htmp54);
      fOutput->Add(htmp54);

      histName = Form("fh2SLPtZg_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzg = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
      h2LJZg.push_back(htmpzg);
      fOutput->Add(htmpzg);

      histName = Form("fh2SLPtZgTrue_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzgTrue = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
      h2LJZgTrue.push_back(htmpzgTrue);
      fOutput->Add(htmpzgTrue);

      histName = Form("fh2SLPtZgNoRef_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};z_{g};",histName.Data());
      TH2F *htmpzgNoRef = new TH2F(histName.Data(),histTitle.Data(),500,0,500,50,0,0.5);
      h2LJZgNoRef.push_back(htmpzgNoRef);
      fOutput->Add(htmpzgNoRef);

      histName = Form("fh2SLPtDeltaPhi_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhi = new TH2F(histName.Data(),histTitle.Data(),500,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhi.push_back(htmpDeltaPhi);
      fOutput->Add(htmpDeltaPhi);

      histName = Form("fh2SLPtDeltaPhiTrue_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhiTrue = new TH2F(histName.Data(),histTitle.Data(),500,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhiTrue.push_back(htmpDeltaPhiTrue);
      fOutput->Add(htmpDeltaPhiTrue);

      histName = Form("fh2SLPtDeltaPhiNoRef_%d_LJ%d",i,j);
      histTitle = Form("%s;p_{T};#Delta_{#phi};",histName.Data());
      TH2F *htmpDeltaPhiNoRef = new TH2F(histName.Data(),histTitle.Data(),500,0,500,36,0.,TMath::Pi());
      h2LJDeltaPhiNoRef.push_back(htmpDeltaPhiNoRef);
      fOutput->Add(htmpDeltaPhiNoRef);
    }
    fh2SLPtSubjetPtRatio21.push_back(h2LJ21);
    fh2SLPtSubjetPtRatio32.push_back(h2LJ32);
    fh2SLPtSubjetPtRatio43.push_back(h2LJ43);
    fh2SLPtSubjetPtRatio54.push_back(h2LJ54);
    fh2SLPtZg.push_back(h2LJZg);
    fh2SLPtZgTrue.push_back(h2LJZgTrue);
    fh2SLPtZgNoRef.push_back(h2LJZgNoRef);
    fh2SLPtDeltaPhi.push_back(h2LJDeltaPhi);
    fh2SLPtDeltaPhiTrue.push_back(h2LJDeltaPhiTrue);
    fh2SLPtDeltaPhiNoRef.push_back(h2LJDeltaPhiNoRef);
  }
}

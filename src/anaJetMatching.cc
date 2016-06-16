#include "UserCode/diall/interface/anaJetMatching.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetMatching)
   
anaJetMatching::anaJetMatching(const char *name, const char *title) 
:anaBaseTask(name,title),
  fJetsNameBase(""),
  fJetsContBase(),
  fJetsNameTag(""),
  fJetsContTag(),
  fNcentBins(4),
  fMatchingType(0),
  fMatchId(1),
  fh2PtJet1VsPtJet2(0),
  fh2PtJet2VsRelPt(0),
  fh2PtJet1VsDeltaR(0)
{

  fh2PtJet1VsDeltaR            = new TH2F*[fNcentBins];
  fh2PtJet1VsPtJet2            = new TH2F*[fNcentBins];
  fh2PtJet2VsRelPt             = new TH2F*[fNcentBins];
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    fh2PtJet1VsDeltaR[i]           = 0;
    fh2PtJet1VsPtJet2[i]           = 0;
    fh2PtJet2VsRelPt[i]            = 0;
  }
  
}

//----------------------------------------------------------
void anaJetMatching::Exec(Option_t * /*option*/)
{

  anaBaseTask::Exec();

  // printf("anaJetMatching executing\n");
  if(!fInitOutput) CreateOutputObjects();

  //Get objects from event
  if(!fJetsContBase && !fJetsNameBase.IsNull())
    fJetsContBase = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsNameBase.Data()));
  if(!fJetsContBase) { Printf("No fJetsContBase"); return; }
  
  if(!fJetsContTag && !fJetsNameTag.IsNull())
    fJetsContTag = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsNameTag.Data()));
  if(!fJetsContTag) { Printf("No fJetsContTag"); return; }

  // Printf("start matching");
   MatchJetsGeo();

}

//----------------------------------------------------------
void anaJetMatching::MatchJetsGeo() {

  //Bijective matching between two jet collections
  
  if(!fJetsContBase || !fJetsContTag) return;

  //Determine centrality bin
  Double_t cent = 0.;
  if(fHiEvent) cent = fHiEvent->GetCentrality();
  Int_t fCentBin = 0;
  if(fNcentBins==4) {
    if(cent>=0. && cent<10.)       fCentBin = 0;
    else if(cent>=10. && cent<30.) fCentBin = 1;
    else if(cent>=30. && cent<50.) fCentBin = 2;
    else if(cent>=50. && cent<80.) fCentBin = 3;
    else fCentBin = -1;
  } 

  const Int_t nJets1 = fJetsContBase->GetNJets();
  const Int_t nJets2 = fJetsContTag->GetNJets();
  if(nJets1==0 || nJets2==0) {
     //Printf("nJets1: %d  nJets2: %d",nJets1,nJets2);	  
     return;
  }

  for (int i = 0; i < nJets1; i++) {
    lwJet *jet1 = fJetsContBase->GetJet(i);
    SetMatchIdForJet(jet1,-1);
//    jet1->SetMatchId1(-1);
  }
  for (int i = 0; i < nJets2; i++) {
    lwJet *jet2 = fJetsContTag->GetJet(i);
    SetMatchIdForJet(jet2,-1);
//    jet2->SetMatchId1(-1);
  }

  TArrayI faMatchIndex1;
  faMatchIndex1.Set(nJets2+1);
  faMatchIndex1.Reset(-1);
  
  TArrayI faMatchIndex2;
  faMatchIndex2.Set(nJets1+1);
  faMatchIndex2.Reset(-1);
  
  static TArrayS iFlag(nJets1*nJets2);
  if(iFlag.GetSize()<(nJets1*nJets2))
    iFlag.Set(nJets1*nJets2+1);
  iFlag.Reset(0);

  Double_t maxDist = 0.4; //limit CPU time
  if(fMatchingType==1) maxDist = 0.3;
  if(fMatchingType==2) maxDist = 0.2;
  
  //Find the clostest tag jet to base jet
  Double_t dist = maxDist;
  for (int i = 0; i < nJets1; i++) {
    lwJet *jet1 = fJetsContBase->GetJet(i);
    if(!jet1) continue;
    if(fabs(jet1->Pt())<1e-6) continue; //remove ghosts
    dist = maxDist;
    for (int j = 0; j < nJets2; j++) {
      lwJet *jet2 = fJetsContTag->GetJet(j);
      if(!jet2) continue;
      if(fabs(jet2->Pt())<1e-6) continue; //remove ghosts

      Double_t dR = jet1->DeltaR(jet2);
      if(fMatchingType==2) dR = dR/jet2->Pt();
      if(dR<dist && dR<maxDist){
	faMatchIndex2[i]=j;
	dist = dR;
      }
    }//jet2 loop
    if(faMatchIndex2[i]>=0)
      iFlag[i*nJets2+faMatchIndex2[i]]+=1;//j closest to i
  }//jet1 loop
  

  if(fMatchingType==0 || fMatchingType==2) {
    //other way around
    dist = maxDist;
    for (int j = 0; j < nJets2; j++) {
      lwJet *jet2 = fJetsContTag->GetJet(j);
      if(!jet2) continue;
      if(fabs(jet2->Pt())<1e-6) continue; //remove ghosts
      dist = maxDist;
      for (int i = 0; i < nJets1; i++) {
        lwJet *jet1 = fJetsContBase->GetJet(i);
        if(!jet1) continue;
        if(fabs(jet1->Pt())<1e-6) continue; //remove ghosts

        Double_t dR = jet1->DeltaR(jet2);
        if(fMatchingType==2) dR = dR/jet2->Pt();
        if(dR<dist && dR<maxDist){
          faMatchIndex1[j]=i;
          dist = dR;
        }
      }
      if(faMatchIndex1[j]>=0)
        iFlag[faMatchIndex1[j]*nJets2+j]+=2;//i closest to j
    }

    // check for "true" correlations
    for(int i = 0;i<nJets1;i++) {
      lwJet *jet1 = fJetsContBase->GetJet(i);
      if(!jet1) continue;
      if(fabs(jet1->Pt())<1e-6) continue; //remove ghosts
      //bool match = false;
      for (int j = 0; j < nJets2; j++) {
        lwJet *jet2 = fJetsContTag->GetJet(j);
        if(!jet2) continue;
        if(fabs(jet2->Pt())<1e-6) continue; //remove ghosts

        // we have a unique correlation
        if(iFlag[i*nJets2+j]==3) {
          SetMatchIdForJet(jet1,j);
	  SetMatchIdForJet(jet2,i);
          //jet1->SetMatchId1(j);
          //jet2->SetMatchId1(i);
          //Printf("%f matched to %f",jet1->Pt(),jet2->Pt());
          //Fill histograms
          if(fCentBin>-1 && fCentBin<fNcentBins) {
            Double_t dR = jet1->DeltaR(jet2);
            fh2PtJet1VsDeltaR[fCentBin]->Fill(jet1->Pt(),dR);
            fh2PtJet1VsPtJet2[fCentBin]->Fill(jet1->Pt(),jet2->Pt());
            if(jet2->Pt()>0.) fh2PtJet2VsRelPt[fCentBin]->Fill(jet2->Pt(),(jet1->Pt()-jet2->Pt())/jet2->Pt());
          }
        //match = true;
        }
      }
    //if(!match) Printf("%f not matched",jet1->Pt());
    }
  }
  else if(fMatchingType==1) {
    for(int i = 0;i<nJets1;i++) {
      lwJet *jet1 = fJetsContBase->GetJet(i);
      if(!jet1) continue;
      if(fabs(jet1->Pt())<1e-6) continue; //remove ghosts

      int j = faMatchIndex2[i];
      if(j>-1) {
        lwJet *jet2 = fJetsContTag->GetJet(j);
        if(!jet2) continue;
        if(fabs(jet2->Pt())<1e-6) continue; //remove ghosts
       
        SetMatchIdForJet(jet1,j);
	SetMatchIdForJet(jet2,i);	 
        //jet1->SetMatchId1(j);
        //jet2->SetMatchId1(i);
        
        //Fill histograms
        if(fCentBin>-1 && fCentBin<fNcentBins) {
          Double_t dR = jet1->DeltaR(jet2);
          fh2PtJet1VsDeltaR[fCentBin]->Fill(jet1->Pt(),dR);
          fh2PtJet1VsPtJet2[fCentBin]->Fill(jet1->Pt(),jet2->Pt());
          if(jet2->Pt()>0.) fh2PtJet2VsRelPt[fCentBin]->Fill(jet2->Pt(),(jet1->Pt()-jet2->Pt())/jet2->Pt());
        }
      }
    }
  }
}

//----------------------------------------------------------
void anaJetMatching::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaJetMatching: fOutput not present");
    return;
  }

  const Int_t nBinsPt          = 40;
  const Int_t nBinsDR          = 50;

  const Double_t minPt       =   0.;
  const Double_t maxPt       = 200.;
  const Double_t minDR       =  0.;
  const Double_t maxDR       =  0.5;
  
  TString histName = "";
  TString histTitle = "";
  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = TString::Format("fh2PtJet1VsDeltaR_%d",i);
    histTitle = TString::Format("%s;#it{p}_{T,jet1};#it{#Delta R}",histName.Data());
    fh2PtJet1VsDeltaR[i] = new TH2F(histName.Data(),histTitle.Data(),nBinsPt,minPt,maxPt,nBinsDR,minDR,maxDR);
    fOutput->Add(fh2PtJet1VsDeltaR[i]);

    histName = TString::Format("fh2PtJet1VsPtJet2_%d",i);
    histTitle = TString::Format("%s;#it{p}_{T,jet1};#it{p}_{T,jet2}",histName.Data());
    fh2PtJet1VsPtJet2[i] = new TH2F(histName.Data(),histTitle.Data(),nBinsPt,minPt,maxPt,nBinsPt,minPt,maxPt);
    fOutput->Add(fh2PtJet1VsPtJet2[i]);

    histName = TString::Format("fh2PtJet2VsRelPt_%d",i);
    histTitle = TString::Format("%s;#it{p}_{T,jet2};(#it{p}_{T,jet2}-#it{p}_{T,jet1})/#it{p}_{T,jet1}",histName.Data());
    fh2PtJet2VsRelPt[i] = new TH2F(histName.Data(),histTitle.Data(),nBinsPt,minPt,maxPt,241,-2.41,2.41);
    fOutput->Add(fh2PtJet2VsRelPt[i]);
  }

}

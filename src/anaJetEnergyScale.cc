#include "UserCode/diall/interface/anaJetEnergyScale.h"

#include "UserCode/diall/interface/lwJet.h"

ClassImp(anaJetEnergyScale)
   
anaJetEnergyScale::anaJetEnergyScale(const char *name, const char *title) 
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
  fMinJetPtRaw(0.),
  fUseOnlyLeadingJet(false),
  fMaxMatchDistLeadJet(0.4),
  fhEventSel(0),
  fhCentrality(0),
  fhNPV(0),
  fh2PtEtaNoMatching(0),
  fh3PtEtaPhiNotMatched(0),
  fh3PtEtaPhiMatched(0),
  fh3PtEtaPhiMatched2(0),
  fh3PtTrueNPVDeltaPt(0),
  fh3PtTrueNPVDeltaPtRel(0),
  fh3PtTrueNPVScalePt(0),
  fh3PtTruePtSubNPV(0),
  fh3PtTrueEtaDeltaPt(0),
  fh3PtTrueEtaDeltaPtRel(0),
  fh3PtTrueEtaScalePt(0),
  fh3PtTrueEtaFineScalePt(0),
  fh3PtRawEtaFineScalePt(0),
  fh3PtRecoEtaFineScalePt(0),
  fh3PtTruePtSubEta(0),
  fh3PtTrueEtaDeltaR(0),
  fh3PtTrueEtaDeltaM(0),
  fh3PtTrueEtaDeltaMRel(0),
  fh3PtTrueEtaScaleM(0),
  fh3PtTrueMTrueScaleM(0)
{

  fh2PtEtaNoMatching           = new TH2F*[fNcentBins];
  fh3PtEtaPhiNotMatched        = new TH3F*[fNcentBins];
  fh3PtEtaPhiMatched           = new TH3F*[fNcentBins];
  fh3PtEtaPhiMatched2           = new TH3F*[fNcentBins];
  fh3PtTrueNPVDeltaPt          = new TH3F*[fNcentBins];
  fh3PtTrueNPVDeltaPtRel       = new TH3F*[fNcentBins];
  fh3PtTrueNPVScalePt          = new TH3F*[fNcentBins];
  fh3PtTruePtSubNPV            = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaPt          = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaPtRel       = new TH3F*[fNcentBins];
  fh3PtTrueEtaScalePt          = new TH3F*[fNcentBins];
  fh3PtTrueEtaFineScalePt      = new TH3F*[fNcentBins];
  fh3PtRawEtaFineScalePt      = new TH3F*[fNcentBins];
  fh3PtRecoEtaFineScalePt      = new TH3F*[fNcentBins];
  fh3PtTruePtSubEta            = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaR           = new TH3F*[fNcentBins];

  fh3PtTrueEtaDeltaM           = new TH3F*[fNcentBins];
  fh3PtTrueEtaDeltaMRel        = new TH3F*[fNcentBins];
  fh3PtTrueEtaScaleM           = new TH3F*[fNcentBins];
  fh3PtTrueMTrueScaleM         = new TH3F*[fNcentBins];
  
  for (Int_t i = 0; i < fNcentBins; i++) {
    fh2PtEtaNoMatching[i]           = 0;
    fh3PtEtaPhiNotMatched[i]        = 0;
    fh3PtEtaPhiMatched[i]           = 0;
    fh3PtEtaPhiMatched2[i]          = 0;
    fh3PtTrueNPVDeltaPt[i]          = 0;
    fh3PtTrueNPVDeltaPtRel[i]       = 0;
    fh3PtTrueNPVScalePt[i]          = 0;
    fh3PtTruePtSubNPV[i]            = 0;
    fh3PtTrueEtaDeltaPt[i]          = 0;
    fh3PtTrueEtaDeltaPtRel[i]       = 0;
    fh3PtTrueEtaScalePt[i]          = 0;
    fh3PtTrueEtaFineScalePt[i]      = 0;
    fh3PtRawEtaFineScalePt[i]       = 0;
    fh3PtRecoEtaFineScalePt[i]      = 0;
    fh3PtTruePtSubEta[i]            = 0;
    fh3PtTrueEtaDeltaR[i]           = 0;

    fh3PtTrueEtaDeltaM[i]           = 0;
    fh3PtTrueEtaDeltaMRel[i]        = 0;
    fh3PtTrueEtaScaleM[i]           = 0;
    fh3PtTrueMTrueScaleM[i]         = 0;
  }

}

//----------------------------------------------------------
void anaJetEnergyScale::Exec(Option_t * /*option*/)
{
 
  anaBaseTask::Exec();
  if(!SelectEvent()) return;

  //printf("anaJetEnergyScale executing\n");
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

   //number of generated pile-up vertices
   int npv = 1;
   if(fHiEvent) npv = fHiEvent->GetNPV();
   fhNPV->Fill(npv);

   fhEventSel->Fill(0.5);

   bool debugMatching = false;
   bool printDebug = false;
   //if(fJetsGenCont->GetJet(0)->Pt()>80.) printDebug = true;
   
   if(fUseForestMatching) {
     int nRecJets = fJetsRecCont->GetNJets();
     if(fUseOnlyLeadingJet) nRecJets = 1;
       
     for(Int_t ij = 0; ij<nRecJets; ij++) {
       lwJet *jet = fJetsRecCont->GetJet(ij);
       if(!jet) continue;

       if(!fUseOnlyLeadingJet) {
         //reject unmatched or badly matched jets
         if(jet->GetRefDr()>fMaxDist
            || jet->GetSubEvent()!=0
            || jet->GetRefPt()<1.) //changed for pPb
           //         || jet->GetRefPt()<10.)//e-3)
           continue;
       }
       
       //select jets originating from certain parton flavor
       if(fRefPartonFlavorMin>-1 && fRefPartonFlavorMax>-1) {
         if(std::abs(jet->GetRefParton())<std::abs(fRefPartonFlavorMin) || std::abs(jet->GetRefParton())>std::abs(fRefPartonFlavorMax)) continue;
       }

       double pt  = jet->Pt();
       if(fUseRawPt) pt = jet->GetRawPt();       
       double dpt = pt-jet->GetRefPt();
       double dm  = jet->M()-jet->GetRefM();
       double refpt = jet->GetRefPt();

       if(fUseOnlyLeadingJet) {
         for(Int_t ijgen = 0; ijgen<1; ++ijgen) { //fJetsGenCont->GetNJets()
           lwJet *jetgen = fJetsGenCont->GetJet(ijgen);
           if(!jetgen) continue;
           double dR = jet->DeltaR(jetgen);
           if(dR>fMaxMatchDistLeadJet) continue;
           refpt = jetgen->Pt();
         }
       }
       
       if(fCentBin>-1 && fCentBin<fNcentBins)
         fh3PtTruePtSubEta[fCentBin]->Fill(jet->GetRefPt(),pt,jet->GetRefEta(),weight);
       
       if(pt<fMinJetPtRec) continue;
       if(jet->GetRawPt()<fMinJetPtRaw) continue;

       if(fCentBin>-1 && fCentBin<fNcentBins)
         fh3PtEtaPhiMatched[fCentBin]->Fill(jet->GetRefPt(),jet->GetRefEta(),jet->GetRefPhi(),weight);

       if(fCentBin>-1 && fCentBin<fNcentBins) {
         fh3PtTrueEtaDeltaPt[fCentBin]->Fill(refpt,jet->GetRefEta(),dpt,weight);
         fh3PtTrueEtaDeltaM[fCentBin]->Fill(refpt,jet->GetRefEta(),dm,weight);
         if(refpt>0.) {
           fh3PtTrueEtaDeltaPtRel[fCentBin]->Fill(refpt,jet->GetRefEta(),dpt/refpt,weight);
           fh3PtTrueEtaScalePt[fCentBin]->Fill(refpt,jet->GetRefEta(),pt/refpt,weight);
           //fh3PtTrueEtaFineScalePt[fCentBin]->Fill(refpt,jet->GetRefEta(),pt/refpt,weight);
           fh3PtTrueEtaFineScalePt[fCentBin]->Fill(refpt,jet->Eta(),pt/refpt,weight);
           fh3PtRawEtaFineScalePt[fCentBin]->Fill(jet->GetRawPt(),jet->Eta(),pt/refpt,weight);
           fh3PtRecoEtaFineScalePt[fCentBin]->Fill(jet->Pt(),jet->Eta(),jet->Pt()/refpt,weight);
           fh3PtTrueEtaDeltaR[fCentBin]->Fill(refpt,jet->GetRefEta(),jet->GetRefDr(),weight);
         }
         if(jet->GetRefM()>0.) {
           fh3PtTrueEtaDeltaMRel[fCentBin]->Fill(refpt,jet->GetRefEta(),dm/jet->GetRefM(),weight);
           fh3PtTrueEtaScaleM[fCentBin]->Fill(refpt,jet->GetRefEta(),jet->M()/jet->GetRefM(),weight);
         }
     
         if(abs(jet->GetRefEta())<1.3) {
         
           fh3PtTrueNPVDeltaPt[fCentBin]->Fill(refpt,npv,dpt,weight);
           if(refpt>0.) {
             fh3PtTrueNPVDeltaPtRel[fCentBin]->Fill(refpt,npv,dpt/refpt,weight);
             fh3PtTrueNPVScalePt[fCentBin]->Fill(refpt,npv,pt/refpt,weight);
           }
           fh3PtTruePtSubNPV[fCentBin]->Fill(refpt,pt,npv,weight);

           if(jet->GetRefM()>0.)
             fh3PtTrueMTrueScaleM[fCentBin]->Fill(refpt,jet->GetRefM(),jet->M()/jet->GetRefM(),weight);
         }//eta selection
       }
     }

     for(Int_t ij = 0; ij<fJetsGenCont->GetNJets(); ij++) {
       lwJet *jet = fJetsGenCont->GetJet(ij);
       if(!jet) continue;
       if(fCentBin>-1 && fCentBin<fNcentBins) {
         fh2PtEtaNoMatching[fCentBin]->Fill(jet->Pt(),jet->Eta(),weight);
         if( jet->GetMatchId1()<0 || jet->GetMatchId1()>fJetsRecCont->GetNJets() )
           fh3PtEtaPhiNotMatched[fCentBin]->Fill(jet->Pt(),jet->Eta(),jet->Phi(),weight);
         else
           fh3PtEtaPhiMatched2[fCentBin]->Fill(jet->Pt(),jet->Eta(),jet->Phi(),weight);
       }
     }
   } else { //use matching from anaJetMatching
     const Int_t nJets1 = fJetsGenCont->GetNJets();
     if(nJets1==0) return;
     
     for(Int_t ij = 0; ij<fJetsGenCont->GetNJets(); ij++) {
       lwJet *jet1 = fJetsGenCont->GetJet(ij);
       if(!jet1) continue;

       if(fCentBin>-1 && fCentBin<fNcentBins) 
         fh2PtEtaNoMatching[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),weight);
          
       int id = jet1->GetMatchId1();
       lwJet *jet2 = fJetsRecCont->GetJet(id);
       bool matched = true;
       if(!jet2) matched = false;
       double dR = 999.;
       if(jet2) {
         dR = jet1->DeltaR(jet2);
         if(dR>fMaxDist) { 
            //Printf("dR: %f fMaxDist: %f id: %d",dR,fMaxDist,id);
            matched = false;
         }
       }
       if(!matched) {
         if(fCentBin>-1 && fCentBin<fNcentBins)
           fh3PtEtaPhiNotMatched[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);
         continue;
       }

       if(fCentBin>-1 && fCentBin<fNcentBins)
         fh3PtEtaPhiMatched[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet1->Phi(),weight);

       double ptrec = jet2->Pt();
       if(fRhoMap) ptrec = jet2->Pt() - fRhoMap->GetValue(jet2->Eta())*jet2->GetArea();
       if(fUseRawPt) ptrec = jet2->GetRawPt();
       
       double dpt = ptrec-jet1->Pt();
       double dm  = jet2->M() - jet1->M();
       
       if(fCentBin>-1 && fCentBin<fNcentBins) {
         fh3PtTrueEtaDeltaPt[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dpt,weight);
         fh3PtTrueEtaDeltaM[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dm,weight);
         if(jet1->Pt()>0.) {
           fh3PtTrueEtaDeltaPtRel[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dpt/jet1->Pt(),weight);
           fh3PtTrueEtaScalePt[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),ptrec/jet1->Pt(),weight);
           //fh3PtTrueEtaFineScalePt[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),ptrec/jet1->Pt(),weight);
           fh3PtTrueEtaFineScalePt[fCentBin]->Fill(jet1->Pt(),jet2->Eta(),ptrec/jet1->Pt(),weight);
           fh3PtRawEtaFineScalePt[fCentBin]->Fill(jet2->GetRawPt(),jet2->Eta(),ptrec/jet1->Pt(),weight);
           fh3PtRecoEtaFineScalePt[fCentBin]->Fill(jet2->Pt(),jet2->Eta(),jet2->Pt()/jet1->Pt(),weight);
         }
         if(jet1->M()>0.) {
           fh3PtTrueEtaDeltaMRel[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),dm/jet1->M(),weight);
           fh3PtTrueEtaScaleM[fCentBin]->Fill(jet1->Pt(),jet1->Eta(),jet2->M()/jet1->M(),weight);
         }

         fh3PtTruePtSubEta[fCentBin]->Fill(jet1->Pt(),ptrec,jet1->Eta(),weight);
     
         if(abs(jet1->Eta())<1.3) {
         
           fh3PtTrueNPVDeltaPt[fCentBin]->Fill(jet1->Pt(),npv,dpt,weight);
           if(jet1->Pt()>0.) {
             fh3PtTrueNPVDeltaPtRel[fCentBin]->Fill(jet1->Pt(),npv,dpt/jet1->Pt(),weight);
             fh3PtTrueNPVScalePt[fCentBin]->Fill(jet1->Pt(),npv,ptrec/jet1->Pt(),weight);
           }
           fh3PtTruePtSubNPV[fCentBin]->Fill(jet1->Pt(),ptrec,npv,weight);

           //mass
           if(jet1->M()>0.)
             fh3PtTrueMTrueScaleM[fCentBin]->Fill(jet1->Pt(),jet1->M(),jet2->M()/jet1->M(),weight);
         }//eta selection
       }

       //********* debug matching difference ******************
       if(debugMatching && printDebug) {
         if(fCentBin==0) {
           Printf("reco jet pt: %f eta: %f phi: %f",jet2->Pt(),jet2->Eta(),jet2->Phi());
           Printf("match MV pt: %f eta: %f phi: %f dr: %f",jet1->Pt(),jet1->Eta(),jet1->Phi(),dR);
           Printf("match forest pt: %f eta: %f phi: %f dr: %f",jet2->GetRefPt(),jet2->GetRefEta(),jet2->GetRefPhi(),jet2->GetRefDr());
           Printf("\n");
         }
       }
       //******************************************************
       
     }//jet loop
/*
      if(debugMatching && printDebug && fCentBin==0) {
        //loop over gen jets
        //Printf("gen jets");
        for(Int_t ij = 0; ij<fJetsGenCont->GetNJets(); ij++) {
            lwJet *jet = fJetsGenCont->GetJet(ij);
            if(!jet) continue;
            //double matchpt = -999;
            int id = jet->GetMatchId1();
            //lwJet *jet2 = fJetsRecCont->GetJet(id);
            //if(jet2) matchpt = jet2->Pt();
            //Printf("pt: %f eta: %f phi: %f matchId: %d mvpt: %f refpt: %f",jet->Pt(),jet->Eta(),jet->Phi(),jet->GetMatchId1(),matchpt,jet->GetRefPt());
            
        }
        //Printf("\n reco jets");
        for(Int_t ij = 0; ij<fJetsRecCont->GetNJets(); ij++) {
          lwJet *jet = fJetsRecCont->GetJet(ij);
          if(!jet) continue;
          //Printf("pt: %f eta: %f phi: %f matchId: %d refpt: %f refdr: %f",jet->Pt(),jet->Eta(),jet->Phi(),jet->GetMatchId1(),jet->GetRefPt(),jet->GetRefDr());
        }
      }
*/
   }
}
//----------------------------------------------------------
void anaJetEnergyScale::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaJetEnergyScale: fOutput not present");
    return;
  }

  TString histTitle = "";
  TString histName  = "";

  const Int_t nBinsPtDet  = 200;
  const Double_t minPtDet =  0.;
  const Double_t maxPtDet = 400.;
  double *binsPtDet = new double[nBinsPtDet+1];
  for(int i=0; i<=nBinsPtDet; ++i) binsPtDet[i]=(double)minPtDet + (maxPtDet-minPtDet)/nBinsPtDet*(double)i ;

  const Int_t nBinsPtPart  = 200;
  const Double_t minPtPart = 0.;
  const Double_t maxPtPart = 400.;
  double *binsPtPart = new double[nBinsPtPart+1];
  for(int i=0; i<=nBinsPtPart; ++i) binsPtPart[i]=(double)minPtPart + (maxPtPart-minPtPart)/nBinsPtPart*(double)i ;

  const Int_t nBinsDPt  = 200;
  const Double_t minDPt = -100.;
  const Double_t maxDPt = 100.;
  double *binsDPt = new double[nBinsDPt+1];
  for(int i=0; i<=nBinsDPt; ++i) binsDPt[i]=(double)minDPt + (maxDPt-minDPt)/nBinsDPt*(double)i ;

  const Int_t nBinsDPtRel  = 600;
  const Double_t minDPtRel = -3.;
  const Double_t maxDPtRel = 3.;
  double *binsDPtRel = new double[nBinsDPtRel+1];
  for(int i=0; i<=nBinsDPtRel; ++i) binsDPtRel[i]=(double)minDPtRel + (maxDPtRel-minDPtRel)/nBinsDPtRel*(double)i ;

  const Int_t nBinsScalePt  = 300;
  const Double_t minScalePt = 0.;
  const Double_t maxScalePt = 3.;
  double *binsScalePt = new double[nBinsScalePt+1];
  for(int i=0; i<=nBinsScalePt; ++i) binsScalePt[i]=(double)minScalePt + (maxScalePt-minScalePt)/nBinsScalePt*(double)i ;

  const Int_t nBinsDeltaR  = 300;
  const Double_t minDeltaR = 0.;
  const Double_t maxDeltaR = 3.;
  double *binsDeltaR = new double[nBinsDeltaR+1];
  for(int i=0; i<=nBinsDeltaR; ++i) binsDeltaR[i]=(double)minDeltaR + (maxDeltaR-minDeltaR)/nBinsDeltaR*(double)i ;

  //
  const Int_t nBinsMPart  = 100;
  const Double_t minMPart =   0.;
  const Double_t maxMPart = 100.;
  double *binsMPart = new double[nBinsMPart+1];
  for(int i=0; i<=nBinsMPart; ++i) binsMPart[i]=(double)minMPart + (maxMPart-minMPart)/nBinsMPart*(double)i ;
  
  const Int_t nBinsDM  = 100;
  const Double_t minDM = -25.;
  const Double_t maxDM = 25.;
  double *binsDM = new double[nBinsDM+1];
  for(int i=0; i<=nBinsDM; ++i) binsDM[i]=(double)minDM + (maxDM-minDM)/nBinsDM*(double)i ;

  const Int_t nBinsDMRel  = 600;
  const Double_t minDMRel = -3.;
  const Double_t maxDMRel = 3.;
  double *binsDMRel = new double[nBinsDMRel+1];
  for(int i=0; i<=nBinsDMRel; ++i) binsDMRel[i]=(double)minDMRel + (maxDMRel-minDMRel)/nBinsDMRel*(double)i ;

  const Int_t nBinsScaleM  = 300;
  const Double_t minScaleM = 0.;
  const Double_t maxScaleM = 6.;
  double *binsScaleM = new double[nBinsScaleM+1];
  for(int i=0; i<=nBinsScaleM; ++i) binsScaleM[i]=(double)minScaleM + (maxScaleM-minScaleM)/nBinsScaleM*(double)i ;

  const Int_t nBinsNPV = 100;
  const Double_t minNPV = 0.;
  const Double_t maxNPV = 100.;
  double *binsNPV = new double[nBinsNPV+1];
  for(int i=0; i<=nBinsNPV; ++i) binsNPV[i]=(double)minNPV + (maxNPV-minNPV)/nBinsNPV*(double)i ;

  const Int_t nBinsPhi = 72;
  const Double_t minPhi = -TMath::Pi();
  const Double_t maxPhi = TMath::Pi();
  double *binsPhi = new double[nBinsPhi+1];
  for(int i=0; i<=nBinsPhi; ++i) binsPhi[i]=(double)minPhi + (maxPhi-minPhi)/nBinsPhi*(double)i ;
  
  const int nBinsEta = 30;
  double etaEdge[nBinsEta+1] = {-5.191, -3.139, -2.964, -2.853, -2.650, -2.500, -2.322, -2.172, -1.930, -1.653, -1.305, -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783, 1.044, 1.305, 1.653, 1.930, 2.172, 2.322, 2.500, 2.650, 2.853, 2.964, 3.139, 5.191};
  double *binsEta = new double[nBinsEta];
  for(int i = 0; i<=nBinsEta; ++i)
    binsEta[i] = etaEdge[i];

  const int nBinsEtaFine = 82;
  double etaEdgeFine[nBinsEtaFine+1] = {
    -5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013,
    -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853,
    -2.650, -2.500, -2.322, -2.172, -2.043, -1.930, -1.830,
    -1.740, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218,
    -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609,
    -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
    0.000,
    0.087,  0.174,  0.261,  0.348,  0.435,  0.522,  0.609,
    0.696,  0.783,  0.879,  0.957,  1.044,  1.131,  1.218,
    1.305,  1.392,  1.479,  1.566,  1.653,  1.740,  1.830,
    1.930,  2.043,  2.172,  2.322,  2.500,  2.650,  2.853,
    2.964,  3.139,  3.314,  3.489,  3.664,  3.839,  4.013,
    4.191,  4.363,  4.538,  4.716,  4.889,  5.191
  };
  double *binsEtaFine = new double[nBinsEtaFine];
  for(int i = 0; i<=nBinsEtaFine; ++i)
    binsEtaFine[i] = etaEdgeFine[i];

  fhEventSel = new TH1F("fhEventSel","fhEventSel",10,0,10);
  fOutput->Add(fhEventSel);

  fhCentrality = new TH1F("fhCentrality","fhCentrality",100,0,100);
  fOutput->Add(fhCentrality);

  fhNPV = new TH1F("fhNPV","fhNPV",nBinsNPV,minNPV,maxNPV);
  fOutput->Add(fhNPV);

  for (Int_t i = 0; i < fNcentBins; i++) {
    histName = Form("fh2PtEtaNoMatching_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh2PtEtaNoMatching[i] = new TH2F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsEta,binsEta);
    fOutput->Add(fh2PtEtaNoMatching[i]);

    histName = Form("fh3PtEtaPhiNotMatched_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh3PtEtaPhiNotMatched[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
    fOutput->Add(fh3PtEtaPhiNotMatched[i]);

    histName = Form("fh3PtEtaPhiMatched_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh3PtEtaPhiMatched[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
    fOutput->Add(fh3PtEtaPhiMatched[i]);
 
    histName = Form("fh3PtEtaPhiMatched2_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;",histName.Data());
    fh3PtEtaPhiMatched2[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsPhi,binsPhi);
    fOutput->Add(fh3PtEtaPhiMatched2[i]);
 
    histName = Form("fh3PtTrueNPVDeltaPt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;#it{p}_{T,rec}-#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVDeltaPt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPt,binsDPt);
    fOutput->Add(fh3PtTrueNPVDeltaPt[i]);

    histName = Form("fh3PtTrueNPVDeltaPtRel_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;(#it{p}_{T,rec}-#it{p}_{T,gen})/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVDeltaPtRel[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsDPtRel,binsDPtRel);
    fOutput->Add(fh3PtTrueNPVDeltaPtRel[i]);

    histName = Form("fh3PtTrueNPVScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};NPV;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueNPVScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsNPV,binsNPV,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueNPVScalePt[i]);

    histName = Form("fh3PtTruePtSubNPV_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#it{p}_{T,rec};NPV",histName.Data());
    fh3PtTruePtSubNPV[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsNPV,binsNPV);
    fOutput->Add(fh3PtTruePtSubNPV[i]);

    histName = Form("fh3PtTrueEtaDeltaPt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}-#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaDeltaPt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPt,binsDPt);
    fOutput->Add(fh3PtTrueEtaDeltaPt[i]);

    histName = Form("fh3PtTrueEtaDeltaPtRel_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;(#it{p}_{T,rec}-#it{p}_{T,gen})/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaDeltaPtRel[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDPtRel,binsDPtRel);
    fOutput->Add(fh3PtTrueEtaDeltaPtRel[i]);

    histName = Form("fh3PtTrueEtaScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueEtaScalePt[i]);

    //new
    histName = Form("fh3PtTrueEtaFineScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtTrueEtaFineScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEtaFine,binsEtaFine,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtTrueEtaFineScalePt[i]);

    histName = Form("fh3PtRawEtaFineScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,raw};#eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtRawEtaFineScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEtaFine,binsEtaFine,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtRawEtaFineScalePt[i]);

    histName = Form("fh3PtRecoEtaFineScalePt_%d",i);
    histTitle = Form("%s;#it{p}_{T,rec};#eta;#it{p}_{T,rec}/#it{p}_{T,gen}",histName.Data());
    fh3PtRecoEtaFineScalePt[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEtaFine,binsEtaFine,nBinsScalePt,binsScalePt);
    fOutput->Add(fh3PtRecoEtaFineScalePt[i]);
    
    histName = Form("fh3PtTrueEtaDeltaR_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#Delta R_{rec,gen}",histName.Data());
    fh3PtTrueEtaDeltaR[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDeltaR,binsDeltaR);
    fOutput->Add(fh3PtTrueEtaDeltaR[i]);

    //mass scale
    histName = Form("fh3PtTrueEtaDeltaM_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{M}_{rec}-#it{M}_{gen}",histName.Data());
    fh3PtTrueEtaDeltaM[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDM,binsDM);
    fOutput->Add(fh3PtTrueEtaDeltaM[i]);

    histName = Form("fh3PtTrueEtaDeltaMRel_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;(#it{M}_{rec}-#it{M}_{gen})/#it{M}_{gen}",histName.Data());
    fh3PtTrueEtaDeltaMRel[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsDMRel,binsDMRel);
    fOutput->Add(fh3PtTrueEtaDeltaMRel[i]);

    histName = Form("fh3PtTrueEtaScaleM_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};Eta;#it{M}_{rec}/#it{M}_{gen}",histName.Data());
    fh3PtTrueEtaScaleM[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsEta,binsEta,nBinsScaleM,binsScaleM);
    fOutput->Add(fh3PtTrueEtaScaleM[i]);

    histName = Form("fh3PtTrueMTrueScaleM_Cent%d",i);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};(#it{M}_{sub}-#it{M}_{true})/#it{M}_{true}",histName.Data());
    fh3PtTrueMTrueScaleM[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsScaleM,minScaleM,maxScaleM);
    fOutput->Add(fh3PtTrueMTrueScaleM[i]);

    //pt response matrix
    histName = Form("fh3PtTruePtSubEta_%d",i);
    histTitle = Form("%s;#it{p}_{T,gen};#it{p}_{T,rec};Eta",histName.Data());
    fh3PtTruePtSubEta[i] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,binsPtPart,nBinsPtDet,binsPtDet,nBinsEta,binsEta);
    fOutput->Add(fh3PtTruePtSubEta[i]);
  }
  
  if(binsPhi)               delete [] binsPhi;
  if(binsEta)               delete [] binsEta;
  if(binsEtaFine)           delete [] binsEtaFine;
  if(binsPtPart)            delete [] binsPtPart;
  if(binsPtDet)             delete [] binsPtDet;
  if(binsDPt)               delete [] binsDPt;
  if(binsDPtRel)            delete [] binsDPtRel;
  if(binsScalePt)           delete [] binsScalePt;
  if(binsDeltaR)            delete [] binsDeltaR;
  if(binsMPart)             delete [] binsMPart;
  if(binsDM)                delete [] binsDM;
  if(binsDMRel)             delete [] binsDMRel;
  if(binsScaleM)            delete [] binsScaleM;
  if(binsNPV)               delete [] binsNPV;

}

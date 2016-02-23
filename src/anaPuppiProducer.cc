#include "UserCode/diall/interface/anaPuppiProducer.h"

#include "UserCode/diall/interface/lwJet.h"
#include "UserCode/diall/interface/pfParticle.h"

#include "Math/QuantFuncMathCore.h"
#include "Math/SpecFuncMathCore.h"
#include "Math/ProbFunc.h"
#include "TClass.h"
#include "TMath.h"

#include <iostream>

ClassImp(anaPuppiProducer)
   
anaPuppiProducer::anaPuppiProducer(const char *name, const char *title) 
:anaBaseTask(name,title),
  fConeRadius(0.3),
  fCentBin(-1),
  fNExLJ(2),
  fMinPtExLJ(20.),
  fdRMaxJet(0.4),
  //  fAddMetricType(kSumPt),
  fPuppiWeightType(kAlpha),
  fPtMinParticle(0.),
  fPFParticlesName(""),
  fPFParticles(0x0),
  fJetsName(""),
  fJetsCont(0x0),
  fPuppiParticlesName(""),
  fPuppiParticles(0x0),
  fMapEtaRanges(),
  fWeightCut(0.01),
  fh2CentMedianAlpha(),
  fh2CentRMSAlpha(),
  fh2CentMedianMetric2(),
  fh2CentRMSMetric2(),
  fStoreTree(false),
  fTreeOut(0x0),
  fcent(-999),
  fnpart(0),
  fMedAlpha(-1),
  fMedAlpha2(-1),
  fMedSumPt(-1),
  fMedMeanPt(-1),
  fMedMetric2(-1),
  fMedMeanMd(-1),
  fnSigJets(0)
{

  //Set default eta ranges
  //[-5,-3,-2.1,2.1,3,5] -> 5 intervals
  fMapEtaRanges[1] = -5.;
  fMapEtaRanges[2] = -3.;
  fMapEtaRanges[3] = -2.1;
  fMapEtaRanges[4] =  2.1;
  fMapEtaRanges[5] =  3.;
  fMapEtaRanges[6] =  5.;
  
}

//________________________________________________________________________
anaPuppiProducer::~anaPuppiProducer() {
  // Destructor
 
}

//----------------------------------------------------------
void anaPuppiProducer::Exec(Option_t * /*option*/)
{
  //printf("anaPuppiProducer executing\n");
  anaBaseTask::Exec();
    if(!fInitOutput) CreateOutputObjects();

   if(!fEventObjects) {
     Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
     return;
   }
   
   //Get objects from event

   //Get jet container
   if(!fJetsCont && !fJetsName.IsNull())
     fJetsCont = dynamic_cast<lwJetContainer*>(fEventObjects->FindObject(fJetsName.Data()));
   if(!fJetsCont) {
     Printf("%s: Jets not found %s",GetName(),fJetsName.Data());
     return;
   }
   TClonesArray *jets = fJetsCont->GetJets();
   if(!jets) return;
      
   //Get pf particles
   if(!fPFParticles && !fPFParticlesName.IsNull()) {
     fPFParticles = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fPFParticlesName.Data()));
   }
   if(!fPFParticles) return;

   //Make array for puppi particles
   if(!fPuppiParticlesName.IsNull()) {
     if(!fEventObjects->FindObject(fPuppiParticlesName) && !fPuppiParticles) {
       fPuppiParticles = new TClonesArray("pfParticle");
       fPuppiParticles->SetName(fPuppiParticlesName);
       fEventObjects->Add(fPuppiParticles);
     }
   }
   if(fPuppiParticles) fPuppiParticles->Delete();

   //Determine centrality bin
   double cent = 0.;
   if(!fHiEvent) Printf("%s: couldn't locate fHiEvent",GetName());
   else {
     cent = fHiEvent->GetCentrality();
   }
   if(cent>=0. && cent<10.)       fCentBin = 0;
   else if(cent>=10. && cent<30.) fCentBin = 1;
   else if(cent>=30. && cent<50.) fCentBin = 2;
   else if(cent>=50. && cent<80.) fCentBin = 3;
   else fCentBin = -1;
   
   //Find signal jets at detector level
   //Int_t nSignalJetsDet = 0;
   fnSigJets = 0;
   // Int_t sigDetIds[999];
   //Double_t sigDetPhi[999];
   //Double_t sigDetEta[999];
   Int_t nj = TMath::Min(fNExLJ,fJetsCont->GetNJets());
   for(Int_t ij = 0; ij<nj; ij++) {
     lwJet *jet = static_cast<lwJet*>(jets->At(ij));
     if(jet->Pt()>fMinPtExLJ) {
       // sigDetIds[nSignalJetsDet] = ij;
       //sigDetPhi[nSignalJetsDet] = jet->Phi();
       //sigDetEta[nSignalJetsDet] = jet->Eta();
       fSigJetPt[fnSigJets] = jet->Pt();
       fSigJetPhi[fnSigJets] = jet->Phi();
       fSigJetEta[fnSigJets] = jet->Eta();
       //nSignalJetsDet++;
       fnSigJets++;
     }
   }
   //Printf("signal jets located"); 
   //pf candidate loop to calculate puppi metrics for each particle

   //plan: apply pt cut to particles, put particles passing in std::vector and work with those for the rest of the algo
   std::vector<pfParticle*> fSelPFParticles;
   for (int i = 0; i < fPFParticles->GetEntriesFast(); i++) {
     pfParticle *p1 = static_cast<pfParticle*>(fPFParticles->At(i));
     if(!p1) continue;
     if(p1->Pt()<fPtMinParticle) continue;
     fSelPFParticles.push_back(p1);
   }
   //Printf("pf candidates selected");
   double drmin = 0.02;
   for (unsigned int i = 0; i < fSelPFParticles.size(); i++) {
     pfParticle *p1 = fSelPFParticles[i];
     if(!p1) continue;
     if(p1->GetId()==1) drmin = 0.02;
     else               drmin = 0.05;

     double alpha = 0.;
     // double mass = 0.;
     double sumpt = 0.;
     double alpha2 = 0.;
     double meanmd = 0.;
     int    nconst = 0;
     TLorentzVector lsum(0.,0.,0.,0.);

     for (unsigned int j = 0; j < fSelPFParticles.size(); j++) {
       if(i==j) continue;
       pfParticle *p2 = fSelPFParticles[j];
       if(!p2) continue;
       double dr = p1->DeltaR(p2);
       if(dr<drmin || dr>fConeRadius) continue;
       alpha += p2->Pt() /dr/dr;
       alpha2 += p2->Pt() /dr;
       sumpt += p2->Pt();
       lsum+=p2->GetLorentzVector();
       meanmd += sqrt(p2->M()*p2->M() + p2->Pt()*p2->Pt()) - p2->Pt();
       nconst++;
     }
     if(alpha!=0.)  alpha  = log(alpha);
     if(alpha2!=0.) alpha2 = log(alpha2);
     p1->SetPuppiAlpha(alpha);
     p1->SetPuppiAlpha2(alpha2);
     p1->SetPuppiMetric2(lsum.M());
     p1->SetPuppiSumPt(sumpt);
     if(nconst>0) p1->SetPuppiMeanPt(sumpt/((double)nconst));
     if(nconst>0) p1->SetPuppiMeanMd(meanmd/((double)nconst));
   }//particles loop
   //Printf("metrics stored");
   //calculation of median and RMS alpha in eta ranges
   std::map<int,double> fMapMedianMetric;  //median selected metric in eta regions
   std::map<int,double> fMapRmsMetric;     //rms selected in eta regions

   std::map<int,double> fMapMedianAlpha;   //median default metric in eta regions
   std::map<int,double> fMapRmsAlpha;      //rms default metric in eta regions
   std::map<int,double> fMapMedianAlpha2;  //median alpha2 in eta regions
   std::map<int,double> fMapRmsAlpha2;     //rms alpha2 in eta regions
   std::map<int,double> fMapMedianSumPt;   //median sumPt in eta regions
   std::map<int,double> fMapRmsSumPt;      //rms sumPt in eta regions
   std::map<int,double> fMapMedianMeanPt;  //median mean pt in eta regions
   std::map<int,double> fMapRmsMeanPt;     //rms mean pt in eta regions
   std::map<int,double> fMapMedianMetric2; //median metric2 in eta regions
   std::map<int,double> fMapRmsMetric2;    //rms metric2 in eta regions
   std::map<int,double> fMapMedianMeanMd;  //median mean md in eta regions
   std::map<int,double> fMapRmsMeanMd;     //rms mean md in eta regions

   Int_t neta = (Int_t)fMapEtaRanges.size();
   for(Int_t ieta = 1; ieta<neta; ieta++) {
     static Double_t metricArrExLJ[9999]  = {0.};
     static Double_t alphaArrExLJ[9999]   = {0.};
     static Double_t alpha2ArrExLJ[9999]  = {0.};
     static Double_t sumPtArrExLJ[9999]   = {0.};
     static Double_t meanPtArrExLJ[9999]  = {0.};
     static Double_t metric2ArrExLJ[9999] = {0.};
     static Double_t meanMdArrExLJ[9999]  = {0.};
     
     Int_t count = 0;

     Double_t etaMin = fMapEtaRanges.at(ieta)+fConeRadius;
     Double_t etaMax = fMapEtaRanges.at(ieta+1)-fConeRadius;

     //Printf("ieta: %d  etaMin: %f etaMax: %f",ieta,etaMin,etaMax);
     
     for (unsigned int i = 0; i < fSelPFParticles.size(); i++) {
       pfParticle *p1 = fSelPFParticles[i];
       if(!p1) continue; 
       if(p1->Eta()>=etaMin && p1->Eta()<etaMax) {       
         //check distance to closest signal jet
         double drSig = 999.;
         //for(int is = 0; is<nSignalJetsDet; is++) {
         for(int is = 0; is<fnSigJets; is++) {
           //double dPhi = p1->Phi() - sigDetPhi[is];
           //double dEta = p1->Eta() - sigDetEta[is];
           double dPhi = p1->Phi() - fSigJetPhi[is];
           double dEta = p1->Eta() - fSigJetEta[is];
           dPhi = TVector2::Phi_mpi_pi(dPhi);
           double dr2tmp = dPhi * dPhi + dEta * dEta;
           double drtmp = 0.;
           if(dr2tmp>0.) drtmp = TMath::Sqrt(dr2tmp);
           if(drtmp<drSig) {
             drSig = drtmp;
           }
         }//signal jets

         //Excluding regions close to leading detector-level jet
         if(drSig>fdRMaxJet) {
           if(fPuppiWeightType==kAlpha)
             metricArrExLJ[count] = p1->GetPuppiAlpha();
           else if(fPuppiWeightType==kAlpha2)
             metricArrExLJ[count] = p1->GetPuppiAlpha2();
           else if(fPuppiWeightType==kSumPt)
             metricArrExLJ[count] = p1->GetPuppiSumPt();
           else if(fPuppiWeightType==kMeanPt)
             metricArrExLJ[count] = p1->GetPuppiMeanPt();
           else if(fPuppiWeightType==kMetric2)
             metricArrExLJ[count] = p1->GetPuppiMetric2();
           alphaArrExLJ[count] = p1->GetPuppiAlpha();
           alpha2ArrExLJ[count] = p1->GetPuppiAlpha2();
           sumPtArrExLJ[count] = p1->GetPuppiSumPt();
           meanPtArrExLJ[count] = p1->GetPuppiMeanPt();
           metric2ArrExLJ[count] = p1->GetPuppiMetric2();
           meanMdArrExLJ[count] = p1->GetPuppiMeanMd();

           p1->SetPuppiId(1);
           
           count++;
         }
       }//eta selection
     }//particles loop
     
     //static Int_t indexes[9999] = {-1};//indexes for sorting
     //TMath::Sort(count,metricArrExLJ,indexes);
     Double_t medMetric = TMath::Median(count,metricArrExLJ);
     //static Int_t indexes2[9999] = {-1};//indexes for sorting
     //TMath::Sort(count,metric2ArrExLJ,indexes2);

     Double_t medAlpha   = TMath::Median(count,alphaArrExLJ);
     Double_t medAlpha2  = TMath::Median(count,alpha2ArrExLJ);
     Double_t medSumPt   = TMath::Median(count,sumPtArrExLJ);
     Double_t medMeanPt  = TMath::Median(count,meanPtArrExLJ);
     Double_t medMetric2 = TMath::Median(count,metric2ArrExLJ);
     Double_t medMeanMd  = TMath::Median(count,meanMdArrExLJ);
     /*
     Printf("ieta: %d",ieta);
     Printf("N particles UE: %d",count);
     Printf("medAlpha: %f", medAlpha);
     Printf("medAlpha2: %f", medAlpha2);
     Printf("medSumPt: %f", medSumPt);
     Printf("medMeanPt: %f", medMeanPt);
     Printf("medMetric2: %f", medMetric2);
     Printf("medMeanMd: %f", medMeanMd);
     Printf("---------");
     */ 
     //Calculate LHS RMS. LHS defined as all entries up to median
     Int_t nias = TMath::FloorNint((Double_t)count/2.);
     Double_t rmsMetric   = 0.;
     Double_t rmsAlpha    = 0.;
     Double_t rmsAlpha2   = 0.;
     Double_t rmsSumPt    = 0.;
     Double_t rmsMeanPt   = 0.;
     Double_t rmsMetric2  = 0.;
     Double_t rmsMeanMd   = 0.;
     for(Int_t ia = 0; ia<count; ia++) { //taking entries starting from nias since sorted from high to low
       Double_t alph = metricArrExLJ[ia];//indexes[ia]];
       if(alph<medMetric)
         rmsMetric += (alph - medMetric)*(alph - medMetric);
       
       //      if(alph>medMetric) Printf("WARNING: alph (%f) larger than medMetric (%f)",alph,medMetric);
      
      if(alphaArrExLJ[ia]<medAlpha)
        rmsAlpha += (alphaArrExLJ[ia] - medAlpha)*(alphaArrExLJ[ia] - medAlpha);

      if(alpha2ArrExLJ[ia]<medAlpha2)
        rmsAlpha2 += (alpha2ArrExLJ[ia] - medAlpha2)*(alpha2ArrExLJ[ia] - medAlpha2);

      if(sumPtArrExLJ[ia]<medSumPt)
        rmsSumPt += (sumPtArrExLJ[ia] - medSumPt)*(sumPtArrExLJ[ia] - medSumPt);

      if(meanPtArrExLJ[ia]<medMeanPt)
        rmsMeanPt += (meanPtArrExLJ[ia] - medMeanPt)*(meanPtArrExLJ[ia] - medMeanPt);
 
       Double_t metr2 = metric2ArrExLJ[ia];//indexes[ia]];
       if(metr2<medMetric2) 
         rmsMetric2 += (metr2 - medMetric2)*(metr2 - medMetric2);

       if(meanMdArrExLJ[ia]<medMeanMd)
         rmsMeanMd += (meanPtArrExLJ[ia] - medMeanMd)*(meanPtArrExLJ[ia] - medMeanMd);

       //       if(metr2>medMetric2) Printf("WARNING: metr2 (%f) larger than medMetric2 (%f)",metr2,medMetric2);

     }
     if(rmsMetric>0.)  rmsMetric  = TMath::Sqrt(rmsMetric/((double)nias));
     if(rmsAlpha>0.)   rmsAlpha   = TMath::Sqrt(rmsAlpha/((double)nias));
     if(rmsAlpha2>0.)  rmsAlpha2  = TMath::Sqrt(rmsAlpha2/((double)nias));
     if(rmsSumPt>0.)   rmsSumPt   = TMath::Sqrt(rmsSumPt/((double)nias));
     if(rmsMeanPt>0.)  rmsMeanPt  = TMath::Sqrt(rmsMeanPt/((double)nias));
     if(rmsMetric2>0.) rmsMetric2 = TMath::Sqrt(rmsMetric2/((double)nias));
     if(rmsMeanMd>0.)  rmsMeanMd  = TMath::Sqrt(rmsMeanMd/((double)nias));

     //Fill histograms, only for mid rapidity
     if(ieta==3) {
       fh2CentMedianAlpha->Fill(cent,medMetric);
       fh2CentRMSAlpha->Fill(cent,rmsMetric);
       fh2CentMedianMetric2->Fill(cent,medMetric2);
       fh2CentRMSMetric2->Fill(cent,rmsMetric2);       
     }
     fMapMedianMetric[ieta]  = medMetric;
     fMapRmsMetric[ieta]     = rmsMetric;

     fMapMedianAlpha[ieta]   = medAlpha;
     fMapRmsAlpha[ieta]      = rmsAlpha;
     fMapMedianAlpha2[ieta]  = medAlpha2;
     fMapRmsAlpha2[ieta]     = rmsAlpha2;
     fMapMedianSumPt[ieta]   = medSumPt;
     fMapRmsSumPt[ieta]      = rmsSumPt;
     fMapMedianMeanPt[ieta]  = medMeanPt;
     fMapRmsMeanPt[ieta]     = rmsMeanPt;
     fMapMedianMetric2[ieta] = medMetric2;
     fMapRmsMetric2[ieta]    = rmsMetric2;
     fMapMedianMeanMd[ieta]  = medMeanMd;
     fMapRmsMeanMd[ieta]     = rmsMeanMd;
     
   }//eta bins

   fMedAlpha   =  fMapMedianAlpha[3];
   fMedAlpha2  =  fMapMedianAlpha2[3];
   fMedSumPt   =  fMapMedianSumPt[3];
   fMedMeanPt  =  fMapMedianMeanPt[3];
   fMedMetric2 =  fMapMedianMetric2[3];
   fMedMeanMd  =  fMapMedianMeanMd[3];
   //Printf("medians calculated"); 
   //Set puppi weight for each particle
   int npup = 0;
   int ntree = 0;
   for (unsigned int i = 0; i < fSelPFParticles.size(); i++) {
     pfParticle *p1 = fSelPFParticles[i];
     if(!p1) continue;
     Double_t prob = 0.;
     Int_t etaBin = -1;
     for(Int_t ieta = 1; ieta<neta; ++ieta) {
       Double_t etaMin = fMapEtaRanges.at(ieta);
       Double_t etaMax = fMapEtaRanges.at(ieta+1);
       if(p1->Eta()>=etaMin && p1->Eta()<etaMax)
         etaBin = ieta;
     }
     if(etaBin<1) {
       if(p1->Eta()<fMapEtaRanges.at(1)) etaBin = 1;
       if(p1->Eta()>fMapEtaRanges.at(neta)) etaBin = neta-1;
     }
     Double_t medMetric = fMapMedianMetric[etaBin];
     Double_t rmsMetric = fMapRmsMetric[etaBin];
     Double_t chiMetric = 1.;
//     std::cout << "fPuppiWeightType: " << fPuppiWeightType << "  " << kMeanPtMd << std::endl;
     //Printf("etaBin: %d",etaBin);
     
       double var = p1->GetPuppiAlpha();
       if(fPuppiWeightType==kAlpha2 || fPuppiWeightType==kSumPt || fPuppiWeightType==kMeanPt || fPuppiWeightType==kMetric2 || fPuppiWeightType==kAlpha) {
         if(fPuppiWeightType==kAlpha)
           var = p1->GetPuppiAlpha();
         else if(fPuppiWeightType==kAlpha2)
           var = p1->GetPuppiAlpha2();
         else if(fPuppiWeightType==kSumPt)
           var = p1->GetPuppiSumPt();
         else if(fPuppiWeightType==kMeanPt)
           var = p1->GetPuppiMeanPt();
         else if(fPuppiWeightType==kMetric2)
           var = p1->GetPuppiMetric2();
         if(rmsMetric>0.) chiMetric = (var - medMetric) * fabs(var - medMetric) / rmsMetric / rmsMetric;
         //chiMetric = (var - medMetric) * (var - medMetric) / rmsMetric / rmsMetric;
         prob = ROOT::Math::chisquared_cdf(chiMetric,1.);
       } else if( fPuppiWeightType==kMeanPtMd ) {
         double meanPt = p1->GetPuppiMeanPt();
         double medMeanPt = fMapMedianMeanPt[etaBin];
         double rmsMeanPt = fMapRmsMeanPt[etaBin];
         double chiMeanPt = (meanPt - medMeanPt) * fabs(meanPt - medMeanPt) / rmsMeanPt / rmsMeanPt;
         //double chiMeanPt = (meanPt - medMeanPt) * (meanPt - medMeanPt) / rmsMeanPt / rmsMeanPt;
         double meanMd = p1->GetPuppiMeanMd();
         double medMeanMd = fMapMedianMeanMd[etaBin];
         double rmsMeanMd = fMapRmsMeanMd[etaBin];
         double chiMeanMd = (meanMd - medMeanMd) * fabs(meanMd - medMeanMd) / rmsMeanMd / rmsMeanMd;
         //double chiMeanMd = (meanMd - medMeanMd) * (meanMd - medMeanMd) / rmsMeanMd / rmsMeanMd;
         chiMetric = chiMeanPt + chiMeanMd;
         prob = ROOT::Math::chisquared_cdf(chiMetric,2.);
       } else if( fPuppiWeightType==kSumPtAlpha2 ) {
         double sumPt = p1->GetPuppiSumPt();
         double medSumPt = fMapMedianSumPt[etaBin];
         double rmsSumPt = fMapRmsSumPt[etaBin];
         double chiSumPt = (sumPt - medSumPt) * fabs(sumPt - medSumPt) / rmsSumPt / rmsSumPt;
         double alpha2 = p1->GetPuppiAlpha2();
         double medAlpha2 = fMapMedianAlpha2[etaBin];
         double rmsAlpha2 = fMapRmsAlpha2[etaBin];
         double chiAlpha2 = (alpha2 - medAlpha2) * fabs(alpha2 - medAlpha2) / rmsAlpha2 / rmsAlpha2;
         //double chiAlpha2 = (alpha2 - medAlpha2) * (alpha2 - medAlpha2) / rmsAlpha2 / rmsAlpha2;
         chiMetric = chiSumPt + chiAlpha2;
         prob = ROOT::Math::chisquared_cdf(chiMetric,2.);
       }
     //}
     p1->SetPuppiWeight(prob);

     Double_t chiAlpha     = 0.;
     Double_t chiAlpha2    = 0.;
     Double_t chiSumPt     = 0.;
     Double_t chiMeanPt    = 0.;
     Double_t chiMeanMd    = 0.;
     Double_t chiMetric2   = 0.;

     Double_t probAlpha    = 0.;
     Double_t probAlpha2   = 0.;
     Double_t probSumPt    = 0.;
     Double_t probMeanPt   = 0.;
     Double_t probMeanMd   = 0.;
     Double_t probMetric2  = 0.;
     
     if(fMapRmsAlpha[etaBin]>0.) {
       chiAlpha = (p1->GetPuppiAlpha() - fMapMedianAlpha[etaBin]) * fabs(p1->GetPuppiAlpha() - fMapMedianAlpha[etaBin]) / fMapRmsAlpha[etaBin] / fMapRmsAlpha[etaBin];
       probAlpha = ROOT::Math::chisquared_cdf(chiAlpha,1.);
     }
     if(fMapRmsAlpha2[etaBin]>0.) {
       chiAlpha2 = (p1->GetPuppiAlpha2() - fMapMedianAlpha2[etaBin]) * fabs(p1->GetPuppiAlpha2() - fMapMedianAlpha2[etaBin]) / fMapRmsAlpha2[etaBin] / fMapRmsAlpha2[etaBin];
       probAlpha2 = ROOT::Math::chisquared_cdf(chiAlpha2,1.);
     }
     if(fMapRmsSumPt[etaBin]>0.) {
       chiSumPt = (p1->GetPuppiSumPt() - fMapMedianSumPt[etaBin]) * fabs(p1->GetPuppiSumPt() - fMapMedianSumPt[etaBin]) / fMapRmsSumPt[etaBin] / fMapRmsSumPt[etaBin];
       probSumPt = ROOT::Math::chisquared_cdf(chiSumPt,1.);
     }
     if(fMapRmsMeanPt[etaBin]>0.) {
       chiMeanPt = (p1->GetPuppiMeanPt() - fMapMedianMeanPt[etaBin]) * fabs(p1->GetPuppiMeanPt() - fMapMedianMeanPt[etaBin]) / fMapRmsMeanPt[etaBin] / fMapRmsMeanPt[etaBin];
       probMeanPt = ROOT::Math::chisquared_cdf(chiMeanPt,1.);
     }
     if(fMapRmsMeanMd[etaBin]>0.) {
       chiMeanMd = (p1->GetPuppiMeanMd() - fMapMedianMeanMd[etaBin]) * fabs(p1->GetPuppiMeanMd() - fMapMedianMeanMd[etaBin]) / fMapRmsMeanMd[etaBin] / fMapRmsMeanMd[etaBin];
       probMeanMd = ROOT::Math::chisquared_cdf(chiMeanMd,1.);
     }
     if(fMapRmsMetric2[etaBin]>0.) {
       chiMetric2 = (p1->GetPuppiMetric2() - fMapMedianMetric2[etaBin]) * fabs(p1->GetPuppiMetric2() - fMapMedianMetric2[etaBin]) / fMapRmsMetric2[etaBin] / fMapRmsMetric2[etaBin];
       probMetric2 = ROOT::Math::chisquared_cdf(chiMetric2,1.);
     }

     // if(fMapRmsAlpha[etaBin]>0.) {
     //   chiAlpha = (p1->GetPuppiAlpha() - fMapMedianAlpha[etaBin]) * (p1->GetPuppiAlpha() - fMapMedianAlpha[etaBin]) / fMapRmsAlpha[etaBin] / fMapRmsAlpha[etaBin];
     //   probAlpha = ROOT::Math::chisquared_cdf(chiAlpha,1.);
     // }
     // if(fMapRmsAlpha2[etaBin]>0.) {
     //   chiAlpha2 = (p1->GetPuppiAlpha2() - fMapMedianAlpha2[etaBin]) * (p1->GetPuppiAlpha2() - fMapMedianAlpha2[etaBin]) / fMapRmsAlpha2[etaBin] / fMapRmsAlpha2[etaBin];
     //   probAlpha2 = ROOT::Math::chisquared_cdf(chiAlpha2,1.);
     // }
     // if(fMapRmsSumPt[etaBin]>0.) {
     //   chiSumPt = (p1->GetPuppiSumPt() - fMapMedianSumPt[etaBin]) * (p1->GetPuppiSumPt() - fMapMedianSumPt[etaBin]) / fMapRmsSumPt[etaBin] / fMapRmsSumPt[etaBin];
     //   probSumPt = ROOT::Math::chisquared_cdf(chiSumPt,1.);
     // }
     // if(fMapRmsMeanPt[etaBin]>0.) {
     //   chiMeanPt = (p1->GetPuppiMeanPt() - fMapMedianMeanPt[etaBin]) * (p1->GetPuppiMeanPt() - fMapMedianMeanPt[etaBin]) / fMapRmsMeanPt[etaBin] / fMapRmsMeanPt[etaBin];
     //   probMeanPt = ROOT::Math::chisquared_cdf(chiMeanPt,1.);
     // }
     // if(fMapRmsMetric2[etaBin]>0.) {
     //   chiMetric2 = (p1->GetPuppiMetric2() - fMapMedianMetric2[etaBin]) * (p1->GetPuppiMetric2() - fMapMedianMetric2[etaBin]) / fMapRmsMetric2[etaBin] / fMapRmsMetric2[etaBin];
     //   probMetric2 = ROOT::Math::chisquared_cdf(chiMetric2,1.);
     // }
     
     //weight metric+metric2
     Double_t prob2 = ROOT::Math::chisquared_cdf(chiMetric+chiMetric2,2.);
     p1->SetPuppiWeight2(prob2);
     p1->SetPuppiWeight3(probMetric2);
     
     //put puppi weighted particles in array
     double ptpup = prob*p1->Pt();
     if(prob<fWeightCut) { ptpup = 0.; prob = 0.; }
     // if(fPuppiWeightType==kAlphaMetric2) ptpup = prob2*p1->Pt();
     // else if(fPuppiWeightType==kMetric2) ptpup = prob3*p1->Pt();
     if(fPuppiParticles) {
       if(ptpup>1e-4) {
         pfParticle *pPart = new ((*fPuppiParticles)[npup])
           pfParticle(ptpup,
                      p1->Eta(),
                      p1->Phi(),
                      prob*p1->M(),
                      p1->GetId());
         pPart->SetCharge(p1->GetCharge());
         pPart->SetPuppiId(p1->GetPuppiId());
         pPart->SetPuppiAlpha(p1->GetPuppiAlpha());
         pPart->SetPuppiMetric2(p1->GetPuppiMetric2());
         pPart->SetPuppiSumPt(p1->GetPuppiSumPt());
         pPart->SetPuppiAlpha2(p1->GetPuppiAlpha2());
         pPart->SetPuppiMeanPt(p1->GetPuppiMeanPt());
         pPart->SetPuppiWeight(prob);
         pPart->SetPuppiWeight2(prob2);
         pPart->SetPuppiWeight3(probMetric2);
         ++npup;
       }
     }

     if(fStoreTree) {
       fpt[ntree]       = p1->Pt();
       feta[ntree]      = p1->Eta();
       fphi[ntree]      = p1->Phi();
       fue[ntree]       = p1->GetPuppiId();
       fetabin[ntree]   = etaBin;
         
       falpha[ntree]    = p1->GetPuppiAlpha();
       falpha2[ntree]   = p1->GetPuppiAlpha2();
       fsumpt[ntree]    = p1->GetPuppiSumPt();
       fmeanpt[ntree]   = p1->GetPuppiMeanPt();
       fmeanmd[ntree]   = p1->GetPuppiMeanMd();
       fmetric2[ntree]  = p1->GetPuppiMetric2();

       fwalpha[ntree]   = probAlpha;
       fwalpha2[ntree]  = probAlpha2;
       fwsumpt[ntree]   = probSumPt;
       fwmeanpt[ntree]  = probMeanPt;
       fwmeanmd[ntree]  = probMeanMd;
       fwmetric2[ntree] = probMetric2;
         
       //Get closest jet
       lwJet *jetClosest = 0x0;
       double drmin = 999.;
       for(Int_t ij = 0; ij<fJetsCont->GetNJets(); ++ij) {
         lwJet *jet = static_cast<lwJet*>(jets->At(ij));
         double dr = p1->DeltaR(jet);
         if(dr<drmin) {
           jetClosest = jet;
           drmin = dr;
         }
       }
       if(jetClosest) {
         fptjet[ntree] = jetClosest->Pt();
         fdrjet[ntree] = drmin;
       } else {
         fptjet[ntree] = 0.;
         fdrjet[ntree] = 0.;
       }     
       ++ntree;
     }
     // if(etaBin==5 && p1->GetPuppiId()==1) Printf("etaBin: %d ntree: %d pt: %f eta: %f phi: %f",etaBin,ntree,p1->Pt(),p1->Eta(),p1->Phi());
   }

   if(fStoreTree) {
     fcent = (float)cent;
     fnpart = ntree;
     fTreeOut->Fill();
   }
   if(fPuppiParticles) fPuppiParticles->Sort();
}

//----------------------------------------------------------
void anaPuppiProducer::CreateOutputObjects() {

  anaBaseTask::CreateOutputObjects();

  if(!fOutput) {
    Printf("anaPuppiProducer: fOutput not present");
    return;
  }

  fh2CentMedianAlpha = new TH2F("fh2CentMedianAlpha","fh2CentMedianAlpha;centrality (%);med{#alpha}",10,0,100,40,0,20);
  fOutput->Add(fh2CentMedianAlpha);

  fh2CentRMSAlpha = new TH2F("fh2CentRMSAlpha","fh2CentRMSAlpha;centrality (%);RMS{#alpha}",10,0,100,40,0,4);
  fOutput->Add(fh2CentRMSAlpha);

  fh2CentMedianMetric2 = new TH2F("fh2CentMedianMetric2","fh2CentMedianMetric2;centrality (%);med{metric2}",10,0,100,100,0,100);
  fOutput->Add(fh2CentMedianMetric2);

  fh2CentRMSMetric2 = new TH2F("fh2CentRMSMetric2","fh2CentRMSMetric2;centrality (%);RMS{metric2}",10,0,100,30,0,30);
  fOutput->Add(fh2CentRMSMetric2);

  if(fStoreTree) {
    fTreeOut = new TTree(Form("%sTree",GetName()),"puppi particles tree");
    fTreeOut->Branch("cent", &fcent,      "cent/F");
    fTreeOut->Branch("npart",&fnpart,     "npart/I");

    fTreeOut->Branch("medAlpha",  &fMedAlpha,   "medAlpha/F");
    fTreeOut->Branch("medAlpha2", &fMedAlpha2,  "medAlpha2/F");
    fTreeOut->Branch("medSumPt",  &fMedSumPt,   "medSumPt/F");
    fTreeOut->Branch("medMeanPt", &fMedMeanPt,  "medMeanPt/F");
    fTreeOut->Branch("medMeanMd", &fMedMeanMd,  "medMeanMd/F");
    fTreeOut->Branch("medMetric2",&fMedMetric2, "medMetric2/F");
    
    fTreeOut->Branch("pt",     &fpt,      "pt[npart]/F");
    fTreeOut->Branch("eta",    &feta,     "eta[npart]/F");
    fTreeOut->Branch("phi",    &fphi,     "phi[npart]/F");
    fTreeOut->Branch("ue",     &fue,      "ue[npart]/I");
    fTreeOut->Branch("etabin", &fetabin,  "etabin[npart]/I");
    fTreeOut->Branch("alpha",  &falpha,   "alpha[npart]/F");
    fTreeOut->Branch("alpha2", &falpha2,  "alpha2[npart]/F");
    fTreeOut->Branch("sumpt" , &fsumpt,   "sumpt[npart]/F");
    fTreeOut->Branch("meanpt", &fmeanpt,  "meanpt[npart]/F");
    fTreeOut->Branch("meanmd", &fmeanmd,  "meanmd[npart]/F");
    fTreeOut->Branch("metric2",&fmetric2, "metric2[npart]/F");

    fTreeOut->Branch("walpha",  &fwalpha,     "walpha[npart]/F");
    fTreeOut->Branch("walpha2", &fwalpha2,    "walpha2[npart]/F");
    fTreeOut->Branch("wsumpt" , &fwsumpt,     "wsumpt[npart]/F");
    fTreeOut->Branch("wmeanpt", &fwmeanpt,    "wmeanpt[npart]/F");
    fTreeOut->Branch("wmeanmd", &fwmeanmd,    "wmeanmd[npart]/F");
    fTreeOut->Branch("wmetric2",&fwmetric2,   "wmetric2[npart]/F");

    fTreeOut->Branch("ptjet",&fptjet,     "ptjet[npart]/F");
    fTreeOut->Branch("drjet",&fdrjet,     "drjet[npart]/F");

    fTreeOut->Branch("nSigJets",   &fnSigJets,     "nSigJets/I");
    fTreeOut->Branch("ptSigJet",   &fSigJetPt,     "ptSigJet[nSigJets]/F");
    fTreeOut->Branch("etaSigJet",  &fSigJetEta,    "etaSigJet[nSigJets]/F");
    fTreeOut->Branch("phiSigJet",  &fSigJetPhi,    "phiSigJet[nSigJets]/F");

    fOutput->Add(fTreeOut);
  }
}

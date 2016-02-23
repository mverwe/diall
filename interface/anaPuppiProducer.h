#ifndef anaPuppiProducer_h
#define anaPuppiProducer_h

#include <map>

#include "TString.h"
#include "TH2F.h"
#include "TTree.h"
#include "TH3F.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/hiEventContainer.h"
#include "UserCode/diall/interface/pfParticle.h"
#include "UserCode/diall/interface/lwJetContainer.h"

//
// muon isolation with different methods
//

class anaPuppiProducer : public anaBaseTask {
   
public:
   //enum addMetricType {
   //  kSumPt    = 0,
   //  kMass     = 1
   //};
   enum puppiWeightType {
     kAlpha        = 0,
     kAlphaMetric2 = 1,
     kMetric2      = 2,
     kSumPt        = 3,
     kMeanPt       = 4,
     kAlpha2       = 5,
     kMeanPtMd     = 6
   };

   anaPuppiProducer() {;}
   anaPuppiProducer(const char *name, const char *title);
   virtual ~anaPuppiProducer();// {;}

   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetPFPartName(TString name)      { fPFParticlesName = name ; }
   void SetJetsName(TString name)        { fJetsName = name ; }
   void SetPuppiPartName(TString name)   { fPuppiParticlesName = name ; }
   void SetStoreTree(bool b)             { fStoreTree = b;     }

   void SetPtMinParticle(double min)       { fPtMinParticle = min; }

   void SetConeRadius(Double_t r)          { fConeRadius = r; }
   void SetNExLJ(Int_t i)                  { fNExLJ      = i; }
   void SetPtMinExLJ(Double_t m)           { fMinPtExLJ  = m; }
   void SetMinDistToSigJet(Double_t d)     { fdRMaxJet   = d; }
   void SetEtaLimit(Int_t i, Double_t eta) { fMapEtaRanges[i]=eta;}
//   void SetAddMetricType(addMetricType t)  { fAddMetricType = t; }
   void SetPuppiWeightType(puppiWeightType t) { fPuppiWeightType = t; }
   void SetWeightCut(double w)             { fWeightCut = w;  }  
 
 protected:
   Double_t          fConeRadius;       //cone radius for isolation
   Int_t             fCentBin;          //centrality bin
   Int_t             fNExLJ;            //maximum number of signal jets
   Double_t          fMinPtExLJ;        //pt min to qualify as signal jet
   Double_t          fdRMaxJet;         //minimum distance to signal jet
   //addMetricType     fAddMetricType;    //type of additional metric
   puppiWeightType   fPuppiWeightType;  //puppi weight type
   double            fPtMinParticle;    //pt min for pf particles
   TString           fPFParticlesName;  //name of array with pf candidates
   TClonesArray     *fPFParticles;      //!pf candidates array
   TString           fJetsName;         //name of jet array
   lwJetContainer   *fJetsCont;         //!jet container
   TString           fPuppiParticlesName; //name of array with puppi particles
   TClonesArray     *fPuppiParticles;     //!puppi candidates array
   std::map<int,double> fMapEtaRanges;    //eta ranges
   double            fWeightCut;          //minimum puppi weight. If smaller set to zero  
 
   TH2F             *fh2CentMedianAlpha;   //!centrality vs median alpha
   TH2F             *fh2CentRMSAlpha;      //!centrality vs median alpha
   TH2F             *fh2CentMedianMetric2; //!centrality vs median metric2
   TH2F             *fh2CentRMSMetric2;    //!centrality vs median metric2

   bool              fStoreTree;       //store tree
   TTree            *fTreeOut;         //!output tree
   //out tree members
   float             fcent;
   int               fnpart;
   float             fMedAlpha;
   float             fMedAlpha2;
   float             fMedSumPt;
   float             fMedMeanPt;
   float             fMedMetric2;
   float             fMedMeanMd;
   float             fpt[200000];
   float             feta[200000];
   float             fphi[200000];
   int               fue[200000];
   int               fetabin[200000];
   float             falpha[200000];
   float             fmetric2[200000];
   float             fsumpt[200000];
   float             falpha2[200000];
   float             fmeanpt[200000];
   float             fmeanmd[200000];
   float             fwalpha[200000];
   float             fwmetric2[200000];
   float             fwsumpt[200000];
   float             fwalpha2[200000];
   float             fwmeanpt[200000];
   float             fwmeanmd[200000];
   float             fwmeanptmd[200000];
   float             fptjet[200000];
   float             fdrjet[200000];
   //signal jets
   int               fnSigJets;
   float             fSigJetPt[10];
   float             fSigJetPhi[10];
   float             fSigJetEta[10];
   
      
   ClassDef(anaPuppiProducer,6)
};

#endif

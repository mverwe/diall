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
   enum addMetricType {
     kSumPt    = 0,
     kMass     = 1
   };
   enum puppiWeightType {
     kAlpha        = 0,
     kAlphaMetric2 = 1,
     kMetric2      = 2
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
   void SetAddMetricType(addMetricType t)  { fAddMetricType = t; }
   void SetPuppiWeightType(puppiWeightType t) { fPuppiWeightType = t; }
   
 protected:
   Double_t          fConeRadius;       //cone radius for isolation
   Int_t             fCentBin;          //centrality bin
   Int_t             fNExLJ;            //maximum number of signal jets
   Double_t          fMinPtExLJ;        //pt min to qualify as signal jet
   Double_t          fdRMaxJet;         //minimum distance to signal jet
   addMetricType     fAddMetricType;    //type of additional metric
   puppiWeightType   fPuppiWeightType;  //puppi weight type
   double            fPtMinParticle;    //pt min for pf particles
   TString           fPFParticlesName;  //name of array with pf candidates
   TClonesArray     *fPFParticles;      //!pf candidates array
   TString           fJetsName;         //name of jet array
   lwJetContainer   *fJetsCont;         //!jet container
   TString           fPuppiParticlesName; //name of array with puppi particles
   TClonesArray     *fPuppiParticles;     //!puppi candidates array
   std::map<int,double> fMapEtaRanges;    //eta ranges
   
   TH2F             *fh2CentMedianAlpha;   //!centrality vs median alpha
   TH2F             *fh2CentRMSAlpha;      //!centrality vs median alpha
   TH2F             *fh2CentMedianMetric2; //!centrality vs median metric2
   TH2F             *fh2CentRMSMetric2;    //!centrality vs median metric2

   bool              fStoreTree;       //store tree
   TTree            *fTreeOut;         //!output tree
   //out tree members
   float             fcent;
   int               fnpart;
   float             fpt[200000];
   float             feta[200000];
   float             fphi[200000];
   float             falpha[200000];
   float             fmetric2[200000];
   float             fptjet[200000];
   float             fdrjet[200000];
      
   ClassDef(anaPuppiProducer,6)
};

#endif

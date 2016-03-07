#ifndef anaMuonIsolation_h
#define anaMuonIsolation_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/rhoMap.h"
#include "UserCode/diall/interface/pfParticle.h"
#include "UserCode/diall/interface/lwMuon.h"
#include "UserCode/diall/interface/lwJetContainer.h"

//
// muon isolation with different methods
//

class anaMuonIsolation : public anaBaseTask {
   
public:
  enum isolationType {
    kRaw    = 0,
    kArea   = 1,
    kVS     = 2,
    kCS     = 3,
    kPuppi  = 4,
    kPuppi2 = 5
  };
  
   anaMuonIsolation() {;}
   anaMuonIsolation(const char *name, const char *title);
   virtual ~anaMuonIsolation();// {;}

   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetMuonsName(TString name)    { fMuonsName = name ; }
   void SetPFPartName(TString name)   { fPFParticlesName = name ; }
   void SetRhoMapName(TString name)   { fRhoMapName = name ; }
   void SetRhoMMapName(TString name)  { fRhoMMapName = name ; }
   void SetMuonsGenName(TString name) { fMuonsGenName = name ; }

   void SetConeRadius(Double_t r)         { fConeRadius = r; }
   void SetConeOffset(Double_t o)         { fOffset     = o; }
   void SetIsolationType(isolationType t) { fIsolationType = t; }

   void SetJetContName(TString name)      { fJetContainerName = name ; }
   void SetCheckBjet(Bool_t b)            { fCheckBjet = b ; }
   Bool_t IsMuonFromBjet(lwMuon *mu);
   
 protected:
   Double_t          DoConstituentSubtraction(std::vector<pfParticle> particles, const Double_t muEta, const Double_t muPhi, Double_t &ptlead);
   static Bool_t     SortingFunction(std::pair<double,int> i,std::pair<double, int> j);
   
   isolationType     fIsolationType;    //isolation method (diferent handling of background)
   Double_t          fConeRadius;       //cone radius for isolation
   Double_t          fOffset;           //offset to make cone larger for CS
   TRandom3         *fRandom;           //! random number generator
   Int_t             fCentBin;          //centrality bin
   TString           fMuonsName;        //name of reconstructed muon array
   TClonesArray     *fMuons;            //!muon array
   TString           fPFParticlesName;  //name of array with objects to match
   TClonesArray     *fPFParticles;      //!match array
   TString           fRhoMapName;       //rho map name
   rhoMap           *fRhoMap;           //!rho map
   TString           fRhoMMapName;      //rhom map name
   rhoMap           *fRhoMMap;          //!rhom map
   TString           fMuonsGenName;     //name of generated muon array
   TClonesArray     *fMuonsGen;         //!gen muon array
   Bool_t            fCheckBjet;        // check if muon is from bjet
   Double_t          fMaxDistToBjet;    // maximum distance to bjet
   TString           fJetContainerName; //name of jet container
   lwJetContainer   *fJetContainer;     //!jet container
   
   TH2F             *fh2CentIso;        //!isolation (ptCone/ptMu) vs centrality
   TH2F            **fh2IsoZCone;       //!isolation vs z of leading part in cone
   TH2F            **fh2PtRecoIso;      //! isolation vs pt of reco muon
   TH2F            **fh2PtGenIso;       //! isolation vs pt,gen of reco muon
      
   ClassDef(anaMuonIsolation,3)
};
#endif

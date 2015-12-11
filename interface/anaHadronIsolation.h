#ifndef anaHadronIsolation_h
#define anaHadronIsolation_h

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

class anaHadronIsolation : public anaBaseTask {
   
public:
  enum isolationType {
    kRaw    = 0,
    kArea   = 1,
    kVS     = 2,
    kCS     = 3,
    kPuppi  = 4,
    kPuppi2 = 5
  };
  
   anaHadronIsolation() {;}
   anaHadronIsolation(const char *name, const char *title);
   virtual ~anaHadronIsolation();// {;}

   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetPFPartName(TString name)   { fPFParticlesName = name ; }
   void SetRhoMapName(TString name)   { fRhoMapName = name ; }
   void SetRhoMMapName(TString name)  { fRhoMMapName = name ; }
   
   void SetConeRadius(Double_t r)         { fConeRadius = r; }
   void SetConeOffset(Double_t o)         { fOffset     = o; }
   void SetIsolationType(isolationType t) { fIsolationType = t; }
   void SetMinPtProbe(double pt)          { fPtMinProbe = pt;}

   void SetJetRecoName(TString name)      { fJetContRecoName = name ; }
   void SetJetGenName(TString name)       { fJetContGenName = name ; }
      
 protected:
   Double_t          DoConstituentSubtraction(std::vector<pfParticle> particles, const Double_t muEta, const Double_t muPhi, Double_t &ptlead);
   static Bool_t     SortingFunction(std::pair<double,int> i,std::pair<double, int> j);
   
   isolationType     fIsolationType;    //isolation method (diferent handling of background)
   Double_t          fConeRadius;       //cone radius for isolation
   Double_t          fOffset;           //offset to make cone larger for CS
   double            fPtMinProbe;       //min pt of probe charged hadron
   TRandom3         *fRandom;           //! random number generator
   Int_t             fCentBin;          //centrality bin
   TString           fPFParticlesName;  //name of array with objects to match
   TClonesArray     *fPFParticles;      //!match array
   TString           fRhoMapName;       //rho map name
   rhoMap           *fRhoMap;           //!rho map
   TString           fRhoMMapName;      //rhom map name
   rhoMap           *fRhoMMap;          //!rhom map
   TString           fJetContRecoName;  //name of jet container
   lwJetContainer   *fJetContReco;      //!jet container
   TString           fJetContGenName;   //name of jet container, particle level
   lwJetContainer   *fJetContGen;       //!jet container
   
   TH2F             *fh2CentIso;        //!isolation (ptCone/ptMu) vs centrality
   TH2F            **fh2IsoZCone;       //!isolation vs z of leading part in cone
   TH2F            **fh2PtRecoIso;       //! isolation vs pt of reco muon
   TH3F             *fh3PtProbeIsoPtJet; //!ptprobe vs iso vs ptjet
   TH3F             *fh3ResIsoZlead;     //!res vs iso vs zlead
   TH3F             *fh3PtProbeResIso;   //!ptprobe vs response vs iso
   TH3F             *fh3PtJetResIso;     //!ptjet vs response vs iso
   TH3F             *fh3PtProbeResRawIso; //!ptprobe vs raw response vs iso
   TH3F             *fh3PtJetResRawIso;   //!ptjet vs raw response vs iso
   
   ClassDef(anaHadronIsolation,1)
};
#endif

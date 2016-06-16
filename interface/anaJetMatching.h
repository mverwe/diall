#ifndef anaJetMatching_h
#define anaJetMatching_h

#include "TString.h"
#include "TH2F.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJet.h"

class anaJetMatching : public anaBaseTask {
   
public:
   anaJetMatching() {;}
   anaJetMatching(const char *name, const char *title);
   virtual ~anaJetMatching() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetJetsNameBase(TString name) { fJetsNameBase = name ; }
   void SetJetsNameTag(TString name)  { fJetsNameTag  = name ; }
   void SetNCentBins(Int_t n)         { fNcentBins    = n    ; }
   void SetMatchingType(int t)        { fMatchingType = t    ; }
   void SetMatchId(int i)             { fMatchId      = i    ; }

private:
   void MatchJetsGeo();
   void SetMatchIdForJet(lwJet *jet, int id) {if(fMatchId==1) jet->SetMatchId1(id); else if(fMatchId==2) jet->SetMatchId2(id); };   

 protected:
   TString          fJetsNameBase;    //name of jet container
   lwJetContainer  *fJetsContBase;    //!jet container
   TString          fJetsNameTag;     //name of jet container
   lwJetContainer  *fJetsContTag;     //!jet container
   Int_t            fNcentBins;
   int              fMatchingType;    //0: bijective geo. 1: closest 2: bijective dr/pt
   int              fMatchId;         //1: SetMatchId1. 2: SetMatchId2

   TH2F            **fh2PtJet1VsPtJet2;            //!pT of base jet vs tagged jet
   TH2F            **fh2PtJet2VsRelPt;             //!pT of tagged jet vs pt base jet / pt tagged jet
   TH2F            **fh2PtJet1VsDeltaR;            //!pt jet 1 vs dR
   
   ClassDef(anaJetMatching,1)
};
#endif

#ifndef anaZJetMCResponse_h
#define anaZJetMCResponse_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TClonesArray.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/lwJetContainer.h"

//
// Z-jet MC response analysis
//

class anaZJetMCResponse : public anaBaseTask {
   
public:
   anaZJetMCResponse() {;}
   anaZJetMCResponse(const char *name, const char *title);
   virtual ~anaZJetMCResponse() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetNCentBins(Int_t n)                  { fNcentBins    = n;  }
   void SetZsName(TString name)                { fZsName = name ; }
   void SetJetsName(TString name)              { fJetsName = name ;  }
   void SetGenJetsName(TString name)           { fGenJetsName = name ;  }
   void SetUseForestMatching(bool b)           { fUseForestMatching = b; }
   
 protected:
   Int_t             fNcentBins;              //number of centrality bins
   TString           fZsName;                 //name of Zs
   TClonesArray     *fZs;                     //!Z array
   TString           fJetsName;               //name of jet container
   lwJetContainer   *fJetsCont;               //!jet container
   TString           fGenJetsName;            //name of gen jet container
   lwJetContainer   *fGenJetsCont;            //!gen jet container
   bool              fUseForestMatching; //use matched ref jet from forest
   
   TH1F            **fh1ZPt;                  //!# Z-pt
   TH3F            **fh3ZJetPtRecGen;         //!# Z-pt vs jet pt rec vs jet pt gen
   TH3F            **fh3ZJetXJZRecGen;        //!# Z-pt vs jet ZJZ rec vs jet XJZ gen
   
   ClassDef(anaZJetMCResponse,2)
};
#endif

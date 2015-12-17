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

   void SetZsName(TString name)                { fZsName = name ; }
   void SetJetsName(TString name)              { fJetsName = name ;  }
   
   
 protected:
   TString           fZsName;                 //name of Zs
   TClonesArray     *fZs;                     //!Z array
   TString           fJetsName;               //name of jet container
   lwJetContainer   *fJetsCont;               //!jet container
   
   TH1F             *fh1ZPt;                  //!# Z-pt
   TH3F             *fh3ZJetPtRecGen;         //!# Z-pt vs jet pt rec vs jet pt gen
   TH3F             *fh3ZJetXJZRecGen;        //!# Z-pt vs jet ZJZ rec vs jet XJZ gen
   
   ClassDef(anaZJetMCResponse,1)
};
#endif

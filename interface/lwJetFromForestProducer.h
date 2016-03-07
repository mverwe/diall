#ifndef lwJetFromForestProducer_h
#define lwJetFromForestProducer_h

//
// jet candidates from HI forest producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/lwJetContainer.h>
#include <UserCode/diall/interface/ForestJets.h>

class lwJetFromForestProducer : public inputBase {
 public:
  lwJetFromForestProducer();
  lwJetFromForestProducer(const char *name);
  virtual ~lwJetFromForestProducer() {;}

  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);

  void        SetJetContName(TString n)    { flwJetContName = n ; }
  void        SetGenJetContName(TString n) { flwGenJetContName = n ; }
  void        SetRadius(Double_t r)        { fRadius        = r ; }
  void        SetMinJetPt(double pt)       { fMinJetPt      = pt; }
  void        DoPFJetID(bool b)            { fDoPFJetID = b; }
  
  const char* GetJetContName()    const { return flwJetContName.Data()    ; }
  const char* GetGenJetContName() const { return flwGenJetContName.Data() ; }
  Double_t    GetRadius()         const { return fRadius                  ; }
  
 protected:
  bool                         IsGoodPFJet(int i) const;
  
  TString                      flwJetContName;      // name of jet container
  lwJetContainer              *flwJetContainer;     //!lwJetContainer
  TString                      flwGenJetContName;   // name of gen jet container
  lwJetContainer              *flwGenJetContainer;  //!lwGenJetContainer
  ForestJets                   fForestJets;         //!jets in forest tree
  Double_t                     fRadius;             // the R parameter to use
  double                       fMinJetPt;           // min jet pt
  bool                         fDoPFJetID;          // do PF jet quality selection

 private:
  lwJetFromForestProducer(const lwJetFromForestProducer& obj); // copy constructor
  lwJetFromForestProducer& operator=(const lwJetFromForestProducer& other); // assignment
  
  ClassDef(lwJetFromForestProducer,1)
};
#endif

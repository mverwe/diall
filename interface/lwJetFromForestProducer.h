#ifndef lwJetFromForestProducer_h
#define lwJetFromForestProducer_h

//
// jet candidates from HI forest producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include <CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/lwJetContainer.h>
#include <UserCode/diall/interface/ForestJets.h>

#include "UserCode/diall/interface/hiEventContainer.h"
#include <UserCode/diall/include/residualJetCorrChris.h>

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

  void                   SetDoJEC(bool b)            { fDoJEC         = b; }
  void                   SetL1Fastjet(TString s)     { fL1Fastjet     = s; }
  void                   SetL2Relative(TString s)    { fL2Relative    = s; }
  void                   SetL3Absolute(TString s)    { fL3Absolute    = s; }
  void                   SetL2L3Residual(TString s)  { fL2L3Residual  = s; }

  void SetHiEvtName(TString name)       { fEvtName  = name ; }
  void                   SetDoResidualChris(bool b, residualJetCorrChris obj)  { fDoResidualChris = b; fResidualChris = obj; }
  
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

  bool             fDoJEC;                  //apply JEC to standard jets
  FactorizedJetCorrector *fJetCorrector;    //jet corrector
  TString          fL1Fastjet;
  TString          fL2Relative;
  TString          fL3Absolute;
  TString          fL2L3Residual;

  TString           fEvtName;                  //name of hi event container
  hiEventContainer *fHiEvent;                  //!event container
  bool              fDoResidualChris;          //residual correction from Chris
  //TString          fResidualChrisFile;     //file with residual corrections from Chris
  residualJetCorrChris fResidualChris;     //residual correction object
  std::string      fCorrFormChris;         //correction form
  
 private:
  lwJetFromForestProducer(const lwJetFromForestProducer& obj); // copy constructor
  lwJetFromForestProducer& operator=(const lwJetFromForestProducer& other); // assignment
  
  ClassDef(lwJetFromForestProducer,2)
};
#endif

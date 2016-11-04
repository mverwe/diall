#ifndef LWJetProducer_h
#define LWJetProducer_h

class FJWrapper;

#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <TNamed.h>
#include <TClonesArray.h>

#include <CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h>

#include "UserCode/diall/interface/anaBaseTask.h"
#include <UserCode/diall/interface/FJWrapper.h>
#include <UserCode/diall/interface/lwJetContainer.h>
#include <UserCode/diall/interface/rhoMap.h>

namespace fastjet {
  class PseudoJet;
}

//
// Light-weight jet producer
//

class LWJetProducer : public anaBaseTask {
 public:

  enum JetType {
    kNone=0,
    kKT=1<<0,
    kAKT=1<<1
  };

  typedef boost::shared_ptr<fastjet::ClusterSequence>        ClusterSequencePtr;
  typedef boost::shared_ptr<fastjet::JetDefinition>          JetDefPtr;
  
  LWJetProducer();
  LWJetProducer(const char *name, const char* title);
  virtual ~LWJetProducer() {;}

  void Exec(Option_t *option="");
  void CreateOutputObjects() {;}

  Bool_t Init();
  Int_t  FindJets();
  
  void                   SetParticlesName(TString n) { fConstName     = n ; }
  void                   SetJetContName(TString n)   { flwJetContName = n ; }
  void                   SetCSJetContName(TString n) { flwCSJetContName = n ; }
  
  void                   SetJetType(UInt_t t)        { fJetType       = t ; }
  void                   SetRadius(Double_t r)       { fRadius        = r ; }
  void                   SetRecombScheme(Int_t s)    { fRecombScheme  = s ; }
  void                   SetGhostArea(Double_t gh)   { fGhostArea     = gh; }
  void                   SetPtMinConst(Double_t pt)  { fPtMinConst    = pt; } 
  void                   SetIdConst(int i)           { fIdConst       = i ; }
  void                   SetChargedOnly(bool b)      { fChargedJets   = b ; }
  
  void                   SetDoJEC(bool b)            { fDoJEC         = b; }
  void                   SetDoJECCS(bool b)          { fDoJECCS       = b; }
  void                   SetL1Fastjet(TString s)     { fL1Fastjet     = s; }
  void                   SetL2Relative(TString s)    { fL2Relative    = s; }
  void                   SetL3Absolute(TString s)    { fL3Absolute    = s; }
  void                   SetL2L3Residual(TString s)  { fL2L3Residual  = s; }

  void                   SetDoConstituentSubtraction(Bool_t b) {fDoConstSubtraction = b;}
  void                   SetRhoMapName(TString n)    { fRhoMapName    = n ; }
  void                   SetRhoMMapName(TString n)   { fRhoMMapName   = n ; }
  void                   SetAlpha(double a)          { fAlpha         = a ; }

  void                   SetDoSoftDrop(bool b)       { fDoSoftDrop = b; }
  void                   SetUseKtForSoftDrop(bool b) { fUseKtForSoftDrop = b;}
  void                   SetSoftDropZCut(double zcut){ fSDZcut = zcut   ; }
  void                   SetSoftDropBeta(double beta){ fSDBeta = beta  ; }
 
  UInt_t                 GetJetType()          const { return fJetType         ; }
  Double_t               GetRadius()           const { return fRadius          ; }
  Int_t                  GetRecombScheme()     const { return fRecombScheme    ; }
  lwJetContainer        *GetJetContainer()     const { return flwJetContainer  ; }
    

 protected:
  Bool_t           GetSortedArray(Int_t indexes[], std::vector<fastjet::PseudoJet> array) const;
  
  UInt_t           fJetType;                // jet type (algorithm)
  Double_t         fRadius;                 // the R parameter to use
  Int_t            fRecombScheme;           // recombination scheme used by fastjet
  Double_t         fGhostArea;              // ghost area
  Double_t         fPtMinConst;             // minimum pT of input constituents
  int              fIdConst;                // id of constituents
  bool             fChargedJets;            // use only charged particles for jet reco
  
  std::vector<fastjet::PseudoJet> fjInputs; // fastjet inputs
  std::vector<fastjet::PseudoJet> fjJets;   // fastjet reconstructed jets

  Bool_t           fIsInit;                 //!=true if already initialized
  TClonesArray    *fConst;                  //! container with jet constituents
  TString          fConstName;              // name of constituents
  lwJetContainer  *flwJetContainer;         //!lwJetContainer
  TString          flwJetContName;          // name of jet container
  FJWrapper        fFastJetWrapper;         //!fastjet wrapper

  Bool_t           fDoConstSubtraction;     // do constituent subtraction
  TString          fRhoMapName;             //rho map name
  rhoMap          *fRhoMap;                 //!rho map
  TString          fRhoMMapName;            //rhom map name
  rhoMap          *fRhoMMap;                //!rhom map
  lwJetContainer  *flwCSJetContainer;       //!lwJetContainer for constituent subtracted jets
  TString          flwCSJetContName;        // name of constituent subtracted jet container
  double           fAlpha;                  // power of pt in constituent subtraction metric

  bool             fDoSoftDrop;             // run softdrop
  bool             fUseKtForSoftDrop;       // use kt instead of C/A for reclustering
  double           fSDZcut;                 // zcut for SoftDrop
  double           fSDBeta;                 // beta for SoftDrop
  lwJetContainer  *flwSDJetContainer;       //!lwJetContainer for SoftDrop jets

  bool             fDoJEC;                  //apply JEC to standard jets
  bool             fDoJECCS;                //apply JEC to constituent subtracted jets
  FactorizedJetCorrector *fJetCorrector;    //jet corrector
  TString          fL1Fastjet;
  TString          fL2Relative;
  TString          fL3Absolute;
  TString          fL2L3Residual; 
 
 private:
  LWJetProducer(const LWJetProducer& obj); // copy constructor
  LWJetProducer& operator=(const LWJetProducer& other); // assignment
  
  ClassDef(LWJetProducer,6)

};
#endif

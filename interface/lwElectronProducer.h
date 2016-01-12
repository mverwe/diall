#ifndef lwElectronProducer_h
#define lwElectronProducer_h

//
// muon candidate producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/ForestElectrons.h>

class lwElectronProducer : public inputBase {
 public:
  lwElectronProducer();
  lwElectronProducer(const char *name);
  virtual ~lwElectronProducer() {;}

  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetlwElectronsRecoName(TString n) { flwElectronsRecoName = n; }
  void     SetlwElectronsGeneName(TString n) { flwElectronsGeneName = n; }

  const char* GetlwElectronsRecoName() const { return flwElectronsRecoName.Data() ; }
  const char* GetlwElectronsGeneName() const { return flwElectronsGeneName.Data() ; }
  
 protected:
  Bool_t   AcceptElectron(Int_t i);
  
  TString                      flwElectronsRecoName; // name of reco muons
  TClonesArray                *flwElectronsReco;     //!reco muons
  TString                      flwElectronsGeneName; // name of gene muons
  TClonesArray                *flwElectronsGene;     //!gene muons
  ForestElectrons              fElectrons;           //! Electrons in forest tree
  Float_t                      fPtMin;               // minimum pT

 private:
  lwElectronProducer(const lwElectronProducer& obj); // copy constructor
  lwElectronProducer& operator=(const lwElectronProducer& other); // assignment
  
  ClassDef(lwElectronProducer,1)
};
#endif

#ifndef hiEventProducer_h
#define hiEventProducer_h

//
// heavy-ion event container producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TBranch.h>

#include "UserCode/diall/interface/inputBase.h"
#include "UserCode/diall/interface/hiEventContainer.h"
#include <UserCode/diall/interface/ForestFJRho.h>
#include "UserCode/diall/interface/rhoMap.h"

class hiEventProducer : public inputBase {
 public:
  hiEventProducer();
  hiEventProducer(const char *name);
  virtual ~hiEventProducer() {;}

  void     SetInput(TChain *chain);
  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetHIEventContName(TString n) { fhiEventContName = n; }

  const char* GetHIEventContName() const { return fhiEventContName.Data() ; }
  
 protected:
  TString                      fhiEventContName;  // name of HI event container
  hiEventContainer            *fhiEventContainer; //!HI event properties
  UInt_t                       fRun;     //!
  ULong64_t                    fEvt;     //!
  UInt_t                       fLumi;    //!
  Float_t                      fWeight;  //!
  Float_t                      fVx;      //!
  Float_t                      fVy;      //!
  Float_t                      fVz;      //!
  Int_t                        fHiBin;   //
  Float_t                      fHiHF;    //!
  Int_t                        fNPV;     //!
  Int_t                        fHLT_AK4PFJet80_Eta5p1_v1; //!
  Int_t                        fHLT_HISinglePhoton30_Eta3p5_v1; //!
  Int_t                        fHLT_FullTrack24ForPPRef_v1; //!
  Int_t                        fHLT_FullTrack45ForPPRef_v1; //!

  Int_t                        fcollisionEvtSel; //!
  Int_t                        fHBHENoise; //!
  Int_t                        fHBHENoiseLoose; //!
  Int_t                        fPrimaryVertexFilter; //!
  Int_t                        fClusterCompatibilityFilter; //!
  Int_t                        fHFCoincFilter;//!
  
  ForestFJRho                  fForestFJRho; //!rho in forest tree
  rhoMap                       *fRhoMap;     //rho mapping

  TBranch        *b_run;    //!
  TBranch        *b_evt;    //!
  TBranch        *b_lumi;   //!
  TBranch        *b_weight; //!
  TBranch        *b_vx;     //!
  TBranch        *b_vy;     //!
  TBranch        *b_vz;     //!
  TBranch        *b_hiBin;  //!
  TBranch        *b_hiHF;   //!
  TBranch        *b_nPU;    //!
  TBranch        *b_HLT_AK4PFJet80_Eta5p1_v1; //!
  TBranch        *b_HLT_HISinglePhoton30_Eta3p5_v1 ; //!
  TBranch        *b_HLT_FullTrack24ForPPRef_v1; //!
  TBranch        *b_HLT_FullTrack45ForPPRef_v1; //!
  TBranch        *b_collisionEvtSel;
  TBranch        *b_HBHENoise;
  TBranch        *b_HBHENoiseLoose;
  TBranch        *b_PrimaryVertexFilter;
  TBranch        *b_ClusterCompatibilityFilter;
  TBranch        *b_phfCoincFilter;
  
 private:
  hiEventProducer(const hiEventProducer& obj); // copy constructor
  hiEventProducer& operator=(const hiEventProducer& other); // assignment
  
  ClassDef(hiEventProducer,2)
};
#endif

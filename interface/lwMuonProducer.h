#ifndef lwMuonProducer_h
#define lwMuonProducer_h

//
// muon candidate producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/ForestMuons.h>

class lwMuonProducer : public inputBase {
 public:
  lwMuonProducer();
  lwMuonProducer(const char *name);
  virtual ~lwMuonProducer() {;}

  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetlwMuonsRecoName(TString n) { flwMuonsRecoName = n; }
  void     SetlwMuonsGeneName(TString n) { flwMuonsGeneName = n; }
  void     SetEtaCut(float eta) {fMaxEtaAbs = eta; }
  void     SetPtCut(float pt) {fPtMin = pt; }
  void     SetTrkChi2Cut(float cut) {fMaxTrkChi2 = cut; }
  void     SetGlbChi2Cut(float cut) {fMaxGlbChi2 = cut; }
  void     SetNMuHitsCut(float cut) {fMinNMuHits = cut; }
  void     SetMatchedStationsCut(float cut) {fMinMS = cut; }
  void     SetDxyCut(float cut) {fMaxDxy = cut; }
  void     SetDzCut(float cut) {fMaxDz = cut; }
  void     SetTrkDxyCut(float cut) {fMaxTrkDxy = cut; }
  void     SetTrkDzCut(float cut) {fMaxTrkDz = cut; }
  void     SetNPixHitsCut(float cut) {fMinNPixHits = cut; }
  void     SetTrkLWMCut(float cut) {fMinTrkLWM = cut; }
  
  const char* GetlwMuonsRecoName() const { return flwMuonsRecoName.Data() ; }
  const char* GetlwMuonsGeneName() const { return flwMuonsGeneName.Data() ; }
  
 protected:
  Bool_t   AcceptMuon(Int_t i);
  
  TString                      flwMuonsRecoName;// name of reco muons
  TClonesArray                *flwMuonsReco;    //!reco muons
  TString                      flwMuonsGeneName;// name of gene muons
  TClonesArray                *flwMuonsGene;    //!gene muons
  ForestMuons                  fMuons;          //! Muons in forest tree
  Float_t                      fPtMin;          // minimum pT
  Float_t                      fMaxEtaAbs;      // max eta
  Float_t                      fMaxTrkChi2;     // max chi2
  Float_t                      fMaxGlbChi2;     // max chi2
  Int_t                        fMinNMuHits;     // min muon hits
  Int_t                        fMinMS;          // #matched stations
  Float_t                      fMaxDxy;         // max dxy
  Float_t                      fMaxDz;          // max dz
  Float_t                      fMaxTrkDxy;      // max innerTrack dxy
  Float_t                      fMaxTrkDz;       // max innerTrack dz
  Int_t                        fMinNPixHits;    // min pixel hits
  Int_t                        fMinTrkLWM;      // min tracker layer hits

 private:
  lwMuonProducer(const lwMuonProducer& obj); // copy constructor
  lwMuonProducer& operator=(const lwMuonProducer& other); // assignment
  
  ClassDef(lwMuonProducer,1)
};
#endif

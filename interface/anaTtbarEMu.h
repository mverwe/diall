#ifndef anaTtbarEMu_h
#define anaTtbarEMu_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TClonesArray.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/triggerMap.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/diParticle.h"

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooVoigtian.h>
#include <RooHistPdf.h>
#include <RooFormulaVar.h>

#include <utility>
#include <vector>

class anaTtbarEMu : public anaBaseTask {
   
public:
  enum metType {
    kGen   = 0,
    kGenEm = 1,
    kPFRaw = 2,
    kVS    = 3,
    kPuppi = 4,
    kCS    = 5
  };
  
   anaTtbarEMu() {;}
   anaTtbarEMu(const char *name, const char *title);
   virtual ~anaTtbarEMu() {;}
   void Exec(Option_t *option="");
   void ConstructModel(RooDataHist Hist, RooDataHist *bkg_hist, bool BKGSubtract);
   std::pair<double, double> compMETProjU(diParticle* zP4, double metPx, double metPy, int& errorFlag, int count);
   std::pair<double, double> compHadronicRecoilProjU(diParticle* zP4, TLorentzVector MET, int& errorFlag, int count);
   void CreateOutputObjects();
   double FWHM (double sigma, double gamma);
   double FWHMError (double sigma, double gamma, double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg);
   double FWHMError_fixed (double sigma, double gamma, double esigma, double egamma, double Vss, double Vsg, double Vgs, double Vgg);
   TString NToString(Float_t type);
   void SetCheckPid(Bool_t b)          { fCheckPid = b; }
   void SetMetType(metType t)          { fMetType = t; }
   void SetMinPt(Float_t m)            { fMinPt = m; }
   void SetTriggerMapName(TString name){ fTriggerMapName = name ; }
   void SetElectronsName(TString name) { fElectronsName = name ; }
   void SetMuonsName(TString name)     { fMuonsName = name ; }
   void SetGenJetsName(TString name)   { fGenJetsName = name ; }     
   void SetRecoJetsName(TString name)  { fRecoJetsName = name ; } 
   void SetParticlesName(TString name) { fParticlesName = name ; }
   
 protected:
   Bool_t            CheckPid(particleBase *p);

   Bool_t            fCheckPid;             //check if candidates are really muons (for simulation)
   metType           fMetType;              //matching type (defines where to store)
   Float_t           fMinPt;                //minimum pT of particles
   TString           fTriggerMapName;        //name of electrons
   TString           fElectronsName;        //name of electrons
   TString           fMuonsName;            //name of muons
   TString           fGenJetsName;          //name of jet container
   TString           fRecoJetsName;         //name of jet container
   TString           fParticlesName;        //name of pfParticles
   TString           fEMusName;             // name of emu pair candidates
   TString           fTopsName;             // name of top quark candidates
   triggerMap       *fTriggerMap;           //! trigger map                                                                                                                                         
   TClonesArray     *fElectrons;            //!electron arr
   TClonesArray     *fMuons;                //!muon array
   lwJetContainer   *fGenJets;              //!jet container
   lwJetContainer   *fRecoJets;             //!jet container
   TClonesArray     *fParticles;            //!pfParticle array
   TClonesArray     *fEMus;                 //!Electron Muon candidates 
   TClonesArray     *fTops;                 //!Top quark candidates container

   Int_t            fEMuCands;
   TH1F             *fh1NEvents;            //!# selected events
   TTree            *fAnaTtbarEMuInfo;
   std::vector<Int_t> fh1NElectrons;         //!# selected electrons in event
   std::vector<Int_t> fh1NMuons;             //!# selected muons in event
   std::vector<Int_t> fh1NJetsIncl;          //!# selected jets in event
   std::vector<Int_t> fh1NBJetsIncl;         //!# selected b-tagged jets in event
   
   std::vector<Float_t> fh1HT;                 //!# HT varialbe
   std::vector<Float_t>fh1MassEMu;            //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadJetPt;          //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonPt;       //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonAbsEta;   //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonEta;      //!# dilepton mass varialbe
   std::vector<Int_t>fh1NJets;              //!# dilepton mass varialbe

   
   std::vector<Double_t>fh1DeltaPhi_Rec2jets;  //!# HT varialbe
   std::vector<Float_t>fh1MassEMu_Rec2jets;   //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadJetPt_Rec2jets;          //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadJetAbsEta_Rec2jets;          //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadJetEta_Rec2jets;          //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonPt_Rec2jets;       //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonAbsEta_Rec2jets;   //!# dilepton mass varialbe
   std::vector<Float_t>fh1LeadLeptonEta_Rec2jets;      //!# dilepton mass varialbe

   TH2F             *fh2MetCentPtMin[10];   //!MET vs centrality for various min pt cuts
   
   //ClassDef(anaTtbarEMu,2)
};
#endif

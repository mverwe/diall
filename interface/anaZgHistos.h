#ifndef anaZgHistos_h
#define anaZgHistos_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "THnSparse.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/jewelZgReweight.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/subjetSmearing.h"
#include "UserCode/diall/interface/subjetSmearingResolution.h"

class anaZgHistos : public anaBaseTask {
   
public:
   anaZgHistos() {;}
   anaZgHistos(const char *name, const char *title);
   virtual ~anaZgHistos() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetNCentBins(Int_t n)             { fNcentBins    = n;  }
   void SetJetsName(TString name)         { fJetsName = name ; }
   void SetJetsRefName(TString name)         { fJetsRefName = name ; }
   void SetJetEtaRange(float min, float max) { fJetEtaMin = min; fJetEtaMax = max; }
   void SetDeltaRRange(float min, float max) { fMinDeltaR = min; fMaxDeltaR = max; }
   
   void SetMinRefPt(float min)            { fMinRefPt = min ; }
   void SetUseRhoMCWeight(bool b)         { fUseRhoMCWeight = b; }
   void SetRhoMax(float max)              { fRhoMax = max; }

   void SetNotUseRef(bool b)              { fNotUseRef = b; }

   void DoSubjetSmearing(bool b)          { fDoSubjetSmearing = b; }
   void SetSubjetSmearing(subjetSmearing sm, int icent) {if(icent<100) fSubjetSmearing[icent] = sm;}
   void DoSubjetSmearingResolution(bool b)          { fDoSubjetSmearingResolution = b; }
   void SetSubjetSmearingResolution(subjetSmearingResolution sm, int icent) {if(icent<100) fSubjetSmearingResolution[icent] = sm;}

   void DoJetShift(bool b, float shift)   { fDoJetShift = b; fJetShift = shift; }

   void SetZgReweight(bool b, TF1 *f1)    { fDoZgReweight = b; f1ZgReweight = f1; }
   void SetZgReweightMulti(bool b, jewelZgReweight rw) { fDoJewelZgReweight = b; fJewelZgReweight = rw; }
 
 protected:
   double DeltaR(double phi1, double phi2, double eta1, double eta2);
   
   int              fNcentBins;
   TString          fJetsName;    //name of SD jet container
   lwJetContainer  *fJetsCont;    //!SD jet container
   TString          fJetsRefName;    //name of ref jet container (ungroomed)
   lwJetContainer  *fJetsRefCont;    //!ref jet container (ungroomed)
   float            fJetEtaMin;   //min jet eta
   float            fJetEtaMax;   //max jet eta
   float            fMinDeltaR;   //min distance between subjets
   float            fMaxDeltaR;   //max distance between subjets
   float            fMinRefPt;    //min pt of corresponding true jet: reject fakes
   bool             fUseRhoMCWeight; //flag to activate rho mc weighting
   float            fRhoMax;      // max rho for event selection

   bool             fNotUseRef;   //used matched jet as ref info
   
   bool             fDoSubjetSmearing;    //flag to activate subjet smearing
   subjetSmearing   fSubjetSmearing[100]; //subjet resolution smearing module

   bool                     fDoSubjetSmearingResolution;    //flag to activate subjet smearing resolution
   subjetSmearingResolution fSubjetSmearingResolution[100]; //subjet resolution smearing module

   bool             fDoJetShift; //shift jet pt with X%
   float            fJetShift;   //fraction to shift jet pt

   bool             fDoZgReweight; //flag to activate zg reweighting
   TF1             *f1ZgReweight; //!reweight function based on pp data/mc ratio

   bool            fDoJewelZgReweight; //flag  
   jewelZgReweight fJewelZgReweight; //jewel true zg reweight factors
 
   TH1F            *fhCentrality;  //!centrality
   TH2F            *fh2RhoCent;         //!rho_central vs cent
   
   TH3F           **fh3PtEtaPhi;   //!jet pt vs eta vs phi
   TH2F           **fh2PtNSubjets; //!jet pt vs number of subjets
   TH2F           **fh2PtGenNSubjets; //!jet gen pt vs number of subjets
   TH2F           **fh2PtMass;     //!jet pt vs mass
   TH2F           **fh2PtPtGF;     //!pt jet vs ptg/pt  
   TH2F           **fh2PtNDrop;    //!pt jet vs number of dropped branches
 
   TH2F           **fh2PtSubjetPtRatio21; //!jet pt vs subjetratio 2/1
  
   TH2F           **fh2PtSubjetPtFrac1; //!jet pt vs subjetratio 1/jet
   TH2F           **fh2PtSubjetPtFrac2; //!jet pt vs subjetratio 2/jet

   TH2F           **fh2PtSubjetPtInvMass21; //!jet pt vs subjetratio 2/1

   TH2F           **fh2PtZg; //!jet pt vs zg
   TH2F           **fh2PtZgDRNoPass; //!jet pt vs zg for jets which fail DR cut
   TH2F           **fh2PtZgAll; //!jet pt vs zg all jets (including zg<zcut)
   TH2F           **fh2PtZgTrue; //!jet pt vs zg for true jets
   TH2F           **fh2PtZgNoRef; //!jet pt vs zg for jets without MC match
   TH3F           **fh3PtRecPtTrueZg; //!jet pt reco vs pt true vs zg
   TH3F           **fh3PtTrueDeltaPtDeltaZg; //!jet pt true vs deltaPt vs deltaZg
   
   TH3F           **fh3PtTruePtSJScalePtSJ;  //!jet pt true vs pt subjet vs scale pt subjet
   TH3F           **fh3PtTruePtLSJScalePtLSJ;  //!jet pt true vs pt leading subjet vs scale pt leading subjet
   TH3F           **fh3PtTruePtSLSJScalePtSLSJ;  //!jet pt true vs pt subleading subjet vs scale pt subleading subjet

   TH3F           **fh3PtTruePtRecoSJScalePtSJ;  //!jet pt true vs pt subjet vs scale pt subjet
   TH3F           **fh3PtTruePtRecoLSJScalePtLSJ;  //!jet pt true vs pt leading subjet vs scale pt leading subjet
   TH3F           **fh3PtTruePtRecoSLSJScalePtSLSJ;  //!jet pt true vs pt subleading subjet vs scale pt subleading subjet
   
   TH2F           **fh2PtThetag; //!jet pt vs zg
   TH2F           **fh2PtThetagTrue; //!jet pt vs zg for true jets
   TH2F           **fh2PtThetagNoRef; //!jet pt vs zg for jets without MC match
   TH3F           **fh3PtRecPtTrueThetag; //!jet pt reco vs pt true vs thetag

   TH2F           **fh2PtDeltaR12; //!jet pt vs DR12
   TH2F           **fh2PtDeltaR12True; //!jet pt vs DR12 for true jets
   TH2F           **fh2PtDeltaR12NoRef; //!jet pt vs DR12 for jets without MC match
   TH3F           **fh3PtRecPtTrueDeltaR12; //!jet pt reco vs pt true vs DR12

   TH2F           **fh2PtMgOverPt; //!jet pt vs zg
   TH2F           **fh2PtMgOverPtTrue; //!jet pt vs zg for true jets
   TH2F           **fh2PtMgOverPtNoRef; //!jet pt vs zg for jets without MC match
   TH3F           **fh3PtRecPtTrueMgOverPt; //!jet pt reco vs pt true vs thetag

   TH3F           **fh3PtZgThetag; //!jet pt vs zg vs thetag
   
   TH3F           **fh3PtTrueZgScaleZg; //! jet pt true vs zg true vs zgRec/zgTrue

   THnSparse      **fhnZgResponse;                  //!response matrix
   
   ClassDef(anaZgHistos,3)
};
#endif

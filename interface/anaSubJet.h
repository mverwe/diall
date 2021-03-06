#ifndef anaSubJet_h
#define anaSubJet_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "THnSparse.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"

class anaSubJet : public anaBaseTask {
   
public:
   anaSubJet() {;}
   anaSubJet(const char *name, const char *title);
   virtual ~anaSubJet() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetNCentBins(Int_t n)             { fNcentBins    = n;  }
   void SetJetsName(TString name)         { fJetsName = name ; }
   void SetJetsRefName(TString name)         { fJetsRefName = name ; }
   void SetJets000Name(TString name)         { fJets000Name = name ; } 
   void SetJetEtaRange(float min, float max) { fJetEtaMin = min; fJetEtaMax = max; }
   
   void SetMinRefPt(float min)            { fMinRefPt = min ; }

   void SetDoDijets(bool b)               { fDoDijets = b; }
   void AddLeadingJetPtBin(float min, float max) {fPtLeadingMin.push_back(min); fPtLeadingMax.push_back(max); }
   void SetPtMinSubleading(float min)     { fPtSubleadingMin = min; }
   void SetMinDPhi(float dphi)            { fMinDPhi = dphi; }
   void SetMinMassLeading(float m)        { fMinMassLeading = m; }

   void SetStoreTree(bool b)              { fStoreTree = b;     }
   void SetStoreTreeRef(bool b)           { fStoreTreeRef = b;  }
   void SetMinPtJetTree(float min)        { fMinPtJetTree = min ; }
   void ClearSubjetTreeVars();
   
 protected:
   double DeltaR(double phi1, double phi2, double eta1, double eta2);
   
   int              fNcentBins;
   TString          fJetsName;    //name of SD jet container
   lwJetContainer  *fJetsCont;    //!SD jet container
   TString          fJetsRefName;    //name of ref jet container (ungroomed)
   lwJetContainer  *fJetsRefCont;    //!ref jet container (ungroomed)
   TString          fJets000Name;    //name of ref jet container (zcut=0)
   lwJetContainer  *fJets000Cont;    //!ref jet container (zcut=0)
   float            fJetEtaMin;   //min jet eta
   float            fJetEtaMax;   //max jet eta
   float            fMinRefPt;    //min pt of corresponding true jet: reject fakes
   bool             fDoDijets;    //do dijet analysis

   std::vector<float> fPtLeadingMin; //vector with lower bin edges
   std::vector<float> fPtLeadingMax; //vector with upper bin edges
   float            fPtSubleadingMin; //min pt for subleading jet
   float            fMinDPhi;        //minimum delta-phi_1,2
   float            fMinMassLeading; //minimum mass for leading jet

   TH1F            *fhCentrality;  //!centrality
   
   TH3F           **fh3PtEtaPhi;   //!jet pt vs eta vs phi
   TH2F           **fh2PtNSubjets; //!jet pt vs number of subjets
   TH2F           **fh2PtGenNSubjets; //!jet gen pt vs number of subjets
   TH2F           **fh2PtMass;     //!jet pt vs mass
   
   TH2F           **fh2PtSubjetPtRatio21; //!jet pt vs subjetratio 2/1
   TH2F           **fh2PtSubjetPtRatio32; //!jet pt vs subjetratio 3/2
   TH2F           **fh2PtSubjetPtRatio43; //!jet pt vs subjetratio 4/3
   TH2F           **fh2PtSubjetPtRatio54; //!jet pt vs subjetratio 5/4

   TH2F           **fh2PtSubjetPtFrac1; //!jet pt vs subjetratio 1/jet
   TH2F           **fh2PtSubjetPtFrac2; //!jet pt vs subjetratio 2/jet
   TH2F           **fh2PtSubjetPtFrac3; //!jet pt vs subjetratio 3/jet
   TH2F           **fh2PtSubjetPtFrac4; //!jet pt vs subjetratio 4/jet

   TH2F           **fh2PtSubjetPtInvMass21; //!jet pt vs subjetratio 2/1
   TH2F           **fh2PtSubjetPtInvMass32; //!jet pt vs subjetratio 3/2
   TH2F           **fh2PtSubjetPtInvMass43; //!jet pt vs subjetratio 4/3
   TH2F           **fh2PtSubjetPtInvMass54; //!jet pt vs subjetratio 5/4

   TH2F           **fh2PtZg; //!jet pt vs zg
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

   THnSparse      **fhnZgResponse;                  //!response matrix
   
   //dijet analysis histos
   //subjet observables in leading, subleading jet

   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtRatio21;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtRatio32;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtRatio43;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtRatio54;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtInvMass21;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtInvMass32;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtInvMass43;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtInvMass54;
   std::vector<std::vector<TH2F*>> fh2SLPtZg;
   std::vector<std::vector<TH2F*>> fh2SLPtZgTrue; //!jets with mc match
   std::vector<std::vector<TH2F*>> fh2SLPtZgNoRef; //!jets without mc match
   std::vector<std::vector<TH2F*>> fh2SLPtThetag;
   std::vector<std::vector<TH2F*>> fh2SLPtThetagTrue; //!jets with mc match
   std::vector<std::vector<TH2F*>> fh2SLPtThetagNoRef; //!jets without mc match
   std::vector<std::vector<TH2F*>> fh2SLPtDeltaPhi; //!DPhi between leading and subleading jet
   std::vector<std::vector<TH2F*>> fh2SLPtDeltaPhiTrue; //!DPhi between leading and subleading jet
   std::vector<std::vector<TH2F*>> fh2SLPtDeltaPhiNoRef; //!DPhi between leading and subleading jet
   std::vector<std::vector<TH3F*>> fh3SLPtZgDeltaPhi;
   
   bool               fStoreTree;
   bool               fStoreTreeRef;
   TTree             *fTreeOut;
   float              fMinPtJetTree;
   
   struct subjetTreeVars {
     int                fRun;
     float              fCent;
     std::vector<float> fPt;
     std::vector<float> fPtRaw;
     std::vector<float> fEta;
     std::vector<float> fPhi;
     std::vector<float> fM;
     std::vector<float> fPtG;
     std::vector<float> fPtRawG;
     std::vector<float> fEtaG;
     std::vector<float> fPhiG;
     std::vector<float> fMG;
     std::vector<float> fPtSJ1;
     std::vector<float> fPtSJ2;
     std::vector<float> fEtaSJ1;
     std::vector<float> fEtaSJ2;
     std::vector<float> fPhiSJ1;
     std::vector<float> fPhiSJ2;
     std::vector<float> fMSJ1;
     std::vector<float> fMSJ2;
     std::vector<float> fZg;
     std::vector<float> fThetag;
     std::vector<int>   fDropped;
     std::vector<float> fCHF;
     std::vector<float> fNHF;
     std::vector<float> fCEF;
     std::vector<float> fNEF;
     std::vector<float> fMUF;
     std::vector<float> fCHM;
     std::vector<float> fNHM;
     std::vector<float> fCEM;
     std::vector<float> fNEM;
     std::vector<float> fMUM;
     std::vector<float> fPtRef;
     std::vector<float> fEtaRef;
     std::vector<float> fPhiRef;
     std::vector<float> fPtGRef;
     std::vector<float> fEtaGRef;
     std::vector<float> fPhiGRef;
     std::vector<float> fPtSJ1Ref;
     std::vector<float> fPtSJ2Ref;
     std::vector<float> fEtaSJ1Ref;
     std::vector<float> fEtaSJ2Ref;
     std::vector<float> fPhiSJ1Ref;
     std::vector<float> fPhiSJ2Ref;
     std::vector<float> fMSJ1Ref;
     std::vector<float> fMSJ2Ref;
     std::vector<float> fZgRef;
     std::vector<float> fThetagRef;
     std::vector<int>   fDroppedRef;
     std::vector<bool>  fSwap;
     std::vector<float> fDeltaRSJ11;
     std::vector<float> fDeltaRSJ12;
     std::vector<float> fDeltaRSJ21;
     std::vector<float> fDeltaRSJ22;

     //zcut=0 info
     std::vector<float> fZCut0PtG;
     std::vector<float> fZCut0PtRawG;
     std::vector<float> fZCut0EtaG;
     std::vector<float> fZCut0PhiG;
     std::vector<float> fZCut0MG;
     std::vector<float> fZCut0PtSJ1;
     std::vector<float> fZCut0PtSJ2;
     std::vector<float> fZCut0EtaSJ1;
     std::vector<float> fZCut0EtaSJ2;
     std::vector<float> fZCut0PhiSJ1;
     std::vector<float> fZCut0PhiSJ2;
     std::vector<float> fZCut0MSJ1;
     std::vector<float> fZCut0MSJ2;
     std::vector<float> fZCut0Zg;
     std::vector<float> fZCut0Thetag;

     std::vector<float> fZCut0PtGRef;
     std::vector<float> fZCut0PtRawGRef;
     std::vector<float> fZCut0EtaGRef;
     std::vector<float> fZCut0PhiGRef;
     std::vector<float> fZCut0MGRef;
     std::vector<float> fZCut0PtSJ1Ref;
     std::vector<float> fZCut0PtSJ2Ref;
     std::vector<float> fZCut0EtaSJ1Ref;
     std::vector<float> fZCut0EtaSJ2Ref;
     std::vector<float> fZCut0PhiSJ1Ref;
     std::vector<float> fZCut0PhiSJ2Ref;
     std::vector<float> fZCut0MSJ1Ref;
     std::vector<float> fZCut0MSJ2Ref;
     std::vector<float> fZCut0ZgRef;
     std::vector<float> fZCut0ThetagRef;
   };

   subjetTreeVars fSubjetTreeVars;
   
   ClassDef(anaSubJet,2)
};
#endif

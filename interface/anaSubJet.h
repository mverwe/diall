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
   void SetJetEtaRange(float min, float max) { fJetEtaMin = min; fJetEtaMax = max; }
   
   void SetMinRefPt(float min)            { fMinRefPt = min ; }

   void SetDoDijets(bool b)               { fDoDijets = b; }
   void AddLeadingJetPtBin(float min, float max) {fPtLeadingMin.push_back(min); fPtLeadingMax.push_back(max); }
   void SetPtMinSubleading(float min)     { fPtSubleadingMin = min; }
   void SetMinDPhi(float dphi)            { fMinDPhi = dphi; }
   void SetMinMassLeading(float m)        { fMinMassLeading = m; }
   
 protected:
   int              fNcentBins;
   TString          fJetsName;    //name of SD jet container
   lwJetContainer  *fJetsCont;    //!SD jet container
   TString          fJetsRefName;    //name of ref jet container (ungroomed)
   lwJetContainer  *fJetsRefCont;    //!ref jet container (ungroomed)
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
   TTree             *fTreeOut;

   struct subjetTreeVars {
     std::vector<float> fPtLead;
     std::vector<float> fEtaLead;
     std::vector<float> fPhiLead;
     std::vector<float> fMLead;
     std::vector<float> fPtSubLead;
     std::vector<float> fEtaSubLead;
     std::vector<float> fPhiSubLead;
     std::vector<float> fMSubLead;
   };

   subjetTreeVars fSubjetTreeVars;
   
   ClassDef(anaSubJet,2)
};
#endif

#ifndef anaDijetAj_h
#define anaDijetAj_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"

class anaDijetAj : public anaBaseTask {
   
public:
   anaDijetAj() {;}
   anaDijetAj(const char *name, const char *title);
   virtual ~anaDijetAj() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetNCentBins(Int_t n)             { fNcentBins    = n;  }
   void SetJetsName(TString name)         { fJetsName = name ; } 
   void SetJetEtaRange(float min, float max) { fJetEtaMin = min; fJetEtaMax = max; }
   
   void SetMinRefPt(float min)            { fMinRefPt = min ; }

   void SetDoDijets(bool b)               { fDoDijets = b; }
   void AddLeadingJetPtBin(float min, float max) {fPtLeadingMin.push_back(min); fPtLeadingMax.push_back(max); }
   void SetPtMinSubleading(float min)     { fPtSubleadingMin = min; }
   void SetMinDPhi(float dphi)            { fMinDPhi = dphi; }
   void SetMinMassLeading(float m)        { fMinMassLeading = m; }
   
 protected:
   int              fNcentBins;
   TString          fJetsName;    //name of jet container
   lwJetContainer  *fJetsCont;    //!jet container
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
   TH2F           **fh2PtMass;     //!jet pt vs mass
   
   //dijet analysis histos
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtAj;
   std::vector<std::vector<TH2F*>> fh2SLPtSubjetPtRatio;
      
   ClassDef(anaDijetAj,1)
};
#endif

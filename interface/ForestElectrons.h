#ifndef ForestElectrons_h
#define ForestElectrons_h

#define maxForestElectrons 20

#include <iostream>
#include <vector>
#include "TBranch.h"

class ForestElectrons {
public :
   ForestElectrons(){};
   ~ForestElectrons(){};

   Int_t           nEle;
   std::vector<int>     *eleCharge;
   std::vector<int>     *eleChargeConsistent;
   std::vector<float>   *eleEn;
   std::vector<float>   *eleD0;
   std::vector<float>   *eleDz;
   std::vector<float>   *eleD0Err;
   std::vector<float>   *eleDzErr;
   std::vector<float>   *eleTrkPt;
   std::vector<float>   *eleTrkEta;
   std::vector<float>   *eleTrkPhi;
   std::vector<int>     *eleTrkCharge;
   std::vector<float>   *eleTrkChi2;
   std::vector<float>   *eleTrkNdof;
   std::vector<float>   *eleTrkNormalizedChi2;
   std::vector<float>   *elePt;
   std::vector<float>   *eleEta;
   std::vector<float>   *elePhi;
   std::vector<float>   *eleSCEn;
   std::vector<float>   *eleESEn;
   std::vector<float>   *eleSCEta;
   std::vector<float>   *eleSCPhi;
   std::vector<float>   *eleSCRawEn;
   std::vector<float>   *eleSCEtaWidth;
   std::vector<float>   *eleSCPhiWidth;
   std::vector<float>   *eleHoverE;
   std::vector<float>   *eleEoverP;
   std::vector<float>   *eleEoverPInv;
   std::vector<float>   *eleBrem;
   std::vector<float>   *eledEtaAtVtx;
   std::vector<float>   *eledPhiAtVtx;
   std::vector<float>   *eleSigmaIEtaIEta;
   std::vector<float>   *eleSigmaIEtaIEta_2012;
   std::vector<float>   *eleSigmaIPhiIPhi;
   std::vector<int>     *eleMissHits;
   std::vector<float>   *eleESEffSigmaRR;
   std::vector<float>   *elePFChIso;
   std::vector<float>   *elePFPhoIso;
   std::vector<float>   *elePFNeuIso;
   std::vector<float>   *elePFPUIso;
   std::vector<float>   *eleBC1E;
   std::vector<float>   *eleBC1Eta;
   std::vector<float>   *eleBC2E;
   std::vector<float>   *eleBC2Eta;

   TBranch        *b_nEle;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_eleChargeConsistent;   //!
   TBranch        *b_eleEn;   //!
   TBranch        *b_eleD0;   //!
   TBranch        *b_eleDz;   //!
   TBranch        *b_eleD0Err;   //!
   TBranch        *b_eleDzErr;   //!
   TBranch        *b_eleTrkPt;   //!
   TBranch        *b_eleTrkEta;   //!
   TBranch        *b_eleTrkPhi;   //!
   TBranch        *b_eleTrkCharge;   //!
   TBranch        *b_eleTrkChi2;   //!
   TBranch        *b_eleTrkNdof;   //!
   TBranch        *b_eleTrkNormalizedChi2;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_eleSCEn;   //!
   TBranch        *b_eleESEn;   //!
   TBranch        *b_eleSCEta;   //!
   TBranch        *b_eleSCPhi;   //!
   TBranch        *b_eleSCRawEn;   //!
   TBranch        *b_eleSCEtaWidth;   //!
   TBranch        *b_eleSCPhiWidth;   //!
   TBranch        *b_eleHoverE;   //!
   TBranch        *b_eleEoverP;   //!
   TBranch        *b_eleEoverPInv;   //!
   TBranch        *b_eleBrem;   //!
   TBranch        *b_eledEtaAtVtx;   //!
   TBranch        *b_eledPhiAtVtx;   //!
   TBranch        *b_eleSigmaIEtaIEta;   //!
   TBranch        *b_eleSigmaIEtaIEta_2012;   //!
   TBranch        *b_eleSigmaIPhiIPhi;   //!
   TBranch        *b_eleMissHits;   //!
   TBranch        *b_eleESEffSigmaRR;   //!
   TBranch        *b_elePFChIso;   //!
   TBranch        *b_elePFPhoIso;   //!
   TBranch        *b_elePFNeuIso;   //!
   TBranch        *b_elePFPUIso;   //!
   TBranch        *b_eleBC1E;   //!
   TBranch        *b_eleBC1Eta;   //!
   TBranch        *b_eleBC2E;   //!
   TBranch        *b_eleBC2Eta;   //!

};
#endif

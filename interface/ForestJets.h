
#ifndef ForestJets_h
#define ForestJets_h

#define maxJets 20000
#define maxGenJets 20000

#include "TBranch.h"

class ForestJets {
public :
   ForestJets(){};
   ~ForestJets(){};

   // Declaration of leaf types
   Int_t           evt;
   Float_t         b;
   Int_t           nref;
   Float_t         rawpt[maxJets];   //[nref]
   Float_t         jtpt[maxJets];   //[nref]
   Float_t         jteta[maxJets];   //[nref]
   Float_t         jty[maxJets];   //[nref]
   Float_t         jtphi[maxJets];   //[nref]
   Float_t         jtpu[maxJets];   //[nref]
   Float_t         jtm[maxJets];   //[nref]
   Float_t         jtarea[maxJets];   //[nref]
   Float_t         jtPfCHF[maxJets];   //[nref]
   Float_t         jtPfNHF[maxJets];   //[nref]
   Float_t         jtPfCEF[maxJets];   //[nref]
   Float_t         jtPfNEF[maxJets];   //[nref]
   Float_t         jtPfMUF[maxJets];   //[nref]
   Int_t           jtPfCHM[maxJets];   //[nref]
   Int_t           jtPfNHM[maxJets];   //[nref]
   Int_t           jtPfCEM[maxJets];   //[nref]
   Int_t           jtPfNEM[maxJets];   //[nref]
   Int_t           jtPfMUM[maxJets];   //[nref]
   Float_t         jttau1[maxJets] = {-999};   //[nref]
   Float_t         jttau2[maxJets] = {-999};   //[nref]
   Float_t         jttau3[maxJets] = {-999};   //[nref]
   std::vector<std::vector<float> > *jtSubJetPt = 0;
   std::vector<std::vector<float> > *jtSubJetEta = 0;
   std::vector<std::vector<float> > *jtSubJetPhi= 0;
   std::vector<std::vector<float> > *jtSubJetM = 0;
   Float_t         discr_fr01[maxJets];   //[nref]
   Float_t         trackMax[maxJets];   //[nref]
   Float_t         trackSum[maxJets];   //[nref]
   Int_t           trackN[maxJets];   //[nref]
   Float_t         trackHardSum[maxJets];   //[nref]
   Int_t           trackHardN[maxJets];   //[nref]
   Float_t         chargedMax[maxJets];   //[nref]
   Float_t         chargedSum[maxJets];   //[nref]
   Int_t           chargedN[maxJets];   //[nref]
   Float_t         chargedHardSum[maxJets];   //[nref]
   Int_t           chargedHardN[maxJets];   //[nref]
   Float_t         photonMax[maxJets];   //[nref]
   Float_t         photonSum[maxJets];   //[nref]
   Int_t           photonN[maxJets];   //[nref]
   Float_t         photonHardSum[maxJets];   //[nref]
   Int_t           photonHardN[maxJets];   //[nref]
   Float_t         neutralMax[maxJets];   //[nref]
   Float_t         neutralSum[maxJets];   //[nref]
   Int_t           neutralN[maxJets];   //[nref]
   Float_t         hcalSum[maxJets];   //[nref]
   Float_t         ecalSum[maxJets];   //[nref]
   Float_t         eMax[maxJets];   //[nref]
   Float_t         eSum[maxJets];   //[nref]
   Int_t           eN[maxJets];   //[nref]
   Float_t         muMax[maxJets];   //[nref]
   Float_t         muSum[maxJets];   //[nref]
   Int_t           muN[maxJets];   //[nref]
   Float_t         discr_ssvHighEff[maxJets];   //[nref]
   Float_t         discr_ssvHighPur[maxJets];   //[nref]
   Float_t         discr_csvMva[maxJets];   //[nref]
   Float_t         discr_csvSimple[maxJets];   //[nref]
   
   Float_t         matchedPt[maxJets];   //[nref]
   Float_t         matchedR[maxJets];   //[nref]
   Int_t           beamId1;
   Int_t           beamId2;
   Float_t         pthat;
   Float_t         refpt[maxJets];   //[nref]
   Float_t         refeta[maxJets];   //[nref]
   Float_t         refy[maxJets];   //[nref]
   Float_t         refphi[maxJets];   //[nref]
   Float_t         refm[maxJets];   //[nref]
   Float_t         reftau1[maxJets] = {-999};   //[nref]
   Float_t         reftau2[maxJets] = {-999};   //[nref]
   Float_t         reftau3[maxJets] = {-999};   //[nref]
   Float_t         refdphijt[maxJets];   //[nref]
   Float_t         refdrjt[maxJets];   //[nref]
   Float_t         refparton_pt[maxJets];   //[nref]
   Int_t           refparton_flavor[maxJets];   //[nref]
   Int_t           refparton_flavorForB[maxJets];   //[nref]

   Float_t         refptG[maxJets];   //[nref]
   Float_t         refetaG[maxJets];   //[nref]
   Float_t         refphiG[maxJets];   //[nref]
   Float_t         refmG[maxJets];   //[nref]
   std::vector<std::vector<float> > *refSubJetPt = 0;
   std::vector<std::vector<float> > *refSubJetEta = 0;
   std::vector<std::vector<float> > *refSubJetPhi= 0;
   std::vector<std::vector<float> > *refSubJetM = 0;

   Float_t         genChargedSum[maxJets];   //[nref]
   Float_t         genHardSum[maxJets];   //[nref]

   Float_t         signalChargedSum[maxJets];   //[nref]
   Float_t         signalHardSum[maxJets];   //[nref]

   Int_t           subid[maxJets];   //[nref]

   Int_t           ngen;
   Int_t           genmatchindex[maxGenJets];   //[ngen]
   Float_t         genpt[maxGenJets];   //[ngen]
   Float_t         geneta[maxGenJets];   //[ngen]
   Float_t         geny[maxGenJets];   //[ngen]
   Float_t         genphi[maxGenJets];   //[ngen]
   Float_t         genm[maxGenJets] = {0.};   //[ngen]
   Float_t         gentau1[maxGenJets] = {-999};   //[nref]
   Float_t         gentau2[maxGenJets] = {-999};   //[nref]
   Float_t         gentau3[maxGenJets] = {-999};   //[nref]
   Float_t         gendphijt[maxGenJets];   //[ngen]
   Float_t         gendrjt[maxGenJets];   //[ngen]
   Int_t           gensubid[maxGenJets];   //[ngen]

   Float_t         smpt[maxJets];   //[nref]
   Float_t         fr01Chg[maxJets];   //[nref]
   Float_t         fr01EM[maxJets];   //[nref]
   Float_t         fr01[maxJets];   //[nref]

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_b;   //!
   TBranch        *b_nref;   //!
   TBranch        *b_rawpt;   //!
   TBranch        *b_jtpt;   //!
   TBranch        *b_jteta;   //!
   TBranch        *b_jty;   //!
   TBranch        *b_jtphi;   //!
   TBranch        *b_jtpu;   //!
   TBranch        *b_jtm;   //!
   TBranch        *b_jtarea;   //!
   TBranch        *b_jtPfCHF;   //!
   TBranch        *b_jtPfNHF;   //!
   TBranch        *b_jtPfCEF;   //!
   TBranch        *b_jtPfNEF;   //!
   TBranch        *b_jtPfMUF;   //!
   TBranch        *b_jtPfCHM;   //!
   TBranch        *b_jtPfNHM;   //!
   TBranch        *b_jtPfCEM;   //!
   TBranch        *b_jtPfNEM;   //!
   TBranch        *b_jtPfMUM;   //!
   TBranch        *b_jttau1;   //!
   TBranch        *b_jttau2;   //!
   TBranch        *b_jttau3;   //!
   TBranch        *b_jtSubJetPt;   //!
   TBranch        *b_jtSubJetEta;   //!
   TBranch        *b_jtSubJetPhi;   //!
   TBranch        *b_jtSubJetM;   //!
   TBranch        *b_discr_fr01;   //!
   TBranch        *b_trackMax;   //!
   TBranch        *b_trackSum;   //!
   TBranch        *b_trackN;   //!
   TBranch        *b_trackHardSum;   //!
   TBranch        *b_trackHardN;   //!
   TBranch        *b_chargedMax;   //!
   TBranch        *b_chargedSum;   //!
   TBranch        *b_chargedN;   //!
   TBranch        *b_chargedHardSum;   //!
   TBranch        *b_chargedHardN;   //!
   TBranch        *b_photonMax;   //!
   TBranch        *b_photonSum;   //!
   TBranch        *b_photonN;   //!
   TBranch        *b_photonHardSum;   //!
   TBranch        *b_photonHardN;   //!
   TBranch        *b_neutralMax;   //!
   TBranch        *b_neutralSum;   //!
   TBranch        *b_neutralN;   //!
   TBranch        *b_hcalSum;   //!
   TBranch        *b_ecalSum;   //!
   TBranch        *b_eMax;   //!
   TBranch        *b_eSum;   //!
   TBranch        *b_eN;   //!
   TBranch        *b_muMax;   //!
   TBranch        *b_muSum;   //!
   TBranch        *b_muN;   //!
   TBranch        *b_discr_ssvHighEff;   //!
   TBranch        *b_discr_ssvHighPur;   //!
   TBranch        *b_discr_csvMva;       //!
   TBranch        *b_discr_csvSimple;    //!
   
   TBranch        *b_matchedPt;   //!
   TBranch        *b_matchedR;   //!
   TBranch        *b_beamId1;   //!
   TBranch        *b_beamId2;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_refpt;   //!
   TBranch        *b_refeta;   //!
   TBranch        *b_refy;   //!
   TBranch        *b_refphi;   //!
   TBranch        *b_refm;   //!
   TBranch        *b_reftau1;   //!
   TBranch        *b_reftau2;   //!
   TBranch        *b_reftau3;   //!
   TBranch        *b_refdphijt;   //!
   TBranch        *b_refdrjt;   //!
   TBranch        *b_refparton_pt;   //!
   TBranch        *b_refparton_flavor;   //!
   TBranch        *b_refparton_flavorForB;   //!

   TBranch        *b_refptG;   //!
   TBranch        *b_refetaG;   //!
   TBranch        *b_refphiG;   //!
   TBranch        *b_refmG;   //!
   TBranch        *b_refSubJetPt;   //!
   TBranch        *b_refSubJetEta;   //!
   TBranch        *b_refSubJetPhi;   //!
   TBranch        *b_refSubJetM;   //!
   
   TBranch        *b_genChargedSum;   //!
   TBranch        *b_genHardSum;   //!
   TBranch        *b_signalChargedSum;   //!
   TBranch        *b_signalHardSum;   //!
   TBranch        *b_subid;   //!

   TBranch        *b_ngen;   //!
   TBranch        *b_genmatchindex;   //!
   TBranch        *b_genpt;   //!
   TBranch        *b_geneta;   //!
   TBranch        *b_geny;   //!
   TBranch        *b_genphi;   //!
   TBranch        *b_genm;   //!
   TBranch        *b_gentau1;   //!
   TBranch        *b_gentau2;   //!
   TBranch        *b_gentau3;   //!
   TBranch        *b_gendphijt;   //!
   TBranch        *b_gendrjt;   //!
   TBranch        *b_gensubid;   //!
   TBranch        *b_smpt;   //!
   TBranch        *b_fr01Chg;   //!
   TBranch        *b_fr01EM;   //!
   TBranch        *b_fr01;   //!

};
#endif


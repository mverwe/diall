#ifndef lwJet_h
#define lwJet_h

//
// light-weight reconstructed jet
//

#include "TClonesArray.h"

#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/pfParticle.h"

class lwJet : public particleBase {
 public:
  
  lwJet();
  lwJet(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id = -1, Int_t c = 0);
  virtual ~lwJet() {;}
  lwJet(const lwJet& obj); // copy constructor
  lwJet& operator=(const lwJet& other); // assignment

  Double_t    GetArea()                 const { return fArea           ; }
  float       GetTau(int num)           const { if(num>0 && num<4) {return fTau[num-1];} else return -1.; }

  float       GetRawPt()                const { return fRawPt          ; }
  float       GetRawM()                 const { return fRawM           ; }
  float       GetRefPt()                const { return fRefPt          ; }
  float       GetRefEta()               const { return fRefEta         ; }
  float       GetRefPhi()               const { return fRefPhi         ; }
  float       GetRefM()                 const { return fRefM           ; }

  float       GetRefPtG()               const { return fRefPtG         ; }
  float       GetRefEtaG()              const { return fRefEtaG        ; }
  float       GetRefPhiG()              const { return fRefPhiG        ; }
  float       GetRefMG()                const { return fRefMG          ; }
  
  float       GetRefDr()                const { return fRefDr          ; }
  float       GetRefTau(int num)        const { if(num>0 && num<4) { return fRefTau[num-1];} else return -1.; }
  std::vector<float> GetSubJetPt()      const { return fSubJetPt       ; }
  std::vector<float> GetSubJetEta()     const { return fSubJetEta      ; }
  std::vector<float> GetSubJetPhi()     const { return fSubJetPhi      ; }
  std::vector<float> GetSubJetM()       const { return fSubJetM        ; }
  int         GetNDroppedBranches()     const { return fNDropped        ; }
 
  std::vector<float> GetRefSubJetPt()   const { return fRefSubJetPt    ; }
  std::vector<float> GetRefSubJetEta()  const { return fRefSubJetEta   ; }
  std::vector<float> GetRefSubJetPhi()  const { return fRefSubJetPhi   ; }
  std::vector<float> GetRefSubJetM()    const { return fRefSubJetM     ; }
  int         GetRefNDroppedBranches()  const { return fRefNDropped    ; } 
 
  int         GetSubEvent()             const { return fSube           ; }
  
  Int_t       GetNConstituents()        const { return fConstIds.size(); }
  Int_t       GetConstituentId(Int_t i) const { return fConstIds[i]    ; }
  particleBase *GetConstituent(Int_t i, TClonesArray *c) const { if(!c) return 0; return dynamic_cast<particleBase*>(c->At(fConstIds[i])); }

  Int_t       GetRefParton()            const { return fRefParton      ; }
  Int_t       GetRefPartonForB()        const { return fRefPartonForB  ; }
  Float_t     GetCsvSimpleDiscr()       const { return fCsvSimpleDiscr ; }
    
  Float_t       GetChargedSum()           const { return fChargedSum; }
  Float_t       GetChargedMax()           const { return fChargedMax; }
  Int_t         GetChargedN()             const { return fChargedN;   }
  Float_t       GetChargedHardSum()       const { return fChargedHardSum; }
  Float_t       GetChargedHardMax()       const { return fChargedHardMax; }
  Int_t         GetChargedHardN()         const { return fChargedHardN;   }
  Float_t       GetPhotonSum()            const { return fPhotonSum; }
  Float_t       GetPhotonMax()            const { return fPhotonMax; }
  Int_t         GetPhotonN()              const { return fPhotonN;   }
  Float_t       GetNeutralSum()           const { return fNeutralSum; }
  Float_t       GetNeutralMax()           const { return fNeutralMax; }
  Int_t         GetNeutralN()             const { return fNeutralN;   }
  Float_t       GetEmSum()                const { return fEmSum; }
  Float_t       GetEmMax()                const { return fEmMax; }
  Int_t         GetEmN()                  const { return fEmN; }
  Float_t       GetMuSum()                const { return fMuSum; }
  Float_t       GetMuMax()                const { return fMuMax; }
  Int_t         GetMuN()                  const { return fMuN; }

  float         GetCHF()                  const { return fCHF; }
  float         GetNHF()                  const { return fNHF; }
  float         GetCEF()                  const { return fCEF; }
  float         GetNEF()                  const { return fNEF; }
  float         GetMUF()                  const { return fMUF; }

  int           GetCHM()                  const { return fCHM; }
  int           GetNHM()                  const { return fNHM; }
  int           GetCEM()                  const { return fCEM; }
  int           GetNEM()                  const { return fNEM; }
  int           GetMUM()                  const { return fMUM; }
  
  void        SetArea(Double_t a)          { fArea = a; }
  void        SetRawPt(Double_t pt)        { fRawPt = pt; }
  void        SetRawM(Double_t m)          { fRawM = m; }
  void        SetTau(int num, float tau)   { if(num>0 && num<4) fTau[num-1] = tau; }
  void        SetSubJetPt(std::vector<float> v)  { fSubJetPt  = v; }
  void        SetSubJetEta(std::vector<float> v) { fSubJetEta = v; }
  void        SetSubJetPhi(std::vector<float> v) { fSubJetPhi = v; }
  void        SetSubJetM(std::vector<float> v)   { fSubJetM   = v; }
  void        SetNDroppedBranches(int d)   { fNDropped = d; } 
 
  void        SetRefPt(Double_t pt)        { fRefPt = pt; }
  void        SetRefEta(Double_t eta)      { fRefEta = eta; }
  void        SetRefPhi(Double_t phi)      { fRefPhi = phi; }
  void        SetRefM(Double_t m)          { fRefM = m; }

  void        SetRefPtG(Double_t pt)        { fRefPtG = pt; }
  void        SetRefEtaG(Double_t eta)      { fRefEtaG = eta; }
  void        SetRefPhiG(Double_t phi)      { fRefPhiG = phi; }
  void        SetRefMG(Double_t m)          { fRefMG = m; }
  
  void        SetRefTau(int num, float tau)   { if(num>0 && num<4) fRefTau[num-1] = tau; }
  void        SetRefDr(Double_t dr)        { fRefDr = dr; }

  void        SetRefSubJetPt(std::vector<float> v)  { fRefSubJetPt  = v; }
  void        SetRefSubJetEta(std::vector<float> v) { fRefSubJetEta = v; }
  void        SetRefSubJetPhi(std::vector<float> v) { fRefSubJetPhi = v; }
  void        SetRefSubJetM(std::vector<float> v)   { fRefSubJetM   = v; }
  void        SetRefNDroppedBranches(int d){ fRefNDropped = d; }  

  void        SetSubEvent(Int_t su)        { fSube  = su; }

  void        AddConstituent(Int_t i )     { fConstIds.push_back(i); }
  void        SetRefToParton(Int_t i)      { fRefParton = i;}
  void        SetRefToPartonForB(Int_t i)  { fRefPartonForB = i; }
  void        SetCsvSimpleDiscr(Float_t d) { fCsvSimpleDiscr = d;}

  void        SetChargedProp(Float_t max, Float_t sum, Int_t n);
  void        SetChargedHardProp(Float_t max, Float_t sum, Int_t n);
  void        SetPhotonProp(Float_t max, Float_t sum, Int_t n);
  void        SetNeutralProp(Float_t max, Float_t sum, Int_t n);
  void        SetEmProp(Float_t max, Float_t sum, Int_t n);
  void        SetMuProp(Float_t max, Float_t sum, Int_t n);

  void        SetCHF(float f) { fCHF = f; }
  void        SetNHF(float f) { fNHF = f; }
  void        SetCEF(float f) { fCEF = f; }
  void        SetNEF(float f) { fNEF = f; }
  void        SetMUF(float f) { fMUF = f; }

  void        SetCHM(int f) { fCHM = f; }
  void        SetNHM(int f) { fNHM = f; }
  void        SetCEM(int f) { fCEM = f; }
  void        SetNEM(int f) { fNEM = f; }
  void        SetMUM(int f) { fMUM = f; }
  
 protected:
  Double_t          fArea;          //jet area
  std::vector<Int_t>  fConstIds;      //ids of constituents
  Int_t             fRefParton;     //ref to parton
  Int_t             fRefPartonForB; //ref to parton for b
  Float_t           fCsvSimpleDiscr;//csv simple b-jet discriminator

  float             fTau[3];
  
  std::vector<float>  fSubJetPt;  //list of subjet pt
  std::vector<float>  fSubJetEta; //list of subjet eta
  std::vector<float>  fSubJetPhi; //list of subjet phi
  std::vector<float>  fSubJetM;   //list of subjet m
  int               fNDropped;    //number of dropped branches by softdrop

  float             fRawPt;         //raw pt (no jec)
  float             fRawM;          //raw mass (no jec)
  float             fRefPt;         //matched gen-level jet pt
  float             fRefEta;        //matched gen-level jet eta
  float             fRefPhi;        //matched gen-level jet phi
  float             fRefM;          //matched gen-level jet mass

  float             fRefPtG;        //matched gen-level jet pt
  float             fRefEtaG;       //matched gen-level jet eta
  float             fRefPhiG;       //matched gen-level jet phi
  float             fRefMG;         //matched gen-level jet mass
  
  float             fRefTau[3];
  float             fRefDr;         //distance between reco and gen jet

  std::vector<float>  fRefSubJetPt;  //list of subjet pt
  std::vector<float>  fRefSubJetEta; //list of subjet eta
  std::vector<float>  fRefSubJetPhi; //list of subjet phi
  std::vector<float>  fRefSubJetM;   //list of subjet m
  int               fRefNDropped;    //number of dropped branches by softdrop
  
  int               fSube;          //sub-event id (0=hard process)
  float             fChargedMax;    //max track pt
  float             fChargedSum;    //summed track pt
  int               fChargedN;      //n tracks
  float             fChargedHardMax;//max track pt with track pt>4
  float             fChargedHardSum;//summed track pt of tracks with pt>4
  int               fChargedHardN;  //n tracks with pt>4
  float             fPhotonMax;     //max photon e
  float             fPhotonSum;
  int               fPhotonN;
  float             fNeutralMax;
  float             fNeutralSum;
  int               fNeutralN;
  float             fEmMax;
  float             fEmSum;
  int               fEmN;
  float             fMuMax;
  float             fMuSum;
  int               fMuN;

  float             fCHF;
  float             fNHF;
  float             fCEF;
  float             fNEF;
  float             fMUF;
  int             fCHM;
  int             fNHM;
  int             fCEM;
  int             fNEM;
  int             fMUM;
    
  ClassDef(lwJet,7)
};
#endif

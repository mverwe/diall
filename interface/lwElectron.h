#ifndef lwElectron_h
#define lwElectron_h

//
// light-weight electron candidate
//

#include "UserCode/diall/interface/particleBase.h"

class lwElectron : public particleBase {
 public:
  lwElectron();
  lwElectron(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id = -1);
  virtual ~lwElectron() {;}
  lwElectron(const lwElectron& obj); // copy constructor
  lwElectron& operator=(const lwElectron& other); // assignment

  void     SetClosestGen(Int_t i)      { fMatchId1 = i; }
  void     SetClosestPF(Int_t i)       { fMatchId2  = i; }

  Int_t    GetClosestGen()   const     { return fMatchId1; }
  Int_t    GetClosestPF()    const     { return fMatchId2; }
  
 protected:
  
  ClassDef(lwElectron,1)
    };
#endif

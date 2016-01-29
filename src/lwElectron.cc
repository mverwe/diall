//
// light-weight muon candidate
//

#include "UserCode/diall/interface/lwElectron.h"

ClassImp(lwElectron)

//__________________________________________________________
lwElectron::lwElectron() :
particleBase()
{
  //default constructor
}

//__________________________________________________________
lwElectron::lwElectron(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id) :
  particleBase(pt,eta,phi,m,id,0)
{
  //standard constructor
}

//__________________________________________________________
lwElectron::lwElectron(const lwElectron &obj) :
  particleBase(obj)
{
  //copy constructor
}

//__________________________________________________________
lwElectron &lwElectron::operator=(const lwElectron &other)
{
  //assignment operator

  if(this!=&other) {
    lwElectron::operator=(other);
  }
  return *this;
}

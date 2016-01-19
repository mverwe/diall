//
// particle reconstructed from two decay products
//

#include "UserCode/diall/interface/diParticle.h"

ClassImp(diParticle)

//__________________________________________________________
diParticle::diParticle() :
particleBase(),
  fDecayParticles(0x0)
{
  //default constructor
  fDecayParticles = new TObjArray();
}

//__________________________________________________________
diParticle::diParticle(Double_t pt, Double_t eta, Double_t phi, Double_t m, Int_t id) :
  particleBase(pt,eta,phi,m,id,0),
  fDecayParticles(0x0)
{
  //standard constructor
  fDecayParticles = new TObjArray();
}

//__________________________________________________________
diParticle::diParticle(const diParticle &obj) :
  particleBase(obj)
{
  //copy constructor
  //fDecayParticles = obj.fDecayParticles;
}

//__________________________________________________________
void diParticle::AddParticle(particleBase *p) {

  fDecayParticles->Add(p);
  
}

//__________________________________________________________
diParticle &diParticle::operator=(const diParticle &other)
{
  //assignment operator

  if(this!=&other) {
    diParticle::operator=(other);
  }
  return *this;
}


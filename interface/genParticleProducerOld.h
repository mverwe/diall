#ifndef genParticleProducerOld_h
#define genParticleProducerOld_h

//
// generated particles producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/ForestGenParticlesOld.h>

class genParticleProducerOld : public inputBase {
 public:
  genParticleProducerOld();
  genParticleProducerOld(const char *name);
  virtual ~genParticleProducerOld() {;}

  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetGenParticlesName(TString n) { fGenParticlesName = n; }
  void     SetStatus(Int_t i )            { fStatus = i;           }
  void     SetSubevent(Int_t i )          { fSube   = i;           }
  
  const char* GetGenParticlesName() const { return fGenParticlesName.Data() ; }
  
 protected:
  Bool_t   AcceptParticle(Int_t i);
  
  TString                      fGenParticlesName;// name of generated particles branch
  TClonesArray                *fGenParticles;    //!generated particles
  ForestGenParticlesOld        fFGenParts;       //! generated particles in forest tree
  Int_t                        fStatus;          // required status
  Int_t                        fSube;            // required subevent
  
 private:
  genParticleProducerOld(const genParticleProducerOld& obj); // copy constructor
  genParticleProducerOld& operator=(const genParticleProducerOld& other); // assignment
  
  ClassDef(genParticleProducerOld,1)
};
#endif

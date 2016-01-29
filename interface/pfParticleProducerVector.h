#ifndef pfParticleProducerVector_h
#define pfParticleProducerVector_h

//
// particle flow candidate producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>

#include "UserCode/diall/interface/inputBase.h"
#include <UserCode/diall/interface/pfParticle.h>
#include <UserCode/diall/interface/ForestPFsVector.h>

class pfParticleProducerVector : public inputBase {
 public:
  pfParticleProducerVector();
  pfParticleProducerVector(const char *name);
  virtual ~pfParticleProducerVector() {;}

  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetpfParticlesName(TString n) { fpfParticlesName = n; }

  const char* GetpfParticlesName() const { return fpfParticlesName.Data() ; }
  
 protected:
  TString                      fpfParticlesName;  // name of HI event container
  TClonesArray                *fpfParticles;      //!HI event properties
  ForestPFsVector              fPFs;              //!PFs in tree

 private:
  pfParticleProducerVector(const pfParticleProducerVector& obj); // copy constructor
  pfParticleProducerVector& operator=(const pfParticleProducerVector& other); // assignment
  
  ClassDef(pfParticleProducerVector,1)
};
#endif

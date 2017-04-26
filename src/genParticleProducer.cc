//
// producer of generated particles
//

#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/genParticle.h"

ClassImp(genParticleProducer)

//__________________________________________________________
genParticleProducer::genParticleProducer() :
inputBase("genParticleProducer"),
  fGenParticlesName("genParticles"),
  fGenParticles(0x0),
  fFGenParts(),
  fStatus(1),
  fSube(0)
{
  //default constructor
}

//__________________________________________________________
genParticleProducer::genParticleProducer(const char *name) :
  inputBase(name),
  fGenParticlesName("genParticles"),
  fGenParticles(0x0),
  fFGenParts(),
  fStatus(1),
  fSube(0)
{
  //standard constructor
}

//__________________________________________________________
Bool_t genParticleProducer::Init() {

  if(!inputBase::Init()) return kFALSE;

  if(fInputMode==hiForest) {
    // Set branch addresses and branch pointers
    //Printf("Set branch addresses and branch pointers");
    //fChain->SetBranchStatus("*", 1);
/*
    if (fChain->GetBranch("event")) fChain->SetBranchAddress("event", &fFGenParts.event, &fFGenParts.b_event);
    if (fChain->GetBranch("b")) fChain->SetBranchAddress("b", &fFGenParts.b, &fFGenParts.b_b);
    if (fChain->GetBranch("npart")) fChain->SetBranchAddress("npart", &fFGenParts.npart, &fFGenParts.b_npart);
    if (fChain->GetBranch("ncoll")) fChain->SetBranchAddress("ncoll", &fFGenParts.ncoll, &fFGenParts.b_ncoll);
    if (fChain->GetBranch("nhard")) fChain->SetBranchAddress("nhard", &fFGenParts.nhard, &fFGenParts.b_nhard);
    if (fChain->GetBranch("phi0")) fChain->SetBranchAddress("phi0", &fFGenParts.phi0, &fFGenParts.b_phi0);
    if (fChain->GetBranch("scale")) fChain->SetBranchAddress("scale", &fFGenParts.scale, &fFGenParts.b_scale);
    if (fChain->GetBranch("n")) fChain->SetBranchAddress("n", fFGenParts.n, &fFGenParts.b_n);
    if (fChain->GetBranch("ptav")) fChain->SetBranchAddress("ptav", fFGenParts.ptav, &fFGenParts.b_ptav);
    if (fChain->GetBranch("mult")) fChain->SetBranchAddress("mult", &fFGenParts.mult, &fFGenParts.b_mult);
*/
    //new version
    fChain->SetBranchStatus("*", 1);
    if (fChain->GetBranch("pt"))   fChain->SetBranchAddress("pt", &fFGenParts.pt, &fFGenParts.b_pt);
    if (fChain->GetBranch("eta"))  fChain->SetBranchAddress("eta", &fFGenParts.eta, &fFGenParts.b_eta);
    if (fChain->GetBranch("phi"))  fChain->SetBranchAddress("phi", &fFGenParts.phi, &fFGenParts.b_phi);
    if (fChain->GetBranch("m"))    fChain->SetBranchAddress("m", &fFGenParts.m, &fFGenParts.b_m);
    if (fChain->GetBranch("pdg"))  fChain->SetBranchAddress("pdg", &fFGenParts.pdg, &fFGenParts.b_pdg);
    if (fChain->GetBranch("chg"))  fChain->SetBranchAddress("chg", &fFGenParts.chg, &fFGenParts.b_chg);
    if (fChain->GetBranch("sube")) fChain->SetBranchAddress("sube", &fFGenParts.sube, &fFGenParts.b_sube);
/*
    fChain->SetBranchStatus("*", 1);
    if (fChain->GetBranch("vx")) fChain->SetBranchAddress("vx", &fFGenParts.vx, &fFGenParts.b_vx);
    if (fChain->GetBranch("vy")) fChain->SetBranchAddress("vy", &fFGenParts.vy, &fFGenParts.b_vy);
    if (fChain->GetBranch("vz")) fChain->SetBranchAddress("vz", &fFGenParts.vz, &fFGenParts.b_vz);
    if (fChain->GetBranch("vr")) fChain->SetBranchAddress("vr", &fFGenParts.vr, &fFGenParts.b_vr);
*/
    fInit = kTRUE;
    //Printf("done setting branch addresses");
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t genParticleProducer::InitEventObjects() {

  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(fGenParticlesName)) {
      fGenParticles = new TClonesArray("genParticle");
      fGenParticles->SetName(fGenParticlesName);
      fEventObjects->Add(fGenParticles);
    }
  }
  
  return kTRUE;
}

//__________________________________________________________
Bool_t genParticleProducer::Run(Long64_t entry) {

  //overloaded run funtion
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;
  //Printf("tree loaded");
  if(!InitEventObjects()) return kFALSE;
  //Printf("InitEventObjects done"); 
  //clear arrays
  if(fGenParticles) fGenParticles->Delete();
  //Printf("fGenParticles cleared");
  //generated particles
  int ngen = 0;
  //Printf("print nparticles");
  //std::cout << "nparticles: " << fFGenParts.pt->size() << std::endl;
  for(UInt_t i = 0; i<fFGenParts.pt->size(); i++) {
    //Printf("igen: %d",i);
    if(!AcceptParticle(i)) continue;

    genParticle *gp = new genParticle(fFGenParts.pt->at(i),
                                      fFGenParts.eta->at(i),
                                      fFGenParts.phi->at(i),
                                      //                                      fFGenParts.m->at(i),
                                      0,
                                      i);
    
    //gp->SetCharge((int)(fFGenParts.chg->at(i)));
    //int pdg = fFGenParts.pdg->at(i);
    gp->SetCharge(fFGenParts.chg->at(i));//fabs(pdg)/pdg);
    //if(fFGenParts.chg->at(i)==0) Printf("!!!!!!!!!!!!! found zero charge");
    //Printf("pt=%f chg: %d",fFGenParts.pt->at(i),fFGenParts.chg->at(i));
    gp->SetPID((int)fFGenParts.pdg->at(i));
    //if(fabs(fFGenParts.pdg->at(i))==13) Printf("%d pdg: %d",i,fFGenParts.pdg->at(i)); 
    gp->SetPIDMom(0);
    (*fGenParticles)[ngen] = gp;
    ++ngen;
  }
  fGenParticles->Sort();

  return kTRUE;
}

//__________________________________________________________
Bool_t genParticleProducer::AcceptParticle(Int_t i) {

  //gen particle selection

  // if((*fFGenParts.sta)[i]!=fStatus)                  return kFALSE;
  //if((*fFGenParts.sube)[i]!=fSube)                   return kFALSE;
  int pdgabs = fabs((int)fFGenParts.pdg->at(i));
  //Printf("pdgabs: %d",pdgabs);
  if(pdgabs==12 || pdgabs==14 || pdgabs==16) //12,14,16 exclude neutrinos
   return kFALSE;
  
  return kTRUE;
}

//__________________________________________________________
Long64_t genParticleProducer::LoadTree(Long64_t entry) {

  //overloaded LoadTree function 
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }
  
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
  }

  //  fChain->SetMakeClass(1);
  //Printf("LoadTree");
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("genParticleProducer: centry smaller than 0");
    return centry;  
  }
  
  //Printf("GetEntry: %d",(int)entry);
  fChain->GetEntry(entry);

  return centry;
}

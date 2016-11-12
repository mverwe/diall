#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaCreateJetTree.h"
#include "UserCode/diall/interface/anaJetEnergyScale.h"
#include "UserCode/diall/interface/anaJetMass.h"
#include "UserCode/diall/interface/anaJetMatching.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaPuppiParticles.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;


void analyzeGenChJetMass(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

  std::cout << "nfiles: " << urls.size() << std::endl;
  for (auto i = urls.begin(); i != urls.end(); ++i)
    std::cout << *i << std::endl;

  size_t firstFile = 0;
  size_t lastFile = urls.size();

  if(firstF>-1) {
    firstFile = (size_t)firstF;
    lastFile = std::min((size_t)lastF,lastFile);
  }
  std::cout << "firstFile: " << firstFile << "  lastFile: " << lastFile << std::endl;

  Int_t lastEvent = nentries;
  if(firstEvent>0) {
    lastEvent = firstEvent + nentries;
  }
  std::cout << "firstEvent: " << firstEvent << std::endl;
  
  //add files to chain
  TChain *chain = NULL;
  //chain = new TChain("hiEvtAnalyzer/HiTree");
  //for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  //Printf("hiTree done");

  TChain *genTree = new TChain("HiGenParticleAna/hi");
  for(size_t i=firstFile; i<lastFile; i++) genTree->Add(urls[i].c_str());
  chain = genTree;
  //chain->AddFriend(genTree);
  Printf("genTree done");

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  genParticleProducer *p_gen = new genParticleProducer("genParticleProd");
  p_gen->SetInput(chain);
  p_gen->SetGenParticlesName("genParticles");
  p_gen->SetEventObjects(fEventObjects);


  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");


  //anti-kt jet finder on gen particles charged
  LWJetProducer *lwjaktGen = new LWJetProducer("LWGenJetProducerAKTR030Ch","LWGenJetProducerAKTR030Ch");
  //lwjaktGen->SetInput(chain);
  lwjaktGen->ConnectEventObject(fEventObjects);
  lwjaktGen->SetJetType(LWJetProducer::kAKT);
  lwjaktGen->SetRadius(0.4);
  lwjaktGen->SetGhostArea(0.005);
  lwjaktGen->SetPtMinConst(0.);
  lwjaktGen->SetChargedOnly(true);
  lwjaktGen->SetParticlesName("genParticles");
  lwjaktGen->SetJetContName("GenJetsAKTR040");
  lwjaktGen->SetDoConstituentSubtraction(kFALSE);
  handler->Add(lwjaktGen);

  anaJetMass *anaJetMassCh = new anaJetMass("anaJetMassChR040","anaJetMassChR040");
  anaJetMassCh->ConnectEventObject(fEventObjects);
  anaJetMassCh->SetHiEvtName("");//hiEventContainer");
  anaJetMassCh->SetJetsName("GenJetsAKTR040");
  handler->Add(anaJetMassCh);

  //anti-kt jet finder on gen particles all -> FULL jets
  LWJetProducer *lwjaktGenFull = new LWJetProducer("LWGenJetProducerAKTR030Full","LWGenJetProducerAKTR030Full");
  //lwjaktGen->SetInput(chain);
  lwjaktGenFull->ConnectEventObject(fEventObjects);
  lwjaktGenFull->SetJetType(LWJetProducer::kAKT);
  lwjaktGenFull->SetRadius(0.4);
  lwjaktGenFull->SetGhostArea(0.005);
  lwjaktGenFull->SetPtMinConst(0.);
  lwjaktGenFull->SetChargedOnly(false);
  lwjaktGenFull->SetParticlesName("genParticles");
  lwjaktGenFull->SetJetContName("GenJetsAKTR040Full");
  lwjaktGenFull->SetDoConstituentSubtraction(kFALSE);
  handler->Add(lwjaktGenFull);

  anaJetMass *anaJetMassFull = new anaJetMass("anaJetMassFullR040","anaJetMassFullR040");
  anaJetMassFull->ConnectEventObject(fEventObjects);
  anaJetMassFull->SetHiEvtName("");//hiEventContainer");
  anaJetMassFull->SetJetsName("GenJetsAKTR040Full");
  handler->Add(anaJetMassFull);
  
 
  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntries(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  lastEvent = std::min((int)entries_tot,lastEvent);
  Printf("firstEvent: %d lastEvent: %d",firstEvent,lastEvent);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    //p_evt->Run(jentry);   //hi event properties
    p_gen->Run(jentry);   //generated particles

    //Execute all analysis tasks
    handler->ExecuteTask();
  }
    
  fEventObjects->Print();

  TFile *out = new TFile(outname,"RECREATE");
  TList *tasks = handler->GetListOfTasks();
  TIter next(tasks);
  anaBaseTask *obj;
  while ((obj = dynamic_cast<anaBaseTask*>(next()) ))
    if(obj->GetOutput()) obj->GetOutput()->Write(obj->GetName(),TObject::kSingleKey);
  
  out->Write();
  out->Close();
  
}

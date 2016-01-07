#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaJetEnergyScale.h"
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

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

Bool_t doPuppi         = kTRUE;//kFALSE;

void analyzePuppi(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int ptminType = 0, int jetSignalType = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */

  double ptMinPuppi = 0.;
  if(ptminType==0) ptMinPuppi = 0.;
  else if(ptminType==1) ptMinPuppi = 1.;
  else if(ptminType==2) ptMinPuppi = 2.;

  TString signalJets = "aktPUR040";
  if(jetSignalType==0) signalJets = "aktPUR040";
  else if(jetSignalType==1) signalJets = "akt4Gen";
  std::cout << "signalJets: " << signalJets << std::endl; 
 
  // std::vector<std::string> urls = CollectFiles(list);

  // Printf("anaFile: %d",anaFile);
  
  std::cout << "nfiles: " << urls.size() << std::endl;
  for (auto i = urls.begin(); i != urls.end(); ++i)
    std::cout << *i << std::endl;

  size_t firstFile = 0;
  size_t lastFile = urls.size();

  if(firstF>-1) {
    firstFile = (size_t)firstF;
    lastFile = (size_t)lastF;
  }
  std::cout << "firstFile: " << firstFile << "  lastFile: " << lastFile << std::endl;

  Int_t lastEvent = nentries;
  if(firstEvent>0) {
    lastEvent = firstEvent + nentries;
  }
  std::cout << "firstEvent: " << firstEvent << std::endl;
  
  //add files to chain
  TChain *chain = NULL;
  chain = new TChain("hiEvtAnalyzer/HiTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("hiTree done");
  
  TChain *pfTree = new TChain("pfcandAnalyzer/pfTree");
  for(size_t i=firstFile; i<lastFile; i++) pfTree->Add(urls[i].c_str());
  chain->AddFriend(pfTree);
  Printf("pfTree done");
  
  // TChain *muTree = new TChain("hltMuTree/HLTMuTree");
  // for(size_t i=firstFile; i<lastFile; i++) muTree->Add(urls[i].c_str());
  // chain->AddFriend(muTree);
  // Printf("muTree done");

  TChain *jetTree = new TChain("akPu4PFJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  chain->AddFriend(jetTree);
  Printf("jetTree done");

  TChain *genTree = new TChain("HiGenParticleAna/hi");
  for(size_t i=firstFile; i<lastFile; i++) genTree->Add(urls[i].c_str());
  chain->AddFriend(genTree);
  Printf("genTree done");
  
  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  pfParticleProducer *p_pf = new pfParticleProducer("pfPartProd");
  p_pf->SetInput(chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);

  genParticleProducer *p_gen = new genParticleProducer("genParticleProd");
  p_gen->SetInput(chain);
  p_gen->SetGenParticlesName("genParticles");
  p_gen->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_PUJet = new lwJetFromForestProducer("lwJetForestProd");
  p_PUJet->SetInput(chain);
  p_PUJet->SetJetContName("aktPUR040");
  p_PUJet->SetGenJetContName("akt4Gen");
  p_PUJet->SetEventObjects(fEventObjects);
  p_PUJet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  //analysis modules which also produce
  anaPuppiProducer *pupProd = new anaPuppiProducer("pupProd","pupProd");
  pupProd->ConnectEventObject(fEventObjects);
  pupProd->SetHiEvtName("hiEventContainer");
  pupProd->SetPFPartName("pfParticles");
  pupProd->SetPuppiPartName("puppiParticles");
  pupProd->SetJetsName(signalJets);
  pupProd->SetAddMetricType(anaPuppiProducer::kMass);
  pupProd->SetPtMinParticle(ptMinPuppi);//1.);
  if(doPuppi) handler->Add(pupProd);

  //anti-kt jet finder on reconstructed PUPPI particles ptmin=0
  LWJetProducer *lwjakt = new LWJetProducer("LWJetProducerAKTR040Puppi","LWJetProducerAKTR040Puppi");
  lwjakt->ConnectEventObject(fEventObjects);
  lwjakt->SetJetType(LWJetProducer::kAKT);
  lwjakt->SetRadius(0.4);
  lwjakt->SetGhostArea(0.005);
  lwjakt->SetPtMinConst(0.);
  lwjakt->SetParticlesName("puppiParticles");
  lwjakt->SetJetContName("JetsAKTR040Puppi");
  if(doPuppi) handler->Add(lwjakt);

  //anti-kt jet finder on generated particles
  LWJetProducer *lwjaktGen = new LWJetProducer("LWGenJetProducerAKTR040","LWGenJetProducerAKTR040");
  lwjaktGen->ConnectEventObject(fEventObjects);
  lwjaktGen->SetJetType(LWJetProducer::kAKT);
  lwjaktGen->SetRadius(0.4);
  lwjaktGen->SetGhostArea(0.005);
  lwjaktGen->SetPtMinConst(0.);
  lwjaktGen->SetParticlesName("genParticles");
  lwjaktGen->SetJetContName("GenJetsAKTR040");
  handler->Add(lwjaktGen);
 
  //Initialization of all analysis modules
  anaPuppiParticles *pupAna = new anaPuppiParticles("pupAna","pupAna");
  pupAna->ConnectEventObject(fEventObjects);
  pupAna->SetHiEvtName("hiEventContainer");
  pupAna->SetParticlesName("puppiParticles");
  pupAna->SetJetsName(signalJets);//"aktPUR040");
  if(doPuppi) handler->Add(pupAna);

  //particle-detector-level jet matching
  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetJetsNameBase("JetsAKTR040Puppi");
  match->SetJetsNameTag("akt4Gen");//GenJetsAKTR040");
  if(doPuppi) handler->Add(match);

  anaJetEnergyScale *anajes = new anaJetEnergyScale("anaJES","anaJES");
  anajes->ConnectEventObject(fEventObjects);
  anajes->SetHiEvtName("hiEventContainer");
  anajes->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
  anajes->SetRecJetsName("JetsAKTR040Puppi");
  anajes->SetNCentBins(4);
  if(doPuppi) handler->Add(anajes);
  
  anaJetMatching *matchGen = new anaJetMatching("jetMatchingGenGen","jetMatchingGenGen");
  matchGen->ConnectEventObject(fEventObjects);
  matchGen->SetHiEvtName("hiEventContainer");
  matchGen->SetJetsNameBase("GenJetsAKTR040");
  matchGen->SetJetsNameTag("akt4Gen");
  if(doPuppi) handler->Add(matchGen);
 
  anaJetEnergyScale *anajesgen = new anaJetEnergyScale("anajesgen","anajesgen");
  anajesgen->ConnectEventObject(fEventObjects);
  anajesgen->SetHiEvtName("hiEventContainer");
  anajesgen->SetGenJetsName("akt4Gen");
  anajesgen->SetRecJetsName("GenJetsAKTR040");
  anajesgen->SetNCentBins(4);
  if(doPuppi) handler->Add(anajesgen);
 
  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntriesFast(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    p_evt->Run(jentry);   //hi event properties
    //Printf("produce pf particles");
    p_pf->Run(jentry);    //pf particles
    //Printf("produce gen particles");
    p_gen->Run(jentry);   //generated particles
    //Printf("produce PU jets");
    p_PUJet->Run(jentry); //forest jets
	    
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

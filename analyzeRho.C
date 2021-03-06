#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
//#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/pfParticleProducerVector.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/triggerProducer.h"
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
#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

bool doPuppi         = false;
bool doJetFinding    = true;
bool useMetric2      = false;
bool storeTree       = false;
bool doCSJets        = true;
bool doJECCS         = false;//true;
bool doZJetResponse  = false;//true;
double alphaCS       = 1.; 

TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/jetsPbPb/puppi/perf/jec";

void analyzeRho(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

  TString strL1 = Form("%s/75X_mcRun2_HeavyIon_v12_L1FastJet_AK4PF_offline.txt",baseJEC.Data());
  TString strL2Rel = Form("%s/75X_mcRun2_HeavyIon_v12_L2Relative_AK4PF_offline.txt",baseJEC.Data());
  TString strL3Abs = Form("%s/75X_mcRun2_HeavyIon_v12_L3Absolute_AK4PF_offline.txt",baseJEC.Data());
  TString strL2L3Res = Form("%s/75X_mcRun2_HeavyIon_v12_L2L3Residual_AK4PF_offline.txt",baseJEC.Data());

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

  TChain *hltTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hltTree->Add(urls[i].c_str());
  chain->AddFriend(hltTree);
  
  TChain *pfTree = new TChain("pfcandAnalyzer/pfTree");
  for(size_t i=firstFile; i<lastFile; i++) pfTree->Add(urls[i].c_str());
  //  chain->AddFriend(pfTree);
  Printf("pfTree done");
  
  // TChain *muTree = new TChain("hltMuTree/HLTMuTree");
  // for(size_t i=firstFile; i<lastFile; i++) muTree->Add(urls[i].c_str());
  // chain->AddFriend(muTree);
  // Printf("muTree done");

  // TChain *jetTree = new TChain("akPu4PFJetAnalyzer/t");
  // for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  // chain->AddFriend(jetTree);
  // Printf("jetTree done");

  // TChain *genTree = new TChain("HiGenParticleAna/hi");
  // for(size_t i=firstFile; i<lastFile; i++) genTree->Add(urls[i].c_str());
  // chain->AddFriend(genTree);
  // Printf("genTree done");
  
  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  triggerProducer *p_trg = new triggerProducer("trigProd");
  p_trg->SetInput(chain);
  p_trg->SetTriggerMapName("triggerMap");
  p_trg->AddTrigger("HLT_HIL1MinimumBiasHF1AND_v1");
  p_trg->SetEventObjects(fEventObjects);

  pfParticleProducerVector *p_pf = new pfParticleProducerVector("pfPartProd");
  p_pf->SetInput(pfTree);//chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);

  /*
  pfParticleProducerVector *p_pf = new pfParticleProducerVector("pfPartProd");
  p_pf->SetInput(chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);
  */

  // genParticleProducer *p_gen = new genParticleProducer("genParticleProd");
  // p_gen->SetInput(chain);
  // p_gen->SetGenParticlesName("genParticles");
  // p_gen->SetEventObjects(fEventObjects);

  // lwJetFromForestProducer *p_PUJet = new lwJetFromForestProducer("lwJetForestProd");
  // p_PUJet->SetInput(chain);
  // p_PUJet->SetJetContName("aktPUR040");
  // p_PUJet->SetGenJetContName("akt4Gen");
  // p_PUJet->SetEventObjects(fEventObjects);
  // p_PUJet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  //Initialization of all analysis modules
 
  //kt jet finder
  LWJetProducer *lwjkt = new LWJetProducer("LWJetProducerKTR040","LWJetProducerKTR040");
  lwjkt->ConnectEventObject(fEventObjects);
  lwjkt->SetJetType(LWJetProducer::kKT);
  lwjkt->SetRadius(0.4);
  lwjkt->SetGhostArea(0.005);
  lwjkt->SetPtMinConst(0.);
  //lwjkt->SetIdConst(4);//charged=1; EM=4
  lwjkt->SetParticlesName("pfParticles");
  lwjkt->SetJetContName("JetsKTR040");
  lwjkt->SetDoConstituentSubtraction(kFALSE);
  handler->Add(lwjkt);

  anaRhoProducer *rhoProd = new anaRhoProducer("anaRhoProducerKTR040","anaRhoProducerKTR040");
  rhoProd->ConnectEventObject(fEventObjects);
  rhoProd->SetJetsName("JetsKTR040");
  rhoProd->SetHiEvtName("hiEventContainer");
  rhoProd->SetTriggerMapName("triggerMap");
  rhoProd->AddTriggerSel("HLT_HIL1MinimumBiasHF1AND_v1");
  rhoProd->DoHBHENoiseFilter(true);
  rhoProd->DoHFCoincFilter(true);
  rhoProd->SetNExcl(2);
  rhoProd->SetEtaRangeAll(-5.+0.2,5.-0.2);
  rhoProd->SetEtaLimit(1,-5.);//bin,eta
  rhoProd->SetEtaLimit(2,-3.);
  rhoProd->SetEtaLimit(3,-2.1);
  rhoProd->SetEtaLimit(4,-1.3);
  rhoProd->SetEtaLimit(5, 1.3);
  rhoProd->SetEtaLimit(6, 2.1);
  rhoProd->SetEtaLimit(7, 3.);
  rhoProd->SetEtaLimit(8, 5.);
  handler->Add(rhoProd);

  anaRhoProducer *rhoProdV2 = new anaRhoProducer("anaRhoProducerKTR040V2","anaRhoProducerKTR040V2");
  rhoProdV2->ConnectEventObject(fEventObjects);
  rhoProdV2->SetJetsName("JetsKTR040");
  rhoProdV2->SetHiEvtName("hiEventContainer");
  rhoProdV2->SetTriggerMapName("triggerMap");
  rhoProdV2->AddTriggerSel("HLT_HIL1MinimumBiasHF1AND_v1");
  rhoProdV2->DoHBHENoiseFilter(true);
  rhoProdV2->DoHFCoincFilter(true);
  rhoProdV2->SetNExcl(2);
  rhoProdV2->SetEtaRangeAll(-5.+0.2,5.-0.2);
  rhoProdV2->SetEtaLimit(1,-5.);//bin,eta
  rhoProdV2->SetEtaLimit(2,-3.);
  rhoProdV2->SetEtaLimit(3,-2.);
  rhoProdV2->SetEtaLimit(4,-1.5);
  rhoProdV2->SetEtaLimit(5,-1.);
  rhoProdV2->SetEtaLimit(6, 1.);
  rhoProdV2->SetEtaLimit(7, 1.5);
  rhoProdV2->SetEtaLimit(8, 2.);
  rhoProdV2->SetEtaLimit(9, 3.);
  rhoProdV2->SetEtaLimit(10, 5.);
  handler->Add(rhoProdV2);

  //with min pt cut on constituents
  LWJetProducer *lwjktptmin = new LWJetProducer("LWJetProducerKTR040PtMin100","LWJetProducerKTR040PtMin100");
  lwjktptmin->ConnectEventObject(fEventObjects);
  lwjktptmin->SetJetType(LWJetProducer::kKT);
  lwjktptmin->SetRadius(0.4);
  lwjktptmin->SetGhostArea(0.005);
  lwjktptmin->SetPtMinConst(1.);
  //lwjktptmin->SetIdConst(4);//charged
  lwjktptmin->SetParticlesName("pfParticles");
  lwjktptmin->SetJetContName("JetsKTR040PtMin100");
  lwjktptmin->SetDoConstituentSubtraction(kFALSE);
  handler->Add(lwjktptmin);

  anaRhoProducer *rhoProdPtMin100 = new anaRhoProducer("anaRhoProducerKTR040PtMin100","anaRhoProducerKTR040PtMin100");
  rhoProdPtMin100->ConnectEventObject(fEventObjects);
  rhoProdPtMin100->SetJetsName("JetsKTR040PtMin100");
  rhoProdPtMin100->SetHiEvtName("hiEventContainer");
  rhoProdPtMin100->SetTriggerMapName("triggerMap");
  rhoProdPtMin100->AddTriggerSel("HLT_HIL1MinimumBiasHF1AND_v1");
  rhoProdPtMin100->DoHBHENoiseFilter(true);
  rhoProdPtMin100->DoHFCoincFilter(true);
  rhoProdPtMin100->SetNExcl(2);
  rhoProdPtMin100->SetEtaRangeAll(-5.+0.2,5.-0.2);
  rhoProdPtMin100->SetEtaLimit(1,-5.);//bin,eta
  rhoProdPtMin100->SetEtaLimit(2,-3.);
  rhoProdPtMin100->SetEtaLimit(3,-2.1);
  rhoProdPtMin100->SetEtaLimit(4,-1.3);
  rhoProdPtMin100->SetEtaLimit(5, 1.3);
  rhoProdPtMin100->SetEtaLimit(6, 2.1);
  rhoProdPtMin100->SetEtaLimit(7, 3.);
  rhoProdPtMin100->SetEtaLimit(8, 5.);
  handler->Add(rhoProdPtMin100);

  anaRhoProducer *rhoProdV2PtMin100 = new anaRhoProducer("anaRhoProducerKTR040V2PtMin100","anaRhoProducerKTR040V2PtMin100");
  rhoProdV2PtMin100->ConnectEventObject(fEventObjects);
  rhoProdV2PtMin100->SetJetsName("JetsKTR040PtMin100");
  rhoProdV2PtMin100->SetHiEvtName("hiEventContainer");
  rhoProdV2PtMin100->SetTriggerMapName("triggerMap");
  rhoProdV2PtMin100->AddTriggerSel("HLT_HIL1MinimumBiasHF1AND_v1");
  rhoProdV2PtMin100->DoHBHENoiseFilter(true);
  rhoProdV2PtMin100->DoHFCoincFilter(true);
  rhoProdV2PtMin100->SetNExcl(2);
  rhoProdV2PtMin100->SetEtaRangeAll(-5.+0.2,5.-0.2);
  rhoProdV2PtMin100->SetEtaLimit(1,-5.);//bin,eta
  rhoProdV2PtMin100->SetEtaLimit(2,-3.);
  rhoProdV2PtMin100->SetEtaLimit(3,-2.);
  rhoProdV2PtMin100->SetEtaLimit(4,-1.5);
  rhoProdV2PtMin100->SetEtaLimit(5,-1.);
  rhoProdV2PtMin100->SetEtaLimit(6, 1.);
  rhoProdV2PtMin100->SetEtaLimit(7, 1.5);
  rhoProdV2PtMin100->SetEtaLimit(8, 2.);
  rhoProdV2PtMin100->SetEtaLimit(9, 3.);
  rhoProdV2PtMin100->SetEtaLimit(10, 5.);
  handler->Add(rhoProdV2PtMin100);

  LWJetProducer *lwjktR020 = new LWJetProducer("LWJetProducerKTR020","LWJetProducerKTR020");
  lwjktR020->ConnectEventObject(fEventObjects);
  lwjktR020->SetJetType(LWJetProducer::kKT);
  lwjktR020->SetRadius(0.4);
  lwjktR020->SetGhostArea(0.005);
  lwjktR020->SetPtMinConst(0.);
  //lwjktR020->SetIdConst(1);//charged
  lwjktR020->SetParticlesName("pfParticles");
  lwjktR020->SetJetContName("JetsKTR020");
  lwjktR020->SetDoConstituentSubtraction(kFALSE);
  handler->Add(lwjktR020);

  anaRhoProducer *rhoProdR020 = new anaRhoProducer("anaRhoProducerKTR020","anaRhoProducerKTR020");
  rhoProdR020->ConnectEventObject(fEventObjects);
  rhoProdR020->SetRhoName("rhoMapR020");
  rhoProdR020->SetRhoMName("rhoMMapR020");
  rhoProdR020->SetJetsName("JetsKTR020");
  rhoProdR020->SetHiEvtName("hiEventContainer");
  rhoProdR020->SetTriggerMapName("triggerMap");
  rhoProdR020->AddTriggerSel("HLT_HIL1MinimumBiasHF1AND_v1");
  rhoProdR020->DoHBHENoiseFilter(true);
  rhoProdR020->DoHFCoincFilter(true);
  rhoProdR020->SetNExcl(2);
  rhoProdR020->SetEtaRangeAll(-5.+0.2,5.-0.2);
  rhoProdR020->SetEtaLimit(1,-5.);//bin,eta
  rhoProdR020->SetEtaLimit(2,-3.);
  rhoProdR020->SetEtaLimit(3,-2.1);
  rhoProdR020->SetEtaLimit(4,-1.3);
  rhoProdR020->SetEtaLimit(5, 1.3);
  rhoProdR020->SetEtaLimit(6, 2.1);
  rhoProdR020->SetEtaLimit(7, 3.);
  rhoProdR020->SetEtaLimit(8, 5.);
  handler->Add(rhoProdR020);

  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntries(); //93064
  Printf("total number of events: %lld",entries_tot);
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld firstEvent: %d  lastEvent: %d",nentries,firstEvent,lastEvent);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    if(jentry%1000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    bool suc = p_evt->Run(jentry);   //hi event properties
    if(!suc) Printf("something wrong with hiEventProducer");
    p_trg->Run(jentry);
    //Printf("produce pf particles");
    p_pf->Run(jentry);    //pf particles
	    
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

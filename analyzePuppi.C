#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
//#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/pfParticleProducerVector.h"
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
#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

bool doPuppi         = true;
bool doJECPuppi      = true;
bool doJetFinding    = true;
bool useMetric2      = false;
bool storeTree       = false;
bool doCSJets        = false;//true;
bool doJECCS         = false;//true;
bool doZJetResponse  = false;
bool doRhoVariation  = false;
double alphaCS       = 1.; 

TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/jetsPbPb/puppi/perf/jec";

void analyzePuppi(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int ptminType = 0, int jetSignalType = 0, int weightType = 0) {

  TString strL1 = Form("%s/75X_mcRun2_HeavyIon_v12_L1FastJet_AK4PF_offline.txt",baseJEC.Data());
  TString strL2Rel = Form("%s/75X_mcRun2_HeavyIon_v12_L2Relative_AK4PF_offline.txt",baseJEC.Data());
  TString strL3Abs = Form("%s/75X_mcRun2_HeavyIon_v12_L3Absolute_AK4PF_offline.txt",baseJEC.Data());
  TString strL2L3Res = Form("%s/75X_mcRun2_HeavyIon_v12_L2L3Residual_AK4PF_offline.txt",baseJEC.Data());
  
  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)

    weightType: metric used for puppi
    0: alpha (pt/r^2)
    1: alpha2 (pt/r)
    2: sumpt
    3: meanpt
    4: metric2 (mass)
   */

  double ptMinPuppi = 0.;
  if(ptminType==0) ptMinPuppi = 1e-4;
  else if(ptminType==1) ptMinPuppi = 1.;
  else if(ptminType==2) ptMinPuppi = 2.;

  TString signalJets = "aktPUR040";
  if(jetSignalType==0) signalJets = "aktPUR040";
  else if(jetSignalType==1) signalJets = "akt4Gen";
  std::cout << "signalJets: " << signalJets << std::endl; 
 
  // std::vector<std::string> urls = CollectFiles(list);

  // Printf("anaFile: %d",anaFile);
  
  std::cout << "nfiles: " << urls.size() << std::endl;
  //  for (auto i = urls.begin(); i != urls.end(); ++i)
  //std::cout << *i << std::endl;

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

  TChain *csJetTree = new TChain("akCs4PFJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) csJetTree->Add(urls[i].c_str());
  chain->AddFriend(csJetTree);
  Printf("csJetTree done");

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

  /*
  pfParticleProducer *p_pf = new pfParticleProducer("pfPartProd");
  p_pf->SetInput(chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);
  */

  pfParticleProducerVector *p_pf = new pfParticleProducerVector("pfPartProd");
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

  lwJetFromForestProducer *p_CSJet = new lwJetFromForestProducer("lwJetForestProdCS");
  p_CSJet->SetInput(csJetTree);
  p_CSJet->SetJetContName("aktCSR040");
  p_CSJet->SetGenJetContName("");
  p_CSJet->SetEventObjects(fEventObjects);
  p_CSJet->SetRadius(0.4);
  
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
  //pupProd->SetAddMetricType(anaPuppiProducer::kMass);
  pupProd->SetPtMinParticle(ptMinPuppi);//1.);
  pupProd->SetStoreTree(storeTree);
  pupProd->SetConeRadius(0.3);
  if(weightType==0)
    pupProd->SetPuppiWeightType(anaPuppiProducer::kAlpha);
  else if(weightType==1)
    pupProd->SetPuppiWeightType(anaPuppiProducer::kAlpha2);
  else if(weightType==2)
    pupProd->SetPuppiWeightType(anaPuppiProducer::kSumPt);
  else if(weightType==3)
    pupProd->SetPuppiWeightType(anaPuppiProducer::kMeanPt);
  else if(weightType==4)
    pupProd->SetPuppiWeightType(anaPuppiProducer::kMetric2);
  pupProd->SetWeightCut(0.01);
  if(doPuppi) handler->Add(pupProd);

  //anti-kt jet finder on reconstructed PUPPI particles ptmin=0
  LWJetProducer *lwjakt = new LWJetProducer("LWJetProducerAKTR040Puppi","LWJetProducerAKTR040Puppi");
  lwjakt->ConnectEventObject(fEventObjects);
  lwjakt->SetJetType(LWJetProducer::kAKT);
  lwjakt->SetRadius(0.4);
  lwjakt->SetGhostArea(0.005);
  lwjakt->SetPtMinConst(ptMinPuppi);//0.);
  lwjakt->SetParticlesName("puppiParticles");
  lwjakt->SetJetContName("JetsAKTR040Puppi");
  if(doJECPuppi) {
    lwjakt->SetDoJECCS(true);
    lwjakt->SetL1Fastjet(strL1);
    lwjakt->SetL2Relative(strL2Rel);
    lwjakt->SetL3Absolute(strL3Abs);
  }
  if(doPuppi && doJetFinding) handler->Add(lwjakt);

  //anti-kt jet finder on generated particles
  LWJetProducer *lwjaktGen = new LWJetProducer("LWGenJetProducerAKTR040","LWGenJetProducerAKTR040");
  lwjaktGen->ConnectEventObject(fEventObjects);
  lwjaktGen->SetJetType(LWJetProducer::kAKT);
  lwjaktGen->SetRadius(0.4);
  lwjaktGen->SetGhostArea(0.005);
  lwjaktGen->SetPtMinConst(0.);
  lwjaktGen->SetParticlesName("genParticles");
  lwjaktGen->SetJetContName("GenJetsAKTR040");
  //if(doJetFinding) handler->Add(lwjaktGen);
 
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
  if(doPuppi && doJetFinding) handler->Add(match);

  anaJetEnergyScale *anajes = new anaJetEnergyScale("anaJES","anaJES");
  anajes->ConnectEventObject(fEventObjects);
  anajes->SetHiEvtName("hiEventContainer");
  anajes->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
  anajes->SetRecJetsName("JetsAKTR040Puppi");
  anajes->SetNCentBins(4);
  if(doPuppi && doJetFinding) handler->Add(anajes);
  
  anaJetMatching *matchGen = new anaJetMatching("jetMatchingGenGen","jetMatchingGenGen");
  matchGen->ConnectEventObject(fEventObjects);
  matchGen->SetHiEvtName("hiEventContainer");
  matchGen->SetJetsNameBase("GenJetsAKTR040"); //gen jets from gen particles
  matchGen->SetJetsNameTag("akt4Gen");         //forest gen jets
  //if(doJetFinding) handler->Add(matchGen);
 
  anaJetEnergyScale *anajesgen = new anaJetEnergyScale("anajesgen","anajesgen");
  anajesgen->ConnectEventObject(fEventObjects);
  anajesgen->SetHiEvtName("hiEventContainer");
  anajesgen->SetGenJetsName("akt4Gen");
  anajesgen->SetRecJetsName("GenJetsAKTR040");
  anajesgen->SetNCentBins(4);
  //if(doJetFinding) handler->Add(anajesgen);

  if(doCSJets || doRhoVariation) {
    //kt jet finder
    LWJetProducer *lwjkt = new LWJetProducer("LWJetProducerKT","LWJetProducerKT");
    lwjkt->ConnectEventObject(fEventObjects);
    lwjkt->SetJetType(LWJetProducer::kKT);
    lwjkt->SetRadius(0.2);
    lwjkt->SetGhostArea(0.005);
    lwjkt->SetPtMinConst(0.);
    lwjkt->SetParticlesName("pfParticles");
    lwjkt->SetJetContName("JetsKTR020");
    lwjkt->SetDoConstituentSubtraction(kFALSE);
    handler->Add(lwjkt);
  }
  if(doCSJets) {
    anaRhoProducer *rhoProd = new anaRhoProducer("anaRhoProducerKTR020","anaRhoProducerKTR020");
    rhoProd->ConnectEventObject(fEventObjects);
    rhoProd->SetJetsName("JetsKTR020");
    rhoProd->SetHiEvtName("hiEventContainer");
    rhoProd->SetNExcl(2);
    rhoProd->SetMinPtExcl(0.);
    rhoProd->SetEtaRangeExcl(-10.,10.);
    rhoProd->SetEtaRangeAll(-5.,5.);
    rhoProd->SetEtaLimit(1,-5.);//bin,eta
    rhoProd->SetEtaLimit(2,-3.);
    rhoProd->SetEtaLimit(3,-2.1);
    rhoProd->SetEtaLimit(4,-1.3);
    rhoProd->SetEtaLimit(5, 1.3);
    rhoProd->SetEtaLimit(6, 2.1);
    rhoProd->SetEtaLimit(7, 3.);
    rhoProd->SetEtaLimit(8, 5.);
    rhoProd->SetUseMean(false);//true);
    handler->Add(rhoProd);

    //anti-kt jet finder on reconstructed pf candidates
    LWJetProducer *lwjaktCS = new LWJetProducer("LWCSJetProducerAKTR040","LWCSJetProducerAKTR040");
    lwjaktCS->ConnectEventObject(fEventObjects);
    lwjaktCS->SetJetType(LWJetProducer::kAKT);
    lwjaktCS->SetRadius(0.4);
    lwjaktCS->SetGhostArea(0.005);
    lwjaktCS->SetPtMinConst(0.);
    lwjaktCS->SetParticlesName("pfParticles");
    lwjaktCS->SetJetContName("JetsAKTR040");
    lwjaktCS->SetCSJetContName("JetsAKTR040CS");
    lwjaktCS->SetDoConstituentSubtraction(kTRUE);
    lwjaktCS->SetAlpha(alphaCS);
    if(doJECCS) {
      lwjaktCS->SetDoJECCS(true);
      lwjaktCS->SetL1Fastjet(strL1);
      lwjaktCS->SetL2Relative(strL2Rel);
      lwjaktCS->SetL3Absolute(strL3Abs);
      //lwjaktCS->SetL2L3Residual(strL2L3Res);
    }
    lwjaktCS->SetRhoMapName("rhoMap");
    lwjaktCS->SetRhoMMapName("rhoMMap");
    handler->Add(lwjaktCS);
    
    anaJetMatching *matchCS = new anaJetMatching("jetMatchingCS","jetMatchingCS");
    matchCS->ConnectEventObject(fEventObjects);
    matchCS->SetHiEvtName("hiEventContainer");
    matchCS->SetJetsNameBase("JetsAKTR040CS");
    matchCS->SetJetsNameTag("akt4Gen");//GenJetsAKTR040");
    handler->Add(matchCS);

    anaJetEnergyScale *anajesCSMaxDist04 = new anaJetEnergyScale("anajesCSMaxDist04","anajesCSMaxDist04");
    anajesCSMaxDist04->ConnectEventObject(fEventObjects);
    anajesCSMaxDist04->SetHiEvtName("hiEventContainer");
    anajesCSMaxDist04->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
    anajesCSMaxDist04->SetRecJetsName("JetsAKTR040CS");
    anajesCSMaxDist04->SetNCentBins(4);
    anajesCSMaxDist04->SetMaxDistance(0.4);
    handler->Add(anajesCSMaxDist04);

    anaJetEnergyScale *anajesCSMaxDist03 = new anaJetEnergyScale("anajesCSMaxDist03","anajesCSMaxDist03");
    anajesCSMaxDist03->ConnectEventObject(fEventObjects);
    anajesCSMaxDist03->SetHiEvtName("hiEventContainer");
    anajesCSMaxDist03->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
    anajesCSMaxDist03->SetRecJetsName("JetsAKTR040CS");
    anajesCSMaxDist03->SetNCentBins(4);
    anajesCSMaxDist03->SetMaxDistance(0.3);
    handler->Add(anajesCSMaxDist03);

    anaJetEnergyScale *anajesCSMaxDist02 = new anaJetEnergyScale("anajesCSMaxDist02","anajesCSMaxDist02");
    anajesCSMaxDist02->ConnectEventObject(fEventObjects);
    anajesCSMaxDist02->SetHiEvtName("hiEventContainer");
    anajesCSMaxDist02->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
    anajesCSMaxDist02->SetRecJetsName("JetsAKTR040CS");
    anajesCSMaxDist02->SetNCentBins(4);
    anajesCSMaxDist02->SetMaxDistance(0.2);
    handler->Add(anajesCSMaxDist02);

    anaJetEnergyScale *anajesCSMaxDist01 = new anaJetEnergyScale("anajesCSMaxDist01","anajesCSMaxDist01");
    anajesCSMaxDist01->ConnectEventObject(fEventObjects);
    anajesCSMaxDist01->SetHiEvtName("hiEventContainer");
    anajesCSMaxDist01->SetGenJetsName("akt4Gen");//GenJetsAKTR040");
    anajesCSMaxDist01->SetRecJetsName("JetsAKTR040CS");
    anajesCSMaxDist01->SetNCentBins(4);
    anajesCSMaxDist01->SetMaxDistance(0.1);
    handler->Add(anajesCSMaxDist01);

    anaJetMatching *matchRaw = new anaJetMatching("jetMatchingRaw","jetMatchingRaw");
    matchRaw->ConnectEventObject(fEventObjects);
    matchRaw->SetHiEvtName("hiEventContainer");
    matchRaw->SetJetsNameBase("JetsAKTR040");
    matchRaw->SetJetsNameTag("akt4Gen");
    handler->Add(matchRaw);
    
    anaJetEnergyScale *anajesRaw = new anaJetEnergyScale("anajesRaw","anajesRaw");
    anajesRaw->ConnectEventObject(fEventObjects);
    anajesRaw->SetHiEvtName("hiEventContainer");
    anajesRaw->SetGenJetsName("akt4Gen");
    anajesRaw->SetRecJetsName("JetsAKTR040");
    anajesRaw->SetNCentBins(4);
    anajesRaw->SetRhoMapName("rhoMap");
    anajesRaw->SetMaxDistance(0.4);
    handler->Add(anajesRaw);

    anaJetMatching *matchCSCS = new anaJetMatching("jetMatchingCSCS","jetMatchingCSCS");
    matchCSCS->ConnectEventObject(fEventObjects);
    matchCSCS->SetHiEvtName("hiEventContainer");
    matchCSCS->SetJetsNameBase("JetsAKTR040CS");
    matchCSCS->SetJetsNameTag("aktCSR040");
    handler->Add(matchCSCS);

    anaJetEnergyScale *anajesCSCS = new anaJetEnergyScale("anajesCSCS","anajesCSCS");
    anajesCSCS->ConnectEventObject(fEventObjects);
    anajesCSCS->SetHiEvtName("hiEventContainer");
    anajesCSCS->SetGenJetsName("aktCSR040");
    anajesCSCS->SetRecJetsName("JetsAKTR040CS");
    anajesCSCS->SetNCentBins(4);
    anajesCSCS->SetMaxDistance(0.4);
    handler->Add(anajesCSCS);

    if(doZJetResponse) {
      //Z to mumu
      anaZToMuMu *ZToMuMu = new anaZToMuMu("ZToMuMu","ZToMuMu");
      ZToMuMu->ConnectEventObject(fEventObjects);
      ZToMuMu->SetHiEvtName("hiEventContainer");
      ZToMuMu->SetMuonsName("genParticles");
      ZToMuMu->SetCheckPid(true);
      ZToMuMu->SetZsName("zMuMuBosons");
      handler->Add(ZToMuMu);

      anaZJetMCResponse *ZResp = new anaZJetMCResponse("ZJetMCResponse","ZJetMCResponse");
      ZResp->ConnectEventObject(fEventObjects);
      ZResp->SetHiEvtName("hiEventContainer");
      ZResp->SetZsName("zMuMuBosons");
      ZResp->SetJetsName("JetsAKTR040CS");
      ZResp->SetGenJetsName("akt4Gen");
      ZResp->SetUseForestMatching(false);
      handler->Add(ZResp);
    }
  }

  if(doRhoVariation) {
    Printf("doRhoVariation");
    if(!doCSJets) {
      //anti-kt jet finder on reconstructed pf candidates
      LWJetProducer *lwjaktRaw = new LWJetProducer("LWRawJetProducerAKTR040","LWRawJetProducerAKTR040");
      lwjaktRaw->ConnectEventObject(fEventObjects);
      lwjaktRaw->SetJetType(LWJetProducer::kAKT);
      lwjaktRaw->SetRadius(0.4);
      lwjaktRaw->SetGhostArea(0.005);
      lwjaktRaw->SetPtMinConst(0.);
      lwjaktRaw->SetParticlesName("pfParticles");
      lwjaktRaw->SetJetContName("JetsAKTR040");
      lwjaktRaw->SetDoConstituentSubtraction(kFALSE);
      handler->Add(lwjaktRaw);
     
      anaJetMatching *matchRaw = new anaJetMatching("jetMatchingRaw","jetMatchingRaw");
      matchRaw->ConnectEventObject(fEventObjects);
      matchRaw->SetHiEvtName("hiEventContainer");
      matchRaw->SetJetsNameBase("JetsAKTR040");
      matchRaw->SetJetsNameTag("akt4Gen");
      handler->Add(matchRaw);
    }
      
    anaRhoProducer *rhoProdEta2 = new anaRhoProducer("anaRhoProducerKTR020Eta2","anaRhoProducerKTR020Eta2");
    rhoProdEta2->ConnectEventObject(fEventObjects);
    rhoProdEta2->SetJetsName("JetsKTR020");
    rhoProdEta2->SetHiEvtName("hiEventContainer");
    rhoProdEta2->SetNExcl(2);
    rhoProdEta2->SetMinPtExcl(0.);
    rhoProdEta2->SetEtaRangeExcl(-2.,2.);
    rhoProdEta2->SetEtaRangeAll(-5.+0.2,5.-0.2);
    rhoProdEta2->SetEtaLimit(1,-5.);//bin,eta
    rhoProdEta2->SetEtaLimit(2,-3.);
    rhoProdEta2->SetEtaLimit(3,-2.1);
    rhoProdEta2->SetEtaLimit(4,-1.3);
    rhoProdEta2->SetEtaLimit(5, 1.3);
    rhoProdEta2->SetEtaLimit(6, 2.1);
    rhoProdEta2->SetEtaLimit(7, 3.);
    rhoProdEta2->SetEtaLimit(8, 5.);
    rhoProdEta2->SetUseMean(false);//true);
    rhoProdEta2->SetRhoName("rhoMapEta2");
    handler->Add(rhoProdEta2);
    
    anaJetEnergyScale *anajesRawEta2 = new anaJetEnergyScale("anajesRawEta2","anajesRawEta2");
    anajesRawEta2->ConnectEventObject(fEventObjects);
    anajesRawEta2->SetHiEvtName("hiEventContainer");
    anajesRawEta2->SetGenJetsName("akt4Gen");
    anajesRawEta2->SetRecJetsName("JetsAKTR040");
    anajesRawEta2->SetNCentBins(4);
    anajesRawEta2->SetRhoMapName("rhoMapEta2");
    anajesRawEta2->SetMaxDistance(0.4);
    handler->Add(anajesRawEta2);

    anaRhoProducer *rhoProdEta13 = new anaRhoProducer("anaRhoProducerKTR020Eta13","anaRhoProducerKTR020Eta13");
    rhoProdEta13->ConnectEventObject(fEventObjects);
    rhoProdEta13->SetJetsName("JetsKTR020");
    rhoProdEta13->SetHiEvtName("hiEventContainer");
    rhoProdEta13->SetNExcl(2);
    rhoProdEta13->SetMinPtExcl(20.);
    rhoProdEta13->SetEtaRangeExcl(-1.3,1.3);
    rhoProdEta13->SetEtaRangeAll(-5.+0.2,5.-0.2);
    rhoProdEta13->SetEtaLimit(1,-5.);//bin,eta
    rhoProdEta13->SetEtaLimit(2,-3.);
    rhoProdEta13->SetEtaLimit(3,-2.1);
    rhoProdEta13->SetEtaLimit(4,-1.3);
    rhoProdEta13->SetEtaLimit(5, 1.3);
    rhoProdEta13->SetEtaLimit(6, 2.1);
    rhoProdEta13->SetEtaLimit(7, 3.);
    rhoProdEta13->SetEtaLimit(8, 5.);
    rhoProdEta13->SetUseMean(false);//true);
    rhoProdEta13->SetRhoName("rhoMapEta13");
    //  handler->Add(rhoProdEta13);
    
    anaJetEnergyScale *anajesRawEta13 = new anaJetEnergyScale("anajesRawEta13","anajesRawEta13");
    anajesRawEta13->ConnectEventObject(fEventObjects);
    anajesRawEta13->SetHiEvtName("hiEventContainer");
    anajesRawEta13->SetGenJetsName("akt4Gen");
    anajesRawEta13->SetRecJetsName("JetsAKTR040");
    anajesRawEta13->SetNCentBins(4);
    anajesRawEta13->SetRhoMapName("rhoMapEta13");
    anajesRawEta13->SetMaxDistance(0.4);
    // handler->Add(anajesRawEta13);

    anaRhoProducer *rhoProdEta3Ex5 = new anaRhoProducer("anaRhoProducerKTR020Eta3Ex5","anaRhoProducerKTR020Eta3Ex5");
    rhoProdEta3Ex5->ConnectEventObject(fEventObjects);
    rhoProdEta3Ex5->SetJetsName("JetsKTR020");
    rhoProdEta3Ex5->SetHiEvtName("hiEventContainer");
    rhoProdEta3Ex5->SetNExcl(5);
    rhoProdEta3Ex5->SetMinPtExcl(20.);
    rhoProdEta3Ex5->SetEtaRangeExcl(-3.,3.);
    rhoProdEta3Ex5->SetEtaRangeAll(-5.+0.2,5.-0.2);
    rhoProdEta3Ex5->SetEtaLimit(1,-5.);//bin,eta
    rhoProdEta3Ex5->SetEtaLimit(2,-3.);
    rhoProdEta3Ex5->SetEtaLimit(3,-2.1);
    rhoProdEta3Ex5->SetEtaLimit(4,-1.3);
    rhoProdEta3Ex5->SetEtaLimit(5, 1.3);
    rhoProdEta3Ex5->SetEtaLimit(6, 2.1);
    rhoProdEta3Ex5->SetEtaLimit(7, 3.);
    rhoProdEta3Ex5->SetEtaLimit(8, 5.);
    rhoProdEta3Ex5->SetUseMean(false);//true);
    rhoProdEta3Ex5->SetRhoName("rhoMapEta3Ex5");
    //  handler->Add(rhoProdEta3Ex5);
    
    anaJetEnergyScale *anajesRawEta3Ex5 = new anaJetEnergyScale("anajesRawEta3Ex5","anajesRawEta3Ex5");
    anajesRawEta3Ex5->ConnectEventObject(fEventObjects);
    anajesRawEta3Ex5->SetHiEvtName("hiEventContainer");
    anajesRawEta3Ex5->SetGenJetsName("akt4Gen");
    anajesRawEta3Ex5->SetRecJetsName("JetsAKTR040");
    anajesRawEta3Ex5->SetNCentBins(4);
    anajesRawEta3Ex5->SetRhoMapName("rhoMapEta3Ex5");
    anajesRawEta3Ex5->SetMaxDistance(0.4);
    //  handler->Add(anajesRawEta3Ex5);

    
    anaRhoProducer *rhoProdEta10Ex3 = new anaRhoProducer("anaRhoProducerKTR020Eta10Ex3","anaRhoProducerKTR020Eta10Ex3");
    rhoProdEta10Ex3->ConnectEventObject(fEventObjects);
    rhoProdEta10Ex3->SetJetsName("JetsKTR020");
    rhoProdEta10Ex3->SetHiEvtName("hiEventContainer");
    rhoProdEta10Ex3->SetNExcl(3);
    rhoProdEta10Ex3->SetMinPtExcl(20.);
    rhoProdEta10Ex3->SetEtaRangeExcl(-1.,1.);
    rhoProdEta10Ex3->SetEtaRangeAll(-5.+0.2,5.-0.2);
    rhoProdEta10Ex3->SetEtaLimit(1,-5.);//bin,eta
    rhoProdEta10Ex3->SetEtaLimit(2,-3.);
    rhoProdEta10Ex3->SetEtaLimit(3,-2.1);
    rhoProdEta10Ex3->SetEtaLimit(4,-1.3);
    rhoProdEta10Ex3->SetEtaLimit(5, 1.3);
    rhoProdEta10Ex3->SetEtaLimit(6, 2.1);
    rhoProdEta10Ex3->SetEtaLimit(7, 3.);
    rhoProdEta10Ex3->SetEtaLimit(8, 5.);
    rhoProdEta10Ex3->SetUseMean(false);//true);
    rhoProdEta10Ex3->SetRhoName("rhoMapEta10Ex3");
    //  handler->Add(rhoProdEta10Ex3);
    
    anaJetEnergyScale *anajesRawEta10Ex3 = new anaJetEnergyScale("anajesRawEta10Ex3","anajesRawEta10Ex3");
    anajesRawEta10Ex3->ConnectEventObject(fEventObjects);
    anajesRawEta10Ex3->SetHiEvtName("hiEventContainer");
    anajesRawEta10Ex3->SetGenJetsName("akt4Gen");
    anajesRawEta10Ex3->SetRecJetsName("JetsAKTR040");
    anajesRawEta10Ex3->SetNCentBins(4);
    anajesRawEta10Ex3->SetRhoMapName("rhoMapEta10Ex3");
    anajesRawEta10Ex3->SetMaxDistance(0.4);
    // handler->Add(anajesRawEta10Ex3);

  }

  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntries(); //93064
  Printf("total number of events: %lld",entries_tot);
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld firstEvent: %d  lastEvent: %d",nentries,firstEvent,lastEvent);
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
    p_CSJet->Run(jentry);
    
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

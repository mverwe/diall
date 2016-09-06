
#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/jewelZgReweight.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducerVector.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/subjetSmearing.h"
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
#include "UserCode/diall/interface/anaSubJet.h"
#include "UserCode/diall/interface/anaZToMuMu.h"
#include "UserCode/diall/interface/anaZgHistos.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;

TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/jetsPbPb/puppi/perf/jec";

void analyzeSoftdrop(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int activateJetShift = 0, int activateZgReweight = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */

  
  TString strL1 = Form("%s/75X_mcRun2_HeavyIon_v12_L1FastJet_AK4PF_offline.txt",baseJEC.Data());
  TString strL2Rel = Form("%s/75X_mcRun2_HeavyIon_v12_L2Relative_AK4PF_offline.txt",baseJEC.Data());
  TString strL3Abs = Form("%s/75X_mcRun2_HeavyIon_v12_L3Absolute_AK4PF_offline.txt",baseJEC.Data());
  TString strL2L3Res = Form("%s/75X_mcRun2_HeavyIon_v12_L2L3Residual_AK4PF_offline.txt",baseJEC.Data());

  TString jetSDName = "akt4PFSD";
  TString jetTreeSDName = "akSoftDrop4PFJetAnalyzer";
  TString jetSDChName = "akt4PFSDCh";
  TString jetTreeSDChName = "akSoftDropCh4PFJetAnalyzer";
  TString jetName = "akt4PF";
  TString jetTreeName = "ak4PFJetAnalyzer";

  bool doJetShift = false;
  if(activateJetShift>0) doJetShift = true;
  float jetShift = 0.02;
  if(activateJetShift==1) jetShift = 0.02;
  if(activateJetShift==2) jetShift = -0.02;
  
  double minptjet = 30.;
  bool doDijet = false;
  if(!doDijet) minptjet = 40.;

  std::cout << "analyzing subjets for: " << jetName << " tree: " << jetTreeName << std::endl;
  std::cout << "analyzing subjets for: " << jetSDName << " tree: " << jetTreeSDName << std::endl;
   
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
  chain = new TChain("hiEvtAnalyzer/HiTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("hiTree done");

  TChain *jetTreeSD = new TChain(Form("%s/t",jetTreeSDName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTreeSD->Add(urls[i].c_str());
  chain->AddFriend(jetTreeSD);
  Printf("jetTreeSD done");

  TChain *jetTree = new TChain(Form("%s/t",jetTreeName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  Printf("jetTree done");

  TChain *rhoTree = new TChain("hiFJRhoAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) rhoTree->Add(urls[i].c_str());
  chain->AddFriend(rhoTree);

  TChain *pfTree = new TChain("pfcandAnalyzer/pfTree");
  for(size_t i=firstFile; i<lastFile; i++) pfTree->Add(urls[i].c_str());
  //chain->AddFriend(pfTree);
  Printf("pfTree done");
  
  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  pfParticleProducerVector *p_pf = new pfParticleProducerVector("pfPartProd");
  p_pf->SetInput(pfTree);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);
  
  lwJetFromForestProducer *p_SDJet = new lwJetFromForestProducer("lwJetForestProdSD");
  p_SDJet->SetInput(chain);
  p_SDJet->SetJetContName(jetSDName);
  p_SDJet->SetGenJetContName("akt4Gen");
  p_SDJet->SetEventObjects(fEventObjects);
  p_SDJet->SetMinJetPt(minptjet);
  p_SDJet->SetRadius(0.4);

  lwJetFromForestProducer *p_Jet = new lwJetFromForestProducer("lwJetForestProd");
  p_Jet->SetInput(jetTree);
  p_Jet->SetJetContName(jetName);
  p_Jet->SetGenJetContName("");//akt4Gen");
  p_Jet->SetEventObjects(fEventObjects);
  p_Jet->SetMinJetPt(minptjet);
  p_Jet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  //anti-kt jet finder on reconstructed pf candidates
  LWJetProducer *lwjaktCA = new LWJetProducer("LWJetProducerAKTR040","LWJetProducerAKTR040");
  //lwjaktCA->SetInput(chain);
  lwjaktCA->ConnectEventObject(fEventObjects);
  lwjaktCA->SetJetType(LWJetProducer::kAKT);
  lwjaktCA->SetRadius(0.4);
  lwjaktCA->SetGhostArea(0.01);
  //lwjaktCA->SetGhostArea(0.005);
  lwjaktCA->SetPtMinConst(0.);
  lwjaktCA->SetParticlesName("pfParticles");
  lwjaktCA->SetJetContName("JetsAKTR040SDCA");
  lwjaktCA->SetDoConstituentSubtraction(kFALSE);
  lwjaktCA->SetDoSoftDrop(true);
  lwjaktCA->SetUseKtForSoftDrop(false);
  lwjaktCA->SetSoftDropZCut(0.1);
  lwjaktCA->SetSoftDropBeta(0.);
  lwjaktCA->SetDoJEC(false);//
  //lwjaktCA->SetL1Fastjet(strL1);
  //lwjaktCA->SetL2Relative(strL2Rel);
  //lwjaktCA->SetL3Absolute(strL3Abs);
  handler->Add(lwjaktCA);

  jetSDName = "JetsAKTR040SDCA";

  LWJetProducer *lwjaktKT = new LWJetProducer("LWJetProducerAKTR040","LWJetProducerAKTR040");
  //lwjaktKT->SetInput(chain);
  lwjaktKT->ConnectEventObject(fEventObjects);
  lwjaktKT->SetJetType(LWJetProducer::kAKT);
  lwjaktKT->SetRadius(0.4);
  lwjaktKT->SetGhostArea(0.01);
  //lwjaktKT->SetGhostArea(0.005);
  lwjaktKT->SetPtMinConst(0.);
  lwjaktKT->SetParticlesName("pfParticles");
  lwjaktKT->SetJetContName("JetsAKTR040SDKT");
  lwjaktKT->SetDoConstituentSubtraction(kFALSE);
  lwjaktKT->SetDoSoftDrop(true);
  lwjaktKT->SetUseKtForSoftDrop(true);
  lwjaktKT->SetSoftDropZCut(0.1);
  lwjaktKT->SetSoftDropBeta(0.);
  lwjaktKT->SetDoJEC(false);//
  //lwjaktKT->SetL1Fastjet(strL1);
  //lwjaktKT->SetL2Relative(strL2Rel);
  //lwjaktKT->SetL3Absolute(strL3Abs);
  handler->Add(lwjaktKT);

  TString jetSDKTName = "JetsAKTR040SDKT";
  
  anaJetQA *jetQA = new anaJetQA("anaJetQA","anaJetQA");
  jetQA->ConnectEventObject(fEventObjects);
  jetQA->SetJetsName(jetName);
  handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDKTName);
  match->SetMatchingType(0);
  handler->Add(match);

  anaZgHistos *anazghistos = new anaZgHistos("anaZgHistos","anaZgHistos");
  anazghistos->ConnectEventObject(fEventObjects);
  anazghistos->SetHiEvtName("hiEventContainer");
  anazghistos->SetJetsName(jetSDKTName);
  anazghistos->SetJetsRefName(jetName);
  anazghistos->SetNCentBins(1);
  anazghistos->SetJetEtaRange(-1.3,1.3);
  anazghistos->SetDeltaRRange(0.1,999.);
  anazghistos->SetUseRhoMCWeight(false);
  anazghistos->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistos);

  anaZgHistos *anazghistosNoDrCut = new anaZgHistos("anaZgHistosNoDrCut","anaZgHistosNoDrCut");
  anazghistosNoDrCut->ConnectEventObject(fEventObjects);
  anazghistosNoDrCut->SetHiEvtName("hiEventContainer");
  anazghistosNoDrCut->SetJetsName(jetSDKTName);
  anazghistosNoDrCut->SetJetsRefName(jetName);
  anazghistosNoDrCut->SetNCentBins(1);
  anazghistosNoDrCut->SetJetEtaRange(-1.3,1.3);
  anazghistosNoDrCut->SetDeltaRRange(0.,999.);
  anazghistosNoDrCut->SetUseRhoMCWeight(false);
  anazghistosNoDrCut->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosNoDrCut);

  anaZgHistos *anazghistosdrSmall = new anaZgHistos("anaZgHistosDrSmall","anaZgHistosDrSmall");
  anazghistosdrSmall->ConnectEventObject(fEventObjects);
  anazghistosdrSmall->SetHiEvtName("hiEventContainer");
  anazghistosdrSmall->SetJetsName(jetSDKTName);
  anazghistosdrSmall->SetJetsRefName(jetName);
  anazghistosdrSmall->SetNCentBins(1);
  anazghistosdrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosdrSmall->SetDeltaRRange(0.1,0.2);
  anazghistosdrSmall->SetUseRhoMCWeight(false);
  anazghistosdrSmall->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrSmall);

  anaZgHistos *anazghistosdrLarge = new anaZgHistos("anaZgHistosDrLarge","anaZgHistosDrLarge");
  anazghistosdrLarge->ConnectEventObject(fEventObjects);
  anazghistosdrLarge->SetHiEvtName("hiEventContainer");
  anazghistosdrLarge->SetJetsName(jetSDKTName);
  anazghistosdrLarge->SetJetsRefName(jetName);
  anazghistosdrLarge->SetNCentBins(1);
  anazghistosdrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosdrLarge->SetDeltaRRange(0.2,999.);
  anazghistosdrLarge->SetUseRhoMCWeight(false);
  anazghistosdrLarge->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrLarge);


  anaZgHistos *anazghistosGen = new anaZgHistos("anazghistosGen","anazghistosGen");
  anazghistosGen->ConnectEventObject(fEventObjects);
  anazghistosGen->SetHiEvtName("hiEventContainer");
  anazghistosGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosGen->SetJetsRefName(""); //
  anazghistosGen->SetNCentBins(1);
  anazghistosGen->SetJetEtaRange(-1.3,1.3);
  anazghistosGen->SetDeltaRRange(0.1,999.);
  anazghistosGen->SetUseRhoMCWeight(false);
  handler->Add(anazghistosGen);

  anaZgHistos *anazghistosDrSmallGen = new anaZgHistos("anazghistosDrSmallGen","anazghistosDrSmallGen");
  anazghistosDrSmallGen->ConnectEventObject(fEventObjects);
  anazghistosDrSmallGen->SetHiEvtName("hiEventContainer");
  anazghistosDrSmallGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosDrSmallGen->SetJetsRefName(""); //
  anazghistosDrSmallGen->SetNCentBins(1);
  anazghistosDrSmallGen->SetJetEtaRange(-1.3,1.3);
  anazghistosDrSmallGen->SetDeltaRRange(0.1,0.2);
  anazghistosDrSmallGen->SetUseRhoMCWeight(false);
  handler->Add(anazghistosDrSmallGen);

  anaZgHistos *anazghistosDrLargeGen = new anaZgHistos("anazghistosDrLargeGen","anazghistosDrLargeGen");
  anazghistosDrLargeGen->ConnectEventObject(fEventObjects);
  anazghistosDrLargeGen->SetHiEvtName("hiEventContainer");
  anazghistosDrLargeGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosDrLargeGen->SetJetsRefName(""); //
  anazghistosDrLargeGen->SetNCentBins(1);
  anazghistosDrLargeGen->SetJetEtaRange(-1.3,1.3);
  anazghistosDrLargeGen->SetDeltaRRange(0.2,999.);
  anazghistosDrLargeGen->SetUseRhoMCWeight(false);
  handler->Add(anazghistosDrLargeGen);

  anaZgHistos *anazghistosNoFakes = new anaZgHistos("anaZgHistosNoFakes","anaZgHistosNoFakes");
  anazghistosNoFakes->ConnectEventObject(fEventObjects);
  anazghistosNoFakes->SetHiEvtName("hiEventContainer");
  anazghistosNoFakes->SetJetsName(jetSDName);
  anazghistosNoFakes->SetJetsRefName(jetName);
  anazghistosNoFakes->SetNCentBins(1);
  anazghistosNoFakes->SetJetEtaRange(-1.3,1.3);
  anazghistosNoFakes->SetMinRefPt(10.);
  handler->Add(anazghistosNoFakes);


  anaJetMatching *matchCAKT = new anaJetMatching("jetMatchingCAKT","jetMatchingCAKT");
  matchCAKT->ConnectEventObject(fEventObjects);
  matchCAKT->SetHiEvtName("hiEventContainer");
  matchCAKT->SetJetsNameBase(jetSDName);
  matchCAKT->SetJetsNameTag(jetSDKTName);
  matchCAKT->SetMatchingType(0);
  handler->Add(matchCAKT);

  anaZgHistos *anazghistosCAKT = new anaZgHistos("anaZgHistosCAKT","anaZgHistosCAKT");
  anazghistosCAKT->ConnectEventObject(fEventObjects);
  anazghistosCAKT->SetHiEvtName("hiEventContainer");
  anazghistosCAKT->SetJetsName(jetSDKTName);
  anazghistosCAKT->SetJetsRefName(jetSDName);
  anazghistosCAKT->SetNCentBins(1);
  anazghistosCAKT->SetJetEtaRange(-1.3,1.3);
  anazghistosCAKT->SetDeltaRRange(0.1,999.);
  anazghistosCAKT->SetUseRhoMCWeight(false);
  anazghistosCAKT->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosCAKT);
 
  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------
  Long64_t entries_tot =  chain->GetEntries(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    p_evt->Run(jentry);   //hi event properties
    p_pf->Run(jentry);    //pf candidates
    // Printf("produce SD jets");
    p_SDJet->Run(jentry); //forest SoftDrop jets
    //Printf("produce ungroomed jets");
    p_Jet->Run(jentry); //forest jets
	    
    //Execute all analysis tasks
    //Printf("exec tasks");
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

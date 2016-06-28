#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
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
#include "UserCode/diall/interface/anaSubJet.h"
#include "UserCode/diall/interface/anaZToMuMu.h"
#include "UserCode/diall/interface/anaZgHistos.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;

void analyzeZgHistos(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int activateJetShift = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */

  TString jetSDName = "aktCs4PFSoftDrop";
  TString jetTreeSDName = "akCsSoftDrop4PFJetAnalyzer";//"akCs4PFSoftDropJetAnalyzer";//
  TString jetName = "aktCs4PF";
  TString jetTreeName = "akCs4PFJetAnalyzer";

  bool doDRVar = true;

  bool doJetShift = false;
  if(activateJetShift>0) doJetShift = true;
  float jetShift = 0.04;
  if(activateJetShift==1) jetShift = 0.04;
  if(activateJetShift==2) jetShift = -0.04;

  double minptjet = 30.;
  bool doDijet = false;
  if(!doDijet) minptjet = 80.;

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

  TChain *skimTree = new TChain("skimanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) skimTree->Add(urls[i].c_str());
  chain->AddFriend(skimTree);
  Printf("skimTree done");

  TChain *hltTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hltTree->Add(urls[i].c_str());
  chain->AddFriend(hltTree);
  Printf("hltTree done");

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
  p_trg->AddTrigger("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  p_trg->AddTrigger("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  p_trg->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_SDJet = new lwJetFromForestProducer("lwJetForestProdSD");
  p_SDJet->SetInput(chain);
  p_SDJet->SetJetContName(jetSDName);
  p_SDJet->SetGenJetContName("");
  p_SDJet->SetEventObjects(fEventObjects);
  p_SDJet->SetMinJetPt(minptjet);
  p_SDJet->SetRadius(0.4);

  lwJetFromForestProducer *p_Jet = new lwJetFromForestProducer("lwJetForestProd");
  p_Jet->SetInput(jetTree);
  p_Jet->SetJetContName(jetName);
  p_Jet->SetGenJetContName("akt4Gen");
  p_Jet->SetEventObjects(fEventObjects);
  p_Jet->SetMinJetPt(minptjet);
  p_Jet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaJetQA *jetQA = new anaJetQA("anaJetQA","anaJetQA");
  jetQA->ConnectEventObject(fEventObjects);
  jetQA->SetJetsName(jetName);
  jetQA->SetTriggerMapName("triggerMap");
  jetQA->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  jetQA->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetTriggerMapName("triggerMap");
  match->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  match->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDName);
  match->SetMatchingType(0);
  handler->Add(match);

  anaZgHistos *anazghistos = new anaZgHistos("anaZgHistos","anaZgHistos");
  anazghistos->ConnectEventObject(fEventObjects);
  anazghistos->SetHiEvtName("hiEventContainer");
  anazghistos->DoCollisionEventSel(true);
  anazghistos->DoHBHENoiseFilter(true);
  anazghistos->DoHBHENoiseFilterLoose(true);
  anazghistos->DoPrimaryVertexFilter(true);
  anazghistos->DoClusterCompatibilityFilter(true);
  anazghistos->DoHFCoincFilter(true);
  anazghistos->SetTriggerMapName("triggerMap");
  anazghistos->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  anazghistos->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  anazghistos->SetRhoMax(330.);
  anazghistos->SetJetsName(jetSDName);
  anazghistos->SetJetsRefName(jetName);
  anazghistos->SetNCentBins(4);
  anazghistos->SetJetEtaRange(-1.3,1.3);
  anazghistos->SetDeltaRRange(0.1,999.);
  anazghistos->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistos);

  anaZgHistos *anazghistosdrSmall = new anaZgHistos("anaZgHistosDrSmall","anaZgHistosDrSmall");
  anazghistosdrSmall->ConnectEventObject(fEventObjects);
  anazghistosdrSmall->SetHiEvtName("hiEventContainer");
  anazghistosdrSmall->DoCollisionEventSel(true);
  anazghistosdrSmall->DoHBHENoiseFilter(true);
  anazghistosdrSmall->DoHBHENoiseFilterLoose(true);
  anazghistosdrSmall->DoPrimaryVertexFilter(true);
  anazghistosdrSmall->DoClusterCompatibilityFilter(true);
  anazghistosdrSmall->DoHFCoincFilter(true);
  anazghistosdrSmall->SetTriggerMapName("triggerMap");
  anazghistosdrSmall->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  anazghistosdrSmall->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  anazghistosdrSmall->SetRhoMax(330.);
  anazghistosdrSmall->SetJetsName(jetSDName);
  anazghistosdrSmall->SetJetsRefName(jetName);
  anazghistosdrSmall->SetNCentBins(4);
  anazghistosdrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosdrSmall->SetDeltaRRange(0.1,0.2);
  anazghistosdrSmall->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrSmall);

  anaZgHistos *anazghistosdrLarge = new anaZgHistos("anaZgHistosDrLarge","anaZgHistosDrLarge");
  anazghistosdrLarge->ConnectEventObject(fEventObjects);
  anazghistosdrLarge->SetHiEvtName("hiEventContainer");
  anazghistosdrLarge->DoCollisionEventSel(true);
  anazghistosdrLarge->DoHBHENoiseFilter(true);
  anazghistosdrLarge->DoHBHENoiseFilterLoose(true);
  anazghistosdrLarge->DoPrimaryVertexFilter(true);
  anazghistosdrLarge->DoClusterCompatibilityFilter(true);
  anazghistosdrLarge->DoHFCoincFilter(true);
  anazghistosdrLarge->SetTriggerMapName("triggerMap");
  anazghistosdrLarge->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  anazghistosdrLarge->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
  anazghistosdrLarge->SetRhoMax(330.);
  anazghistosdrLarge->SetJetsName(jetSDName);
  anazghistosdrLarge->SetJetsRefName(jetName);
  anazghistosdrLarge->SetNCentBins(4);
  anazghistosdrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosdrLarge->SetDeltaRRange(0.2,999.);
  anazghistosdrLarge->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosdrLarge);

  if(doDRVar) {
    anaZgHistos *anazghistosDRVarDown = new anaZgHistos("anaZgHistosDrVarDown","anaZgHistosDrVarDown");
    anazghistosDRVarDown->ConnectEventObject(fEventObjects);
    anazghistosDRVarDown->SetHiEvtName("hiEventContainer");
    anazghistosDRVarDown->DoCollisionEventSel(true);
    anazghistosDRVarDown->DoHBHENoiseFilter(true);
    anazghistosDRVarDown->DoHBHENoiseFilterLoose(true);
    anazghistosDRVarDown->DoPrimaryVertexFilter(true);
    anazghistosDRVarDown->DoClusterCompatibilityFilter(true);
    anazghistosDRVarDown->DoHFCoincFilter(true);
    anazghistosDRVarDown->SetTriggerMapName("triggerMap");
    anazghistosDRVarDown->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
    anazghistosDRVarDown->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
    anazghistosDRVarDown->SetRhoMax(330.);
    anazghistosDRVarDown->SetJetsName(jetSDName);
    anazghistosDRVarDown->SetJetsRefName(jetName);
    anazghistosDRVarDown->SetNCentBins(4);
    anazghistosDRVarDown->SetJetEtaRange(-1.3,1.3);
    anazghistosDRVarDown->SetDeltaRRange(0.1*0.85,999.);
    anazghistosDRVarDown->DoJetShift(doJetShift,jetShift);
    handler->Add(anazghistosDRVarDown);

    anaZgHistos *anazghistosDRVarUp = new anaZgHistos("anaZgHistosDrVarUp","anaZgHistosDrVarUp");
    anazghistosDRVarUp->ConnectEventObject(fEventObjects);
    anazghistosDRVarUp->SetHiEvtName("hiEventContainer");
    anazghistosDRVarUp->DoCollisionEventSel(true);
    anazghistosDRVarUp->DoHBHENoiseFilter(true);
    anazghistosDRVarUp->DoHBHENoiseFilterLoose(true);
    anazghistosDRVarUp->DoPrimaryVertexFilter(true);
    anazghistosDRVarUp->DoClusterCompatibilityFilter(true);
    anazghistosDRVarUp->DoHFCoincFilter(true);
    anazghistosDRVarUp->SetTriggerMapName("triggerMap");
    anazghistosDRVarUp->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
    anazghistosDRVarUp->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1");
    anazghistosDRVarUp->SetRhoMax(330.);
    anazghistosDRVarUp->SetJetsName(jetSDName);
    anazghistosDRVarUp->SetJetsRefName(jetName);
    anazghistosDRVarUp->SetNCentBins(4);
    anazghistosDRVarUp->SetJetEtaRange(-1.3,1.3);
    anazghistosDRVarUp->SetDeltaRRange(0.1*1.15,999.);
    anazghistosDRVarUp->DoJetShift(doJetShift,jetShift);
    handler->Add(anazghistosDRVarUp);
  }

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
    p_trg->Run(jentry);
    //Printf("produce jets");
    p_SDJet->Run(jentry); //forest SoftDrop jets
    p_Jet->Run(jentry); //forest jets

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

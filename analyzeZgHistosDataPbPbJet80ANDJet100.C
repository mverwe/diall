#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/triggerProducer.h"
#include "UserCode/diall/interface/triggerObjectProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaCreateJetTree.h"
//#include "UserCode/diall/interface/anaJetEnergyScale.h"
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

  TString triggerName = "HLT_HIPuAK4CaloJet100_Eta5p1_v1";
  //  TString triggerName = "HLT_HIPuAK4CaloJet80_Eta5p1_v1";
  // TString triggerName = "HLT_HIPuAK4CaloJet60_Eta5p1_v1";
  TString triggerName2 = "HLT_HIPuAK4CaloJet80_Eta5p1_v1";
  TString triggerName3 = "HLT_HIPuAK4CaloJet60_Eta5p1_v1";
  TString triggerName4 = "HLT_HIPuAK4CaloJet40_Eta5p1_v1";

  TString triggerSelect = triggerName2;
  TString triggerSelectSubset = triggerName;
  
  std::vector<std::string> trigList;
  trigList.push_back(triggerName.Data());
  trigList.push_back(triggerName2.Data());
  trigList.push_back(triggerName3.Data());

  TString triggerObjName = triggerName;
  TString triggerObjName2 = triggerName2;
  TString triggerObjName3 = triggerName3;
  TString triggerObjName4 = triggerName4;

  triggerObjName.ReplaceAll("v1","v");
  triggerObjName2.ReplaceAll("v1","v");
  triggerObjName3.ReplaceAll("v1","v");
  triggerObjName4.ReplaceAll("v1","v");

  std::vector<std::string> trigObjList;
  trigObjList.push_back(triggerObjName.Data());
  trigObjList.push_back(triggerObjName2.Data());
  trigObjList.push_back(triggerObjName3.Data());


  bool doDRVar = false;

  bool doJetShift = false;
  if(activateJetShift>0) doJetShift = true;
  float jetShift = 0.04;
  if(activateJetShift==1) jetShift = 0.04;
  if(activateJetShift==2) jetShift = -0.04;

  double minptjet = 10.;
  //bool doDijet = false;
  //if(!doDijet) minptjet = 80.;

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

  std::vector<TChain*> hltObjTree;
  for(std::vector<std::string>::const_iterator s = trigObjList.begin(); s != trigObjList.end(); ++s) {
    TChain *hltObjTreeTmp = new TChain(Form("hltobject/%s",s->c_str()));
    for(size_t i=firstFile; i<lastFile; i++) hltObjTreeTmp->Add(urls[i].c_str());
    hltObjTree.push_back(hltObjTreeTmp);
    Printf("hltObjTree %s done",s->c_str());
  }
  //Printf("hltObjTree %s done",triggerObjName.Data());

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
  p_trg->AddTrigger(triggerName.Data());
  p_trg->AddTrigger(triggerName2.Data());
  p_trg->AddTrigger(triggerName3.Data());
  // for(unsigned int ich = 0; ich<hltObjTree.size(); ++ich) {
  //   Printf("adding trigger object %d",ich);
  //   p_trg->AddTriggerObjectTree(hltObjTree[ich]);
  // }
  p_trg->SetEventObjects(fEventObjects);

  std::vector<triggerObjectProducer*> p_trgObj;
  for(unsigned int ich = 0; ich<hltObjTree.size(); ++ich) {
    triggerObjectProducer * trigObjProdTmp = new triggerObjectProducer(Form("trigObjProd_%s",hltObjTree[ich]->GetName()));
    trigObjProdTmp->SetInput(hltObjTree[ich]);
    trigObjProdTmp->SetTriggerMapName(trigObjList[ich]);
    trigObjProdTmp->SetEventObjects(fEventObjects);
    p_trgObj.push_back(trigObjProdTmp);
  }
    
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
  jetQA->AddTriggerSel(triggerSelect.Data());
  //jetQA->AddTriggerSel(triggerName2.Data());
  //jetQA->AddTriggerSel(triggerName3.Data());
    //handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetTriggerMapName("triggerMap");
  match->AddTriggerSel(triggerSelect.Data());
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDName);
  match->SetMatchingType(0);
  handler->Add(match);

  anaZgHistos *anazghistosOnlyJet60 = new anaZgHistos("anaZgHistosOnlyJet60","anaZgHistosOnlyJet60");
  anazghistosOnlyJet60->ConnectEventObject(fEventObjects);
  anazghistosOnlyJet60->SetHiEvtName("hiEventContainer");
  anazghistosOnlyJet60->SetMaxZVertex(9999.);
  anazghistosOnlyJet60->DoCollisionEventSel(true);
  anazghistosOnlyJet60->DoHBHENoiseFilter(true);
  anazghistosOnlyJet60->DoHBHENoiseFilterLoose(true);
  anazghistosOnlyJet60->DoPrimaryVertexFilter(true);
  anazghistosOnlyJet60->DoClusterCompatibilityFilter(true);
  anazghistosOnlyJet60->DoHFCoincFilter(true);
  anazghistosOnlyJet60->SetTriggerMapName("triggerMap");
  anazghistosOnlyJet60->AddTriggerSel(triggerName3.Data());
  //  anazghistosOnlyJet60->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistosOnlyJet60->SetTriggerObjectName("HLT_HIPuAK4CaloJet60_Eta5p1_v");
  anazghistosOnlyJet60->SetTriggerObjectEtaRange(-1.3,1.3);
  anazghistosOnlyJet60->SetVzBarycenter(-0.5160);
  anazghistosOnlyJet60->SetRhoMax(330.);
  anazghistosOnlyJet60->SetJetsName(jetSDName);
  anazghistosOnlyJet60->SetJetsRefName(jetName);
  anazghistosOnlyJet60->SetNCentBins(4);
  anazghistosOnlyJet60->SetJetEtaRange(-1.3,1.3);
  anazghistosOnlyJet60->SetDeltaRRange(0.1,999.);
  anazghistosOnlyJet60->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosOnlyJet60);
  
  anaZgHistos *anazghistosOnlyJet80 = new anaZgHistos("anaZgHistosOnlyJet80","anaZgHistosOnlyJet80");
  anazghistosOnlyJet80->ConnectEventObject(fEventObjects);
  anazghistosOnlyJet80->SetHiEvtName("hiEventContainer");
  anazghistosOnlyJet80->SetMaxZVertex(9999.);
  anazghistosOnlyJet80->DoCollisionEventSel(true);
  anazghistosOnlyJet80->DoHBHENoiseFilter(true);
  anazghistosOnlyJet80->DoHBHENoiseFilterLoose(true);
  anazghistosOnlyJet80->DoPrimaryVertexFilter(true);
  anazghistosOnlyJet80->DoClusterCompatibilityFilter(true);
  anazghistosOnlyJet80->DoHFCoincFilter(true);
  anazghistosOnlyJet80->SetTriggerMapName("triggerMap");
  anazghistosOnlyJet80->AddTriggerSel(triggerSelect.Data());
  //  anazghistosOnlyJet80->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistosOnlyJet80->SetTriggerObjectName("HLT_HIPuAK4CaloJet80_Eta5p1_v");
  anazghistosOnlyJet80->SetTriggerObjectEtaRange(-1.3,1.3);
  anazghistosOnlyJet80->SetVzBarycenter(-0.5160);
  anazghistosOnlyJet80->SetRhoMax(330.);
  anazghistosOnlyJet80->SetJetsName(jetSDName);
  anazghistosOnlyJet80->SetJetsRefName(jetName);
  anazghistosOnlyJet80->SetNCentBins(4);
  anazghistosOnlyJet80->SetJetEtaRange(-1.3,1.3);
  anazghistosOnlyJet80->SetDeltaRRange(0.1,999.);
  anazghistosOnlyJet80->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosOnlyJet80);

  anaZgHistos *anazghistosOnlyJet100 = new anaZgHistos("anaZgHistosOnlyJet100","anaZgHistosOnlyJet100");
  anazghistosOnlyJet100->ConnectEventObject(fEventObjects);
  anazghistosOnlyJet100->SetHiEvtName("hiEventContainer");
  anazghistosOnlyJet100->SetMaxZVertex(9999.);
  anazghistosOnlyJet100->DoCollisionEventSel(true);
  anazghistosOnlyJet100->DoHBHENoiseFilter(true);
  anazghistosOnlyJet100->DoHBHENoiseFilterLoose(true);
  anazghistosOnlyJet100->DoPrimaryVertexFilter(true);
  anazghistosOnlyJet100->DoClusterCompatibilityFilter(true);
  anazghistosOnlyJet100->DoHFCoincFilter(true);
  anazghistosOnlyJet100->SetTriggerMapName("triggerMap");
  anazghistosOnlyJet100->AddTriggerSel(triggerName.Data());
  //  anazghistosOnlyJet100->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistosOnlyJet100->SetTriggerObjectName("HLT_HIPuAK4CaloJet100_Eta5p1_v");
  anazghistosOnlyJet100->SetTriggerObjectEtaRange(-1.3,1.3);
  anazghistosOnlyJet100->SetVzBarycenter(-0.5160);
  anazghistosOnlyJet100->SetRhoMax(330.);
  anazghistosOnlyJet100->SetJetsName(jetSDName);
  anazghistosOnlyJet100->SetJetsRefName(jetName);
  anazghistosOnlyJet100->SetNCentBins(4);
  anazghistosOnlyJet100->SetJetEtaRange(-1.3,1.3);
  anazghistosOnlyJet100->SetDeltaRRange(0.1,999.);
  anazghistosOnlyJet100->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosOnlyJet100);


  anaZgHistos *anazghistos = new anaZgHistos("anaZgHistosJet80ANDJet100","anaZgHistosJet80ANDJet100");
  anazghistos->ConnectEventObject(fEventObjects);
  anazghistos->SetHiEvtName("hiEventContainer");
  anazghistos->DoCollisionEventSel(true);
  anazghistos->DoHBHENoiseFilter(true);
  anazghistos->DoHBHENoiseFilterLoose(true);
  anazghistos->DoPrimaryVertexFilter(true);
  anazghistos->DoClusterCompatibilityFilter(true);
  anazghistos->DoHFCoincFilter(true);
  anazghistos->SetTriggerMapName("triggerMap");
  anazghistos->AddTriggerSel(triggerSelect.Data());
  anazghistos->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistos->SetTriggerObjectName("HLT_HIPuAK4CaloJet80_Eta5p1_v");
  anazghistos->SetTriggerObjectEtaRange(-1.3,1.3);
  anazghistos->SetVzBarycenter(-0.5160);
  anazghistos->SetRhoMax(330.);
  anazghistos->SetJetsName(jetSDName);
  anazghistos->SetJetsRefName(jetName);
  anazghistos->SetNCentBins(4);
  anazghistos->SetJetEtaRange(-1.3,1.3);
  anazghistos->SetDeltaRRange(0.1,999.);
  anazghistos->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistos);


  anaZgHistos *anazghistosJet60ANDJet100 = new anaZgHistos("anaZgHistosJet60ANDJet100","anaZgHistosJet60ANDJet100");
  anazghistosJet60ANDJet100->ConnectEventObject(fEventObjects);
  anazghistosJet60ANDJet100->SetHiEvtName("hiEventContainer");
  anazghistosJet60ANDJet100->DoCollisionEventSel(true);
  anazghistosJet60ANDJet100->DoHBHENoiseFilter(true);
  anazghistosJet60ANDJet100->DoHBHENoiseFilterLoose(true);
  anazghistosJet60ANDJet100->DoPrimaryVertexFilter(true);
  anazghistosJet60ANDJet100->DoClusterCompatibilityFilter(true);
  anazghistosJet60ANDJet100->DoHFCoincFilter(true);
  anazghistosJet60ANDJet100->SetTriggerMapName("triggerMap");
  anazghistosJet60ANDJet100->AddTriggerSel(triggerName3.Data());
  anazghistosJet60ANDJet100->AddTriggerSelSubset(triggerName.Data());
  anazghistosJet60ANDJet100->SetTriggerObjectName("HLT_HIPuAK4CaloJet60_Eta5p1_v");
  anazghistosJet60ANDJet100->SetTriggerObjectEtaRange(-1.3,1.3);
  anazghistosJet60ANDJet100->SetVzBarycenter(-0.5160);
  anazghistosJet60ANDJet100->SetRhoMax(330.);
  anazghistosJet60ANDJet100->SetJetsName(jetSDName);
  anazghistosJet60ANDJet100->SetJetsRefName(jetName);
  anazghistosJet60ANDJet100->SetNCentBins(4);
  anazghistosJet60ANDJet100->SetJetEtaRange(-1.3,1.3);
  anazghistosJet60ANDJet100->SetDeltaRRange(0.1,999.);
  anazghistosJet60ANDJet100->DoJetShift(doJetShift,jetShift);
  handler->Add(anazghistosJet60ANDJet100);

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
  anazghistosdrSmall->AddTriggerSel(triggerSelect.Data());
  anazghistosdrSmall->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistosdrSmall->SetRhoMax(330.);
  anazghistosdrSmall->SetJetsName(jetSDName);
  anazghistosdrSmall->SetJetsRefName(jetName);
  anazghistosdrSmall->SetNCentBins(4);
  anazghistosdrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosdrSmall->SetDeltaRRange(0.1,0.2);
  anazghistosdrSmall->DoJetShift(doJetShift,jetShift);
  // handler->Add(anazghistosdrSmall);

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
  anazghistosdrLarge->AddTriggerSel(triggerSelect.Data());
  anazghistosdrLarge->AddTriggerSelSubset(triggerSelectSubset.Data());
  anazghistosdrLarge->SetRhoMax(330.);
  anazghistosdrLarge->SetJetsName(jetSDName);
  anazghistosdrLarge->SetJetsRefName(jetName);
  anazghistosdrLarge->SetNCentBins(4);
  anazghistosdrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosdrLarge->SetDeltaRRange(0.2,999.);
  anazghistosdrLarge->DoJetShift(doJetShift,jetShift);
  //  handler->Add(anazghistosdrLarge);

  anaCreateJetTree *anaJetTree = new anaCreateJetTree(Form("anaJetTree_%s_%s",jetName.Data(),triggerName2.Data()),Form("anaJetTree_%s_%s",jetName.Data(),triggerName2.Data()));
  anaJetTree->ConnectEventObject(fEventObjects);
  anaJetTree->SetHiEvtName("hiEventContainer");
  anaJetTree->DoCollisionEventSel(true);
  anaJetTree->DoHBHENoiseFilter(true);
  anaJetTree->DoHBHENoiseFilterLoose(true);
  anaJetTree->DoPrimaryVertexFilter(true);
  anaJetTree->DoClusterCompatibilityFilter(true);
  anaJetTree->DoHFCoincFilter(true);
  anaJetTree->SetTriggerMapName("triggerMap");
  anaJetTree->AddTriggerSel(triggerName2.Data());
  //anaJetTree->SetGenJetsName(Form("akt%dGenFor%s",rad[ij],jetName[ij].Data()));
  anaJetTree->SetRecJetsName(jetSDName);
  anaJetTree->SetGenJetsName(jetName);
  anaJetTree->SetNCentBins(1);
  anaJetTree->SetUseForestMatching(false);
  anaJetTree->SetMaxDistance(0.2);
  anaJetTree->SetStoreSubjets(true);
  //anaJetTree->SetMinJetPtRec(80.);
  anaJetTree->SetMinJetPtRef(140.);
  anaJetTree->SetStorePFIdInfo(true);
  //handler->Add(anaJetTree);
  
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
    anazghistosDRVarDown->AddTriggerSel(triggerSelect.Data());
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
    anazghistosDRVarDown->AddTriggerSel(triggerSelect.Data());
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
  int entryDiv = ((int)((lastEvent-firstEvent)/20));
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
    //if(jentry%10000==0) cout << "entry: "<< jentry << endl;
    if(jentry%entryDiv==0) cout << "entry: "<< jentry << endl;
    //Run producers
    //Printf("produce hiEvent");
    p_evt->Run(jentry);   //hi event properties
    p_trg->Run(jentry);

    for(unsigned int ich = 0; ich<p_trgObj.size(); ++ich) {
      p_trgObj[ich]->Run(jentry);
    }
      
    //stop here if trigger of interest didn't fire
    triggerMap *trgMap = p_trg->GetTriggerMap();
    bool passTrig = false;
    for(std::vector<std::string>::const_iterator s = trigList.begin(); s != trigList.end(); ++s) {
       int fire = trgMap->TriggerFired(*s);
       if(fire>0) passTrig = true;
    }
    if(!passTrig) continue;
    
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

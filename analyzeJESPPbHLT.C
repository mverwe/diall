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

//TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/pPb8TeV/trigger/caloJet40Drops"; //13 TeV offline JEC
//TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/pPb8TeV/offlineJEC/onlineJEC"; //HLT JEC
TString baseJEC = "/afs/cern.ch/user/m/mverweij/work/pPb8TeV/offlineJEC/signalOnly/jec";

void analyzeJES(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int doCaloJEC = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */

  //TString strL1 = Form("%s/80X_mcRun2_asymptotic_2016_miniAODv2_v1_L1FastJet_AK4Calo.txt",baseJEC.Data());
  TString strL1 = Form("%s/HI_PythiaCUETP8M1_5020GeV_753p1_v1_L1FastJet_AK4Calo_offline.txt",baseJEC.Data());
  //TString strL2Rel = Form("%s/80X_mcRun2_asymptotic_2016_miniAODv2_v1_L2Relative_AK4Calo.txt",baseJEC.Data());
  TString strL3Abs = Form("%s/80X_mcRun2_asymptotic_2016_miniAODv2_v1_L3Absolute_AK4Calo.txt",baseJEC.Data());
  TString strL2L3Res = Form("%s/80X_mcRun2_asymptotic_2016_miniAODv2_v1_L2L3Residual_AK4Calo.txt",baseJEC.Data());
  //TString strL2Rel = Form("%s/80X_mcRun2_pA_v0_L2Relative_AK4CaloHLT.txt",baseJEC.Data());
  TString strL2Rel = Form("%s/MV_Unf_L2Relative_ak4Calo_v2.txt",baseJEC.Data());
  //TString strL3Abs = Form("%s/80X_mcRun2_pA_v0_L3Absolute_AK4CaloHLT.txt",baseJEC.Data());
  //TString strL2L3Res = Form("%s/80X_mcRun2_pA_v0_L2L3Residual_AK4CaloHLT.txt",baseJEC.Data());

  const int njetTypes = 2;
  int rad[njetTypes] = {4,4};
  TString jetName[njetTypes] = {"aktR040CaloHlt","aktR040PFHlt"};
  //TString jetTreeName[njetTypes] = {"ak3PFJetAnalyzer","ak4PFJetAnalyzer","ak5PFJetAnalyzer",};
  TString jetTreeName[njetTypes] = {"ak4CaloHltJetAnalyzer","ak4PFHltJetAnalyzer"};
  
  //TString jetName[njetTypes] = {"aktR020","aktR030","aktR040","aktR050"};
  //TString jetTreeName[njetTypes] = {"ak2PFJetAnalyzer","ak3PFJetAnalyzer","ak4PFJetAnalyzer","ak5PFJetAnalyzer",};

  for(int i = 0; i<njetTypes; ++i)
    std::cout << "analyzing JES for: " << jetName[i] << " tree: " << jetTreeName[i] << std::endl;
   
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
  
  TChain *jetTree[njetTypes];
  for(int ij = 0; ij<njetTypes; ++ij) {
    jetTree[ij] = new TChain(Form("%s/t",jetTreeName[ij].Data()));
    for(size_t i=firstFile; i<lastFile; i++) jetTree[ij]->Add(urls[i].c_str());
    //chain->AddFriend(jetTree[ij]);
    Printf("jetTree %s done",jetTreeName[ij].Data());
  }

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_PUJet[njetTypes];
  for(int ij = 0; ij<njetTypes; ++ij) {
    p_PUJet[ij] = new lwJetFromForestProducer(Form("lwJetForestProd_%d",ij));
    p_PUJet[ij]->SetInput(jetTree[ij]);
    p_PUJet[ij]->SetJetContName(jetName[ij]);
    p_PUJet[ij]->SetGenJetContName(Form("akt%dGenFor%s",rad[ij],jetName[ij].Data()));
    p_PUJet[ij]->SetEventObjects(fEventObjects);
    p_PUJet[ij]->SetRadius((double)rad[ij]/10.);
    if(doCaloJEC && jetName[ij].Contains("Calo")) {
      p_PUJet[ij]->SetDoJEC(true);
      p_PUJet[ij]->SetL1Fastjet(strL1);
      p_PUJet[ij]->SetL2Relative(strL2Rel);
      p_PUJet[ij]->SetL3Absolute(strL3Abs);
    }
  }
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaJetEnergyScale *anajesForest[njetTypes];
  anaJetEnergyScale *anajesForestRaw[njetTypes];

  anaCreateJetTree *anaJetTree[njetTypes];
  
  for(int ij = 0; ij<njetTypes; ++ij) {
    anajesForest[ij] = new anaJetEnergyScale(Form("anaJESForest_%s",jetName[ij].Data()),Form("anaJESForest_%s",jetName[ij].Data()));
    anajesForest[ij]->ConnectEventObject(fEventObjects);
    anajesForest[ij]->SetHiEvtName("hiEventContainer");
    anajesForest[ij]->SetGenJetsName(Form("akt%dGenFor%s",rad[ij],jetName[ij].Data()));
    anajesForest[ij]->SetRecJetsName(jetName[ij]);
    anajesForest[ij]->SetNCentBins(1);
    anajesForest[ij]->SetUseForestMatching(true);
    //anajesForest[ij]->SetMaxDistance(0.2);
    handler->Add(anajesForest[ij]);

    anajesForestRaw[ij] = new anaJetEnergyScale(Form("anaJESForestRaw_%s",jetName[ij].Data()),Form("anaJESForestRaw_%s",jetName[ij].Data()));
    anajesForestRaw[ij]->ConnectEventObject(fEventObjects);
    anajesForestRaw[ij]->SetHiEvtName("hiEventContainer");
    anajesForestRaw[ij]->SetGenJetsName(Form("akt%dGenFor%s",rad[ij],jetName[ij].Data()));
    anajesForestRaw[ij]->SetRecJetsName(jetName[ij]);
    anajesForestRaw[ij]->SetNCentBins(1);
    anajesForestRaw[ij]->SetUseForestMatching(true);
    anajesForestRaw[ij]->SetUseRawPt(true);
    //anajesForestRaw[ij]->SetMaxDistance(0.2);
    handler->Add(anajesForestRaw[ij]);
    
    if(jetName[ij].Contains("Calo")) {
      anaJetTree[ij] = new anaCreateJetTree(Form("anaJetTree_%s",jetName[ij].Data()),Form("anaJetTree_%s",jetName[ij].Data()));
      anaJetTree[ij]->ConnectEventObject(fEventObjects);
      anaJetTree[ij]->SetHiEvtName("hiEventContainer");
      anaJetTree[ij]->SetGenJetsName(Form("akt%dGenFor%s",rad[ij],jetName[ij].Data()));
      anaJetTree[ij]->SetRecJetsName(jetName[ij]);
      anaJetTree[ij]->SetNCentBins(1);
      anaJetTree[ij]->SetUseForestMatching(true);
      //anaJetTree[ij]->SetMaxDistance(0.2);
      handler->Add(anaJetTree[ij]);
    }
  }

  // anaJetEnergyScale *anajesForestQuarks = new anaJetEnergyScale("anaJESForestQuarks","anaJESForestQuarks");
  // anajesForestQuarks->ConnectEventObject(fEventObjects);
  // anajesForestQuarks->SetHiEvtName("hiEventContainer");
  // anajesForestQuarks->SetGenJetsName("akt4Gen");
  // anajesForestQuarks->SetRecJetsName(jetName);
  // anajesForestQuarks->SetNCentBins(1);
  // anajesForestQuarks->SetUseForestMatching(true);
  // //anajesForestQuarks->SetMaxDistance(0.2);
  // anajesForestQuarks->SetRefPartonFlavor(0,2);
  // handler->Add(anajesForestQuarks);

  // anaJetEnergyScale *anajesForestGluons = new anaJetEnergyScale("anaJESForestGluons","anaJESForestGluons");
  // anajesForestGluons->ConnectEventObject(fEventObjects);
  // anajesForestGluons->SetHiEvtName("hiEventContainer");
  // anajesForestGluons->SetGenJetsName("akt4Gen");
  // anajesForestGluons->SetRecJetsName(jetName);
  // anajesForestGluons->SetNCentBins(1);
  // anajesForestGluons->SetUseForestMatching(true);
  // //anajesForestGluons->SetMaxDistance(0.2);
  // anajesForestGluons->SetRefPartonFlavor(21,21);
  // handler->Add(anajesForestGluons);
  
  // anaJetEnergyScale *anajesForestRaw = new anaJetEnergyScale("anaJESForestRaw","anaJESForestRaw");
  // anajesForestRaw->ConnectEventObject(fEventObjects);
  // anajesForestRaw->SetHiEvtName("hiEventContainer");
  // anajesForestRaw->SetGenJetsName("");
  // anajesForestRaw->SetRecJetsName(jetName);
  // anajesForestRaw->SetNCentBins(1);
  // anajesForestRaw->SetUseForestMatching(true);
  // anajesForestRaw->SetUseRawPt(true);
  //handler->Add(anajesForestRaw);

 
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
    p_evt->Run(jentry);   //hi event properties
    //Printf("produce PU jets");
    for(int ij = 0; ij<njetTypes; ++ij)
      p_PUJet[ij]->Run(jentry); //forest jets
	    
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

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

#include <iostream>

using namespace std;

void analyzeJES(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */


  TString jetName = "aktCSR040";
  std::cout << "analyzing JES for: " << jetName << std::endl; 
 
  std::cout << "nfiles: " << urls.size() << std::endl;
  //for (auto i = urls.begin(); i != urls.end(); ++i)
  //  std::cout << *i << std::endl;

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
  
  TChain *jetTree = new TChain("akCs4PFJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  chain->AddFriend(jetTree);
  Printf("jetTree done");

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_PUJet = new lwJetFromForestProducer("lwJetForestProd");
  p_PUJet->SetInput(chain);
  p_PUJet->SetJetContName(jetName);
  p_PUJet->SetGenJetContName("akt4Gen");
  p_PUJet->SetEventObjects(fEventObjects);
  p_PUJet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaJetEnergyScale *anajesForest = new anaJetEnergyScale("anaJESForest","anaJESForest");
  anajesForest->ConnectEventObject(fEventObjects);
  anajesForest->SetHiEvtName("hiEventContainer");
  anajesForest->SetGenJetsName("");
  anajesForest->SetRecJetsName(jetName);
  anajesForest->SetNCentBins(4);
  anajesForest->SetUseForestMatching(true);
  handler->Add(anajesForest);

  anaJetEnergyScale *anajesForestRaw = new anaJetEnergyScale("anaJESForestRaw","anaJESForestRaw");
  anajesForestRaw->ConnectEventObject(fEventObjects);
  anajesForestRaw->SetHiEvtName("hiEventContainer");
  anajesForestRaw->SetGenJetsName("");
  anajesForestRaw->SetRecJetsName(jetName);
  anajesForestRaw->SetNCentBins(4);
  anajesForestRaw->SetUseForestMatching(true);
  anajesForestRaw->SetUseRawPt(true);
  handler->Add(anajesForestRaw);

  //particle-detector-level jet matching
  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  //match->SetJetsNameBase(jetName);
  //match->SetJetsNameTag("akt4Gen");
  match->SetJetsNameTag(jetName);
  match->SetJetsNameBase("akt4Gen");
  match->SetMatchingType(0);
  handler->Add(match);
  
  anaJetEnergyScale *anajes = new anaJetEnergyScale("anaJES","anaJES");
  anajes->ConnectEventObject(fEventObjects);
  anajes->SetHiEvtName("hiEventContainer");
  anajes->SetGenJetsName("akt4Gen");
  anajes->SetRecJetsName(jetName);
  anajes->SetNCentBins(4);
  handler->Add(anajes);

  anaJetEnergyScale *anajesRaw = new anaJetEnergyScale("anaJESRaw","anaJESRaw");
  anajesRaw->ConnectEventObject(fEventObjects);
  anajesRaw->SetHiEvtName("hiEventContainer");
  anajesRaw->SetGenJetsName("akt4Gen");
  anajesRaw->SetRecJetsName(jetName);
  anajesRaw->SetNCentBins(4);
  anajesRaw->SetUseForestMatching(false);
  anajesRaw->SetUseRawPt(true);
  handler->Add(anajesRaw);

 
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

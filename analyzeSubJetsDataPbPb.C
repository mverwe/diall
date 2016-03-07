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

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>

using namespace std;

void analyzeSubJets(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

  /*
    ptminType: minimum raw pt for particles used by puppi
    0 : 0 GeV
    1 : 1 GeV
    2 : 2 GeV

    jetSignalType: jets used to select jetty region in events
    0 : detector-level jets (akPu4PF)
    1 : particle-level jets (gen jets)
   */

  double ptminjet = 80.;
  TString jetName = "aktCs4PFSoftDrop";
  TString jetTreeName = "akCs4PFSoftDropJetAnalyzer";
  //jetName = "akCs4PFFilter";
  //jetTreeName = "akCs4PFFilterJetAnalyzer";

  std::cout << "analyzing subjets for: " << jetName << " tree: " << jetTreeName << std::endl;
   
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
  Printf("hltTree done");
  
  TChain *jetTree = new TChain(Form("%s/t",jetTreeName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  chain->AddFriend(jetTree);
  Printf("jetTree done");

  // TChain *csJetTree = new TChain(Form("%s/t","akCs4PFJetAnalyzer"));
  // for(size_t i=firstFile; i<lastFile; i++) csJetTree->Add(urls[i].c_str());
  // chain->AddFriend(csJetTree);
  // Printf("csJetTree done");

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  triggerProducer *p_trg = new triggerProducer("trigProd");
  p_trg->SetInput(hltTree);
  p_trg->SetTriggerMapName("triggerMap");
  p_trg->AddTrigger("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  p_trg->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_PUJet = new lwJetFromForestProducer("lwJetForestProd");
  p_PUJet->SetInput(chain);
  p_PUJet->SetJetContName(jetName);
  p_PUJet->SetGenJetContName("");
  p_PUJet->SetEventObjects(fEventObjects);
  p_PUJet->SetRadius(0.4);
  p_PUJet->SetMinJetPt(ptminjet);
  // lwJetFromForestProducer *p_CSJet = new lwJetFromForestProducer("lwJetForestProd");
  // p_CSJet->SetInput(csJetTree);
  // p_CSJet->SetJetContName("aktCs4PF");
  // p_CSJet->SetGenJetContName("");
  // p_CSJet->SetEventObjects(fEventObjects);
  // p_CSJet->SetRadius(0.4);
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaSubJet *anasubjets = new anaSubJet("anaSubJets","anaSubJets");
  anasubjets->ConnectEventObject(fEventObjects);
  anasubjets->SetHiEvtName("hiEventContainer");
  anasubjets->SetTriggerMapName("triggerMap");
  anasubjets->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  anasubjets->SetJetsName(jetName);
  anasubjets->SetNCentBins(4);
  anasubjets->SetJetEtaRange(-2.,2.);
  anasubjets->SetDoDijets(true);
  anasubjets->AddLeadingJetPtBin(120.,150.);
  anasubjets->AddLeadingJetPtBin(150.,180.);
  anasubjets->AddLeadingJetPtBin(180.,220.);
  anasubjets->AddLeadingJetPtBin(220.,260.);
  anasubjets->AddLeadingJetPtBin(260.,300.);
  anasubjets->AddLeadingJetPtBin(300.,500.);
  anasubjets->SetPtMinSubleading(30.);
  handler->Add(anasubjets);

  anaSubJet *anasubjetsMassCut = new anaSubJet("anasubjetsMassCut","anasubjetsMassCut");
  anasubjetsMassCut->ConnectEventObject(fEventObjects);
  anasubjetsMassCut->SetHiEvtName("hiEventContainer");
  anasubjetsMassCut->SetTriggerMapName("triggerMap");
  anasubjetsMassCut->AddTriggerSel("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
  anasubjetsMassCut->SetJetsName(jetName);
  anasubjetsMassCut->SetNCentBins(4);
  anasubjetsMassCut->SetJetEtaRange(-2.,2.);
  anasubjetsMassCut->SetDoDijets(true);
  anasubjetsMassCut->AddLeadingJetPtBin(120.,150.);
  anasubjetsMassCut->AddLeadingJetPtBin(150.,180.);
  anasubjetsMassCut->AddLeadingJetPtBin(180.,220.);
  anasubjetsMassCut->AddLeadingJetPtBin(220.,260.);
  anasubjetsMassCut->AddLeadingJetPtBin(260.,300.);
  anasubjetsMassCut->AddLeadingJetPtBin(300.,500.);
  anasubjetsMassCut->SetPtMinSubleading(30.);
  anasubjetsMassCut->SetMinMassLeading(10.);
  handler->Add(anasubjetsMassCut);

  // anaSubJet *anasubjetsCSJets = new anaSubJet("anasubjetsCSJets","anasubjetsCSJets");
  // anasubjetsCSJets->ConnectEventObject(fEventObjects);
  // anasubjetsCSJets->SetHiEvtName("hiEventContainer");
  // anasubjetsCSJets->SetJetsName("aktCs4PF");
  // anasubjetsCSJets->SetNCentBins(4);
  // anasubjetsCSJets->SetJetEtaRange(-2.,2.);
  // anasubjetsCSJets->SetDoDijets(false);
  // handler->Add(anasubjetsCSJets);
 
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
    p_trg->Run(jentry);	    

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

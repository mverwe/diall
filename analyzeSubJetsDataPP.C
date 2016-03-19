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

  TString jetSDName = "akt4PFSD";
  TString jetTreeSDName = "akSoftDrop4PFJetAnalyzer";
  TString jetName = "akt4PF";
  TString jetTreeName = "ak4PFJetAnalyzer";
  
  TString triggerName = "HLT_AK4PFJet80_Eta5p1_v1";

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

  TChain *hltTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hltTree->Add(urls[i].c_str());
  Printf("hltTree done");

  TChain *jetTreeSD = new TChain(Form("%s/t",jetTreeSDName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTreeSD->Add(urls[i].c_str());
  chain->AddFriend(jetTreeSD);
  Printf("jetTreeSD done");

  TChain *jetTree = new TChain(Form("%s/t",jetTreeName.Data()));
  for(size_t i=firstFile; i<lastFile; i++) jetTree->Add(urls[i].c_str());
  Printf("jetTree done");
  
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
  p_trg->AddTrigger(triggerName.Data());
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
  jetQA->SetTriggerMapName("triggerMap");
  jetQA->AddTriggerSel(triggerName.Data());
  jetQA->SetJetsName(jetName);
  handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetTriggerMapName("triggerMap");
  match->AddTriggerSel(triggerName.Data());
  match->SetNCentBins(1);
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDName);
  match->SetMatchingType(0);
  handler->Add(match);
  
  anaSubJet *anasubjets = new anaSubJet("anaSubJets","anaSubJets");
  anasubjets->ConnectEventObject(fEventObjects);
  anasubjets->SetHiEvtName("hiEventContainer");
  anasubjets->SetTriggerMapName("triggerMap");
  anasubjets->AddTriggerSel(triggerName.Data());
  anasubjets->SetJetsName(jetSDName);
  anasubjets->SetJetsRefName(jetName);
  anasubjets->SetNCentBins(1);
  anasubjets->SetJetEtaRange(-2.,2.);
  anasubjets->SetDoDijets(doDijet);
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
  anasubjetsMassCut->AddTriggerSel(triggerName.Data());
  anasubjetsMassCut->SetJetsName(jetSDName);
  anasubjetsMassCut->SetJetsRefName(jetName);
  anasubjetsMassCut->SetNCentBins(1);
  anasubjetsMassCut->SetJetEtaRange(-2.,2.);
  anasubjetsMassCut->SetDoDijets(doDijet);
  anasubjetsMassCut->AddLeadingJetPtBin(120.,150.);
  anasubjetsMassCut->AddLeadingJetPtBin(150.,180.);
  anasubjetsMassCut->AddLeadingJetPtBin(180.,220.);
  anasubjetsMassCut->AddLeadingJetPtBin(220.,260.);
  anasubjetsMassCut->AddLeadingJetPtBin(260.,300.);
  anasubjetsMassCut->AddLeadingJetPtBin(300.,500.);
  anasubjetsMassCut->SetPtMinSubleading(30.);
  anasubjetsMassCut->SetMinMassLeading(10.);
  handler->Add(anasubjetsMassCut);
 
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

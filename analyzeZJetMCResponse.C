#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

void analyzeZJetMCResponse(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0) {

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
  
  //add files to chain
  TChain *chain = NULL;
  chain = new TChain("hiEvtAnalyzer/HiTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("hiTree done");

  TChain *hltTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hltTree->Add(urls[i].c_str());
  chain->AddFriend(hltTree);
  Printf("hltTree done");

  TChain *skimTree = new TChain("skimanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) skimTree->Add(urls[i].c_str());
  chain->AddFriend(skimTree);
  Printf("skimTree done");

  TChain *muTree = new TChain("ggHiNtuplizer/EventTree");
  for(size_t i=firstFile; i<lastFile; i++) muTree->Add(urls[i].c_str());
  chain->AddFriend(muTree);
  Printf("muTree done");

  TChain *caloJetTree = new TChain("akPu4CaloJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) caloJetTree->Add(urls[i].c_str());
  //chain->AddFriend(caloJetTree);
  Printf("caloJetTree done");

  TChain *genTree = new TChain("HiGenParticleAna/hi");
  for(size_t i=firstFile; i<lastFile; i++) genTree->Add(urls[i].c_str());
  //chain->AddFriend(genTree);
  Printf("genTree done: %d",(int)genTree->GetEntries());

  TList *fEventObjects = new TList();

  //---------------------------------------------------------------
  // producers
  //
  hiEventProducer *p_evt = new hiEventProducer("hiEvtProd");
  p_evt->SetInput(chain);
  p_evt->SetHIEventContName("hiEventContainer");
  p_evt->SetEventObjects(fEventObjects);

  lwMuonProducer *p_mu = new lwMuonProducer("lwMuonProd");
  p_mu->SetInput(chain);
  p_mu->SetlwMuonsRecoName("lwMuonsReco");
  p_mu->SetlwMuonsGeneName("");
  p_mu->SetEventObjects(fEventObjects);

  genParticleProducer *p_gen = new genParticleProducer("genParticleProd");
  p_gen->SetInput(genTree);
  p_gen->SetGenParticlesName("genParticles");
  p_gen->SetEventObjects(fEventObjects);

  lwJetFromForestProducer *p_caloJet = new lwJetFromForestProducer("lwJetForestProdCalo");
  p_caloJet->SetInput(caloJetTree);
  p_caloJet->SetJetContName("akt4Calo");
  p_caloJet->SetGenJetContName("");
  p_caloJet->SetEventObjects(fEventObjects);
  p_caloJet->SetRadius(0.4);

  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

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
  ZResp->SetJetsName("akt4Calo");
  handler->Add(ZResp);


 //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------	  
  Long64_t entries_tot =  chain->GetEntriesFast(); //93064
  if(nentries<0) lastEvent = chain->GetEntries();
  // Long64_t nentries = 20;//chain->GetEntriesFast();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) {
     if (jentry%10000==0) Printf("Processing event %d  %d",(int)(jentry), (int)(lastEvent));
    //Run producers
    // Printf("produce hiEvent");
    p_evt->Run(jentry);   //hi event properties

    //Printf("produce pf particles");
    p_gen->Run(jentry);     //gen particles
    p_caloJet->Run(jentry); //calo jets
    
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
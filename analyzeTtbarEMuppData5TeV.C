#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/lwElectronProducer.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/trackProducer.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/triggerProducer.h"
#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaTtbarEMu.h"
#include "UserCode/diall/interface/anaJetMatching.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPFCandidates.h"
#include "UserCode/diall/interface/anaPFvsCaloJet.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaPuppiParticles.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#include <TList.h>
#include <TChain.h>
#include <TFile.h>

//#include "CollectFiles.C"

#include <iostream>

using namespace std;

void analyzeTtbarEMuppData5TeV(std::vector<std::string> urls, const char *outname = "AnaResults.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int isData = 1) {


  
  //Printf("anaFile: %d",anaFile);
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
  std::cout << "isData: " << isData << std::endl;  

  //add files to chain
  TChain *chain = NULL;
  chain = new TChain("hiEvtAnalyzer/HiTree");
  for(size_t i=firstFile; i<lastFile; i++) chain->Add(urls[i].c_str());
  Printf("hltTree done");


  TChain *hiEvtTree = new TChain("hltanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) hiEvtTree->Add(urls[i].c_str());
  chain->AddFriend(hiEvtTree);
  Printf("hiTree done");


  TChain *skimTree = new TChain("skimanalysis/HltTree");
  for(size_t i=firstFile; i<lastFile; i++) skimTree->Add(urls[i].c_str());
  chain->AddFriend(skimTree);
  Printf("skimTree done");

  //TChain *pileupTree = new TChain("pileup/tree");
  // for(size_t i=firstFile; i<lastFile; i++) pileupTree->Add(urls[i].c_str());
  //chain->AddFriend(pileupTree);
  
  TChain *pfTree = new TChain("pfcandAnalyzer/pfTree");
  for(size_t i=firstFile; i<lastFile; i++) pfTree->Add(urls[i].c_str());
  chain->AddFriend(pfTree);
  Printf("pfTree done");
  
  TChain *muTree = new TChain("ggHiNtuplizer/EventTree");
  for(size_t i=firstFile; i<lastFile; i++) muTree->Add(urls[i].c_str());
  chain->AddFriend(muTree);
  Printf("muTree done");

  TChain *pfJetTree = new TChain("ak4PFJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) pfJetTree->Add(urls[i].c_str());
  chain->AddFriend(pfJetTree);
  Printf("pfJetTree done");

  
  TChain *caloJetTree = new TChain("ak4CaloJetAnalyzer/t");
  for(size_t i=firstFile; i<lastFile; i++) caloJetTree->Add(urls[i].c_str());
  //chain->AddFriend(caloJetTree);
  Printf("caloJetTree done");

  TChain *trackTree = new TChain("ppTrack/trackTree");
  for(size_t i=firstFile; i<lastFile; i++) trackTree->Add(urls[i].c_str());
  //chain->AddFriend(trackTree);
  Printf("trackTree done");

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
  p_trg->SetInput(hiEvtTree);
  p_trg->SetTriggerMapName("triggerMap");
  p_trg->SetEventObjects(fEventObjects);
  
  pfParticleProducer *p_pf = new pfParticleProducer("pfPartProd");
  p_pf->SetInput(chain);
  p_pf->SetpfParticlesName("pfParticles");
  p_pf->SetEventObjects(fEventObjects);

  lwMuonProducer *p_mu = new lwMuonProducer("lwMuonProd");
  p_mu->SetInput(chain);
  p_mu->SetlwMuonsRecoName("lwMuonsReco");
  //p_mu->SetlwMuonsGeneName("lwMuonsGene");
  p_mu->SetEventObjects(fEventObjects);

  lwElectronProducer *p_ele = new lwElectronProducer("lwElectronProd");
  p_ele->SetInput(chain);
  p_ele->SetlwElectronsRecoName("lwElectronsReco");
  //p_ele->SetlwElectronsGeneName("lwElectronsGene");
  p_ele->SetEventObjects(fEventObjects);


  lwJetFromForestProducer *p_pfJet = new lwJetFromForestProducer("lwJetForestProdPF");
  p_pfJet->SetInput(chain);//chain);
  p_pfJet->SetJetContName("akt4PF");
  p_pfJet->DoPFJetID(true);//true);
  p_pfJet->SetGenJetContName("");//akt4Gen");
  p_pfJet->SetEventObjects(fEventObjects);
  p_pfJet->SetRadius(0.4);
  
  lwJetFromForestProducer *p_caloJet = new lwJetFromForestProducer("lwJetForestProdCalo");
  p_caloJet->SetInput(caloJetTree);
  p_caloJet->SetJetContName("akt4Calo");
  p_caloJet->SetGenJetContName("");
  p_caloJet->SetEventObjects(fEventObjects);
  p_caloJet->SetRadius(0.4);

  trackProducer *p_trk = new trackProducer("trackProd");
  p_trk->SetInput(chain);
  p_trk->SetTracksName("tracks");
  p_trk->SetEventObjects(fEventObjects);
  
  
  //---------------------------------------------------------------
  //analysis modules
  //

  //handler to which all modules will be added
  anaBaseTask *handler = new anaBaseTask("handler","handler");

  anaJetMatching *matchingPFCaloJet = new anaJetMatching("matchingPFCaloJet","matchingPFCaloJet");
  matchingPFCaloJet->ConnectEventObject(fEventObjects);
  matchingPFCaloJet->SetHiEvtName("hiEventContainer");
  matchingPFCaloJet->DoPFJet80(true);
  matchingPFCaloJet->DoExcludePhoton30(false);//true);
  matchingPFCaloJet->SetJetsNameBase("akt4PF");
  matchingPFCaloJet->SetJetsNameTag("akt4Calo");
  matchingPFCaloJet->SetNCentBins(1);
  matchingPFCaloJet->SetMatchingType(0);
  //handler->Add(matchingPFCaloJet);  
    
  anaTtbarEMu *anaTtbar = new anaTtbarEMu("anaPFvsCaloJet","anaPFvsCaloJet");
  anaTtbar->ConnectEventObject(fEventObjects);
  anaTtbar->SetHiEvtName("hiEventContainer");
  anaTtbar->SetTriggerMapName("triggerMap");
  anaTtbar->SetParticlesName("pfParticles");
  anaTtbar->SetElectronsName("lwElectronsReco");
  anaTtbar->SetMuonsName("lwMuonsReco");
  anaTtbar->SetRecoJetsName("akt4PF");
  anaTtbar->SetMetType(anaTtbarEMu::kPFRaw);
  handler->Add(anaTtbar);
  
  anaPFvsCaloJet *anaCaloPFJet = new anaPFvsCaloJet("anaCalovsPFJet","anaCalovsPFJet");
  anaCaloPFJet->ConnectEventObject(fEventObjects);
  anaCaloPFJet->SetHiEvtName("hiEventContainer");
  //  anaCaloPFJet->DoPFJet80(true);
  //  anaCaloPFJet->DoExcludePhoton30(false);//true);
  anaCaloPFJet->SetPFJetsName("akt4Calo");
  anaCaloPFJet->SetCaloJetsName("akt4PF");
  if(isData) {
    anaCaloPFJet->DoPFJet80(true);
    anaCaloPFJet->DoCollisionEventSel(true);
    anaCaloPFJet->DoHBHENoiseFilter(true);
  }
  //handler->Add(anaCaloPFJet);

  /*
  //PF-GEN matching
  anaJetMatching *matchingGenPFJet = new anaJetMatching("matchingGenPFJet","matchingGenPFJet");
  matchingGenPFJet->ConnectEventObject(fEventObjects);
  matchingGenPFJet->SetHiEvtName("");
  matchingGenPFJet->SetJetsNameBase("akt4Gen");
  matchingGenPFJet->SetJetsNameTag("akt4PF");
  matchingGenPFJet->SetNCentBins(1);
  handler->Add(matchingGenPFJet);  
  
  anaPFvsCaloJet *anaGenPFJet = new anaPFvsCaloJet("anaGenVsPFJet","anaGenVsPFJet");
  anaGenPFJet->ConnectEventObject(fEventObjects);
  anaGenPFJet->SetHiEvtName("");
  anaGenPFJet->SetPFJetsName("akt4Gen");
  anaGenPFJet->SetCaloJetsName("akt4PF");
  handler->Add(anaGenPFJet);
  */
  
  //---------------------------------------------------------------
  //Event loop
  //---------------------------------------------------------------	  
  Long64_t entries_tot =  chain->GetEntries(); //93064
  std::cout << entries_tot << std::endl;
  if(nentries<0) lastEvent = chain->GetEntries();  
  // Long64_t nentries = 20;//chain->GetEntriesFast();
  Printf("nentries: %lld  tot: %lld",nentries,entries_tot);
  for (Long64_t jentry=firstEvent; jentry<lastEvent; ++jentry) { //49945
    //  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if (jentry%10000==0) Printf("Processing event %d  %d",(int)(jentry), (int)(lastEvent));
    //Run producers
    if (jentry>=entries_tot)break;
    // Printf("produce hiEvent");
    if(!p_evt->Run(jentry));   //hi event properties

    //p_trg->Run(jentry);
    if(!p_mu->Run(jentry));
    if(!p_ele->Run(jentry));
    //Printf("produce pf particles");
    if(!p_pf->Run(jentry));    //pf particles
    if(!p_pfJet->Run(jentry)); //jets
    //p_caloJet->Run(jentry); //jets
    
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

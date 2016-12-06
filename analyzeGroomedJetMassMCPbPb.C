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

void analyzeGroomedJetMass(std::vector<std::string> urls, const char *outname = "eventObjects.root", Long64_t nentries = 20, Int_t firstF = -1, Int_t lastF = -1, Int_t firstEvent = 0, int activateJetShift = 0, int activateZgReweight = 0) {

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

  bool doJES = false;
  subjetSmearing sm[4];
  TF1 *f1Scale[4];
  if(doJES) {
    for( int i = 0; i<4; ++i) {
      sm[i].setInputFileList(Form("/afs/cern.ch/user/m/mverweij/work/PbPb5TeV/mc/subjet/eigenVectors/PbPb/Pythia8/dijet120/inputCent%d.list",i));
      sm[i].initFileList();
      f1Scale[i] = new TF1("f1Scale","[0]",0.,1000.);
      f1Scale[i]->SetParameter(0,-0.04);
      sm[i].setDoScaleVariation(true);
      sm[i].setResolutionFunction(f1Scale[i]);
    }
  }

  bool doJetShift = false;
  if(activateJetShift>0) doJetShift = true;
  float jetShift = 0.;
  if(activateJetShift==1) jetShift = 0.04;
  if(activateJetShift==2) jetShift = -0.04;
  Printf("jetShift: %f",jetShift);

  bool doZgReweight = false;
  if(activateZgReweight>0) doZgReweight = true;
  TF1 *f1ZgReweight = new TF1("f1ZgReweight","pol1",0.,1.);
  f1ZgReweight->SetParameters(9.25354e-01,3.30200e-01);

  bool doJewelZgReweight = false;
  jewelZgReweight jrw;

  TString jetSDName = "aktCs4PFSoftDrop";
  TString jetTreeSDName = "akCsSoftDrop4PFJetAnalyzer";//"akCs4PFSoftDropJetAnalyzer";
  TString jetName = "aktCs4PF";
  TString jetTreeName = "akCs4PFJetAnalyzer";//"akCs4PFSoftDropJetAnalyzer";
  //jetName = "akCs4PFFilter";
  //jetTreeName = "akCs4PFFilterJetAnalyzer";

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

  TChain *pfTree = new TChain("pfcandAnalyzerCS/pfTree");
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
  LWJetProducer *lwjakt = new LWJetProducer("LWJetProducerAKTR040CA","LWJetProducerAKTR040CA");
  //lwjakt->SetInput(chain);
  lwjakt->ConnectEventObject(fEventObjects);
  lwjakt->SetJetType(LWJetProducer::kAKT);
  lwjakt->SetRadius(0.4);
  lwjakt->SetGhostArea(0.01);
  //lwjakt->SetGhostArea(0.005);
  lwjakt->SetPtMinConst(0.);
  lwjakt->SetParticlesName("pfParticles");
  lwjakt->SetJetContName("JetsAKTR040CA");
  lwjakt->SetDoConstituentSubtraction(kFALSE);
  lwjakt->SetDoSoftDrop(true);
  lwjakt->SetUseKtForSoftDrop(false);
  lwjakt->SetSoftDropZCut(0.5);
  lwjakt->SetSoftDropBeta(1.5);
  lwjakt->SetDoJEC(true);
  lwjakt->SetL1Fastjet(strL1);
  lwjakt->SetL2Relative(strL2Rel);
  lwjakt->SetL3Absolute(strL3Abs);
  handler->Add(lwjakt);

  jetSDName = "JetsAKTR040CASD";
  
  anaJetQA *jetQA = new anaJetQA("anaJetQA","anaJetQA");
  jetQA->ConnectEventObject(fEventObjects);
  jetQA->SetJetsName(jetName);
  handler->Add(jetQA);

  anaJetMatching *match = new anaJetMatching("jetMatching","jetMatching");
  match->ConnectEventObject(fEventObjects);
  match->SetHiEvtName("hiEventContainer");
  match->SetJetsNameBase(jetName);
  match->SetJetsNameTag(jetSDName);
  match->SetMatchingType(0);
  handler->Add(match);

  anaZgHistos *anazghistos = new anaZgHistos("anaZgHistos","anaZgHistos");
  anazghistos->ConnectEventObject(fEventObjects);
  anazghistos->SetHiEvtName("hiEventContainer");
  anazghistos->SetJetsName(jetSDName);
  anazghistos->SetJetsRefName(jetName);
  anazghistos->SetNCentBins(4);
  anazghistos->SetJetEtaRange(-1.3,1.3);
  anazghistos->SetDeltaRRange(0.1,999.);
  anazghistos->SetUseRhoMCWeight(true);
  if(doJES) {
    anazghistos->DoSubjetSmearing(true);
    for(int ic = 0; ic<4; ++ic)
      anazghistos->SetSubjetSmearing(sm[ic],ic);
  }
  anazghistos->DoJetShift(doJetShift,jetShift);
  anazghistos->SetZgReweight(doZgReweight,f1ZgReweight);
  anazghistos->SetZgReweightMulti(doJewelZgReweight,jrw);
  handler->Add(anazghistos);

  anaZgHistos *anazghistosNoDrCut = new anaZgHistos("anaZgHistosNoDrCut","anaZgHistosNoDrCut");
  anazghistosNoDrCut->ConnectEventObject(fEventObjects);
  anazghistosNoDrCut->SetHiEvtName("hiEventContainer");
  anazghistosNoDrCut->SetJetsName(jetSDName);
  anazghistosNoDrCut->SetJetsRefName(jetName);
  anazghistosNoDrCut->SetNCentBins(4);
  anazghistosNoDrCut->SetJetEtaRange(-1.3,1.3);
  anazghistosNoDrCut->SetDeltaRRange(0.,999.);
  anazghistosNoDrCut->SetUseRhoMCWeight(true);
  if(doJES) {
    anazghistosNoDrCut->DoSubjetSmearing(true);
    for(int ic = 0; ic<4; ++ic)
      anazghistosNoDrCut->SetSubjetSmearing(sm[ic],ic);
  }
  anazghistosNoDrCut->DoJetShift(doJetShift,jetShift);
  anazghistosNoDrCut->SetZgReweight(doZgReweight,f1ZgReweight);
  anazghistosNoDrCut->SetZgReweightMulti(doJewelZgReweight,jrw);
  handler->Add(anazghistosNoDrCut);

  anaZgHistos *anazghistosdrSmall = new anaZgHistos("anaZgHistosDrSmall","anaZgHistosDrSmall");
  anazghistosdrSmall->ConnectEventObject(fEventObjects);
  anazghistosdrSmall->SetHiEvtName("hiEventContainer");
  anazghistosdrSmall->SetJetsName(jetSDName);
  anazghistosdrSmall->SetJetsRefName(jetName);
  anazghistosdrSmall->SetNCentBins(4);
  anazghistosdrSmall->SetJetEtaRange(-1.3,1.3);
  anazghistosdrSmall->SetDeltaRRange(0.1,0.2);
  anazghistosdrSmall->SetUseRhoMCWeight(true);
  if(doJES) {
    anazghistosdrSmall->DoSubjetSmearing(true);
    for(int ic = 0; ic<4; ++ic)
      anazghistosdrSmall->SetSubjetSmearing(sm[ic],ic);
  }
  anazghistosdrSmall->DoJetShift(doJetShift,jetShift);
  anazghistosdrSmall->SetZgReweight(doZgReweight,f1ZgReweight);
  anazghistosdrSmall->SetZgReweightMulti(doJewelZgReweight,jrw);
  handler->Add(anazghistosdrSmall);

  anaZgHistos *anazghistosdrLarge = new anaZgHistos("anaZgHistosDrLarge","anaZgHistosDrLarge");
  anazghistosdrLarge->ConnectEventObject(fEventObjects);
  anazghistosdrLarge->SetHiEvtName("hiEventContainer");
  anazghistosdrLarge->SetJetsName(jetSDName);
  anazghistosdrLarge->SetJetsRefName(jetName);
  anazghistosdrLarge->SetNCentBins(4);
  anazghistosdrLarge->SetJetEtaRange(-1.3,1.3);
  anazghistosdrLarge->SetDeltaRRange(0.2,999.);
  anazghistosdrLarge->SetUseRhoMCWeight(true);
  if(doJES) {
    anazghistosdrLarge->DoSubjetSmearing(true);
    for(int ic = 0; ic<4; ++ic)
      anazghistosdrLarge->SetSubjetSmearing(sm[ic],ic);
  }
  anazghistosdrLarge->DoJetShift(doJetShift,jetShift);
  anazghistosdrLarge->SetZgReweight(doZgReweight,f1ZgReweight);
  anazghistosdrLarge->SetZgReweightMulti(doJewelZgReweight,jrw);
  handler->Add(anazghistosdrLarge);

  //------------------------------------------------------------------------------------------------
  //                  GEN JETS
  //------------------------------------------------------------------------------------------------

  anaZgHistos *anazghistosGen = new anaZgHistos("anazghistosGen","anazghistosGen");
  anazghistosGen->ConnectEventObject(fEventObjects);
  anazghistosGen->SetHiEvtName("hiEventContainer");
  anazghistosGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosGen->SetJetsRefName(""); //
  anazghistosGen->SetNCentBins(4);
  anazghistosGen->SetJetEtaRange(-1.3,1.3);
  anazghistosGen->SetDeltaRRange(0.1,999.);
  anazghistosGen->SetUseRhoMCWeight(true);
  anazghistosGen->SetZgReweightMulti(doJewelZgReweight,jrw);
  // handler->Add(anazghistosGen);

  anaZgHistos *anazghistosDrSmallGen = new anaZgHistos("anazghistosDrSmallGen","anazghistosDrSmallGen");
  anazghistosDrSmallGen->ConnectEventObject(fEventObjects);
  anazghistosDrSmallGen->SetHiEvtName("hiEventContainer");
  anazghistosDrSmallGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosDrSmallGen->SetJetsRefName(""); //
  anazghistosDrSmallGen->SetNCentBins(4);
  anazghistosDrSmallGen->SetJetEtaRange(-1.3,1.3);
  anazghistosDrSmallGen->SetDeltaRRange(0.1,0.2);
  anazghistosDrSmallGen->SetUseRhoMCWeight(true);
  anazghistosDrSmallGen->SetZgReweightMulti(doJewelZgReweight,jrw);
  // handler->Add(anazghistosDrSmallGen);

  anaZgHistos *anazghistosDrLargeGen = new anaZgHistos("anazghistosDrLargeGen","anazghistosDrLargeGen");
  anazghistosDrLargeGen->ConnectEventObject(fEventObjects);
  anazghistosDrLargeGen->SetHiEvtName("hiEventContainer");
  anazghistosDrLargeGen->SetJetsName("akt4Gen"); //ungroomed jets with groomed subjet variables at gen level
  anazghistosDrLargeGen->SetJetsRefName(""); //
  anazghistosDrLargeGen->SetNCentBins(4);
  anazghistosDrLargeGen->SetJetEtaRange(-1.3,1.3);
  anazghistosDrLargeGen->SetDeltaRRange(0.2,999.);
  anazghistosDrLargeGen->SetUseRhoMCWeight(true);
  anazghistosDrLargeGen->SetZgReweightMulti(doJewelZgReweight,jrw);
  // handler->Add(anazghistosDrLargeGen);

  anaZgHistos *anazghistosNoFakes = new anaZgHistos("anaZgHistosNoFakes","anaZgHistosNoFakes");
  anazghistosNoFakes->ConnectEventObject(fEventObjects);
  anazghistosNoFakes->SetHiEvtName("hiEventContainer");
  anazghistosNoFakes->SetJetsName(jetSDName);
  anazghistosNoFakes->SetJetsRefName(jetName);
  anazghistosNoFakes->SetNCentBins(4);
  anazghistosNoFakes->SetJetEtaRange(-2.,2.);
  anazghistosNoFakes->SetMinRefPt(10.);
  // handler->Add(anazghistosNoFakes);
 
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

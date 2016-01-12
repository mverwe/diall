#include "UserCode/diall/interface/diParticle.h"
#include "UserCode/diall/interface/ForestElectrons.h"
#include "UserCode/diall/interface/ForestMuons.h"
#include "UserCode/diall/interface/ForestPFs.h"
#include "UserCode/diall/interface/ForestGenParticles.h"
#include "UserCode/diall/interface/ForestGenParticlesOld.h"
#include "UserCode/diall/interface/ForestJets.h"
#include "UserCode/diall/interface/ForestTracks.h"
#include "UserCode/diall/interface/genParticle.h"
#include "UserCode/diall/interface/genParticleProducer.h"
#include "UserCode/diall/interface/genParticleProducerOld.h"
#include "UserCode/diall/interface/hiEventContainer.h"
#include "UserCode/diall/interface/hiEventProducer.h"
#include "UserCode/diall/interface/inputBase.h"
#include "UserCode/diall/interface/lwElectron.h"
#include "UserCode/diall/interface/lwElectronProducer.h"
#include "UserCode/diall/interface/lwJet.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include "UserCode/diall/interface/lwJetFromForestProducer.h"
#include "UserCode/diall/interface/lwMuon.h"
#include "UserCode/diall/interface/lwMuonProducer.h"
#include "UserCode/diall/interface/particleBase.h"
#include "UserCode/diall/interface/pfParticle.h"
#include "UserCode/diall/interface/pfParticleProducer.h"
#include "UserCode/diall/interface/rhoMap.h"
#include "UserCode/diall/interface/trackProducer.h"

#include "UserCode/diall/interface/FJ_includes.h"
#include "UserCode/diall/interface/FJJetShape.h"
#include "UserCode/diall/interface/FJWrapper.h"
#include "UserCode/diall/interface/LWJetProducer.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/anaHadronIsolation.h"
#include "UserCode/diall/interface/anaJetEnergyScale.h"
#include "UserCode/diall/interface/anaJetMatching.h"
#include "UserCode/diall/interface/anaJetQA.h"
#include "UserCode/diall/interface/anaMET.h"
#include "UserCode/diall/interface/anaMuonIsolation.h"
#include "UserCode/diall/interface/anaMuonMatcher.h"
#include "UserCode/diall/interface/anaPFCandidates.h"
#include "UserCode/diall/interface/anaPFvsCaloJet.h"
#include "UserCode/diall/interface/anaPuppiParticles.h"
#include "UserCode/diall/interface/anaPuppiProducer.h"
#include "UserCode/diall/interface/anaRhoProducer.h"
#include "UserCode/diall/interface/anaZJetMCResponse.h"
#include "UserCode/diall/interface/anaZToMuMu.h"

#ifdef __CINT__

#pragma link off all class;
#pragma link off all function;
#pragma link off all global;
#pragma link off all typedef;
#pragma link off all namespace;

#pragma link C++ class diParticle;
#pragma link C++ class ForestElectrons;
#pragma link C++ class ForestMuons;
#pragma link C++ class ForestPFs;
#pragma link C++ class ForestGenParticles;
#pragma link C++ class ForestGenParticlesOld;
#pragma link C++ class ForestJets;
#pragma link C++ class ForestTracks;
#pragma link C++ class genParticle;
#pragma link C++ class genParticleProducer;
#pragma link C++ class genParticleProducerOld;
#pragma link C++ class hiEventContainer;
#pragma link C++ class hiEventProducer;
#pragma link C++ class inputBase;
#pragma link C++ class lwElectron;
#pragma link C++ class lwElectronProducer;
#pragma link C++ class lwJet;
#pragma link C++ class lwJetContainer;
#pragma link C++ class lwJetFromForestProducer;
#pragma link C++ class lwMuon;
#pragma link C++ class lwMuonProducer;
#pragma link C++ class particleBase;
#pragma link C++ class pfParticle;
#pragma link C++ class pfParticleProducer;
#pragma link C++ class rhoMap;
#pragma link C++ class trackProducer;

#pragma link C++ class FJJetShape;
#pragma link C++ class FJWrapper;
#pragma link C++ class LWJetProducer;

#pragma link C++ class anaBaseTask;
#pragma link C++ class anaHadronIsolation;
#pragma link C++ class anaJetEnergyScale;
#pragma link C++ class anaJetMatching;
#pragma link C++ class anaJetQA;
#pragma link C++ class anaMET;
#pragma link C++ class anaMuonIsolation;
#pragma link C++ class anaMuonMatcher;
#pragma link C++ class anaPFCandidates;
#pragma link C++ class anaPFvsCaloJet;
#pragma link C++ class anaPuppiParticles;
#pragma link C++ class anaPuppiProducer;
#pragma link C++ class anaRhoProducer;
#pragma link C++ class anaZJetMCResponse;
#pragma link C++ class anaZToMuMu;

#endif

// Local Variables:
// mode: c++
// mode: sensitive
// c-basic-offset: 8
// End:

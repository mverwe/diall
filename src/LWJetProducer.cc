#include <iostream>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

#include "UserCode/diall/interface/FJWrapper.h"
#include "UserCode/diall/interface/LWJetProducer.h"
#include <UserCode/diall/interface/lwJetContainer.h>
#include <UserCode/diall/interface/lwJet.h>

#include <CondFormats/JetMETObjects/interface/JetCorrectorParameters.h>

#include "fastjet/contrib/SoftDrop.hh"

using namespace std;

ClassImp(LWJetProducer)

//__________________________________________________________
LWJetProducer::LWJetProducer() :
anaBaseTask("LWJetProducer","LWJetProducer"),
  fJetType(kAKT),
  fRadius(0.3),
  fRecombScheme(fastjet::E_scheme),
  fGhostArea(0.005),
  fPtMinConst(0.),
  fIdConst(-1),
  fChargedJets(false),
  fjInputs(),
  fjJets(),
  fIsInit(0),
  fConst(),
  fConstName(""),
  flwJetContainer(),
  flwJetContName("jetContainer"),
  fFastJetWrapper("LWJetProducer","LWJetProducer"),
  fDoConstSubtraction(kFALSE),
  fRhoMapName(""),
  fRhoMap(),
  fRhoMMapName(""),
  fRhoMMap(),
  flwCSJetContainer(),
  flwCSJetContName(""),
  fAlpha(0.),
  fDoSoftDrop(false),
  fUseKtForSoftDrop(false),
  fSDZcut(0.1),
  fSDBeta(0.),
  flwSDJetContainer(0),
  fDoJEC(false),
  fDoJECCS(false),
  fJetCorrector(0),
  fL1Fastjet(""),
  fL2Relative(""),
  fL3Absolute(""),
  fL2L3Residual("")
{
  //default constructor
}

//__________________________________________________________
LWJetProducer::LWJetProducer(const char *name, const char *title) :
  anaBaseTask(name,title),
  fJetType(kAKT),
  fRadius(0.3),
  fRecombScheme(fastjet::E_scheme),
  fGhostArea(0.005),
  fPtMinConst(0.),
  fIdConst(-1),
  fChargedJets(false),
  fjInputs(),
  fjJets(),
  fIsInit(0),
  fConst(),
  fConstName(""),
  flwJetContainer(),
  flwJetContName("jetContainer"),
  fFastJetWrapper(name,name),
  fDoConstSubtraction(kFALSE),
  fRhoMapName(""),
  fRhoMap(),
  fRhoMMapName(""),
  fRhoMMap(),
  flwCSJetContainer(),
  flwCSJetContName(""),
  fAlpha(0.),
  fDoSoftDrop(false),
  fUseKtForSoftDrop(false),
  fSDZcut(0.1),
  fSDBeta(0.),
  flwSDJetContainer(0),
  fDoJEC(false),
  fDoJECCS(false),
  fJetCorrector(0),
  fL1Fastjet(""),
  fL2Relative(""),
  fL3Absolute(""),
  fL2L3Residual("")
{
  //standard constructor
}

//----------------------------------------------------------
void LWJetProducer::Exec(Option_t * /*option*/)
{
  //Initialize and find jets
  if(!fIsInit) {
    if(!Init()) return;
  }
  FindJets();
}

//__________________________________________________________
Bool_t LWJetProducer::Init() {

  TString name("kt");
  fastjet::JetAlgorithm jalgo(fastjet::kt_algorithm);
  if ((fJetType & kAKT) != 0) {
    name  = "antikt";
    jalgo = fastjet::antikt_algorithm;
  }
  //Printf("Running %s jet finder",name.Data());
    
  // setup fj wrapper
  fFastJetWrapper.SetName(GetName());//name);
  fFastJetWrapper.SetTitle(GetName());//name);
  fFastJetWrapper.SetAreaType(fastjet::active_area_explicit_ghosts);
  fFastJetWrapper.SetGhostArea(fGhostArea);
  fFastJetWrapper.SetR(fRadius);
  fFastJetWrapper.SetAlgorithm(jalgo);
  fFastJetWrapper.SetRecombScheme(static_cast<fastjet::RecombinationScheme>(fRecombScheme));

  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  }
  
  //Retrieve event objects
  if(!fConst && !fConstName.IsNull()) {
    fConst = dynamic_cast<TClonesArray*>(fEventObjects->FindObject(fConstName));
    if(!fConst) {
      Printf("%s: couldn't find constituents %s",GetName(),fConstName.Data());
      return kFALSE;
    }
  }

  //get background density maps
  if(!fRhoMap && !fRhoMapName.IsNull())
    fRhoMap = dynamic_cast<rhoMap*>(fEventObjects->FindObject(fRhoMapName.Data()));

  if(!fRhoMMap && !fRhoMMapName.IsNull())
    fRhoMMap = dynamic_cast<rhoMap*>(fEventObjects->FindObject(fRhoMMapName.Data()));

  //Create event objects
  if(!flwJetContName.IsNull() && !fEventObjects->FindObject(flwJetContName)) {
    flwJetContainer = new lwJetContainer(flwJetContName);
    flwJetContainer->Init();
    flwJetContainer->SetJetRadius(fRadius);
    fEventObjects->Add(flwJetContainer);
  } else {
    Printf("%s: 1--Object with name %s already in event! Returning",GetName(),flwJetContName.Data());
    return kFALSE;
  }
  
  if(fDoConstSubtraction && !flwCSJetContName.IsNull() && !fEventObjects->FindObject(flwCSJetContName)) {
    flwCSJetContainer = new lwJetContainer(flwCSJetContName);
    flwCSJetContainer->Init();
    flwCSJetContainer->SetJetRadius(fRadius);
    fEventObjects->Add(flwCSJetContainer);
  }

  if(fDoSoftDrop) {
    TString flwSDJetContName = Form("%sSD",flwJetContName.Data());
    flwSDJetContainer = new lwJetContainer(flwSDJetContName);
    flwSDJetContainer->Init();
    flwSDJetContainer->SetJetRadius(fRadius);
    fEventObjects->Add(flwSDJetContainer);
  } else
    flwSDJetContainer = NULL;

  if(!fL1Fastjet.IsNull() || !fL2Relative.IsNull() || !fL3Absolute.IsNull() || !fL2L3Residual.IsNull()) {
    JetCorrectorParameters *ResJetPar = 0x0; 
    if(!fL2L3Residual.IsNull()) ResJetPar = new JetCorrectorParameters(fL2L3Residual.Data()); 
    JetCorrectorParameters *L3JetPar  = 0x0;
    if(!fL3Absolute.IsNull()) L3JetPar = new JetCorrectorParameters(fL3Absolute.Data());
    JetCorrectorParameters *L2JetPar  = 0x0;
    if(!fL2Relative.IsNull()) L2JetPar = new JetCorrectorParameters(fL2Relative.Data());
    JetCorrectorParameters *L1JetPar  = 0x0;
    if(!fL1Fastjet.IsNull()) L1JetPar = new JetCorrectorParameters(fL1Fastjet.Data());
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
    vector<JetCorrectorParameters> vPar;
    if(L1JetPar)  vPar.push_back(*L1JetPar);
    if(L2JetPar)  vPar.push_back(*L2JetPar);
    if(L3JetPar)  vPar.push_back(*L3JetPar);
    if(ResJetPar) vPar.push_back(*ResJetPar);
    fJetCorrector = new FactorizedJetCorrector(vPar);
  }

  fIsInit = kTRUE;
  return kTRUE;
}

//__________________________________________________________
Int_t LWJetProducer::FindJets() {

  //  Printf(" LWJetProducer::FindJets() ");
  //clear
  fFastJetWrapper.Clear();
  fjInputs.clear();
  flwJetContainer->ClearVec();
  flwJetContainer->ConnectConstituents(fConst);

  //collect input particles and pass to fj wrapper
  Int_t npart = fConst->GetEntriesFast();
  fjInputs.reserve(npart);

  int nacc = 0;
  for (int i = 0; i < npart; i++) {
    particleBase *fRecoParticle = static_cast<particleBase*>(fConst->At(i));
    if(fRecoParticle->GetLorentzVector().Pt()<fPtMinConst) continue;
    if(fIdConst>-1 && TMath::Abs(fRecoParticle->GetId())!=fIdConst) continue;
    if(fChargedJets && fRecoParticle->GetCharge()==0) continue;

    //Printf("%d Accept particle: pt=%f eta=%f phi=%f ch=%d",nacc+1,fRecoParticle->Pt(),fRecoParticle->Eta(),fRecoParticle->Phi(),fRecoParticle->GetCharge());
    
    fFastJetWrapper.AddInputVector(fRecoParticle->GetLorentzVector().Px(),
                                   fRecoParticle->GetLorentzVector().Py(),
                                   fRecoParticle->GetLorentzVector().Pz(),
                                   fRecoParticle->GetLorentzVector().E(),
                                   i);
    ++nacc;
  }
  //  Printf("nacc: %d",nacc);
  if (fFastJetWrapper.GetInputVectors().size() == 0) return 0;

  // run jet finder
  fFastJetWrapper.Run();

  // retrieve fastjet jets and sort according to jet pt
  std::vector<fastjet::PseudoJet> jets_incl = fFastJetWrapper.GetInclusiveJets();
  static Int_t indexes[9999] = {-1};
  GetSortedArray(indexes, jets_incl);

  //loop over jets and store in jet container
  Int_t jetCount = 0;
  Int_t jetCountSD = 0;
  for (UInt_t ijet = 0; ijet < jets_incl.size(); ++ijet) {
    Int_t ij = indexes[ijet];
    if(jets_incl[ij].perp()<1e-6) continue; //remove pure ghost jets
    double pt = jets_incl[ij].perp();
    double mass = jets_incl[ij].m();
    //Printf("Found jet %d pt=%f mass=%f",ij,pt,mass);
    if(fJetCorrector && fDoJEC) {
      fJetCorrector->setJetEta(jets_incl[ij].eta());
      fJetCorrector->setJetPt(pt);
      fJetCorrector->setJetA(fFastJetWrapper.GetJetArea(ij));
      fJetCorrector->setRho(0.); 
      double correction = fJetCorrector->getCorrection();
      pt *= correction;
      mass *= correction;
    }
    lwJet *jet = new lwJet(pt, jets_incl[ij].eta(), jets_incl[ij].phi(), mass,jets_incl[ij].user_index());
    jet->SetRawPt(jets_incl[ij].perp());
    jet->SetRawM(jets_incl[ij].m());
    jet->SetArea(fFastJetWrapper.GetJetArea(ij));

    // Fill constituent info
    std::vector<fastjet::PseudoJet> constituents(fFastJetWrapper.GetJetConstituents(ij));
    std::vector<fastjet::PseudoJet> inputsRecluster;
    Int_t nc = 0;
    for (UInt_t ic = 0; ic < constituents.size(); ++ic) {
      Int_t uid = constituents[ic].user_index();
      if (uid == -1) continue; //ghost particle
      else {
        jet->AddConstituent(uid);
        inputsRecluster.push_back(constituents[ic]);
        nc++;
      }
    }

    if(nc>0) { //only store if not pure ghost jet
      flwJetContainer->AddJet(jet,jetCount);
      ++jetCount;
      //Printf("jet added %d",jetCount);
      if(fDoSoftDrop) {
        JetDefPtr fjJetDefinitionRecluster_;
        if(fUseKtForSoftDrop)
          fjJetDefinitionRecluster_ = JetDefPtr(new fastjet::JetDefinition(fastjet::kt_algorithm,999.));
        else
          fjJetDefinitionRecluster_ = JetDefPtr(new fastjet::JetDefinition(fastjet::cambridge_algorithm,999.));
        ClusterSequencePtr fjClusterSeqRecluster_ = ClusterSequencePtr( new fastjet::ClusterSequence( inputsRecluster, *fjJetDefinitionRecluster_ ));

        std::vector<fastjet::PseudoJet> tempJets = fastjet::sorted_by_pt(fjClusterSeqRecluster_->inclusive_jets(10.));
        if(tempJets.size()<1) continue;

        fastjet::contrib::SoftDrop * sd = new fastjet::contrib::SoftDrop(fSDBeta, fSDZcut, fRadius );
        sd->set_verbose_structure(true);
        sd->set_reclustering(false);

        fastjet::PseudoJet transformedJet = tempJets[0];
        if ( transformedJet == 0 ) {
          //fjClusterSeqRecluster_->delete_self_when_unused();
          if(sd) { delete sd; sd = 0;}
          continue;
        }

        transformedJet = (*sd)(transformedJet);
        //double sym = transformedJet.structure_of<fastjet::contrib::SoftDrop>().symmetry();
        //int ndrop = transformedJet.structure_of<fastjet::contrib::SoftDrop>().dropped_count();
              
        //get subjets
        std::vector<fastjet::PseudoJet> subjets = transformedJet.pieces();
        std::vector<fastjet::PseudoJet>::const_iterator itSubJetBegin = subjets.begin(),
          itSubJet = itSubJetBegin, itSubJetEnd = subjets.end();


        std::vector<float> sjpt;
        std::vector<float> sjeta;
        std::vector<float> sjphi;
        std::vector<float> sjm;
        
        for (; itSubJet != itSubJetEnd; ++itSubJet ){
          
          fastjet::PseudoJet const & subjet = *itSubJet;
          sjpt.push_back(subjet.perp());
          sjeta.push_back(subjet.eta());
          sjphi.push_back(subjet.phi());
          sjm.push_back(subjet.m());
        }
          
        double ptSD = transformedJet.perp();
        double massSD = transformedJet.m();
        if(fJetCorrector && fDoJEC) {
          fJetCorrector->setJetEta(transformedJet.eta());
          fJetCorrector->setJetPt(pt);
          fJetCorrector->setJetA(fFastJetWrapper.GetJetArea(ij));
          fJetCorrector->setRho(0.); 
          double correction = fJetCorrector->getCorrection();
          ptSD *= correction;
          massSD *= correction;
        }

        double sym = transformedJet.structure_of<fastjet::contrib::SoftDrop>().symmetry();
        if(ptSD>100.) Printf("%s: SD jet: %f eta: %f phi: %f zg: %f nsubjets: %d",GetName(),ptSD,transformedJet.eta(),transformedJet.phi(),sym,(int)sjpt.size());
        
        lwJet *jetSD = new lwJet(ptSD, transformedJet.eta(), transformedJet.phi(), massSD,transformedJet.user_index());
        jetSD->SetRawPt(transformedJet.perp());
        jetSD->SetRawM(transformedJet.m());
        jetSD->SetSubJetPt(sjpt);
        jetSD->SetSubJetEta(sjeta);
        jetSD->SetSubJetPhi(sjphi);
        jetSD->SetSubJetM(sjm);

        flwSDJetContainer->AddJet(jetSD,jetCountSD);
        ++jetCountSD;
        
        if(sd) { delete sd; sd = 0;}
      }
      
    }
  }
  //Printf("jet finding done. Now sort");
  flwJetContainer->SortJets();
  //Printf("Sorting done");
  if(flwSDJetContainer) flwSDJetContainer->SortJets();
  //Printf("Event had %d jets  %d",jetCount,(Int_t)fFastJetWrapper.GetInclusiveJets().size());

  if(fDoConstSubtraction && flwCSJetContainer && fRhoMap && fRhoMMap) {
    //Printf("doing constituent subtraction");
    flwCSJetContainer->ClearVec();
    //first put jets in separate vectors for eta ranges
    std::vector<fastjet::PseudoJet> unsub;
    std::vector<fastjet::PseudoJet> jets_sub;
    jets_sub.reserve(jets_incl.size());
    Int_t neta = fRhoMap->GetNEtaBins();
    for(Int_t ie = 0; ie<neta; ie++) {
      unsub.clear();

      //Get rho and rhoM for eta regio
      double rho = fRhoMap->GetValue(ie+1);
      double rhom = fRhoMMap->GetValue(ie+1);
      
      //initialize constituent subtraction
      fastjet::contrib::ConstituentSubtractor *subtractor;
      subtractor     = new fastjet::contrib::ConstituentSubtractor(rho,rhom,fAlpha,-1.);
      
      for (UInt_t ijet = 0; ijet < jets_incl.size(); ++ijet) {
        Int_t ieta = fRhoMap->GetIndex(jets_incl[ijet].eta());
        if((ieta-1)!=ie) continue;
        unsub.push_back(jets_incl[ijet]);
      }
        
      for (unsigned i = 0; i < unsub.size(); i++) {
        fastjet::PseudoJet subtracted_jet(0.,0.,0.,0.);
        if(unsub[i].perp()>0.) {
          subtracted_jet = (*subtractor)(unsub[i]);
          jets_sub.push_back(subtracted_jet);
        }
      }
      if(subtractor) { delete subtractor; subtractor = 0;} 
    }
  
    //loop over CS jets and store in jet container
    jetCount = 0;
    for (UInt_t ij = 0; ij < jets_sub.size(); ++ij) {
      if(fabs(jets_sub[ij].perp()<1e-6)) continue; //remove pure ghost jets
      double pt = jets_sub[ij].perp();
      double mass = jets_sub[ij].m();
      if(fJetCorrector && fDoJECCS) {
        fJetCorrector->setJetEta(jets_sub[ij].eta());
        fJetCorrector->setJetPt(pt);
        fJetCorrector->setJetA(fFastJetWrapper.GetJetArea(ij));
        fJetCorrector->setRho(0.);
        double correction = fJetCorrector->getCorrection();
        //if(pt>40.) Printf("pt: %f eta: %f mass: %f correction: %f",pt,jets_sub[ij].eta(),mass,correction);
        pt *= correction;
        mass *= correction;
      }
      lwJet *jet = new lwJet(pt, jets_sub[ij].eta(), jets_sub[ij].phi(), mass,jets_sub[ij].user_index());
      jet->SetRawPt(jets_sub[ij].perp());
      jet->SetRawM(jets_sub[ij].m());
      jet->SetArea(fFastJetWrapper.GetJetArea(ij));
      flwCSJetContainer->AddJet(jet,jetCount);
      ++jetCount;
    }
    flwCSJetContainer->SortJets();
    jets_sub.clear();
  }//constituent subtraction

  //  Printf("done jet finding");

  // return fFastJetWrapper.GetInclusiveJets().size();
  return jets_incl.size();
}

//_______________________________________________________________
Bool_t LWJetProducer::GetSortedArray(Int_t indexes[], std::vector<fastjet::PseudoJet> array) const
{
  // Get the leading jets.

  static Float_t pt[9999] = {0};

  const Int_t n = (Int_t)array.size();

  if (n < 1)
    return kFALSE;

  for (Int_t i = 0; i < n; i++)
    pt[i] = array[i].perp();

  TMath::Sort(n, pt, indexes);

  return kTRUE;
}

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: BetaChamberEventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file BetaChamberEventAction.cc
/// \brief Implementation of the BetaChamberEventAction class

#include "BetaChamberEventAction.hh"
#include "BetaChamberRunAction.hh"
#include "BetaChamberAnalysis.hh"
#include "BetaChamberSiliconHit.hh"
#include "BetaChamberAbsorberHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4VTrajectory.hh"
#include "G4TrajectoryContainer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberEventAction::BetaChamberEventAction()
: G4UserEventAction(),
  fSensDetector(0),
  fEdepSi(0.),
  fSiSensID(-1),
  fAbsSensID(-1),
   LPIPS(0.),
   stepsInPIPS(0),
   NewEvent(false)
{
    G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberEventAction::~BetaChamberEventAction()
{


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberEventAction::BeginOfEventAction(const G4Event*)
{
  fEdepSi = 0.;
  LPIPS=0.;
  stepsInPIPS=0;
  NewEvent=1;
  G4cout <<"status event = "<<NewEvent<<"\n";


  auto sdManager = G4SDManager::GetSDMpointer();
  fSiSensID =sdManager->GetCollectionID("SiliconChipSV/SiliconSensColl");
  fAbsSensID=sdManager->GetCollectionID("AbsorberSV/AbsorberSensColl");
  fSensDetector =sdManager->FindSensitiveDetector("SiliconChipSV");
  G4cout<< "fSensDetector ="<< fSensDetector<<"\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberEventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  //fRunAction->AddEdep(fEdep);
   // bisher-----------------------------------------------------------
/*
   //----------------------- Silicon SD --------------------------------------
   auto hce = event->GetHCofThisEvent();
   auto hHCSilicon = static_cast<BetaChamberSiliconHitsCollection*>(hce->GetHC(fSiSensID));
   auto analysisManager = G4AnalysisManager::Instance();

   auto nhit  = hHCSilicon->entries();
   analysisManager->FillH1(0, nhit );

   for (auto i=0;i<nhit ;i++) {
    auto hit = (*hHCSilicon)[i];
    auto localPos = hit->GetPos();
    analysisManager->FillH2(0, localPos.x(), localPos.y());
   }
  // ----- test mc_ekin
   G4int vertex_totnum = event->GetNumberOfPrimaryVertex();
   G4PrimaryVertex* primary_vertex;
   G4PrimaryParticle* primary_particle;
   G4int mc_totnumparticle=0;

   for(G4int i =0;i<vertex_totnum;i++){
       primary_vertex=event->GetPrimaryVertex(i);
       G4int vertex_NOP=primary_vertex->GetNumberOfParticle();


       for(G4int j=0;j<vertex_NOP;j++){
           primary_particle=primary_vertex->GetPrimary(j);
           G4ThreeVector P = primary_particle->GetMomentum()/keV;
           G4double      M = primary_particle->GetMass()/keV;
           G4double mc_ekin=0.;
           if(P.mag()<M/100) mc_ekin=P.mag2()/2*M;
           else mc_ekin=sqrt(P.mag2()+M*M)-M;
           mc_totnumparticle++;
           G4double momentumX =primary_particle->GetPx()/keV;
           G4double momentumY =primary_particle->GetPy()/keV;
           G4double momentumZ =primary_particle->GetPz()/keV;
           analysisManager->FillNtupleDColumn(3, mc_ekin);
           analysisManager->FillNtupleDColumn(5, momentumX);
           analysisManager->FillNtupleDColumn(6, momentumY);
           analysisManager->FillNtupleDColumn(7, momentumZ);
       }
  }
  analysisManager->FillNtupleIColumn(4, mc_totnumparticle);


  // Fill ntuple

  // Silicon Hits
  analysisManager->FillNtupleIColumn(0, hHCSilicon->entries());

  // Silicon Energy

 G4double totalSiE = 0.;
 for (auto i=0;i<nhit;i++) {
   auto hit = (*hHCSilicon)[i];
   auto edep = hit->GetEdep();
   if (edep>0.) {
     totalSiE += edep/keV;
   }
 }
 analysisManager->FillNtupleDColumn(1, totalSiE);



 // Time 1
 for (auto i=0;i<nhit;i++) {
    analysisManager->FillNtupleDColumn(2,(*hHCSilicon)[i]->GetTime());
 }
   analysisManager->AddNtupleRow();

*/
 // ----------------------------------------------------------- bisher


 auto hce = event->GetHCofThisEvent();
 auto hHCSilicon = static_cast<BetaChamberSiliconHitsCollection*>(hce->GetHC(fSiSensID));
 auto analysisManager = G4AnalysisManager::Instance();


 auto nhit  = hHCSilicon->entries();
 analysisManager->FillH1(0, nhit );

 for (auto i=0;i<nhit ;i++) {
  auto hit = (*hHCSilicon)[i];
  auto localPos = hit->GetPos();
  analysisManager->FillH2(0, localPos.x(), localPos.y());
 }
// ----- test mc_ekin
 G4int vertex_totnum = event->GetNumberOfPrimaryVertex();
 G4PrimaryVertex* primary_vertex;
 G4PrimaryParticle* primary_particle;
 G4int mc_totnumparticle=0;

 for(G4int i =0;i<vertex_totnum;i++){
     primary_vertex=event->GetPrimaryVertex(i);
     G4int vertex_NOP=primary_vertex->GetNumberOfParticle();
     G4double xp =primary_vertex->GetX0()/mm;
     G4double yp =primary_vertex->GetY0()/mm;
     G4double zp =primary_vertex->GetZ0()/mm;

     analysisManager->FillNtupleDColumn(1,5, xp);
     analysisManager->FillNtupleDColumn(1,6, yp);
     analysisManager->FillNtupleDColumn(1,7, zp);
     for(G4int j=0;j<vertex_NOP;j++){
         primary_particle=primary_vertex->GetPrimary(j);
         G4ThreeVector P = primary_particle->GetMomentum()/keV;
         G4double      M = primary_particle->GetMass()/keV;
         G4double mc_ekin=0.;
         if(P.mag()<M/100) mc_ekin=P.mag2()/2*M;
         else mc_ekin=sqrt(P.mag2()+M*M)-M;
         mc_totnumparticle++;
         G4double momentumX =primary_particle->GetPx()/keV;
         G4double momentumY =primary_particle->GetPy()/keV;
         G4double momentumZ =primary_particle->GetPz()/keV;
         analysisManager->FillNtupleDColumn(1,0, mc_ekin);
          analysisManager->FillNtupleDColumn(1,1, P.mag());
         analysisManager->FillNtupleDColumn(1,2, momentumX);
         analysisManager->FillNtupleDColumn(1,3, momentumY);
         analysisManager->FillNtupleDColumn(1,4, momentumZ);
     }
     analysisManager -> AddNtupleRow(1);
}
//analysisManager->FillNtupleIColumn(4, mc_totnumparticle);


// Fill ntuple

// Silicon Hits
//analysisManager->FillNtupleIColumn(0, hHCSilicon->entries());

// Silicon Energy

  G4cout<< "fSensDetector ="<< fSensDetector<<"\n";
  G4cout<< "Nsteps ="<< fSensDetector->GetNsteps()<"\n";

G4double totalSiE = 0.;
for (auto i=0;i<nhit;i++) {
 auto hit = (*hHCSilicon)[i];
 auto edep = hit->GetEdep();
 if (edep>0.) {
    //  G4cout<<"hit  "<<i <<" deposited "<< edep/keV<<" keV \n";
   totalSiE += edep/keV;
 }
}
analysisManager->FillNtupleDColumn(2,0, totalSiE);
analysisManager->FillNtupleDColumn(2,1, LPIPS);
analysisManager->FillNtupleDColumn(2,2, stepsInPIPS);
 analysisManager -> AddNtupleRow(2);


// ---- Absorber
 auto hHCAbsorber = static_cast<BetaChamberAbsorberHitsCollection*>(hce->GetHC(fAbsSensID));
 auto nhitAbs  = hHCAbsorber->entries();

G4double totalAbsorberE = 0.;
for (auto i=0;i<nhitAbs;i++) {
 auto hit = (*hHCAbsorber)[i];
 auto edep = hit->GetEdep();
 if (edep>0.) {
   totalAbsorberE += edep/keV;
 }
}
analysisManager->FillNtupleDColumn(3,0, totalAbsorberE);
analysisManager -> AddNtupleRow(3);


// --- test
 // G4cout<<"Energiedeposited: " << fEdepSi/keV <<" keV \n";

 NewEvent=0;
 G4cout <<"status event = "<<NewEvent<<"\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberEventAction::trackVolume(){ //const G4String volName


    // if(volName=="SiliconChipLOG")stepsInPIPS++;
    stepsInPIPS++;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberEventAction::EventStatus(G4int status){ //const G4String volName

    NewEvent= status;
    G4cout << "Inside EventStatus: "<<status<<"\n";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

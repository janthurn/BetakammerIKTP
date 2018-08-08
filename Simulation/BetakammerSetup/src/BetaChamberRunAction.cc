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
// $Id: BetaChamberRunAction.cc 99560 2016-09-27 07:03:29Z gcosmo $
//
/// \file BetaChamberRunAction.cc
/// \brief Implementation of the BetaChamberRunAction class

#include "BetaChamberRunAction.hh"
#include "BetaChamberPrimaryGeneratorAction.hh"
#include "BetaChamberDetectorConstruction.hh"
#include "BetaChamberRunActionMessenger.hh"
#include "BetaChamberEventAction.hh"
#include "BetaChamberAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
// #include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberRunAction::BetaChamberRunAction(BetaChamberEventAction * eventAction) : G4UserRunAction(), fEventAction(eventAction),
  fEdep(0.),
  fEdep2(0.),
  fpLengthPIPS(0.)
{
    fMessenger = new BetaChamberRunActionMessenger(this);
    fHistoName[0]="BetaChamberOut";

    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output
    analysisManager->SetVerboseLevel(1);
    //analysisManager->SetFileName("BetaChamberOut");

    // Creating 1D histograms
    analysisManager->CreateH1("hitSiliconChip","Silicon chip # Hits", 250, 0., 25); // h1 Id = 0
    // Creating 2D histograms
    analysisManager->CreateH2("silicon XY","silicon chip X vs Y",           // h2 Id = 0
              1000, -50., 50, 120, -30., 30.);

     if ( fEventAction ) {

         // bisher
         /*
              analysisManager->CreateNtuple("siliconOutput", "Hits");
              analysisManager->CreateNtupleIColumn("SiliconHits");  // column Id = 0
              analysisManager->CreateNtupleDColumn("SiliconEnergy"); // column Id = 1
              analysisManager->CreateNtupleDColumn("Time1");    // column Id = 2
              analysisManager->CreateNtupleDColumn("MC_Ekin");    // column Id = 3
              analysisManager->CreateNtupleIColumn("mc_tonnum"); // column Id = 4
              analysisManager->CreateNtupleDColumn("mc_px"); // column Id = 5
              analysisManager->CreateNtupleDColumn("mc_py"); // column Id = 6
              analysisManager->CreateNtupleDColumn("mc_pz"); // column Id = 7
              analysisManager->CreateNtupleDColumn("AbsorberEnergy"); // column Id =8
              analysisManager->CreateNtupleIColumn("AbsorberHits"); // column Id = 9
              analysisManager->CreateNtupleDColumn("EAbsorber/ESilicon"); // column Id = 9
              analysisManager->FinishNtuple();
              */
            analysisManager->SetFirstNtupleId(1);
            analysisManager->CreateNtuple("MC_Input", "MC Parameter");
            analysisManager->CreateNtupleDColumn("MC_Ekin");    // column Id = 0
            analysisManager->CreateNtupleDColumn("MC_Momentum");    // column Id = 1
            analysisManager->CreateNtupleDColumn("mc_px"); // column Id = 2
            analysisManager->CreateNtupleDColumn("mc_py"); // column Id = 3
            analysisManager->CreateNtupleDColumn("mc_pz"); // column Id = 4
            analysisManager->CreateNtupleDColumn("mc_xpos"); // column Id = 5
            analysisManager->CreateNtupleDColumn("mc_ypos"); // column Id = 6
            analysisManager->CreateNtupleDColumn("mc_zpos"); // column Id = 7
            analysisManager->FinishNtuple();
            analysisManager->CreateNtuple("siliconOutput", "Hits");
            analysisManager->CreateNtupleDColumn("SiliconEnergy"); // column Id = 8
            analysisManager->CreateNtupleDColumn("SiliconStepLength"); // column Id = 8
            analysisManager->CreateNtupleDColumn("SiliconSteps"); // column Id = 8
            analysisManager->CreateNtuple("AbsorberOutput", "Hits");
            analysisManager->CreateNtupleDColumn("AbsorberEnergy"); // column Id = 9
            analysisManager->FinishNtuple();

    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberRunAction::~BetaChamberRunAction()
{
     delete G4AnalysisManager::Instance();
     delete fMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberRunAction::BeginOfRunAction(const G4Run*)
{
  // // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  //
  // // reset accumulables to their initial values
  // G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  // accumulableManager->Reset();
  fpLengthPIPS=0.;
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  // The default file name is set in B5RunAction::B5RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile(fHistoName[0]);

  G4String extension = analysisManager->GetFileType();
  fHistoName[1]= fHistoName[0];
  G4cout <<"Opened "<<fHistoName[1] << " to save data";
}

void BetaChamberRunAction::DefineSchema(){

    // TTree*nt=new TTree("siliconOutput","BetaChamberOutput tree");
    // nt->Branch("vertex_totnum",&vertex_totnum,"vertex_totnum/I");
    // nt->Branch("mc_totnumparticle",&mc_totnumparticle,"mc_totnumparticle/I");
    // nt->Branch("mc_ekin",&mc_ekin,"mc_ekin[mc_totnumparticle]/F");

}

void BetaChamberRunAction::FillPerEvent(G4double lPIPS){

    fpLengthPIPS+=lPIPS;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void BetaChamberRunAction::EndOfRunAction(const G4Run* run)
// {
//   G4int nofEvents = run->GetNumberOfEvent();
//   if (nofEvents == 0) return;
//
//   // Merge accumulables
//   G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
//   accumulableManager->Merge();
//
//   // Compute dose = total energy deposit in a run and its variance
//   //
//   G4double edep  = fEdep.GetValue();
//   G4double edep2 = fEdep2.GetValue();
//
//   G4double rms = edep2 - edep*edep/nofEvents;
//   if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;
//
//   const BetaChamberDetectorConstruction* detectorConstruction = static_cast<const BetaChamberDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
//   G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
//   G4double dose = edep/mass;
//   G4double rmsDose = rms/mass;
//
//   // Run conditions
//   //  note: There is no primary generator action object for "master"
//   //        run manager for multi-threaded mode.
//   const BetaChamberPrimaryGeneratorAction* generatorAction = static_cast<const BetaChamberPrimaryGeneratorAction*> (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
//   G4String runCondition;
//   if (generatorAction)
//   {
//     const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
//     runCondition += particleGun->GetParticleDefinition()->GetParticleName();
//     runCondition += " of ";
//     G4double particleEnergy = particleGun->GetParticleEnergy();
//     runCondition += G4BestUnit(particleEnergy,"Energy");
//   }
//
//   // Print
//   //
//   if (IsMaster()) {
//     G4cout
//      << G4endl
//      << "--------------------End of Global Run-----------------------";
//   }
//   else {
//     G4cout
//      << G4endl
//      << "--------------------End of Local Run------------------------";
//   }
//
//   G4cout
//      << G4endl
//      << " The run consists of " << nofEvents << " "<< runCondition
//      << G4endl
//      << " Cumulated dose per run, in scoring volume : "
//      << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
//      << G4endl
//      << "------------------------------------------------------------"
//      << G4endl
//      << G4endl;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberRunAction::EndOfRunAction(const G4Run* ){


    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberRunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberRunAction::SetOutputFileName(G4String& val){ fHistoName[0]=val;}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

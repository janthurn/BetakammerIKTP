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
// $Id: BetaChamberSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file BetaChamberSteppingAction.cc
/// \brief Implementation of the BetaChamberSteppingAction class

#include "BetaChamberSteppingAction.hh"
#include "BetaChamberEventAction.hh"
#include "BetaChamberDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberSteppingAction::BetaChamberSteppingAction(BetaChamberEventAction* eventAction,BetaChamberDetectorConstruction* det)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fDetector(det),
  fScoringVolume(0),
  fEdepSi(0),
  fNSteps(0)
{

    fNSteps =0;
    fEdepSi=0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberSteppingAction::~BetaChamberSteppingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const BetaChamberDetectorConstruction* detectorConstruction
      = static_cast<const BetaChamberDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }

  G4LogicalVolume* volumePost = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4LogicalVolume* volumePre = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();



      const G4double stepl=step->GetStepLength()/mm;
    //   if(NamePost==NameVolume && NamePre==NameVolume){
          fEventAction->AddTrackPIPS(stepl);
        //   fEventAction->trackVolume(); //volumePost->GetName()




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

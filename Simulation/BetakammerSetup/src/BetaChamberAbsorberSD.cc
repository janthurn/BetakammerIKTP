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
// $Id: BetaChamberSiliconSD.cc 101036 2016-11-04 09:00:23Z gcosmo $
//
/// \file BetaChamberSiliconSD.cc
/// \brief Implementation of the BetaChamberSiliconSD class

#include "BetaChamberAbsorberSD.hh"
#include "BetaChamberAbsorberHit.hh"


#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberAbsorberSD::BetaChamberAbsorberSD(G4String name) : G4VSensitiveDetector(name),
                                                fHitsCollection(nullptr), fAbsID(-1)
                                                //Init_state(0)
{
  collectionName.insert("AbsorberSensColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberAbsorberSD::~BetaChamberAbsorberSD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BetaChamberAbsorberSD::Initialize(G4HCofThisEvent* hce)
{
  // aus Beispiel B5
  fHitsCollection = new BetaChamberAbsorberHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fAbsID<0) {
    fAbsID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fAbsID,fHitsCollection);
   fHitsCollection->insert(new BetaChamberAbsorberHit());
  // // fill calorimeter hits with zero energy deposition
  // for (auto i=0;i<kNofEmCells;i++) {
  //   fHitsCollection->insert(new BetaChamberSiliconHit(i));
  // }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool BetaChamberAbsorberSD::ProcessHits(G4Step*step, G4TouchableHistory*)
{

  auto edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();

  auto hit = (*fHitsCollection)[0];
  // check if it is first touch
  if (!(hit->GetLogV())) {
    // fill volume information
    hit->SetLogV(physical->GetLogicalVolume());
    G4AffineTransform transform = touchable->GetHistory()->GetTopTransform();
    transform.Invert();
    hit->SetRot(transform.NetRotation());
    hit->SetPos(transform.NetTranslation());
  }
  // add energy deposition
  hit->AddEdep(edep);
 // G4cout <<"ProcessHits"<<"\n";
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

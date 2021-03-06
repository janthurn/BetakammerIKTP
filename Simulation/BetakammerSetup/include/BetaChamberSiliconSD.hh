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
// $Id: B5EmCalorimeterSD.hh 101036 2016-11-04 09:00:23Z gcosmo $
//
/// \file B5EmCalorimeterSD.hh
/// \brief Definition of the B5EmCalorimeterSD class

#ifndef BetaChamberSiliconSD_h
#define BetaChamberSiliconSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include "BetaChamberSiliconHit.hh"
#include "G4Event.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class BetaChamberEventAction;

/// EM calorimeter sensitive detector

class BetaChamberSiliconSD : public G4VSensitiveDetector
{
  public:
    BetaChamberSiliconSD(G4String name); //, BetaChamberEventAction* eventAction
    virtual ~BetaChamberSiliconSD();

    virtual void Initialize(G4HCofThisEvent* HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory* ROhist);
    G4int GetNsteps(){return fNSteps;};


  private:
     BetaChamberSiliconHitsCollection* fHitsCollection;
    //  BetaChamberEventAction* fEvtaction;

     G4int fSiID;
     G4int fNSteps;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

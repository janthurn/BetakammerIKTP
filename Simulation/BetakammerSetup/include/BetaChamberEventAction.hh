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
// $Id: BetaChamberentAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file BetaChamberEventAction.hh
/// \brief Definition of the 2umEventAction class

#ifndef BetaChamberEventAction_h
#define BetaChamberEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "BetaChamberSteppingAction.hh"
#include "G4VSensitiveDetector.hh"


#include <vector>
class BetaChamberRunAction;
// class BetaChamberSiliconSD;

/// Event action class
///

class BetaChamberEventAction : public G4UserEventAction
{
  public:
    // BetaChamberEventAction(BetaChamberSteppingAction* runAction);
    BetaChamberEventAction();
    virtual ~BetaChamberEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdepSi += edep; }
    // void AddTrack(G4double lp,G4double la){LPIPS+=lp; LAbsorber+=la;};
    void AddTrackPIPS(G4double lp){LPIPS+=lp;};
    void trackVolume(); //const G4String
    G4int GetEventStatus(){return NewEvent;};
    void EventStatus(G4int);

  private:
    BetaChamberSteppingAction*  fSteppingAction;
    G4VSensitiveDetector*  fSensDetector;
    G4double     fEdepSi;
    G4int fSiSensID;
    G4int fAbsSensID;
    G4double LPIPS;//, LAbsorber;
    G4int stepsInPIPS;

    G4int NewEvent;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

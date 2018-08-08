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
// $Id: BetaChamberPrimaryGeneratorAction.hh 90623 2015-06-05 09:24:30Z gcosmo $
//
/// \file BetaChamberPrimaryGeneratorAction.hh
/// \brief Definition of the BetaChamberPrimaryGeneratorAction class

#ifndef BetaChamberPrimaryGeneratorAction_h
#define BetaChamberPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "BetaChamberDetectorConstruction.hh"
#include "BetaChamberPositionSampler.hh"
#include "BetaChamberVGenerator.hh"
#include "BetaChamberPrimaryGeneratorMessenger.hh"

#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Tubs;
class G4GenericMessenger;
/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued
/// in front of the phantom across 80% of the (X,Y) phantom size.

class BetaChamberPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    enum EConfinementCode { noconfined, volume };

    BetaChamberPrimaryGeneratorAction();
    virtual ~BetaChamberPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    // ------------------
    void SetBCGenerator(BetaChamberVGenerator* gen){fBCGenerator=gen;}
    BetaChamberVGenerator *GetBCGenerator() { return fBCGenerator; }

    void SetVolumeName(G4String name) {fVolumeName = name;}
    G4String GetVolumeName() {return fVolumeName;}

    void SetParticlePosition(G4ThreeVector vec) {fPosition = vec;}
    G4ThreeVector GetParticlePosition() {return fPosition;}

    void SetConfinementCode(EConfinementCode code);
    EConfinementCode GetConfinementCode() {return fConfinementCode;}

    // ------------------

  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4ParticleDefinition* fElektron;

    BetaChamberDetectorConstruction * BetaChamber;
    BetaChamberPrimaryGeneratorMessenger* fMessenger;
    BetaChamberVGenerator * fBCGenerator;
    BetaChamberPositionSampler * fPositionSampler;


    G4String fVolumeName;
    G4ThreeVector fPosition;
    EConfinementCode fConfinementCode;

    G4double sampleZshift;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

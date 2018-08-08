//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//

/**
 * AUTHOR: Nuno Barros
 * FIRST SUBMISSION: 01-02-2013
 *
 * REVISION:
 *
 *
 */

// ---------------------------------------------------------------------------

#ifndef _BetaChamberDetectorConstructionMessenger_HH
#define _BetaChamberDetectorConstructionMessenger_HH

// ---------------------------------------------------------------------------

#include "globals.hh"
#include "G4UImessenger.hh"

// ---------------------------------------------------------------------------

class BetaChamberDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;

class BetaChamberDetectorConstructionMessenger : public G4UImessenger
{

public:

  //default constructor

  BetaChamberDetectorConstructionMessenger(BetaChamberDetectorConstruction*);

  // destructor

  ~BetaChamberDetectorConstructionMessenger();

  // methods

   void SetNewValue(G4UIcommand *cmd, G4String newvalue);

private:

  BetaChamberDetectorConstruction* fDetector;

  G4UIdirectory* fDirectory;


  G4UIcmdWithAnInteger* fDetektorNummer;
  G4UIcmdWithAnInteger* fSondenNummer;
  G4UIcmdWithAnInteger* fSetupNummer;
  G4UIcmdWithAnInteger* fSondenPositionNummer;
  G4UIcmdWithAnInteger* fKalibrationsquelleConfig;
  G4UIcmdWithAnInteger* fKalibrationsquelleMaterial;
  G4UIcmdWithADouble*   fKalibrationsquelleDurchmesser;
  G4UIcmdWithADouble*   fSiliziumWaferDicke;
  G4UIcmdWithADouble*   fBismutDicke;
  G4UIcmdWithAnInteger* fAllowedTransitionMaterial;
  G4UIcmdWithABool*     fBoolstepLimit;
  G4UIcmdWithABool*     fUseSiBacking;
  G4UIcmdWithAnInteger* fATSampleShape;

};

// ---------------------------------------------------------------------------

#endif

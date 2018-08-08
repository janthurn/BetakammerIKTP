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

#include <G4UIdirectory.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>

#include "BetaChamberDetectorConstructionMessenger.hh"
#include "BetaChamberDetectorConstruction.hh"


// ---------------------------------------------------------------------------

BetaChamberDetectorConstructionMessenger::BetaChamberDetectorConstructionMessenger(BetaChamberDetectorConstruction *teststand) :
  fDetector(teststand),
  fDirectory(0), fDetektorNummer(0),fSondenNummer(0), fSetupNummer(0),fSondenPositionNummer(0),fKalibrationsquelleConfig(0),
  fKalibrationsquelleMaterial(0),fKalibrationsquelleDurchmesser(0), fSiliziumWaferDicke(0),fBismutDicke(0),fAllowedTransitionMaterial(0),fBoolstepLimit(0),
  fATSampleShape(0)
{
  // directories
  fDirectory =  new G4UIdirectory("/BetaChamber/geometry/");
  fDirectory->SetGuidance("Tune parameters for BetaChamber");

  /*
  fDetektorNummer = new G4UIcmdWithAnInteger("/BetaChamber/geometry/DetectorNumber",this);
  fDetektorNummer->SetGuidance("defines the detector number.");
  fDetektorNummer->AvailableForStates(G4State_PreInit);
   */
  fSondenNummer = new G4UIcmdWithAnInteger("/BetaChamber/geometry/SampleNumber",this);
  fSondenNummer->SetGuidance("defines the number of the Bismut samples.");
  fSondenNummer->AvailableForStates(G4State_PreInit);

  fSetupNummer = new G4UIcmdWithAnInteger("/BetaChamber/geometry/SetupNumber",this);
  fSetupNummer->SetGuidance("defines the number of the simulation setup.");
  fSetupNummer->AvailableForStates(G4State_PreInit);

  fSondenPositionNummer = new G4UIcmdWithAnInteger("/BetaChamber/geometry/SondenPositionNummer",this);
  fSondenPositionNummer->SetGuidance("defines the position of the postion of the sample.");
  fSondenPositionNummer->AvailableForStates(G4State_PreInit);


  fKalibrationsquelleConfig = new G4UIcmdWithAnInteger("/BetaChamber/geometry/KalibrationConfig",this);
  fKalibrationsquelleConfig->SetGuidance("defines the config of the calibration sample.");
  fKalibrationsquelleConfig->AvailableForStates(G4State_PreInit);

  fKalibrationsquelleMaterial = new G4UIcmdWithAnInteger("/BetaChamber/geometry/KalibrationMaterial",this);
  fKalibrationsquelleMaterial->SetGuidance("defines the config of the calibration material.");
  fKalibrationsquelleMaterial->AvailableForStates(G4State_PreInit);


  fKalibrationsquelleDurchmesser = new G4UIcmdWithADouble("/BetaChamber/geometry/KalibrationQuellpunktDurchmesser",this);
  fKalibrationsquelleDurchmesser->SetGuidance("defines the faktor in faktor*OrbDiameter (base 1e-3*mm).");
  fKalibrationsquelleDurchmesser->AvailableForStates(G4State_PreInit);

  fSiliziumWaferDicke= new G4UIcmdWithADouble("/BetaChamber/geometry/SiliziumDicke",this);
  fSiliziumWaferDicke->SetGuidance("sets SiliziumDicke*base  (base 1e-3*mm).");
  fSiliziumWaferDicke->AvailableForStates(G4State_PreInit);

  fBismutDicke= new G4UIcmdWithADouble("/BetaChamber/geometry/BismutSchichtDicke",this);
  fBismutDicke->SetGuidance("sets Bismutschichtdicke*base  (base 1e-3*mm).");
  fBismutDicke->AvailableForStates(G4State_PreInit);

  fAllowedTransitionMaterial = new G4UIcmdWithAnInteger("/BetaChamber/geometry/AllowedTransitionMaterial",this);
  fAllowedTransitionMaterial->SetGuidance("defines the material used for allowed transition.");
  fAllowedTransitionMaterial->AvailableForStates(G4State_PreInit);

  fUseSiBacking = new G4UIcmdWithABool("/BetaChamber/geometry/SwitchOnSiBacking",this);
  fUseSiBacking->SetGuidance("defines the material used for allowed transition.");
  fUseSiBacking->AvailableForStates(G4State_PreInit);
/*
  fBoolstepLimit = new G4UIcmdWithABool("/BetaChamber/geometry/SwitchOnStepLimit",this);
  fBoolstepLimit->SetGuidance("set userLimit to 0.1 siliziumdicke.");
  fBoolstepLimit->AvailableForStates(G4State_PreInit);
   */
   fATSampleShape = new G4UIcmdWithAnInteger("/BetaChamber/geometry/ChooseATSampleShape",this);
   fATSampleShape->SetGuidance("defines the sample shape: 1 = tube, 2 = quadratic.");
   fATSampleShape->AvailableForStates(G4State_PreInit);
}


// ---------------------------------------------------------------------------

BetaChamberDetectorConstructionMessenger::~BetaChamberDetectorConstructionMessenger()
{
 // delete fDirectory;
//  delete fDetektorNummer;
  delete fSondenNummer;
  delete fSondenPositionNummer;
  delete fSetupNummer;
  delete fKalibrationsquelleConfig;
  delete fKalibrationsquelleMaterial;
  delete fKalibrationsquelleDurchmesser;
  delete fSiliziumWaferDicke;
  delete fBismutDicke;
  delete fAllowedTransitionMaterial;
  delete fUseSiBacking;
  delete fATSampleShape;
}

// ---------------------------------------------------------------------------

void BetaChamberDetectorConstructionMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{
 /*
 if (cmd == fDetektorNummer){
	  fDetector->SetDetektorNumber(fDetektorNummer->GetNewIntValue(newvalue));
  }*/
   if (cmd == fSondenNummer){
	  fDetector->SetSondenNumber(fSondenNummer->GetNewIntValue(newvalue));
  }
  else if (cmd == fSetupNummer){
	  fDetector->SetSetupNumber(fSetupNummer->GetNewIntValue(newvalue));
  }
  else if (cmd == fSondenPositionNummer){
	  fDetector->SetPositionsNumber(fSondenPositionNummer->GetNewIntValue(newvalue));
  }

  else if (cmd == fKalibrationsquelleConfig){
	  fDetector->SetKalibrationsSetup(fKalibrationsquelleConfig->GetNewIntValue(newvalue));
  }
  else if (cmd == fKalibrationsquelleMaterial){
	  fDetector->SetKalibrationsMaterial(fKalibrationsquelleMaterial->GetNewIntValue(newvalue));
  }
  else if (cmd == fKalibrationsquelleDurchmesser){
	  fDetector->SetKalibrationsDiameter(fKalibrationsquelleDurchmesser->GetNewDoubleValue(newvalue));
  }
  else if (cmd == fSiliziumWaferDicke){
    fDetector->SetSiliconSampleThickness(fSiliziumWaferDicke->GetNewDoubleValue(newvalue));
  }
  else if (cmd == fBismutDicke){
    fDetector->SetBismutSampleThickness(fBismutDicke->GetNewDoubleValue(newvalue));
  }
  else if (cmd == fAllowedTransitionMaterial){
     fDetector->SetAllowedTransition(fAllowedTransitionMaterial->GetNewIntValue(newvalue));
  }

  else if (cmd == fUseSiBacking){
     fDetector->SetSiliconBacking(fUseSiBacking->GetNewBoolValue(newvalue));
  }
  else if (cmd == fATSampleShape){
    fDetector->SetATSampleShape(fATSampleShape->GetNewIntValue(newvalue));
  }

  /*
 else if (cmd == fBoolstepLimit){
   fDetector->SetBoolStepLimit(fBoolstepLimit->GetNewBoolValue(newvalue));
 }
 */
}

// ---------------------------------------------------------------------------

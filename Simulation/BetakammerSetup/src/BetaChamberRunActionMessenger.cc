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

#include "BetaChamberRunActionMessenger.hh"
#include "BetaChamberRunAction.hh"


// ---------------------------------------------------------------------------

BetaChamberRunActionMessenger::BetaChamberRunActionMessenger(BetaChamberRunAction *run) :
 fRun(run)
{
  // directories
  fDirectory =  new G4UIdirectory("/BetaChamber/RunAction/");
  fDirectory->SetGuidance("run control");

  fHFileCmd = new G4UIcmdWithAString("/BetaChamber/RunAction/SetFileName",this);
  fHFileCmd->SetGuidance("Name of the output file");

/*
  fSondenNummer = new G4UIcmdWithAnInteger("/BetaChamber/RunAction/SampleNumber",this);
  fSondenNummer->SetGuidance("defines the number of the Bismut samples.");
  fSondenNummer->AvailableForStates(G4State_PreInit);



  fKalibrationsquelleDurchmesser = new G4UIcmdWithADouble("/BetaChamber/RunAction/KalibrationQuellpunktDurchmesser",this);
  fKalibrationsquelleDurchmesser->SetGuidance("defines the faktor in faktor*OrbDiameter (base 1e-3*mm).");
  fKalibrationsquelleDurchmesser->AvailableForStates(G4State_PreInit);

  fBoolstepLimit = new G4UIcmdWithABool("/BetaChamber/geometry/SwitchOnStepLimit",this);
  fBoolstepLimit->SetGuidance("set userLimit to 0.1 siliziumdicke.");
  fBoolstepLimit->AvailableForStates(G4State_PreInit);
   */

}


// ---------------------------------------------------------------------------

BetaChamberRunActionMessenger::~BetaChamberRunActionMessenger()
{
 delete fDirectory;
 delete fHFileCmd;
}

// ---------------------------------------------------------------------------

void BetaChamberRunActionMessenger::SetNewValue(G4UIcommand *cmd, G4String newvalue)
{

   if (cmd == fHFileCmd){
	  fRun->SetOutputFileName(newvalue);
  }
  // else if (cmd == fSetupNummer){
  //  fDetector->SetSetupNumber(fSetupNummer->GetNewIntValue(newvalue));
  // }
}

// ---------------------------------------------------------------------------

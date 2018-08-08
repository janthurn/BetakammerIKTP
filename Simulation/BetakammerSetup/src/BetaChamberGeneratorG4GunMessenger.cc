//---------------------------------------------------------------------------//
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
// History:
// --------
// 13 Sep 2005  X. Liu       First implementation
// 25 Jan 2006 L. Pandola   Added commands
// 11 Apr 2006 L. Pandola   Cone-sampling corrected
// 11 Nov 2006 L. Pandola   Added commands for "centric-sampling" with a cone
// 29 May 2008 L. Pandola   Added commands to sample energy from a file spectrum

//Geant4 includes
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4ParticleGun.hh"

//MaGe includes
#include "BetaChamberGeneratorG4Gun.hh"
#include "BetaChamberGeneratorG4GunMessenger.hh"


BetaChamberGeneratorG4GunMessenger::BetaChamberGeneratorG4GunMessenger(BetaChamberGeneratorG4Gun* ptr) :
  fPointerToMGG4Gun(ptr)
{
  fDirectory = new G4UIdirectory("/BetaChamber/generator/G4gun/");
  fDirectory->SetGuidance("Control of MG G4Gun event generator");

  // ---------------------------------------------------------------------------//
  fReadFromFileCmd = new G4UIcmdWithABool("/BetaChamber/generator/G4gun/spectrum_from_file",this);
  fReadFromFileCmd->SetDefaultValue(false);
  fReadFromFileCmd->SetGuidance("Turn on/off the sampling of energy from a histogram read from file");
  fReadFromFileCmd->SetGuidance("default is false (= fixed energy)");
  fReadFromFileCmd->SetGuidance("File to be read can be set with /BetaChamber/generator/g4gun/spectrum_filename");

  fSetFileNameCmd = new G4UIcmdWithAString("/BetaChamber/generator/G4gun/spectrum_filename",this);
  fSetFileNameCmd->SetGuidance("Set file name for sampling energy spectrum from a file-based histogram");
}

//---------------------------------------------------------------------------//

BetaChamberGeneratorG4GunMessenger::BetaChamberGeneratorG4GunMessenger(const BetaChamberGeneratorG4GunMessenger & other) : G4UImessenger(other)
{;}

BetaChamberGeneratorG4GunMessenger::~BetaChamberGeneratorG4GunMessenger()
{
  delete fDirectory;
  delete fReadFromFileCmd;
  delete fSetFileNameCmd;
}

//---------------------------------------------------------------------------//

void BetaChamberGeneratorG4GunMessenger::SetNewValue(G4UIcommand* cmd,G4String str)
{
  if (cmd == fReadFromFileCmd)    {
      fPointerToMGG4Gun->SetReadSpectrumFromFile(fReadFromFileCmd->GetNewBoolValue(str));
  }
  else if (cmd == fSetFileNameCmd)   {
      fPointerToMGG4Gun->SetFileNameFromFile(str);
  }
}

//---------------------------------------------------------------------------//

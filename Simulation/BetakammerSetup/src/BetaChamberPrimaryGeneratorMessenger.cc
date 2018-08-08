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

#include "G4PhysicalVolumeStore.hh"
#include "BetaChamberGeneratorDecay0.hh"
#include "BetaChamberGeneratorG4Gun.hh"


#include "BetaChamberPrimaryGeneratorMessenger.hh"
#include "BetaChamberPrimaryGeneratorAction.hh"


//================================================================================
// -------------------------------------------------------------------------------
//================================================================================
BetaChamberPrimaryGeneratorMessenger::BetaChamberPrimaryGeneratorMessenger(BetaChamberPrimaryGeneratorAction* generator) :
  fGenerator(generator)
{
  // directories
  fGeneratorDirectory =  new G4UIdirectory("/BetaChamber/generator/");
  fGeneratorDirectory->SetGuidance("Control commands for generators");

  fSelectCmd = new G4UIcmdWithAString("/BetaChamber/generator/select",this);
  fSelectCmd->SetGuidance("Select generator for primary events");
  fSelectCmd->SetGuidance("Options: ");
  fSelectCmd->SetGuidance("gun: G4gun and random sampling");
  fSelectCmd->SetGuidance("decay0: ");
  G4String candidates = "gun decay0";
  //candidates += "SPS neutronsGS wangneutrons AmBe sources4a GSS Pb210";
  fSelectCmd->SetCandidates(candidates);

  fVolumeCmd = new G4UIcmdWithAString("/BetaChamber/generator/volume", this);
  fVolumeCmd->SetGuidance("Selects the volume where the source is confined");
  fVolumeCmd->AvailableForStates(G4State_Init,G4State_Idle);


  fConfineCmd = new G4UIcmdWithAString("/BetaChamber/generator/confine", this);
  fConfineCmd->SetGuidance("Selects confinement for the source.");
  fConfineCmd->SetGuidance("Options are:");
  fConfineCmd->SetGuidance("noconfined : source not confined");
  fConfineCmd->SetGuidance("volume : source confined in a (physical) volume.");
  candidates = "noconfined volume";
  fConfineCmd->SetCandidates(candidates);



}


//================================================================================
// -------------------------------------------------------------------------------
//================================================================================

BetaChamberPrimaryGeneratorMessenger::~BetaChamberPrimaryGeneratorMessenger()
{
 delete fGeneratorDirectory;
 delete fSelectCmd;
 delete fConfineCmd;
 delete fVolumeCmd;
}

//================================================================================
// -------------------------------------------------------------------------------
//================================================================================
void BetaChamberPrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newvalue)
{

   if (command == fSelectCmd){
       if(newvalue=="gun"){
           fGenerator->SetBCGenerator(new BetaChamberGeneratorG4Gun);
       }
       else if(newvalue=="decay0"){
           fGenerator->SetBCGenerator(new BetaChamberGeneratorDecay0);
       }
   }
   // ------------------------------------------------------------------------
   else if (command == fVolumeCmd) {
     G4cout << " choose volume command "<<"\n";
     fGenerator->SetVolumeName(newvalue);

     G4bool ifound = false;
     G4PhysicalVolumeStore* volumeStore = G4PhysicalVolumeStore::GetInstance();
     G4int nVolumes = (G4int) volumeStore->size();
     G4String candidateList;
     for(G4int i=0;i<nVolumes;i++) {
       candidateList += (*volumeStore)[i]->GetName();
       candidateList += ", ";
       if ((*volumeStore)[i]->GetName() == newvalue) ifound = true;
     }

     if (ifound) {
       if (fGenerator->GetConfinementCode() != BetaChamberPrimaryGeneratorAction::noconfined) {
         fGenerator->SetVolumeName(newvalue);
         G4cout << "Source confined in " << newvalue << "\n";
       }
       else G4cout << "Source not confined: nothing happens " << "\n";
     }
     else {
       G4cout << "Volume not found " << "\n";
       G4cout << "The list of volumes is: " << candidateList << "\n";
     }
   }
   // ---------------------------------------------------------------------------

   else if(command == fConfineCmd) {
     if(newvalue == "noconfined") {
       fGenerator->SetConfinementCode(BetaChamberPrimaryGeneratorAction::noconfined);
       G4cout << "Source not confined" << "\n";
     }
     else if(newvalue == "volume") {
       fGenerator->SetConfinementCode(BetaChamberPrimaryGeneratorAction::volume);
       G4cout << "Source confined in volume" << "\n";
     }
   }

}

//================================================================================
// -------------------------------------------------------------------------------
//================================================================================

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
// $Id: BetaChamberPrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file BetaChamberPrimaryGeneratorAction.cc
/// \brief Implementation of the BetaChamberPrimaryGeneratorAction class

#include "BetaChamberPrimaryGeneratorAction.hh"
#include "BetaChamberPrimaryGeneratorMessenger.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4GenericMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BetaChamberPrimaryGeneratorAction::BetaChamberPrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),fElektron(0),fBCGenerator(0),fConfinementCode(noconfined)
{

  G4cout <<"Generator of PrimaryAction"<<"\n";

  fVolumeName = "NULL";
  G4ThreeVector zero(0.,0.,0.);
  fPosition = zero; //this is the default position

  BetaChamber= new BetaChamberDetectorConstruction();
  fMessenger = new BetaChamberPrimaryGeneratorMessenger(this);
  sampleZshift = BetaChamber->GetSampleShift();
  fPositionSampler = new BetaChamberPositionSampler();

}

// ===================================================================================================
// ===================================================================================================
// ===================================================================================================

BetaChamberPrimaryGeneratorAction::~BetaChamberPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
  delete fBCGenerator;
}

// ===================================================================================================
// ===================================================================================================
// ===================================================================================================

void BetaChamberPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

    if(fBCGenerator){
        G4int copyN = 0;
        if(fConfinementCode==volume){
            fPosition = fPositionSampler->SampleUniformlyInVolume(GetVolumeName(),copyN);
            // G4cout<< "("<< fPosition.getX()<<" , "<<fPosition.getY() <<" ,  "<<fPosition.getZ() << " )"<<"\n";
            fBCGenerator->SetParticlePosition(fPosition);
        }

     fBCGenerator->GeneratePrimaryVertex(anEvent);
    }


}

// ===================================================================================================
// ===================================================================================================
// ===================================================================================================
void BetaChamberPrimaryGeneratorAction::SetConfinementCode(EConfinementCode code)
{
  fConfinementCode = code;

  if (fConfinementCode == noconfined)
    {
      //reset the starting position
      G4ThreeVector BismutSampleZposition(0.,0.,sampleZshift);
      fPosition = BismutSampleZposition;
      fBCGenerator-> SetParticlePosition(fPosition);
      G4cout << "Default position re-set to " << fPosition << "\n";
    }
}



// old
/*
void BetaChamberPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event

  G4double SampleRadius = 0;
  G4double SampleThickness = 0;
  G4double Bishift= BetaChamber->GetSampleShift();
  if (!fSample)
  {
    G4cout <<"Setting " << fNameSampleVol << " as sample volume";
    G4LogicalVolume* sampleLV = G4LogicalVolumeStore::GetInstance()->GetVolume(fNameSampleVol);
    if ( sampleLV ) fSample = dynamic_cast<G4Tubs*>(sampleLV->GetSolid());
  }

  if ( fSample ) {
    SampleRadius = fSample->GetOuterRadius();
    SampleThickness = fSample->GetZHalfLength()*2.;
  }
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("BetaChamberPrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }



  //--- Random position inside the sample
  G4cout << "--------------------------------------\n ";
  G4cout << "sample radius: "<< SampleRadius<< " mm \n";
  G4cout << "sample thickness: "<< SampleThickness<< " mm\n";
  G4cout << "sample shift: "<< Bishift<< " mm\n";
  G4double bitop= Bishift+0.5*SampleThickness;
  G4cout << "sample top: "<< bitop<< " mm\n";
   G4double bibottom= Bishift-0.5*SampleThickness;
  G4cout << "sample bottom: "<< bibottom<< " mm\n";

  G4double r0 = SampleRadius*G4UniformRand();
  G4cout << "Random radius: "<< r0<< " mm\n";
  G4double phi0= 2*pi*G4UniformRand();
  G4cout << "Random phi: "<< (phi0/(2*pi))*360<< " Grad ergibt: \n";
  G4double x0 =  r0*sin(phi0);
   G4cout << "x0: "<< x0<< " mm\n";
  G4double y0 = r0*cos(phi0);
  G4cout << "y0: "<< y0<< " mm\n";
  G4double z0 =  Bishift+SampleThickness*(G4UniformRand()-0.5);
  if(z0>=bibottom && z0<=bitop)G4cout << "z0 innerhalb sample \n";
  G4cout << "z0: "<< z0<< " mm\n";

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  G4cout << "SetParticlePosition "<< "\n";
  G4cout << "--------------------------------------\n ";

  G4ParticleDefinition* particle= fParticleGun->GetParticleDefinition();
  auto ekin = fParticleGun->GetParticleEnergy();
  auto mass = particle->GetPDGMass();
   G4cout << "Masse: "<< mass<< " MeV\n";
   G4cout << "kinetic energy: "<< ekin<< " MeV\n";
  //auto ekin = std::sqrt(pp*pp+mass*mass)-mass;
  auto pp = std::sqrt(pow((ekin+mass),2)-mass*mass);
   G4cout << "particle momentum: "<< pp<< " MeV\n";
 G4cout << "--------------------------------------\n ";
   G4double px=2*(G4UniformRand()-0.5);
   G4double py=(1-px)*2*(G4UniformRand()-0.5);
   G4double pz=(1-px-py);
   G4cout << "px: "<< px<< " \n";
   G4cout << "py: "<< py<< " \n";
   G4cout << "pz: "<< pz<< " \n";
   G4cout << "px+py+pz = "<<px+py+ pz<< " \n";

fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));





  fParticleGun->GeneratePrimaryVertex(anEvent);
  G4cout << "GeneratePrimaryVertex "<< "\n";
  G4cout << "--------------------------------------------\n ";
  G4cout << "--------------------------------------------\n ";
  G4cout << "--------------------------------------------\n ";
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

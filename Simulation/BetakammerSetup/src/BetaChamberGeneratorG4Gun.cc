

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "BetaChamberGeneratorG4Gun.hh"
#include "BetaChamberGeneratorReadSpectrumFile.hh"
#include "BetaChamberGeneratorG4GunMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

//================================================================================
//================================================================================
//================================================================================
BetaChamberGeneratorG4Gun::BetaChamberGeneratorG4Gun(): fEnergySpectrumTypeGenerator(0){

 fGeneratorName = "G4Gun";
 fTheMessenger = new BetaChamberGeneratorG4GunMessenger(this);
 fParticleGun = new G4ParticleGun();
 fEnergySpectrumFromFile = false;
}

//================================================================================
//================================================================================
//================================================================================

BetaChamberGeneratorG4Gun::BetaChamberGeneratorG4Gun(const BetaChamberGeneratorG4Gun & other) : BetaChamberVGenerator(other)
{;}

//================================================================================
//================================================================================
//================================================================================

BetaChamberGeneratorG4Gun::~BetaChamberGeneratorG4Gun(){
  delete fTheMessenger;
  delete fParticleGun;
  if (fEnergySpectrumTypeGenerator)
  delete fEnergySpectrumTypeGenerator;
}

//================================================================================
//================================================================================
//================================================================================

void BetaChamberGeneratorG4Gun::GeneratePrimaryVertex(G4Event *event){

 if (fEnergySpectrumFromFile){
    if (!fEnergySpectrumTypeGenerator){
   	  fEnergySpectrumTypeGenerator = new BetaChamberGeneratorReadSpectrumFile();
   	}
    if (!fEnergySpectrumTypeGenerator){ //still un-initialized = problem
   	  G4Exception("BetaChamberGeneratorG4Gun::GeneratePrimaryVertex()", "err001", FatalException, "Exit MaGe");
   	}
         G4double energy = fEnergySpectrumTypeGenerator->ShootEnergy();
         fParticleGun->SetParticleEnergy(energy);
 }

 // =============================================================================
    else {
     G4double cosTheta = -1.0 + 2.0*G4UniformRand();
     G4double phi = 2*pi * G4UniformRand();
     G4double sinTheta = sqrt(1 - cosTheta*cosTheta);
     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta));


    // G4double px= (G4UniformRand()-0.5);
    // G4double py= (G4UniformRand()-0.5);
    // G4double zdirection= (G4UniformRand()-0.5);
    ////// G4double pz= sqrt(1-pow(px,2)-pow(py,2));
    // if(zdirection>0) pz =pz;
    // else pz = pz*(-1);
    //  G4cout << "xdirection = "    <<px<<"\n";
    //  G4cout << "ydirection = "    <<py<<"\n";
    //  G4cout << "zdirection = "    << zdirection<<" -> " <<pz<<"\n";
    //  G4cout << "Betrag = "<< sqrt(px*px+py*py+pz*pz)<<"\n";

     //G4ThreeVector direction(px,py,pz);
     //fParticleGun->SetParticleMomentumDirection(direction);

     //G4cout << "After Setting: \n";
     //G4double xdirectionV2= fParticleGun->GetParticleMomentumDirection().x();
     //G4double ydirectionV2= fParticleGun->GetParticleMomentumDirection().y();
     //G4double zdirectionV2= fParticleGun->GetParticleMomentumDirection().z();

     //G4cout << "xdirection = "    <<xdirectionV2<<"\n";
     //G4cout << "ydirection = "    <<ydirectionV2<<"\n";
     //G4cout << "zdirection = "    <<zdirectionV2<<"\n";

     //G4cout << "Betrag = "<< sqrt(xdirectionV2*xdirectionV2+ydirectionV2*ydirectionV2+zdirectionV2*zdirectionV2)<<"\n";

    }

  fParticleGun->GeneratePrimaryVertex(event);
}

//================================================================================
//================================================================================
//================================================================================

void BetaChamberGeneratorG4Gun::SetFileNameFromFile(G4String filename)
{
  if (!fEnergySpectrumTypeGenerator)
    {
      fEnergySpectrumTypeGenerator = new BetaChamberGeneratorReadSpectrumFile();
    //   MGLog(debugging) << "Instantiate MGGeneratorReadSpectrumFile in SetFileNameFromFile()" << "\n";
    }
  if (fEnergySpectrumTypeGenerator)
    fEnergySpectrumTypeGenerator->ChangeFileName(filename);
  else
    {
      G4cout << "Not able to initialize BetaChamberGeneratorReadSpectrumFile" << "\n";
    }
  if (!fEnergySpectrumFromFile)
    {
      G4cout << "Please, set the energy spectrum from file flag" << "\n";
      G4cout << "to true, using the appropriate command: " << "\n";
      G4cout << "/BetaChamber/generator/g4gun/spectrum_from_file true " << "\n";
    }
}

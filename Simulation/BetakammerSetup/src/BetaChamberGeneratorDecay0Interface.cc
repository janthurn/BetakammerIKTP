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
// 25 Oct 2004   L.Pandola    First implementation
// 04 Nov 2004   L.Pandola    Change: not necessary to give immediately the file name
// 06 Dec 2004   L.Pandola    Renaming fest!
// 23 Dec 2005   L.Pandola    Further renaming to comply with MGGenerator
// 09 Jul 2010   L.Pandola    Added machinery to generate/kill selectively
//                            particles.
// 14 Mar 2012   L.Pandola    Re-start file if it is over and there are other events.
//

//Geant4 includes
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include <string.h>

//MaGe includes
#include "BetaChamberGeneratorDecay0Interface.hh"
#include "BetaChamberGeneratorDecay0InterfaceMessenger.hh"

BetaChamberGeneratorDecay0Interface::BetaChamberGeneratorDecay0Interface(G4String File) :
  fFileName(File)
{
  fTheMessenger = new BetaChamberGeneratorDecay0InterfaceMessenger(this);

  particle_position =  G4ThreeVector(0,0,0);
  particle_time = 0.0;
  fKillElectrons = false;
  fKillPositrons = false;
  fKillGammas = false;
  fRecycleFile = false;
}

BetaChamberGeneratorDecay0Interface::~BetaChamberGeneratorDecay0Interface()
{
  delete fTheMessenger;
  if (fInputFile.is_open()) fInputFile.close();
}

void BetaChamberGeneratorDecay0Interface::OpenFile()
{
  char header[160];
  fInputFile.open(fFileName);

  if (!(fInputFile.is_open())) {//not open correctly
    G4cout << "File not valid!" << "\n";
    G4cout << "\n";
  }
  G4int ifound;
//  G4cout << "Decay0 file " << fFileName << " found" << "\n";
  //Here one has to read the header
  //Problem: the header has not always the same lenght
  for (G4int j=0;;j++)
    {
      fInputFile.getline(header,160);
      ifound = strncmp (header," First",6);
      if (!ifound) break;
      if (j == 50) {
	G4cout << "Corrupted file? " << "\n";
	G4cout << "\n";
      }
    }
  //here one has 2 more lines to skip
  fInputFile.getline(header,160);
  fInputFile.getline(header,160);
  G4cout << "Decay0 file " << fFileName << " initialized" << "\n";
}

void BetaChamberGeneratorDecay0Interface::ChangeFileName(G4String newFile)
{
   if (fFileName != newFile) //check if the new file is equal to the other
    {
      if (fInputFile.is_open()) fInputFile.close(); //close the old file
      fFileName = newFile;
      OpenFile(); //open the new one
    }
}

void BetaChamberGeneratorDecay0Interface::GeneratePrimaryVertex(G4Event* evt)
{
  if (!(fInputFile.is_open())) {
    G4cout << "File must be set!" << "\n";
    G4cout << "\n";
  }
  G4int nEntries=0;
  G4int nEvent=0;
  G4double time=0.0;
  G4String particleName;
  G4double px,py,pz,relativeTime;
  G4int particleCode;

  fInputFile >> nEvent >> time >> nEntries;

  if (fInputFile.eof())
    {
      fInputFile.close();
      G4cout << "File over: not enough events!" << "\n";
      if (fRecycleFile)
	{
	  G4cout << "Going to re-open it" << "\n";
	  OpenFile();
	}
      else
	{
	  G4cout << "Fatal Error" << "\n";
	  G4cout << "\n";
	  return;
	}
    }


  particle_time = time*s;

  G4PrimaryVertex* vertex = new G4PrimaryVertex (particle_position,
						 particle_time);

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();

  for (G4int i=0;i<nEntries;i++) //loop of the number of particles per event
    {
      fInputFile >> particleCode >> px >> py >> pz >> relativeTime;

      particleName = ConvertCodeToName(particleCode);

      G4bool generateParticle = true;
      if (particleName == "gamma" && fKillGammas)
	generateParticle = false;
      if (particleName == "e-" && fKillElectrons)
	generateParticle = false;
      if (particleName == "e+" && fKillPositrons)
	generateParticle = false;

      //Create primary particle
      if (generateParticle)
	{
	  G4PrimaryParticle* particle =
	    new G4PrimaryParticle(theParticleTable->FindParticle(particleName),
				  px*MeV,py*MeV,pz*MeV);

	  particle->SetProperTime(relativeTime*s+particle_time);

	  //Add particle to vertex
	  vertex->SetPrimary(particle);
	}
    }

  evt->AddPrimaryVertex(vertex);
}

G4String BetaChamberGeneratorDecay0Interface::ConvertCodeToName(G4int code)
{
  if (code == 1)
    return "gamma";
  else if (code == 2)
    return "e+";
  else if (code == 3)
    return "e-";
  else if (code == 47)
    return "alpha";
  else
    G4cout << "Unknown particle code" << "\n";
  return " ";
}

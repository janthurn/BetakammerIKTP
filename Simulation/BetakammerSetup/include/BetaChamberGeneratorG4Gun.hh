
#ifndef _BetaChamberGeneratorG4Gun_HH
#define _BetaChamberGeneratorG4Gun_HH

#include "BetaChamberVGenerator.hh"

#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"

class G4Event;
class G4ParticleGun;
class BetaChamberGeneratorReadSpectrumFile;
class BetaChamberGeneratorG4GunMessenger;


class BetaChamberGeneratorG4Gun : public BetaChamberVGenerator
{

    public:
        BetaChamberGeneratorG4Gun();
        BetaChamberGeneratorG4Gun(const BetaChamberGeneratorG4Gun &);

        ~BetaChamberGeneratorG4Gun();

          void GeneratePrimaryVertex(G4Event *anevent);
          void SetParticlePosition(G4ThreeVector vec) {fParticleGun->SetParticlePosition(vec);} ;

          void SetReadSpectrumFromFile(G4bool bb){fEnergySpectrumFromFile = bb;};
          void SetFileNameFromFile(G4String);

    private:
        G4ParticleGun      *fParticleGun;
        G4bool fEnergySpectrumFromFile; //custom or normal (= beam)
        BetaChamberGeneratorReadSpectrumFile* fEnergySpectrumTypeGenerator;
        BetaChamberGeneratorG4GunMessenger* fTheMessenger;




};
#endif

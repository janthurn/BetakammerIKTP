
#ifndef _BetaChamberGeneratorG4GunMessenger_HH
#define _BetaChamberGeneratorG4GunMessenger_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithAString;
class G4UIcommand;
class G4ParticleGun;
class BetaChamberGeneratorG4Gun;


class BetaChamberGeneratorG4GunMessenger : public G4UImessenger
{

    public:

        BetaChamberGeneratorG4GunMessenger(BetaChamberGeneratorG4Gun *);
        BetaChamberGeneratorG4GunMessenger(const BetaChamberGeneratorG4GunMessenger &);
        ~BetaChamberGeneratorG4GunMessenger();

        void SetNewValue(G4UIcommand* cmd,G4String str);

    private:
        BetaChamberGeneratorG4Gun* fPointerToMGG4Gun;
        G4UIdirectory* fDirectory;


        G4UIcmdWithABool* fReadFromFileCmd;
        G4UIcmdWithAString* fSetFileNameCmd;



};
#endif

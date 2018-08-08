// ============================================================================
//
//  Sample the postion of the primary event inside the given volume
// (template given by mage)
//
//=============================================================================
//
// Created by Jan T.  21 08 2017
//
// ===========================================================================
#ifndef BetaChamberPositionSampler_h
#define BetaChamberPositionSampler_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ThreeVector.hh"
#include "G4VSolid.hh"

class BetaChamberPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithAnInteger;


class G4Navigator;
class G4VPhysicalVolume;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class BetaChamberPositionSampler
{
public:
      BetaChamberPositionSampler();
      virtual ~BetaChamberPositionSampler();

      G4ThreeVector SampleUniformlyInVolume(G4String volName,G4int copynumber);

      // ------

      G4ThreeVector pick_point_in_box(G4double x_lo, G4double x_hi, G4double y_lo, G4double y_hi,G4double z_lo, G4double z_hi);
      G4ThreeVector pick_point_in_annulus(G4double r1, G4double r2, G4double h);
      G4ThreeVector pick_point_in_orb(G4double r);
      G4ThreeVector pick_point_in_sphere(G4double r);

private:
      G4int fCopyNumber;
      G4String fVolumeName;
      G4bool InitializeSamplingVolume();
      G4VPhysicalVolume* fMyVolume;
      G4Navigator* fMyNavigator;
      G4GeometryType fSolid_type;

      G4double fSolid_par_arr[6];
      G4bool         fHasDaughters;



};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

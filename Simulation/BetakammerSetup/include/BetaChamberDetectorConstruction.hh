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
// $Id: BetaChamberDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file BetaChamberDetectorConstruction.hh
/// \brief Definition of the BetaChamberDetectorConstruction class

#ifndef BetaChamberDetectorConstruction_h
#define BetaChamberDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <map>
#include "BetaChamberSiliconSD.hh"
#include "BetaChamberAbsorberSD.hh"

using std::vector;
using std::string;


class G4VPhysicalVolume;
class G4LogicalVolume;
class BetaChamberEventAction;
class BetaChamberDetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.

class BetaChamberDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        BetaChamberDetectorConstruction();  //BetaChamberEventAction* eventAction
        virtual ~BetaChamberDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();

        G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; };
        G4double         GetSampleShift()  const {return fsampleShift;};
        void             SetSampleShift(G4double value) { fsampleShift=value;}
        G4String         GetSampleVolumeName() const {return fNameSampleVol;};
        const G4VPhysicalVolume* GetAbsorber() {return fPhysicsAbsorber;};
        const G4VPhysicalVolume* GetSensVol() {return fPhysicsSens;};
        void SetSensVol(G4VPhysicalVolume* vol) { fPhysicsSens=vol;};

    // -------------------------------------------------------------------
    // ---- Functions for Messenger
    public:

        void SetSetupNumber(G4int status) { SetupNummer = status;};
        G4int GetSetupNumber(){return SetupNummer;};

        void SetSondenNumber(G4int status) { SondenNummer = status;};
        G4int GetSondenNumber(){return SondenNummer;};

        void SetPositionsNumber(G4int status) { PositionsNummer = status;};
        G4int GetPositionsNumber(){return PositionsNummer;};

        void SetKalibrationsSetup(G4int status) { CalibSetup = status;};
        G4int GetKalibrationsSetup(){return CalibSetup;};

        void SetKalibrationsMaterial(G4int status) { CalibMaterial = status;};
        G4int GetKalibrationsMaterial(){return CalibMaterial;};

        void SetKalibrationsDiameter(G4double status) { CalibSourceSpotDiameter = status;};
        G4double GetKalibrationsDiameter(){return CalibSourceSpotDiameter;};

        void SetSiliconSampleThickness(G4double status) { SiliconSampleThickness = status;};
        G4double GetSiliconSampleThickness(){return SiliconSampleThickness;};

        void SetBismutSampleThickness(G4double status) { BismutSampleThickness = status;};
        G4double GetBismutSampleThickness(){return BismutSampleThickness;};

        void SetAllowedTransition(G4int status){AllowedTransitionMaterial=status; };
        G4int GetAllowedTransition(){return AllowedTransitionMaterial;};

        void SetSiliconBacking(G4bool status){UseSiliconBacking=status; };
        G4int GetSiliconBacking(){return UseSiliconBacking;};

        void SetATSampleShape(G4int status){ATSampleShape=status; };
        G4int GetATSampleShape(){return ATSampleShape;};

    private:
        G4double GetDimensions(const char* entry);
        G4double GetDimensions(string entry);

        void SetDetectorDimensions(G4double,G4double);
        void SetShifts(G4int,G4int,G4int);
        void SetSampleVolumeName(G4int);
        void DefineMaterials();

        // --- Build each part of the chamber

        G4VPhysicalVolume* constructLab();
        void constructPIPS();
        void constructSensitiveDetector();
        void constructSampleHolder25umSetup();
        void constructSampleHolderCalibration();
        void constructSampleHolder2umSetup(G4int);
        void constructSampleHoldersSiliconOnly();
        void constructSampleHolderSiliziumProbenSetup();
        void constructSampleHolderSaphirSetup();
        void constructSampleHolderBismutProbenSystematics();
        void constructSampleHolderBismutProbenFormTests();
        void constructAbsorptionTests();
        std::map<G4String,G4double> fDetDimensions;

   protected:
        // BetaChamberEventAction* fEvtaction;
        G4LogicalVolume*  fScoringVolume;
        BetaChamberSiliconSD* SiliconChipSD;
        BetaChamberAbsorberSD* AbsorberSD;
        G4double fsampleShift;
        G4int SondenNummer;
        G4int SetupNummer;
        G4int PositionsNummer;
        G4int CalibSetup;
        G4int CalibMaterial;
        G4double CalibSourceSpotDiameter;
        G4double SiliconSampleThickness;
        G4double BismutSampleThickness;
        G4int AllowedTransitionMaterial;
        G4bool UseSiliconBacking;
        G4String fNameSampleVol;
        G4int ATSampleShape;

        G4VPhysicalVolume *fPhysicsAbsorber;
        G4VPhysicalVolume *fPhysicsSens;


        BetaChamberDetectorConstructionMessenger* fMessenger;

        // --- World -------
        G4LogicalVolume*  m_phallLog;
        G4VPhysicalVolume* m_phallPhys;
        G4LogicalVolume* CopperZylinderLOG;
        G4LogicalVolume* VacuumLOG;
        G4LogicalVolume* KupferDeckelLOG;

        G4VPhysicalVolume* CopperZylinderPHYS;
        G4VPhysicalVolume* KupferDeckelObenPHYS;
        G4VPhysicalVolume* KupferDeckelUntenPHYS;
        G4VPhysicalVolume* VacuumPHYS;



        // --- PIPS ---------
        G4LogicalVolume* housingLOG;
        G4LogicalVolume* housingTopLOG;
        G4LogicalVolume* housingShellLOG;
        G4LogicalVolume* housingBottomLOG;
        G4LogicalVolume* ElastomerRingTopLOG;
        G4LogicalVolume* SiliconChipLOG;
        G4LogicalVolume* ElastomerRingBottomLOG;
        G4LogicalVolume* MessingPlateLOG;
        G4LogicalVolume* MessingPinLOG;
        G4LogicalVolume* PEbackingLOG;
        G4LogicalVolume* cylinderTopLOG;
        G4LogicalVolume* ringTopLOG;
        G4LogicalVolume* SilberkontaktLOG;


        G4VPhysicalVolume* housingPHYS;
        G4VPhysicalVolume* housingTopPHYS;
        G4VPhysicalVolume* housingShellPHYS;
        G4VPhysicalVolume* housingBottomPHYS;
        G4VPhysicalVolume* ElastomerRingBottomPHYS;
        G4VPhysicalVolume* ElastomerRingTopPHYS;
        G4VPhysicalVolume* MessingPlatePHYS;
        G4VPhysicalVolume* MessingPinPHYS;

        G4VPhysicalVolume* cylinderTopPHYS;
        G4VPhysicalVolume* ringTopPHYS;
        G4VPhysicalVolume* MessingPHYS;
        G4VPhysicalVolume* PEbackingPHYS;
        G4VPhysicalVolume* SiliconChipPHYS;
        G4VPhysicalVolume* SilberkontaktPHYS;

        //  --- sample holder

        G4LogicalVolume* GrundKoerper1LOG;
        G4LogicalVolume* GrundKoerper2LOG;
        G4LogicalVolume* GrundKoerper3LOG;
        G4LogicalVolume* HalterSondePart1LOG;
        G4LogicalVolume* HalterSondePart2LOG;
        G4LogicalVolume* HalterSondePart3LOG;
        G4LogicalVolume* DistanzRingLOG;
        G4LogicalVolume* GegenSondeLOG;
        G4LogicalVolume* FixierRingLOG;
        G4LogicalVolume* BismutSampleLOG;
        G4LogicalVolume* SiWaferLOG;
        G4LogicalVolume* mylarBackingLOG;
        G4LogicalVolume* BismutSamplePart1LOG;
        G4LogicalVolume* BismutSamplePart2LOG;

        G4VPhysicalVolume* GrundKoerper1PHYS;
        G4VPhysicalVolume* GrundKoerper2PHYS;
        G4VPhysicalVolume* GrundKoerper3PHYS;
        G4VPhysicalVolume* HalterSondePart1PHYS;
        G4VPhysicalVolume* HalterSondePart2PHYS;
        G4VPhysicalVolume* HalterSondePart3PHYS;
        G4VPhysicalVolume* BismutSamplePHYS ;
        G4VPhysicalVolume* BismutSamplePart1PHYS ;
        G4VPhysicalVolume* BismutSamplePart2PHYS ;
        G4VPhysicalVolume* mylarBackingPHYS;
        G4VPhysicalVolume* GegenSondePHYS;
        G4VPhysicalVolume* DistanzRing1PHYS;
        G4VPhysicalVolume* DistanzRing2PHYS;
        G4VPhysicalVolume* FixierRingPHYS;
        G4VPhysicalVolume* SiWaferPHYS;


        // --- calibration sample
        G4LogicalVolume* BaseLOG;
        G4LogicalVolume* mylarLOG;
        G4LogicalVolume* TopRingLOG;
        G4LogicalVolume* probeLOG;
        // G4LogicalVolume* NeueProbeLOG;

        G4VPhysicalVolume* BasePHYS;
        G4VPhysicalVolume* mylarPHYS;
        G4VPhysicalVolume* TopRingPHYS;
        G4VPhysicalVolume* probePHYS;
        // G4VPhysicalVolume* NeueProbePHYS;

        // --- Allowed transition sample
        G4LogicalVolume* ATSampleLOG;
        G4LogicalVolume* SaphirBackingLOG;

        G4VPhysicalVolume* ATSamplePHYS;
        G4VPhysicalVolume *SaphirBackingPHYS;

        //---- physics Test
        // G4LogicalVolume* AbsorberLOG;
        // G4LogicalVolume* QuelleLOG;
        //
        // G4VPhysicalVolume* AbsorberPHYS;
        // G4VPhysicalVolume*  QuellePHYS;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

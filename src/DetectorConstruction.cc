#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "MySensitiveDetector.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction() 
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Define materials
    G4Material *quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); // SiO2
    G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic"); // Vacuum

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Define the world volume (1m^3 of vacuum)
    G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m); 
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(
        0, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0);

    //Visualization for the world volume (invisible)
    G4VisAttributes *worldVisAttr = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.0)); // Invisible
    worldVisAttr->SetVisibility(false); // Set visibility to false
    logicWorld->SetVisAttributes(worldVisAttr);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Define the detector volume (1cm^3 of quartz)
    G4double targetSize = 0.5 * m; //half length of the 1 m cube
    G4Box *solidTarget = new G4Box("solidTarget", targetSize, targetSize, targetSize); // Create a box of quartz
    G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, quartz, "logicTarget"); // Logical volume for the target
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicTarget, "physTarget", logicWorld, false, 0); // Place the target volume in the world volume

    fScoringVolume = logicTarget; // Set the scoring volume to the target volume

    // Visualization for SiO₂ box (visible)
    G4VisAttributes *targetVisAttr = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); // White color
    targetVisAttr->SetForceSolid(false);
    targetVisAttr->SetVisibility(true);
    logicTarget->SetVisAttributes(targetVisAttr); // Set the visualization attributes for the target volume

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    G4SDManager *sdManager = G4SDManager::GetSDMpointer(); // Get the sensitive detector manager
    auto *sensitiveDetector = new MySensitiveDetector("QuartzSD"); // Create a new sensitive detector for the quartz material
    sdManager->AddNewDetector(sensitiveDetector); // Add the sensitive detector to the manager
    logicTarget->SetSensitiveDetector(sensitiveDetector); // Set the sensitive detector for the target volume

return physWorld;
}


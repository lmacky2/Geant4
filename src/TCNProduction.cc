#include <iostream> // Standard C++ header for input/output

// Geant4 headers
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"

// Custom headers
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "MySensitiveDetector.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    // 1. Create the run manager
    G4RunManager* runManager = new G4RunManager(); // Create the run manager

    // 2. User initializations
    runManager->SetUserInitialization(new MyDetectorConstruction()); // geometry + materials
    runManager->SetUserInitialization(new PhysicsList());           // physics processes
    runManager->SetUserInitialization(new ActionInitialization());  // actions (run, event, tracking, etc.)

    // 3. Initialize geometry + physics
    runManager->Initialize();

    // 4. Visualization manager (we can keep this in both modes)
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // 5. UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // 6. Decide: interactive (Qt) vs batch (macro)
    G4UIExecutive* ui = nullptr;

    if (argc == 1)
    {
        // ---------- INTERACTIVE MODE (desktop, Qt GUI) ----------
        ui = new G4UIExecutive(argc, argv);

        // Visualization commands (only make sense with a viewer)
        UImanager->ApplyCommand("/vis/verbose 2");               // verbosity for visualization
        UImanager->ApplyCommand("/tracking/verbose 1");          // verbosity for tracking
        UImanager->ApplyCommand("/vis/open OGL");                // open OpenGL window

        // Set up the viewer
        UImanager->ApplyCommand("/vis/viewer/set/upVector 0 0 1");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 3 -1 1");

        // Trajectory colours by particle ID
        UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set gamma blue");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set e- red");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set e+ red");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set proton yellow");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set neutron cyan");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set alpha orange");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set mu- purple");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set mu+ purple");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set ion green");

        // Build and draw scene
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand("/vis/scene/add/axes");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

        // Start interactive session (this is where Qt / OGL runs)
        ui->SessionStart();

        delete ui;
    }
    else
    {
        // ---------- BATCH MODE (cluster, macro) ----------
        // argv[1] is expected to be the macro file name, e.g. "run_beluga.mac"
        std::string command = "/control/execute ";
        std::string fileName = argv[1];

        UImanager->ApplyCommand((command + fileName).c_str());
        // No UIExecutive, no /vis/open OGL, no Qt at all
    }

    // 7. Cleanup
    delete visManager;
    delete runManager;

    return 0;
}
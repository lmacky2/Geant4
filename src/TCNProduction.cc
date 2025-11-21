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
	G4RunManager *runManager = new G4RunManager(); // Create the run manager

	runManager->SetUserInitialization(new MyDetectorConstruction()); // Set the detector construction. Defines the geometry and materials.
	runManager->SetUserInitialization(new PhysicsList()); // Set the physics list. Defines the physics processes and interactions.
	runManager->SetUserInitialization(new ActionInitialization()); // Set the action initialization. This includes the run, event, and tracking actions.
	

	runManager->Initialize(); // Initialize the run manager. This will initialize the geometry and physics.

	G4UIExecutive *ui = new G4UIExecutive(argc, argv); 

	G4VisManager *visManager = new G4VisExecutive();
	visManager ->Initialize();

	///////////////////////////////////////////////////////VISUALIZATION COMMANDS/////////////////////////////////////////////////////////////////////////////

	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/vis/verbose 2"); //Set verbosity level for visualization
	UImanager->ApplyCommand("/tracking/verbose 1"); //Set verbosity level for tracking
	UImanager->ApplyCommand("/vis/open OGL"); //Open OpenGL window

	//Set up the viewer first
	UImanager->ApplyCommand("/vis/viewer/set/upVector 0 0 1"); //Set up vector
	UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 3 -1 1"); //Set the viewpoint vector

	//Set trajectory color by particle ID
	UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByParticleID"); //Create trajectories by particle ID
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set gamma blue"); //Set color for gamma particles
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set e- red"); //Set color for electrons
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set e+ red"); //Set color for positrons	
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set proton yellow"); //Set color for protons
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set neutron cyan"); //Set color for neutrons
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set alpha orange"); //Set color for alpha particles
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set mu- purple"); //Set color for muons
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set mu+ purple"); //Set color for anti-muons
	UImanager->ApplyCommand("/vis/modeling/trajectories/drawByParticleID/set ion green"); //Set color for ions
	
	//Build and draw scene
	UImanager->ApplyCommand("/vis/drawVolume"); //Draw the geometry
	UImanager->ApplyCommand("/vis/scene/add/axes"); //Add axes to the scene
	UImanager->ApplyCommand("/vis/scene/add/trajectories smooth"); //Add trajectories to the scene
	UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	ui->SessionStart();

	return 0;
}


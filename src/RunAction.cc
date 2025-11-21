#include "RunAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

RunAction::RunAction(SteppingAction* stepping, TrackingAction* tracking)
    : fSteppingAction(stepping), fTrackingAction(tracking) {}

RunAction::~RunAction() {
    if (energyLog.is_open()) {
        energyLog.close(); // Close the energy log file if it's open
    }
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4cout << "### Run started ###" << G4endl;

    energyLog.open("../../output/EnergyLog.txt", std::ios::out | std::ios::app);
    if (!energyLog){
        G4Exception("RunAction::BeginOfRunAction", "FileError", FatalException,
                    "Could not open energy_log.txt for writing.");
    }
}

void RunAction::EndOfRunAction(const G4Run*)
{
    if (energyLog.is_open()) {
        energyLog.close(); // Close the energy log file if it's open
    }

    if (fTrackingAction) {
        fTrackingAction->WriteSummaryToFile();
        fTrackingAction->WriteTCNTallyToFile(); // Write TCN tally to file
        G4cout << "Tracking tally written to output2.txt" << G4endl;
    } else {
        G4cerr << "TrackingAction pointer is null." << G4endl;
    }
}
std::ofstream& RunAction::GetEnergyLogFile() const {
    return energyLog; // Return the energy log file stream

}

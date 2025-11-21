#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"

#include <fstream>

SteppingAction::SteppingAction(MyDetectorConstruction* detConstruction)
{
    fScoringVolume = detConstruction->GetScoringVolume();

    // Open the output file
    outputFile.open("../output/Output.txt", std::ios::out);
    if (outputFile.is_open()) {
        outputFile << "TrackID\tParentID\tStepNum\tX(mm)\tY(mm)\tZ(mm)\tKineticEnergy(MeV)\tParticle\tProcess\n";
        outputFile << "-------------------------------------------------------------\n";
    }
}

SteppingAction::~SteppingAction()
{
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    const G4Track* track = step->GetTrack();
    G4StepPoint* preStep = step->GetPreStepPoint();

    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    G4int stepNum = track->GetCurrentStepNumber();
    G4ThreeVector pos = preStep->GetPosition();
    G4double energy = preStep->GetKineticEnergy();
    G4String particleName = track->GetDefinition()->GetParticleName();

    const G4VProcess* process = preStep->GetProcessDefinedStep();
    G4String processName = (process != nullptr) ? process->GetProcessName() : G4String("Transportation");

    // --- Per-step logging ---
    outputFile << trackID << '\t'
               << parentID << '\t'
               << stepNum << '\t'
               << pos.x() / mm << '\t'
               << pos.y() / mm << '\t'
               << pos.z() / mm << '\t'
               << energy / MeV << '\t'
               << particleName << '\t'
               << processName << '\n';

    const auto& secondaries = *step->GetSecondary();
    if (secondaries.empty()) return;

    if (!process) return;
    G4String procName = process->GetProcessName();

    // Check if any ion (nucleus) was created in this step
    bool hasIon = false;
    for (const auto& sec : secondaries) {
        if (sec->GetDefinition()->GetParticleType() == "nucleus") {
            hasIon = true;
            break;
        }
    }

    if (hasIon || track->GetDefinition()->GetParticleType() == "nucleus") {
        std::ofstream reactionOut("../output/ReactionLog.txt", std::ios::app);
        reactionOut << "=== Reaction ===\n";
        reactionOut << "Process: " << procName << "\n";
        reactionOut << "Primary: " << track->GetDefinition()->GetParticleName()
                    << " (TrackID=" << trackID << "), Energy: "
                    << energy / MeV << " MeV\n";
        reactionOut << "Secondaries:\n";

        for (const auto& sec : secondaries) {
            const G4ParticleDefinition* def = sec->GetDefinition();
            G4String name = def->GetParticleName();
            G4double secEnergy = sec->GetKineticEnergy();
            reactionOut << "  " << name << ", Energy: " << secEnergy / MeV << " MeV\n";
        }

        reactionOut << "\n";
        reactionOut.close();
    }
}
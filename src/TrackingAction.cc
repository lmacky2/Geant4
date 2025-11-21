//Header files
#include "TrackingAction.hh"
#include "G4Track.hh"
#include <fstream>
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <set>

// Constructor and Destructor
TrackingAction::TrackingAction() {}
TrackingAction::~TrackingAction() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{

    if (track->GetParentID() == 0) return; // Skip primary particles (optional: include primaries by removing this check)

    const G4ParticleDefinition* particleDef = track->GetDefinition(); 
    G4String type = particleDef->GetParticleType(); // Get the type of the particle
    G4double energy = track->GetKineticEnergy(); // Get the kinetic energy of the particle
    G4String name = particleDef->GetParticleName(); // Get the name of the particle

    // Kill low-energy gammas and electrons (< 5 MeV)
    if ((name == "e-" || name == "e+" || name == "gamma") && energy < 5 * MeV) {
        const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
        return;
    }

    // Tally ions and all other particles
    particleCountMap[name]++;

   // Only count specific TCN nuclides
    const std::set<G4String> tcnNuclides = {"Be10", "Al26", "C14", "Ne21", "Cl36", "He3"};
   if (type == "nucleus" && tcnNuclides.count(name)) {
       tcnTallyMap[name]++;

}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrackingAction::WriteSummaryToFile() const
{
    std::ofstream outFile("../../output/ParticleTally.txt"); // Open the output file for writing
    if (outFile.is_open()) {
        outFile << "Particle\tCount\n"; // Header for the output file
        for (const auto& entry : particleCountMap) {
            outFile << entry.first << '\t' << entry.second << '\n';
        }
        outFile.close();
    } else {
        G4cerr << "Error: Could not open ParticleTally.txt for writing.\n";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrackingAction::WriteTCNTallyToFile() const
{
    std::ofstream out("../../output/TCNtally.txt");
    if (out.is_open()) {
        out << "Nuclide\tCount\n";
        for (const auto& entry : tcnTallyMap) {
            out << entry.first << '\t' << entry.second << '\n';
        }
        out.close();
    } else {
        G4cerr << "Error: Could not open TCNtally.txt for writing.\n";
    }
}
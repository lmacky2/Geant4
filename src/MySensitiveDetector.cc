#include "MySensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"

MySensitiveDetector::MySensitiveDetector(const G4String &name)
    : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector() {}

G4bool MySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0)
    {
        G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
        G4cout << "Energy deposited: " << edep / keV << " keV by " << particleName << G4endl;
    }
    return true;
}
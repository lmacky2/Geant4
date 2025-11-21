
#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

#include <fstream>
#include <iostream>
#include <algorithm>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), 
    fParticleGun(0)
{
    fParticleGun = new G4ParticleGun(1); // Create a particle gun with 1 particle per event
    fParticleGun->SetParticleDefinition(G4MuonMinus::MuonMinus()); // Set the particle definition
    
    LoadSpectrum("../data/spectrum_1000kmwe_flat.dat");

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun; // Clean up the particle gun
}

void PrimaryGeneratorAction::LoadSpectrum(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        G4Exception("PrimaryGeneratorAction::LoadSpectrum", "FileError", FatalException,
                    ("Could not open file: " + filename).c_str());
    }

    double energy, flux;
    while (file >> energy >> flux) {
        energies_.push_back(energy);
        spectrum_.push_back(flux);
    }

    cdf_.resize(spectrum_.size(), 0.0);
    for (size_t i = 1; i < spectrum_.size(); ++i) {
        double width = energies_[i] - energies_[i - 1];
        cdf_[i] = cdf_[i - 1] + 0.5 * (spectrum_[i] + spectrum_[i - 1]) * width;
    }

    double total = cdf_.back();
    for (auto& val : cdf_) val /= total;
}

double PrimaryGeneratorAction::SampleEnergy() {
    double u = G4UniformRand();
    auto it = std::lower_bound(cdf_.begin(), cdf_.end(), u);
    size_t idx = std::distance(cdf_.begin(), it);

    if (idx == 0) return energies_.front();
    if (idx >= energies_.size()) return energies_.back();

    double x0 = cdf_[idx - 1], x1 = cdf_[idx];
    double y0 = energies_[idx - 1], y1 = energies_[idx];

    return y0 + (u - x0) * (y1 - y0) / (x1 - x0);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{    
    G4double r = G4UniformRand();
    G4double costheta = std::pow(r, 1./2.3);
    G4double sintheta = std::sqrt(1. - costheta*costheta);
    G4double phi = 2.*CLHEP::pi * G4UniformRand();

    G4double dx = sintheta * std::cos(phi);
    G4double dy = sintheta * std::sin(phi);
    G4double dz = -costheta;

    G4double radius = 0.5*m;
    G4double x = (2*G4UniformRand() - 1) * radius;
    G4double y = (2*G4UniformRand() - 1) * radius;
    G4double z = 0.5*m;

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dx, dy, dz));
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));

    G4double energy = SampleEnergy() * MeV;
    fParticleGun->SetParticleEnergy(energy);
    
    //Log the energy
    const RunAction* runAction = dynamic_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
    if (runAction){
        runAction->GetEnergyLogFile() << energy / MeV << "\n";
    }

    G4ThreeVector dir(dx, dy, dz);
    G4cout << "Direction: " << dir << ", Theta = " << dir.theta() * 180. / CLHEP::pi
           << " deg, Energy = " << energy / GeV << " GeV" << G4endl;

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

    
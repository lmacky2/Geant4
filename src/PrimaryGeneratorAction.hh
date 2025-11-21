#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemofUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

#include <vector>
#include <string>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    void LoadSpectrum(const std::string& filename);
    double SampleEnergy(); // Sample an energy value based on the spectrum

    std::vector<double> energies_; // Energy values from the spectrum file
    std::vector<double> spectrum_; // Corresponding spectrum values
    std::vector<double> cdf_; // Cumulative distribution function for sampling

    G4ParticleGun *fParticleGun;
};

#endif
#ifndef MYSENSITIVEDETECTOR_HH
#define MYSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(const G4String &name);
    virtual ~MySensitiveDetector();

    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
};

#endif
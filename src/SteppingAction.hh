#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "DetectorConstruction.hh"
#include "G4UserSteppingAction.hh"

#include <fstream>
#include <map>
#include <set>

class MyDetectorConstruction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(MyDetectorConstruction* detectorConstruction);
    ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);
    void WriteSecondarySummaryToFile() const;
 

private:
    std::ofstream outputFile;
    G4LogicalVolume* fScoringVolume;
    std::map<G4String, int> particleCountMap;
    std::set<G4int> countedTrackIDs;
    
};

#endif
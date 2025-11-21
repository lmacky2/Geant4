#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"

#include <fstream>

class SteppingAction;
class TrackingAction;

class RunAction : public G4UserRunAction
{
public:
    RunAction(SteppingAction* stepping, TrackingAction* tracking);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    std::ofstream& GetEnergyLogFile() const; 

private:
    SteppingAction* fSteppingAction = nullptr;
    TrackingAction* fTrackingAction = nullptr;

    mutable std::ofstream energyLog; // File to log energy information
};

#endif
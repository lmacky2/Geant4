#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

class TrackingAction;

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    ~ActionInitialization() override;

    void Build() const override;

    TrackingAction* GetTrackingAction() const;
    
private:
    mutable TrackingAction* fTrackingAction = nullptr;
};

#endif


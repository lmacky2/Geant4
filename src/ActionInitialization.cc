#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "SteppingAction.hh"
#include "ActionInitialization.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
    auto detector = static_cast<const MyDetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction());
       
        auto* stepping = new SteppingAction(const_cast<MyDetectorConstruction*>(detector));
        fTrackingAction = new TrackingAction();

        SetUserAction(stepping);
        SetUserAction(new PrimaryGeneratorAction());
        SetUserAction(new RunAction(stepping, fTrackingAction));
        SetUserAction(fTrackingAction);

}

TrackingAction* ActionInitialization::GetTrackingAction() const
{
    return fTrackingAction;
}
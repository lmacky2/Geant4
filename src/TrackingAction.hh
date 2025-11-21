#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include <map>
#include "G4String.hh"

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction();
    ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track* track);
    void WriteSummaryToFile() const;
    void WriteTCNTallyToFile() const; // Function to write TCN tally to file

private:
    std::map<G4String, int> particleCountMap;
    std::map<G4String, int> tcnTallyMap; // Tally for TCN particles
};

#endif

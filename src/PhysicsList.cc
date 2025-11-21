#include "PhysicsList.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4VModularPhysicsList.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4MuonNuclearProcess.hh"
#include "G4MuonVDNuclearModel.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4ProcessManager.hh"


PhysicsList::PhysicsList()
{
    //Electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics_option4()); // EM processes for e-, e+, gamma, muons and hadrons

    //Decay and stopping processes
    RegisterPhysics(new G4DecayPhysics()); // Decay processes for unstable particles
    RegisterPhysics(new G4RadioactiveDecayPhysics()); // Radioactive decay processes for unstable nuclei
    RegisterPhysics(new G4StoppingPhysics()); // Stopping processes for heavy ions

    //Hadronic physics
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP()); // Hadron processes for protons, neutrons, and pions
    RegisterPhysics(new G4NeutronTrackingCut()); // Neutron tracking cut for better performance


    RegisterPhysics(new G4HadronElasticPhysics()); // Elastic scattering processes for protons and neutrons
    RegisterPhysics(new G4IonBinaryCascadePhysics()); // Binary cascade processes for ions


    RegisterPhysics(new G4IonPhysics()); // Ionization processes for ions
  
    
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
    // Set default cut values for all particle types
    SetCutValue(0.000001*mm, "proton");
    SetCutValue(0.000001*mm, "neutron");
    SetCutValue(0.000001*mm, "gamma");
    SetCutValue(100*km, "e-"); // Set long cut for electrons to avoid unnecessary tracking
    SetCutValue(100*km, "e+");
    SetCutValue(0.000001*mm, "mu-");
    SetCutValue(0.000001*mm, "mu+");
    SetCutValue(0.000001*mm, "alpha");
    SetCutValue(0.000001*mm, "ion"); // Set cut for ions
    

    // Call the base class method to finalize the cuts
    G4VUserPhysicsList::SetCuts();
}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess(); // Builds default processes

    // Create and attach muon-nuclear process to both mu+ and mu-
    auto muNuclearProcess = new G4MuonNuclearProcess();
    auto muNuclearModel = new G4MuonVDNuclearModel();
    muNuclearProcess->RegisterMe(muNuclearModel);

    // Add to mu- and mu+ process managers
    G4ProcessManager* pManagerMuMinus = G4MuonMinus::MuonMinus()->GetProcessManager();
    G4ProcessManager* pManagerMuPlus  = G4MuonPlus::MuonPlus()->GetProcessManager();

    pManagerMuMinus->AddDiscreteProcess(muNuclearProcess);
    pManagerMuPlus->AddDiscreteProcess(muNuclearProcess);
}
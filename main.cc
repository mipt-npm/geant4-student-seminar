#include <DetectorConstruction.hh>
#include <PrimaryGeneratorAction.hh>
#include <QGSP_BERT.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <RunAction.hh>
#include <random>
#include "G4RunManager.hh"
#include "Settings.hh"
#include "Messenger.hh"

int main(int argc, char **argv) {
    random_device rd;
    uniform_int_distribution<long> uid(0, LONG_MAX);
    long seed = uid(rd);
    HepRandom::setTheSeed(seed);
    cout<<"Seed: " << seed<<endl;
    auto settings = new Settings();
    auto messender = Messenger(settings);
    G4UIExecutive *ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    TupleId *tupleId = new TupleId();
    G4RunManager *runManager = new G4RunManager;
    runManager->SetUserInitialization(new DetectorConstruction(tupleId,settings));
    runManager->SetUserInitialization(new QGSP_BERT);
    runManager->SetUserAction(new PrimaryGeneratorAction(settings));
    runManager->SetUserAction(new RunAction(tupleId, settings));
    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}

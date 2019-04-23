// from AnaEx01

#include "HistoManager.hh"
#include "HistoManagerMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
 : fFactoryOn(false)
{
  _messenger = new HistoManagerMessenger(this);
  _outFileName = "muOnSi";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
      
  // Create directories 
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetNtupleDirectoryName("trees");
    
  // Open an output file
  //
  G4bool fileOpen = analysisManager->OpenFile(_outFileName);
  if (! fileOpen) {
    G4cerr << "\n---> HistoManager::Book(): cannot open " 
           << analysisManager->GetFileName() << G4endl;
    return;
  }
  
  // Create histograms.
  // Histogram ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstHistoId(1);  
  
  // id = 0
  analysisManager->CreateH1("Edep","Deposited energy in Silicon", 5000, 0., 0.2*MeV);
  analysisManager->SetH1XAxisTitle(0, "Energy [MeV]");
  analysisManager->SetH1YAxisTitle(0, "Entries");
  // id = 1
  // analysisManager->CreateH1("EGap","Edep in gap (MeV)", 100, 0., 100*MeV);
  // // id = 2
  // analysisManager->CreateH1("LAbs","trackL in absorber (mm)", 100, 0., 1*m);
  // // id = 3
  // analysisManager->CreateH1("LGap","trackL in gap (mm)", 100, 0., 50*cm);

  // Create ntuples.
  // Ntuples ids are generated automatically starting from 0.
  // The start value can be changed by:
  // analysisManager->SetFirstMtupleId(1);  
  
  // Create 1st ntuple (id = 0)
  analysisManager->CreateNtuple("EdepTree", "Etot and summed or averaged quantities");
  analysisManager->CreateNtupleIColumn("event"); // column Id = 0
  analysisManager->CreateNtupleDColumn("Etot"); // column Id = 1
  analysisManager->CreateNtupleIColumn("nHits"); // column Id = 2
  analysisManager->CreateNtupleDColumn("x"); // column Id = 3
  analysisManager->CreateNtupleDColumn("y"); // column Id = 4
  analysisManager->CreateNtupleDColumn("z"); // column Id = 5
  analysisManager->FinishNtuple();

  // // Create 2nd ntuple (id = 1)
  analysisManager->CreateNtuple("Hits", "All the hits");
  analysisManager->CreateNtupleIColumn("event"); // column Id = 0
  analysisManager->CreateNtupleIColumn("trackId"); // column Id = 1  
  analysisManager->CreateNtupleDColumn("Edep"); // column Id = 2
  analysisManager->CreateNtupleIColumn("hit"); // column Id = 3
  analysisManager->CreateNtupleDColumn("x"); // column Id = 4
  analysisManager->CreateNtupleDColumn("y"); // column Id = 5
  analysisManager->CreateNtupleDColumn("z"); // column Id = 6
  analysisManager->FinishNtuple();
  
  fFactoryOn = true;       

  G4cout << "\n----> Output file is open in " 
         << analysisManager->GetFileName() << "." 
         << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
  if (! fFactoryOn) return;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
  analysisManager->Write();
  analysisManager->CloseFile(); 
   
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
      
  delete G4AnalysisManager::Instance();
  fFactoryOn = false;
}


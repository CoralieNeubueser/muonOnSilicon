// from B2a

#include "SiDetSD.hh"
#include "HistoManager.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiDetSD::SiDetSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiDetSD::~SiDetSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiDetSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
    = new SiDetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SiDetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep==0.) return false;

  // event ID
  const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  if(!evt) return false;
  G4int eID = evt->GetEventID();
  
  SiDetHit* newHit = new SiDetHit();
  newHit->SetEvtID(eID);
  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPreStepPoint()->GetPosition());

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiDetSD::EndOfEvent(G4HCofThisEvent*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
  
  G4int nofHits = fHitsCollection->entries();
  if(nofHits == 0)
    return;

  G4double edep;
  G4double Etot = 0;
  G4double x = 0;
  G4double y = 0;
  G4double z = 0;
  for(G4int i= 0; i < nofHits; ++i){
    edep = (*fHitsCollection)[i]->GetEdep();

    // fill tree with full hit information (id = 1)
    analysisManager->FillNtupleIColumn(1, 0, (*fHitsCollection)[i]->GetEvtID());
    analysisManager->FillNtupleIColumn(1, 1, (*fHitsCollection)[i]->GetTrackID());
    analysisManager->FillNtupleDColumn(1, 2, edep);
    analysisManager->FillNtupleIColumn(1, 3, i);
    analysisManager->FillNtupleDColumn(1, 4, (*fHitsCollection)[i]->GetPos()[0]);
    analysisManager->FillNtupleDColumn(1, 5, (*fHitsCollection)[i]->GetPos()[1]);
    analysisManager->FillNtupleDColumn(1, 6, (*fHitsCollection)[i]->GetPos()[2]);
    analysisManager->AddNtupleRow(1);  

    // calculate quantities for the other tree
    Etot += edep;
    x += (*fHitsCollection)[i]->GetPos()[0] * edep;
    y += (*fHitsCollection)[i]->GetPos()[1] * edep;
    z += (*fHitsCollection)[i]->GetPos()[2] * edep;
  }

  x /= Etot;
  y /= Etot;
  z /= Etot;
  
  // fill histogram
  analysisManager->FillH1(0, Etot);

  // fill tree with summed or averaged quantities (id = 0)
  analysisManager->FillNtupleIColumn(0, 0, (*fHitsCollection)[0]->GetEvtID());
  analysisManager->FillNtupleDColumn(0, 1, Etot);
  analysisManager->FillNtupleIColumn(0, 2, nofHits);
  analysisManager->FillNtupleDColumn(0, 3, x);
  analysisManager->FillNtupleDColumn(0, 4, y);
  analysisManager->FillNtupleDColumn(0, 5, z);
  analysisManager->AddNtupleRow(0);  
  
  if ( verboseLevel>1 ) { 
    //G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the silicon detector: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

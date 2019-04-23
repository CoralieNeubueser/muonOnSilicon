// from AnaEx01

#ifndef HISTOMANAGER_HH
#define HISTOMANAGER_HH

#include "globals.hh"

#include "g4root.hh"
//#include "g4csv.hh"
//#include "g4xml.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManagerMessenger;

class HistoManager
{
public:
  HistoManager();
  ~HistoManager();

  void Book();
  void Save();

  void SetOutFileName(G4String name) {_outFileName = name;};
  
private:
  G4bool fFactoryOn;
  G4String _outFileName;
  HistoManagerMessenger* _messenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


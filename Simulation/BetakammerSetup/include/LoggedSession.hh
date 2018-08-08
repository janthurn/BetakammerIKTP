#ifndef LoggedSession_hh
#define LoggedSession_hh

#include "G4UIsession.hh"
#include <iostream>
#include <fstream>

using namespace std;

class LoggedSession : G4UIsession {
public:
	LoggedSession(G4String,G4String);
	~LoggedSession();

	G4UIsession* SessionStart();
	G4int ReceiveG4cout(G4String);
	G4int ReceiveG4cerr(G4String);

private:
	ofstream outFile;
	ofstream errFile;
};

#endif 

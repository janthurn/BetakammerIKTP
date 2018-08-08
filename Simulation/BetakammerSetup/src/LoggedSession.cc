#include "LoggedSession.hh"
#include "G4UImanager.hh"

LoggedSession::LoggedSession(G4String outputFileName,G4String errFileName){
//	outFile = ofstream(outputFileName);
//	errFile = ofstream(inputFileName);
	outFile.open(outputFileName);
	errFile.open(errFileName);
	G4UImanager* UI = G4UImanager::GetUIpointer();
	UI->SetCoutDestination(this);
}

LoggedSession::~LoggedSession(){
	outFile.close();
	errFile.close();
}

G4UIsession* LoggedSession::SessionStart(){
	return NULL;
}

G4int LoggedSession::ReceiveG4cout(G4String output){
	outFile<<output;
	outFile.flush();
	return 0;
}

G4int LoggedSession::ReceiveG4cerr(G4String err){
	errFile<<err;
	errFile.flush();
	return 0;
}

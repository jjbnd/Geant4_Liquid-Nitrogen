#ifndef __RUN_ACTION_HH__
#define __RUN_ACTION_HH__

#include "G4UserRunAction.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);
};


#endif // __RUN_ACTION_HH__
#ifndef __EVENT_ACTION_HH__
#define __EVENT_ACTION_HH__

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
};

#endif // __EVENT_ACTION_HH__
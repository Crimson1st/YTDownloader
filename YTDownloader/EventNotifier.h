#ifndef EVENTNOTIFIER_H
#define EVENTNOTIFIER_H

#include <wx/event.h>
class EventNotifier : public wxEvtHandler
{
public:
    EventNotifier();
    virtual ~EventNotifier();
    
    static EventNotifier* Get();
};

#endif // EVENTNOTIFIER_H

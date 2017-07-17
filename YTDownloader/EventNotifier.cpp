#include "EventNotifier.h"

EventNotifier::EventNotifier() {}

EventNotifier::~EventNotifier() {}

EventNotifier* EventNotifier::Get()
{
    static EventNotifier notifier;
    return &notifier;
}
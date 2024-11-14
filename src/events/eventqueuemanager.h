#include "ieventqueuemanager.h"
#include <eventpp/eventqueue.h>
#include <eventpp/utilities/anyid.h>
#include <eventpp/utilities/anydata.h>
#include <servicelocators/servicelocator.hpp>

#ifndef EVENTS_H
#define EVENTS_H

struct BaseEvent {

};

struct EventQueueManager : ServiceBase<IEventQueueManager> {

    private:
        eventpp::EventQueue<int, void (const BaseEvent &)> queue;
};

#endif //EVENTS_H

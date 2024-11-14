//
// Created by Joakim Wennergren on 2024-11-13.
//

#ifndef IEVENTQUEUEMANAGER_H
#define IEVENTQUEUEMANAGER_H

#include <servicelocators/servicelocator.hpp>

class IEventQueueManager : public IService
{
public:
    virtual ~IEventQueueManager() = default;
};

#endif //IEVENTQUEUEMANAGER_H

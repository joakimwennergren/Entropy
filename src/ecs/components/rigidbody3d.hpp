#pragma once

#include <btBulletDynamicsCommon.h>

#include "ecs/components/boxcollisionshape3d.hpp"

namespace Entropy
{
    namespace Components
    {
        struct RigidBody3D
        {
        public:
            void Initialize(std::shared_ptr<ServiceLocator> serviceLocator, flecs::entity *entity)
            {
                _serviceLocator = serviceLocator;
                auto physics3d = _serviceLocator->GetService<Physics3D>();

                auto boxcollider = entity->get_ref<Entropy::Components::BoxCollisionShape3D>();

                if (boxcollider.get() != nullptr)
                {
                    body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(btScalar(0.0f), boxcollider->box_motion_state, boxcollider->boxShape, btVector3(0, 0, 0)));
                    body->setUserPointer(entity);
                    physics3d->GetWorld()->addRigidBody(body);
                }
            }

            void SetPosition(float x, float y, float z)
            {
                // Create a transform for the box shape

                boxTransform.setIdentity();                 // Set the transform to identity (no translation or rotation by default)
                boxTransform.setOrigin(btVector3(x, y, z)); // Set the position of the shape

                // Create a motion state and attach the transform to it
                btDefaultMotionState *motionState = new btDefaultMotionState(boxTransform);

                body->setMotionState(motionState);
            }

        private:
            btTransform boxTransform;
            btRigidBody *body;
            std::shared_ptr<ServiceLocator> _serviceLocator;
        };
    }
}
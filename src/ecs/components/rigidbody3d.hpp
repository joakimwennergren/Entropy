#pragma once

#include <spdlog/spdlog.h>

#include <btBulletDynamicsCommon.h>

#include "ecs/components/boxcollisionshape3d.hpp"
#include "ecs/components/trianglemeshcollisionshape3d.hpp"

#include <physics/3d/physics3d.hpp>

namespace Entropy
{
    namespace Components
    {
        struct RigidBody3D
        {
        public:
            void Initialize(flecs::entity entity)
            {
                // _entity = entity;
                // auto physics3d = _serviceLocator->GetService<Physics::Physics3D>();

                // if (entity.has<Entropy::Components::BoxCollisionShape3D>())
                // {
                //     auto boxcollider = entity.get_ref<Entropy::Components::BoxCollisionShape3D>();
                //     if (boxcollider.get() != nullptr)
                //     {
                //         body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(btScalar(0.0f), boxcollider->box_motion_state, boxcollider->boxShape, btVector3(0, 0, 0)));
                //         body->setUserPointer(&_entity);
                //         physics3d->GetWorld()->addRigidBody(body);
                //     }
                // }
            }

            void SetPosition()
            {
                /*
                auto model = _entity.get<Entropy::Components::Model>()->model;
                auto pos = _entity.get<Entropy::Components::Position>()->pos;

                // Assuming model->dimensions represents the bounding box dimensions
                float x = pos.x; // Model's position along the x-axis
                float y = pos.y; // Model's position along the y-axis
                float z = pos.z; // Model's position along the z-axis

                // Calculate the bottom center of the model's bounding box
                float bottomCenterX = x;                           // Bottom center along the x-axis
                float bottomCenterY = y - model->dimensions.min.y; // Bottom center along the y-axis
                float bottomCenterZ = z;                           // Bottom center along the z-axis

                // Set the origin of the rigid body's transformation to the bottom center
                boxTransform.setIdentity(); // Set the transform to identity (no translation or rotation by default)
                boxTransform.setOrigin(btVector3(bottomCenterX, -y - (model->dimensions.max.y / 2.0), bottomCenterZ));

                // Create a motion state and attach the transform to it
                btDefaultMotionState *motionState = new btDefaultMotionState(boxTransform);

                body->setMotionState(motionState);
                 */
            }

        private:
            btTransform boxTransform;
            btRigidBody *body;
            flecs::entity _entity;
        };
    }
}

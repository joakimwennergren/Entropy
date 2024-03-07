#pragma once

#include <btBulletDynamicsCommon.h>

namespace Entropy
{
    namespace Components
    {
        struct BoxCollisionShape3D
        {
        public:
            void UpdateMotionState()
            {
                boxShape = new btBoxShape(btVector3(_size.x, _size.y, _size.z));
                box_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(_position.x, _position.y, _position.z)));
            }

            glm::vec3 _size;
            glm::vec3 _position;
            btCollisionShape *boxShape;
            btDefaultMotionState *box_motion_state;
        };
    }
}
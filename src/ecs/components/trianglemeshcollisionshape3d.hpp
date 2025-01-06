#pragma once

#include <btBulletDynamicsCommon.h>

#include <gltf/model.hpp>

namespace Entropy
{
    namespace Components
    {
        struct TriangleMeshCollisionShape3D
        {
        public:
            void Initialize(Entropy::GLTF::Model *model)
            {
                // Create a btTriangleMesh to hold the model's triangle data
                triangleMesh = new btTriangleMesh();

                meshShape = new btConvexHullShape();

                for (auto vertice : model->vertices_temp)
                {
                    btVector3 vert = btVector3(vertice.position.x, vertice.position.y, vertice.position.z);
                    meshShape->addPoint(vert);
                }

                meshShape->optimizeConvexHull();
            }
            btTriangleMesh *triangleMesh = nullptr;
            btDefaultMotionState *motionState = nullptr;
            btConvexHullShape *meshShape = nullptr;
        };

    }
}
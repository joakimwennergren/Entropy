#include "scenegraph.hpp"

using namespace Symbios::Contexts;

/**
 * Static methods should be defined outside the class.
 */

SceneGraph *SceneGraph::pinstance_{nullptr};
std::mutex SceneGraph::mutex_;

/**
 * The first time we call GetInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the value. RU:
 */
SceneGraph *SceneGraph::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SceneGraph();
    }
    return pinstance_;
}
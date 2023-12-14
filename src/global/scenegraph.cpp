#include "scenegraph.hpp"

using namespace Entropy::Global;

SceneGraph *SceneGraph::pinstance_{nullptr};
std::mutex SceneGraph::mutex_;

SceneGraph *SceneGraph::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new SceneGraph();
    }
    return pinstance_;
}
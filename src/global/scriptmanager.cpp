#include "scriptmanager.hpp"

using namespace Entropy::Global;

ScriptManager *ScriptManager::pinstance_{nullptr};
std::mutex ScriptManager::mutex_;

ScriptManager *ScriptManager::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new ScriptManager();
    }
    return pinstance_;
}

void ScriptManager::Bind()
{
}
#include "CrossWindow/CrossWindow.h"

void xmain(int argc, const char **argv)
{
    // 🖼️ Create Window Description
    xwin::WindowDesc windowDesc;
    windowDesc.name = "Test";
    windowDesc.title = "My Title";
    windowDesc.visible = true;
    windowDesc.width = 640;
    windowDesc.backgroundColor = 0xFF00CCFF;
    windowDesc.height = 480;

    bool closed = false;

    // 🌟 Initialize
    xwin::Window window;
    xwin::EventQueue eventQueue;

    if (!window.create(windowDesc, eventQueue))
    {
        return;
    }

    // 🏁 Engine loop
    bool isRunning = true;

    while (isRunning)
    {
        // ♻️ Update the event queue
        eventQueue.update();

        // 🎈 Iterate through that queue:
        while (!eventQueue.empty())
        {
            const xwin::Event &event = eventQueue.front();

            if (event.type == xwin::EventType::MouseInput)
            {
                const xwin::MouseInputData mouse = event.data.mouseInput;
            }
            if (event.type == xwin::EventType::Close)
            {
                window.close();
                isRunning = false;
            }

            eventQueue.pop();
        }
    }
}
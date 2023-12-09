#include <jni.h>
#include <android/log.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_android.h>
#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

// Entropy
#include <global/vulkancontext.hpp>
#include <global/scenegraph.hpp>
#include <graphics/renderers/renderer.hpp>
#include <application/application.hpp>
#include <graphics/primitives/2d/sprite.hpp>
#include <ft2build.h>

using namespace Entropy::Global;
using namespace Entropy::Graphics::Renderers;
using namespace Symbios::Graphics::Primitives;

class Game : public Application
{
public:
private:
    void OnInit() {};
    void OnRender(float deltaTime){};
};

struct ShaderObject
{
    uint32_t* code;
    size_t size;
};

struct SpriteObject
{
    size_t length;
    stbi_uc* content;
    uint32_t width, height;
};

extern "C" {

    #include <game-activity/native_app_glue/android_native_app_glue.c>

    VkInstance vkInstance;
    VkSurfaceKHR  windowSurface;
    ShaderObject vertShader;
    ShaderObject fragShader;
    Game game;
    std::shared_ptr<Sprite> sprite;
    SpriteObject spriteObj;
    std::shared_ptr<Renderer> renderer;

    SpriteObject ReadSprite(std::string path, android_app *app)
    {
        // Read the file:
        AAsset* file = AAssetManager_open(app->activity->assetManager,
                                          path.c_str(), AASSET_MODE_BUFFER);
        size_t fileLength = AAsset_getLength(file);
        stbi_uc* fileContent = new unsigned char[fileLength];
        AAsset_read(file, fileContent, fileLength);
        AAsset_close(file);

        uint32_t imgWidth, imgHeight, n;
        unsigned char* imageData = stbi_load_from_memory(
                fileContent, fileLength, reinterpret_cast<int*>(&imgWidth),
                reinterpret_cast<int*>(&imgHeight), reinterpret_cast<int*>(&n), 4);
        assert(n == 4);

        return SpriteObject {
            .length = fileLength,
            .content = imageData,
            .width = imgWidth,
            .height = imgHeight
        };
    }

    ShaderObject ReadShader(android_app *app, std::string filePath)
    {
        AAsset* file = AAssetManager_open(app->activity->assetManager,
                                          filePath.c_str(), AASSET_MODE_BUFFER);
        size_t fileLength = AAsset_getLength(file);
        char* fileContent = new char[fileLength];
        AAsset_read(file, fileContent, fileLength);
        AAsset_close(file);
        return ShaderObject{.code = (uint32_t * )fileContent, .size = fileLength};
    }

    void CreateVulkanInstance()
    {
        // Enable just the Khronos validation layer.
        static const char *layers[] = {"VK_LAYER_KHRONOS_validation"};

        // Get the layer count using a null pointer as the last parameter.
        uint32_t instance_layer_present_count = 0;
        vkEnumerateInstanceLayerProperties(&instance_layer_present_count, nullptr);

        // Enumerate layers with a valid pointer in the last parameter.
        VkLayerProperties layer_props[instance_layer_present_count];
        vkEnumerateInstanceLayerProperties(&instance_layer_present_count, layer_props);

        // Make sure selected validation layers are available.
        VkLayerProperties *layer_props_end = layer_props + instance_layer_present_count;
        for (const char *layer: layers) {
            assert(layer_props_end !=
                   std::find_if(layer_props, layer_props_end,
                                [layer](VkLayerProperties layerProperties) {
                                    return strcmp(layerProperties.layerName, layer) == 0;
                                }));
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Entropy application";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Entropy";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensionProps(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProps.data());

        std::vector<const char *> extensions;
        for (const auto &extension : extensionProps)
        {
            //extensions.push_back(extension.extensionName);
        }

        extensions.push_back("VK_KHR_surface");
        extensions.push_back("VK_KHR_android_surface");

        createInfo.enabledExtensionCount = (uint32_t)extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        createInfo.enabledLayerCount = sizeof(layers) / sizeof(layers[0]);
        createInfo.ppEnabledLayerNames = layers;

        if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS)
        {
            exit(EXIT_FAILURE);
        }
    }

    void CreateAndroidSurface(android_app *app)
    {
        VkAndroidSurfaceCreateInfoKHR createInfo{
                .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
                .pNext = nullptr,
                .flags = 0,
                .window = app->window};

        vkCreateAndroidSurfaceKHR(vkInstance, &createInfo, nullptr,
                                          &windowSurface);
    }

    /*!
     * Handles commands sent to this Android application
     * @param pApp the app the commands are coming from
     * @param cmd the command to handle
     */
    void handle_cmd(android_app *pApp, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                // A new window is created, associate a renderer with it. You may replace this with a
                // "game" class if that suits your needs. Remember to change all instances of userData
                // if you change the class here as a reinterpret_cast is dangerous this in the
                // android_main function and the APP_CMD_TERM_WINDOW handler case.
                CreateVulkanInstance();
                CreateAndroidSurface(pApp);
                vertShader = ReadShader(pApp, "vert.spv");
                fragShader = ReadShader(pApp, "frag.spv");
                VulkanContext::GetInstance()->_instance = vkInstance;
                VulkanContext::GetInstance()->_windowSurface = windowSurface;
                VulkanContext::GetInstance()->Initialize(vkInstance);
                game = Game();
                renderer = std::make_shared<Renderer>(vertShader.code, vertShader.size, fragShader.code, fragShader.size);
                game._renderer = renderer;
                pApp->userData = &game;
                spriteObj = ReadSprite("logo.png", pApp);
                sprite = std::make_shared<Sprite>(spriteObj.content, spriteObj.width, spriteObj.height);
                SceneGraph::GetInstance()->renderables.push_back(sprite);
                break;
            case APP_CMD_TERM_WINDOW:
                // The window is being destroyed. Use this to clean up your userData to avoid leaking
                // resources.
                //
                // We have to check if userData is assigned just in case this comes in really quickly
                if (pApp->userData) {
                    //
                    //auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);
                    pApp->userData = nullptr;
                    //delete pRenderer;
                }
                break;
            default:
                break;
        }
    }

    /*!
     * Enable the motion events you want to handle; not handled events are
     * passed back to OS for further processing. For this example case,
     * only pointer and joystick devices are enabled.
     *
     * @param motionEvent the newly arrived GameActivityMotionEvent.
     * @return true if the event is from a pointer or joystick device,
     *         false for all other input devices.
     */
    bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
        auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
        return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
                sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
    }

    /*!
     * This the main entry point for a native activity
     */
    void android_main(struct android_app *pApp) {

        // Register an event handler for Android events
        pApp->onAppCmd = handle_cmd;

        // Set input event filters (set it to NULL if the app wants to process all inputs).
        // Note that for key inputs, this example uses the default default_key_filter()
        // implemented in android_native_app_glue.c.
        android_app_set_motion_event_filter(pApp, motion_event_filter_func);

        float y = 800.0;
        // This sets up a typical game/event loop. It will run until the app is destroyed.
        int events;
        android_poll_source *pSource;
        do {
            // Process all pending events before running game logic.
            if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
                if (pSource) {
                    pSource->process(pApp, pSource);
                }
            }

            // Check if any user data is associated. This is assigned in handle_cmd
            if (pApp->userData) {

                // We know that our user data is a Renderer, so reinterpret cast it. If you change your
                // user data remember to change it here
                auto *game = reinterpret_cast<Game *>(pApp->userData);

                // Process game input
                //pRenderer->handleInput();

                // Render a frame
                sprite->SetPosition(550.0, y * -1.0);
                sprite->SetScale(400.0, 400.0);
                game->_renderer->Render();


            }
        } while (!pApp->destroyRequested);
    }
}
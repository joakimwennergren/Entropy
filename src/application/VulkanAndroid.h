/*
* Android Vulkan function pointer prototypes
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#ifndef VULKANANDROID_H
#define VULKANANDROID_H

// Vulkan needs to be loaded dynamically on android

#pragma once

#ifndef VULKANANDROID_HPP
#define VULKANANDROID_HPP

#if defined(__ANDROID__)
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <android/configuration.h>
#include <memory>

#endif

#endif // VULKANANDROID_HPP


#endif // VULKANANDROID_H
 
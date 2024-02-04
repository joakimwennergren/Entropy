/*
* Android Vulkan function pointer loader
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "VulkanAndroid.h"

#if defined(__ANDROID__)
	#include <android/log.h>
	#include <dlfcn.h>

android_app* androidApp;

#endif

//
// Created by Diego S. Seabra on 16/06/22.
//

#ifndef VULKAN_ENGINE_GEFORCENOW_H
#define VULKAN_ENGINE_GEFORCENOW_H

#include <GfnRuntimeSdk_CAPI.h>
#include <GfnRuntimeSdk_Wrapper.h>
#include <GfnSdk.h>
#include <iostream>

class CGeforceNow
{
public:
    static void Init();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_GEFORCENOW_H

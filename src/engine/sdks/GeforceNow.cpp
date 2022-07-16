//
// Created by Diego S. Seabra on 16/06/22.
//

#include "GeforceNow.h"
#include "../profiling/Logger.h"

void GeforceNow::Init()
{
    Logger::Debug("Initializing Geforce Now SDK");

//    GfnRuntimeError err = GfnInitializeSdk(gfnDefaultLanguage);
//    if (GFNSDK_FAILED(err))
//    {
//        // Initialization errors generally indicate a flawed environment. Check error code for details.
//        // See GfnError in GfnSdk.h for error codes.
//        printf("Error initializing the sdk: %d\n", err);
//    }
//    std::cout << "Geforce now init error: " << err << std::endl;
}

void GeforceNow::Shutdown()
{
    Logger::Debug("Shutting down Geforce Now SDK");

    // Shut down the Geforce NOW Runtime SDK. Note that it's safe to call
    // gfnShutdownRuntimeSdk even if the SDK was not initialized.
//    GfnShutdownSdk();
}

//
// Created by Diego S. Seabra on 10/05/22.
//

#ifndef VULKAN_ENGINE_EDITORINTERFACE_H
#define VULKAN_ENGINE_EDITORINTERFACE_H

struct CEditorInterfaceImpl
{
    // Constructor/Destructor
    CEditorInterfaceImpl();
    ~CEditorInterfaceImpl();

    // Auxiliary Methods
    void CreateImGuiContext();
    void BindImGuiToVulkan();
};

class CEditorInterface
{
public:
    static void Init();
    static void Draw();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_EDITORINTERFACE_H

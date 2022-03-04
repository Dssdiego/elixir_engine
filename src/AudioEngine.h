//
// Created by Diego S. Seabra on 03/03/22.
//

#ifndef VULKAN_ENGINE_AUDIOENGINE_H
#define VULKAN_ENGINE_AUDIOENGINE_H

// TODO: Audio Engine implementation example:
//   https://www.youtube.com/watch?v=jY3tPM1oNyU

class AudioEngine
{
public:
    static void Init();
    static void Update();
    static void Shutdown();

    void LoadSound();
    void unLoadSound();
//    void Set3dListenerAndOrientation();
    int PlaySound();
    void StopChannel();
    void PauseChannel();
    void SetChannel3dPosition();
    void SetChannelVolume();
    bool IsPlaying(int channelId); // const ?
};


#endif //VULKAN_ENGINE_AUDIOENGINE_H

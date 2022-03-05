//
// Created by Diego S. Seabra on 03/03/22.
//

#ifndef VULKAN_ENGINE_AUDIOENGINE_H
#define VULKAN_ENGINE_AUDIOENGINE_H

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

struct Vector3
{
    float x;
    float y;
    float z;
};

// using the pimpl idiom
struct Implementation
{
    Implementation();
    ~Implementation();

    void Update();

    FMOD::Studio::System* mStudioSystem;
    FMOD::System* mSystem;

    int mNextChannelId;

    typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;
    typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
    typedef std::map<std::string, FMOD::Sound*> SoundMap;
    typedef std::map<int, FMOD::Channel*> ChannelMap;

    BankMap mBanks;
    EventMap mEvents;
    SoundMap mSounds;
    ChannelMap mChannels;
};

class CAudioEngine
{
public:
    static void Init();
    static void Update();
    static void Shutdown();
    static int ErrorCheck(FMOD_RESULT result);

    void LoadSound(const std::string& sSoundName, bool b3d = false, bool bLooping = false, bool bStream = false);
    void UnLoadSound(const std::string& sSoundName);
    int PlaySoundFile(const std::string& sSoundName, const Vector3& vPosition = Vector3{0,0,0}, float fVolumedB = 0.0f);
    void LoadBank(const std::string& sBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
    void LoadEvent(const std::string& sEventName);
    void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{0,0,0}, float fVolumedB = 0.0f);
    void PlayEvent(const std::string& sEventName);
    void StopChannel(int nChannelId);
    void StopEvent(const std::string& sEventName, bool bImmediate = false);
    void GetEventParameter(const std::string& sEventName, const std::string& sEventParameter, float* fParameter);
    void SetEventParameter(const std::string& sEventName, const std::string& sParameterName, float fValue);
    void StopAllChannels();
    void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
    void SetChannelVolume(int nChannelId, float fVolumedB);
    bool IsPlaying(int nChannelId);
    bool IsEventPlaying(const std::string& sEventName);
    float dbToVolume(float fdB);
    float VolumeTodB(float fVolume);
    FMOD_VECTOR VectorToFmod(const Vector3& vPosition);
};

//#include <fmod_studio.hpp>
//#include <iostream>

//class AudioEngine
//{
//public:
//    void Init();
//    void Update();
//    void Shutdown();
//
//    void LoadSound(std::string fileName);
//    void unLoadSound();
////    void Set3dListenerAndOrientation();
//    int PlaySound();
//    void StopChannel();
//    void PauseChannel();
//    void SetChannel3dPosition();
//    void SetChannelVolume();
//    bool IsPlaying(int channelId); // const ?
//
//private:
//    FMOD::Studio::System* system = nullptr;
//};


#endif //VULKAN_ENGINE_AUDIOENGINE_H

//
// Created by Diego S. Seabra on 03/03/22.
//

#include "AudioEngine.h"

Implementation* mImplementation = nullptr;

Implementation::Implementation()
{
    mStudioSystem = nullptr;
    CAudioEngine::ErrorCheck(FMOD::Studio::System::create(&mStudioSystem));
    CAudioEngine::ErrorCheck(mStudioSystem->initialize(32,
                                                       FMOD_STUDIO_INIT_LIVEUPDATE,
                                                       FMOD_INIT_PROFILE_ENABLE,
                                                       nullptr));

    mSystem = nullptr;
    CAudioEngine::ErrorCheck(mStudioSystem->getCoreSystem(&mSystem));
}

Implementation::~Implementation()
{
    CAudioEngine::ErrorCheck(mStudioSystem->unloadAll());
    CAudioEngine::ErrorCheck(mStudioSystem->release());
}

void Implementation::Update()
{
    // check the current sound to see if any sounds are stopped
    std::vector<ChannelMap::iterator> vStoppedChannels;
    for(auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
    {
        bool bIsPlaying = false;
        it->second->isPlaying(&bIsPlaying);
        if (!bIsPlaying)
        {
            vStoppedChannels.push_back(it);
        }
    }

    // erase the stopped sound from our checking
    for (auto& it : vStoppedChannels)
    {
        mChannels.erase(it);
    }

    // update the fmod system object
    CAudioEngine::ErrorCheck(mStudioSystem->update());
}

void CAudioEngine::Init()
{
    mImplementation = new Implementation;
}

void CAudioEngine::Update()
{
    mImplementation->Update();
}

void CAudioEngine::Shutdown()
{
    delete mImplementation;
}

int CAudioEngine::ErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        throw std::runtime_error(&"FMOD ERROR: " [result]);
    }

    return 0;
}

void CAudioEngine::LoadSound(const std::string &sSoundName, bool b3d, bool bLooping, bool bStream)
{
    // check if the sound is loaded
    auto tFoundIt = mImplementation->mSounds.find(sSoundName);
    if (tFoundIt != mImplementation->mSounds.end())
        return;

    FMOD_MODE eMode = FMOD_DEFAULT;
    eMode |= b3d ? FMOD_3D : FMOD_2D;
    eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

    // load the sound
    FMOD::Sound* sound = nullptr;
    CAudioEngine::ErrorCheck(mImplementation->mSystem->createSound(sSoundName.c_str(), eMode, nullptr, &sound));
    if (sound)
    {
        mImplementation->mSounds[sSoundName] = sound;
    }
}

void CAudioEngine::UnLoadSound(const std::string &sSoundName)
{
    // check if the sound is loaded
    auto foundIt = mImplementation->mSounds.find(sSoundName);
    if (foundIt == mImplementation->mSounds.end())
        return;

    // unload the sound
    CAudioEngine::ErrorCheck(foundIt->second->release());
    mImplementation->mSounds.erase(foundIt);
}

int CAudioEngine::PlaySoundFile(const std::string &sSoundName, const Vector3 &vPosition, float fVolumedB)
{
    // check if the sound is loaded
    int channelId = mImplementation->mNextChannelId++;
    auto foundIt = mImplementation->mSounds.find(sSoundName);

    // the sound is not loaded, so we load it
    if (foundIt == mImplementation->mSounds.end())
    {
        LoadSound(sSoundName, true);
        foundIt = mImplementation->mSounds.find(sSoundName);
        if (foundIt == mImplementation->mSounds.end())
        {
            return channelId;
        }
    }

    // play the sound in a new created channel
    FMOD::Channel* channel = nullptr;
    CAudioEngine::ErrorCheck(mImplementation->mSystem->playSound(foundIt->second, nullptr, true, &channel));
    if (channel)
    {
        FMOD_MODE currMode;
        foundIt->second->getMode(&currMode);

        // if the sound is in 3d space, we set its 3d attributes in fmod
        if (currMode & FMOD_3D)
        {
            FMOD_VECTOR fmodVector = VectorToFmod(vPosition);
            CAudioEngine::ErrorCheck(channel->set3DAttributes(&fmodVector, nullptr));
        }
        CAudioEngine::ErrorCheck(channel->setVolume(dbToVolume(fVolumedB)));
        CAudioEngine::ErrorCheck(channel->setPaused(false));

        mImplementation->mChannels[channelId] = channel;
    }

    return channelId;
}

float CAudioEngine::dbToVolume(float fdB)
{
    return powf(10.0f, 0.05f * fdB);
}

float CAudioEngine::VolumeTodB(float fVolume)
{
    return 20.0f * log10f(fVolume);
}

// TODO: continuar aqui: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/
FMOD_VECTOR CAudioEngine::VectorToFmod(const Vector3 &vPosition)
{
//    FMOD_VECTOR fmodVector{
//        vPosition.x,
//        vPosition.y,
//        vPosition.z,
//    };
//    fmodVector.x = vPosition.x;
//    fmodVector.y = vPosition.y;
//    fmodVector.z = vPosition.z;
    return FMOD_VECTOR{ vPosition.x, vPosition.y, vPosition.z };
}

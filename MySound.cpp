
#include "MySound.h"

#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "stdafx.h"

MySoundEngine::MySoundEngine() : initialized(false), soundFileNameBuffer()
{
    dprintf(MY_DEBUG_INFO, "Initialize OpenAL\n");

    ALboolean ret = alutInit(0, 0);
    int err = alGetError();
    if (ret == AL_TRUE && err == 0)
    {
        printf("OpenAL initialized successfully\n");
    }
    else
    {
        printf("OpenAL initialize failed ret %d (err: %d)\n", ret, err);
    }
    
    alGetError();
     
    initialized = true;
    
    listenerPos = irr::core::vector3df(0.f,0.f,0.f);
    listenerVel = irr::core::vector3df(0.f,0.f,0.f);
    listenerDir = irr::core::vector3df(0.f,0.f,-1.f);
    listenerUp = irr::core::vector3df(0.f,1.f,0.f);;

    updateListener();
}

MySoundEngine::~MySoundEngine()
{
    dprintf(MY_DEBUG_INFO, "free up OpenAL: %lu buffers in use\n", soundFileNameBuffer.size());
    for (bufferList_t::iterator it = soundFileNameBuffer.begin();
        it != soundFileNameBuffer.end();
        it++)
    {
        dprintf(MY_DEBUG_INFO, "free up %s\n", it->first);
        alDeleteBuffers(1, &it->second);
    }
    soundFileNameBuffer.clear();

//#ifdef __linux__
    //alutExit();
//#else
//    alcMakeContextCurrent(NULL);
//    alcDestroyContext(pContext);
//    alcCloseDevice(pDevice);
//#endif
}
    
MySound* MySoundEngine::play3D(const std::string& soundFileName, irr::core::vector3df pos, bool playLooped,
                               bool startPaused, bool track)
{
    if (this==0 || !initialized) return 0;
    dprintf(MY_DEBUG_NOTE, "AL: play3D [%s], looped: %s, start paused: %s\n",
          soundFileName.c_str(), playLooped?"yes":"no", startPaused?"yes":"no");
    int i = 0;
    bufferList_t::iterator it = soundFileNameBuffer.find(soundFileName);
    
    unsigned int buffer;
    if (it == soundFileNameBuffer.end())
    {
        dprintf(MY_DEBUG_NOTE, "AL: buffer not found\n");

        alGenBuffers(1, &buffer);
        if (alGetError() != AL_NO_ERROR)
        {
            dprintf(MY_DEBUG_NOTE, "AL: could not create buffer\n");
            return 0;
        }
        
//        ALenum format;
//        ALsizei size;
//        ALvoid* data;
//        ALsizei freq;
//        ALboolean loop;
        buffer = alutCreateBufferFromFile(soundFileName.c_str());
//        alutLoadWAVFile(newElem->filename, &format, &data, &size, &freq, &loop);
//        alBufferData(newElem->buffer, format, data, size, freq);
//        alutUnloadWAV(format, data, size, freq);
        alGetError();

        soundFileNameBuffer[soundFileName] = buffer;
    }
    else
    {
        buffer = it->second;
        dprintf(MY_DEBUG_NOTE, "AL: buffer found %d\n");
    }
    
    MySound* sound = new MySound(buffer, playLooped, pos);
    
    if(!startPaused) sound->setIsPaused(false);
    
    return sound;
}

MySound* MySoundEngine::play2D(const std::string& soundFileName, bool playLooped,
                               bool startPaused, bool track)
{
    return play3D(soundFileName, listenerPos, playLooped, startPaused, track);
}

void MySoundEngine::setListenerPosition(const irr::core::vector3df &pos, const irr::core::vector3df &lookdir,
                                        const irr::core::vector3df &velPerSecond,
                                        const irr::core::vector3df &upVector)
{
    listenerPos = pos;
    listenerVel = velPerSecond;
    listenerDir = lookdir;
    listenerUp = upVector;
    
    updateListener();
}

irr::core::vector3df MySoundEngine::getListenerPosition() const
{
    return listenerPos;
}
        
void MySoundEngine::updateListener()
{
    ALfloat ListenerOri[] = { listenerDir.X, listenerDir.Y, listenerDir.Z, listenerUp.X, listenerUp.Y, listenerUp.Z};
    alListenerfv(AL_POSITION,    &listenerPos.X);
    alListenerfv(AL_VELOCITY,    &listenerVel.X);
    alListenerfv(AL_ORIENTATION, ListenerOri);    
    alGetError();
}


MySound::MySound(unsigned int newBuffer, bool plooped, irr::core::vector3df &pos) : 
        initialized(false), paused(true), looped(plooped), maxDistance(1000.0f), minDistance(1.0f), volume(1.0f),
        playbackSpeed(1.0f), buffer(newBuffer), isStopped(true), soundSourcePos(pos)
        
{
    alGenSources(1, &source);
    
    int err = alGetError();
    if (err != AL_NO_ERROR)
    {
        dprintf(MY_DEBUG_INFO, "create sound failed: %d\n", err);
        return;
    }

    dprintf(MY_DEBUG_NOTE, "create sound %u\n", source);

    soundSourceVel = irr::core::vector3df(0.f,0.f,0.f);
    
    initialized = true;
    alSourcei (source, AL_BUFFER,   buffer   );
    alSourcef (source, AL_PITCH,    playbackSpeed     );
    alSourcef (source, AL_GAIN,     volume     );
    alSourcefv(source, AL_POSITION, &soundSourcePos.X);
    alSourcefv(source, AL_VELOCITY, &soundSourceVel.X);
    alSourcei (source, AL_LOOPING,  looped     );
    alGetError();
}

MySound::~MySound()
{
    if (!this || !initialized) return;
    
    stop();
    dprintf(MY_DEBUG_NOTE, "destroy sound %u\n", source);
    alDeleteSources(1, &source);
    int err = alGetError();
    if (err != AL_NO_ERROR)
    {
        dprintf(MY_DEBUG_ERROR, "destroy sound failed: %d\n", err);
    }
}

void MySound::setVolume(float newVolume)
{
    if (!this || !initialized) return;
    volume = newVolume;
    alSourcef (source, AL_GAIN,     volume     );
    alGetError();
}

float MySound::getVolume() const
{
    if (!this || !initialized) return 0.f;
    return volume;
}

void MySound::setPlaybackSpeed(float newPlaybackSpeed)
{
    if (!this || !initialized) return;
    playbackSpeed = newPlaybackSpeed;
    alSourcef (source, AL_PITCH,    playbackSpeed     );
    alGetError();
}

float MySound::getPlaybackSpeed() const
{
    if (!this || !initialized) return 0.f;
    return playbackSpeed;
}

void MySound::setIsLooped(bool newLoop)
{
    if (!this || !initialized) return;
    looped = newLoop;
    alSourcei (source, AL_LOOPING,  looped     );
    alGetError();
}

bool MySound::isLooped() const
{
    if (!this || !initialized) return false;
    return looped;
}

void MySound::setPosition(irr::core::vector3df &newPos)
{
    if (!this || !initialized) return;
    soundSourcePos = newPos;
    alSourcefv(source, AL_POSITION, &soundSourcePos.X);
    alGetError();
}

void MySound::setPosition(irr::core::vector3df newPos)
{
    if (!this || !initialized) return;
    soundSourcePos = newPos;
    alSourcefv(source, AL_POSITION, &soundSourcePos.X);
    alGetError();
}

irr::core::vector3df MySound::getPosition() const
{
    if (!this || !initialized) return irr::core::vector3df();
    return soundSourcePos;
}

void MySound::setVelocity(irr::core::vector3df &newVelocity)
{
    if (!this || !initialized) return;
    soundSourceVel = newVelocity;
    alSourcefv(source, AL_VELOCITY, &soundSourceVel.X);
    alGetError();
}

irr::core::vector3df MySound::getVelocity() const
{
    if (!this || !initialized) return irr::core::vector3df();
    return soundSourceVel;
}

void MySound::setMaxDistance(float newMaxDistance)
{
    if (!this || !initialized) return;
    maxDistance = newMaxDistance;
    alSourcef (source, AL_MAX_DISTANCE, maxDistance);
}

float MySound::getMaxDistance() const
{
    if (!this || !initialized) return 0.f;
    return maxDistance;
}

void MySound::setMinDistance(float newMinDistance)
{
    if (!this || !initialized) return;
    minDistance = newMinDistance;
    alSourcef (source, AL_REFERENCE_DISTANCE, minDistance);
}

float MySound::getMinDistance() const
{
    if (!this || !initialized) return 0.f;
    return minDistance;
}


void MySound::play()
{
    if (!this || !initialized) return;

    dprintf(MY_DEBUG_NOTE, "play sound %u\n", source);
    
    isStopped = false;
//    paused = false;
    alSourcePlay(source);    
    alGetError();
}

void MySound::stop()
{
    if (!this || !initialized) return;

    dprintf(MY_DEBUG_NOTE, "stop sound %u\n", source);
    
    isStopped = true;
    alSourceStop(source);
    alGetError();
}

void MySound::setIsPaused(bool newPaused)
{
    if (!this || !initialized) return;

#ifdef MY_SOUND_DEBUG
//    printf("pause sound %u %d -> %d\n", source, paused, newPaused);
#endif

    if (paused == newPaused) return;
    
    paused = newPaused;
    
    if (paused == false)
        alSourcePlay(source);    
    else
        alSourcePause(source);

    alGetError();
}

bool MySound::isPaused() const
{
    if (!this || !initialized) return false;
    return paused;
}
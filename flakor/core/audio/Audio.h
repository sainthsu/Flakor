#ifndef _FK_AUDIO_H_
#define _FK_AUDIO_H_

#include "base/interface/IAudio.h"
#include <string>

FLAKOR_NS_BEGIN

enum class AudioState
{
    ERROR  = -1,
    INITIALZING,
    PLAYING,
    PAUSED,
    RELEASE
};

class ProfileHelper;

class Audio : public IAudio
{
public:
    static const float TIME_UNKNOWN;
    
    Audio();
    ~Audio();
    
protected:
    const std::string * filePath;
    ProfileHelper* profileHelper;
    
    float _leftVolume;
    float _rightVolume;
    
    bool _mute;
    bool loop;
    float duration;
    AudioState state;
    
    bool is3dAudio;
    
    virtual float getVolume() override;
    virtual void setVolume(float volume) override;
    virtual void setVolume(float leftVolume, float rightVolume) override;
    
    virtual float getLeftVolume() override;
    virtual float getRightVolume() override;
    
};


FLAKOR_NS_END

#endif // AUDIO_H

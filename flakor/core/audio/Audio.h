#ifndef _FK_AUDIO_H_
#define _FK_AUDIO_H_

#include "base/interface/IAudio.h"

FLAKOR_NS_BEGIN

class Audio : public IAudio
{
public:
    Audio();
    ~Audio();
    
protected:
    float _leftVolume;
    float _rightVolume;
    
    bool _mute;
    bool _release;
    
    virtual float getVolume() override;
    virtual void setVolume(float volume) override;
    virtual void setVolume(float leftVolume, float rightVolume) override;
    
    virtual float getLeftVolume() override;
    virtual float getRightVolume() override;
    
};


FLAKOR_NS_END

#endif // AUDIO_H

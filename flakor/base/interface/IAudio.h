#ifndef _FK_IAUDIO_H_
#define _FK_IAUDIO_H_

FLAKOR_NS_BEGIN

class IAudio
{
public:
    virtual void prepare() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual void release() = 0;
	
    virtual float getVolume() = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setVolume(float leftVolume, float rightVolume) = 0;
	
    virtual float getLeftVolume() = 0;
    virtual float getRightVolume() = 0;
	

    virtual void onMasterVolumeChanged(float masterVolume) = 0;

    virtual void setLooping(bool looping) = 0;
	
}

FLAKOR_NS_END

#endif

#include "Audio.h"

FLAKOR_NS_BEGIN

Audio::Audio()
:_leftVolume(0)
,_rightVolume(0)
,_mute(false)
,_release(false)
{
}

Audio::~Audio()
{
    _leftVolume = _rightVolume = 0;
}

float Audio::getVolume()
{
    return (_leftVolume+_rightVolume)*0.5;
}

void Audio::setVolume(float volume)
{
    _leftVolume = _rightVolume = volume;
}

void Audio::setVolume(float leftVolume, float rightVolume)
{
    _leftVolume = leftVolume;
    _rightVolume = rightVolume;
}

float Audio::getLeftVolume()
{
    return _leftVolume;
}

float Audio::getRightVolume()
{
    return _rightVolume;
}

FLAKOR_NS_END
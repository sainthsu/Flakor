#include "Audio.h"

FLAKOR_NS_BEGIN

const float Audio::TIME_UNKNOWN = -1.0f;

Audio::Audio()
:profileHelper(NULL)
, duration(TIME_UNKNOWN)
, state(AudioState::INITIALZING)
,_leftVolume(0)
,_rightVolume(0)
,_mute(false)
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
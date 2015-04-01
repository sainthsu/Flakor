#ifndef _FK_SOUND_H_
#define _FK_SOUND_H_

#include "base/interface/IAudio.h"

FLAKOR_NS_BEGIN

class Sound : public IAudio
{
public:
  Sound();
  ~Sound();

protected:
  float leftVolume;
  float rightVolume;

  AudioState state;
  bool loop;

  int soundID;
  int loopCount;
};

FLAKOR_NS_END

#endif // SOUND_H

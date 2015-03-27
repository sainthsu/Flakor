#ifndef _FK_SOUND_H_
#define _FK_SOUND_H_

#include "base/interface/IAudio.h"

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

#endif // SOUND_H

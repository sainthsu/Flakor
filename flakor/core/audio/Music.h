#ifndef _FK_MUSIC_H_
#define _FK_MUSIC_H_

#include "base/interface/IAudio.h"

class Music : public IAudio
{
public:
  Music();
  ~Music();

  void play();

protected:
  float volume;
  bool loop;
};

#endif // MUSIC_H

#ifndef _FK_MUSIC_H_
#define _FK_MUSIC_H_

#include "core/audio/Audio.h"

FLAKOR_NS_BEGIN

class Music : public Audio
{
public:
  Music();
  ~Music();

  void play();

protected:

};

FLAKOR_NS_END

#endif // MUSIC_H

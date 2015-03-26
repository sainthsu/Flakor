#ifndef _FK_MUSIC_H_
#define _FK_MUSIC_H_


class Music
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

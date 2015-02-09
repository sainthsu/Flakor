#ifndef _FK_IGAME_H_
#define _FK_IGAME_H_

//game interface that the engine 
FLAKOR_NS_BEGIN

class IGame
{
public:

	/** Called when the {@link Application} is first created. */
	virtual void create () = 0;

	/** Called when the {@link Application} is resized. This can happen at any point during a non-paused state but will never happen
	 * before a call to {@link #create()}.
	 * 
	 * @param width the new width in pixels
	 * @param height the new height in pixels */
	virtual void resize (int width, int height) = 0;

	virtual void update (float delta) = 0;
	/** Called when the {@link Application} should render itself. */
	virtual void render () = 0;

	/** Called when the {@link Application} is paused, usually when it's not active or visible on screen. An Application is also
	 * paused before it is destroyed. */
	virtual void pause () = 0;

	/** Called when the {@link Application} is resumed from a paused state, usually when it regains focus. */
	virtual void resume () = 0;

	/** Called when the {@link Application} is destroyed. Preceded by a call to {@link #pause()}. */
	virtual void dispose () = 0;

};

FLAKOR_NS_END

#endif

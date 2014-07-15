#ifdef _FK_CONFIG_H_
#define _FK_CONFIG_H_

typedef enum _screenOrientation
{
	/** The app will be fixed in its default Landscape mode. */
    LANDSCAPE_FIXED,
    /** The app will automatically rotate between the Landscape modes, 
	depending on the orientation of the device. */
    LANDSCAPE_SENSOR,
    /** The app will be fixed in its default Portrait mode. */
    PORTRAIT_FIXED,
    /** The app will automatically rotate between the Portrait modes, 
	depending on the orientation of the device. */
    PORTRAIT_SENSOR,
}ScreenOrientation;

class AudioConfig
{
	protected:
		bool musicOn;
		bool soundOn;
		int maxSimultaneousStreams;
	public:
		bool isMusicOn();
		void setMusicOn();
		bool isSoundOn();
		void setSoundOn();
		int getMaxSimultaneousStreams();
		void setMaxSimultaneousStreams(int maxSimultaneousStreams);
}

class InputConfig
{
	
}

class EngineConfig
{
	
}

#endif

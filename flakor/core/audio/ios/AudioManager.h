/*
 Copyright (c) 2010 Steve Oldmeadow
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 $Id$
 */

#import "FlakorAudio.h"
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
    #import <AVFoundation/AVFoundation.h>
#else
    #error "must bigger than iOS 3.0"
#endif

/** Different modes of the engine */
typedef enum {
    kAMM_FxOnly,                    //!Other apps will be able to play audio
    kAMM_FxPlusMusic,                //!Only this app will play audio
    kAMM_FxPlusMusicIfNoOtherAudio,    //!If another app is playing audio at start up then allow it to continue and don't play music
    kAMM_MediaPlayback,                //!This app takes over audio e.g music player app
    kAMM_PlayAndRecord                //!App takes over audio and has input and output
} tAudioManagerMode;

/** Possible states of the engine */
typedef enum {
    kAMStateUninitialised, //!Audio manager has not been initialised - do not use
    kAMStateInitialising,  //!Audio manager is in the process of initialising - do not use
    kAMStateInitialised       //!Audio manager is initialised - safe to use
} tAudioManagerState;

typedef enum {
    kAMRBDoNothing,                //Audio manager will not do anything on resign or becoming active
    kAMRBStopPlay,                //Background music is stopped on resign and resumed on become active
    kAMRBStop                    //Background music is stopped on resign but not resumed - maybe because you want to do this from within your game
} tAudioManagerResignBehavior;

/** Notifications */
extern NSString * const kFAN_AudioManagerInitialised;

@interface AsynchInitialiser : NSOperation {}
@end

/** AudioManager supports two long audio source channels called left and right*/
typedef enum {
    kASC_Left = 0,
    kASC_Right = 1
} tAudioSourceChannel;    

typedef enum {
    kLAS_Init,
    kLAS_Loaded,
    kLAS_Playing,
    kLAS_Paused,
    kLAS_Stopped,
} tLongAudioSourceState;

@class LongAudioSource;
@protocol LongAudioSourceDelegate <NSObject>
@optional
/** The audio source completed playing */
- (void) audioSourceDidFinishPlaying:(LongAudioSource *) audioSource;
/** The file used to load the audio source has changed */
- (void) audioSourceFileDidChange:(LongAudioSource *) audioSource;
@end

/**
 LongAudioSource represents an audio source that has a long duration which makes
 it costly to load into memory for playback as an effect using AudioEngine. Examples
 include background music and narration tracks. The audio file may or may not be compressed.
 Bear in mind that current iDevices can only use hardware to decode a single compressed
 audio file at a time and playing multiple compressed files will result in a performance drop
 as software decompression will take place.
 @since v0.99
 */
@interface LongAudioSource : NSObject <AVAudioPlayerDelegate, AudioInterruptProtocol>{
    AVAudioPlayer    *audioSourcePlayer;
    NSString        *audioSourceFilePath;
    NSInteger        numberOfLoops;
    float            volume;
    id<LongAudioSourceDelegate> delegate; 
    BOOL            mute;
    BOOL            enabled_;
    BOOL            backgroundMusic;
    // whether background music is paused
    BOOL            paused;
    BOOL            stopped;
@public    
    BOOL            systemPaused;//Used for auto resign handling
    NSTimeInterval    systemPauseLocation;//Used for auto resign handling
@protected
    tLongAudioSourceState state;
}    
@property (readonly) AVAudioPlayer *audioSourcePlayer;
@property (readonly) NSString *audioSourceFilePath;
@property (readwrite, nonatomic) NSInteger numberOfLoops;
@property (readwrite, nonatomic) float volume;
@property (assign) id<LongAudioSourceDelegate> delegate;
/* This long audio source functions as background music */
@property (readwrite, nonatomic) BOOL backgroundMusic;
@property (readonly) BOOL paused;

/** Loads the file into the audio source */
-(void) load:(NSString*) filePath;
/** Plays the audio source */
-(void) play;
/** Stops playing the audio soruce */
-(void) stop;
/** Pauses the audio source */
-(void) pause;
/** Rewinds the audio source */
-(void) rewind;
/** Resumes playing the audio source if it was paused */
-(void) resume;
/** Returns whether or not the audio source is playing */
-(BOOL) isPlaying;

@end

/** 
 AudioManager manages audio requirements for a game.  It provides access to a AudioEngine object
 for playing sound effects.  It provides access to two LongAudioSource object (left and right channel)
 for playing long duration audio such as background music and narration tracks.  Additionally it manages
 the audio session to take care of things like audio session interruption and interacting with the audio
 of other apps that are running on the device.
 
 Requirements:
 - Firmware: OS 2.2 or greater 
 - Files: AudioManager.*, FlakorAudio.*
 - Frameworks: OpenAL, AudioToolbox, AVFoundation
 @since v0.8
 */
@interface AudioManager : NSObject <LongAudioSourceDelegate, AudioInterruptProtocol, AVAudioSessionDelegate> {
    AudioEngine        *soundEngine;
    LongAudioSource    *backgroundMusic;
    NSMutableArray        *audioSourceChannels;
    NSString*            _audioSessionCategory;
    BOOL                _audioWasPlayingAtStartup;
    tAudioManagerMode    _mode;
    SEL backgroundMusicCompletionSelector;
    id backgroundMusicCompletionListener;
    BOOL willPlayBackgroundMusic;
    BOOL _mute;
    BOOL _resigned;
    BOOL _interrupted;
    BOOL _audioSessionActive;
    BOOL enabled_;
    
    //For handling resign/become active
    BOOL _isObservingAppEvents;
    tAudioManagerResignBehavior _resignBehavior;
}

@property (readonly) AudioEngine *soundEngine;
@property (readonly) LongAudioSource *backgroundMusic;
@property (readonly) BOOL willPlayBackgroundMusic;

/** Returns the shared singleton */
+ (AudioManager *) sharedManager;
+ (tAudioManagerState) sharedManagerState;
/** Configures the shared singleton with a mode*/
+ (void) configure: (tAudioManagerMode) mode;
/** Initializes the engine asynchronously with a mode */
+ (void) initAsynchronously: (tAudioManagerMode) mode;
/** Initializes the engine synchronously with a mode, channel definition and a total number of channels */
- (id) init: (tAudioManagerMode) mode;
-(void) audioSessionInterrupted;
-(void) audioSessionResumed;
-(void) setResignBehavior:(tAudioManagerResignBehavior) resignBehavior autoHandle:(BOOL) autoHandle;
/** Returns true is audio is muted at a hardware level e.g user has ringer switch set to off */
-(BOOL) isDeviceMuted;
/** Returns true if another app is playing audio such as the iPod music player */
-(BOOL) isOtherAudioPlaying;
/** Sets the way the audio manager interacts with the operating system such as whether it shares output with other apps or obeys the mute switch */
-(void) setMode:(tAudioManagerMode) mode;
/** Shuts down the shared audio manager instance so that it can be reinitialised */
+(void) end;

/** Call if you want to use built in resign behavior but need to do some additional audio processing on resign active. */
- (void) applicationWillResignActive;
/** Call if you want to use built in resign behavior but need to do some additional audio processing on become active. */
- (void) applicationDidBecomeActive;

//New AVAudioPlayer API
/** Loads the data from the specified file path to the channel's audio source */
-(LongAudioSource*) audioSourceLoad:(NSString*) filePath channel:(tAudioSourceChannel) channel;
/** Retrieves the audio source for the specified channel */
-(LongAudioSource*) audioSourceForChannel:(tAudioSourceChannel) channel;

//Legacy AVAudioPlayer API
/** Plays music in background. The music can be looped or not
 It is recommended to use .aac files as background music since they are decoded by the device (hardware).
 */
-(void) playBackgroundMusic:(NSString*) filePath loop:(BOOL) loop;
/** Preloads a background music */
-(void) preloadBackgroundMusic:(NSString*) filePath;
/** Stops playing the background music */
-(void) stopBackgroundMusic;
/** Pauses the background music */
-(void) pauseBackgroundMusic;
/** Rewinds the background music */
-(void) rewindBackgroundMusic;
/** Resumes playing the background music */
-(void) resumeBackgroundMusic;
/** Returns whether or not the background music is playing */
-(BOOL) isBackgroundMusicPlaying;

-(void) setBackgroundMusicCompletionListener:(id) listener selector:(SEL) selector;

@end

/** Fader for long audio source objects */
@interface LongAudioSourceFader : PropertyModifier{}
@end

static const int kFANoBuffer = -1;

/** Allows buffers to be associated with file names */
@interface BufferManager:NSObject{
    NSMutableDictionary* loadedBuffers;
    NSMutableArray    *freedBuffers;
    AudioEngine *soundEngine;
    int nextBufferId;
}

-(id) initWithEngine:(AudioEngine *) theSoundEngine;
-(int) bufferForFile:(NSString*) filePath create:(BOOL) create;
-(void) releaseBufferForFile:(NSString *) filePath;

@end


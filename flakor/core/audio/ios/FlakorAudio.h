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



/** 
@file
@b IMPORTANT
There are 3 different ways of using CocosDenshion. Depending on which you choose you 
will need to include different files and frameworks.

@par SimpleAudioEngine
This is recommended for basic audio requirements. If you just want to play some sound fx
and some background music and have no interest in learning the lower level workings then
this is the interface to use.

Requirements:
 - Firmware: OS 2.2 or greater 
 - Files: SimpleAudioEngine.*, CocosDenshion.*
 - Frameworks: OpenAL, AudioToolbox, AVFoundation
 
@par CDAudioManager
CDAudioManager is basically a thin wrapper around an AVAudioPlayer object used for playing
background music and a CDSoundEngine object used for playing sound effects. It manages the
audio session for you deals with audio session interruption. It is fairly low level and it
is expected you have some understanding of the underlying technologies. For example, for 
many use cases regarding background music it is expected you will work directly with the
backgroundMusic AVAudioPlayer which is exposed as a property.
 
Requirements:
  - Firmware: OS 2.2 or greater 
  - Files: CDAudioManager.*, CocosDenshion.*
  - Frameworks: OpenAL, AudioToolbox, AVFoundation

@par CDSoundEngine
CDSoundEngine is a sound engine built upon OpenAL and derived from Apple's oalTouch 
example. It can playback up to 32 sounds simultaneously with control over pitch, pan
and gain.  It can be set up to handle audio session interruption automatically.  You 
may decide to use CDSoundEngine directly instead of CDAudioManager or SimpleAudioEngine
because you require OS 2.0 compatibility.
 
Requirements:
  - Firmware: OS 2.0 or greater 
  - Files: CocosDenshion.*
  - Frameworks: OpenAL, AudioToolbox
 
*/ 

#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <Foundation/Foundation.h>
#import "AudioConfig.h"


#if !defined(FA_DEBUG) || FA_DEBUG == 0
#define FALOG(...) do {} while (0)
#define FALOGINFO(...) do {} while (0)

#elif FA_DEBUG == 1
#define FALOG(...) NSLog(__VA_ARGS__)
#define FALOGINFO(...) do {} while (0)

#elif FA_DEBUG > 1
#define FALOG(...) NSLog(__VA_ARGS__)
#define FALOGINFO(...) NSLog(__VA_ARGS__)
#endif // FA_DEBUG


#import "OpenALSupport.h"

//Tested source limit on 2.2.1 and 3.1.2 with up to 128 sources and appears to work. Older OS versions e.g 2.2 may support only 32
#define FA_SOURCE_LIMIT 32 //Total number of sources we will ever want, may actually get less
#define FA_NO_SOURCE 0xFEEDFAC //Return value indicating playback failed i.e. no source
#define FA_IGNORE_AUDIO_SESSION 0xBEEFBEE //Used internally to indicate audio session will not be handled
#define FA_MUTE      0xFEEDBAB //Return value indicating sound engine is muted or non functioning
#define FA_NO_SOUND = -1;

#define FA_SAMPLE_RATE_HIGH 44100
#define FA_SAMPLE_RATE_MID  22050
#define FA_SAMPLE_RATE_LOW  16000
#define FA_SAMPLE_RATE_BASIC 8000
#define FA_SAMPLE_RATE_DEFAULT 44100

extern NSString * const kFAN_BadAlContext;
extern NSString * const kFAN_AsynchLoadComplete;

extern float const kFA_PitchDefault;
extern float const kFA_PitchLowerOneOctave;
extern float const kFA_PitchHigherOneOctave;
extern float const kFA_PanDefault;
extern float const kFA_PanFullLeft;
extern float const kFA_PanFullRight;
extern float const kFA_GainDefault;

enum bufferState {
    FA_BS_EMPTY = 0,
    FA_BS_LOADED = 1,
    FA_BS_FAILED = 2
};

typedef struct _sourceGroup {
    int startIndex;
    int currentIndex;
    int totalSources;
    bool enabled;
    bool nonInterruptible;
    int *sourceStatuses;//pointer into array of source status information
} sourceGroup;

typedef struct _bufferInfo {
    ALuint bufferId;
    int bufferState;
    void* bufferData;
    ALenum format;
    ALsizei sizeInBytes;
    ALsizei frequencyInHertz;
} bufferInfo;    

typedef struct _sourceInfo {
    bool usable;
    ALuint sourceId;
    ALuint attachedBufferId;
} sourceInfo;    

#pragma mark AudioTransportProtocol

@protocol AudioTransportProtocol <NSObject>
/** Play the audio */
-(BOOL) play;
/** Pause the audio, retain resources */
-(BOOL) pause;
/** Stop the audio, release resources */
-(BOOL) stop;
/** Return playback to beginning */
-(BOOL) rewind;
@end

#pragma mark AudioInterruptProtocol

@protocol AudioInterruptProtocol <NSObject>
/** Is audio mute */
-(BOOL) mute;
/** If YES then audio is silenced but not stopped, calls to start new audio will proceed but silently */
-(void) setMute:(BOOL) muteValue;
/** Is audio enabled */
-(BOOL) enabled;
/** If NO then all audio is stopped and any calls to start new audio will be ignored */
-(void) setEnabled:(BOOL) enabledValue;
@end

#pragma mark FAUtilities
/**
 Collection of utilities required by FlakorAudio
 */
@interface FAUtilities : NSObject
{
}    

/** Fundamentally the same as the corresponding method is FileUtils */
+(NSString*) fullPathFromRelativePath:(NSString*) relPath;

@end


#pragma mark SoundEngine

/** AudioEngine is built upon OpenAL and works with SDK 2.0.
 AudioEngine is a sound engine built upon OpenAL and derived from Apple's oalTouch
 example. It can playback up to 32 sounds simultaneously with control over pitch, pan
 and gain.  It can be set up to handle audio session interruption automatically.  You 
 may decide to use CDSoundEngine directly instead of CDAudioManager or SimpleAudioEngine
 because you require OS 2.0 compatibility.
 
 Requirements:
 - Firmware: OS 2.0 or greater 
 - Files: FlakorAudio.*
 - Frameworks: OpenAL, AudioToolbox
 
 @since v0.8
 */
@class AudioSource;
@interface AudioEngine : NSObject <AudioInterruptProtocol> {
    
    bufferInfo        *_buffers;
    sourceInfo        *_sources;
    sourceGroup        *_sourceGroups;
    ALCcontext        *context;
    NSUInteger        _sourceGroupTotal;
    UInt32            _audioSessionCategory;
    BOOL            _handleAudioSession;
    ALfloat            _preMuteGain;
    NSObject        *_mutexBufferLoad;
    BOOL            mute_;
    BOOL            enabled_;

    ALenum            lastErrorCode_;
    BOOL            functioning_;
    float            asynchLoadProgress_;
    BOOL            getGainWorks_;
    
    //For managing dynamic allocation of sources and buffers
    int sourceTotal_;
    int bufferTotal;
     
}

@property (readwrite, nonatomic) ALfloat masterGain;
@property (readonly)  ALenum lastErrorCode;//Last OpenAL error code that was generated
@property (readonly)  BOOL functioning;//Is the sound engine functioning
@property (readwrite) float asynchLoadProgress;
@property (readonly)  BOOL getGainWorks;//Does getting the gain for a source work
/** Total number of sources available */
@property (readonly) int sourceTotal;
/** Total number of source groups that have been defined */
@property (readonly) NSUInteger sourceGroupTotal;

/** Sets the sample rate for the audio mixer. For best performance this should match the sample rate of your audio content */
+(void) setMixerSampleRate:(Float32) sampleRate;

/** Initializes the engine with a group definition and a total number of groups */
-(id)init;

/** Plays a sound in a channel group with a pitch, pan and gain. The sound could played looped or not */
-(ALuint) playSound:(int) soundId sourceGroupId:(int)sourceGroupId pitch:(float) pitch pan:(float) pan gain:(float) gain loop:(BOOL) loop;

/** Creates and returns a sound source object for the specified sound within the specified source group.
 */
-(AudioSource *) soundSourceForSound:(int) soundId sourceGroupId:(int) sourceGroupId;

/** Stops playing a sound */
- (void) stopSound:(ALuint) sourceId;
/** Stops playing a source group */
- (void) stopSourceGroup:(int) sourceGroupId;
/** Stops all playing sounds */
-(void) stopAllSounds;
/** Pause a sound */
-(void) pauseSound:(ALuint) sourceId;
/** Pause all sounds */
-(void) pauseAllSounds;
/** Resume a sound */
-(void) resumeSound:(ALuint) sourceId;
/** Resume all sounds */
-(void) resumeAllSounds;
-(void) defineSourceGroups:(NSArray*) sourceGroupDefinitions;
-(void) defineSourceGroups:(int[]) sourceGroupDefinitions total:(NSUInteger) total;
-(void) setSourceGroupNonInterruptible:(int) sourceGroupId isNonInterruptible:(BOOL) isNonInterruptible;
-(void) setSourceGroupEnabled:(int) sourceGroupId enabled:(BOOL) enabled;
-(BOOL) sourceGroupEnabled:(int) sourceGroupId;
-(BOOL) loadBufferFromData:(int) soundId soundData:(ALvoid*) soundData format:(ALenum) format size:(ALsizei) size freq:(ALsizei) freq;
-(BOOL) loadBuffer:(int) soundId filePath:(NSString*) filePath;
-(void) loadBuffersAsynchronously:(NSArray *) loadRequests;
-(BOOL) unloadBuffer:(int) soundId;
-(ALCcontext *) openALContext;

/** Returns the duration of the buffer in seconds or a negative value if the buffer id is invalid */
-(float) bufferDurationInSeconds:(int) soundId;
/** Returns the size of the buffer in bytes or a negative value if the buffer id is invalid */
-(ALsizei) bufferSizeInBytes:(int) soundId;
/** Returns the sampling frequency of the buffer in hertz or a negative value if the buffer id is invalid */
-(ALsizei) bufferFrequencyInHertz:(int) soundId;

/** Used internally, never call unless you know what you are doing */
-(void) _soundSourcePreRelease:(AudioSource *) soundSource;

@end

#pragma mark AudioSource
/** AudioSource is a wrapper around an OpenAL sound source.
 It allows you to manipulate properties such as pitch, gain, pan and looping while the 
 sound is playing.
 
 @since v1.0
 */
@interface AudioSource : NSObject <AudioTransportProtocol, AudioInterruptProtocol> {
    ALenum lastError;
@public
    ALuint _sourceId;
    ALuint _sourceIndex;
    AudioEngine* _engine;
    int _soundId;
    float _preMuteGain;
    BOOL enabled_;
    BOOL mute_;
}
@property (readwrite, nonatomic) float pitch;
@property (readwrite, nonatomic) float gain;
@property (readwrite, nonatomic) float pan;
@property (readwrite, nonatomic) BOOL looping;
@property (readonly)  BOOL isPlaying;
@property (readwrite, nonatomic) int soundId;
/** Returns the duration of the attached buffer in seconds or a negative value if the buffer is invalid */
@property (readonly) float durationInSeconds;

/** Stores the last error code that occurred. Check against AL_NO_ERROR */
@property (readonly) ALenum lastError;
/** Do not init yourself, get an instance from the sourceForSound factory method on AudioEngine */
-(id)init:(ALuint) theSourceId sourceIndex:(int) index soundEngine:(AudioEngine*) engine;

@end

#pragma mark AudioInterruptTargetGroup

/** Container for objects that implement audio interrupt protocol i.e. they can be muted and enabled.
 Setting mute and enabled for the group propagates to all children. 
 Designed to be used with your CDSoundSource objects to get them to comply with global enabled and mute settings
 if that is what you want to do.*/
@interface AudioInterruptTargetGroup : NSObject <AudioInterruptProtocol> {
    BOOL mute_;
    BOOL enabled_;
    NSMutableArray *children_;
}
-(void) addAudioInterruptTarget:(NSObject<AudioInterruptProtocol>*) interruptibleTarget;
@end

#pragma mark AsynchBufferLoader

/** AsynchBufferLoader
 TODO
 */
@interface AsynchBufferLoader : NSOperation {
    NSArray *_loadRequests;
    AudioEngine *_soundEngine;
}    

-(id) init:(NSArray *)loadRequests soundEngine:(AudioEngine *) theSoundEngine;

@end

#pragma mark BufferLoadRequest

/** BufferLoadRequest */
@interface BufferLoadRequest: NSObject
{
    NSString *filePath;
    int         soundId;
    //id       loader;
}

@property (readonly) NSString *filePath;
@property (readonly) int soundId;

- (id)init:(int) theSoundId filePath:(const NSString *) theFilePath;
@end

/** Interpolation type */
typedef enum {
    kIT_Linear,            //!Straight linear interpolation fade
    kIT_SCurve,            //!S curved interpolation
    kIT_Exponential     //!Exponential interpolation
} tFAInterpolationType;

#pragma mark FloatInterpolator
@interface FloatInterpolator: NSObject
{
    float start;
    float end;
    float lastValue;
    tFAInterpolationType interpolationType;
}
@property (readwrite, nonatomic) float start;
@property (readwrite, nonatomic) float end;
@property (readwrite, nonatomic) tFAInterpolationType interpolationType;

/** Return a value between min and max based on t which represents fractional progress where 0 is the start
 and 1 is the end */
-(float) interpolate:(float) t;
-(id) init:(tFAInterpolationType) type startVal:(float) startVal endVal:(float) endVal;

@end

#pragma mark PropertyModifier

/** Base class for classes that modify properties such as pitch, pan and gain */
@interface PropertyModifier: NSObject
{
    FloatInterpolator *interpolator;
    float startValue;
    float endValue;
    id target;
    BOOL stopTargetWhenComplete;
    
}
@property (readwrite, nonatomic) BOOL stopTargetWhenComplete;
@property (readwrite, nonatomic) float startValue;
@property (readwrite, nonatomic) float endValue;
@property (readwrite, nonatomic) tFAInterpolationType interpolationType;

-(id) init:(id) theTarget interpolationType:(tFAInterpolationType) type startVal:(float) startVal endVal:(float) endVal;
/** Set to a fractional value between 0 and 1 where 0 equals the start and 1 equals the end*/
-(void) modify:(float) t;

-(void) _setTargetProperty:(float) newVal;
-(float) _getTargetProperty;
-(void) _stopTarget;
-(Class) _allowableType;

@end

#pragma mark AudioSourceFader

/** Fader for AudioSource objects */
@interface AudioSourceFader : PropertyModifier{}
@end

#pragma mark AudioSourcePanner

/** Panner for AudioSource objects */
@interface AudioSourcePanner : PropertyModifier{}
@end

#pragma mark AudioSourcePitchBender

/** Pitch bender for AudioSource objects */
@interface AudioSourcePitchBender : PropertyModifier{}
@end

#pragma mark AudioEngineFader

/** Fader for AudioEngine objects */
@interface AudioEngineFader : PropertyModifier{}
@end





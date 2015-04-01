/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

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
****************************************************************************/

#include "core/audio/SimpleAudioEngine.h"
#include "BaseAudioEngine_objc.h"
#include "Flakor.h"

USING_FLAKOR_NS;

static void static_end()
{
    [BaseAudioEngine  end];
}

static void static_preloadBackgroundMusic(const char* pszFilePath)
{
    [[BaseAudioEngine sharedEngine] preloadBackgroundMusic: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    [[BaseAudioEngine sharedEngine] playBackgroundMusic: [NSString stringWithUTF8String: pszFilePath] loop: bLoop];
}

static void static_stopBackgroundMusic()
{
    [[BaseAudioEngine sharedEngine] stopBackgroundMusic];
}

static void static_pauseBackgroundMusic()
{
     [[BaseAudioEngine sharedEngine] pauseBackgroundMusic];
}

static void static_resumeBackgroundMusic()
{
    [[BaseAudioEngine sharedEngine] resumeBackgroundMusic];
} 

static void static_rewindBackgroundMusic()
{
    [[BaseAudioEngine sharedEngine] rewindBackgroundMusic];
}

static bool static_willPlayBackgroundMusic()
{
    return [[BaseAudioEngine sharedEngine] willPlayBackgroundMusic];
}

static bool static_isBackgroundMusicPlaying()
{
    return [[BaseAudioEngine sharedEngine] isBackgroundMusicPlaying];
}

static float static_getBackgroundMusicVolume()
{
    return [[BaseAudioEngine sharedEngine] backgroundMusicVolume];
}

static void static_setBackgroundMusicVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [BaseAudioEngine sharedEngine].backgroundMusicVolume = volume;
}
     
static float static_getEffectsVolume()
{
    return [[BaseAudioEngine sharedEngine] effectsVolume];
}
     
static void static_setEffectsVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [BaseAudioEngine sharedEngine].effectsVolume = volume;
}

static unsigned int static_playEffect(const char* pszFilePath, bool bLoop, Float32 pszPitch, Float32 pszPan, Float32 pszGain)
{
    return [[BaseAudioEngine sharedEngine] playEffect:[NSString stringWithUTF8String: pszFilePath] loop:bLoop pitch:pszPitch pan: pszPan gain:pszGain];
}
     
static void static_stopEffect(int nSoundId)
{
    [[BaseAudioEngine sharedEngine] stopEffect: nSoundId];
}
     
static void static_preloadEffect(const char* pszFilePath)
{
    [[BaseAudioEngine sharedEngine] preloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}
     
static void static_unloadEffect(const char* pszFilePath)
{
    [[BaseAudioEngine sharedEngine] unloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_pauseEffect(unsigned int uSoundId)
{
    [[BaseAudioEngine sharedEngine] pauseEffect: uSoundId];
}

static void static_pauseAllEffects()
{
    [[BaseAudioEngine sharedEngine] pauseAllEffects];
}

static void static_resumeEffect(unsigned int uSoundId)
{
    [[BaseAudioEngine sharedEngine] resumeEffect: uSoundId];
}

static void static_resumeAllEffects()
{
    [[BaseAudioEngine sharedEngine] resumeAllEffects];
}

static void static_stopAllEffects()
{
    [[BaseAudioEngine sharedEngine] stopAllEffects];
}

namespace flakor {

static SimpleAudioEngine *s_pEngine;

SimpleAudioEngine::SimpleAudioEngine()
{

}

SimpleAudioEngine::~SimpleAudioEngine()
{

}

SimpleAudioEngine* SimpleAudioEngine::getInstance()
{
    if (! s_pEngine)
    {
        s_pEngine = new (std::nothrow) SimpleAudioEngine();
    }
    
    return s_pEngine;
}

void SimpleAudioEngine::end()
{
    if (s_pEngine)
    {
        delete s_pEngine;
        s_pEngine = NULL;
    }
    
    static_end();
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* pszFilePath)
{
    // Changing file path to full path
    String* fullPath = FileUtils::getBundleFilePath(pszFilePath);
    static_preloadBackgroundMusic(fullPath->getCString());
}

void SimpleAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    // Changing file path to full path
    String* fullPath = FileUtils::getBundleFilePath(pszFilePath);
    static_playBackgroundMusic(fullPath->getCString(), bLoop);
}

void SimpleAudioEngine::stopBackgroundMusic(bool bReleaseData)
{
    static_stopBackgroundMusic();
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    static_pauseBackgroundMusic();
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    static_resumeBackgroundMusic();
} 

void SimpleAudioEngine::rewindBackgroundMusic()
{
    static_rewindBackgroundMusic();
}

bool SimpleAudioEngine::willPlayBackgroundMusic()
{
    return static_willPlayBackgroundMusic();
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return static_isBackgroundMusicPlaying();
}

float SimpleAudioEngine::getBackgroundMusicVolume()
{
    return static_getBackgroundMusicVolume();
}

void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    static_setBackgroundMusicVolume(volume);
}

float SimpleAudioEngine::getEffectsVolume()
{
    return static_getEffectsVolume();
}

void SimpleAudioEngine::setEffectsVolume(float volume)
{
    static_setEffectsVolume(volume);
}

unsigned int SimpleAudioEngine::playEffect(const char *pszFilePath, bool bLoop,
                                           float pitch, float pan, float gain)
{
    // Changing file path to full path
    String* fullPath = FileUtils::getBundleFilePath(pszFilePath);
    return static_playEffect(fullPath->getCString(), bLoop, pitch, pan, gain);
}

void SimpleAudioEngine::stopEffect(unsigned int nSoundId)
{
    static_stopEffect(nSoundId);
}

void SimpleAudioEngine::preloadEffect(const char* pszFilePath)
{
    // Changing file path to full path
    String* fullPath = FileUtils::getBundleFilePath(pszFilePath);
    static_preloadEffect(fullPath->getCString());
}

void SimpleAudioEngine::unloadEffect(const char* pszFilePath)
{
    // Changing file path to full path
    String* fullPath = FileUtils::getBundleFilePath(pszFilePath);
    static_unloadEffect(fullPath->getCString());
}

void SimpleAudioEngine::pauseEffect(unsigned int uSoundId)
{
    static_pauseEffect(uSoundId);
}

void SimpleAudioEngine::resumeEffect(unsigned int uSoundId)
{
    static_resumeEffect(uSoundId);
}

void SimpleAudioEngine::pauseAllEffects()
{
    static_pauseAllEffects();
}

void SimpleAudioEngine::resumeAllEffects()
{
    static_resumeAllEffects();
}

void SimpleAudioEngine::stopAllEffects()
{
    static_stopAllEffects();
}

} // endof namespace CocosDenshion {

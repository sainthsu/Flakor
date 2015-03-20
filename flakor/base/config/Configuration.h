/****************************************************************************
Copyright (c) 2014 flakor.org

http://www.flakor.org

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

#ifndef _FK_CONFIGURATION_H_
#define _FK_CONFIGURATION_H_

#include "base/lang/Object.h"
#include "GL.h"
#include "base/lang/Str.h"
#include <string>

FLAKOR_NS_BEGIN

typedef enum _ConfigurationType {
    ConfigurationError,
    ConfigurationString,
    ConfigurationInt,
    ConfigurationDouble,
    ConfigurationBoolean
} ConfigurationType;


/**
 * @addtogroup global
 * @{
 */
/**
 @brief Configuration contains some openGL variables
 */
class Configuration : public Object
{
public:
    /** returns a shared instance of Configuration */
    static Configuration *sharedConfiguration(void);

    /** purge the shared instance of Configuration */
    static void purgeConfiguration(void);

public:
    /**
     *  @js NA
     *  @lua NA
     */
	virtual ~Configuration(void);

    /** OpenGL Max texture size. */
	int getMaxTextureSize(void) const;

    /** OpenGL Max Modelview Stack Depth. */
	int getMaxModelviewStackDepth(void) const;

    /** returns the maximum texture units
     @since v2.0.0
     */
	int getMaxTextureUnits(void) const;

    /** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
     OpenGL ES 2.0 already supports NPOT (iOS).
     @since v0.99.2
     */
	bool supportsNPOT(void) const;

    /** Whether or not PVR Texture Compressed is supported */
	bool supportsPVRTC(void) const;

    /** Whether or not BGRA8888 textures are supported.
     @since v0.99.2
     */
	bool supportsBGRA8888(void) const;

    /** Whether or not glDiscardFramebufferEXT is supported
     @since v0.99.2
     */
	bool supportsDiscardFramebuffer(void) const;

    /** Whether or not shareable VAOs are supported.
     @since v2.0.0
     */
	bool supportsShareableVAO(void) const;

    /** returns whether or not an OpenGL is supported */
    bool checkForGLExtension(const std::string &searchName) const;

    bool init(void);

	/** returns the value of a given key as a string.
	 If the key is not found, it will return the default value */
	const char* getCString( const char *key, const char *default_value=NULL ) const;

	/** returns the value of a given key as a boolean.
	 If the key is not found, it will return the default value */
	bool getBool( const char *key, bool default_value=false ) const;

	/** returns the value of a given key as a double.
	 If the key is not found, it will return the default value */
	double getNumber( const char *key, double default_value=0.0 ) const;

	/** returns the value of a given key as a double */
	Object * getObject( const char *key ) const;

	/** sets a new key/value pair  in the configuration dictionary */
	void setObject( const char *key, Object *value );

	/** dumps the current configuration on the console */
	void dumpInfo(void) const;

	/** gathers OpenGL / GPU information */
	void gatherGPUInfo( void );

	/** Loads a config file. If the keys are already present, then they are going to be replaced. Otherwise the new keys are added. */
	void loadConfigFile( const char *filename );

private:
    Configuration(void);
    static Configuration *s_gSharedConfiguration;
	static std::string		s_sConfigfile;
    
protected:
    GLint           m_nMaxTextureSize;
    GLint           m_nMaxModelviewStackDepth;
    bool            m_bSupportsPVRTC;
    bool            m_bSupportsNPOT;
    bool            m_bSupportsBGRA8888;
    bool            m_bSupportsDiscardFramebuffer;
    bool            m_bSupportsShareableVAO;
    GLint           m_nMaxSamplesAllowed;
    GLint           m_nMaxTextureUnits;
    char *          m_pGlExtensions;
	
	Dictionary	*m_pValueDict;
};

// end of global group
/// @}

FLAKOR_NS_END

#endif // _FK_CONFIGURATION_H_

/****************************************************************************
Copyright (c) 2013-2014 Saint Hsu

http://www.flakor.org

****************************************************************************/

#ifndef _FK_GPUINFO_H_
#define _FK_GPUINFO_H_

#include <string>

#include "base/lang/Object.h"
#include "core/opengl/GL.h"

FLAKOR_NS_BEGIN

/**
 * @addtogroup global
 * @{
 */
/**
 @brief GPUInfo contains some openGL variables
 */
class  GPUInfo : public Object
{
public:
    
    /** returns a shared instance of GPUInfo */
    static GPUInfo *getInstance();

    /** purge the shared instance of GPUInfo */
    static void destroyInstance();

public:
    /**
     * @js NA
     * @lua NA
     */
	virtual ~GPUInfo();

    /** OpenGL Max texture size. */
	int getMaxTextureSize() const;

    /** OpenGL Max Modelview Stack Depth. */
	int getMaxModelviewStackDepth() const;

    /** returns the maximum texture units
     */
	int getMaxTextureUnits() const;

    /** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
     OpenGL ES 2.0 already supports NPOT (iOS).
     */
	bool supportsNPOT() const;

    /** Whether or not PVR Texture Compressed is supported */
	bool supportsPVRTC() const;
    
     /** Whether or not ETC Texture Compressed is supported */
    bool supportsETC() const;
    
    /** Whether or  not S3TC Texture Compressed is supported */
    bool supportsS3TC() const;
    
    /** Whether or  not ATITC Texture Compressed is supported */
    bool supportsATITC() const;
    
    /** Whether or not BGRA8888 textures are supported.
     @since v0.99.2
     */
	bool supportsBGRA8888() const;

    /** Whether or not glDiscardFramebufferEXT is supported
     */
	bool supportsDiscardFramebuffer() const;

    /** Whether or not shareable VAOs are supported.
     */
	bool supportsShareableVAO() const;
    
    /** Max support directional light in shader, for Sprite3D
     */
    int getMaxSupportDirLightInShader() const;
    
    /** Max support point light in shader, for Sprite3D
     */
    int getMaxSupportPointLightInShader() const;
    
    /** Max support spot light in shader, for Sprite3D
     */
    int getMaxSupportSpotLightInShader() const;

    /** returns whether or not an OpenGL is supported */
    bool checkForGLExtension(const std::string &searchName) const;

    //bool init();

	/** returns the value of a given key as a double */
	//const Value& getValue(const std::string& key, const Value& defaultValue = Value::Null) const;

	/** sets a new key/value pair  in the configuration dictionary */
	//void setValue(const std::string& key, const Value& value);

    /** returns the GPUInfo info */
    std::string getInfo() const;

	/** gathers OpenGL / GPU information */
	void gatherGPUInfo();

	/** Loads a config file. If the keys are already present, then they are going to be replaced. Otherwise the new keys are added. */
	//void loadConfigFile(const std::string& filename);

private:
    GPUInfo(void);
    static GPUInfo    *s_sharedGPUInfo;
	static std::string		s_configfile;
    
protected:
    GLint           _maxTextureSize;
    GLint           _maxModelviewStackDepth;
    bool            _supportsPVRTC;
    bool            _supportsETC1;
    bool            _supportsS3TC;
    bool            _supportsATITC;
    bool            _supportsNPOT;
    bool            _supportsBGRA8888;
    bool            _supportsDiscardFramebuffer;
    bool            _supportsShareableVAO;
    GLint           _maxSamplesAllowed;
    GLint           _maxTextureUnits;
    char *          _glExtensions;
    int             _maxDirLightInShader; //max support directional light in shader
    int             _maxPointLightInShader; // max support point light in shader
    int             _maxSpotLightInShader; // max support spot light in shader
	
	//ValueMap        _valueDict;
};

// end of global group
/// @}

FLAKOR_NS_END

#endif // _FK_GPUINFO_H_

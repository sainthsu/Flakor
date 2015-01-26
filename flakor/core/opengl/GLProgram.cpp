/****************************************************************************
Copyright 2013-2014 Saint

http://www.flakor.org
****************************************************************************/

#ifndef WIN32
#include <alloca.h>
#endif
#include <assert.h>
#include "macros.h"
#include "math/uthash.h"
#include "base/lang/Str.h"
#include "math/GLMatrix.h"

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8) || defined(WP8_SHADER_COMPILER)
#include "PrecompiledShaders.h"
#endif

#include "core/opengl/GLProgram.h"

FLAKOR_NS_BEGIN

typedef struct _hashUniformEntry
{
    GLvoid*         value;       // value
    unsigned int    location;    // Key
    UT_hash_handle  hh;          // hash entry
} tHashUniformEntry;

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP = "ShaderPositionTextureColor_noMVP";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST = "ShaderPositionTextureColorAlphaTest";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV = "ShaderPositionTextureColorAlphaTest_NoMV";
const char* GLProgram::SHADER_NAME_POSITION_COLOR = "ShaderPositionColor";
const char* GLProgram::SHADER_NAME_POSITION_COLOR_NO_MVP = "ShaderPositionColor_noMVP";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE = "ShaderPositionTexture";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR = "ShaderPositionTexture_uColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_A8_COLOR = "ShaderPositionTextureA8Color";
const char* GLProgram::SHADER_NAME_POSITION_U_COLOR = "ShaderPosition_uColor";
const char* GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR = "ShaderPositionLengthTextureColor";

const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL = "ShaderLabelDFNormal";
const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW = "ShaderLabelDFGlow";
const char* GLProgram::SHADER_NAME_LABEL_NORMAL = "ShaderLabelNormal";
const char* GLProgram::SHADER_NAME_LABEL_OUTLINE = "ShaderLabelOutline";

const char* GLProgram::SHADER_3D_POSITION = "Shader3DPosition";
const char* GLProgram::SHADER_3D_POSITION_TEXTURE = "Shader3DPositionTexture";
const char* GLProgram::SHADER_3D_SKINPOSITION_TEXTURE = "Shader3DSkinPositionTexture";
const char* GLProgram::SHADER_3D_POSITION_NORMAL = "Shader3DPositionNormal";
const char* GLProgram::SHADER_3D_POSITION_NORMAL_TEXTURE = "Shader3DPositionNormalTexture";
const char* GLProgram::SHADER_3D_SKINPOSITION_NORMAL_TEXTURE = "Shader3DSkinPositionNormalTexture";


// uniform names
const char* GLProgram::UNIFORM_NAME_AMBIENT_COLOR = "FK_AmbientColor";
const char* GLProgram::UNIFORM_NAME_P_MATRIX = "FK_PMatrix";
const char* GLProgram::UNIFORM_NAME_MV_MATRIX = "FK_MVMatrix";
const char* GLProgram::UNIFORM_NAME_MVP_MATRIX  = "FK_MVPMatrix";
const char* GLProgram::UNIFORM_NAME_NORMAL_MATRIX = "FK_NormalMatrix";
const char* GLProgram::UNIFORM_NAME_TIME = "FK_Time";
const char* GLProgram::UNIFORM_NAME_SIN_TIME = "FK_SinTime";
const char* GLProgram::UNIFORM_NAME_COS_TIME = "FK_CosTime";
const char* GLProgram::UNIFORM_NAME_RANDOM01 = "FK_Random01";
const char* GLProgram::UNIFORM_NAME_SAMPLER0 = "FK_Texture0";
const char* GLProgram::UNIFORM_NAME_SAMPLER1 = "FK_Texture1";
const char* GLProgram::UNIFORM_NAME_SAMPLER2 = "FK_Texture2";
const char* GLProgram::UNIFORM_NAME_SAMPLER3 = "FK_Texture3";
const char* GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE = "FK_alpha_value";

// Attribute names
const char* GLProgram::ATTRIBUTE_NAME_COLOR = "a_color";
const char* GLProgram::ATTRIBUTE_NAME_POSITION = "a_position";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD1 = "a_texCoord1";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD2 = "a_texCoord2";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD3 = "a_texCoord3";
const char* GLProgram::ATTRIBUTE_NAME_NORMAL = "a_normal";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT = "a_blendWeight";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_INDEX = "a_blendIndex";

GLProgram* GLProgram::createWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    auto ret = new (std::nothrow) GLProgram();
    if(ret && ret->initWithByteArrays(vShaderByteArray, fShaderByteArray)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }

    FK_SAFE_DELETE(ret);
    return nullptr;
}

GLProgram* GLProgram::createWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename)
{
    auto ret = new (std::nothrow) GLProgram();
    if(ret && ret->initWithFilenames(vShaderFilename, fShaderFilename)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }

    FK_SAFE_DELETE(ret);
    return nullptr;
}

GLProgram::GLProgram()
: _programID(0)
, _vertShaderID(0)
, _fragShaderID(0)
, _hashForUniforms(nullptr)
, _flags()
{
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
}

GLProgram::~GLProgram()
{
    FKLOGINFO("%s %d deallocing GLProgram: %p", __FUNCTION__, __LINE__, this);

    if (_vertShaderID)
    {
        glDeleteShader(_vertShaderID);
    }
    
    if (_fragShaderID)
    {
        glDeleteShader(_fragShaderID);
    }
    
    _vertShaderID = _fragShaderID = 0;

    if (_programID) 
    {
        glDeleteProgram(_programID);
    }

    tHashUniformEntry *current_element, *tmp;

    // Purge uniform hash
    HASH_ITER(hh, _hashForUniforms, current_element, tmp)
    {
        HASH_DEL(_hashForUniforms, current_element);
        free(current_element->value);
        free(current_element);
    }
}

bool GLProgram::initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8)
    GLboolean hasCompiler = false;
    glGetBooleanv(GL_SHADER_COMPILER, &hasCompiler);
    _hasShaderCompiler = (hasCompiler == GL_TRUE);

    if(!_hasShaderCompiler)
    {
        return initWithPrecompiledProgramByteArray(vShaderByteArray,fShaderByteArray);
    }
#endif

    _programID = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    _vertShaderID = _fragShaderID = 0;

    if (vShaderByteArray)
    {
        if (!compileShader(&_vertShaderID, GL_VERTEX_SHADER, vShaderByteArray))
        {
            FKLOG("flakor: ERROR: Failed to compile vertex shader");
            return false;
       }
    }

    // Create and compile fragment shader
    if (fShaderByteArray)
    {
        if (!compileShader(&_fragShaderID, GL_FRAGMENT_SHADER, fShaderByteArray))
        {
            FKLOG("flakor: ERROR: Failed to compile fragment shader");
            return false;
        }
    }

    if (_vertShaderID)
    {
        glAttachShader(_programID, _vertShaderID);
    }
    CHECK_GL_ERROR_DEBUG();

    if (_fragShaderID)
    {
        glAttachShader(_programID, _fragShaderID);
    }
    _hashForUniforms = nullptr;
    
    CHECK_GL_ERROR_DEBUG();

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8) || defined(WP8_SHADER_COMPILER)
    _shaderId = PrecompiledShaders::getInstance()->addShaders(vShaderByteArray, fShaderByteArray);
#endif

    return true;
}

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8)
GLProgram* GLProgram::createWithPrecompiledProgramByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    auto ret = new (std::nothrow) GLProgram();
    if(ret && ret->initWithPrecompiledProgramByteArray(vShaderByteArray, fShaderByteArray)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }

    FK_SAFE_DELETE(ret);
    return nullptr;
}

bool GLProgram::initWithPrecompiledProgramByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    bool haveProgram = false;

    _programID = glCreateProgram();
    CHECK_GL_ERROR_DEBUG();

    _vertShaderID = _fragShaderID = 0;

    haveProgram = PrecompiledShaders::getInstance()->loadProgram(_program, vShaderByteArray, fShaderByteArray);

    CHECK_GL_ERROR_DEBUG();
    _hashForUniforms = nullptr;

    CHECK_GL_ERROR_DEBUG();  

    return haveProgram;
}
#endif

//ADD FILE UTILS LATER
bool GLProgram::initWithFilenames(const std::string &vShaderFilename, const std::string &fShaderFilename)
{
    std::string vertexSource = NULL;//fileUtils->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(vShaderFilename));
    std::string fragmentSource = NULL;//fileUtils->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(fShaderFilename));

    return initWithByteArrays(vertexSource.c_str(), fragmentSource.c_str());
}

void GLProgram::bindPredefinedVertexAttribs()
{
    static const struct {
        const char *attributeName;
        int location;
    } attribute_locations[] =
    {
        {GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION},
        {GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR},
        {GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD},
        {GLProgram::ATTRIBUTE_NAME_TEX_COORD1, GLProgram::VERTEX_ATTRIB_TEX_COORD1},
        {GLProgram::ATTRIBUTE_NAME_TEX_COORD2, GLProgram::VERTEX_ATTRIB_TEX_COORD2},
        {GLProgram::ATTRIBUTE_NAME_TEX_COORD3, GLProgram::VERTEX_ATTRIB_TEX_COORD3},
        {GLProgram::ATTRIBUTE_NAME_NORMAL, GLProgram::VERTEX_ATTRIB_NORMAL},
    };

    const int size = sizeof(attribute_locations) / sizeof(attribute_locations[0]);

    for(int i=0; i<size;i++) {
        glBindAttribLocation(_programID, attribute_locations[i].location, attribute_locations[i].attributeName);
    }
}

void GLProgram::parseVertexAttribs()
{
    _vertexAttribs.clear();

    // Query and store vertex attribute meta-data from the program.
    GLint activeAttributes;
    GLint length;
    glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    if(activeAttributes > 0)
    {
        VertexAttrib attribute;

        glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
        if(length > 0)
        {
            GLchar* attribName = (GLchar*) alloca(length + 1);

            for(int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                glGetActiveAttrib(_programID, i, length, nullptr, &attribute.size, &attribute.type, attribName);
                attribName[length] = '\0';
                attribute.name = std::string(attribName);

                // Query the pre-assigned attribute location
                attribute.index = glGetAttribLocation(_programID, attribName);
                _vertexAttribs[attribute.name] = attribute;
            }
        }
    }
}

void GLProgram::parseUniforms()
{
    _userUniforms.clear();

    // Query and store uniforms from the program.
    GLint activeUniforms;
    glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if(activeUniforms > 0)
    {
        GLint length;
        glGetProgramiv(_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
        if(length > 0)
        {
            Uniform uniform;

            GLchar* uniformName = (GLchar*)alloca(length + 1);

            for(int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                glGetActiveUniform(_programID, i, length, nullptr, &uniform.size, &uniform.type, uniformName);
                uniformName[length] = '\0';

                // Only add uniforms that are not built-in.
                // The ones that start with 'FK_' are built-ins
                if(strncmp("FK_", uniformName, 3) != 0) {

                    // remove possible array '[]' from uniform name
                    if(length > 3)
                    {
                        char* c = strrchr(uniformName, '[');
                        if(c)
                        {
                            *c = '\0';
                        }
                    }
                    uniform.name = std::string(uniformName);
                    uniform.location = glGetUniformLocation(_programID, uniformName);
                    GLenum __gl_error_code = glGetError(); 
                    if (__gl_error_code != GL_NO_ERROR) 
                    { 
                        FKLOG("error: 0x%x", (int)__gl_error_code);
                    } 
                    assert(__gl_error_code == GL_NO_ERROR);
                    
                    _userUniforms[uniform.name] = uniform;
                }
            }
        }
    }
}

Uniform* GLProgram::getUniform(const std::string &name)
{
    const auto itr = _userUniforms.find(name);
    if( itr != _userUniforms.end())
        return &itr->second;
    return nullptr;
}

VertexAttrib* GLProgram::getVertexAttrib(const std::string &name)
{
    const auto itr = _vertexAttribs.find(name);
    if( itr != _vertexAttribs.end())
        return &itr->second;
    return nullptr;
}

String* GLProgram::getDescription() const
{
    return String::createWithFormat("<GLProgram = "
                                      FK_FORMAT_PRINTF_SIZE_T
                                      " | Program = %i, VertexShader = %i, FragmentShader = %i>",
                                      (size_t)this, _programID, _vertShaderID, _fragShaderID);
}

bool GLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (FK_TARGET_PLATFORM != FK_PLATFORM_WIN32 && FK_TARGET_PLATFORM != FK_PLATFORM_LINUX && FK_TARGET_PLATFORM != FK_PLATFORM_MAC)
        (type == GL_VERTEX_SHADER ? "precision highp float;\n precision highp int;\n" : "precision mediump float;\n precision mediump int;\n"),
#endif
        "uniform mat4 FK_PMatrix;\n"
        "uniform mat4 FK_MVMatrix;\n"
        "uniform mat4 FK_MVPMatrix;\n"
        "uniform mat3 FK_NormalMatrix;\n"
        "uniform vec4 FK_Time;\n"
        "uniform vec4 FK_SinTime;\n"
        "uniform vec4 FK_CosTime;\n"
        "uniform vec4 FK_Random01;\n"
        "uniform sampler2D FK_Texture0;\n"
        "uniform sampler2D FK_Texture1;\n"
        "uniform sampler2D FK_Texture2;\n"
        "uniform sampler2D FK_Texture3;\n"
        "//FK INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, nullptr);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
        glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
        GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
        
        glGetShaderSource(*shader, length, nullptr, src);
        FKLOG("flakor: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            FKLOG("flakor: vertex shader%s", getVertexShaderLog().c_str());
        }
        else
        {
            FKLOG("flakor: %s", getFragmentShaderLog().c_str());
        }
        free(src);

        return false;
    }
    return (status == GL_TRUE);
}

GLint GLProgram::getAttribLocation(const std::string &attributeName) const
{
    return glGetAttribLocation(_programID, attributeName.c_str());
}

GLint GLProgram::getUniformLocation(const std::string &attributeName) const
{
    return glGetUniformLocation(_programID, attributeName.c_str());
}

void GLProgram::bindAttribLocation(const std::string &attributeName, GLuint index) const
{
    glBindAttribLocation(_programID, index, attributeName.c_str());
}

void GLProgram::updateUniforms()
{
    _builtInUniforms[UNIFORM_AMBIENT_COLOR] = glGetUniformLocation(_programID, UNIFORM_NAME_AMBIENT_COLOR);
    _builtInUniforms[UNIFORM_P_MATRIX] = glGetUniformLocation(_programID, UNIFORM_NAME_P_MATRIX);
    _builtInUniforms[UNIFORM_MV_MATRIX] = glGetUniformLocation(_programID, UNIFORM_NAME_MV_MATRIX);
    _builtInUniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(_programID, UNIFORM_NAME_MVP_MATRIX);
    _builtInUniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_programID, UNIFORM_NAME_NORMAL_MATRIX);
    
    _builtInUniforms[UNIFORM_TIME] = glGetUniformLocation(_programID, UNIFORM_NAME_TIME);
    _builtInUniforms[UNIFORM_SIN_TIME] = glGetUniformLocation(_programID, UNIFORM_NAME_SIN_TIME);
    _builtInUniforms[UNIFORM_COS_TIME] = glGetUniformLocation(_programID, UNIFORM_NAME_COS_TIME);

    _builtInUniforms[UNIFORM_RANDOM01] = glGetUniformLocation(_programID, UNIFORM_NAME_RANDOM01);

    _builtInUniforms[UNIFORM_SAMPLER0] = glGetUniformLocation(_programID, UNIFORM_NAME_SAMPLER0);
    _builtInUniforms[UNIFORM_SAMPLER1] = glGetUniformLocation(_programID, UNIFORM_NAME_SAMPLER1);
    _builtInUniforms[UNIFORM_SAMPLER2] = glGetUniformLocation(_programID, UNIFORM_NAME_SAMPLER2);
    _builtInUniforms[UNIFORM_SAMPLER3] = glGetUniformLocation(_programID, UNIFORM_NAME_SAMPLER3);

    _flags.usesP = _builtInUniforms[UNIFORM_P_MATRIX] != -1;
    _flags.usesMV = _builtInUniforms[UNIFORM_MV_MATRIX] != -1;
    _flags.usesMVP = _builtInUniforms[UNIFORM_MVP_MATRIX] != -1;
    _flags.usesNormal = _builtInUniforms[UNIFORM_NORMAL_MATRIX] != -1;
    _flags.usesTime = (
                       _builtInUniforms[UNIFORM_TIME] != -1 ||
                       _builtInUniforms[UNIFORM_SIN_TIME] != -1 ||
                       _builtInUniforms[UNIFORM_COS_TIME] != -1
                       );
    _flags.usesRandom = _builtInUniforms[UNIFORM_RANDOM01] != -1;

    this->use();
    
    // Since sample most probably won't change, set it to 0,1,2,3 now.
    if(_builtInUniforms[UNIFORM_SAMPLER0] != -1)
       setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER0], 0);
    if(_builtInUniforms[UNIFORM_SAMPLER1] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER1], 1);
    if(_builtInUniforms[UNIFORM_SAMPLER2] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER2], 2);
    if(_builtInUniforms[UNIFORM_SAMPLER3] != -1)
        setUniformLocationWith1i(_builtInUniforms[UNIFORM_SAMPLER3], 3);
}

bool GLProgram::link()
{
    FKAssert(_programID != 0, "Cannot link invalid program");

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8)
    if(!_compiled)
    {
        // precompiled shader program is already linked

        //bindPredefinedVertexAttribs();
        parseVertexAttribs();
        parseUniforms();
        return true;
    }
#endif

    GLint status = GL_TRUE;

    bindPredefinedVertexAttribs();

    glLinkProgram(_programID);

    parseVertexAttribs();
    parseUniforms();

    if (_vertShaderID)
    {
        glDeleteShader(_vertShaderID);
    }
    
    if (_fragShaderID)
    {
        glDeleteShader(_fragShaderID);
    }
    
    _vertShaderID = _fragShaderID = 0;
    
#if DEBUG || (FK_TARGET_PLATFORM == FK_PLATFORM_WP8)
    glGetProgramiv(_programID, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE)
    {
        FKLOG("flakor: ERROR: Failed to link program: %i", _programID);
        glDeleteProgram(_programID);
        _programID = 0;
    }
#endif

#if (FK_TARGET_PLATFORM == FK_PLATFORM_WP8) || defined(WP8_SHADER_COMPILER)
    if (status == GL_TRUE)
    {
        PrecompiledShaders::getInstance()->addProgram(_program, _shaderId);
    }
#endif

    return (status == GL_TRUE);
}

void GLProgram::use()
{
    glUseProgram(_programID);
}

std::string GLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc) const
{
    std::string ret;
    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return "";

    char *logBytes = (char*)malloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);

    ret = logBytes;

    free(logBytes);
    return ret;
}

std::string GLProgram::getVertexShaderLog() const
{
    return this->logForOpenGLObject(_vertShaderID, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

std::string GLProgram::getFragmentShaderLog() const
{
    return this->logForOpenGLObject(_fragShaderID, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

std::string GLProgram::getProgramLog() const
{
    return this->logForOpenGLObject(_programID, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

// Uniform cache

bool GLProgram::updateUniformLocation(GLint location, const GLvoid* data, unsigned int bytes)
{
    if (location < 0)
    {
        return false;
    }

    bool updated = true;
    tHashUniformEntry *element = nullptr;
    HASH_FIND_INT(_hashForUniforms, &location, element);

    if (! element)
    {
        element = (tHashUniformEntry*)malloc( sizeof(*element) );

        // key
        element->location = location;

        // value
        element->value = malloc( bytes );
        memcpy(element->value, data, bytes );

        HASH_ADD_INT(_hashForUniforms, location, element);
    }
    else
    {
        if (memcmp(element->value, data, bytes) == 0)
        {
            updated = false;
        }
        else
        {
            memcpy(element->value, data, bytes);
        }
    }

    return updated;
}

GLint GLProgram::getUniformLocationForName(const char* name) const
{
    FKAssert(name != nullptr, "Invalid uniform name" );
    FKAssert(_programID != 0, "Invalid operation. Cannot get uniform location when program is not initialized");
    
    return glGetUniformLocation(_programID, name);
}

void GLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    bool updated = updateUniformLocation(location, &i1, sizeof(i1)*1);
    
    if( updated )
    {
        glUniform1i( (GLint)location, i1);
    }
}

void GLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
    GLint ints[2] = {i1,i2};
    bool updated = updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform2i( (GLint)location, i1, i2);
    }
}

void GLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
    GLint ints[3] = {i1,i2,i3};
    bool updated = updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform3i( (GLint)location, i1, i2, i3);
    }
}

void GLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    GLint ints[4] = {i1,i2,i3,i4};
    bool updated = updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform4i( (GLint)location, i1, i2, i3, i4);
    }
}

void GLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, ints, sizeof(int)*2*numberOfArrays);
    
    if( updated )
    {
        glUniform2iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, ints, sizeof(int)*3*numberOfArrays);
    
    if( updated )
    {
        glUniform3iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, ints, sizeof(int)*4*numberOfArrays);
    
    if( updated )
    {
        glUniform4iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
    bool updated = updateUniformLocation(location, &f1, sizeof(f1)*1);

    if( updated )
    {
        glUniform1f( (GLint)location, f1);
    }
}

void GLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
    GLfloat floats[2] = {f1,f2};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform2f( (GLint)location, f1, f2);
    }
}

void GLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    GLfloat floats[3] = {f1,f2,f3};
    bool updated = updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform3f( (GLint)location, f1, f2, f3);
    }
}

void GLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLfloat floats[4] = {f1,f2,f3,f4};
    bool updated = updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform4f( (GLint)location, f1, f2, f3,f4);
    }
}


void GLProgram::setUniformLocationWith1fv( GLint location, const GLfloat* floats, unsigned int numberOfArrays )
{
    bool updated = updateUniformLocation(location, floats, sizeof(float)*numberOfArrays);

    if( updated )
    {
        glUniform1fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWith2fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, floats, sizeof(float)*2*numberOfArrays);

    if( updated )
    {
        glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWith3fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, floats, sizeof(float)*3*numberOfArrays);

    if( updated )
    {
        glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWith4fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated = updateUniformLocation(location, floats, sizeof(float)*4*numberOfArrays);

    if( updated )
    {
        glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLProgram::setUniformLocationWithMatrix2fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices) {
    bool updated = updateUniformLocation(location, matrixArray, sizeof(float)*4*numberOfMatrices);
    
    if( updated )
    {
        glUniformMatrix2fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void GLProgram::setUniformLocationWithMatrix3fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices) {
    bool updated = updateUniformLocation(location, matrixArray, sizeof(float)*9*numberOfMatrices);
    
    if( updated )
    {
        glUniformMatrix3fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}


void GLProgram::setUniformLocationWithMatrix4fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    bool updated = updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

    if( updated )
    {
        glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void GLProgram::setUniformsForBuiltins()
{ 
    Matrix4 matrixMV;
	GLGet(GL_MODELVIEW,&matrixMV);

    setUniformsForBuiltins(matrixMV);
}

void GLProgram::setUniformsForBuiltins(const Matrix4 &matrixMV)
{
    Matrix4 matrixP;
 	GLGet(GL_PROJECTION,&matrixP);

    if(_flags.usesP)
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_P_MATRIX], matrixP.get(), 1);

    if(_flags.usesMV)
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MV_MATRIX], matrixMV.get(), 1);

    if(_flags.usesMVP) {
        Matrix4 matrixMVP = matrixP * matrixMV;
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MVP_MATRIX], matrixMVP.get(), 1);
    }

    if (_flags.usesNormal)
    {
        Matrix4 mvInverse = matrixMV;
        mvInverse[12] = mvInverse[13] = mvInverse[14] = 0.0f;
        mvInverse.invert();
        mvInverse.transpose();
        GLfloat normalMat[9];
        normalMat[0] = mvInverse[0];normalMat[1] = mvInverse[1];normalMat[2] = mvInverse[2];
        normalMat[3] = mvInverse[4];normalMat[4] = mvInverse[5];normalMat[5] = mvInverse[6];
        normalMat[6] = mvInverse[8];normalMat[7] = mvInverse[9];normalMat[8] = mvInverse[10];
        setUniformLocationWithMatrix3fv(_builtInUniforms[UNIFORM_NORMAL_MATRIX], normalMat, 1);
    }

    if(_flags.usesTime) {
        // This doesn't give the most accurate global time value.
        // Cocos2D doesn't store a high precision time value, so this will have to do.
        // Getting Mach time per frame per shader using time could be extremely expensive.
        float time = 100;//director->getTotalFrames() * director->getAnimationInterval();
        
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_TIME], time/10.0, time, time*2, time*4);
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_SIN_TIME], time/8.0, time/4.0, time/2.0, sinf(time));
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_COS_TIME], time/8.0, time/4.0, time/2.0, cosf(time));
    }
    
    if(_flags.usesRandom)
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_RANDOM01], FKRANDOM_0_1(), FKRANDOM_0_1(), FKRANDOM_0_1(), FKRANDOM_0_1());
}

void GLProgram::reset()
{
    _vertShaderID = _fragShaderID = 0;
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
    

    // it is already deallocated by android
    //GL::deleteProgram(_program);
    _programID = 0;

    
    tHashUniformEntry *current_element, *tmp;
    
    // Purge uniform hash
    HASH_ITER(hh, _hashForUniforms, current_element, tmp) 
    {
        HASH_DEL(_hashForUniforms, current_element);
        free(current_element->value);
        free(current_element);
    }
    _hashForUniforms = nullptr;
}

FLAKOR_NS_END

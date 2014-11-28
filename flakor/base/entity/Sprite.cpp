/****************************************************************************
Copyright (c) 2013-2014 saint(saint@aliyun.com)

http://www.flakor.org
****************************************************************************/

#include "base/entity/Sprite.h"
#include "base/element/Element.h"
#include "core/texture/Image.h"
#include "core/texture/Texture2D.h"
#include "core/opengl/shader/Shaders.h"
#include "core/opengl/GLProgram.h"

FLAKOR_NS_BEGIN

#if FK_SPRITEBATCHNODE_RENDER_SUBPIXEL
	#define RENDER_IN_SUBPIXEL
#else
	#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

// MARK: create, init, dealloc
Sprite* Sprite::createWithTexture(Texture2D *texture)
{
    Sprite *sprite = new (std::nothrow) Sprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    FK_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    Sprite *sprite = new (std::nothrow) Sprite();
    if (sprite && sprite->initWithTexture(texture, rect, rotated))
    {
        sprite->autorelease();
        return sprite;
    }
    FK_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::create(const std::string& filename)
{
    Sprite *sprite = new (std::nothrow) Sprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    FK_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::create(const std::string& filename, const Rect& rect)
{
    Sprite *sprite = new (std::nothrow) Sprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    FK_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::create()
{
    Sprite *sprite = new (std::nothrow) Sprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    FK_SAFE_DELETE(sprite);
    return nullptr;
}

bool Sprite::init(void)
{
    return initWithTexture(nullptr, RectZero );
}

bool Sprite::initWithTexture(Texture2D *texture)
{
    FKAssert(texture != nullptr, "Invalid texture for sprite");

    Rect rect = MakeRect(0,0,0,0);
    rect.size = texture->getContentSize();

    return initWithTexture(texture, rect);
}

bool Sprite::initWithTexture(Texture2D *texture, const Rect& rect)
{
    return initWithTexture(texture, rect, false);
}

bool Sprite::initWithFile(const std::string& filename)
{
    FKAssert(filename.size()>0, "Invalid filename for sprite");

    Texture2D *texture = new Texture2D();
	texture->initWithFile(filename.c_str());
    if (texture)
    {
        Rect rect = RectZero;
        rect.size = texture->getContentSize();
        return initWithTexture(texture, rect);
    }

    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release();
    return false;
}

bool Sprite::initWithFile(const std::string &filename, const Rect& rect)
{
    FKAssert(filename.size()>0, "Invalid filename");

    Texture2D *texture = new Texture2D();
	texture->initWithFile(filename.c_str());
    if (texture)
    {
        return initWithTexture(texture, rect);
    }

    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release();
    return false;
}

// designated initializer
bool Sprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    bool result;
    if (Entity::init())
    {
        //_batchNode = nullptr;
        
        _recursiveDirty = false;
        setDirty(false);
        
        _opacityModifyRGB = true;
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        
        _flippedX = _flippedY = false;
        
        // default transform anchor: center
        setAnchorPoint(Point(0.5f, 0.5f));
        
        // zwoptex default values
        _offsetPosition = PointZero;

        // add vbo 3+4+2
        _vbo = VBO::create(VERTEX_SIZE,VERTICES_PER_SPRITE);
        
		GLProgram* program = new GLProgram();
		program->initWithByteArrays(Shader::PositionTextureColor_noMVP_vert,Shader::PositionTextureColor_noMVP_frag);
        // shader state
        setGLProgram(program);

        // update texture (calls updateBlendFunc)
        setTexture(texture);
        setTextureRect(rect, rotated, rect.size);
        
        // by default use "Self Render".
        // if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
        //setBatchNode(nullptr);
        result = true;
    }
    else
    {
        result = false;
    }
    _recursiveDirty = true;
    setDirty(true);
    return result;
}

Sprite::Sprite(void)
: _shouldBeHidden(false)
, _texture(nullptr)
, _insideBounds(true)
//, _batchNode(nullptr)
{
#if FK_SPRITE_DEBUG_DRAW
    _debugDrawNode = DrawEntity::create();
    addChild(_debugDrawNode);
#endif //FK_SPRITE_DEBUG_DRAW
}

Sprite::~Sprite(void)
{
    FK_SAFE_RELEASE(_texture);
}

/*
 * Texture methods
 */

/*
 * This array is the data of a white image with 2 by 2 dimension.
 * It's used for creating a default texture when sprite's texture is set to nullptr.
 * Supposing codes as follows:
 *
 *   auto sp = new (std::nothrow) Sprite();
 *   sp->init();  // Texture was set to nullptr, in order to make opacity and color to work correctly, we need to create a 2x2 white texture.
 *
 * The test is in "TestCpp/SpriteTest/Sprite without texture".
 */
static unsigned char fk_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define FK_2x2_WHITE_IMAGE_KEY  "/fk_2x2_white_image"

// MARK: texture
void Sprite::setTexture(const std::string &filename)
{
    Texture2D *texture = new Texture2D();
	texture->initWithFile(filename.c_str());

    setTexture(texture);

    Rect rect = RectZero;
    rect.size = texture->getContentSize();
    setTextureRect(rect);
}

void Sprite::setTexture(Texture2D *texture)
{
    // accept texture==nil as argument
    FKAssert( !texture || dynamic_cast<Texture2D*>(texture), "setTexture expects a Texture2D. Invalid argument");

    if (texture == nullptr)
    {
        // Gets the texture by key firstly.
        texture = new Texture2D();
		texture->initData(fk_2x2_white_image,16);

        // If texture wasn't in cache, create it from RAW data.
        if (texture == nullptr)
        {
            Image* image = new (std::nothrow) Image();
            bool isOK = image->initWithRawData(fk_2x2_white_image, sizeof(fk_2x2_white_image), 2, 2, 8);
            FK_UNUSED_PARAM(isOK);
            FKAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");

            //texture = Director::getInstance()->getTextureCache()->addImage(image, FK_2x2_WHITE_IMAGE_KEY);
			delete(image );
            //FK_SAFE_RELEASE(image);
        }
    }

    if (_texture != texture)
    {
        FK_SAFE_RETAIN(texture);
        FK_SAFE_RELEASE(_texture);
        _texture = texture;
		
        updateBlendFunc();
    }
}

Texture2D* Sprite::getTexture() const
{
    return _texture;
}

void Sprite::setTextureRect(const Rect& rect)
{
    setTextureRect(rect, false, rect.size);
}

void Sprite::setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize)
{
    _rectRotated = rotated;

    setContentSize(untrimmedSize);
    setVertexRect(rect);
    setTextureCoords(rect);


    _offsetPosition.x = (contentSize.width - _rect.size.width) / 2;
    _offsetPosition.y =relativeOffset.y + (contentSize.height - _rect.size.height) / 2;

    // self rendering
        
    // Atlas: Vertex
    float x1 = 0 + _offsetPosition.x;
    float y1 = 0 + _offsetPosition.y;
    float x2 = x1 + _rect.size.width;
    float y2 = y1 + _rect.size.height;
    
	/*
	* (x1,y1)_____
	  |           |
	  |___________(x2,y2)|	
	*/

	//leftTop bottomRight;
	float vertexs[] = { x1,y1,0,
						x1,y2,0,
						x2,y2,0
						x2,y1,0};
	_vbo->updateData(VERTEX_INDEX,3,vertexs);
	
}

// override this method to generate "double scale" sprites
void Sprite::setVertexRect(const Rect& rect)
{
    _rect = rect;
}

void Sprite::setTextureCoords(Rect rect)
{
    rect = FK_RECT_POINTS_TO_PIXELS(rect);

    Texture2D *tex = _texture;
    if (! tex)
    {
        return;
    }

    float atlasWidth = (float)tex->getPixelsWidth();
    float atlasHeight = (float)tex->getPixelsHeight();

    float left, right, top, bottom;

    if (_rectRotated)
    {
#if FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right   = left+(rect.size.height*2-2)/(2*atlasWidth);
        top     = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom  = top+(rect.size.width*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right   = (rect.origin.x+rect.size.height) / atlasWidth;
        top     = rect.origin.y/atlasHeight;
        bottom  = (rect.origin.y+rect.size.width) / atlasHeight;
#endif // FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL

        if (_flippedX)
        {
            FK_SWAP(top, bottom, float);
        }

        if (_flippedY)
        {
            FK_SWAP(left, right, float);
        }

        /*_quad.bl.texCoords.u = left;
        _quad.bl.texCoords.v = top;
        _quad.br.texCoords.u = left;
        _quad.br.texCoords.v = bottom;
        _quad.tl.texCoords.u = right;
        _quad.tl.texCoords.v = top;
        _quad.tr.texCoords.u = right;
        _quad.tr.texCoords.v = bottom;*/

		float texCoords[] = {left,top,
							left,bottom,
							right,top,
							right,bottom};
		_vbo->updateData(TEXTURECOORDINATES_INDEX,2,texCoords);
    }
    else
    {
#if FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right    = left + (rect.size.width*2-2)/(2*atlasWidth);
        top        = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom    = top + (rect.size.height*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right    = (rect.origin.x + rect.size.width) / atlasWidth;
        top        = rect.origin.y/atlasHeight;
        bottom    = (rect.origin.y + rect.size.height) / atlasHeight;
#endif // ! FK_FIX_ARTIFACTS_BY_STRECHING_TEXEL

        if(_flippedX)
        {
            FK_SWAP(left,right,float);
        }

        if(_flippedY)
        {
            FK_SWAP(top,bottom,float);
        }


    }
}

// MARK: visit, draw, transform

void Sprite::updateTransform(void)
{
    //FKASSERT(_batchNode, "updateTransform is only valid when Sprite is being rendered using an SpriteBatchNode");

    // recalculate matrix only if it is dirty
    if( isDirty() ) 
	{

        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !visible || ( parent && static_cast<Sprite*>(parent)->visible == false) )
        {
      
        }
        else
        {

            //
            // calculate the Quad based on the Affine Matrix
            //

            Size &size = _rect.size;

            float x1 = _offsetPosition.x;
            float y1 = _offsetPosition.y;

            float x2 = x1 + size.width;
            float y2 = y1 + size.height;
            float x = _transformToBatch.m[12];
            float y = _transformToBatch.m[13];

            float cr = _transformToBatch.m[0];
            float sr = _transformToBatch.m[1];
            float cr2 = _transformToBatch.m[5];
            float sr2 = -_transformToBatch.m[4];
            float ax = x1 * cr - y1 * sr2 + x;
            float ay = x1 * sr + y1 * cr2 + y;

            float bx = x2 * cr - y1 * sr2 + x;
            float by = x2 * sr + y1 * cr2 + y;

            float cx = x2 * cr - y2 * sr2 + x;
            float cy = x2 * sr + y2 * cr2 + y;

            float dx = x1 * cr - y2 * sr2 + x;
            float dy = x1 * sr + y2 * cr2 + y;

            _quad.bl.vertices = Vec3( RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), _positionZ );
            _quad.br.vertices = Vec3( RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), _positionZ );
            _quad.tl.vertices = Vec3( RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), _positionZ );
            _quad.tr.vertices = Vec3( RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), _positionZ );
        }

        // MARMALADE CHANGE: ADDED CHECK FOR nullptr, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
        if (_textureAtlas)
		{
            _textureAtlas->updateQuad(&_quad, _atlasIndex);
        }

        _recursiveDirty = false;
        setDirty(false);
    }

    // MARMALADE CHANGED
    // recursively iterate over children
	/* if( _hasChildren )
    {
        // MARMALADE: CHANGED TO USE Node*
        // NOTE THAT WE HAVE ALSO DEFINED virtual Entity::updateTransform()
        arrayMakeObjectsPerformSelector(_children, updateTransform, Sprite*);
    }*/
    Entity::updateTransform();
}

// draw
void Sprite::draw()
{
	/*
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = transformDirty ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;

    if(_insideBounds)
    {
        _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
        renderer->addCommand(&_quadCommand);
    }
	*/
	
	_vbo->onBufferData();
	_texture

}

// MARK: visit, draw, transform

void Sprite::addChild(Entity *child, int zOrder, int tag)
{
    FKAssert(child != nullptr, "Argument must be non-nullptr");

	/*
    if (_batchNode)
    {
        Sprite* childSprite = dynamic_cast<Sprite*>(child);
        FKAssert( childSprite, "Sprite only supports Sprites as children when using SpriteBatchNode");
        FKAssert(childSprite->getTexture()->getName() == _textureAtlas->getTexture()->getName(), "");
        //put it in descendants array of batch node
        _batchNode->appendChild(childSprite);

        if (!_reorderChildDirty)
        {
            setReorderChildDirtyRecursively();
        }
    }*/

    //Entity already sets isReorderChildDirty_ so this needs to be after batchNode check
    Entity::addChild(child, zOrder, tag);
}

/*void Sprite::addChild(Entity *child, int zOrder, const std::string &name)
{
    FKAssert(child != nullptr, "Argument must be non-nullptr");
    
    if (_batchNode)
    {
        Sprite* childSprite = dynamic_cast<Sprite*>(child);
        FKAssert( childSprite, "CCSprite only supports Sprites as children when using SpriteBatchNode");
        FKAssert(childSprite->getTexture()->getName() == _textureAtlas->getTexture()->getName(), "");
        //put it in descendants array of batch node
        _batchNode->appendChild(childSprite);
        
        if (!_reorderChildDirty)
        {
            setReorderChildDirtyRecursively();
        }
    }
    //Entity already sets isReorderChildDirty_ so this needs to be after batchNode check
    Entity::addChild(child, zOrder, name);
}*/

void Sprite::reorderChild(Entity *child, int zOrder)
{
    FKAssert(child != nullptr, "child must be non null");
    FKAssert(_children.contains(child), "child does not belong to this");

    /*if( _batchNode && ! _reorderChildDirty)
    {
        setReorderChildDirtyRecursively();
        _batchNode->reorderBatch(true);
    }*/

    Entity::reorderChild(child, zOrder);
}

void Sprite::removeChild(Entity *child, bool cleanup)
{
    /*if (_batchNode)
    {
        _batchNode->removeSpriteFromAtlas((Sprite*)(child));
    }*/

    Entity::removeChild(child, cleanup);
}

void Sprite::removeAllChildrenWithCleanup(bool cleanup)
{
    /*if (_batchNode)
    {
        for(const auto &child : _children) {
            Sprite* sprite = dynamic_cast<Sprite*>(child);
            if (sprite)
            {
                _batchNode->removeSpriteFromAtlas(sprite);
            }
        }
    }*/

    Entity::removeAllChildrenWithCleanup(cleanup);
}

/*
void Sprite::sortAllChildren()
{
    if (_reorderChildDirty)
    {
        std::sort(std::begin(children), std::end(children), nodeComparisonLess);

        if ( _batchNode)
        {
            for(const auto &child : _children)
                child->sortAllChildren();
        }

        _reorderChildDirty = false;
    }
}
*/
//
// Entity property overloads
// used only when parent is SpriteBatchNode
//

void Sprite::setReorderChildDirtyRecursively(void)
{
    //only set parents flag the first time
    if ( ! childrenSortPending)
    {
        childrenSortPending = true;
        Entity* entity = static_cast<Entity*>(parent);
        while (entity)// && node != _batchNode)
        {
            static_cast<Sprite*>(entity)->setReorderChildDirtyRecursively();
            entity=entity->getParent();
        }
    }
}

void Sprite::setDirtyRecursively(bool bValue)
{
    _recursiveDirty = bValue;
    setDirty(bValue);

    for(const auto &child: children) {
        Sprite* sp = dynamic_cast<Sprite*>(child);
        if (sp)
        {
            sp->setDirtyRecursively(true);
        }
    }
}

// FIXME: HACK: optimization
#define SET_DIRTY_RECURSIVELY() {                       \
                    if (! _recursiveDirty) {            \
                        _recursiveDirty = true;         \
                        setDirty(true);                 \
                        if (!children.empty())         \
                            setDirtyRecursively(true);  \
                        }                               \
                    }

void Sprite::setPosition(const Point& pos)
{
    Entity::setPosition(pos);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setPosition(float x, float y)
{
    Entity::setPosition(x, y);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotation(float rotation)
{
    Entity::setRotation(rotation);
    
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotationSkewX(float fRotationX)
{
    Entity::setRotationSkewX(fRotationX);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotationSkewY(float fRotationY)
{
    Entity::setRotationSkewY(fRotationY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setSkewX(float sx)
{
    Entity::setSkewX(sx);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setSkewY(float sy)
{
    Entity::setSkewY(sy);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScaleX(float scaleX)
{
    Entity::setScaleX(scaleX);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScaleY(float scaleY)
{
    Entity::setScaleY(scaleY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScale(float fScale)
{
    Entity::setScale(fScale);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScale(float scaleX, float scaleY)
{
    Entity::setScale(scaleX, scaleY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setPositionZ(float fVertexZ)
{
    Entity::setPositionZ(fVertexZ);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setAnchorPoint(const Vec2& anchor)
{
    Entity::setAnchorPoint(anchor);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::ignoreAnchorPointForPosition(bool value)
{
    FKAssert(! _batchNode, "ignoreAnchorPointForPosition is invalid in Sprite");
    Entity::ignoreAnchorPointForPosition(value);
}

void Sprite::setVisible(bool bVisible)
{
    Entity::setVisible(bVisible);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setFlippedX(bool flippedX)
{
    if (_flippedX != flippedX)
    {
        _flippedX = flippedX;
        setTextureRect(_rect, _rectRotated, _contentSize);
    }
}

bool Sprite::isFlippedX(void) const
{
    return _flippedX;
}

void Sprite::setFlippedY(bool flippedY)
{
    if (_flippedY != flippedY)
    {
        _flippedY = flippedY;
        setTextureRect(_rect, _rectRotated, _contentSize);
    }
}

bool Sprite::isFlippedY(void) const
{
    return _flippedY;
}

//
// MARK: RGBA protocol
//

void Sprite::updateColor(void)
{
    float red = color.red;
	float green = color.green;
	float blue = color.blue;
	float alpha = color.alpha;
    
    // special opacity for premultiplied textures
	if (_opacityModifyRGB)
    {
		red *= alpha;
		green *= alpha;
		blue *= alpha;
    }

	float colors[] = {red,green,blue,alpha,
					red,green,blue,alpha,
					red,green,blue,alpha,
					red,green,blue,alpha}
	vbo->updateData(COLOR_INDEX,4,colors);
    // self render
    // do nothing

}

void Sprite::setOpacityModifyRGB(bool modify)
{
    if (_opacityModifyRGB != modify)
    {
        _opacityModifyRGB = modify;
        updateColor();
    }
}

bool Sprite::isOpacityModifyRGB(void) const
{
    return _opacityModifyRGB;
}

/*
SpriteBatchNode* Sprite::getBatchNode()
{
    return _batchNode;
}


void Sprite::setBatchNode(SpriteBatchNode *spriteBatchNode)
{
    _batchNode = spriteBatchNode; // weak reference

    // self render
    if( ! _batchNode ) {
        _atlasIndex = INDEX_NOT_INITIALIZED;
        setTextureAtlas(nullptr);
        _recursiveDirty = false;
        setDirty(false);

        float x1 = _offsetPosition.x;
        float y1 = _offsetPosition.y;
        float x2 = x1 + _rect.size.width;
        float y2 = y1 + _rect.size.height;
        _quad.bl.vertices = Vec3( x1, y1, 0 );
        _quad.br.vertices = Vec3( x2, y1, 0 );
        _quad.tl.vertices = Vec3( x1, y2, 0 );
        _quad.tr.vertices = Vec3( x2, y2, 0 );

    } else {

        // using batch
        _transformToBatch = Mat4::IDENTITY;
        setTextureAtlas(_batchNode->getTextureAtlas()); // weak ref
    }
}
*/
// MARK: Texture protocol

void Sprite::updateBlendFunc(void)
{
    //FKAssert(! _batchNode, "Sprite: updateBlendFunc doesn't work when the sprite is rendered using a SpriteBatchNode");

    // it is possible to have an untextured sprite
    if (! _texture || ! _texture->hasPremultipliedAlpha())
    {
        blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
        setOpacityModifyRGB(false);
    }
    else
    {
        blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        setOpacityModifyRGB(true);
    }
}

std::string Sprite::toString() const
{
    int texture_id = -1;
    /*if( _batchNode )
        texture_id = _batchNode->getTextureAtlas()->getTexture()->getName();
    else*/
        texture_id = _texture->getTextureID();
    return String::createWithFormat("<Sprite | Tag = %d, TextureID = %d>", _tag, texture_id )->m_sString;
}

FLAKOR_NS_END

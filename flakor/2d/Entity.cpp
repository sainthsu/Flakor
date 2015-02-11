#include "targetMacros.h"
#include "2d/Entity.h"
#include "base/lang/Str.h"
#include "core/input/Touch.h"
#include "math/GLMatrix.h"

#if FK_ENTITY_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

FLAKOR_NS_BEGIN

int Entity::globalOrderOfArrival = 1;

Entity::Entity(void)
: position(PointZero)
, contentSize(SizeZero)
, rotationX(0.0f)
, rotationY(0.0f)
, scaleX(1.0f)
, scaleY(1.0f)
, vertexZ(0.0f)
, skewX(0.0f)
, skewY(0.0f)
, anchorPointInPixels(PointZero)
, anchorPoint(PointZero)
, additionalMatrix()
, camera(NULL)	  
, zOrder(0)
, orderOfArrival(0)
// children (lazy allocs)
, children(NULL)
, parent(NULL)
, tag(Entity::TAG_INVALID)
// userData is always inited as null
, userData(NULL)
, cullingEnabled(true)
, running(false)
, transformDirty(true)
, inverseDirty(true)
, additionalTransformDirty(false)
, visible(true)
// "whole screen" objects. like Scenes and Layers, should set relativeAnchorPoint to false
, relativeAnchorPoint(false)
, childrenVisible(true)
, childrenSortPending(false)
, anchorPointAsCenter(true)
//, scriptHandler(0)
//, updateScriptHandler(0)
{
}

Entity::~Entity(void)
{
	FKLOG("FLAKOR:deallocing");
	//unregisterScriptHandler
	FK_SAFE_RELEASE(camera);
	FK_SAFE_RELEASE(userObject);
}

//overwrite this method for your own init action
bool Entity::init(void)
{
	return true;
}

Entity * Entity::create()
{
	Entity * entity = new Entity();
	if (entity && entity->init())
	{
		entity->autorelease();
	}
	else
	{
		FK_SAFE_DELETE(entity);
	}
	return entity;
}

Entity * Entity::create(float x,float y)
{
	Entity *entity = create();
	if(entity != NULL)
	{
		entity->setPosition(x,y);
	}

	return entity;
}

String* Entity::toString() const
{
    return String::createWithFormat("{Entity: | Tag = %d}", tag);
}

void Entity::setPosition(const Point &position)
{
	this->position = position;
	transformDirty = inverseDirty = true;
}

void Entity::setPosition(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	transformDirty = inverseDirty = true;
}

Point& Entity::getPosition()
{
	return this->position;
}

void Entity::getPosition(float* x, float* y)
{
	*x = this->position.x;
	*y = this->position.y;
}

void  Entity::setPositionX(float x)
{
	this->position.x = x;
	transformDirty = inverseDirty = true;
}

float Entity::getPositionX(void)
{
	return this->position.x;
}

void  Entity::setPositionY(float y)
{
	this->position.y = y;
	transformDirty = inverseDirty = true;
}

float Entity::getPositionY(void)
{
	return this->position.y;
}

void Entity::setRelativeAnchorPoint(bool relative)
{
	if(relativeAnchorPoint != relative)
	{
		relativeAnchorPoint = relative;
		transformDirty = inverseDirty = true;
	}
}

bool Entity::isRelativeAnchorPoint()
{
	return relativeAnchorPoint;
}

const Point& Entity::getAnchorPointInPixels()
{
	//new MakePoint(contentSize.width*anchorPoint.x , contentSize.height*anchorPoint.y);
	return anchorPointInPixels;
}

const Size& Entity::getContentSize() const
{
	return contentSize;
}

void Entity::setContentSize(const Size& size)
{
	this->contentSize = contentSize;
}

const Point& Entity::getAnchorPoint()
{
	return anchorPoint;
}

void Entity::setAnchorPoint(const Point &point)
{
	anchorPoint = point;
}

void Entity::setAnchorPointAsCenter(bool use)
{
	anchorPointAsCenter = use;
}

bool Entity::isAnchorPointAsCenter() const
{
	return anchorPointAsCenter;
}

void Entity::setZOrder(int z)
{
	_setZOrder(z);
	if(parent != NULL)
	{
		parent->reorderChild(this,z);
	}
}

void Entity::_setZOrder(int z)
{
	zOrder = z;
}

int Entity::getZOrder()
{
	return zOrder;
}

void Entity::setOrderOfArrival(int arrival)
{
	orderOfArrival = arrival;
}

int Entity::getOrderOfArrival(void) const
{
	return orderOfArrival;
}

bool Entity::hasParent()
{
	return (parent != NULL);
}

void Entity::setParent(Entity *parent)
{
	this->parent = parent;
}

Entity* Entity::getParent()
{
	return parent;
}

void Entity::setVertexZ(float z)
{
	vertexZ = z;
}

float Entity::getVertexZ()
{
	return vertexZ;
}

bool Entity::isRotated()
{
	return (rotationX != 0.f) || (rotationY != 0.f) ;
}

float Entity::getRotation()
{
	FKAssert(rotationX == rotationY, "Entity#rotation. rotationX != rotationY. Don't know which one to return!");
	return rotationX;
}

void Entity::setRotation(float rotation)
{
	rotationX = rotationY = rotation;
	transformDirty = inverseDirty = true;
}

void Entity::setRotationX(float x)
{
	rotationX = x;
	transformDirty = inverseDirty = true;
}

float Entity::getRotationX()
{
	return rotationX;
}

void Entity::setRotationY(float y)
{
	rotationY = y;
	transformDirty = inverseDirty = true;
}

float Entity::getRotationY()
{
	return rotationY;
}

bool Entity::isScaled()
{
	return (scaleX != 0.f) || (scaleY != 0.f);
}

void Entity::setScaleX(float x)
{
	scaleX = x;
	transformDirty = inverseDirty = true;
}

float Entity::getScaleX()
{
	return scaleX;
}

void Entity::setScaleY(float y)
{
	scaleY = y;
	transformDirty = inverseDirty = true;
}

float Entity::getScaleY()
{
	return scaleY;	
}

void Entity::setScale(float scale)
{
	scaleX = scaleY = scale;
	transformDirty = inverseDirty = true;
}

float Entity::getScale()
{
	FKAssert(scaleX == scaleY, "Entity#scale. scaleX != scaleY. Don't know which one to return!");
	return scaleX;
}

void Entity::setScale(float x,float y)
{
	scaleX = x;
	scaleY = y;
	transformDirty = inverseDirty = true;
}

bool Entity::isSkewed()
{
	return (skewX != 0) || (skewY != 0);
}

void Entity::setSkewX(float x)
{
	skewX = x;
	transformDirty = inverseDirty = true;
}

float Entity::getSkewX()
{
	return skewX;
}

void Entity::setSkewY(float y)
{
	skewY = y;
	transformDirty = inverseDirty = true;
}

float Entity::getSkewY()
{
	return skewY;
}

//maybe not need this two method
void Entity::setVisible(bool visible)
{
	this->visible = visible;
}

bool Entity::isVisible()
{
	return this->visible;
}

void Entity::setChildrenVisible(bool visible)
{
	childrenVisible = visible;
}

bool Entity::isChildrenVisible()
{
	return childrenVisible;
}

void Entity::setChildrenIgnoreUpdate(bool visible)
{
	childrenIgnoreUpdate = visible;
}

bool Entity::isChildrenIgnoreUpdate()
{
	return childrenIgnoreUpdate;
}

void Entity::addChild(Entity * child)
{
	addChild(child,child->zOrder,child->tag);
}

void Entity::addChild(Entity * child, int zOrder)
{
	addChild(child,zOrder,child->tag);
}

void Entity::addChild(Entity* child, int zOrder, int tag)
{
	FKAssert( child != NULL, "Child argument must be non-nil");
	FKAssert( child->parent == NULL, "child already added. It can't be added again");

	if( ! children )
	{
		this->childrenAlloc();
	}

	this->insertChild(child, zOrder);

	child->tag = tag;

	child->setParent(this);
	child->setOrderOfArrival(globalOrderOfArrival++);

	if( running )
	{
		child->onEnter();
		child->onEnterTransitionDidFinish();
	}
}

Entity* Entity::getChildByTag(int tag)
{
	//FKAssert();
	if ( children && children->count() > 0 )
	{
		Object *child;
		FK_ARRAY_FOREACH(children,child)
		{
			Entity *entity = (Entity *)child;
			if(entity && entity->tag == tag)
				return entity;
		}
	}
	return NULL;
}

Entity* Entity::getChildByZOrder(int z)
{
	if ( children && children->count() > 0 )
	{
		Object *child;
		FK_ARRAY_FOREACH(children,child)
		{
			Entity *entity = (Entity *)child;
			if(entity && entity->zOrder == z)
				return entity;
		}
	}
	return NULL;
}

Entity* Entity::getFirstChild()
{
	return (Entity *)children->firstObject();
}

Entity* Entity::getLastChild()
{
	return (Entity *)children->lastObject();
}

Array* Entity::getChildren()
{
	return children;
}

unsigned int Entity::getChildrenCount(void) const
{
	return children->count();
}

void Entity::removeChild(Entity* child)
{
	removeChild(child,true);
}

void Entity::removeChild(Entity* child, bool cleanup)
{
	if(child == NULL)
	{
		return;
	}

	if ( children->containsObject(child) )
	{
		this->detachChild(child,cleanup);
	}

}

void Entity::removeChildByTag(int tag)
{
	removeChildByTag(tag,true);
}

void Entity::removeChildByTag(int tag,bool cleanup)
{
	FKAssert( tag != Entity::TAG_INVALID, "Invalid tag");

	Entity *child = this->getChildByTag(tag);

	if (child == NULL)
	{
		FKLOG("flakor: removeChildByTag(tag = %d): child not found!", tag);
	}
	else
	{
		this->removeChild(child, cleanup);
	}
}

void Entity::removeChildByZOrder(int z)
{
	Entity *child = this->getChildByZOrder(z);

	if (child == NULL)
	{
		FKLOG("flakor: removeChildByZOrder(ZOrder = %d): child not found!", tag);
	}
	else
	{
		this->removeChild(child, true);
	}

}

void Entity::removeAllChildren()
{
	removeAllChildren(true);
}

void Entity::removeAllChildren(bool cleanup)
{
	// not using detachChild improves speed here
	if (children && children->count() > 0 )
	{
		Entity* child;
		int count = children->count();
		
	    for(int i = 0;i<count;child = (Entity*)children->data->arr[i],i++)
		{
			if (child)
			{
				// IMPORTANT:
				//  -1st do onExit
				//  -2nd cleanup
				if(running)
				{
					child->onExitTransitionDidStart();
					child->onExit();
				}

				if (cleanup)
				{
					child->cleanup();
				}
				// set parent nil at the end
				child->setParent(NULL);
			}
		}

		children->removeAllObjects();
	}
}

void Entity::reorderChild(Entity * child, int zOrder)
{
	FKAssert(child != NULL,"Child must be non-nil!");
	childrenSortPending = true;
	child->setOrderOfArrival(globalOrderOfArrival++);
	child->_setZOrder(zOrder);
}

void Entity::sortAllChildren()
{
	sortAllChildren(true);
}

void Entity::sortAllChildren(bool immediate)
{
	if(immediate)
	{
		int i,j,length = children->data->num;
		Entity **data=  (Entity **)children->data->arr;
		Entity *tmp;

		//insertion sort
		for(i=1;i<length;i++)
		{
			tmp = data[i];
			j=i-1;

			while(j>=0 && ((tmp->zOrder < data[j]->zOrder)  ||
						(tmp->zOrder== data[j]->zOrder && tmp->orderOfArrival < data[j]->orderOfArrival ) ))
			{
				data[j+1] = data[j];
				j--;
			}
			data[j+1] = tmp;
		}

		childrenSortPending = false;
	}
	else
	{
		childrenSortPending = true;
	}

}

int Entity::getTag() const
{
	return tag;
}

void Entity::setTag(int tag)
{	
	this->tag = tag;
}

void* Entity::getUserData() const
{
	return userData;
}

void Entity::setUserData(void *data)
{
	this->userData = data;
}

Object* Entity::getUserObject() const
{
	return userObject;
}

void Entity::setUserObject(Object *pUserObject)
{
	userObject = pUserObject;
}

void Entity::setAddtionalMatrix(Matrix4& matrix)
{
	this->additionalMatrix = matrix;
	transformDirty = true;
	additionalTransformDirty = true;
}

Matrix4 Entity::getAddtionalMatrix()
{
	return this->additionalMatrix;
}


void Entity::setCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* Entity::getCamera() const
{
	return camera;
}


bool Entity::isRunning()
{
	return running;
}

/**
 * @brief Entity::onEnter enter this entity
 */
void Entity::onEnter()
{
	running = true;

	/* finish it later
	   if (scriptType != scriptTypeNone)
	   {
	   ScriptEngineManager::sharedManager()->getScriptEngine()->executeEntityEvent(this, EntityOnEnter);
	   }*/

	//Judge the running state for prevent called onEnter method more than once,it's possible that this function called by addChild
	if (children && children->count() > 0)
	{
		Object* child;
		Entity* entity;
		FK_ARRAY_FOREACH(children, child)
		{
			entity = (Entity*)child;
			if (!entity->isRunning())
			{
				entity->onEnter();
			}
		}
	}

	//this->resumeSchedulerAndActions();
}

void Entity::onEnterTransitionDidFinish()
{
	/*
	   if (scriptType != ScriptTypeNone)
	   {
	   ScriptEngineManager::sharedManager()->getScriptEngine()->executeEntityEvent(this, EntityOnEnterTransitionDidFinish);
	   }*/

	arrayMakeObjectsPerformSelector(children, onEnterTransitionDidFinish, Entity*);
}

void Entity::onExit()
{
	//this->pauseSchedulerAndActions();

	running = false;

	arrayMakeObjectsPerformSelector(children, onExit, Entity*);

	/*
	   if ( scriptType != ScriptTypeNone)
	   {
	   ScriptEngineManager::sharedManager()->getScriptEngine()->executeEntityEvent(this, EntityOnExit);
	   }*/
}

void Entity::onExitTransitionDidStart()
{
	arrayMakeObjectsPerformSelector(children, onExitTransitionDidStart, Entity*);
}

void Entity::cleanup(void)
{
	arrayMakeObjectsPerformSelector(children,cleanup,Entity*);
}

void Entity::draw(void)
{
	//	overwrite to handle your own draw action
    FKLOG("in base entity draw");
}

void Entity::onVisit(void)
{
	if(!visible)
	{
		return;
	}

    GLMode(GL_MODELVIEW);
    GLPush();

    this->transform();
    
	if(children == NULL || children->count() <=0 || !this->childrenVisible)
	{
		this->draw();
	}
	else
	{
		if(this->childrenSortPending)
		{
			sortAllChildren(true);
		}
		int childCount = children->count();
		int i = 0;
		Entity* child = NULL;

        FKLOG("children count:%d",childCount);
		//draw children behind this entity
		for(;i<childCount;i++)
		{
			child = (Entity *)children->data->arr[i];
			if(child && child->zOrder < 0)
			{
				child->onVisit();
			}
			else
			{
				break;
			}
		}

		//draw self
		this->draw();

		//draw children in font of this entity
		for(;i<childCount;i++)
		{
			child = (Entity *)children->data->arr[i];
			if(child)
			{
				child->onVisit();
			}
		}

	}
    
    GLMode(GL_MODELVIEW);
    GLPop();
}

/**
 * call it when entity attach to a parent
 * */
void Entity::onAttached()
{

}

void Entity::onDetached()
{

}

void Entity::update(float delta)
{
	if(ignoreUpdate)
		return;
    if(children == NULL || children->count() <=0 || this->childrenIgnoreUpdate)
	{
		this->onUpdate(delta);
	}
	else
	{
            Object* child;
            Entity* entity;
            FK_ARRAY_FOREACH(children, child)
            {
                entity = (Entity*)child;
                if (!entity->isRunning())
                {
                    entity->update(delta);
                }
            }
	}
	
}

void Entity::transform(void)
{
	Matrix4 transfrom4x4;

	// Convert 3x3 into 4x4 matrix
	transfrom4x4 = this->entityToParentTransform();

	// Update Z vertex manually
	transfrom4x4[14] = vertexZ;
    
    FKLOG("entity matrix:%s",transfrom4x4.toString());

    GLMode(GL_MODELVIEW);
	GLMultiply( &transfrom4x4 );

	// XXX: Expensive calls. Camera should be integrated into the cached affine matrix
	if ( camera != NULL)/// && !(grid != NULL && grid->isActive()) )
	{
        
		bool translate = (anchorPointInPixels.x != 0.0f || anchorPointInPixels.y != 0.0f);

		if( translate )
			GLTranslatef(RENDER_IN_SUBPIXEL(anchorPointInPixels.x), RENDER_IN_SUBPIXEL(anchorPointInPixels.y), 0 );

		camera->locate();

		if( translate )
			GLTranslatef(RENDER_IN_SUBPIXEL(-anchorPointInPixels.x), RENDER_IN_SUBPIXEL(-anchorPointInPixels.y), 0 );
         
	}

}

void Entity::transformAncestors(void)
{
	if(parent != NULL)
	{
		parent->transformAncestors();
		parent->transform();
	}
}

void Entity::updateTransform(void)
{
	// Recursively iterate over children
	arrayMakeObjectsPerformSelector(children, updateTransform, Entity*);
}

void Entity::childrenAlloc(void)
{
	children = Array::createWithCapacity(4);
	children->retain();
}

void Entity::insertChild(Entity* child, int z)
{
	childrenSortPending = true;
	fkArrayAppendObjectWithResize(children->data, child);
	child->_setZOrder(z);
}

void Entity::detachChild(Entity *child, bool doCleanup)
{
	if(running)
	{
		child->onExitTransitionDidStart();
		child->onExit();
	}
	if(doCleanup)
	{
		child->cleanup();
	}

	child->setParent(NULL);

	children->removeObject(child);
}

Matrix4 Entity::entityToParentTransform(void)
{
	if (transformDirty)
	{

		if(anchorPointAsCenter)
		{
			// Translate values
			float x = position.x;
			float y = position.y;

			anchorPointInPixels.x = contentSize.width * anchorPoint.x;
			anchorPointInPixels.y = contentSize.height * anchorPoint.y;

			if (!relativeAnchorPoint)
			{
				x += anchorPointInPixels.x;
				y += anchorPointInPixels.y;
			}

			// Rotation values
			// Change rotation code to handle X and Y
			// If we skew with the exact same value for both x and y then we're simply just rotating
			float cx = 1, sx = 0, cy = 1, sy = 0;
			if (rotationX || rotationY)
			{
				float radiansX = -FK_DEGREES_TO_RADIANS(rotationX);
				float radiansY = -FK_DEGREES_TO_RADIANS(rotationY);
				cx = cosf(radiansX);
				sx = sinf(radiansX);
				cy = cosf(radiansY);
				sy = sinf(radiansY);
			}

			bool needSkewMatrix = ( skewX || skewY );

			// optimization:
			// inline anchor point calculation if skew is not needed
			// Adjusted transform calculation for rotational skew
			if (! needSkewMatrix && !anchorPointInPixels.equals(PointZero))
			{
				x += cy * -anchorPointInPixels.x * scaleX + -sx * -anchorPointInPixels.y * scaleY;
				y += sy * -anchorPointInPixels.x * scaleX +  cx * -anchorPointInPixels.y * scaleY;
			}


			// Build Transform Matrix
			// Adjusted transform calculation for rotational skew
			transformMatrix = Matrix4::make( cy * scaleX,  -sx * scaleY,0,x,
											sy *scaleX, cx * scaleY,0,y,
											0,0,1,0,
											0,0,0,1);

			// XXX: Try to inline skew
			// If skew is needed, apply skew and then anchor point
			if (needSkewMatrix)
			{
				Matrix4 skewMatrix = Matrix4::make(1.0f, tanf(FK_DEGREES_TO_RADIANS(skewX)),0.f,0.0f,
												tanf(FK_DEGREES_TO_RADIANS(skewY)), 1.0f,0.f,0.0f,
												0.0f, 0.0f ,1,0,
												0,0,0,1);
				transformMatrix = skewMatrix*transformMatrix;

				delete(&skewMatrix);			
				
				// adjust anchor point
				if (!anchorPointInPixels.equals(PointZero))
				{
					transformMatrix = transformMatrix.translate(-anchorPointInPixels.x, -anchorPointInPixels.y,0);
				}
			}
		}
		else
		{
            Matrix4 matrix = Matrix4::make(1.0f, 0.f,0.f,0.0f,
                                           0.0f, 1.0f,0.f,0.0f,
                                           0.0f, 0.0f ,1,0,
                                           0,0,0,1);
            
			matrix.translate(position.x,position.y,0);

			anchorPointInPixels.x = contentSize.width * anchorPoint.x;
			anchorPointInPixels.y = contentSize.height * anchorPoint.y;

			if (relativeAnchorPoint)
			{
				matrix.translate(-anchorPointInPixels.x,-anchorPointInPixels.y,0);
			}

			if(isScaled())
			{
				if(!scaleCenter.equals(PointZero))
				{
					matrix.scale(scaleX,scaleY , 0);
				}
				else
				{
					matrix.translate(-scaleCenter.x,-scaleCenter.y,0);
					matrix.scale(scaleX,scaleY,0);
					matrix.translate(scaleCenter.x,scaleCenter.y,0);
				}	
			}

			if(isRotated())
			{
				if(!rotationCenter.equals(PointZero))
				{
					//matrix.rotate(rotationX,rotationY,0);
				}
				else
				{
					matrix.translate(-rotationCenter.x,-rotationCenter.y,0);
					//matrix.rotate(rotationX,rotationY,0);
					matrix.translate(rotationCenter.x,rotationCenter.y,0);
				}	
			}

			if(isSkewed())
			{
				if(!skewCenter.equals(PointZero))
				{
					matrix.skew2D(skewX,skewY);
				}
				else
				{
					matrix.translate(-skewCenter.x,-skewCenter.y,0);
					matrix.skew2D(skewX,skewY);
					matrix.translate(skewCenter.x,skewCenter.y,0);
				}	
			}

			transformMatrix = matrix;
		}

		
		if (additionalTransformDirty)
		{
			transformMatrix =  transformMatrix * additionalMatrix; //??pre or post
			additionalTransformDirty = false;
		}

		transformDirty = false;
	}

	return transformMatrix;
}


Matrix4 Entity::parentToEntityTransform(void)
{
		Matrix4* inverse = NULL;
		if(transformDirty)
		{
			*inverse = this->entityToParentTransform().invert();
		}
		return *inverse;
}

Matrix4 Entity::entityToWorldTransform()
{
    Matrix4 t = this->entityToParentTransform();

    for (Entity *p = parent; p != NULL; p = p->getParent())
        t = t*(p->entityToParentTransform());

    return t;
}

Matrix4 Entity::worldToEntityTransform(void)
{
    return this->entityToWorldTransform().invert();
}

Point Entity::convertToEntitySpace(const Point& worldPoint)
{
    Point ret = worldToEntityTransform()*worldPoint;
    return ret;
}

Point Entity::convertToWorldSpace(const Point& entityPoint)
{
    Point ret =  entityToWorldTransform()*entityPoint;
    return ret;
}

Point Entity::convertToEntitySpaceAR(const Point& worldPoint)
{
    Point entityPoint = convertToEntitySpace(worldPoint);
    return entityPoint-anchorPointInPixels;
}

Point Entity::convertToWorldSpaceAR(const Point& entityPoint)
{
    Point pt = entityPoint+anchorPointInPixels;
    return convertToWorldSpace(pt);
}

/*
Point Entity::convertToWindowSpace(const Point& nodePoint)
{
    Point worldPoint = this->convertToWorldSpace(nodePoint);
    return Engine::shareEngine()->convertToUI(worldPoint);
}*/

// convenience methods which take a CCTouch instead of Point
Point Entity::convertTouchToEntitySpace(Touch *touch)
{
    Point point = touch->getLocation();
    return this->convertToEntitySpace(point);
}

Point Entity::convertTouchToEntitySpaceAR(Touch *touch)
{
    Point point = touch->getLocation();
    return this->convertToEntitySpaceAR(point);
}

void Entity::setColor(const Color& color)
{
	this->color = color;
}

void Entity::setColor(float red,float green,float blue)
{
	this->color.setColor(red,green,blue);
}

void Entity::setColor(float red,float green,float blue,float alpha)
{
	this->color.setColor(red,green,blue,alpha);
}

void Entity::setRed(float red)
{
	this->color.red = red;
}

void Entity::setGreen(float green)
{
	this->color.green = green;
}

void Entity::setBlue(float blue)
{
	this->color.blue = blue;
}

void Entity::setAlpha(float alpha)
{
	this->color.alpha = alpha;
}

Color& Entity::getColor()
{
	return color;
}

float Entity::getRed()
{
	return color.red;
}

float Entity::getGreen()
{
	return color.green;
}

float Entity::getBlue()
{
	return color.blue;
}

float Entity::getAlpha()
{
	return color.alpha;
}

void Entity::setIngnoreUpdate(bool ignore)
{
	ignoreUpdate = ignore;
}


bool Entity::getIngnoreUpdate()
{
	return ignoreUpdate;
}

void Entity::onUpdate(float delta)
{
	
}

void Entity::reset()
{
	
}

FLAKOR_NS_END


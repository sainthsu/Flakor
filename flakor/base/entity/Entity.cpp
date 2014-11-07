#include "Entity.h"

FLAKOR_NS_BEGIN

int Entity::s_globalOrderOfArrival = 1;

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
, obAnchorPointInPixels(PointZero)
, anchorPoint(PointZero)
, additionalTransform(Matrix4())
, camera(NULL)	  
, ZOrder(0)
, orderOfArrival(0)
// children (lazy allocs)
, children(NULL)
, parent(NULL)
, tag(EntityTagInvalid)
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
, childrenSortPending(false)
//, scriptHandler(0)
//, updateScriptHandler(0)
{
}

Entity::~Entity(void)
{
	FKLOG("FLAKOR:deallocing");
	//unregisterScriptHandler
	FK_SAFE_RELEASE(camera);
	FK_SAFE_RELEASE(userData);
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
		entity.setPosition(x,y);
	}

	return entity;
}

const char* Entity::toString(void)
{
	return String::createWithFormat("{Entity: | Tag = %d}", tag)->getCString();
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
	return PointMake(contentSize.width*anchorPoint.x , contentSize.height*anchorPoint.y);
}

const Size& Entity::getContentSize()
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
	ZOrder = z;
}

int Entity::getZOrder()
{
	return ZOrder;
}

void Entity::setOrderofArrival(int arrival)
{
	orderofArrival = arrival;
}

int Entity::getOrderofArrival(void)
{
	return orderofArruval;
}

bool Entity::hasParent()
{
	return (parent != NULL);
}

void Entity::setParent(const Entity *parent)
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
	return (rotationX != 0.f) || (rotation != 0.f) ;
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
	return ratationX;
}

void Entity::setRotationY(float y)
{
	rotationY = y;
	transformDirty = inverseDirty = true;
}

float Entity::getRotationY()
{
	return ratationY;
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
	return sclaleX;
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
	this.visible = visible;
}

bool Entity::isVisible()
{
	return this.visible;
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
	child->setOrderOfArrival(s_globalOrderOfArrival++);

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
	return children.firstObject();
}

Entity* Entity::getLastChild()
{
	return chilidren.lastObject();
}

Array* Entity::getChildren()
{
	return children;
}

unsigned int Entity::getChildrenCount(void)
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
	FKAssert( tag != EntityTagInvalid, "Invalid tag");

	Entity *child = this->getChildByTag(tag);

	if (child == NULL)
	{
		FK_LOG("flakor: removeChildByTag(tag = %d): child not found!", tag);
	}
	else
	{
		this->removeChild(child, cleanup);
	}
}

void Entity::removeChildByZOrder(int z)
{
	Entity *child = this->getChildByZOrder(tag);

	if (child == NULL)
	{
		FK_LOG("flakor: removeChildByZOrder(ZOrder = %d): child not found!", tag);
	}
	else
	{
		this->removeChild(child, cleanup);
	}

}

void Entity::removeAllChildren()
{
	removeAllChildren(true);
}

void Entity::removeAllChildren(bool cleanup)
{
	// not using detachChild improves speed here
	if ( children && children->count() > 0 )
	{
		Entity* child;
		FK_ARRAY_FOREACH(children, child)
		{
			Entity* entity = (Entity*) child;
			if (entity)
			{
				// IMPORTANT:
				//  -1st do onExit
				//  -2nd cleanup
				if(running)
				{
					entity->onExitTransitionDidStart();
					entity->onExit();
				}

				if (cleanup)
				{
					entity->cleanup();
				}
				// set parent nil at the end
				entity->setParent(NULL);
			}
		}

		children->removeAllObjects();
	}
}

void Entity::reorderChild(Entity * child, int zOrder)
{
	FKAssert(child != NULL,"Child must be non-nil!");
	childrenSortPending = true;
	child->setOrderOfArrival(s_globalOrderOfArrival++);
	child->_setZorder(zOrder);
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

			while(j>=0 && ((tmp->ZOrder < data[j]->ZOrder)  ||
						(temp->ZOrder== x[j]->ZOrder && temp->orderOfArrival < x[j]->orderOfArrival ) ))
			{
				data[j+1] = data[j];
				j--;
			}
			data[j+1] = tmp;
		}

		ChildrenSortPending = false;
	}
	else
	{
		ChildrenSortPending = true;
	}

}

int Entity::getTag()
{
	return tag;
}

void Entity::setTag(int tag)
{	
	this->tag = tag;
}

void* Entity::getUserData()
{
	return userData;
}

void Entity::setUserData(void *data)
{
	this->userData = data;
}

void Entity::setAddtionalMatrix(const Matrix4 matrix)
{
	this.addtionalMatrix = matrix;
	transformDirty = true;
	addtionalTransformDirty = true;
}

Matrix4* Entity::getAddtionalMatrix()
{
	return this.addtionalMatrix;
}

void Entity::setCamera(const Camera* camera);
{
	this.camera = camera;
}

Camera* Entity::getCamera()
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
		FKARRAY_FOREACH(children, child)
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

	arrayMakeObjectsPerformSelector(m_pChildren, onExit, Entity*);

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
}

void Entity::onVisit(void)
{
	if(!visible)
	{
		return;
	}

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
		Entity child = NULL;

		//draw children behind this entity
		for(;i<childCount;i++)
		{
			child = (Entity *)children->CArray->arr[i];
			if(child && child->ZOrder < 0)
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
			child = (Entity *)children->CArray->arr[i];
			if(child)
			{
				child->onVisit();
			}
		}

	}
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

}

void Entity::transform(void)
{
	Matrix4 transfrom4x4;

	// Convert 3x3 into 4x4 matrix
	transfrom4x4 = this->entityToParentTransform();

	// Update Z vertex manually
	transfrom4x4.m[14] = vertexZ;

	kmGLMultMatrix( &transfrom4x4 );

	// XXX: Expensive calls. Camera should be integrated into the cached affine matrix
	if ( camera != NULL)/// && !(grid != NULL && grid->isActive()) )
	{
		bool translate = (m_obAnchorPointInPoints.x != 0.0f || m_obAnchorPointInPoints.y != 0.0f);

		if( translate )
			kmGLTranslatef(RENDER_IN_SUBPIXEL(m_obAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(m_obAnchorPointInPoints.y), 0 );

		m_pCamera->locate();

		if( translate )
			kmGLTranslatef(RENDER_IN_SUBPIXEL(-m_obAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(-m_obAnchorPointInPoints.y), 0 );
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
	m_bReorderChildDirty = true;
	ccArrayAppendObjectWithResize(m_pChildren->data, child);
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

Martrix4 Entity::entityToParentTransform(void)
{
	if (transformDirty)
	{

		if(useAnchorPointAsTransformCenter)
		{
			// Translate values
			float x = obPosition.x;
			float y = obPosition.y;

			float obAnchorPointInPointsX = contentSize.width * anchorPoint.x;
			float obAnchorPointInPointsY = contentSize.height * anchorPoint.y;

			if (!relativeAnchorPoint)
			{
				x += obAnchorPointInPointsX;
				y += obAnchorPointInPointsY;
			}

			// Rotation values
			// Change rotation code to handle X and Y
			// If we skew with the exact same value for both x and y then we're simply just rotating
			float cx = 1, sx = 0, cy = 1, sy = 0;
			if (rotationX || rotationY)
			{
				float radiansX = -DEGREES_TO_RADIANS(rotationX);
				float radiansY = -DEGREES_TO_RADIANS(rotationY);
				cx = cosf(radiansX);
				sx = sinf(radiansX);
				cy = cosf(radiansY);
				sy = sinf(radiansY);
			}

			bool needsSkewMatrix = ( skewX || skewY );

			// optimization:
			// inline anchor point calculation if skew is not needed
			// Adjusted transform calculation for rotational skew
			if (! needsSkewMatrix && !m_obAnchorPointInPoints.equals(PointZero))
			{
				x += cy * -obAnchorPointInPointX * scaleX + -sx * -obAnchorPointInPointY * scaleY;
				y += sy * -obAnchorPointInPointX * scaleX +  cx * -obAnchorPointInPointY * scaleY;
			}


			// Build Transform Matrix
			// Adjusted transform calculation for rotational skew
			transformMatrix = new Matrix4( cy * scaleX,  sy *scaleX,
				-sx * scaleY, cx * scaleY,
				x, y );

			// XXX: Try to inline skew
			// If skew is needed, apply skew and then anchor point
			if (needsSkewMatrix)
			{
				CCAffineTransform skewMatrix = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
					tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
					0.0f, 0.0f );
				m_sTransform = CCAffineTransformConcat(skewMatrix, m_sTransform);

				// adjust anchor point
				if (!m_obAnchorPointInPoints.equals(PointZero))
				{
					m_sTransform = CCAffineTransformTranslate(m_sTransform, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
				}
			}
		}
		else
		{
			Matrix4 matrix = new Matrix4();
			matrix.translate(position.x,position.y);

			float obAnchorPointInPointsX = contentSize.width * anchorPoint.x;
			float obAnchorPointInPointsY = contentSize.height * anchorPoint.y;

			if (relativeAnchorPoint)
			{
				matrix.translate(-obAnchorPointInPointsX,-obAnchorPointInPointsY);
			}

			if(isScaled())
			{
				if(!scaleCenter.equal(PointZero))
				{
					matrix.scale(scaleX,scaleY);
				}
				else
				{
					matrix.translate(-scaleCenter.x,-scaleCenter.y);
					matrix.ratate(scaleX,scaleY);
					matrix.translate(scaleCenter.x,scaleCenter.y);
				}	
			}

			if(isRotated())
			{
				if(!rotationCenter.equal(PointZero))
				{
					matrix.ratate(rotationX,rotationY);
				}
				else
				{
					matrix.translate(-rotationCenter.x,-rotationCenter.y);
					matrix.ratate(rotationX,rotationY);
					matrix.translate(rotationCenter.x,rotationCenter.y);
				}	
			}

			if(isSkewed())
			{
				if(!skewCenter.equal(PointZero))
				{
					matrix.ratate(skewX,skewY);
				}
				else
				{
					matrix.translate(-skewCenter.x,-skewCenter.y);
					matrix.ratate(skewX,skewY);
					matrix.translate(skewCenter.x,skewCenter.y);
				}	
			}

			transformMatrix = matrix;
		}

		
		if (additionalTransformDirty)
		{
			transformMatrix =  transformMatrix * additionalTransform; //??pre or post
			additionalTransformDirty = false;
		}

		transformDirty = false;
	}

	return transformMatrix;
}


Matrix4 Entity::parentToEntityTransform(void)
{
		if(transformDirty)
		{
			inverse = this->entityToParentTransform()->inverse();
		}
		return inverse;
}

Matrix4 Entity::entityToWorldTransform()
{
    Matrix4 t = this->entityToParentTransform();

    for (Entity *p = parent; p != NULL; p = p->getParent())
        t = t*(p->entityToParentTransform());

    return t;
}

Matrix Entity::worldToEntityTransform(void)
{
    return this->entityToWorldTransform()->inverse();
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
    return ccpSub(entityPoint, obAnchorPointInPoints);
}

Point Entity::convertToWorldSpaceAR(const Point& entityPoint)
{
    Point pt = ccpAdd(entityPoint, obAnchorPointInPoints);
    return convertToWorldSpace(pt);
}

Point Entity::convertToWindowSpace(const Point& nodePoint)
{
    Point worldPoint = this->convertToWorldSpace(nodePoint);
    return Engine::shareEngine()->convertToUI(worldPoint);
}

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


FLAKOR_NS_END


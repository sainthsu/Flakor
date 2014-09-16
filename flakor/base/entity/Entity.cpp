#include "Entity.h"

FLAKOR_NS_BEGIN

int Entity::s_globalOrderOfArrival = 1;

Entity::Entity(void)
: obPosition(PointZero)
, obContentSize(SizeZero)
, rotationX(0.0f)
, rotationY(0.0f)
, scaleX(1.0f)
, scaleY(1.0f)
, vertexZ(0.0f)
, skewX(0.0f)
, skewY(0.0f)
, obAnchorPointInPixels(PointZero)
, obAnchorPoint(PointZero)
, additionalTransform(Matrix4())
, camera(NULL)
// children (lazy allocs)
, ZOrder(0)
, children(NULL)
, parent(NULL)
, tag(EntityTagInvalid)
// userData is always inited as null
, userData(NULL)
, cullingEnabled(true)
//, orderOfArrival(0)
, running(false)
, transformDirty(true)
, inverseDirty(true)
, additionalTransformDirty(false)
, visible(true)
// "whole screen" objects. like Scenes and Layers, should set relativeAnchorPoint to true
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
        FK_SAFE_DELETE(pRet);
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
	obPosition = position;
	transformDirty = inverseDirty = true;
}

void Entity::setPosition(float x, float y)
{
	obPosition.x = x;
	obPosition.y = y;
	transformDirty = inverseDirty = true;
}

Point& Entity::getPosition()
{
	return obPosition;
}

void Entity::getPosition(float* x, float* y)
{
	*x = obPosition.x;
	*y = obPosition.y;
}

void  Entity::setPositionX(float x)
{
	obPosition.x = x;
	transformDirty = inverseDirty = true;
}

float Entity::getPositionX(void)
{
	return obPosition.x;
}

void  Entity::setPositionY(float y)
{
	obPosition.y = y;
	transformDirty = inverseDirty = true;
}

float Entity::getPositionY(void)
{
	return obPosition.y;
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
	return PointMake(obContentSize.width*anchorPoint.x , obContentSize.height*anchorPoint.y);
}

const Size& Entity::getContentSize()
{
	return obContentSize;
}

void Entity::setContentSize(const Size& contentSize)
{
	obContentSize = contentSize;
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
    //child->setOrderOfArrival(s_globalOrderOfArrival++);

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

void Entity::sortChildren()
{
	sortChildren(true);
}

void Entity::sortChildren(bool immediate)
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
          ScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, EntityOnEnter);
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
          ScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, EntityOnEnterTransitionDidFinish);
      }*/

      arrayMakeObjectsPerformSelector(children, onEnterTransitionDidFinish, Entity*);
}

void Entity::onExit()
{
  //this->pauseSchedulerAndActions();

      running = false;

      arrayMakeObjectsPerformSelector(m_pChildren, onExit, CCNode*);

      /*
      if ( scriptType != ScriptTypeNone)
      {
          ScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, EntityOnExit);
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

	if(children == NULL || children->count() <=0 || !this->chilrenVisible)
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
      kmMat4 transfrom4x4;

      // Convert 3x3 into 4x4 matrix
      CCAffineTransform tmpAffine = this->nodeToParentTransform();
      CGAffineToGL(&tmpAffine, transfrom4x4.mat);

      // Update Z vertex manually
      transfrom4x4.mat[14] = m_fVertexZ;

      kmGLMultMatrix( &transfrom4x4 );

      // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
      if ( m_pCamera != NULL && !(m_pGrid != NULL && m_pGrid->isActive()) )
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

        // Translate values
        float x = obPosition.x;
        float y = obPosition.y;

        float obAnchorPointInPointsX = obContentSize.width * obAnchorPoint.x;
        float obAnchorPointInPointsY = obContentSize.height * obAnchorPoint.y;


        if (ignoreAnchorPointForPosition)
        {
            x += m_obAnchorPointInPoints.x;
            y += m_obAnchorPointInPoints.y;
        }

	// Rotation values
	// Change rotation code to handle X and Y
	// If we skew with the exact same value for both x and y then we're simply just rotating
	float cx = 1, sx = 0, cy = 1, sy = 0;
	if (rotationX || rotationY)
	{
	    float radiansX = -CC_DEGREES_TO_RADIANS(rotationX);
	    float radiansY = -CC_DEGREES_TO_RADIANS(rotationY);
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
            x += cy * -m_obAnchorPointInPoints.x * scaleX + -sx * -m_obAnchorPointInPoints.y * scaleY;
            y += sy * -m_obAnchorPointInPoints.x * scaleX +  cx * -m_obAnchorPointInPoints.y * scaleY;
        }


        // Build Transform Matrix
        // Adjusted transform calculation for rotational skew
        m_sTransform = CCAffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
            -sx * m_fScaleY, cx * m_fScaleY,
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
            if (!m_obAnchorPointInPoints.equals(CCPointZero))
            {
                m_sTransform = CCAffineTransformTranslate(m_sTransform, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
            }
        }

        if (additionalTransformDirty)
        {
            m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }

        m_bTransformDirty = false;
    }

    return m_sTransform;
}


Point Entity::convertToWindowSpace(const Point& entityPoint)
{

}



FLAKOR_NS_END


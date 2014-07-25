#include "Entity.h"

FLAKOR_NS_BEGIN

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
	parent = parent;
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

void Entity::setVisible(bool visible)
{
	
}

bool Entity::isVisible()
{

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
	FKAssert( child != NULL, "Argument must be non-nil");
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

			while(j>=0 && (tmp->ZOrder < data[j]->ZOrder))
			{
				data[j+1] = data[j];
				j--;
			}
			data[j+1] = tmp;
		}
		
	}
	else
	{
		
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

}

void Entity::onEnter()
{
	
}

void Entity::onEnterTransitionDidFinish()
{

}

void Entity::onExit()
{

}

void Entity::onExitTransitionDidStart()
{
}

void Entity::cleanup(void)
{
	arrayMakeObjectsPerformSelector(children,cleanup,Entityy*);
}

void Entity::draw(void)
{

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
	if(transformDirty)
	{
		float x = obPosition.x;
		float y = obPosition.y;
	}
}

void Entity::transformAncestors(void)
{

}

void Entity::updateTransform(void)
{

}

void Entity::childrenAlloc(void)
{
	children = Array::createWithCapacity(4);
    children->retain();
}

void Entity::insertChild(Entity* child, int z)
{

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

Point Entity::convertToWindowSpace(const Point& entityPoint)
{

}



FLAKOR_NS_END


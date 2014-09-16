#ifdef _FK_ENTITY_H_
#define _FK_ENTITY_H_

#include "./base/geometry/Color.h"
#include "./lang/Object.h"
#include "./lang/Array.h"
#include "Camera.h"

FLAKOR_NS_BEGIN

class Color;
class Camera;

enum {
	EntityTagInvalid = -1,
};

enum {
	EntityOnEnter,
	EntityOnExit,
    EntityOnEnterTransitionDidFinish,
    EntityOnExitTransitionDidStart,
    EntityOnCleanup
};

abstract class Entity :: public Object,public IUpdatable,public IColorable 
{
	protected:
		static int s_globalOrderOfArrival;

		///在屏幕上的绝对坐标
		Point obPosition;
		///绝对尺寸，由宽高组成
		Size obContentSize;

		/**
		 *锚点
		 *AnchorPoint 的两个参量x 和y 的取值通常都是0 到1 之间的实数，表示锚点相对于节点长宽的位置。
		 *例如，把节点左下角作为锚点，值为(0,0) ；
		 *把节点的中心作为锚点，值为(0.5,0.5) ；
		 *把节点右下角作为锚点，值为(1,0) 。
		 **************************************/
		Point obAnchorPoint;
		/**
		 *锚点是否起作用
		 *如果锚点起作用，将会使m_obPosition失去作用。位置由anchorpoint
		 *和父类元素的位置决定
		 */
		bool relativeAnchorPoint;
		///opengl Z轴大小，备用
		float vertexZ;

		Color color;
		///旋转中心点，缩放中心点和倾斜中心点
		Point rotationCenter;
		Point scaleCenter;
		Point skewCenter;

		///x,y轴上的旋转，缩放，倾斜尺寸大小
		float rotationX;
		float rotationY;
		float scaleX;
		float scaleY;
		float skewX;
		float skewY;

		///在父元素排序使用的Z值
		int ZOrder;
		int orderOfArrival;            ///< used to preserve sequence while sorting children with the same localZOrder
		/**
		 *标签
		 */
		int tag;

		/**
		 *是否选中
		 */
		bool selected;
		/**
		 *是否可用，如果是False渲染将忽略这个元素
		 */
		bool enabled;
		/**
		 *是否正在运行中
		 */
		bool running;                    ///< is running

		/**
		 * @brief visible是否可见，如果不可见，那么将不绘制，但是事件属性还是存在的
		 */
		bool visible;
		/**
		 *是否剪裁
		 */
		bool cullingEnabled;
		/**
		 *是否忽略更新
		 */
		bool ignoreUpdate;
		/**
		 *子元素是否可见，如果为FALSE将不渲染子元素
		 */
		bool childrenVisible;
		/**
		 *子元素忽略更新
		 */
		bool childrenIgnoreUpdate;
		/**
		 *子元素是否等待排序
		 */
		bool childrenSortPending;

		bool transformDirty;
		bool inverseDirty
		/**
		 *相机，默认不起作用
		 */
		Camera* camera;
		/**
		 *子元素队列
		 */
		Array* children;
		/**
		 *父元素
		 */
		Entity* parent;                  ///< weak reference to parent entity
		/**
		 *逆矩阵
		 *inverse matrix：在线性代数中，给定一个 n 阶方阵A，若存在一 n 阶方阵B，使得 AB=BA=I_n，其中I_n为 n 阶单位矩阵，则称A 是可逆的，且 B 是A的逆矩阵，记作A^{-1}。
		 *只有正方形（n×n）的矩阵，亦即方阵，才可能、但非必然有逆矩阵。若方阵A的逆矩阵存在，则称A为非奇异方阵或可逆方阵。
		 *
		 */
		Matrix4 *inverseMatrix;
		Matrix4 *transformMatrix;
		Matrix4 *addictionMatrix;

		//用户自定义数据指针
		void* userData;
		Object* userObject;
		ScriptType scriptType;

	public:
		Entity(void);
		virtual ~Entity(void);

		virtual bool init();

		static Entity* create();
		static Entity* create(float x,float y);

		/**
		 * Gets the description string. It makes debugging easier.
		 * @return A string terminated with '\0'
		 */
		const char* toString(void);

		virtual void setPosition(const Point &position);
		virtual void setPosition(float x, float y);
		virtual Point& getPosition();
		/**
		 * Gets position in a more efficient way, returns two number instead of a Point object
		 *
		 * @see setPosition(float, float)
		 */
		virtual void getPosition(float* x, float* y);
		/**
		 * Gets/Sets x or y coordinate individually for position.
		 * These methods are used in kunkua,lua and Javascript Bindings
		 */
		virtual void  setPositionX(float x);
		virtual float getPositionX(void);
		virtual void  setPositionY(float y);
		virtual float getPositionY(void);

		void setRelativeAnchorPoint(bool relative);
		bool isRelativeAnchorPoint();
		/**
		 * Returns the anchorPoint in absolute pixels.
		 * 
		 * @warning You can only read it. If you wish to modify it, use anchorPoint instead.
		 * @see getAnchorPoint()
		 *
		 * @return The anchor point in absolute pixels.
		 */
		virtual const Point& getAnchorPointInPixels();

		virtual const Size& getContentSize() const;
		virtual void setContentSize(const Size& contentSize);
		virtual const Point& getAnchorPoint();
		virtual void setAnchorPoint(const Point &anchorPointer);

		/**
		 * Sets the Z order which stands for the drawing order, and reorder this entity in its parent's children array.
		 *
		 * The Z order of entity is relative to its "brothers": children of the same parent.
		 * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in flakor engine.
		 * The larger number it is, the later this entity will be drawn in each message loop.
		 * Please refer to setVertexZ(float) for the difference.
		 *
		 * @param nZOrder   Z order of this node.
		 */
		virtual void setZorder(int zOrder);

		/**
		 * Sets the z order which stands for the drawing order
		 *
		 * This is an internal method. Don't call it outside the framework.
		 * The difference between setZOrder(int) and _setOrder(int) is:
		 * - _setZOrder(int) is a pure setter for m_nZOrder memeber variable
		 * - setZOrder(int) firstly changes m_nZOrder, then recorder this node in its parent's chilren array.
		 */
		virtual void _setZOrder(int z);

		/**
		 * Gets the Z order of this entity.
		 *
		 * @see setZOrder(int)
		 *
		 * @return The Z order.
		 */
		virtual int getZOrder();

                /**
                 * Sets the arrival order when this entity has a same ZOrder with other children.
                 *
                 * A entity which called addChild subsequently will take a larger arrival order,
                 * If two children have the same Z order, the child with larger arrival order will be drawn later.
                 *
                 * @warning This method is used internally for ZOrder sorting, don't change this manually
                 *
                 * @param orderOfArrival   The arrival order.
                 */
                void setOrderOfArrival(int orderOfArrival);
                /**
                 * Returns the arrival order, indicates which children is added previously.
                 *
                 * @see `setOrderOfArrival(unsigned int)`
                 *
                 * @return The arrival order.
                 */
                int getOrderOfArrival() const;

		virtual bool hasParent();
		virtual void setParent(const Entity *parent);
		/**
		 * Returns a pointer to the parent entity
		 * 
		 * @see setParent(CCNode*)
		 *
		 * @returns A pointer to the parnet node
		 */
		virtual Entity* getParent();

		/**
		 * Sets the real OpenGL Z vertex.
		 *
		 * Differences between openGL Z vertex and flakor Z order:
		 * - OpenGL Z modifies the Z vertex, and not the Z order in the relation between parent-children
		 * - OpenGL Z might require to set 2D projection
		 * - flakor Z order works OK if all the nodes uses the same openGL Z vertex. eg: vertexZ = 0
		 *
		 * @warning Use it at your own risk since it might break the flakor parent-children z order
		 *
		 * @param z  OpenGL Z vertex of this entity.
		 */
		virtual void setVertexZ(float z);
		virtual float getVertexZ();

		virtual bool isRotated();
		virtual float getRotation();
		virtual void setRotation(float rotation);
		/** 
		 * Sets the X rotation (angle) of the node in degrees which performs a horizontal rotational skew.
		 * 
		 * 0 is the default rotation angle. 
		 * Positive values rotate node clockwise, and negative values for anti-clockwise.
		 * 
		 * @param x    The X rotation in degrees which performs a horizontal rotational skew.
		 */
		virtual void setRotationX(float x);
		/**
		 * Gets the X rotation (angle) of the node in degrees which performs a horizontal rotation skew.
		 *
		 * @see setRotationX(float)
		 *
		 * @return The X rotation in degrees.
		 */
		virtual float getRotationX();


		/** 
		 * Sets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
		 * 
		 * 0 is the default rotation angle. 
		 * Positive values rotate node clockwise, and negative values for anti-clockwise.
		 *
		 * @param fRotationY    The Y rotation in degrees.
		 */
		virtual void setRotationY(float y);
		/**
		 * Gets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
		 *
		 * @see setRotationY(float)
		 *
		 * @return The Y rotation in degrees.
		 */
		virtual float getRotationY();

		virtual bool isScaled();
		/**
		 * Changes the scale factor on X axis of this entity
		 *
		 * The deafult value is 1.0 if you haven't changed it before
		 *
		 * @param x   The scale factor on X axis.
		 */
		virtual void setScaleX(float x);
		/**
		 * Returns the scale factor on X axis of this entity
		 *
		 * @see setScaleX(float)
		 *
		 * @return The scale factor on X axis.
		 */
		virtual float getScaleX();


		/**
		 * Changes the scale factor on Y axis of this entity
		 *
		 * The Default value is 1.0 if you haven't changed it before.
		 *
		 * @param y   The scale factor on Y axis.
		 */
		virtual void setScaleY(float y);
		/**
		 * Returns the scale factor on Y axis of this entity
		 *
		 * @see setScaleY(float)
		 *
		 * @return The scale factor on Y axis. 
		 */
		virtual float getScaleY();


		/**
		 * Changes both X and Y scale factor of the entity.
		 *
		 * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
		 *
		 * @param scale     The scale factor for both X and Y axis.
		 */
		virtual void setScale(float scale);
		/**
		 * Gets the scale factor of the entity,  when X and Y have the same scale factor.
		 *
		 * @warning Assert when m_fScaleX != m_fScaleY.
		 * @see setScale(float)
		 *
		 * @return The scale factor of the entity.
		 */
		virtual float getScale();


		/**
		 * Changes both X and Y scale factor of the entity.
		 *
		 * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
		 *
		 * @param x     The scale factor on X axis.
		 * @param y     The scale factor on Y axis.
		 */
		virtual void setScale(float x,float y);

		virtual bool isSkewed();
		/**
		 * Changes the X skew angle of the entity in degrees.
		 *
		 * This angle describes the shear distortion in the X direction.
		 * Thus, it is the angle between the Y axis and the left edge of the shape
		 * The default skewX angle is 0. Positive values distort the node in a CW direction.
		 *
		 * @param x The X skew angle of the node in degrees.
		 */
		virtual void setSkewX(float x);
		/**
		 * Returns the X skew angle of the entity in degrees.
		 *
		 * @see setSkewX(float)
		 *
		 * @return The X skew angle of the entity in degrees.
		 */
		virtual float getSkewX();


		/**
		 * Changes the Y skew angle of the entity in degrees.
		 *
		 * This angle describes the shear distortion in the Y direction.
		 * Thus, it is the angle between the X axis and the bottom edge of the shape
		 * The default skewY angle is 0. Positive values distort the node in a CCW direction.
		 *
		 * @param y    The Y skew angle of the node in degrees.
		 */
		virtual void setSkewY(float y);
		/**
		 * Returns the Y skew angle of the entity in degrees.
		 *
		 * @see setSkewY(float)
		 *
		 * @return The Y skew angle of the entity in degrees.
		 */
		virtual float getSkewY();


		/**
		 * Sets whether the entity is visible
		 *
		 * The default value is true, a entity is default to visible
		 *
		 * @param visible   true if the entity is visible, false if the entity is hidden.
		 */
		virtual void setVisible(bool visible);
		/**
		 * Determines if the entity is visible
		 *
		 * @see setVisible(bool)
		 *
		 * @return true if the entity is visible, false if the entity is hidden.
		 */
		virtual bool isVisible();

		virtual void setChildrenVisible(bool visible);
		virtual bool isChildrenVisible();
		virtual void setChildrenIgnoreUpdate(bool visible);
		virtual bool isChildrenIgnoreUpdate();

		/** 
		 * Adds a child to the container with z-order as 0.
		 *
		 * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
		 *
		 * @param child A child node
		 */
		virtual void addChild(Entity * child);
		/** 
		 * Adds a child to the container with a z-order
		 *
		 * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
		 *
		 * @param child     A child entity
		 * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
		 */
		virtual void addChild(Entity * child, int zOrder);
		/** 
		 * Adds a child to the container with z order and tag
		 *
		 * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
		 *
		 * @param child     A child node
		 * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
		 * @param tag       A interger to identify the node easily. Please refer to setTag(int)
		 */
		virtual void addChild(Entity* child, int zOrder, int tag);
		/**
		 * Gets a child from the container with its tag
		 *
		 * @param tag   An identifier to find the child node.
		 *
		 * @return a CCNode object whose tag equals to the input parameter
		 */
		Entity* getChildByTag(int tag);
		Entity* getChildByZIndex(int zIndex);
		Entity* getFirstChild();
		Entity* getLastChild();

		/**
		 * Return an array of children
		 *
		 * Composing a "tree" structure is a very important feature of CCNode
		 * Here's a sample code of traversing children array:
		 * @code
		 * CCNode* node = NULL;
		 * CCARRAY_FOREACH(parent->getChildren(), node)
		 * {
		 *     node->setPosition(0,0);
		 * }
		 * @endcode
		 * This sample code traverses all children nodes, and set theie position to (0,0)
		 *
		 * @return An array of children
		 */
		virtual Array* getChildren();

		/** 
		 * Get the amount of children.
		 *
		 * @return The amount of children.
		 */
		unsigned int getChildrenCount(void) const;

		/** 
     	* Removes a child from the container with a cleanup
     	*
     	* @see removeChild(CCNode, bool)
     	*
     	* @param child     The child entity which will be removed.
     	*/
    	virtual void removeChild(Entity* child);
    	/** 
     	* Removes a child from the container. It will also cleanup all running actions depending on the cleanup parameter.
     	* 
     	* @param child     The child entity which will be removed.
     	* @param cleanup   true if all running actions and callbacks on the child node will be cleanup, false otherwise.
     	*/
    	virtual void removeChild(Entity* child, bool cleanup);
		virtual void removeChildByTag(int tag);
		virtual void removeChildByTag(int tag,bool cleanup);
		virtual void removeChildByZOrder(int z);
		virtual void removeAllChildren();
		virtual void removeAllChildren(bool cleanup);

		/** 
     	* Reorders a child according to a new z value.
     	*
     	* @param child     An already added child node. It MUST be already added.
     	* @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
     	*/
    	virtual void reorderChild(Entity * child, int zOrder);
		virtual void sortChildren();
		virtual void sortChildren(bool immediate);

		/**
		 * Returns a tag that is used to identify the node easily.
		 *
		 * You can set tags to node then identify them easily.
		 * @code
		 * #define TAG_PLAYER  1
		 * #define TAG_MONSTER 2
		 * #define TAG_BOSS    3
		 * // set tags
		 * node1->setTag(TAG_PLAYER);
		 * node2->setTag(TAG_MONSTER);
		 * node3->setTag(TAG_BOSS);
		 * parent->addChild(node1);
		 * parent->addChild(node2);
		 * parent->addChild(node3);
		 * // identify by tags
		 * CCNode* node = NULL;
		 * CCARRAY_FOREACH(parent->getChildren(), node)
		 * {
		 *     switch(node->getTag())
		 *     {
		 *         case TAG_PLAYER:
		 *             break;
		 *         case TAG_MONSTER:
		 *             break;
		 *         case TAG_BOSS:
		 *             break;
		 *     }
		 * }
		 * @endcode
		 *
		 * @return A interger that identifies the node.
		 */
		virtual int getTag() const;

		/**
		 * Changes the tag that is used to identify the node easily.
		 *
		 * Please refer to getTag for the sample code.
		 *
		 * @param A interger that indentifies the node.
		 */
		virtual void setTag(int tag);

		/**
		 * Returns a custom user data pointer
		 *
		 * You can set everything in UserData pointer, a data block, a structure or an object.
		 * 
		 * @return A custom user data pointer
		 * @js NA
		 */
		virtual void* getUserData();
		/**
		 * Sets a custom user data pointer
		 *
		 * You can set everything in UserData pointer, a data block, a structure or an object, etc.
		 * @warning Don't forget to release the memroy manually, 
		 *          especially before you change this data pointer, and before this node is autoreleased.
		 *
		 * @return A custom user data pointer
		 * @js NA
		 */
		virtual void setUserData(void *data);

		/** 
		 * Returns a user assigned Object
		 * 
		 * Similar to userData, but instead of holding a void* it holds an object
		 *
		 * @return A user assigned CCObject
		 * @js NA
		 */
		virtual Object* getUserObject();
		/**
		 * Returns a user assigned CCObject
		 *
		 * Similar to UserData, but instead of holding a void* it holds an object.
		 * The UserObject will be retained once in this method,
		 * and the previous UserObject (if existed) will be relese.
		 * The UserObject will be released in CCNode's destructure.
		 *
		 * @param A user assigned CCObject
		 */
		virtual void setUserObject(Object *pUserObject);

		/**
		 * Returns a camera object that lets you move the node using a gluLookAt
		 *
		 * @code
		 * Camera* camera = Entity->getCamera();
		 * camera->setEyeXYZ(0, 0, 415/2);
		 * camera->setCenterXYZ(0, 0, 0);
		 * @endcode
		 *
		 * @return A Camera object that lets you move the node like using a gluLookAt in OpenGL 1.0 1.1
		 */
		virtual Camera* getCamera();

		/** 
		 * Returns whether or not the node accepts event callbacks.
		 * 
		 * Running means the node accept event callbacks like onEnter(), onExit(), update()
		 *
		 * @return Whether or not the node is running.
		 */
		virtual bool isRunning();

		/** 
     	* Event callback that is invoked every time when CCNode enters the 'stage'.
     	* If the CCNode enters the 'stage' with a transition, this event is called when the transition starts.
     	* During onEnter you can't access a "sister/brother" node.
     	* If you override onEnter, you shall call its parent's one, e.g., CCNode::onEnter().
     	* @js NA
     	* @lua NA
     	*/
   		virtual void onEnter();

    /** Event callback that is invoked when the CCNode enters in the 'stage'.
     * If the CCNode enters the 'stage' with a transition, this event is called when the transition finishes.
     * If you override onEnterTransitionDidFinish, you shall call its parent's one, e.g. CCNode::onEnterTransitionDidFinish()
     * @js NA
     * @lua NA
     */
    virtual void onEnterTransitionDidFinish();

    /** 
     * Event callback that is invoked every time the CCNode leaves the 'stage'.
     * If the CCNode leaves the 'stage' with a transition, this event is called when the transition finishes.
     * During onExit you can't access a sibling node.
     * If you override onExit, you shall call its parent's one, e.g., CCNode::onExit().
     * @js NA
     * @lua NA
     */
    virtual void onExit();

    /** 
     * Event callback that is called every time the CCNode leaves the 'stage'.
     * If the CCNode leaves the 'stage' with a transition, this callback is called when the transition starts.
     * @js NA
     * @lua NA
     */
    virtual void onExitTransitionDidStart();

    /// @} end of event callbacks.


    /** 
     * Stops all running actions and schedulers
     */
    virtual void cleanup(void);

/** 
     * Override this method to draw your own node.
     * The following GL states will be enabled by default:
     * - glEnableClientState(GL_VERTEX_ARRAY);
     * - glEnableClientState(GL_COLOR_ARRAY);
     * - glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     * - glEnable(GL_TEXTURE_2D);
     * AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR Entity
     * But if you enable any other GL state, you should disable it after drawing your entity.
     */
    virtual void draw(void);

    /** 
     * Visits this entity's children and draw them recursively.
     */
    virtual void visit(void);

		virtual void onAttached();
		virtual void onDetached();
		/* 
		 * Update method will be called automatically every frame if "scheduleUpdate" is called, and the entity is "live"
		 */
		virtual void update(float delta);

		/// @} end of Scheduler and Timer

		/// @{
		/// @name Transformations

		/**
		 * Performs OpenGL view-matrix transformation based on position, scale, rotation and other attributes.
		 */
		void transform(void);
		/**
		 * Performs OpenGL view-matrix transformation of it's ancestors.
		 * Generally the ancestors are already transformed, but in certain cases (eg: attaching a FBO)
		 * It's necessary to transform the ancestors again.
		 */
		void transformAncestors(void);
		/**
		 * Calls children's updateTransform() method recursively.
		 */
		virtual void updateTransform(void);

	private:
		/// lazy allocs
		void childrenAlloc(void);

		/// helper that reorder a child
		void insertChild(Entity* child, int z);

		/// Removes a child, call child->onExit(), do cleanup, remove it from children array.
		void detachChild(Entity *child, bool doCleanup);

		/** Convert flakor coordinates to UI windows coordinate.
		 */
		Point convertToWindowSpace(const Point& entityPoint);
}

FLAKOR_NS_END

#endif

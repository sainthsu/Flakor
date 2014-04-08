#ifdef _FK_ENTITY_H_
#define _FK_ENTITY_H_

FLAKOR_NS_BEGIN

/**
 *m member
 *b -bool
 *p -pointer
 *n -int
 *f -float
 */
abstract class Entity
{
	protected:
		/*＊
		 *在屏幕上的绝对坐标
		 */
		Point m_obPosition;
		/*＊
		 *绝对尺寸，由宽高组成
		 */
		Size m_obContentSize;

		/**
		*锚点
		*AnchorPoint 的两个参量x 和y 的取值通常都是0 到1 之间的实数，表示锚点相对于节点长宽的位置。
		*例如，把节点左下角作为锚点，值为(0,0) ；
		*把节点的中心作为锚点，值为(0.5,0.5) ；
		*把节点右下角作为锚点，值为(1,0) 。
		**************************************/
		Point m_obAnchorPoint;
		/**
		 *锚点是否起作用
		 *如果锚点起作用，将会使m_obPosition失去作用。位置由anchorpoint
		 *和父类元素的位置决定
		 */
		bool m_bRelativeAnchorPoint;
		/**
		 *opengl Z轴大小，备用
		 */
		float m_fVertexZ;

		/**
		 *旋转中心点，缩放中心点和倾斜中心点
		 */
		Point m_rotationCenter;
		Point m_scaleCenter;
		Point m_skewCenter;
		
		/**
		 *x,y轴上的旋转，缩放，倾斜尺寸大小
		 */
		float m_fRatationX;
		float m_fRatationY;
		float m_fScaleX;
		float m_fScaleY;
		float m_fSkewX;
		float m_fSkewY;

		/**
		 *在父元素排序使用的Z值
		 */
		int m_nZOrder;
		/**
		 *标签
		 */
		int m_nTag;

		/**
		 *是否选中
		 */
		bool m_bSelected;
		/**
		 *是否可用，如果是False渲染将忽略这个元素
		 */
		bool m_bEnabled;
		/**
		 *是否正在运行中
		 */
		bool m_bRunning;                    ///< is running
		/**
		 *是否剪裁
		 */
		bool m_bCullingEnabled;
		/**
		 *是否忽略更新
		 */
		bool m_bIgnoreUpdate;
		/**
		 *子元素是否可见，如果为FALSE将不渲染子元素
		 */
		bool m_bChildrenVisible;
		/**
		 *子元素忽略更新
		 */
		bool m_bChildrenIgnoreUpdate;
		/**
		 *子元素是否等待排序
		 */
		bool m_bChildrenSortPending;

		/**
		 *相机，默认不起作用
		 */
		Camera* m_pCamera;
		/**
		 *子元素队列
		 */
		SmartList<Entity>	m_pChildren;
		/**
		 *父元素
		 */
		Entity* m_pParent;                  ///< weak reference to parent node
		/**
		 *逆矩阵
		 *inverse matrix：在线性代数中，给定一个 n 阶方阵A，若存在一 n 阶方阵B，使得 AB=BA=I_n，其中I_n为 n 阶单位矩阵，则称A 是可逆的，且 B 是A的逆矩阵，记作A^{-1}。
		 *只有正方形（n×n）的矩阵，亦即方阵，才可能、但非必然有逆矩阵。若方阵A的逆矩阵存在，则称A为非奇异方阵或可逆方阵。
		 *
		 */
		TransformMatrix *m_pInverseMatrix;
		TransformMatrix *m_pTransformMatrix;
		TransformMatrix *m_pAddictionMatrix;

		//用户自定义数据指针
		void* m_pUserData;

	public:
		Entity(void);
		virtual ~Entity(void);

		virtual bool init();
		
		static Entity* create();
		
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
		virtual const Point& getAnchorPoiter();
		virtual void setAnchorPointer(const Point &anchorPointer);

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
     	* Gets the Z order of this node.
     	*
     	* @see setZOrder(int)
     	*
     	* @return The Z order.
     	*/
    	virtual int getZOrder();

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
     	* @param fVertexZ  OpenGL Z vertex of this entity.
     	*/
		virtual void setVertexZ(float vertexZ);
		virtual float getVertexZ();

		/**
     	* Changes the scale factor on X axis of this entity
     	*
     	* The deafult value is 1.0 if you haven't changed it before
     	*
     	* @param fScaleX   The scale factor on X axis.
     	*/
    	virtual void setScaleX(float fScaleX);
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
     	* @param fScaleY   The scale factor on Y axis.
     	*/
    	virtual void setScaleY(float fScaleY);
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
     	* @param fScaleX     The scale factor on X axis.
     	* @param fScaleY     The scale factor on Y axis.
     	*/
    	virtual void setScale(float fScaleX,float fScaleY);

		/**
     	* Changes the X skew angle of the entity in degrees.
     	*
     	* This angle describes the shear distortion in the X direction.
     	* Thus, it is the angle between the Y axis and the left edge of the shape
     	* The default skewX angle is 0. Positive values distort the node in a CW direction.
     	*
     	* @param fSkewX The X skew angle of the node in degrees.
     	*/
    	virtual void setSkewX(float fSkewX);
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
     	* @param fSkewY    The Y skew angle of the node in degrees.
     	*/
    	virtual void setSkewY(float fSkewY);
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
    	Entity * getChildByTag(int tag);
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
    	virtual SmartList* getChildren();
    
    	/** 
     	* Get the amount of children.
     	*
     	* @return The amount of children.
     	*/
    	unsigned int getChildrenCount(void) const;

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
    virtual void setTag(int nTag);
    
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
    virtual void setUserData(void *pUserData);
    
    /** 
     * Returns a user assigned CCObject
     * 
     * Similar to userData, but instead of holding a void* it holds an object
     *
     * @return A user assigned CCObject
     * @js NA
     */
    virtual CCObject* getUserObject();
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
    	virtual void setUserObject(CCObject *pUserObject);

		/**
     	* Returns a camera object that lets you move the node using a gluLookAt
     	*
     	* @code
     	* CCCamera* camera = node->getCamera();
     	* camera->setEyeXYZ(0, 0, 415/2);
     	* camera->setCenterXYZ(0, 0, 0);
     	* @endcode
     	*
     	* @return A CCCamera object that lets you move the node using a gluLookAt
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

		/* 
     	* Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
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
     	*
     	* This method is moved from CCSprite, so it's no longer specific to CCSprite.
     	* As the result, you apply CCSpriteBatchNode's optimization on your customed CCNode.
     	* e.g., batchNode->addChild(myCustomNode), while you can only addChild(sprite) before.
     	*/
    	virtual void updateTransform(void);

	private:
    	/// lazy allocs
    	void childrenAlloc(void);
    
    	/// helper that reorder a child
    	void insertChild(Entity* child, int z);
    
    	/// Removes a child, call child->onExit(), do cleanup, remove it from children array.
    	void detachChild(Entity *child, bool doCleanup);
    
    	/** Convert cocos2d coordinates to UI windows coordinate.
     	* @js NA
     	* @lua NA
     	*/
    	Point convertToWindowSpace(const Point& entityPoint);
}

FLAKOR_NS_END

#endif

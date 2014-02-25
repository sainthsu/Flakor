#ifdef _FK_ENTITY_H_
#define _FK_ENTITY_H_

FLAKOR_NS_BEGIN

/**
 *m member b -bool p -pointer n -int f -float
 */
abstract class Entity
{
	protected:
		Point m_obPosition;
		Size m_obContentSize;

		Point m_obAnchorPoint;

		bool m_bRelativeAnchorPoint;

		float m_fVertexZ;

		Point m_rotationCenter;
		Point m_scaleCenter;
		Point m_skewCenter;
		
		int m_nZOrder;
		int m_nTag;

		bool m_bEnabled;
		bool m_bRunning;                    ///< is running
		bool m_bCullingEnabled;
		bool m_bIgnoreUpdate;
		bool m_bChildrenVisible;
		bool m_bChildrenIgnoreUpdate;
		bool m_bChildrenSortPending;

		Camera* m_pCamera;
		Entity* m_pParent;                  ///< weak reference to parent node
		void* m_pUserData;

	public:
		Entity(void);
		virtual ~Entity(void);

		virtual bool init();
		
		virtual Entity* create();
		
		/**
     	* Gets the description string. It makes debugging easier.
     	* @return A string terminated with '\0'
     	*/
    	const char* toString(void);

		/**
     	* Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     	*
     	* The Z order of entity is relative to its "brothers": children of the same parent.
     	* It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     	* The larger number it is, the later this node will be drawn in each message loop.
     	* Please refer to setVertexZ(float) for the difference.
     	*
     	* @param nZOrder   Z order of this node.
     	*/
		virtual void setZorder(int zOrder);
}

FLAKOR_NS_END

#endif

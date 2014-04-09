

#include "Entity.h"

FLAKOR_NS_BEGIN

Entity::Entity(void)
: m_obPosition(PointZero)
, m_obContentSize(SizeZero)
, m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fVertexZ(0.0f)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_obAnchorPointInPixels(PointZero)
, m_obAnchorPoint(PointZero)
, m_sAdditionalTransform(CCAffineTransformMakeIdentity())
, m_pCamera(NULL)
// children (lazy allocs)
// lazy alloc
, m_nZOrder(0)
, m_pChildren(NULL)
, m_pParent(NULL)
// "whole screen" objects. like Scenes and Layers, should set m_bIgnoreAnchorPointForPosition to true
, m_nTag(kEntityTagInvalid)
// userData is always inited as null
, m_pUserData(NULL)
, m_pUserObject(NULL)
, m_pShaderProgram(NULL)
, m_eGLServerState(ccGLServerState(0))
, m_uOrderOfArrival(0)
, m_bRunning(false)
, m_bTransformDirty(true)
, m_bInverseDirty(true)
, m_bAdditionalTransformDirty(false)
, m_bVisible(true)
, m_bIgnoreAnchorPointForPosition(false)
, m_bReorderChildDirty(false)
, m_nScriptHandler(0)
, m_nUpdateScriptHandler(0)
, m_pComponentContainer(NULL)
{
	

}

Entity::~Entity(void)
{

}

FLAKOR_NS_END


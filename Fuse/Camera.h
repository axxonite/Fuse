// Camera.h

#pragma once

#include "Node.h"

namespace Fuse
{

class CScene;

//
// Camera.
//

class CCamera : public CNode
{

public :

	enum eClipPlanes
	{
		eNEAR,
		eFAR,
		eLEFT,
		eRIGHT,
		eTOP,
		eBOTTOM,
		eMAX_PLANES
	};

	struct CFrustum
	{
		float	fHalfWidth;		// Left frustum plane
		float	fHalfHeight;	// Top frustum plane
		float	fNear;			// Near frustum plane
		float	fFar;			// Far frustum plane
		float	fCorrectedHeight;
	};

	//---------------------------------------------------------------------------------------------

	CCamera();
	virtual ~CCamera();

	//---------------------------------------------------------------------------------------------

	// Camera must be initialized before being used.
			void	Init();

	// Renders scene associated with camera.
	virtual void	Render();

	// Updates camera internal structures after it has moved.
	virtual void	Update( u32 uUpdateFlags = 0 );

	//---------------------------------------------------------------------------------------------

	// Returns the camera frustum parameters.
	inline	const CFrustum&	GetFrustum() const;

	// Returns the specified camera clip plane.
	inline	const CPlane&	GetClipPlane( eClipPlanes Plane ) const;

	// Determines horizontal frustum view angle. NOTE: This is the full arc view angle.
	inline	float	GetViewAngle() const;
	void			SetViewAngle( float fAngle );
	
	// Determines scene associated with camera,
	inline CScene*	GetScene();
	inline void		SetScene( CScene* pScene);

	inline float	GetNearPlane() const;
	inline void		SetNearPlane( float fNear );
	inline float	GetFarPlane() const;
	inline void		SetFarPlane( float fFar );

	// Returns view and projection matrices.
	inline const	CMatrix4&	GetViewMatrix() const;
	inline const	CMatrix4&	GetProjMatrix()	const;

private :

	CFrustum		m_Frustum;

	CScene*			m_pScene;

	CMatrix4		m_mView;
	CMatrix4		m_mProjection;

	CVector			m_vViewPlaneNormal;
	CVector			m_vViewPlaneUp;
	CVector			m_vViewPlaneRight;

	float			m_fViewAngle;

	CVector			m_vWorldViewPlanePos;
	CVector			m_vWorldViewPlaneNormal;
	CVector			m_vWorldViewPlaneUp;
	CVector			m_vWorldViewPlaneRight;

	CPlane			m_ClipPlanes[eMAX_PLANES];

	bool			m_bInitialized;

	void			ComputeProjMatrix();
	void			UpdateClipPlanes();
	void			UpdateViewMatrix();

};

inline const CPlane& CCamera::GetClipPlane( eClipPlanes Plane ) const
{
	return m_ClipPlanes[Plane];
}

inline const CCamera::CFrustum&	CCamera::GetFrustum() const
{
	return m_Frustum;
}

inline float CCamera::GetNearPlane() const
{
	return m_Frustum.fNear;
}

inline void	 CCamera::SetNearPlane( float fNear )
{
	m_Frustum.fNear = fNear;
	SetViewAngle( m_fViewAngle );
	ComputeProjMatrix();
}

inline float CCamera::GetFarPlane() const
{
	return m_Frustum.fFar;
}

inline void	CCamera::SetFarPlane( float fFar )
{
	m_Frustum.fFar = fFar;
	ComputeProjMatrix();
}

inline float CCamera::GetViewAngle() const
{
	return m_fViewAngle;
}

inline const CMatrix4& CCamera::GetViewMatrix() const
{
	return m_mView;
}

inline	const CMatrix4& CCamera::GetProjMatrix() const
{
	return m_mProjection;
}

inline CScene* CCamera::GetScene()
{
	return m_pScene;
}

inline void CCamera::SetScene( CScene* pScene)
{
	m_pScene = pScene;
}

}
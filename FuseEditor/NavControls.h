// NavControls.h

#pragma once

#include "Node.h"

namespace Fuse
{

class CCamera;

//
// Editor navigation controls.
//

class CNavControls : public CBaseObject
{

public :

	enum eNavModes
	{
		eNONE,
		eORBIT,
		ePAN,
		eZOOM
	};

	enum eConstraintModes
	{
		eCONSTRAINT_NONE,
		eCONSTRAINT_UNKNOWN,
		eCONSTRAINT_X,
		eCONSTRAINT_Y
	};

	//---------------------------------------------------------------------------------------------

	// Default constructor.
	CNavControls();
	// Destructor.
	virtual ~CNavControls();

	//---------------------------------------------------------------------------------------------

	// Initialized navigation controls.
	void	Init( CCamera* pCamera, CNode* pRootNode );

	// Ticks navigation controls.
	void	Tick();

	//---------------------------------------------------------------------------------------------

	// Returns currently active navigation mode.
	inline	eNavModes		GetMode() const;

	// Returns current status string.
	inline	const char*		GetStatus();

private :

	CNode				m_PivotNode;
	CCamera*			m_pCamera;

	bool				m_bHasMoved;
	CMatrix3			m_mYaw;
	CMatrix3			m_mPitch;
	float				m_fPitch;
	eNavModes			m_Mode;
	eConstraintModes	m_ConstraintMode;
	CVector				m_ConstraintRefPoint;
	bool				m_bInvertYaw;
	POINT				m_LastMousePos;

	CVector		m_vTotalDisplacement;
	string		m_Status;

	float		m_fRotateSpeedX;
	float		m_fRotateSpeedY;
	float		m_fZoomSpeed;
	float		m_fWheelZoomSpeed;

	void		AdjustPivot();

	void		ApplyZoom( float fDelta, bool bUpdateStatus = true );

};

inline CNavControls::eNavModes CNavControls::GetMode() const
{
	return m_Mode;
}

inline const char* CNavControls::GetStatus()
{
	return m_Status.c_str();
}

}
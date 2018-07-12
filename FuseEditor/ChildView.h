// ChildView.h : interface of the CChildView class
//

#pragma once

//
// MFC child view.
//

class CChildView : public CWnd
{
public:

	//---------------------------------------------------------------------------------------------

	CChildView();
	virtual ~CChildView();

private :

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

};
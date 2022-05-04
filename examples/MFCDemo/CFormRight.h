#pragma once

#include "CGLControl.h"

// CFormRight 폼 보기

class CFormRight : public CFormView
{
	DECLARE_DYNCREATE(CFormRight)

protected:
	CFormRight();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormRight();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM_RIGHT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CGLControl* m_glControl;

	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CreateGLControl(CGLControl* glControl);
};



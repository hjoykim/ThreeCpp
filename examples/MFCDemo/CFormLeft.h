#pragma once

#include <ecore/ThreeDemoModel.h>


class CFormLeft : public CFormView
{
	DECLARE_DYNCREATE(CFormLeft)

protected:
	CFormLeft();           
	virtual ~CFormLeft();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM_LEFT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:
	CListBox m_listBox;
	CWnd* m_target;
	CArray<ThreeDemoClass*, ThreeDemoClass*> exampleList;
	bool m_initFlag;
	void InitExample();
	void SetTarget(CWnd* m_cwnd);

	afx_msg void OnLbnSelchangeExample();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};



//

#include "pch.h"
#include "MFCDemo.h"
#include "CFormRight.h"


// CFormRight

IMPLEMENT_DYNCREATE(CFormRight, CFormView)

CFormRight::CFormRight()
	: CFormView(IDD_FORM_RIGHT)
{
	m_glControl = new CGLControl();
}

CFormRight::~CFormRight()
{
}

void CFormRight::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormRight, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



#ifdef _DEBUG
void CFormRight::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormRight::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG




void CFormRight::OnDraw(CDC* /*pDC*/)
{
	if (m_glControl)
		m_glControl->Render();
}


int CFormRight::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateGLControl(m_glControl);
	return 0;
}


void CFormRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}


void CFormRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW) return;
	CRect rect = CRect(0, 0, cx, cy);
	if (m_glControl) {
		m_glControl->MoveWindow(rect);
		m_glControl->OnSize(nType, cx, cy);
	}
}


void CFormRight::OnDestroy()
{
	CFormView::OnDestroy();

}


void CFormRight::OnTimer(UINT_PTR nIDEvent)
{

	CFormView::OnTimer(nIDEvent);
}

void CFormRight::CreateGLControl(CGLControl* glControl)
{
	if (glControl == m_glControl)
		glControl->Create(_T("CGLTestView"), _T("CGLTestView"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 10, 10), this, 999999);
	else
		glControl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, 999999);
}

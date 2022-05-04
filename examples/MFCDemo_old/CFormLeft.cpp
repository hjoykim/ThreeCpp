// CFormLeft.cpp: 구현 파일
//

#include "pch.h"
#include "CThreeExample.h"
#include "CFormLeft.h"


// CFormLeft

IMPLEMENT_DYNCREATE(CFormLeft, CFormView)

CFormLeft::CFormLeft()
	: CFormView(IDD_FORM_LEFT)
{

}

CFormLeft::~CFormLeft()
{
}

void CFormLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXAMPLE, m_listBox);
}

BEGIN_MESSAGE_MAP(CFormLeft, CFormView)
END_MESSAGE_MAP()


// CFormLeft 진단

#ifdef _DEBUG
void CFormLeft::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormLeft::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormLeft 메시지 처리기

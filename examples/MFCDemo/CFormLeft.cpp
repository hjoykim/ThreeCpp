// CFormLeft.cpp: 구현 파일
//

#include "pch.h"
#include "MFCDemo.h"
#include "CFormLeft.h"
#include "CFormRight.h"
#include <learningjs/ch1/FirstScene.h>
#include <learningjs/ch1/MaterialLight.h>
#include <learningjs/ch1/MaterialLightAnimation.h>
#include <learningjs/ch2/BasicScene.h>
#include <learningjs/ch2/FoggyScene.h>
#include <learningjs/ch2/ForcedMaterials.h>
#include <learningjs/ch2/CamerasLookAt.h>
#include <learningjs/ch2/BothCamera.h>
#include <learningjs/ch2/Geometries.h>
#include <learningjs/ch2/CustomGeometry.h>
#include <learningjs/ch2/MeshProperties.h>
#include <learningjs/ch3/AmbientLightExample.h>
#include <learningjs/ch3/AreaLightExample.h>
#include <learningjs/ch3/DirectionalLightExample.h>
#include <learningjs/ch3/HemisphereLightExample.h>
#include <learningjs/ch3/LensflaresExample.h>
#include <learningjs/ch3/PointLightExample.h>
#include <learningjs/ch3/SpotLightExample.h>
#include <learningjs/ch4/BasicMeshMaterialExample.h>
#include <learningjs/ch5/Basic2DGeometriesPlane.h>


// CFormLeft

IMPLEMENT_DYNCREATE(CFormLeft, CFormView)

CFormLeft::CFormLeft()	: CFormView(IDD_FORM_LEFT)
{
	m_initFlag = false;
}

CFormLeft::~CFormLeft()
{
	if (exampleList.GetCount() > 0) {
		for (int i = 0; i < exampleList.GetCount(); i++)
			delete exampleList.GetAt(i);
	}
}

void CFormLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXAMPLE, m_listBox);
}

BEGIN_MESSAGE_MAP(CFormLeft, CFormView)
	ON_LBN_SELCHANGE(IDC_EXAMPLE, &CFormLeft::OnLbnSelchangeExample)
	ON_WM_SIZE()
END_MESSAGE_MAP()



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




void CFormLeft::InitExample()
{
	m_listBox.AddString(_T("Chapter1. 02-First-Scene"));
	m_listBox.AddString(_T("Chapter1. 03-Material-Light"));
	m_listBox.AddString(_T("Chapter1. 04-Material-Light-Animation"));
	m_listBox.AddString(_T("Chapter2. 01-Basic-Scene"));
	m_listBox.AddString(_T("Chapter2. 02-Foggy-Scene"));
	m_listBox.AddString(_T("Chapter2. 03-Forced-Materials"));
	m_listBox.AddString(_T("Chapter2. 04-Geometries"));
	m_listBox.AddString(_T("Chapter2. 05-Custom-Geometry"));
	m_listBox.AddString(_T("Chapter2. 06-Mesh-Properties"));
	m_listBox.AddString(_T("Chapter2. 07-Both-Camera"));
	m_listBox.AddString(_T("Chapter2. 08-Cameras-Lookat"));
	m_listBox.AddString(_T("Chapter3. 01-Ambient-Light"));
	m_listBox.AddString(_T("Chapter3. 02-Spot-Light"));
	m_listBox.AddString(_T("Chapter3. 03-Point-Light"));
	m_listBox.AddString(_T("Chapter3. 04-Directional-Light"));
	m_listBox.AddString(_T("Chapter3. 05-Hemisphere-Light"));
	m_listBox.AddString(_T("Chapter3. 06-Area-Light"));
	m_listBox.AddString(_T("Chapter3. 07-Lensflares"));
	m_listBox.AddString(_T("Chapter4. 01-Basic-Material"));
	m_listBox.AddString(_T("Chapter5. 01-Basic2D-Geometries-Plane"));


	if (m_target) {
		RECT rect;
		((CFormRight*)m_target)->GetClientRect(&rect);
		exampleList.Add(new FirstScene(rect.right, rect.bottom));
		exampleList.Add(new MaterialLight(rect.right, rect.bottom));
		exampleList.Add(new MaterialLightAnimation(rect.right, rect.bottom));
		exampleList.Add(new BasicScene(rect.right, rect.bottom));
		exampleList.Add(new FoggyScene(rect.right, rect.bottom));
		exampleList.Add(new ForcedMaterials(rect.right, rect.bottom));		
		exampleList.Add(new Geometries(rect.right, rect.bottom));
		exampleList.Add(new CustomGeometry(rect.right, rect.bottom));
		exampleList.Add(new MeshProperties(rect.right, rect.bottom));
		exampleList.Add(new BothCameraExample(rect.right, rect.bottom));
		exampleList.Add(new CamerasLookAt(rect.right, rect.bottom));
		exampleList.Add(new AmbientLightExample(rect.right, rect.bottom));
		exampleList.Add(new SpotLightExample(rect.right, rect.bottom));
		exampleList.Add(new PointLightExample(rect.right, rect.bottom));
		exampleList.Add(new DirectionalLightExample(rect.right, rect.bottom));
		exampleList.Add(new HemisphereLightExample(rect.right, rect.bottom));
		exampleList.Add(new AreaLightExample(rect.right, rect.bottom));
		exampleList.Add(new LensflaresExample(rect.right, rect.bottom));
		exampleList.Add(new BasicMeshMaterialExample(rect.right, rect.bottom));
		exampleList.Add(new Basic2DGeometriesPlane(rect.right, rect.bottom));
	}
}

void CFormLeft::SetTarget(CWnd* m_cwnd)
{
	m_target = m_cwnd;
}

void CFormLeft::OnLbnSelchangeExample()
{
	int index = m_listBox.GetCurSel();
	ThreeDemoClass* example = (ThreeDemoClass*)exampleList.GetAt(index);
	CGLControl* glControl = ((CFormRight*)m_target)->m_glControl;
	glControl->SetExample(example);
	glControl->Clear();
	example->renderer->clear();
	glControl->Render();
}


void CFormLeft::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_listBox)
		m_listBox.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

}


void CFormLeft::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}

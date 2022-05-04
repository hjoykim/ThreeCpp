#pragma once
#include <afxwin.h>
#include <ecore/ThreeDemoModel.h>
#include <imgui/imgui.h>
#pragma comment(lib,"OpenGL32.lib")
#ifdef GLES
#include <EGL/egl.h>
#endif
class CGLControl :    public CWnd
{
   DECLARE_DYNCREATE(CGLControl)
public :
    UINT_PTR m_unitTimer;
private :
    CWnd* hWnd;
    HDC hdc;
    HGLRC hrc;
    int m_nPixleFormat;
    BOOL m_bHandleCreated;
    int m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight;
    ImGuiIO* imGuiIO;
    MSG msg;

#ifdef GLES
    HWND nativeWindow = NULL;
    EGLDisplay display = NULL;
    EGLConfig config = NULL;
    EGLSurface surface = NULL;
    EGLContext context = null;
    bool testEGLError(HWND nativeWindow, const char* functionLastCalled);
#endif
    void SetViewport(int x, int y, int width, int height);


public :
    float m_fAspect;
    bool m_isAnimate = true;
    ThreeDemoClass* demo;
    bool show_demo_window = true;
    bool show_another_window = true;

    CGLControl();
    virtual ~CGLControl();
    HGLRC GetGLContext(void) { return hrc; }
    void SetGLContext(const HGLRC& glContext) { hrc = glContext; }
    void SetExample(ThreeDemoClass* examplePtr) { demo = examplePtr; }

    void Clear();

    void Render();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);   
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnTimer(UINT_PTR nIDEvent);   
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};


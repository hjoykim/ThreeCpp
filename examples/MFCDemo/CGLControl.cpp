#include "pch.h"
#include "CGLControl.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_opengl3.h>
#ifdef GLES
#include "GLESHeader.h"
#endif
IMPLEMENT_DYNCREATE(CGLControl,CWnd)


BEGIN_MESSAGE_MAP(CGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CGLControl::SetViewport(int x, int y, int width, int height)
{
	m_viewportX = x;
	m_viewportY = y;
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
}

CGLControl::CGLControl()
{
	ZeroMemory(&msg, sizeof(msg));
}

CGLControl::~CGLControl()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (hrc) 
	{
		wglDeleteContext(hrc);
		hrc = NULL;
	}
}

void CGLControl::Clear()
{
	GLfloat clear_buffer[] = { 0.0f,0.0f,0.0f,1.0f };
	glClearBufferfv(GL_COLOR, 0, clear_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CGLControl::Render()
{
	RECT rect;
	GetClientRect(&rect);

	if (demo) {
		if (!demo->initialized) {
			Color c = demo->renderer->getClearColor();
			//glClear(GL_COLOR_BUFFER_BIT);
			Clear();
			glClearColor(c.r, c.g, c.b, 1.0f);
			demo->renderer->clear(true, true, true);
			demo->init();
			demo->initialized = true;
			if (demo->controller)
				demo->controller->sizeChanged(Vector4(0, 0, rect.right, rect.bottom));
			demo->renderer->setViewport(0, 0, rect.right, rect.bottom);
		}

		if (!imGuiIO->WantCaptureMouse)
			demo->controller->update();

		demo->render();
	}

	/*if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return;
	}*/

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (demo)
		demo->showControls();

	ImGui::Render();
	glViewport(0, 0, rect.right, rect.bottom);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SwapBuffers(hdc);
	

}

BOOL CGLControl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

void CGLControl::OnPaint()
{
	//CPaintDC dc(this); 

	ValidateRect(NULL);
}


void CGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;
	m_fAspect = (float)cy / (float)cx;
	if (GetDC()->GetSafeHdc()) {
		if (demo) {
			if (demo->controller)
				demo->controller->sizeChanged(Vector4(0, 0, cx, cy));

			demo->renderer->setViewport(0, 0, cx, cy);
		}
		Render();
	}
}

#ifdef GLES
bool CGLControl::testEGLError(HWND nativeWindow, const char* functionLastCalled) {
	/* eglGetError returns the last error that occurred using EGL, not necessarily the status of the last called function. The user has to
	   check after every single EGL call or at least once every frame. Usually this would be for debugging only, but for this example
	   it is enabled always
	*/
	EGLInt lastError = eglGetError();
	if (lastError !+ EGL_SUCCESS) {
		TCHAR stringBuffer[256];
		_stprintf_s(stringBuffer, _T("%s failed (%x).\n"), functionLastCalled, lastError);
		//MessageBox(nativeWindow,stringBuffer,ERROR_TITLE,MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}
int CGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;
	CDC* pDC = GetDC();
	hdc = pDC->GetSafeHdc();
	display = eglGetDisplay(hdc);
	nativeWindow = GetSafeHwnd();
	if (display == EGL_NO_DISPLAY) {
		display = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	}
	// If a display still couldn't be obtained, return an error.
	if (display == EGL_NO_DISLAY) {
		//MessageBox(0,_T("Failed to get an EGLDisplay"),ERROR_TITLE,MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	EGLint eglMajorVersion = 0;
	EGLint eglMinorVersion = 0;
	if (!eglInitialize(display, &eglMajorVersion, &eglMinorVersion)) {
		//MessageBox(0,_T("Failed to get an EGLDisplay"),ERROR_TITLE,MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	// Bind the correct API

	int result = EGL_FALSE;

	result = eglBindAPI(EGL_OPENGL_ES_API);

	if (result!+ EGL_TRUE)
		return false;

	const EGLint configurationAttributes[] = { EGL_SURFACE_TYPE,EGL_WINDOW_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_NONE };
	EGLint configsReturned;
	if (!eglChooseConfig(display, configurationAttributes, &config, 1, &configsReturned) || (configsReturned != 1)) {
		//MessageBox(0,_T("eglChooseConfig() failed."),ERROR_TITLE,MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	surface = eglCreateWindowSurface(display, config, nativeWindow, NULL);
	if (surface == EGL_NO_SURFACE) {
		eglGetError();
		surface = eglCreateWindowSurface(display, config, NULL, NULL);
	}

	if (!testEGLError(nativeWindow, "eglCreateWindowSurface")) return false;

	eglBindAPI(EGL_OPENGL_ES_API);
	if (!testEGLError(nativeWindow, "eglBindAPI")) return false;

	EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE };
	context = eglCreateContext(display, config, NULL, contextAttributes);
	if (!testEGLError(nativeWindow, "eglCreateContext")) return false;

	eglMakeCurrent(display, surface, surface, context);
	if (!testEGLError(nativeWindow, "eglMakeCurrent")) return false;
#ifdef __APPLE__
	static const char* NAMES[] = { "libEGL.dylib" };
#elif defined _WIN32
	static const char* NAMES[] = { "libEGL.dll" };
#else 
	static const char* NAMES[] = { "libEGL.so.1","libEGL.so" };
#endif
	global_egl_handle = open_handle(NAMES, sizeof(NAMES) / sizeof(NAMES[0]));
	if (global_egl_handle) {
		gladLoadGLES2Loader(GLADloadproc)load_with_egl);
		close_handle(global_egl_handle);
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	imGuiIO = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(GetSafeHwnd());
	ImGui_ImplOpenGL3_Init(0);

	SetTimer(0, 1, NULL);
	return 0;
}

#else
int CGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	CDC* pDC = GetDC();
	hdc = pDC->GetSafeHdc();
	PIXELFORMATDESCRIPTOR pfd;

	// Set Window Pixel Format
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Size of this pfd
	pfd.nVersion = 1;							// Version number : must be 1
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |		// Support window
		PFD_SUPPORT_OPENGL |		// Support OpenGL
		PFD_DOUBLEBUFFER |		// Double buffered
		PFD_GENERIC_FORMAT;

	pfd.iPixelType = PFD_TYPE_RGBA;	// RGBA type
	pfd.cColorBits = 24;				// Specifies the number of color biCXD3Planes in each color buffer
	pfd.cRedBits = 0;				// Specifies the number of red biCXD3Planes in each RGBA color buffer
	pfd.cRedShift = 0;				// Specifies the shift count for red biCXD3Planes in each RGBA color buffer
	pfd.cGreenBits = 0;				// Specifies the number of green biCXD3Planes in each RGBA color buffer
	pfd.cGreenShift = 0;				// Specifies the shift count for green biCXD3Planes in each RGBA color buffer
	pfd.cBlueBits = 0;				// Specifies the number of blue biCXD3Planes in each RGBA color buffer
	pfd.cBlueShift = 0;				// Specifies the shift count for blue biCXD3Planes in each RGBA color buffer
	pfd.cAlphaBits = 0;				// Specifies the number of alpha biCXD3Planes in each RGBA color buffer. Alpha biCXD3Planes are not supported
	pfd.cAlphaShift = 0;				// Specifies the shift count for alpha biCXD3Planes in each RGBA color buffer. Alpha biCXD3Planes are not supported
	pfd.cDepthBits = 24;				// Specifies the depth of the depth (z-axis) buffer
	pfd.cStencilBits = 8;				// Specifies the depth of thef stencil buffer
	pfd.cAuxBuffers = 0;				// Specifies the number of auxiliary buffers. Auxiliary buffers are not supported
	pfd.iLayerType = PFD_MAIN_PLANE;	// Ignored. Earlier implementations of OpenG used this member, but it is no longer used
	pfd.bReserved = 0;				// Specifies the number of overlay and underlay planes
	pfd.dwLayerMask = 0;				// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
	pfd.dwVisibleMask = 0;				// Specifies the transparent color or index of an underlay plane
	pfd.dwDamageMask = 0;				// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used

	// Attempts to match an appropriate pixel format supported by a device context to a given pixel format specification
	int m_GLPixelIndex = ChoosePixelFormat(hdc, &pfd);
	// Choose default
	if (m_GLPixelIndex == 0)
	{
		// Obtains information about the pixel format identified by iPixelFormat of the device associated with hdc
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hdc, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)	return FALSE;
	}
	//Sets the pixel format of the specified device context to the format specified by the iPixelFormat index
	if (!SetPixelFormat(hdc, m_GLPixelIndex, &pfd))	return FALSE;
	//Create Rendering Context
	hrc = ::wglCreateContext(pDC->GetSafeHdc());
	if (hrc == 0)	return FALSE;	//Failure to Create Rendering Context

	// if(m_hMasterGLContext) // HGLRC m_hMasterGLContext;
	// hglrc1 : Specifies the OpenGL rendering context with which to share display lists.
	// hglrc2 : Specifies the OpenGL rendering context to share display lists with hglrc1.
	//			The hglrc2 parameter should not contain any existing display lists when wglShareLists is called.
	// wglShareLists(m_hMasterGLContext, _hGLContext);}

	// Make the RC Current
	if (::wglMakeCurrent(pDC->GetSafeHdc(), hrc) == FALSE)	return FALSE;
#ifdef GLEW
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		AfxMessageBox(_T("Graphic Driver of your PC is not Supported, Please Upgrade your graphic card!"));
		return FALSE;
	}
#endif
#ifdef GLAD
	int success = gladLoadGL();
#endif

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	imGuiIO = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(GetSafeHwnd());
	ImGui_ImplOpenGL3_Init(0);

	SetTimer(0, 1, NULL);
	return 0;

}
#endif

void CGLControl::OnDestroy()
{
	CWnd::OnDestroy();

}


BOOL CGLControl::OnEraseBkgnd(CDC* pDC)
{

	return CWnd::OnEraseBkgnd(pDC);
}


void CGLControl::OnMouseMove(UINT nFlags, CPoint point)
{

	CWnd::OnMouseMove(nFlags, point);
	if (demo && demo->controller) {
		demo->controller->mouseMove(point.x, point.y);
		if (!m_isAnimate) Render();
	}
}


void CGLControl::OnLButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonUp(nFlags, point);
	if (demo && demo->controller) {
		demo->controller->mouseUp();
		if (!m_isAnimate) Render();
	
	}
}


void CGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnRButtonUp(nFlags, point);
	if (demo && demo->controller) {
		demo->controller->mouseUp();
		if (!m_isAnimate) Render();

	}
}


BOOL CGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	
	if (demo && demo->controller) {
		demo->controller->mouseWheel(zDelta);
		if (!m_isAnimate) Render();

	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLControl::OnTimer(UINT_PTR nIDEvent)
{

	if (m_isAnimate) Render();
	CWnd::OnTimer(nIDEvent);
}

void CGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonDown(nFlags, point);
	if (demo && demo->controller) {
		demo->controller->mouseDown(0, point.x, point.y);
		if (!m_isAnimate) Render();
	}
}


void CGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{

	CWnd::OnRButtonDown(nFlags, point);
	if (demo && demo->controller) {
		demo->controller->mouseDown(2, point.x, point.y);
		if (!m_isAnimate) Render();
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CGLControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(GetSafeHwnd(), message, wParam, lParam))
		return true;

	switch (message) {
	case WM_DESTROY :
		::PostQuitMessage(0);
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}



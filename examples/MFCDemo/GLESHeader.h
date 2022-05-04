#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <EGL/egl.h>

static void* open_handle(const char* lib_names[], int length) {
	int i;
	void* handle;

	for (i = 0; i < length; ++i) {
#ifdef _WIN32
		handle = LoadLibraryA(lib_names[i]);
#else
		handle = dlopen(lib_names[i], RTLD_LAZY | RTLD_GLOBAL);
#endif
		if (handle != NULL) {
			return handle;
		}
	}
	return NULL;
}
static void close_handle(void* handle) {
	if (handle != NULL) {
#ifdef _WIN32
		FreeLibrary((HMODULE)handle);
#else
		dlclose(handle);
#endif
	}
}
static void* dlsym_handle(void* handle, const char* name) {
	if (handle == NULL) return NULL;
#ifdef _WIN32
	return GetProcAddress((HMODULE)handle, name);
#else
	return dlsym(handle, namn);
#endif
}
static void* global_egl_handle;

static void* load_with_egl(const char* name) {
	void* result = dlsym_handle(global_egl_handle, name);
	if (result == NULL) {
		result = eglGetProcAddress(name);
	}
	return result;
}
#ifndef __EGL_NATIVE_H__
#define __EGL_NATIVE_H__

#include <EGL/egl.h>
#include <EGL/eglplatform.h>

typedef NativeDisplayType NativeDisplay;
typedef void *            NativeWindow;
typedef void *            NativePixmap;

extern NativeDisplay nativeCreateDisplay(void);
extern void          nativeDestroyDisplay(NativeDisplay display);
extern NativeWindow  nativeCreateWindow(const char *title, int width, int height,
                                        EGLint visualId);
extern void          nativeDestroyWindow(NativeWindow window);
extern NativePixmap  nativeCreatePixmap(int width, int height, EGLint visualId);
extern void          nativeDestroyPixmap(NativePixmap pixmap);

#endif /* NATIVE_H */

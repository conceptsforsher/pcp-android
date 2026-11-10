#include <android_native_app_glue.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#define LOG_TAG "PCP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static struct { android_app* app; bool hasSurface, hasFocus, initialized, destroy; EGLDisplay dpy; EGLSurface surf; EGLContext ctx; int w, h; } g;

static void cmd(struct android_app* app, int32_t c) {
    switch(c){case 0xFF:g.hasSurface=1;break;case 0x100:g.hasSurface=0;break;case 0x102:g.hasFocus=1;break;case 0x103:g.hasFocus=0;break;case 0x106:g.destroy=1;break;}
}

void android_main(struct android_app* app) {
    LOGI("PCP"); g.app=app; app->onAppCmd=cmd;
    while(!g.destroy){int e;struct android_poll_source*s;ALooper_pollAll(g.hasSurface?0:-1,0,&e,(void**)&s);if(s)s->process(app,s);if(g.destroy)break;
    if(g.hasSurface&&g.hasFocus&&!g.initialized){const EGLint a[]={0x3040,0x40,0x3033,0x3038,0x3024,8,0x3023,8,0x3022,8,0x3021,8,0x3025,24,0x3026,8,0x3038};EGLint n;EGLConfig c;
    g.dpy=eglGetDisplay(0);eglInitialize(g.dpy,0,0);eglChooseConfig(g.dpy,a,&c,1,&n);g.surf=eglCreateWindowSurface(g.dpy,c,app->window,0);EGLint ca[]={0x3098,3,0x3038};
    g.ctx=eglCreateContext(g.dpy,c,0,ca);eglMakeCurrent(g.dpy,g.surf,g.surf,g.ctx);eglQuerySurface(g.dpy,g.surf,0x3057,&g.w);eglQuerySurface(g.dpy,g.surf,0x3056,&g.h);
    glViewport(0,0,g.w,g.h);g.initialized=1;LOGI("Ready %dx%d",g.w,g.h);}
    if(g.initialized&&g.hasSurface){glClearColor(0.06f,0.07f,0.11f,1);glClear(0x4000);eglSwapBuffers(g.dpy,g.surf);}}
    if(g.dpy){eglMakeCurrent(g.dpy,0,0,0);if(g.ctx)eglDestroyContext(g.dpy,g.ctx);if(g.surf)eglDestroySurface(g.dpy,g.surf);eglTerminate(g.dpy);}
}
extern"C"{JNIEXPORT void JNICALL Java_chat_pcp_app_PcpApplication_nativeOnCreate(JNIEnv*,jobject){}JNIEXPORT void JNICALL Java_chat_pcp_app_PcpApplication_nativeOnTerminate(JNIEnv*,jobject){}}

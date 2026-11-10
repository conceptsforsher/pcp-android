package chat.pcp.app;

import android.app.Application;
import android.content.Context;

public class PcpApplication extends Application {
    static {
        System.loadLibrary("pcp_app");
    }

    private static Context appContext;

    @Override
    public void onCreate() {
        super.onCreate();
        appContext = getApplicationContext();
        nativeOnCreate();
    }

    @Override
    public void onTerminate() {
        nativeOnTerminate();
        super.onTerminate();
    }

    public static Context getContext() {
        return appContext;
    }

    private native void nativeOnCreate();
    private native void nativeOnTerminate();
}

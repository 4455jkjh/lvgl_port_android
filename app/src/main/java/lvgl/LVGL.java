package lvgl;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.view.View;

public class LVGL implements SurfaceHolder.Callback {
    private  LVGL(){}

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Surface surface = holder.getSurface();
        native_create(surface);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Surface surface = holder.getSurface();
        native_changed(surface, width, height, 0);
    }
    int i;
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Surface surface = holder.getSurface();
        native_destroy(surface);
    }

    static{
        System.loadLibrary("lvgl");
    }
    private static native void native_create(Surface surface);
    private static native void native_changed(Surface surface, int width, int height ,int type);
    private static native void native_destroy(Surface surface);
    public static native void native_touch(int x, int y, boolean touch);
    public static LVGL INSTANCE = new LVGL();
}
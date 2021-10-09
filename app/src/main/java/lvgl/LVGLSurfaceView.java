package lvgl;

import android.view.SurfaceView;
import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class LVGLSurfaceView extends SurfaceView {
    public LVGLSurfaceView(Context c) {
        super(c);
    }
    public LVGLSurfaceView(Context c, AttributeSet a) {
        super(c, a);
        getHolder().addCallback(LVGL.INSTANCE);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int x = (int) event.getX();
        int y = (int) event.getY();
        if (event.getAction() == MotionEvent.ACTION_UP)
            LVGL.native_touch(x, y, false);
        else
            LVGL.native_touch(x, y, true);
        return true;
    }

}
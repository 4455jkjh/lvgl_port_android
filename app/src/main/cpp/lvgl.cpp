#include <android/native_window_jni.h>
#include <android/log.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "lvgl.h"
#include "lvgl/lvgl.h"
#include "lv_demos/lv_demo.h"
#include "lv_clock/lv_clock.h"

struct saved_state {
    int32_t x;
    int32_t y;
    bool is_touched;
};

static int WIDTH = 0;
static int HEIGHT = 0;
#define DISP_BUF_SIZE 128 * 1024

static ANativeWindow *window;
static lv_color_t lv_buf_1[DISP_BUF_SIZE];
static lv_color_t lv_buf_2[DISP_BUF_SIZE];
static pthread_t thread;
static bool run = false;
static saved_state state;

static lv_disp_draw_buf_t lv_disp_buf;
static void render(){
    ANativeWindow_Buffer buffer;
    ANativeWindow_lock(window, &buffer, 0);
    memset(buffer.bits, 0xff, buffer.stride * buffer.height * 4);
    ANativeWindow_unlockAndPost(window);
}
static void copy_px(uint8_t *data, lv_color_t *color_p, int w) {
    for(int i = 0; i < w; i++) {
        data[0] = color_p->ch.red;
        data[1] = color_p->ch.green;
        data[2] = color_p->ch.blue;
        data[3] = color_p->ch.alpha;
        color_p ++;
        data += 4;
    }
}
static uint32_t *buf;
static void window_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    __android_log_print(ANDROID_LOG_INFO, "LVGL", "func:%s", __func__);
    int left = area->x1;
    if(left <0)
        left = 0;
    int right = area->x2 +1;
    if(right > WIDTH)
        right=WIDTH;
    int top = area->y1;
    if(top<0)
        top = 0;
    int bottom = area->y2 +1;
    if(bottom >HEIGHT)
        bottom=HEIGHT;
    int32_t y;

    ANativeWindow_Buffer buffer;
    ANativeWindow_lock(window, &buffer, 0);
    uint32_t *data = (uint32_t*)buffer.bits;
    uint32_t *dest = buf + top * WIDTH + left;
    int w = right - left;
    for(y = top; y < bottom; y++){
        copy_px((uint8_t*)dest, color_p, w);
        dest += WIDTH;
        color_p += w;
    }
    uint32_t *src = buf;
    for(int i = 0; i < buffer.height; i++){
        memcpy(data, src, WIDTH * 4);
        src += WIDTH;
        data += buffer.stride;
    }
    ANativeWindow_unlockAndPost(window);
    lv_disp_flush_ready(disp_drv);
}
static void *refresh_task(void *data) {
    while(run) {
        //usleep(10000);
        lv_task_handler();
        usleep(50000);
    }
    return 0;
}
static void LvTouchRead(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(state.is_touched) {
        data->point.x = state.x;
        data->point.y = state.y;
        data->state = LV_INDEV_STATE_PR;
    } else
        data->state = LV_INDEV_STATE_REL;
   // return false;
}
void native_create(JNIEnv *env, jclass cls, jobject surface){
    __android_log_print(ANDROID_LOG_INFO, "LVGL", "func:%s", __func__);
    window = ANativeWindow_fromSurface(env, surface);
    lv_init();
    lv_disp_draw_buf_init(&lv_disp_buf, lv_buf_1, lv_buf_2, DISP_BUF_SIZE);
}
static void gui_init(int type) {
    switch(type) {
        case 1:
            lv_demo_widgets();
            break;
        case 2:
            lv_demo_benchmark();
            break;
        case 3:
            lv_demo_stress();
            break;
        case 4:
            lv_demo_keypad_encoder();
            break;
        case 5:
            lv_demo_music();
            break;
        default:
            //panel_main(WIDTH, HEIGHT);
            lv_100ask_stm32_tool_clock();
            break;
    }
}
void native_changed(JNIEnv *env, jclass cls, jobject surface, jint width, jint height, jint type){
    __android_log_print(ANDROID_LOG_INFO, "LVGL", "func:%s", __func__);
    if(run)
        return;
    WIDTH = width / 2;
    HEIGHT = height / 2;
    buf = new uint32_t[WIDTH * HEIGHT];
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = WIDTH;
    disp_drv.ver_res = HEIGHT;
    disp_drv.draw_buf = &lv_disp_buf;

    // TODO Use acceleration structure for drawing
    disp_drv.gpu_fill_cb = nullptr;
    disp_drv.gpu_wait_cb = nullptr;
    disp_drv.flush_cb = window_flush;

    lv_disp_drv_register(&disp_drv);
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = LvTouchRead;

    lv_indev_drv_register(&indev_drv);

    ANativeWindow_setBuffersGeometry(window, WIDTH, HEIGHT, WINDOW_FORMAT_RGBA_8888);
    render();
    gui_init(type);
    pthread_create(&thread, 0, refresh_task, 0);
    run = true;
}
void native_destroy(JNIEnv *env, jclass cls, jobject surface){
    run=false;
    __android_log_print(ANDROID_LOG_INFO, "LVGL", "func:%s", __func__);
}

void native_touch(JNIEnv *env, jclass cls, jint x, jint y, jboolean is_touched) {
    state.x = x /2;
    state.y = y / 2;
    state.is_touched = is_touched;

}

#include "lvgl/time.h"

uint32_t currentTimeInMilliseconds() {
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
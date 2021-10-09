#ifndef LV_100ASK_STM32_TOOL_CLOCK_H
#define LV_100ASK_STM32_TOOL_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lvgl.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct _lv_100ask_clock {
	lv_obj_t * bg_tool_clock;     			// ����
	lv_obj_t * g_gauge_clock;	 			// ����
	lv_obj_t * g_label_clock_time;    	 	// չʾʱ����Ϣ
} T_lv_100ask_clock, *PT_lv_100ask_clock;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_100ask_stm32_tool_clock(void);


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_100ASK_STM32_TOOL_CLOCK_H */






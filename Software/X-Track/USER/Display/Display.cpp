#include "Display.h"
#include "HAL/HAL.h"
#include "App/App.h"
#include "lv_examples/lv_examples.h"

/**
  * @brief  通用显示初始化
  * @param  无
  * @retval 无
  */
static void Display_CommonInit()
{
    App_Init();
    //lv_demo_benchmark();
    //lv_demo_stress();
    //lv_demo_widgets();
    //lv_demo_music();
    //lv_demo_keypad_encoder();
}

#ifdef ARDUINO

#define USE_FPS_TEST 0
#if (USE_FPS_TEST == 1) 
static float Display_GetFPS(SCREEN_CLASS* scr, uint32_t loopNum);
#endif

/**
  * @brief  显示初始化
  * @param  无
  * @retval 无
  */
void Display_Init()
{
    static SCREEN_CLASS screen(
        SCREEN_CS_PIN,
        SCREEN_DC_PIN, 
        SCREEN_RST_PIN
    );
    
    /* 屏幕初始化 */
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
#if (USE_FPS_TEST == 1)
    Display_GetFPS(&screen, 50);
    while(1);
#endif
    
    /* 自动错误诊断初始化 */
    DisplayFault_Init(&screen);
    
    /* lvgl初始化 */
    lv_init();
    lv_port_disp_init(&screen);
    lv_port_indev_init();
    lv_fs_if_init();
    //lv_port_log_init();
    
    Display_CommonInit();
    
    /*背光渐亮*/
    HAL::Backlight_SetGradual(HAL::Backlight_GetBKP(), 1000);
}

#if (USE_FPS_TEST == 1) 
static float Display_GetFPS(SCREEN_CLASS* scr, uint32_t loopNum)
{
    uint32_t startTime = millis();
    for(uint32_t f = 0; f < loopNum; f++)
    {
        scr->fillScreen(SCREEN_CLASS::Red);
        scr->fillScreen(SCREEN_CLASS::Green);
        scr->fillScreen(SCREEN_CLASS::Blue);
    }
    uint32_t costTime = millis() - startTime;
    float fps = loopNum * 3 / (costTime / 1000.0f);
    
    scr->fillScreen(SCREEN_CLASS::Blue);
    scr->setTextSize(1);
    scr->setTextColor(SCREEN_CLASS::White);
    scr->setCursor(0, scr->height() / 2);
    
    scr->print("Frame:");
    scr->println(loopNum * 3);
    
    scr->print("Time:");
    scr->print(costTime);
    scr->println("ms");
    
    scr->print("FPS:");
    scr->println(fps);
    
    return fps;
}
#endif

#else

void Display_Init()
{
    Display_CommonInit();
}

#endif

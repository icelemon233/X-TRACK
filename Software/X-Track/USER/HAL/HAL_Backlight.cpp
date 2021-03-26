#include "Display/Display.h"
#include "HAL.h"

/**
  * @brief  背光亮度渐变，受lv_anim控制
  * @param  obj:无用
  * @param  brightness:亮度值
  * @retval None
  */
static void Backlight_AnimCallback(void * obj, int16_t brightness)
{
    HAL::Backlight_SetValue(brightness);
}

/**
  * @brief  背光初始化
  * @param  无
  * @retval 无
  */
void HAL::Backlight_Init()
{
    /*PWM初始化，1000级，20KHz频率*/
    PWM_Init(SCREEN_BLK_PIN, 1000, 20000);
    Backlight_SetValue(0);
}

/**
  * @brief  背光设置，渐变效果
  * @param  target:目标亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void HAL::Backlight_SetGradual(uint16_t target, uint16_t time)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)Backlight_AnimCallback);
    lv_anim_set_values(&a, Backlight_GetValue(), target);
    lv_anim_set_time(&a, time);
    
    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_out);
    lv_anim_set_path(&a, &path);
    
    lv_anim_start(&a);
}

/**
  * @brief  获取背光亮度
  * @param  无
  * @retval 当前亮度(0~1000 -> 0~100%)
  */
uint16_t HAL::Backlight_GetValue()
{
    return timer_get_compare(PIN_MAP[SCREEN_BLK_PIN].TIMx, PIN_MAP[SCREEN_BLK_PIN].TimerChannel);
}

/**
  * @brief  设置背光亮度
  * @param  val: 亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void HAL::Backlight_SetValue(int16_t val)
{
    __LimitValue(val, 0, 1000);
    analogWrite(SCREEN_BLK_PIN, val);
}

/**
  * @brief  从后备寄存器获取掉电保存的背光亮度
  * @param  无
  * @retval 亮度(0~1000 -> 0~100%)
  */
uint16_t HAL::Backlight_GetBKP()
{
    uint16_t value = 0;
    
    /*亮度值是否非法*/
    if(value == 0 || value > 1000)
    {
        /*设置默认亮度*/
        value = 1000;
    }
    
    return value;
}

/**
  * @brief  将当前亮度更新至后备寄存器
  * @param  无
  * @retval 无
  */
void HAL::Backlight_UpdateBKP()
{
}


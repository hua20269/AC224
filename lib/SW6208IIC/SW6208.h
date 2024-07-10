#ifndef _SW6208_H
#define _SW6208_H
#include <iic.h>
#define SW6208_address 0x3C // 设备地址
// define MAX_AES_BUFFER_SIZ 1
//  ntc相关参数
#define BX 3435
#define T25 298.15
#define R25 10000
#ifndef uint16_t
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
#endif

void SW6208_init();                                                  // sw的初始化
float batteryV();                                                    // 电池电压的读取和计算
float battery_outinV();                                              // 输入输出电压
float battery_ictemp();                                              // ic温度
float battery_ntcV();                                                // ntc电压
float battery_inA();                                                 // 系统输入电流
float battery_outA();                                                // 系统输出电流的获取
float battery_volume();                                              // 电池容量获取
uint16_t battery_per();                                              // 电池百分比获取
void sysstate(uint8_t *H_value, uint8_t *L_value, float *battery_A); // 系统充放电L口和A1口的状态

uint8_t Sink_Protocol();   // 充电协议
uint8_t Source_Protocol(); // 放电协议
/**
 * @brief 小电流开关用到的函数
 */
void kqxdl();          // 开启小电流
void I2Csmall_A_ON();  // 小电流使能
void I2Csmall_A_OFF(); // 小电流不使能
uint8_t xdlzt();       // 小电流状态
void A2_ON();          // A2口插入
void A2_OFF();         // A2口拔出

void A1shijian(); // 触发A1口的插入

void AC_OFF(); // 关闭所有输出口  bit 自动清零
void Small_A_Set(); // 小电流预打开   按键作用
void NTCLimit();    // 设置NTC温度上限保护50℃ 改为55℃     温度每上升1度，电压下降800mv
void Open12V();     // 打开12V输入

#endif

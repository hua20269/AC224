#include <esp32gfx.h>
#include <Background.h>
#include <font1.h>
#include <imge.h>
#include <eeprom32.h>
LGFX lcd;                  // 主显示对象
LGFX_Sprite sprite1(&lcd); // 内存画板
// LGFX_Sprite sprite2(&sprite1);
int color = TFT_WHITE;                                                          // 默认白色  调用更改使用
int colorRed = TFT_RED;                                                         // 默认红色  调用更改使用
const int pink1 = lcd.color565(225, 153, 192);                                  // 粉色
const int violef1 = lcd.color565(114, 8, 188);                                  // 深紫色
const int green1 = lcd.color565(137, 230, 146);                                 // 深紫色
const int green2 = lcd.color565(44, 33, 189);                                   // 框2的颜色
const int green3 = lcd.color565(83, 161, 180);                                  // 框1的颜色
const int gre = lcd.color565(34, 208, 186);                                     // 深紫色
const int lv1 = lcd.color565(31, 143, 71);                                      // 深紫色
const int qianhui = lcd.color565(30, 30, 30);                                   // 浅灰色
const int lvse1 = lcd.color565(207, 244, 210);                                  // 绿色
const int lvse2 = lcd.color565(123, 228, 149);                                  // 绿色
const int lvse3 = lcd.color565(86, 197, 150);                                   // 绿色
const int lvse4 = lcd.color565(50, 157, 156);                                   // 绿色
const int lvse5 = lcd.color565(32, 80, 114);                                    // 绿色
int x11 = 0, y11 = 0, x22 = 90, y22 = 0, wh1 = 24, w11 = 25, h11 = 25, r11 = 3; // 位置定位
int fontsize1 = 20, space = 2;
uint16_t ic_ntc = 0;

void DisplayInit() // 屏幕初始化
{
    // 显示库初始化函数
    lcd.init();
    // 设置屏幕显示方向
    if (EEPROM.read(3) == 3)
    {
        lcd.setRotation(3);
    }
    else
        lcd.setRotation(1);
    // if (EEPROM.read(11) == 1) // OTA 更新  给WiFi退让电流
    //     lcd.setBrightness(250);//调暗
    // else
    lcd.setBrightness(170); // 设置屏幕显示亮度
    // 初始化画布及清屏
    // sprite1.createSprite(240, 135); // 设置屏幕大小
    // sprite1.fillScreen(TFT_BLACK);  // 黑色背景
    // sprite1.pushSprite(0, 0);
}

void lcdRotation() // 连接蓝牙实时刷新屏幕方向
{
    if (EEPROM.read(3) == 3)
    {
        lcd.setRotation(3);
    }
    else
        lcd.setRotation(1);
}

void offscreen() //  lcd.sleep  息屏
{
    lcd.sleep();
    lcd.setBrightness(255); // 息屏
}

void onscreen() // 唤醒屏幕
{
    lcd.wakeup();
    lcd.setBrightness(170);
}

void RefreshDisplay() // 刷新画布到屏幕
{
    sprite1.pushSprite(0, 0);
    // sprite1.deleteSprite();
}

void sys_init(uint16_t timenum)
{
    char draw_num[5];
    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    sprite1.setTextColor(TFT_GREEN);
    sprite1.loadFont(alibb);
    sprite1.setTextDatum(CC_DATUM); // 字体居中
    sprite1.drawString("The system is initializing", 120, 40);
    sprite1.drawRoundRect(66, 53, 110, 30, 1, TFT_WHITE);
    sprite1.drawRoundRect(67, 54, 108, 28, 1, TFT_WHITE);
    sprite1.drawRoundRect(68, 55, 106, 26, 1, TFT_WHITE);
    sprite1.fillRoundRect(71, 58, timenum, 20, 3, TFT_WHITE);
    sprintf(draw_num, "%d%%", timenum);
    sprite1.drawString(draw_num, 120, 68);
    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

void PowerLOGO() // 开机LOGO
{
    // 开始画布
    sprite1.createSprite(240, 135);  // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);   // 设置背景颜色
    sprite1.loadFont(KaiTi22);       // 字体设置
    sprite1.setTextColor(TFT_WHITE); // 字体颜色
    // sprite1.setTextDatum(TL_DATUM);  // 字体左上角
    sprite1.setTextDatum(CC_DATUM); // 字体居中（坐标）

    sprite1.drawString("LOGO", 120, 67);
    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

/**
 * @brief 主题1 显示健康度
 * @param batv 电池电压
 * @param bata 电池电流
 * @param ACstate AC口状态
 * @param temp ic温度
 * @param sysv 系统电压
 * @param sysw 功率
 * @param workp 充电状态
 * @param batvolume 电池容量
 * @param xnum  电池循环次次数
 * @param batper 电池百分比
 * @param battemp 电池温度
 */
void lcdlayout01(uint16_t xunhuan, uint8_t bat_per, float battery_V, float ic_temp, float sys_outinv, float battery_A, float bat_ntc, uint8_t sys, uint8_t A_C, uint8_t bt_icon)
{
    char num_n[10];
    float a = 0, b = 0;
    int bat = 0;
    if (bat_per > 100)
        bat_per = 100;
    if (bat_per < 0)
        bat_per = 0;

    if (bat_per >= 0 && bat_per < 15)
        bat = 7;
    if (bat_per >= 15 && bat_per < 30)
        bat = 6;
    if (bat_per >= 30 && bat_per < 45)
        bat = 5;
    if (bat_per >= 45 && bat_per < 60)
        bat = 4;
    if (bat_per >= 60 && bat_per < 75)
        bat = 3;
    if (bat_per >= 75 && bat_per < 90)
        bat = 2;
    if (bat_per >= 90 && bat_per <= 100)
        bat = 1;
    sprite1.createSprite(240, 135); // 设置精灵画布大小    //横屏  240  135
    sprite1.fillScreen(TFT_BLACK);  // 背景填充
    sprite1.loadFont(KaiTi22);      // 设置自定义字库 字体
    sprite1.setTextDatum(TL_DATUM); // 字体左上角显示

    /**
     *  第一列 固定 eg
     */
    // mcu芯片温度T ，布局固定部分  列1行1
    sprite1.fillRoundRect(0, 0, 24, 24, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_RED);
    sprite1.drawString("芯", 1, 1); // 写字 跟位置

    // 电池电压U，
    sprite1.fillRoundRect(0, 27, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("池", 1, 28); // 写字 跟位置

    // 系统电压，
    sprite1.fillRoundRect(0, 54, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("压", 1, 55); // 写字 跟位置

    // 充放电流
    sprite1.fillRoundRect(0, 81, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("流", 1, 82); // 写字 跟位置

    // 充放功率
    sprite1.fillRoundRect(0, 111, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("功", 1, 112); // 写字 跟位置

    /**
     * 第三列 固定 eg
     */
    // 电池温度NTC  ，布局固定部分  列3行1
    sprite1.fillRoundRect(90, 0, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("温", 91, 1); // 写字 跟位置

    // 蓝牙图标
    //  sprite1.setSwapBytes(true); // RGB --> BGR
    // sprite1.pushImage(90, 27, 24, 24, imgbleimg24);
    sprite1.fillRoundRect(90, 27, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("池", 91, 28); // 写字 跟位置

    sprite1.loadFont(JianTi26); // 设置自定义字库 字体
    // A口状态 ，
    sprite1.fillRoundRect(90, 54, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("A", 94, 55); // 写字 跟位置

    // C口状态
    sprite1.fillRoundRect(90, 81, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("C", 94, 82); // 写字 跟位置

    sprite1.loadFont(KaiTi22);
    // 电池容量
    sprite1.fillRoundRect(90, 111, 24, 24, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("循", 90, 112); // 写字 跟位置
    /**
     * 第二列实时数据
     */
    sprite1.loadFont(JianTi20); // 设置自定义字库 字体
    sprite1.setTextColor(TFT_WHITE);
    // 温度值写入sprite1 &lcd
    // 芯片 温度 值写入sprite1 &lcd
    sprintf(num_n, "%.1f℃", ic_temp);
    sprite1.drawString(num_n, 27, 4); // 实时信息显示

    // 电池电压值写入sprite1 &lcd
    sprintf(num_n, "%.2fV", battery_V);
    sprite1.drawString(num_n, 27, 31); // 实时信息显示

    // 系统电压
    sprintf(num_n, "%.2fV", sys_outinv);
    sprite1.drawString(num_n, 27, 58); // 实时信息显示

    // 充放电流
    sprintf(num_n, "%.2fA", battery_A);
    sprite1.drawString(num_n, 27, 85); // 实时信息显示

    // 功率
    sprintf(num_n, "%.1fw", sys_outinv * battery_A);
    sprite1.drawString(num_n, 27, 115); // 实时信息显示

    /**
     * 第四列实时数据
     */
    // 电池温度NTC
    sprintf(num_n, "%.1f℃", bat_ntc);
    sprite1.drawString(num_n, 117, 4);

    // 蓝牙状态
    // sprite1.drawString("OFF", 117, 31);
    // sprite1.drawString("OFF", 117, 31);
    sprintf(num_n, "%.2fA", sys_outinv * battery_A / battery_V);
    sprite1.drawString(num_n, 117, 31); // 电池电流

    // A口状态
    if (A_C == 1 || A_C == 5)
        sprite1.drawString("ON", 117, 58);
    else
        sprite1.drawString("OFF", 117, 58);
    // C口状态
    if (A_C == 4 || A_C == 5)
        sprite1.drawString("ON", 117, 85);
    else
        sprite1.drawString("OFF", 117, 85);
    // 循环次数
    sprintf(num_n, "%d", xunhuan);
    sprite1.drawString(num_n, 117, 115); // 循环次数

    if (bt_icon) // 蓝牙
    {
        sprite1.drawPng(imgbleimg24, sizeof(imgbleimg24), 156, 111); // 蓝牙图标
    }

    /**
     * 右侧电池部分
     */
    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_WHITE);
    sprite1.setTextDatum(CC_DATUM); // 字体居中
    // 电量百分比   右上角
    sprintf(num_n, "%d", bat_per); // 传值
    if (bat_per < 10)
    { // 小于10   一个字符右移一点   改为红色字体
        sprite1.setTextColor(TFT_RED);
        sprite1.drawString(num_n, 211, 17); // 电量百分比红色显示
    }
    else if (bat_per == 100)
    {
        sprite1.drawString(num_n, 211, 17);
    }
    else
    {
        sprite1.drawString(num_n, 211, 17);
    }
    /**
     * 大电池图标
     */
    sprite1.fillRoundRect(208 - 6, 33, 20, 4, 2, TFT_GREEN); // 电池框
    sprite1.drawRoundRect(198 - 6, 36, 40, 66, 7, TFT_GREEN);
    sprite1.drawRoundRect(199 - 6, 37, 38, 64, 7, TFT_GREEN);
    switch (bat)
    {
    case 1:
        sprite1.fillRoundRect(201 - 6, 39, 34, 8, 2, green3);
    case 2:
        sprite1.fillRoundRect(201 - 6, 48, 34, 8, 2, green2);
    case 3:
        sprite1.fillRoundRect(201 - 6, 57, 34, 8, 2, green1);
    case 4:
        sprite1.fillRoundRect(201 - 6, 66, 34, 8, 2, TFT_GOLD);
    case 5:
        sprite1.fillRoundRect(201 - 6, 75, 34, 8, 2, TFT_MAGENTA);
    case 6:
        sprite1.fillRoundRect(201 - 6, 84, 34, 8, 2, TFT_DARKGREY);
    case 7:
        sprite1.fillRoundRect(201 - 6, 93, 34, 8, 2, TFT_RED);
        break;
    default:
        break;
    }
    /**
     * 以下 充放电 及 A  C  口 状态信息------------------------------------------------------------------------------------------
     *   0 放电  1 充电    0x4
     */
    // 状态
    sprite1.loadFont(KaiTi18); // 设置字体
    sprite1.setTextColor(TFT_WHITE);
    if (sys >= 1)
    {
        if (sys == 1)
        {
            sprite1.drawString("放电中", 210, 124);
            // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_RED); // 3
        }
        if (sys == 2)
        {
            sprite1.drawString("充电中", 210, 124);
            // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_RED); // 3
        }
    }
    else
    {
        // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_GREEN); // 3
        sprite1.drawString("空闲", 210, 124); // 态
    }

    //-----------------------------------------------------------------------
    // sprite1.drawLine(0, 108, 240, 108, TFT_PINK); // 下方分割直线  两点确定一条直线，两点坐标
    sprite1.drawFastHLine(0, 108, 240, TFT_PINK); // 下方分割横线  坐标  ,宽度
    // sprite1.drawFastVLine(88, 0, 135, TFT_PINK);  // 分割竖线  坐标  ,高度

    sprite1.pushSprite(0, 0); // 显示在led画布上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放字库
}

/**
 * @brief 主题2  极简主题
 * @param A_C 状态
 * @param bt_icon 蓝牙状态
 * @param sys_outinv 输入输出电压
 * @param battery_A 输入输出电流
 * @param sys_w 输入输出功率
 * @param ic_temp ic温度
 * @param bt_ntc 电池温度
 * @param bat_per 电池百分比
 */
void BackgroundTime2(uint8_t A_C, uint8_t bt_icon, float sys_outinv, float battery_A, float sys_w, float ic_temp,
                     float bat_ntc, uint8_t bat_per, uint16_t xunhuan)
{
    char draw_num[20];
    if (bat_per > 100)
        bat_per = 100;
    if (bat_per < 0)
        bat_per = 0;
    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    // sprite1.setTextDatum(TL_DATUM); // 字体左上角
    sprite1.setTextDatum(CC_DATUM);                        // 字体居中
    sprite1.setTextColor(TFT_BLACK);                       // 设置字体颜色
    sprite1.fillRoundRect(0, 0, 135, 135, 5, qianhui);     // 左背景
    sprite1.fillRoundRect(137, 0, 103, 135, 5, qianhui);   // 右背景
    sprite1.fillRoundRect(5, 29, 125, 33, 5, TFT_RED);     // 压背景
    sprite1.fillRoundRect(5, 64, 125, 33, 5, TFT_GOLD);    // 流背景
    sprite1.fillRoundRect(5, 101, 125, 33, 5, violef1);    // 功背景
    sprite1.fillRoundRect(140, 44, 27, 27, 5, TFT_RED);    // 右1
    sprite1.fillRoundRect(140, 75, 27, 27, 5, TFT_GOLD);   // 右2
    sprite1.fillRoundRect(140, 106, 27, 27, 5, TFT_GREEN); // 右3
    switch (A_C)
    {
    case 0:
        sprite1.fillRoundRect(1, 0, 25, 25, 5, lvse1);   // 上1
        sprite1.fillRoundRect(109, 0, 25, 25, 5, lvse5); // 上5
        break;
    case 1:
        sprite1.fillRoundRect(1, 0, 25, 25, 5, TFT_RED); // 上1
        sprite1.fillRoundRect(109, 0, 25, 25, 5, lvse5); // 上5
        break;
    case 4:
        sprite1.fillRoundRect(1, 0, 25, 25, 5, lvse1);     // 上1
        sprite1.fillRoundRect(109, 0, 25, 25, 5, TFT_RED); // 上5
        break;
    case 5:
        sprite1.fillRoundRect(1, 0, 25, 25, 5, TFT_RED);   // 上1
        sprite1.fillRoundRect(109, 0, 25, 25, 5, TFT_RED); // 上5
        break;
    default:
        break;
    }
    if (bat_per <= 5)
    {
        sprite1.drawPng(dianchiicon0, sizeof(dianchiicon0), 139, 1);
    }
    else if (bat_per > 5 && bat_per <= 25)
    {
        sprite1.drawPng(dianchiicon1, sizeof(dianchiicon1), 139, 1);
    }
    else if (bat_per > 25 && bat_per <= 50)
    {
        sprite1.drawPng(dianchiicon2, sizeof(dianchiicon2), 139, 1);
    }
    else if (bat_per > 50 && bat_per <= 75)
    {
        sprite1.drawPng(dianchiicon3, sizeof(dianchiicon3), 139, 1);
    }
    else if (bat_per > 75 && bat_per <= 100)
    {
        sprite1.drawPng(dianchiicon4, sizeof(dianchiicon4), 139, 1);
    }
    if (bt_icon)
    {
        sprite1.drawPng(lanya, sizeof(lanya), 210, 108);
    }
    sprite1.loadFont(huxiaobo);
    sprite1.drawString("池", 154, 58);  // 上5
    sprite1.drawString("系", 154, 89);  // 上5
    sprite1.drawString("循", 154, 120); // 上5
    sprite1.setTextColor(TFT_RED);      // 设置字体颜色
    sprintf(draw_num, "%.1f℃", ic_temp);
    sprite1.drawString(draw_num, 203, 58); // 上5
    sprite1.setTextColor(TFT_GOLD);        // 设置字体颜色
    sprintf(draw_num, "%.1f℃", bat_ntc);
    sprite1.drawString(draw_num, 203, 89); // 上5
    sprite1.setTextColor(TFT_GREEN);       // 设置字体颜色
    sprintf(draw_num, "%d", xunhuan);
    sprite1.drawString(draw_num, 185, 120); // 上5
    sprite1.setTextColor(TFT_BLACK);        // 设置字体颜色
    sprite1.loadFont(alibb);                // 字体设置
    sprite1.drawString("A", 14, 13);        // 上1
    sprite1.drawString("C", 122, 13);       // 上5
    sprite1.loadFont(alibb40);              // 字体设置
    sprite1.setTextColor(TFT_WHITE);        // 设置字体颜色
    sprintf(draw_num, "%.2fV", sys_outinv);
    sprite1.drawString(draw_num, 65, 45);
    sprintf(draw_num, "%.2fA", battery_A);
    sprite1.drawString(draw_num, 65, 80);
    sprintf(draw_num, "%.1fW", sys_w);
    sprite1.drawString(draw_num, 65, 117);

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

void BackgroundTime3(uint8_t week, float batv, float sysv, uint16_t workp, uint16_t ACstate,
                     float bata, float sysw, float batvolume, int xnum, uint16_t batper, uint8_t bt_icon)
{
    char draw_num[20];
    if (batper > 100)
        batper = 100;
    if (batper < 0)
        batper = 0;
    sprite1.createSprite(240, 135);                    // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);                     // 设置背景颜色
    sprite1.loadFont(huxiaobo);                        // 字体设置
    sprite1.setTextColor(TFT_WHITE);                   // 设置字体颜色
    sprite1.drawPng(dianchi3, sizeof(dianchi3), 0, 2); // 电池3
    sprite1.setTextDatum(CC_DATUM);                    // 字体居中
    sprite1.fillRoundRect(4, 6, 29, 17, 1, lv1);
    sprintf(draw_num, "%d", batper);
    sprite1.drawString(draw_num, 18, 15);
    sprite1.fillRoundRect(206, 0, 30, 30, 5, TFT_GREEN);  // 5
    sprite1.fillRoundRect(0, 38, 62, 25, 3, pink1);       // 中1
    sprite1.fillRoundRect(68, 38, 62, 25, 3, TFT_GOLD);   // 2
    sprite1.fillRoundRect(138, 38, 62, 25, 3, TFT_GREEN); // 3
    sprite1.fillRoundRect(206, 35, 30, 30, 5, TFT_CYAN);  // 4
    sprite1.fillRoundRect(0, 70, 62, 25, 3, TFT_RED);     // 5
    sprite1.fillRoundRect(68, 70, 75, 25, 3, violef1);    // 6
    sprite1.fillRoundRect(151, 70, 48, 25, 3, TFT_BROWN); // 7
    sprite1.fillRoundRect(206, 69, 30, 30, 5, TFT_GREEN); // 8
    sprite1.fillRoundRect(0, 104, 30, 30, 5, TFT_PINK);   // 下1
    sprite1.fillRoundRect(75, 104, 30, 30, 5, TFT_WHITE); // 2
    sprite1.drawFastVLine(200, 0, 30, TFT_WHITE);         // 线条竖
    sprite1.setTextColor(TFT_BLACK);                      // 设置字体颜色
    switch (week)
    {
    case 1:
        sprite1.drawString("一", 221, 15);
        break;
    case 2:
        sprite1.drawString("二", 221, 15);
        break;
    case 3:
        sprite1.drawString("三", 221, 15);
        break;
    case 4:
        sprite1.drawString("四", 221, 15);
        break;
    case 5:
        sprite1.drawString("五", 221, 15);
        break;
    case 6:
        sprite1.drawString("六", 221, 15);
        break;
    case 7:
        sprite1.drawString("日", 221, 15);
        break;
    default:
        sprite1.drawString("NO", 221, 15);
        break;
    }
    sprite1.drawFastHLine(0, 101, 240, TFT_WHITE);  // 线条横
    sprite1.drawFastVLine(151, 102, 33, TFT_WHITE); // 线条竖
    sprite1.loadFont(alibb);
    if (ACstate == 1 || ACstate == 4 || ACstate == 5)
    {
        if (ACstate == 1)
        {
            sprite1.drawString("A", 175, 83);
            // sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_RED); // 2
            // sprite1.fillRoundRect(47, 0, 30, 30, 5, green3);     // 上1
        }
        if (ACstate == 4)
        {
            sprite1.drawString("C", 175, 83);
            // sprite1.fillRoundRect(47, 0, 30, 30, 5, TFT_RED); // 上1
            // sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_YELLOW); // 2
        }
        if (ACstate == 5)
        {
            sprite1.drawString("A&C", 175, 83);
            // sprite1.fillRoundRect(47, 0, 30, 30, 5, TFT_RED); // 上1
            // sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_RED); // 2
        }
    }
    else
    {
        // sprite1.fillRoundRect(47, 0, 30, 30, 5, green3);     // 上1
        // sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_YELLOW); // 2
        sprite1.drawString("A/C", 175, 83);
    }
    // sprite1.drawPng(shujuxianc, sizeof(shujuxianc), 49, 2); // 数据线c
    // sprite1.drawPng(shujuxiana, sizeof(shujuxiana), 87, 2); // 数据线a
    sprintf(draw_num, "%d", xnum);
    sprite1.drawString(draw_num, 221, 84);
    if (workp >= 1)
    {
        if (workp == 1)
        {
            sprite1.drawString("OT", 221, 50);
        }
        if (workp == 2)
        {
            sprite1.drawString("IN", 221, 50);
        }
    }
    else
    {
        sprite1.drawString("NO", 221, 50); // 态
    }
    if (bt_icon)
    {
        sprite1.fillRoundRect(47, 0, 30, 30, 5, TFT_RED); // 4
    }
    else
        sprite1.fillRoundRect(47, 0, 30, 30, 5, pink1); // 4
    sprite1.drawPng(lanya3, sizeof(lanya3), 49, 2);     // 蓝牙
    sprite1.setTextDatum(TL_DATUM);                     // 字体左上角
    sprintf(draw_num, "%.1fV", sysv);
    sprite1.drawString(draw_num, 3, 41);
    sprintf(draw_num, "%.2fA", bata);
    sprite1.drawString(draw_num, 71, 41);
    sprintf(draw_num, "%.1fW", sysw);
    sprite1.drawString(draw_num, 140, 41);
    sprintf(draw_num, "%.2fV", batv);
    sprite1.drawString(draw_num, 3, 73);
    sprintf(draw_num, "%.1fwh", batvolume);
    sprite1.drawString(draw_num, 71, 73);
    sprintf(draw_num, "%d", xnum);
}

void BackgroundTime3_2(uint8_t month, uint8_t day, float battemp, float temp, uint8_t hour, uint8_t minute, uint8_t sec, uint8_t xdl)
{
    char draw_num[20];
    sprite1.setTextDatum(CC_DATUM);  // 字体居中
    sprite1.setTextColor(TFT_BLACK); // 设置字体颜色
    if (xdl == 1)
    {
        sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_RED); // 3
    }
    else
    {
        sprite1.fillRoundRect(85, 0, 30, 30, 5, TFT_GREEN); // 3
    }
    sprite1.drawPng(chongdian3, sizeof(chongdian3), 87, 2); // 充电
    sprite1.fillRoundRect(120, 0, 75, 30, 5, TFT_GOLD);
    sprintf(draw_num, "%d-%d", month, day);
    sprite1.drawString(draw_num, 155, 15);
    sprite1.setTextColor(TFT_WHITE);               // 设置字体颜色
    sprite1.drawPng(batt3, sizeof(batt3), 3, 108); // batt
    sprintf(draw_num, "%.0f℃", battemp);
    sprite1.drawString(draw_num, 52, 121);
    sprite1.drawPng(cput3, sizeof(cput3), 78, 108); // cput
    sprintf(draw_num, "%.0f℃", temp);
    sprite1.drawString(draw_num, 128, 121);
    sprintf(draw_num, "%d:%d:%d", hour, minute, sec);
    sprite1.drawString(draw_num, 197, 121);
    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

/**
 * @brief 主题4    闪极风格   --HUA
 *
 * @param batv 电池电压
 * @param sysv 系统电压
 * @param sys 充放电状态
 * @param ACstate 端口状态
 * @param battery_A 电池电流
 * @param sysw 系统功率
 * @param bat_per 电池百分比
 * @param bt_icon 蓝牙指示
 * @param ic_temp ic温度
 * @param bat_ntc 电池温度
 */
void BackgroundTime4(float battery_V, float sys_outinv, uint8_t sys, float battery_A, float sys_w, uint8_t bat_per,
                     uint8_t bt_icon, float ic_temp, float bat_ntc)
{
    char draw_num[20];
    float v = 0, a = 0; // 电压  // 电流
    // if (bat_per > 100)
    //     bat_per = 100;
    // if (bat_per < 0)
    //     bat_per = 0;

    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    sprite1.loadFont(KaiTi22);      // 字体设置
    sprite1.setTextDatum(TL_DATUM); // 字体左上角
    /**
     *  上 两行信息
     */
    // mcu芯片温度T ，布局固定部分  列1行1
    sprite1.fillRoundRect(0, 0, 24, 24, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_RED);
    sprite1.drawString("芯", 1, 1);

    // 电池温度NTC  ，布局固定部分  列3行1
    sprite1.fillRoundRect(90, 0, 24, 24, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("温", 91, 1);

    // 电池电压UC，布局固定部分
    sprite1.fillRoundRect(0, 27, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("池", 1, 28);
    sprite1.drawPng(imgbleimg24, sizeof(imgbleimg24), 90, 27); // 蓝牙图标

    /**
     * 上两行值写入
     */
    sprite1.loadFont(JianTi20); // 设置自定义字库 字体
    sprite1.setTextColor(TFT_WHITE);
    // 温度值写入sprite1 &lcd
    sprintf(draw_num, "%.1f℃", ic_temp); // 芯片 温度
    sprite1.drawString(draw_num, 27, 4);
    sprintf(draw_num, "%.2f℃", bat_ntc); // 电池温度NTC
    sprite1.drawString(draw_num, 117, 4);
    sprintf(draw_num, "%.2fV", battery_V); // 电池电压
    sprite1.drawString(draw_num, 27, 31);
    if (bt_icon) // 蓝牙
    {
        sprite1.drawString("ON", 117, 31);
    }
    else
    {
        sprite1.drawString("OFF", 117, 31);
    }

    /**
     *
     * 屏幕  左下半部分  输入输出实时信息
     */
    //-------------------------------------------------------------输入信息 ------  左下------------------------------------------

    // 输入信息   左
    sprite1.fillRoundRect(0, 58, 36, 36, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.loadFont(JianTi16);
    sprite1.drawString("IN", 10, 70); // 0,58
    // 充电输入 电压  上
    // sprite1.fillRoundRect(41, 59, 50, 16, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_WHITE);
    // sys  充电状态  ==2  充电
    if (sys == 2)       // 充电
    {                   // 充电状态 传值，，否则置零
        v = sys_outinv; // 充放电压
        a = battery_A;  // 充放电流
    }
    sprintf(draw_num, "%.2fV", v); // 输入电压  sys_outinv
    sprite1.drawString(draw_num, 43, 59);
    // 充电 输入 电流   下方
    sprite1.fillRoundRect(41, 77, 50, 16, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprintf(draw_num, "%.2fA", a); // 输入电流
    sprite1.drawString(draw_num, 45, 78);
    // 充电输入  功率   右
    // sprite1.fillRoundRect(96, 70, 80, 24, 5, TFT_WHITE); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_PINK);
    sprite1.loadFont(JianTi26);
    sprintf(draw_num, "%.2fw", v * a); // 输入功率 = v * a
    sprite1.drawString(draw_num, 96, 70);

    //-------------------------------------------------------------输出信息 ------  左下角------------------------------------------

    // 输出信息   左下
    sprite1.fillRoundRect(0, 99, 36, 36, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.loadFont(JianTi16);
    sprite1.drawString("OUT", 2, 111); //  0,99
    // 输出电压   上
    // sprite1.fillRoundRect(41, 100, 50, 16, 5, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.setTextColor(TFT_WHITE);
    v = 0;
    a = 0;
    if (sys == 1)       // 放电
    {                   // 放电状态 传值，，否则置零
        v = sys_outinv; // 充放电压
        a = battery_A;  // 充放电流
    }
    sprintf(draw_num, "%.2fV", v); // 输出电压 sys_outinv
    sprite1.drawString(draw_num, 43, 100);
    // 输出电流   下
    sprite1.fillRoundRect(41, 118, 50, 16, 5, TFT_YELLOW); // 实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprintf(draw_num, "%.2fA", a); // 输出电流
    sprite1.drawString(draw_num, 45, 119);

    sprite1.setTextColor(TFT_CYAN);
    sprite1.loadFont(JianTi26);
    sprintf(draw_num, "%.2fw", v * a); // 输出功率 = v * a
    sprite1.drawString(draw_num, 96, 111);

    sprite1.loadFont(KaiTi18); // 设置字体
    sprite1.setTextColor(TFT_WHITE);
    if (sys >= 1)
    {
        if (sys == 1)
        {
            sprite1.drawString("放电中", 183, 115);
            // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_RED); // 3
        }
        if (sys == 2)
        {
            sprite1.drawString("充电中", 183, 115);
            // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_RED); // 3
        }
    }
    else
    {
        // sprite1.fillRoundRect(123, 0, 30, 30, 5, TFT_GREEN); // 3
        sprite1.drawString("空闲", 192, 115); // 态
    }

    /**
     * @brief 系统充放电状态 AC口状态
     *
     * @param H_value  系统状态 8充电 4 放电
     * @param L_value   1/5 A口状态  4/5 C口状态
     * @param battery_A 充电或者输出电流
     */

    // if (ACstate == 1 || ACstate == 5)
    //     sprite1.drawString("ON", 183, 115);
    // else
    //     sprite1.drawString("OFF", 183, 115); // A 状态
    // sprite1.setTextColor(TFT_PINK);
    // if (ACstate == 4 || ACstate == 5)
    //     sprite1.drawString("ON", 183, 115);
    // else
    //     sprite1.drawString("OFF", 183, 115); // C 状态

    // 电量百分比   右上角
    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_WHITE);
    sprintf(draw_num, "%d", bat_per);
    // if (bat_per < 10)
    // {
    //     sprite1.setTextColor(TFT_RED);
    //     sprite1.drawString(draw_num, 200, 4);
    // }
    // else
    // {
    //     sprite1.drawString(draw_num, 195, 4);
    // }

    if (bat_per < 10)
    {
        sprite1.setTextColor(TFT_RED);
        sprite1.drawString(draw_num, 203, 4);
    }
    else if (bat_per == 100)
    {
        sprite1.drawString(draw_num, 189, 4);
    }
    else
    {
        sprite1.drawString(draw_num, 195, 4);
    }

    /**
     * 大电池图标
     */
    sprite1.fillRoundRect(200, 32, 20, 5, 2, TFT_BLUE);   // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形   //正极头  电池帽
    sprite1.drawRoundRect(190, 35, 40, 65, 10, TFT_BLUE); // 位置，宽高，圆角半径 ，背景色    ////空心圆角矩形  //双框
    sprite1.drawRoundRect(191, 36, 38, 63, 10, TFT_BLUE); // 位置，宽高，圆角半径 ，背景色    ////空心圆角矩形  //双框2

    // 大电池电量图标，电量 电格剩余量
    if (bat_per >= 0 && bat_per <= 10)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }
    else if (bat_per > 10 && bat_per <= 25)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);    // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }
    else if (bat_per > 25 && bat_per <= 40)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);         // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW);      // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 74, 30, 5, 1, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }
    else if (bat_per > 40 && bat_per <= 55)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);         // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW);      // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 74, 30, 5, 1, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 65, 30, 5, 1, TFT_GREEN);       // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }
    else if (bat_per > 55 && bat_per <= 70)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);         // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW);      // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 74, 30, 5, 1, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 65, 30, 5, 1, TFT_GREEN);       // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 56, 30, 5, 1, TFT_CYAN);        // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }
    else if (bat_per > 70 && bat_per <= 85)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);         // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW);      // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 74, 30, 5, 1, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 65, 30, 5, 1, TFT_GREEN);       // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 56, 30, 5, 1, TFT_CYAN);        // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 47, 30, 5, 1, TFT_WHITE);       // 位置，宽高，圆角半径 ，背景色   ///实心，圆角矩形
    }
    else if (bat_per > 85 && bat_per <= 100)
    {
        sprite1.fillRoundRect(195, 92, 30, 5, 1, TFT_RED);         // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 83, 30, 5, 1, TFT_YELLOW);      // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 74, 30, 5, 1, TFT_GREENYELLOW); // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 65, 30, 5, 1, TFT_GREEN);       // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 56, 30, 5, 1, TFT_CYAN);        // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
        sprite1.fillRoundRect(195, 47, 30, 5, 1, TFT_WHITE);       // 位置，宽高，圆角半径 ，背景色   ///实心，圆角矩形
        sprite1.fillRoundRect(195, 38, 30, 5, 1, TFT_GREEN);       // 位置，宽高，圆角半径 ，背景色   ////实心，圆角矩形
    }

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

/**
 * @brief  主题5    手机风格   --HUA
 *
 * @param batv 电池电压
 * @param sysv 系统电压
 * @param sys 充放电状态
 * @param ACstate 端口状态
 * @param battery_A 电池电流
 * @param sysw 系统功率
 * @param bat_per 电池百分比
 * @param bt_icon 蓝牙指示
 * @param ic_temp ic温度
 * @param bat_ntc 电池温度
 * @param A_C  A C 口状态
 */
void BackgroundTime5(float battery_V, float sys_outinv, uint8_t sys, uint8_t A_C, float battery_A, float sys_w, uint8_t bat_per, float bat_m, uint8_t bt_icon,
                     float ic_temp, float bat_ntc, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t sec, uint8_t week)
{
    char draw_num[20];
    // float v = 0, a = 0; // 电压  // 电流
    if (bat_per > 100)
        bat_per = 100;
    if (bat_per < 0)
        bat_per = 0;

    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    // sprite1.loadFont(KaiTi22);      // 字体设置
    sprite1.setTextDatum(TL_DATUM); // 字体左上角显示
    /**
     * 上方 状态栏 ： 时分秒   蓝牙   闪电图标   电量  百分比
     */
    sprite1.setTextColor(TFT_WHITE);
    sprite1.loadFont(JianTi26);
    sprintf(draw_num, "%02d:%02d:%02d", hour, minute, sec); // 时间
    sprite1.drawString(draw_num, 0, 0);

    sprite1.setTextColor(TFT_GREEN);
    sprite1.loadFont(JianTi20_datetime);
    String weeks = "";
    switch (week) // 获取的为数字信息
    {
    case 1:
        weeks = "周一";
        break;
    case 2:
        weeks = "周二";
        break;
    case 3:
        weeks = "周三";
        break;
    case 4:
        weeks = "周四";
        break;
    case 5:
        weeks = "周五";
        break;
    case 6:
        weeks = "周六";
        break;
    case 7:
        weeks = "周日";
        break;
    default:
        weeks = "NO";
        break;
    }
    sprintf(draw_num, "%d月%d日 %s", month, day, weeks); // 日期 星期
    sprite1.drawString(draw_num, 0, 24);

    if (bt_icon) // 蓝牙
    {
        // sprite1.pushImage(110, 0, 20, 20, imgbleimg20); // 蓝牙图标
        sprite1.drawPng(imgbleimg20, sizeof(imgbleimg20), 110, 1); // 蓝牙图标
    }
    // sprite1.drawPng(imgbleimg20, sizeof(imgbleimg20), 110, 1); // 蓝牙图标

    /**
     * 电量图标 外框
     */
    // 155--193  电量图标长度  高16像素   对齐20像素
    sprite1.fillRoundRect(155, 8, 4, 6, 2, TFT_WHITE);   // 位置，宽高，圆角半径 ，背景色   // 实心，圆角矩形   //正极头  电池帽
    sprite1.drawRoundRect(158, 3, 35, 16, 3, TFT_WHITE); // 位置，宽高，圆角半径 ，背景色   // 空心，圆角矩形   //框

    if (sys == 2) // 充电
    {
        color = TFT_GREEN;
        colorRed = TFT_GREEN;
        if (sys_w > 15 || sys_outinv > 8)
        {
            sprite1.drawPng(imglvshandian20, sizeof(imglvshandian20), 135, 0); // 快充闪电图标
        }
        else
        {
            sprite1.drawPng(imgbaishandian20, sizeof(imgbaishandian20), 139, 1); // 5v充电闪电图标
        }
    }
    else
    { // else 可不要，但效果不好，延迟
        color = TFT_WHITE;
        colorRed = TFT_RED;
    }
    if (bat_per >= 0 && bat_per < 15)
    {
        sprite1.fillRoundRect(160 + 28, 5, 31 - 28, 12, 1, colorRed); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 15 && bat_per < 30)
    {
        sprite1.fillRoundRect(160 + 23, 5, 31 - 23, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 30 && bat_per < 45)
    {
        sprite1.fillRoundRect(160 + 18, 5, 31 - 18, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 45 && bat_per < 60)
    {
        sprite1.fillRoundRect(160 + 13, 5, 31 - 13, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 60 && bat_per < 75)
    {
        sprite1.fillRoundRect(160 + 8, 5, 31 - 8, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 75 && bat_per < 90)
    {
        sprite1.fillRoundRect(160 + 3, 5, 31 - 3, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    if (bat_per >= 90 && bat_per <= 100)
    {
        sprite1.fillRoundRect(160, 5, 31, 12, 1, color); // 位置，宽高，圆角半径 ，背景色
    }
    // /**
    //  *  百分比 数字显示
    //  */
    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_WHITE);
    // 电量百分比   右上角
    sprintf(draw_num, "%d", bat_per); // 传值
    if (bat_per < 10)
    { // 小于10   一个字符右移一点   改为红色字体
        sprite1.setTextColor(TFT_RED);
        sprite1.drawString(draw_num, 205, 0); // 电量百分比红色显示
    }
    else
    {
        sprite1.drawString(draw_num, 195, 0); // 电量百分比显示
    }

    // /**
    //  *  系统信息  温度  功率
    //  */
    sprite1.fillRoundRect(0, 45, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.loadFont(KaiTi22);
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("芯", 1, 45 + 1);

    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_CYAN);
    sprintf(draw_num, "%.2f℃", ic_temp);      // 芯片 温度
    sprite1.drawString(draw_num, 27, 48 - 2); // 芯片 位置
    // 功率
    sprite1.fillRoundRect(0, 45 + 27, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.loadFont(KaiTi22);
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("功", 1, 45 + 27 + 1);

    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_CYAN);
    sprintf(draw_num, "%.2fw", sys_outinv * battery_A); // 功率 = sys_outinv * battery_A
    sprite1.drawString(draw_num, 27, 48 + 25);

    /**
     *  电池信息  温度  容量
     */
    sprite1.fillRoundRect(120, 45, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.loadFont(KaiTi22);
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("温", 120 + 1, 45 + 1);

    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_PINK);                 // 粉色
    sprintf(draw_num, "%.2f℃", bat_ntc);            // 电池温度NTC
    sprite1.drawString(draw_num, 120 + 27, 48 - 2); // 电池温度位置
    // 电池容量
    sprite1.fillRoundRect(120, 45 + 27, 24, 24, 5, TFT_YELLOW); // 位置，宽高，圆角半径 ，背景色  ////实心圆角矩形
    sprite1.loadFont(KaiTi22);
    sprite1.setTextColor(TFT_BLACK);
    sprite1.drawString("容", 120 + 1, 45 + 27 + 1);

    sprite1.loadFont(JianTi26);
    sprite1.setTextColor(TFT_CYAN);
    sprintf(draw_num, "%.1fwH", bat_m); // 容量
    sprite1.drawString(draw_num, 120 + 27, 48 + 25);

    // //-------------------------------下方电压 电流信息-----------------------------------------------------------
    // 芯片画图
    sprite1.fillRoundRect(0, 75 + 24, 36, 36, 5, TFT_PINK); // 位置，宽高，圆角半径 ，背景色  // 实心圆角矩形
    sprite1.setTextColor(TFT_BLACK);
    sprite1.loadFont(KaiTi22);
    sprite1.drawString("芯", 6, 75 + 8 + 24);
    // 电池画图
    sprite1.fillRoundRect(120, 75 + 24, 36, 36, 5, TFT_CYAN); // 位置，宽高，圆角半径 ，背景色  // 实心圆角矩形
    sprite1.drawString("池", 120 + 7, 75 + 8 + 24);
    /**
     *  系统 电压  电流
     */
    sprite1.setTextColor(TFT_WHITE);
    sprite1.loadFont(JianTi20);
    sprintf(draw_num, "%.2fV", sys_outinv); // 充放 电压
    sprite1.drawString(draw_num, 43, 73 + 24);

    sprite1.setTextColor(TFT_BLACK);
    sprite1.fillRoundRect(41, 93 + 24, 58, 17, 5, TFT_CYAN); // 实心圆角矩形
    sprintf(draw_num, "%.2fA", battery_A);                   // 系统 电流
    sprite1.drawString(draw_num, 45, 93 + 24);
    /**
     *  电池 电压  电流
     */
    sprite1.setTextColor(TFT_YELLOW);
    sprite1.loadFont(JianTi20);
    sprintf(draw_num, "%.2fV", battery_V); // 电池电压
    sprite1.drawString(draw_num, 120 + 43, 73 + 24);

    sprite1.setTextColor(TFT_BLACK);
    sprite1.fillRoundRect(120 + 41, 93 + 24, 58, 17, 5, TFT_PINK);  // 实心圆角矩形
    sprintf(draw_num, "%.2fA", sys_outinv * battery_A / battery_V); // 电池电流 ：   功率 / 电池电压
    sprite1.drawString(draw_num, 120 + 45, 93 + 24);

    /**
     * A C 端口
     */
    sprite1.loadFont(JianTi20);
    sprite1.setTextColor(TFT_BLACK);
    // if A_C == 5   A C 全开
    if (A_C == 1 || A_C == 5) // A口开
    {
        sprite1.fillRoundRect(120, 24, 18, 18, 3, TFT_PINK); // 位置，宽高，半径  // 实心 圆角矩形
        sprite1.drawString("A", 122, 25);
    }
    if (A_C == 4 || A_C == 5) // C口开
    {
        sprite1.fillRoundRect(145, 24, 18, 18, 3, TFT_CYAN); // 位置，宽高，半径  // 实心 圆角矩形
        sprite1.drawString("C", 147, 25);
    }

    //---------------------------------------------//////////////////////--------可不要--------------------------
    sprite1.loadFont(KaiTi18); //
    sprite1.setTextColor(TFT_PINK);
    if (sys >= 1)
    {
        if (sys == 1)
        {
            sprite1.drawString("放电中", 183, 24);
        }
        if (sys == 2)
        {
            sprite1.drawString("充电中", 183, 24);
        }
    }
    else
    {
        sprite1.drawString("空闲", 192, 24); // 态
    }
    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}
/**
 * 眼
 */
void Backgroundyan(uint16_t pngindex)
{
    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    switch (pngindex)
    {
    case 1:
        sprite1.drawPng(yan1, sizeof(yan1), 0, 0);
        break;
    case 2:
        sprite1.drawPng(yan2, sizeof(yan2), 0, 0);
        break;
    case 3:
        sprite1.drawPng(yan3, sizeof(yan3), 0, 0);
        break;
    case 4:
        sprite1.drawPng(yan4, sizeof(yan4), 0, 0);
        break;
    case 5:
        sprite1.drawPng(yan5, sizeof(yan5), 0, 0);
        break;
    case 6:
        sprite1.drawPng(yan6, sizeof(yan6), 0, 0);
        break;
    case 7:
        sprite1.drawPng(yan7, sizeof(yan7), 0, 0);
        break;
    case 8:
        sprite1.drawPng(yan8, sizeof(yan8), 0, 0);
        break;
    case 9:
        sprite1.drawPng(yan9, sizeof(yan9), 0, 0);
        break;
    case 10:
        sprite1.drawPng(yan10, sizeof(yan10), 0, 0);
        break;
    case 11:
        sprite1.drawPng(yan11, sizeof(yan11), 0, 0);
        break;
    case 12:
        sprite1.drawPng(yan12, sizeof(yan12), 0, 0);
        break;
    case 13:
        sprite1.drawPng(yan13, sizeof(yan13), 0, 0);
        break;
    case 14:
        sprite1.drawPng(yan14, sizeof(yan14), 0, 0);
        break;
    case 15:
        sprite1.drawPng(yan15, sizeof(yan15), 0, 0);
        break;
    case 16:
        sprite1.drawPng(yan16, sizeof(yan16), 0, 0);
        break;
    case 17:
        sprite1.drawPng(yan17, sizeof(yan17), 0, 0);
        break;
    case 18:
        sprite1.drawPng(yan18, sizeof(yan18), 0, 0);
        break;
    case 19:
        sprite1.drawPng(yan19, sizeof(yan19), 0, 0);
        break;
    default:
        break;
    }
    sprite1.pushSprite(0, 0); // 显示在画布1上
    // sprite1.releasePngMemory();
    sprite1.deleteSprite(); // 删除精灵
}

/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

// 非正当获得设备采取措施
void lost_Page()
{
    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    sprite1.loadFont(JianTi26);     // 字体设置
    sprite1.setTextColor(TFT_RED);  // 字体颜色
    // sprite1.setTextDatum(TL_DATUM);  // 字体左上角
    sprite1.setTextDatum(CC_DATUM); // 字体居中

    sprite1.drawString("Wrongful acquisition", 120, 50);
    sprite1.drawString("The AC is closed.", 120, 80);

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

// 等待连接WiFi
void WiFi_Page()
{
    sprite1.createSprite(240, 135);  // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);   // 设置背景颜色
    sprite1.loadFont(JianTi26);      // 字体设置
    sprite1.setTextColor(TFT_WHITE); // 字体颜色
    // sprite1.setTextDatum(TL_DATUM);  // 字体左上角
    sprite1.setTextDatum(CC_DATUM); // 字体居中

    sprite1.drawString("SSID: 2024", 120, 50);
    sprite1.drawString("PWd: 12345678", 120, 80);

    sprite1.loadFont(JianTi20); // 字体设置
    sprite1.drawString("Click to exit OTA", 120, 120);

    // sprite1.setTextDatum(TL_DATUM); // 字体左上角
    sprite1.drawString("Connecting to WiFi...", 120, 10);

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

// OTA 配网提示页面   使用此方法请弃用 WiFi_Page();
void ota_AP()
{
    sprite1.createSprite(240, 135);  // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);   // 设置背景颜色
    sprite1.loadFont(JianTi26);      // 字体设置
    sprite1.setTextColor(TFT_WHITE); // 字体颜色
    // sprite1.setTextDatum(TL_DATUM);  // 字体左上角
    sprite1.setTextDatum(CC_DATUM); // 字体居中

    sprite1.drawString("AP: AP_Page", 120, 50);
    sprite1.drawString("IP: 192.168.4.1", 120, 80);

    sprite1.loadFont(JianTi20); // 字体设置
    sprite1.drawString("Click to exit OTA", 120, 120);

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}

// 更新进度
void ota_Page(int a, int b)
{
    char draw_num[20];

    sprite1.createSprite(240, 135); // 创建画布大小
    sprite1.fillScreen(TFT_BLACK);  // 设置背景颜色
    sprite1.setTextColor(TFT_GREEN);
    // sprite1.loadFont(KaiTi22);
    // sprite1.setTextDatum(TL_DATUM); // 字体左上角
    sprite1.setTextDatum(CC_DATUM); // 字体居中

    sprite1.loadFont(JianTi26);
    sprite1.drawString("Updating...", 120, 50);
    // sprintf(draw_num, "%.1f%%", cur * 100.0 / total); // 100.0%
    sprintf(draw_num, "%.f %%", a * 100.0 / b); // 无小数 100 %
    sprite1.drawString(draw_num, 120, 80);

    sprite1.loadFont(JianTi20); // 字体设置
    sprite1.drawString("Click to exit OTA", 120, 120);

    sprite1.pushSprite(0, 0); // 显示在画布1上
    sprite1.deleteSprite();   // 删除精灵
    sprite1.unloadFont();     // 释放加载字体
}
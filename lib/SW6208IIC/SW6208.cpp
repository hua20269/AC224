#include <SW6208.h>

/**
 * @brief sw6208初始化
 * 初始化事件：
 *
 */

void SW6208_init()
{
    I2C_Write(SW6208_address, 0x03, 0xf);
    I2C_Write(SW6208_address, 0x30, 0x4);
}

// /**
//  * @brief 实现ln运算
//  *
//  * @param a
//  * @return double
//  */
// double myln(double a)
// {
//     int N = 15; // 我们取了前15+1项来估算
//     int k, nk;
//     double x, xx, y;
//     x = (a - 1) / (a + 1);
//     xx = x * x;
//     nk = 2 * N + 1;
//     y = 1.0 / nk;
//     for (k = N; k > 0; k--)
//     {
//         nk = nk - 2;
//         y = 1.0 / nk + xx * y;
//     }
//     return 2.0 * x * y;
// }

/**
 * @brief sw的adc数据
 * 0x13是adc高八位数据
 * 0x14是adc低四位数据
 * @return uint16_t 返回的adc值  在其他函数中进行处理
 */
uint16_t batteryADC()
{
    uint8_t r_buffer[2];
    int ADCvalue;
    r_buffer[0] = I2C_Read(SW6208_address, 0x13);
    Serial.println("ADC");
    Serial.println(r_buffer[0], HEX);
    delay(10);
    r_buffer[1] = I2C_Read(SW6208_address, 0x14);
    Serial.println(r_buffer[1], HEX);
    ADCvalue = (r_buffer[0] << 4) + r_buffer[1];
    Serial.println(ADCvalue);
    return ADCvalue;
}

/**
 * @brief 电池电压的读取和计算  电压=adc*0.0012v
 */
float batteryV()
{
    if (I2C_Write(SW6208_address, 0x12, 0x0) == 0)
    {
        Serial.println("V-ok!");
    }
    else
    {
        Serial.print("V-NO!");
    }
    float v_value;
    v_value = batteryADC() * 0.0012;
    Serial.println(v_value);
    return v_value;
}

/**
 * @brief 输入输出电压获取
 *
 * @return float 电压
 */
float battery_outinV()
{
    if (I2C_Write(SW6208_address, 0x12, 0x1) == 0)
    {
        Serial.print("wok!");
    }
    else
    {
        Serial.print("wNO!");
    }
    float a_value;
    a_value = batteryADC() * 0.004;
    Serial.println(a_value, 4);
    return a_value;
}

/**
 * @brief ic温度获取
 *
 * @return float ic温度
 */
float battery_ictemp()
{
    if (I2C_Write(SW6208_address, 0x12, 0x2) == 0)
    {
        Serial.print("wok!");
    }
    else
    {
        Serial.print("wNO!");
    }
    float a_value;
    a_value = (batteryADC() - 1839) / 6.82;
    Serial.println(a_value, 4);
    return a_value;
}

/**
 * @brief ntc电阻转换温度
 *
 * @return float ntc温度
 */
float battery_ntcV()
{
    uint8_t r_buffer;
    float temp;
    float N1, N2, N3, N4;
    r_buffer = I2C_Read(SW6208_address, 0x48);
    Serial.println("ADC");
    Serial.println(r_buffer, HEX);
    r_buffer = r_buffer & 0x1;
    Serial.println(r_buffer);
    if (I2C_Write(SW6208_address, 0x12, 0x3) == 0)
    {
        Serial.print("wok!");
    }
    else
    {
        Serial.print("wNO!");
    }
    if (r_buffer == 0)
    {
        temp = batteryADC() * 0.0022 / 0.00008;
    }
    if (r_buffer == 1)
    {
        temp = batteryADC() * 0.0011 / 0.00004;
    }
    Serial.println(temp, 4);
    temp = log(temp / R25); // ln(Rt/Rp)：Rt:当前温度下的阻值 R25:25℃下的NTC阻值(K)
    temp /= BX;             // ln(Rt/Rp)/BX 注：BX值一般为NTC的规格，本次用的NTC型号是3950
    temp += (1 / T25);
    temp = 1 / temp;
    temp -= 273.15;
    temp += 0.5;
    Serial.println(temp, 4);
    return temp;
}

/**
 * @brief 充电电流获取
 *
 * @return float 返回充电电流值
 */

float battery_inA()
{
    if (I2C_Write(SW6208_address, 0x12, 0x4) == 0)
    {
        Serial.print("wok!");
    }
    else
    {
        Serial.print("wNO!");
    }
    float a_value;
    a_value = batteryADC() * 0.002273;
    Serial.println(a_value, 4);
    return a_value * 1.1;
}

/**
 * @brief 输出电流的获取
 *
 * @return float 返回输出电流  小数形式
 */
float battery_outA()
{
    if (I2C_Write(SW6208_address, 0x12, 0x5) == 0)
    {
        Serial.print("wok!");
    }
    else
    {
        Serial.print("wNO!");
    }
    float a_value;
    a_value = batteryADC() * 0.002273;
    Serial.println(a_value, 4);
    return a_value * 1.1;
}

/**
 * @brief 电量获取
 *
 * @return float 返回电量值  小数形式
 */
float battery_volume()
{
    uint8_t r_buffer[2];
    float a_value;
    int ADCvalue;
    r_buffer[0] = I2C_Read(SW6208_address, 0x73);
    Serial.println("ADC");
    Serial.println(r_buffer[0], HEX);
    delay(10);
    r_buffer[1] = I2C_Read(SW6208_address, 0x74);
    Serial.println(r_buffer[1], HEX);
    ADCvalue = (r_buffer[1] << 8) + r_buffer[0];
    Serial.println(ADCvalue);
    a_value = ADCvalue * 0.1695;
    return a_value;
}

/**
 * @brief 电量百分比获取
 *
 * @return uint16_t 返回电量百分比数值
 */
uint16_t battery_per()
{
    uint8_t r_buffer;
    uint16_t a_value;
    r_buffer = I2C_Read(SW6208_address, 0x7E);
    Serial.println("ADC");
    Serial.println(r_buffer, HEX);
    a_value = r_buffer;
    Serial.println(a_value);
    return a_value;

    // return 5;
}

/**
 * @brief 系统充放电状态 AC口状态
 *
 * @param H_value  系统状态 2充电 1 放电
 * @param L_value   1 A口状态  4C口状态    5 AC
 * @param battery_A 充电或者输出电流
 *
 */
void sysstate(uint8_t *H_value, uint8_t *L_value, float *battery_A)
{
    uint8_t svalue;
    svalue = I2C_Read(SW6208_address, 0x0C);
    Serial.println(svalue, HEX);
    *H_value = svalue >> 6;
    Serial.println(*H_value);
    *L_value = svalue & 0xF;
    Serial.println(*L_value);
    if (*H_value == 1) // 放电
    {
        *battery_A = battery_outA();
    }
    if (*H_value == 2) // 充电
    {
        *battery_A = battery_inA();
    }
    else
        *battery_A = battery_outA();
}

/**
 * @brief REG 0x0F: 快充协议指示
 * bit:7 PD版本指示 0: PD2.0 1: PD3.0   注意此指示只在PD沟通后有效
 * bit:6-4 sink快充协议指示 0: 非快充  * 1: PD sink 2: / 3: HV sink 4: AFC sink 5: FCP sink 6: SCP sink 7: PE1.1 sink
 * bit:3-0 source快充协议指示 0: 非快充 1: PD source 2: PPS source 3: QC2.0 source 4: QC3.0 source 5: FCP source 6: PE2.0 /1.1 source 7: SFCP source 8: AFC source 9: SCP source 10-15: reserved
 * @return uint8_t
 */
uint8_t Protocol()
{
    I2C_Read(SW6208_address, 0x0F);
    Serial.println("Protocol------------------------------------");
    return I2C_Read(SW6208_address, 0x0F);
}

/**
 * @brief 开启或退出小电流   0X10    4  写1
 */
void kqxdl()
{
    // uint8_t svalue;
    // svalue = I2C_Read(SW6208_address, 0x2E);
    // svalue = svalue & 0X10;
    // if (
    I2C_Write(SW6208_address, 0x2E, 0X10) == 0; // 写1开启或退出小电流
    // {
    //     Serial.print("kaiqichenggong!");
    // }
    // else
    // {
    //     Serial.print("kaiqishibai");
    // }
}
/**
 * @brief 0x33: 小电流充电配置
 */
void I2Csmall_A_ON()
{
    I2C_Write(SW6208_address, 0x33, 0X3) == 0; // 小电流使能
}
void I2Csmall_A_OFF()
{
    I2C_Write(SW6208_address, 0x33, 0X2) == 0; // 小电流不使能
}
/**
 * @brief 小电流状态
 *
 * @return uint8_t   1/0
 */
uint8_t xdlzt()
{
    uint8_t svalue;
    svalue = I2C_Read(SW6208_address, 0x2E);
    svalue = svalue & 0X1;
    return svalue;
}
/**
 * @brief A1口触发插入事件
 *
 */
void A1shijian()
{
    uint8_t svalue;
    svalue = I2C_Read(SW6208_address, 0x19);
    svalue = svalue & 0X1;
    if (I2C_Write(SW6208_address, 0x19, svalue) == 0)
    {
        Serial.print("A2yes");
    }
    else
    {
        Serial.print("A2NO");
    }
}
// 本设备没有A2物理接口，给小电流开关调用
void A2_ON() // A2口触发插入事件
{
    I2C_Write(SW6208_address, 0x19, 0X4) == 0;
}

void A2_OFF() // A2口触发拔出事件
{
    I2C_Write(SW6208_address, 0x19, 0X8) == 0;
}

// 关闭所有输出口
void AC_OFF()
{
    I2C_Write(SW6208_address, 0x18, 0X10) == 0;
    Serial.println("------6208--------AC_OFF--------");
}

/**
 * @brief 设置NTC温度上限保护50℃ 改为 60℃
 *
 */
void NTCLimit()
{
    I2C_Write(SW6208_address, 0x47, 0X0);  // Boost NTC温度自适应功能使能   使能    高于自适应温度门限后，ntc温度每上升1度，vout下降800mv   (温度达不到，自适应阈值100°)基本没用
    I2C_Write(SW6208_address, 0x48, 0XE0); // Charger NTC高温保护门限 60°       0xC0  55°    默认50°
}
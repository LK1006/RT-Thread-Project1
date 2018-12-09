#ifndef __OLED_Display_H__
#define __OLED_Display_H__

//extern void display_oled(uint8_t line);
extern void initMenu(void);
extern void variable_display(void);
extern void oled_gyro(void);
extern void OLED_Control(void);
void OLED(void);
void oled_pid_MPU(void);
void OLED_Fill(unsigned char bmp_data);
void OLED_PrintU16(unsigned char x,unsigned char y,unsigned int num);
void Display_switch(void);
void OLED_switch(void);
void  key_select(void);

//typedef struct pid
//{
//    uint16_t P;
//    uint16_t I;
//    uint16_t D;
//    uint16_t L;
//}PID;
//extern PID PID_Motor;
//extern void oled_pid(PID *p);

/******* 参数 *******/
typedef struct
{
    float kp;
    float ki;
    float kd;                                //6.9   2.7   0.01
    float core_kp;
    float core_ki;
    float core_kd;
    float angle_expectation;
    float Dir_Kp;
    float Dir_Kd;
    float Speed_expectation;
    float Speed_Kp;
    float Speed_Ki;
    
}Parameter;
extern Parameter Parameter_all;

//void Paramter_modify(Parameter* Point,float num);

/******* 参数 *******/

//typedef struct sensor
//{
//    uint16 sensor_value[2];
//}Sensor;
//extern Sensor Sensor_L;
extern void oled_sensor(void);

extern float a;
extern uint8_t line_oled;

extern uint8_t ON_sensor;
extern uint8_t ON_gyro;
extern uint8_t ON_pid_MPU;
extern uint8_t ON_pid_Dir;
extern uint8_t ON_pid_Speed;

extern uint8_t key_up;
extern uint8_t key_down;
extern uint8_t key_r;
extern uint8_t key_l;
extern uint8_t key_a;
extern uint8_t KEY;


#endif


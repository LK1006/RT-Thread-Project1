#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "led.h"
#include "OLED_Display.h"
#include "morphdigit.h"
#include "ledmatrix.h"
/**************************************
*           按键中断参数                *
*****************************************/  
uint16_t value[2];



uint8_t ON_sensor;
uint8_t ON_gyro;
uint8_t ON_pid_MPU;
uint8_t ON_pid_Dir;
uint8_t ON_pid_Speed;
uint8_t ON_about;

uint8_t key_up;
uint8_t key_down;
uint8_t key_r;
uint8_t key_l;
uint8_t key_a;
uint8_t KEY;

uint8_t itemCounts = 0;


uint8_t modify_MPU;
uint8_t modify_Dir;
uint8_t modify_Speed;

float NUM_Var = 0.2f;

uint16_t count;
uint16_t var;
uint16_t var1;

extern uint8_t p;
extern uint8_t i;
extern uint8_t d;

extern float kp;
extern float kd;
extern float core_kp;
extern float core_ki;
extern float angle_expectation;

uint8_t line_oled;
extern uint8_t Menu_ON;
extern int Lightness_Value;
extern uint8_t Lightness_flag;
extern uint8_t Lightness_Change_finish;
uint16_t Lightness_Value_Change;
uint8_t Lightness_ON;
uint16_t key_a_count;
int sensor;

int x;

 
typedef struct menu//定义一个菜单
{
   uint8_t range_from;//当前显示的项开始及结束序号
   uint8_t range_to; 
   uint8_t itemCount;//项目总数
   uint8_t selected;//当前选择项
   uint8_t *menuItems[17];//菜单项目
   struct menu **subMenus;//子菜单
   struct menu *parent;//上级菜单 ,如果是顶级则为null
   void (**func)();//选择相应项按确定键后执行的函数
}Menu;

  Menu MainMenu={0,3,4,0,
  {
     "1.Color-Num ",
     "2.Color-Dot ",
     "3.System    ",
     "4.About     "
  }
  
  };
   
   Menu searchMenu={0,3,4,0,
   {
      "* ON/OFF * ",
      "MPU_OFF  ",
      "Dir_OFF  ",
      "Speed_OFF"     
      //"5.设置5 ",
     // "6.设置6 "
   }
   };
   
	 Menu colorMenu={0,3,8,0,
   {
      "* COLOR * ",
      "RED       ",
      "GREEN     ",
      "BLUE      ",    
      "YELLOW    ",
      "PURPLE    ",
		  "CYAN      ",
		  "WHITE     "
   }
   };
	 
	 Menu dotMenu={0,3,4,0,
   {
		 "* COLOR-: *",
      "GREEN     ",
      "BLUE      ",          
      "CYAN      "		  
   }
   };
	 
	 Menu sysMenu={0,3,5,0,
   {
		 "* System * ",
      "Light/ON  ",
      "Colorful  ",          
      "Lightness   >",
			"Background  >"
   }
   };
	 
	 Menu backMenu={0,3,4,0,
   {
		 "*Background*",
      "Mode_1  " ,
			"NULL    " ,
		  "NULL    " 
   }
   };
	 
	 
   Menu NUMSystem={0,3,6,0,
   {
      "   +0.2  ",
      "   +0.1  ",
      "   +1.0  ",
      "   +2.0 ",
      "   +5.0 ",
      "   +10.0 "
   } 
   };
   
   Menu PIDMenu={0,7,5,0,
   {
     "angle:",
     "shell_kp:",
     "shell_kd:",
     "core_kp :",
     "core_ki :"     
   }
   };
   
   Menu DirMenu={0,7,2,0,
   {  
     "Dir_Kp:",
     "Dir_Kd:"
   }
   };
   
   Menu SpeedMenu={0,7,3,0,
   {
      "AimSpeed:",
      "Speed_Kp:",
      "Speed_Ki:"
   }
   };
     
    
   
   Menu ParameterMenu={0,3,4,0,
   {
      "MPU_PID  ",
      "Dir_PID  ",
      "Speed_PID",
      "* NUM+/- "
   }
   };
   
//   Menu sensorMenu;
//   Menu gyroMenu;
   Menu lightnessMenu;
	 Menu aboutMenu;
  
   Menu *currentMenu;//当前的菜单

 
void display_oled(uint8_t line_oled) //显示菜单项并设置选中的项反白
{
    int i,a=-2,b=-1;
    OLED_Fill(0x00);
    if((currentMenu == &PIDMenu) ||(currentMenu == &DirMenu) || (currentMenu == &SpeedMenu))
    {
        line_oled = (7-(currentMenu->range_to-line_oled));
        for(i = 0;i<itemCounts;i++)
        {
            b++;
            if(b==line_oled)
            {
                OLED_P6x8Str(20,line_oled,currentMenu->menuItems[i+currentMenu->range_from]);
                if(modify_MPU == 1 || modify_Dir == 1 || modify_Speed == 1)
                {
                    OLED_P6x8Str(0,line_oled," *");                    
                }
                
                else//if(modify_MPU == 0) //|| modify_Dir == 0 || modify_Speed == 0)
                {
                    OLED_P6x8Str(0,line_oled,"->");
                }
            }
            else 
            {
                OLED_P6x8Str(20,b,currentMenu->menuItems[currentMenu->range_from+i]);
            }
        }               
    }

    if((currentMenu == &MainMenu) || (currentMenu == &colorMenu)|| (currentMenu == &dotMenu) || (currentMenu == &sysMenu) || (currentMenu == &searchMenu) || (currentMenu == &backMenu) || (currentMenu == &NUMSystem) || (currentMenu == &ParameterMenu))
    {
				line_oled = (3-(currentMenu->range_to-line_oled))*2;
        for(i = 0;i<4;i++)
        {
            a=a+2;
            if(a==line_oled)
            {
                OLED_Print2(20,line_oled,1,currentMenu->menuItems[i+currentMenu->range_from]);
                OLED_Print2(0,line_oled,0,"角");
            }
            else 
            {
                OLED_Print2(20,a,0,currentMenu->menuItems[currentMenu->range_from+i]);

            }
        }
    }
}
      


void func1(void)
{
   NUM_Var = 0.2f;
}

void func2(void)
{
   NUM_Var = 0.1f;
}

void func3(void)
{
   NUM_Var = 1.0f;
}

void func4(void)
{
   NUM_Var = 2.0f;
}

void func5(void)
{
   NUM_Var = 5.0f;
}

void func6(void)
{
   NUM_Var = 10.0f;
}

void func_system_1(void)
{
//    mpu6050_ratio = 0.0;
}

void func_system_2(void)
{
//    direction_ratio = 0.0;
}

void func_system_3(void)
{
//    speed_ratio = 0.0;
}

void color_choose0(void)
{
	m_color = 0;
}
void color_choose1(void)
{
	m_color = 1;
}
void color_choose2(void)
{
	m_color = 2;
}
void color_choose3(void)
{
	m_color = 3;
}
void color_choose4(void)
{
	m_color = 4;
}
void color_choose5(void)
{
	m_color = 5;
}
void color_choose6(void)
{
	m_color = 6;
}


void color_dot0(void)
{
	dot_color = 0;
}
void color_dot1(void)
{
	dot_color = 1;
}
void color_dot2(void)
{
	dot_color = 3;
}

void system_choose(void)
{
	draw_line_rgb(0,28,64,0,6);
	draw_line_rgb(0,27,64,0,6);
	draw_line_rgb(0,3,64,0,6);
	draw_line_rgb(0,4,64,0,6);
	m_color = 6;
//	Light_ON = 1;
//	disp_background(9,6);
}
void system_choose1(void)
{
//	m_color = 6;
	Change_Color = 1;
//	disp_background(9,6);
}

void background_choose0(void)
{
	Light_ON = 1;
//	disp_background(9,m_color);
}


void initMenu(void)
{     
     MainMenu.subMenus = malloc(sizeof(&MainMenu)*4);
		 MainMenu.subMenus[0] = &colorMenu;
     MainMenu.subMenus[1] = &dotMenu;
     MainMenu.subMenus[2] = &sysMenu;
     MainMenu.subMenus[3] = &aboutMenu;//第五项查询有子菜单
//     MainMenu.subMenus[4] = NULL;
     MainMenu.func = NULL;
     MainMenu.parent = NULL;//表示是顶级菜单
 
		 colorMenu.subMenus = malloc(sizeof(&colorMenu)*8);
		 colorMenu.subMenus[0] = NULL;
		 colorMenu.subMenus[1] = NULL;
		 colorMenu.subMenus[2] = NULL;
	   colorMenu.subMenus[3] = NULL;
	   colorMenu.subMenus[4] = NULL;
	   colorMenu.subMenus[5] = NULL;
	   colorMenu.subMenus[6] = NULL;
		 colorMenu.subMenus[7] = NULL;
	   colorMenu.func = malloc(sizeof(&color_choose0)*8);
	   colorMenu.func[0] = NULL;   
		 colorMenu.func[1] = &color_choose0;   
		 colorMenu.func[2] = &color_choose1;   
		 colorMenu.func[3] = &color_choose2;   
		 colorMenu.func[4] = &color_choose3;   
		 colorMenu.func[5] = &color_choose4;   
		 colorMenu.func[6] = &color_choose5;   
		 colorMenu.func[7] = &color_choose6;   
		 colorMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
		 
		 dotMenu.subMenus = malloc(sizeof(&dotMenu)*8);
		 dotMenu.subMenus[0] = NULL;
		 dotMenu.subMenus[1] = NULL;
		 dotMenu.subMenus[2] = NULL;
	   dotMenu.subMenus[3] = NULL;
	   dotMenu.subMenus[4] = NULL;
	   dotMenu.subMenus[5] = NULL;
	   dotMenu.subMenus[6] = NULL;
		 dotMenu.subMenus[7] = NULL;
	   dotMenu.func = malloc(sizeof(&color_dot0)*4);
	   dotMenu.func[0] = NULL;   
		 dotMenu.func[1] = &color_dot0;   
		 dotMenu.func[2] = &color_dot1;   
		 dotMenu.func[3] = &color_dot2;   
		 dotMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
	
	   sysMenu.subMenus = malloc(sizeof(&sysMenu)*5);
		 sysMenu.subMenus[0] = NULL;
		 sysMenu.subMenus[1] = NULL;
		 sysMenu.subMenus[2] = NULL;
	   sysMenu.subMenus[3] = &lightnessMenu;
		 sysMenu.subMenus[4] = &backMenu;
	   sysMenu.func = malloc(sizeof(&system_choose)*4);
	   sysMenu.func[0] = NULL;   
		 sysMenu.func[1] = &system_choose;   
		 sysMenu.func[2] = &system_choose1;   
		 sysMenu.func[3] = NULL;   
		 sysMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
	
		 lightnessMenu.subMenus = NULL;
	   lightnessMenu.func = NULL;   
		 lightnessMenu.parent = &sysMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
		 
		 backMenu.subMenus = malloc(sizeof(&backMenu)*4);
		 backMenu.subMenus[0] = NULL;
		 backMenu.subMenus[1] = NULL;
		 backMenu.subMenus[2] = NULL;
		 backMenu.subMenus[3] = NULL;
	   backMenu.func = malloc(sizeof(&background_choose0)*4);
	   backMenu.func[0] = NULL;   
		 backMenu.func[1] = &background_choose0;   
		 backMenu.func[2] = NULL;   
		 backMenu.func[3] = NULL;  
		 backMenu.parent = &sysMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
		 
		 aboutMenu.subMenus = NULL;
	   aboutMenu.func = NULL;   
		 aboutMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
	
     searchMenu.subMenus = malloc(sizeof(&searchMenu)*6);
     searchMenu.subMenus[0] = NULL;
     searchMenu.subMenus[1] = NULL;
     searchMenu.subMenus[2] = NULL;
     searchMenu.subMenus[3] = NULL;
     searchMenu.subMenus[4] = NULL;
     searchMenu.subMenus[5] = NULL;
     searchMenu.func = malloc(sizeof(&func1)*6);
     searchMenu.func[0] = NULL;     
     searchMenu.func[1] = &func_system_1;
     searchMenu.func[2] = &func_system_2;
     searchMenu.func[3] = &func_system_3;
     searchMenu.func[4] = NULL;
     searchMenu.func[5] = NULL;
     searchMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
     
     NUMSystem.subMenus = NULL;
//     NUMSystem.subMenus = malloc(sizeof(&NUMSystem)*6);
//     NUMSystem.subMenus[0] = NULL;
//     NUMSystem.subMenus[1] = NULL;
//     NUMSystem.subMenus[2] = NULL;
//     NUMSystem.subMenus[3] = NULL;
//     NUMSystem.subMenus[4] = NULL;
//     NUMSystem.subMenus[5] = NULL;
     NUMSystem.func = malloc(sizeof(&func1)*6);
     NUMSystem.func[0] = &func1;
     NUMSystem.func[1] = &func2;
     NUMSystem.func[2] = &func3;
     NUMSystem.func[3] = &func4;
     NUMSystem.func[4] = &func5;
     NUMSystem.func[5] = &func6;
     NUMSystem.parent = &ParameterMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
     
     ParameterMenu.subMenus = malloc(sizeof(&ParameterMenu)*4);
     ParameterMenu.subMenus[0] = &PIDMenu;
     ParameterMenu.subMenus[1] = &DirMenu;
     ParameterMenu.subMenus[2] = &SpeedMenu;
     ParameterMenu.subMenus[3] = &NUMSystem;
     ParameterMenu.func = NULL;
//     ParameterMenu.func = malloc(sizeof(&func1)*3);
//     ParameterMenu.func[0] = NULL;
//     ParameterMenu.func[1] = NULL;
//     ParameterMenu.func[2] = NULL;
     ParameterMenu.parent = &MainMenu;
     
     PIDMenu.subMenus = NULL;
//     PIDMenu.subMenus = malloc(sizeof(&PIDMenu)*3);
//     PIDMenu.subMenus[0] = NULL;
//     PIDMenu.subMenus[1] = NULL;
//     PIDMenu.subMenus[2] = NULL;
     PIDMenu.func = NULL;
//     PIDMenu.func = malloc(sizeof(&func1)*3);
//     PIDMenu.func[0] = NULL;     
//     PIDMenu.func[1] = NULL;
//     PIDMenu.func[2] = NULL;
//     PIDMenu.func[3] = NULL;
//     PIDMenu.func[4] = NULL;
//     PIDMenu.func[5] = NULL;
     PIDMenu.parent = &ParameterMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
     
     DirMenu.subMenus = NULL;
     DirMenu.func = NULL;
     DirMenu.parent = &ParameterMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
     
     SpeedMenu.subMenus = NULL;
     SpeedMenu.func = NULL;
     SpeedMenu.parent = &ParameterMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
     
     
//     sensorMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
//     sensorMenu.subMenus = NULL;
//     sensorMenu.func = NULL;
     
//     gyroMenu.parent = &MainMenu;//上一级菜单是MainMenu.进入查询子菜单后按返回键，将会显示这个菜单 项
//     gyroMenu.subMenus = NULL;
//     gyroMenu.func = NULL;
     
     currentMenu = &MainMenu;
}



void OLED_Control(void)
{
    if(key_up>=1)
    {
        key_up=0;

            if(modify_MPU == 1)
            {
//              flash_data_reset(&data);

//               switch (currentMenu->selected){
//               case 0: Parameter_all.angle_expectation+=NUM;break;
//               case 1: Parameter_all.kp+=NUM;break;
//               case 2: Parameter_all.kd+=NUM;break;
//               case 3: Parameter_all.core_kp+=NUM;break;
//               case 4: Parameter_all.core_ki+=NUM;break;
//              }
            }
            if(modify_Dir == 1)
            {
//              flash_data_reset(&data);

//               switch (currentMenu->selected){
//               case 0: Parameter_all.Dir_Kp+=NUM;break;
//               case 1: Parameter_all.Dir_Kd+=NUM;break;
//               case 2: Parameter_all.kd+=NUM;break;
//               case 3: Parameter_all.core_kp+=NUM;break;
//               case 4: Parameter_all.core_ki+=NUM;break;
//              }
            }
            if(modify_Speed == 1)
            {
//               flash_data_reset(&data);

//               switch (currentMenu->selected){
//               case 0: Parameter_all.Speed_expectation+=NUM;break;
//               case 1: Parameter_all.Speed_Kp+=NUM;break;
//               case 2: Parameter_all.Speed_Ki+=NUM;break;
//               case 3: Parameter_all.core_kp+=NUM;break;
//               case 4: Parameter_all.core_ki+=NUM;break;
//              }
            }
						if(currentMenu == &lightnessMenu)
						{
							Lightness_Value+=5;
							if(Lightness_Value>145)
							{
								Lightness_Value = 148;
							}
						  TIM_SetTIM2Compare1(Lightness_Value);
						}
            if((currentMenu != &PIDMenu) && (currentMenu != &DirMenu) && (currentMenu != &SpeedMenu) && modify_MPU == 0 && modify_Dir == 0 && modify_Speed == 0)
            {
              if(modify_MPU == 0 && modify_Dir == 0 && modify_Speed == 0)
              {  
                  if(currentMenu->selected == 0)//到了第一项
                  {
                     currentMenu->selected=0;
                  }
                  else
                  {
                     currentMenu->selected--;
                     if(currentMenu->selected < currentMenu->range_from)
                     {
                          currentMenu->range_from = currentMenu->selected;
                          currentMenu->range_to = currentMenu->range_from+3;
                     }
                     display_oled(currentMenu->selected);
                  }
                }
							}
    }
    
    if(key_down>=1)
    {
        key_down=0;
 
            if(modify_MPU == 1)
            {
//                flash_data_reset(&data);

//                switch (currentMenu->selected){
//                case 0: Parameter_all.angle_expectation-=NUM;break;
//                case 1: Parameter_all.kp-=NUM;break;
//                case 2: Parameter_all.kd-=NUM;break;
//                case 3: Parameter_all.core_kp-=NUM;break;
//                case 4: Parameter_all.core_ki-=NUM;break;
//                }
            }
            if(modify_Dir == 1)
            {
//                flash_data_reset(&data);

//                switch (currentMenu->selected){
//                case 0: Parameter_all.Dir_Kp-=NUM;break;
//                case 1: Parameter_all.Dir_Kd-=NUM;break;
//                }
            }
            if(modify_Speed == 1)
            {
//                flash_data_reset(&data);

//                switch (currentMenu->selected){
//                case 0: Parameter_all.Speed_expectation-=NUM;break;
//                case 1: Parameter_all.Speed_Kp-=NUM;break;
//                case 2: Parameter_all.Speed_Ki-=NUM;break;
//                }
            }
            if(currentMenu == &lightnessMenu)
						{
//							Lightness_Value_Change = Lightness_Value;
							Lightness_Value-=5;
							if(Lightness_Value<5)
							{
								Lightness_Value = 1;
							}
								TIM_SetTIM2Compare1(Lightness_Value);
						}
            if(modify_MPU == 0 && modify_Dir == 0 && modify_Speed == 0)
            {

                if( currentMenu->selected == currentMenu->itemCount-1 )
                {
                   currentMenu->selected = currentMenu->itemCount-1;
                }
                else if( (currentMenu == &MainMenu) || (currentMenu==&colorMenu) || (currentMenu==&dotMenu) || (currentMenu == &sysMenu) || (currentMenu == &backMenu) || (currentMenu == &searchMenu) || (currentMenu == &ParameterMenu))
                {
                    currentMenu->selected++;
                    if(currentMenu->selected > currentMenu->range_to)
                    {
                       currentMenu->range_to = currentMenu->selected;
                       currentMenu->range_from = currentMenu->range_to-3;
                       
                    }
                    display_oled(currentMenu->selected);
                } 
            }
    }
    
    if(key_l>=1)
    {
        key_l=0;
        
            if(currentMenu->parent!=NULL)
            {       
//                   PID_init(&direction_pid,0,Parameter_all.Dir_Kp,0,Parameter_all.Dir_Kd,0,0,0);      //2.5     50
//                   PID_init(&speed_pid,Parameter_all.Speed_expectation,Parameter_all.Speed_Kp,Parameter_all.Speed_Ki,0,0,0,0);        //100     20     0.1
//                   PID_init(&mpu6050_pid,Parameter_all.angle_expectation,Parameter_all.kp,Parameter_all.ki,Parameter_all.kd,Parameter_all.core_kp,Parameter_all.core_ki,Parameter_all.core_kd);
//                   ON_sensor = 0;
//                   ON_gyro = 0;
                   ON_pid_MPU = 0;
                   ON_pid_Dir = 0;
                   ON_pid_Speed = 0;
                   modify_MPU = 0;
                   modify_Speed = 0;
                   modify_Dir = 0;
              
									 Lightness_ON = 0;
							     ON_about = 0;
							
//                   flash_data_save(&data);
                   currentMenu = currentMenu->parent;
                   display_oled(currentMenu->selected);

            }

    }
    
    if(key_r>=1)
    {
        key_r=0;
            if(modify_MPU == 0 || modify_Dir == 0 || modify_Speed == 0)
            {
                if((currentMenu->subMenus[currentMenu->selected] != NULL) && (currentMenu->subMenus != NULL))
                {
                   currentMenu = currentMenu->subMenus[currentMenu->selected];
//                   if( (currentMenu == &sensorMenu) || (currentMenu == &gyroMenu) )    
//                   {
//                      variable_display();  lightnessMenu
//                   }
									 if((currentMenu == &lightnessMenu) || (currentMenu == &aboutMenu))
									 {
											variable_display();
									 }
                   if(currentMenu == &PIDMenu)
                   {
                      ON_pid_MPU = 1;
                      itemCounts = 5;                     
                      display_oled(currentMenu->selected);
                   }
                   if(currentMenu == &DirMenu)
                   {
                      ON_pid_Dir = 1;
                      itemCounts = 2;
                      display_oled(currentMenu->selected);
                   }
                   if(currentMenu == &SpeedMenu)
                   {
                      ON_pid_Speed = 1;
                      itemCounts = 3;
                      display_oled(currentMenu->selected);
                   }
                   if(currentMenu == &searchMenu || currentMenu == &NUMSystem || currentMenu == &ParameterMenu ||currentMenu == &DirMenu ||currentMenu == &SpeedMenu || currentMenu == &colorMenu || (currentMenu == &dotMenu) || (currentMenu == &sysMenu) || (currentMenu == &backMenu))
                   {
                      display_oled(currentMenu->selected);                 
                   }
                }
                else
                {
                    if((currentMenu->func[currentMenu->selected] != NULL) && (currentMenu->func!= NULL))
                    {                 
                        OLED_Fill(0xff);
                        currentMenu->func[currentMenu->selected]();//执行相应的函数
                        rt_thread_delay(1000);
                        display_oled(currentMenu->selected);//返回后恢复原来的菜单状态
                    }
               
                }
            }     
            if(modify_MPU == 1 || modify_Dir == 1 || modify_Speed == 1)
            {
                modify_MPU = 0;
                modify_Dir = 0;
                modify_Speed = 0;
                
//                PID_init(&mpu6050_pid,Parameter_all.angle_expectation,Parameter_all.kp,Parameter_all.ki,Parameter_all.kd,
//                                      Parameter_all.core_kp,Parameter_all.core_ki,Parameter_all.core_kd);
//                PID_init(&direction_pid,0,Parameter_all.Dir_Kp,0,Parameter_all.Dir_Kd,0,0,0);      //2.5     50
//                PID_init(&speed_pid,Parameter_all.Speed_expectation,Parameter_all.Speed_Kp,Parameter_all.Speed_Ki,0,0,0,0);        //100     20     0.1
//                flash_data_save(&data);
                display_oled(currentMenu->selected);//返回后恢复原来的菜单状态
            }
    }
    
    if(key_a>=1)
    {
        key_a=0;

        if( (currentMenu == &PIDMenu) && (modify_MPU == 0))
        {
            modify_MPU = 1;
            display_oled(currentMenu->selected);
        }
        if( (currentMenu == &DirMenu) && (modify_Dir == 0))
        {
            modify_Dir = 1;
            display_oled(currentMenu->selected);
        }
        if( (currentMenu == &SpeedMenu) && (modify_Speed == 0))
        {
            modify_Speed = 1;
            display_oled(currentMenu->selected);
        } 
    }
  
    
    
    
}

    


/******************************************
*           五轴按键控制OLED               *
*****************************************/
void key_select(void)
{
    if(KEY_UP == 0)       //判断是否有效的KBI中断
    {  
       rt_thread_delay(200);                 //上
       key_up=1;
    }
    
    
    if(KEY_DOWN == 0)       //判断是否有效的KBI中断
    {  
       rt_thread_delay(200);                 //上
       key_down=1;
    }
    
		
    if(KEY_L == 0)       //判断是否有效的KBI中断
    {  
       rt_thread_delay(200);                 //上
       key_l=1;
    }

		
    if(KEY_R == 0)       //判断是否有效的KBI中断
    {  
       rt_thread_delay(200);                 //上
       key_r=1;
    }

		
    if(KEY_A == 0)       //判断是否有效的KBI中断
    {  
       rt_thread_delay(200);                 //上
       key_a=1;
			 if(Lightness_flag == 1 && Lightness_Change_finish == 1)
			 {
				 Lightness_Change_finish = 0;
				 key_a_count = 0;
				 Lightness_flag = 0;
				 LED2(0);
			 }
			 else
			 {
				 key_a_count++;
			 }			 
			 if(key_a_count >= 6)
			 {
				 Lightness_flag = 1;
				 LED2(1);
			 }
			 if(currentMenu == &aboutMenu)
			 {
				 OLED_Fill(0x00);
				 Draw_LibLogo();
			 }
    }  
		
}

void OLED(void)
{  
    initMenu();
		display_oled(currentMenu->selected);
}


/*************************************************************/
/*************参数显示函数************************************/
/*************************************************************/
void oled_sensor(void)
{
		OLED_PrintU16_3(65,4,Lightness_Value);
//    OLED_PrintU16(35,0,sensor_value_L);
//    OLED_PrintU16(75,0,AD_value[0]);
//    OLED_PrintU16(35,1,sensor_value_R);
//    OLED_PrintU16(75,1,AD_value[1]);
//    OLED_PrintU16(35,2,sensor_value_M);
//    OLED_PrintU16(75,2,AD_value[2]);
//    OLED_PrintU16(35,3,sensor_value_Ver_L);
//    OLED_PrintU16(75,3,AD_value[3]);
//    OLED_PrintU16(35,4,sensor_value_Ver_R);
//    OLED_PrintU16(75,4,AD_value[4]);
//    OLED_PrintFloat(35,5,Dir_error);  
//    OLED_PrintFloat(80,5,Dir_error_test3);
//    OLED_PrintFloat(35,6,Dir_error_test1);
//    OLED_PrintFloat(80,6,Dir_error_test2);
//    OLED_PrintFloat(35,7,angle);
    
}



//void oled_gyro(void)
//{
////    xianshi(29,4,a);
////    OLED_PrintFloat(40,1,angle);
////    OLED_PrintFloat(70,3,encoder_count_L);
////    OLED_PrintFloat(70,4,encoder_count_R);
//}





void oled_pid_MPU(void)
{
    
//    OLED_PrintFloat(55,0,Parameter_all.angle_expectation);
//    OLED_PrintFloat(75,1,Parameter_all.kp);
//    OLED_PrintFloat(75,2,Parameter_all.kd);
//    OLED_PrintFloat(75,3,Parameter_all.core_kp);
//    OLED_PrintFloat(75,4,Parameter_all.core_ki);
    
}

void oled_pid_Dir(void)
{
    
//    OLED_PrintFloat(75,0,Parameter_all.Dir_Kp);
//    OLED_PrintFloat(75,1,Parameter_all.Dir_Kd);
//    OLED_PrintFloat(75,2,Parameter_all.kd);
//    OLED_PrintFloat(75,3,Parameter_all.core_kp);
//    OLED_PrintFloat(75,4,Parameter_all.core_ki);
    
}

void oled_pid_Speed(void)
{
    
//    OLED_PrintFloat(75,0,Parameter_all.Speed_expectation);
//    OLED_PrintFloat(75,1,Parameter_all.Speed_Kp);
//    OLED_PrintFloat(75,2,Parameter_all.Speed_Ki);
//    OLED_PrintFloat(75,3,Parameter_all.core_kp);
//    OLED_PrintFloat(75,4,Parameter_all.core_ki);
    
}

/*************************************************************/
/*************子菜单参数名称显示设置***************************/
/*************************************************************/

void variable_display(void)
{    
  OLED_Fill(0x00);	
	if(currentMenu == &lightnessMenu)
	{
		Lightness_ON = 1;
	  OLED_P8x16Str(6,0,"*Lightness_Sys*");
		OLED_P8x16Str(6,4,"&Value:");		
	}
	if(currentMenu == &aboutMenu)
	{
	  OLED_P6x8Str(1,0,"* Designed By LiKun *");
		OLED_P6x8Str(38,2,"* Owner *");
		OLED_P6x8Str(15,3,"* ************* *");	
		OLED_P6x8Str(26,5,"* 2018.12.9 *");
	}
//    if(currentMenu == &sensorMenu)
//    {
//      ON_sensor = 1;
//      OLED_P6x8Str(0,0,"Left :");
//      OLED_P6x8Str(0,1,"Right:");
//      OLED_P6x8Str(0,2,"Middl:");
//      OLED_P6x8Str(0,3,"Ver_L:");
//      OLED_P6x8Str(0,4,"Ver_R:");
//      OLED_P6x8Str(0,5,"error:");
//      OLED_P6x8Str(0,6,"err_L:");
//      OLED_P6x8Str(0,7,"angle:");
//    }
//    else if(currentMenu == &gyroMenu)
//    {
//      ON_gyro = 1;
//      OLED_P6x8Str(0,1,"angle:");
//      OLED_P6x8Str(0,3,"left_coder :");
//      OLED_P6x8Str(0,4,"right_coder:");
//    }
}

void Display_switch(void)
{
//    if(ON_sensor == 1)
//    {
//      oled_sensor();
//    }
//    if(ON_gyro == 1)
//    {
//      oled_gyro();
//    }
	  if(Lightness_ON == 1)
		{
			oled_sensor();
		}
    if(ON_pid_MPU == 1)
    {
      oled_pid_MPU();
    }
    if(ON_pid_Dir == 1)
    {
      oled_pid_Dir();
    }
    if(ON_pid_Speed == 1)
    {
      oled_pid_Speed();
    }

}

void OLED_switch(void)
{
//    if(KEY_X == 0)
//    {
//       KEY+=1;
//    }
//    if(KEY%2==1)
//    {            
       OLED_Control();           
       Display_switch();
//       led (LED3,LED_ON);              
//     }
//     else
//     {
//        KEY = 0;
////        led (LED3,LED_OFF);              
//     }    
}


/******************************************
*           五轴按键控制OLED               *
*****************************************/
/*
void  kbi0_irq(void)
{
    if(IS_KBI0_IRQ())       //判断是否有效的KBI中断
    {
        if(IS_KBI_CH_IRQ(PTD6)) //判断是否PTD6触发的中断
        {                       //上
//            led_turn(LED3);
//            DELAY_MS(200); 
            //modify = 0;
            if(modify == 1)
            {
//                PID_Motor.P=0x00;
//                flash_data_save(&data) ;  
              //flash_data_reset(&data);
//               PID_Motor.P=0;
//               PID_Motor.I=0;
//               PID_Motor.D=0;
//               flash_data_load(&data) ; 
//               flash_erase_sector(FLASH_SECTOR_NUM - 3);
//               flash_erase_sector(FLASH_SECTOR_NUM - 2);
//               flash_erase_sector(FLASH_SECTOR_NUM - 1);
               switch (currentMenu->selected){
               case 0: kp=kp+NUM;break;
               case 1: kd=kd+NUM;break;
               case 2: core_kp=core_kp+NUM;break;
               case 3: core_ki=core_ki-NUM;break;
              }
            }
            else
            {
                
              if(currentMenu->selected == 0)//到了第一项
              {
                 currentMenu->selected=0;
              }
              else
              {
                 currentMenu->selected--;
                 if(currentMenu->selected< currentMenu->range_from)
                 {
                      currentMenu->range_from = currentMenu->selected;
                      currentMenu->range_to = currentMenu->range_from+3;
                 }
                 display_oled(currentMenu->selected);
              }
            }
         }
    
    
    
        if(IS_KBI_CH_IRQ(PTC3)) //判断是否PTC3触发的中断
        {
//            led_turn(LED2);     //下
//            DELAY_MS(200); 
            if(modify == 1)
            {
                //flash_data_reset(&data);
//                PID_Motor.P=0;
//                PID_Motor.I=0;
//                PID_Motor.D=0;
//                flash_data_load(&data) ;
                switch (currentMenu->selected){
                case 0: kp=kp-NUM;break;
                case 1: kd=kd-NUM;break;
                case 2: core_kp=core_kp-NUM;break;
                case 3: core_ki=core_ki-NUM;break;
            }
            }
            else
            {
              //modify = 0;
            if( currentMenu->selected == currentMenu->itemCount-1 )
            {
               currentMenu->selected = currentMenu->itemCount-1;
            }
            else if( currentMenu!=&sensorMenu && currentMenu!=&gyroMenu )
            {
                currentMenu->selected++;
                if(currentMenu->selected > currentMenu->range_to)
                {
                   currentMenu->range_to = currentMenu->selected;
                   currentMenu->range_from = currentMenu->range_to-3;
                }
                display_oled(currentMenu->selected);
            } 
            }
       }
    
    
        if(IS_KBI_CH_IRQ(PTD7)) //判断是否PTD7触发的中断
        {                       //左
//            led_turn(LED3);
//           DELAY_MS(200);           
            if(currentMenu->parent!=NULL)
            {             

                   ON_sensor = 0;
                   ON_gyro = 0;
                   ON_pid = 0;
                   modify = 0;
                   //flash_data_save(&data);
                   currentMenu = currentMenu->parent;
                   display_oled(currentMenu->selected);

            }
//            DELAY_MS(100);

        }
    

        if(IS_KBI_CH_IRQ(PTC2)) //判断是否PTC2触发的中断
        {                       //右
//            led_turn(LED3);
//            DELAY_MS(200); 
            if(modify == 0)
            {
                if((currentMenu->subMenus[currentMenu->selected] != NULL) && (currentMenu->subMenus != NULL))
                {
                   currentMenu = currentMenu->subMenus[currentMenu->selected];
                   if( (currentMenu == &sensorMenu) || (currentMenu == &gyroMenu) )    
                   {
                      display();
                   }
                   if(currentMenu == &PIDMenu)
                   {
                      ON_pid = 1;                      
                      display_oled(currentMenu->selected);
                   }
                   if(currentMenu == &searchMenu || currentMenu == &NUMSystem)
                   {
                      display_oled(currentMenu->selected);                 
                   }
                }
                else
                {
                    if((currentMenu->func[currentMenu->selected] != NULL) && (currentMenu->func!= NULL))
                    {                 
                        OLED_Fill(0xff);
                        currentMenu->func[currentMenu->selected]();//执行相应的函数
                        DELAY_MS(1000);
                        display_oled(currentMenu->selected);//返回后恢复原来的菜单状态
                    }
               
                }
            }      //           } DELAY_MS(100);
            else
            {
                modify = 0;
                flash_data_save(&data);
                display_oled(currentMenu->selected);//返回后恢复原来的菜单状态
            }

          }
    

        
        if(IS_KBI_CH_IRQ(PTD5)) //判断是否PTD5触发的中断
        {
//            DELAY_MS(200); 
            if( currentMenu == &PIDMenu && modify == 0)
            {
                modify = 1;
                display_oled(currentMenu->selected);
            }           
        }

          
    }
    KBI0_CLEAN_FLAG() ;  
}

*/
#include "valve_control.h"
#include "api/gpio.h"
#include "api/myhelper.h"
#include "api/myapi.h"

//int   Valve_control::Valve_Set_Count;
//bool Valve_control::Set_Flag;
bool Valve_control::Valve_CurrentStatus;
//Status{IsOpen,IsClose,IsError}Valve_CurrentStatus,Valve_RightStatus;



//集水开启输出使能
void Valve_control::Catchment_Valve_Open_Set()
{
    SwitchOut_On(myApp::Out_drain_open); //开集水电磁阀
    SwitchOut_On(myApp::Out_drain_close); //开集水泵
//    Valve_control::Set_Flag=true;
    Valve_control::Valve_CurrentStatus=true;
//    Valve_Set_Count=0;
}

//集水关闭输出使能
void Valve_control::Catchment_Valve_Close_Set()
{
    SwitchOut_Off(myApp::Out_drain_close); //关集水泵
    SwitchOut_Off(myApp::Out_drain_open); //关集水电磁阀
//    Valve_control::Set_Flag=true;
    Valve_control::Valve_CurrentStatus=false;
//    Valve_Set_Count=0;
}


//排水开启输出使能
void Valve_control::drain_Valve_Open_Set()
{
    SwitchOut_On(myApp::Out_catchment_open); //开排水电磁阀
//    Set_Flag=true;
//    Valve_RightStatus=IsOpen;
//    Valve_Set_Count=0;
}

//排水关闭输出使能
void Valve_control::drain_Valve_Close_Set()
{
    SwitchOut_Off(myApp::Out_catchment_open); //关排水电磁阀
//    Set_Flag=true;
//    Valve_RightStatus=IsClose;
//    Valve_Set_Count=0;
}


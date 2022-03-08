//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

static Button_Struct_t TurnBack_Button2;
static Button_Struct_t Normal_Botton, ChargeFirst_Botton, UseFrist_Botton,
    SucapTest_Botton;//To switch Setting_OptiSchemes
static Numeric_Struct_t RTCYear_Numeric, RTCMonth_Numeric, RTCDate_Numeric;
static Numeric_Struct_t RTCHour_Numeric, RTCMinute_Numeric, RTCSecond_Numeric;
static Lable_Struct_t RTCDate_Lable, RTCTime_Lable;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
  Form_Info_Structure.Form_Index = Main_Form_Index;
  MainForm_Init();
}

void Normal_Botton_Callback(void *object, unsigned char key) {
  Setting_OptiSchemes = Normal_Optimized;
  Form_Info_Structure.Form_Index = Main_Form_Index;
  MainForm_Init();
}

void ChargeFirst_Botton_Callback(void *object, unsigned char key) {
  Setting_OptiSchemes = ChargeFirst_Optimized;
  Form_Info_Structure.Form_Index = Main_Form_Index;
  MainForm_Init();
}

void UseFirst_Botton_Callback(void *object, unsigned char key) {
  Setting_OptiSchemes = UseFirst_Optimized;
  Form_Info_Structure.Form_Index = Main_Form_Index;
  MainForm_Init();
}

void SucapTest_Botton_Callback(void *object, unsigned char key) {
  Setting_OptiSchemes = SucapTest_Optimized;
  Form_Info_Structure.Form_Index = Main_Form_Index;
  MainForm_Init();
}

void RTCYear_Numeric_Callback(void *object, unsigned char key) {

}

void SettingsForm_Update(void) {

}

void SettingsForm_Init(void) {
  GUI_Clear(C_WHITE);

  Normal_Botton.X_Pos = 2;
  Normal_Botton.Y_Pos = 2;
  Normal_Botton.Width = 80;
  Normal_Botton.Height = 22;
  Normal_Botton.Text = "Normal";
  Normal_Botton.NextButton = &ChargeFirst_Botton;
  Normal_Botton.CallbackFunction = Normal_Botton_Callback;

  ChargeFirst_Botton.X_Pos = 2;
  ChargeFirst_Botton.Y_Pos = 26;
  ChargeFirst_Botton.Width = 80;
  ChargeFirst_Botton.Height = 22;
  ChargeFirst_Botton.Text = "ChargeFirst";
  ChargeFirst_Botton.NextButton = &UseFrist_Botton;
  ChargeFirst_Botton.CallbackFunction = ChargeFirst_Botton_Callback;

  UseFrist_Botton.X_Pos = 2;
  UseFrist_Botton.Y_Pos = 50;
  UseFrist_Botton.Width = 80;
  UseFrist_Botton.Height = 22;
  UseFrist_Botton.Text = "UseFirst";
  UseFrist_Botton.NextButton = &SucapTest_Botton;
  UseFrist_Botton.CallbackFunction = UseFirst_Botton_Callback;

  SucapTest_Botton.X_Pos = 2;
  SucapTest_Botton.Y_Pos = 74;
  SucapTest_Botton.Width = 80;
  SucapTest_Botton.Height = 22;
  SucapTest_Botton.Text = "SucapTest";
  SucapTest_Botton.NextButton = &TurnBack_Button2;
  SucapTest_Botton.CallbackFunction = SucapTest_Botton_Callback;

  TurnBack_Button2.X_Pos = 2;
  TurnBack_Button2.Y_Pos = 134;
  TurnBack_Button2.Width = 80;
  TurnBack_Button2.Height = 22;
  TurnBack_Button2.Text = "TurnBack";
  TurnBack_Button2.NextButton = NULL;
  TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

  GUI_InitButton(&Normal_Botton);
  GUI_InitButton(&ChargeFirst_Botton);
  GUI_InitButton(&UseFrist_Botton);
  GUI_InitButton(&SucapTest_Botton);
  GUI_InitButton(&TurnBack_Button2);
}
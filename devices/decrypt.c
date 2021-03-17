#include "decrypt.h"
#include "string.h"
#include "system.h"
#include "pid.h"

static unsigned char Receive_Buffer[128];
static unsigned char PingPong_Buffer[128];
unsigned int Receive_BufCounter = 0;
float Parameters[4];

void Receive_CallBack(unsigned char PID, unsigned char Data[8]) {
    if (PID == 0) {
        Parameters[0] = Int16ToFloat((Data[0] << 8) | Data[1]);
        Parameters[1] = Int16ToFloat((Data[2] << 8) | Data[3]);
        Parameters[2] = Int16ToFloat((Data[4] << 8) | Data[5]);
        Parameters[3] = Int16ToFloat((Data[6] << 8) | Data[7]);
//
//        PID_Capacitor_P.I_Sum_Max = Parameters[0];
//        PID_Capacitor_P.Kp = Parameters[1];
//        PID_Capacitor_P.Ki = Parameters[2];
//        PID_Capacitor_P.Kd = Parameters[3];
    }
}

void DTP_Received_CallBack(unsigned char Receive_Byte) {

    unsigned char CheckFlag;
    unsigned int SOF_Pos, EOF_Pos, CheckCounter;

    Receive_Buffer[Receive_BufCounter] = Receive_Byte;
    Receive_BufCounter = Receive_BufCounter + 1;

    CheckFlag = 0;
    SOF_Pos = 0;
    EOF_Pos = 0;
    CheckCounter = 0;
    while (1) {
        if (CheckFlag == 0 && Receive_Buffer[CheckCounter] == 0xff) {
            CheckFlag = 1;
            SOF_Pos = CheckCounter;
        } else if (CheckFlag == 1 && Receive_Buffer[CheckCounter] == 0xff) {
            EOF_Pos = CheckCounter;
            break;
        }
        if (CheckCounter >= (Receive_BufCounter - 1))
            break;
        else
            CheckCounter++;
    }                                                           //Find Package In Buffer


    if ((EOF_Pos - SOF_Pos) == 11) {
        unsigned int Temp_Var;
        unsigned char Data_Buffer[8] = {0};
        unsigned char Valid_Buffer[12] = {0};

        for (Temp_Var = 0; Temp_Var < 12; Temp_Var++)           //Copy Data To Another Buffer
            Valid_Buffer[Temp_Var] = Receive_Buffer[SOF_Pos + Temp_Var];

        EOF_Pos++;
        memset(PingPong_Buffer, 0x00, sizeof(PingPong_Buffer));
        for (Temp_Var = 0; Temp_Var < Receive_BufCounter - EOF_Pos; Temp_Var++)
            PingPong_Buffer[Temp_Var] = Receive_Buffer[EOF_Pos + Temp_Var];
        Receive_BufCounter = Receive_BufCounter - EOF_Pos;
        memset(Receive_Buffer, 0x00, sizeof(Receive_Buffer));
        for (Temp_Var = 0; Temp_Var < Receive_BufCounter; Temp_Var++)
            Receive_Buffer[Temp_Var] = PingPong_Buffer[Temp_Var];

        unsigned char PID_Bit = Valid_Buffer[1] >> 4;           //Get The PID Bit
        if (PID_Bit == ((~(Valid_Buffer[1] & 0x0f)) & 0x0f))    //PID Verify
        {
            for (Temp_Var = 0; Temp_Var < 8; ++Temp_Var)
                Data_Buffer[Temp_Var] = Valid_Buffer[2 + Temp_Var];
            if (Valid_Buffer[10] != 0x00)                       //Some Byte had been replace
            {
                unsigned char Temp_Filter = 0x00;
                for (Temp_Var = 0; Temp_Var < 8; ++Temp_Var)
                    if (((Valid_Buffer[10] & (Temp_Filter | (0x01 << Temp_Var))) >> Temp_Var)
                        == 1)                                   //This Byte Need To Adjust
                        Data_Buffer[Temp_Var] = 0xff;           //Adjust to 0xff
            }
            Receive_CallBack(PID_Bit, Data_Buffer);
        }
    } else if ((EOF_Pos - SOF_Pos) != 0 && EOF_Pos != 0) {
        memset(Receive_Buffer, 0x00, sizeof(Receive_Buffer));
        memset(PingPong_Buffer, 0x00, sizeof(PingPong_Buffer));
        Receive_BufCounter = 0;
    }
}

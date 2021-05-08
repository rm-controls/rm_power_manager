//
// Created by Lao·Zhu on 2021/5/3.
//

#ifndef PROGRAM_DEVICES_FS_OPERATE_H_
#define PROGRAM_DEVICES_FS_OPERATE_H_

typedef enum {
    Poweron_Event = 0,
    SwitchTo_NormalMode_Event = 1,
    SwitchTo_OverPowerMode_Event = 2,
    SwitchTo_HaltMode_Event = 3,
    SwitchTo_TransitionMode_Event = 4,
} Event_Type_e;

typedef struct {
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    unsigned char Events;
} Event_Struct_t;

typedef struct {
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char EventLength;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    unsigned char Reserved1;
    unsigned int LastFileAddr;
    unsigned int NextFileAddr;
} FileHead_Struct_t;

typedef struct {
    FileHead_Struct_t FileHead;
    Event_Struct_t Event[256];
    unsigned char ResetInfo[16];
} File_Struct_t;

typedef struct {
    unsigned char FSHead;
    unsigned char FileNum;
    unsigned char Reserved1;
    unsigned char Reserved2;
    unsigned int FirstFileAddr;
    unsigned int LastFileAddr;
    unsigned char FileLUT[32];
} FileSystem_Struct_t;

extern FileSystem_Struct_t *FileSystem_Structure;
extern File_Struct_t *CurrentFile_Structure;
extern FileHead_Struct_t *LastFileHead_Structure;
extern unsigned int CurrentFile_Address, LastFile_Address;

void FileSystem_Config(void);
void FileSystem_WriteIntoFlash(void);
void FileSystem_FormatFlash(void);
void FileSystem_CreateFiles(void);
void FileSystem_FindRemainSpace(void);
void FileSystem_ReadItem(unsigned char itemnum, File_Struct_t *file);

#endif //PROGRAM_DEVICES_FS_OPERATE_H_

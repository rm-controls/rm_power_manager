//
// Created by Lao·Zhu on 2021/3/26.
//

#ifndef PROGRAM_DEVICES_REFREE_H_
#define PROGRAM_DEVICES_REFREE_H_

typedef enum {
    kGameStatusCmdId = 0x0001,
    kGameResultCmdId = 0x0002,
    kGameRobotHpCmdId = 0x0003,
    kDartStatusCmdId = 0x0004,
    kIcraZoneStatusCmdId = 0x0005,
    kFieldEventsCmdId = 0x0101,
    kSupplyProjectileActionCmdId = 0x0102,
    kRefereeWarningCmdId = 0x0104,
    kDartRemainingCmdId = 0x0105,
    kRobotStatusCmdId = 0x0201,
    kPowerHeatDataCmdId = 0x0202,
    kRobotPosCmdId = 0x0203,
    kBuffCmdId = 0x0204,
    kAerialRobotEnergyCmdId = 0x0205,
    kRobotHurtCmdId = 0x0206,
    kShootDataCmdId = 0x0207,
    kBulletRemainingCmdId = 0x0208,
    kRobotRfidStatusCmdId = 0x0209,
    kDartClientCmdId = 0x020A,
    kStudentInteractiveDataCmdId = 0x0301,
    kRobotInteractiveDataCmdId = 0x0302,
    kRobotCommandCmdId = 0x0303,
} RefereeCmdId;

typedef struct {
    unsigned char sof;
    unsigned short data_length;
    unsigned char seq;
    unsigned char crc8;
} FrameHeaderStruct;

// Unpacking order
typedef enum {
    kStepHeaderSof = 0,
    kStepLengthLow = 1,
    kStepLengthHigh = 2,
    kStepFrameSeq = 3,
    kStepHeaderCrc8 = 4,
    kStepDataCrc16 = 5,
} UnpackStep;

// Unpacking data
typedef struct {
    FrameHeaderStruct *p_header;
    unsigned short data_len;
    unsigned char protocol_packet[128];
    UnpackStep unpack_step;
    unsigned short index;
} UnpackData;

/*------------------------REFEREE_GAME_CMD---- 0x40 -------------------*/
typedef struct {
    unsigned char game_type: 4;
    unsigned char game_progress: 4;
    unsigned short stage_remain_time;
} GameStatus;

typedef struct {
    unsigned char winner;
} GameResult;

typedef struct {
    unsigned short red_1_robot_HP;
    unsigned short red_2_robot_HP;
    unsigned short red_3_robot_HP;
    unsigned short red_4_robot_HP;
    unsigned short red_5_robot_HP;
    unsigned short red_7_robot_HP;
    unsigned short red_outpost_HP;
    unsigned short red_base_HP;
    unsigned short blue_1_robot_HP;
    unsigned short blue_2_robot_HP;
    unsigned short blue_3_robot_HP;
    unsigned short blue_4_robot_HP;
    unsigned short blue_5_robot_HP;
    unsigned short blue_7_robot_HP;
    unsigned short blue_outpost_HP;
    unsigned short blue_base_HP;
} GameRobotHp;

typedef struct {
    unsigned char dart_belong;
    unsigned short stage_remaining_time;
} DartStatus;

typedef struct {
    unsigned char F1_zone_status: 1;
    unsigned char F1_zone_buff_debuff_status: 3;
    unsigned char F2_zone_status: 1;
    unsigned char F2_zone_buff_debuff_status: 3;
    unsigned char F3_zone_status: 1;
    unsigned char F3_zone_buff_debuff_status: 3;
    unsigned char F4_zone_status: 1;
    unsigned char F4_zone_buff_debuff_status: 3;
    unsigned char F5_zone_status: 1;
    unsigned char F5_zone_buff_debuff_status: 3;
    unsigned char F6_zone_status: 1;
    unsigned char F6_zone_buff_debuff_status: 3;
} IcraBuffDebuffZoneStatus;

/*-------------------REFEREE_BATTLEFIELD_CMD_SET---- 0x41 -------------*/
typedef struct {
    unsigned int event_type;
} EventData;

typedef struct {
    unsigned char supply_projectile_id;
    unsigned char supply_robot_id;
    unsigned char supply_projectile_step;
    unsigned char supply_projectile_num;
} SupplyProjectileAction;

typedef struct {
    unsigned char level;
    unsigned char foul_robot_id;
} RefereeWarning;

typedef struct {
    unsigned char dart_remaining_time;
} DartRemainingTime;

/*------------------------REFEREE_ROBOT_CMD---- 0x42 -------------------*/
typedef struct {
    unsigned char robot_id;
    unsigned char robot_level;
    unsigned short remain_HP;
    unsigned short max_HP;
    unsigned short shooter_heat0_cooling_rate;
    unsigned short shooter_heat0_cooling_limit;
    unsigned short shooter_heat1_cooling_rate;
    unsigned short shooter_heat1_cooling_limit;
    unsigned char mains_power_gimbal_output: 1;
    unsigned char mains_power_chassis_output: 1;
    unsigned char mains_power_shooter_output: 1;
} GameRobotStatus;

typedef struct {
    unsigned short chassis_volt;
    unsigned short chassis_current;
    float chassis_power;
    unsigned short chassis_power_buffer;
    unsigned short shooter_heat0;
    unsigned short shooter_heat1;
} PowerHeatData;

typedef struct {
    float x;
    float y;
    float z;
    float yaw;
} GameRobotPos;

typedef struct {
    unsigned char power_rune_buff;
} Buff;

typedef struct {
    unsigned char energy_point;
    unsigned char attack_time;
} AerialRobotEnergy;

typedef struct {
    unsigned char armor_id: 4;
    unsigned char hurt_type: 4;
} RobotHurt;

typedef struct {
    unsigned char bullet_type;
    unsigned char bullet_freq;
    float bullet_speed;
} ShootData;

typedef struct {
    unsigned char bullet_remaining_num;
} BulletRemaining;

typedef struct {
    unsigned int rfid_status;
} RfidStatus;

typedef struct {
    unsigned char dart_launch_opening_status;
    unsigned char dart_attack_target;
    unsigned short target_change_time;
    unsigned char first_dart_speed;
    unsigned char second_dart_speed;
    unsigned char third_dart_speed;
    unsigned char fourth_dart_speed;
    unsigned short last_dart_launch_time;
    unsigned short operate_launch_cmd_time;
} DartClientCmd;

/*********************** Interactive data between robots----0x0301 ********************/
typedef enum {
    kRobotInteractiveCmdIdMin = 0x0200,
    kRobotInteractiveCmdIdMax = 0x02FF,
    kClientGraphicDeleteCmdId = 0x0100,
    kClientGraphicSingleCmdId = 0x0101,
    kClientGraphicDoubleCmdId = 0x0102,
    kClientGraphicFiveCmdId = 0x0103,
    kClientGraphicSevenCmdId = 0x0104,
    kClientCharacterCmdId = 0x0110,
} DataCmdId;

typedef enum {
    kRedHero = 1,
    kRedEngineer = 2,
    kRedStandard1 = 3,
    kRedStandard2 = 4,
    kRedStandard3 = 5,
    kRedAerial = 6,
    kRedSentry = 7,
    kRedRadar = 9,
    kBlueHero = 101,
    kBlueEngineer = 102,
    kBlueStandard1 = 103,
    kBlueStandard2 = 104,
    kBlueStandard3 = 105,
    kBlueAerial = 106,
    kBlueSentry = 107,
    kBlueRadar = 109,
} RobotId;

typedef enum {
    kRedHeroClientId = 0x0101,
    kRedEngineerClientId = 0x0102,
    kRedStandard1ClientId = 0x0103,
    kRedStandard2ClientId = 0x0104,
    kRedStandard3ClientId = 0x0105,
    kRedAerialClientId = 0x0106,
    kBlueHeroClientId = 0x0165,
    kBlueEngineerClientId = 0x0166,
    kBlueStandard1ClientId = 0x0167,
    kBlueStandard2ClientId = 0x0168,
    kBlueStandard3ClientId = 0x0169,
    kBlueAerialClientId = 0x016A,
} ClientId;

typedef struct {
    unsigned short data_cmd_id;
    unsigned short send_ID;
    unsigned short receiver_ID;
} StudentInteractiveHeaderData;

typedef enum {
    kAdd = 1,
    kModify = 2,
    kDelete = 3
} GraphicOperateType;

typedef struct {
    unsigned char graphic_name[3];
    unsigned int operate_type: 3;
    unsigned int graphic_type: 3;
    unsigned int layer: 4;
    unsigned int color: 4;
    unsigned int start_angle: 9;
    unsigned int end_angle: 9;
    unsigned int width: 10;
    unsigned int start_x: 11;
    unsigned int start_y: 11;
    unsigned int radius: 10;
    unsigned int end_x: 11;
    unsigned int end_y: 11;
} GraphicDataStruct;

typedef struct {
    FrameHeaderStruct tx_frame_header_;
    unsigned short cmd_id_;
    StudentInteractiveHeaderData graphic_header_data_;
    GraphicDataStruct graphic_data_struct_;
    unsigned short frame_tail_;
} DrawClientGraphicData;

typedef struct {
    FrameHeaderStruct tx_frame_header_;
    unsigned short cmd_id_;
    StudentInteractiveHeaderData graphic_header_data_;
    GraphicDataStruct graphic_data_struct_;
    unsigned char data_[30];
    unsigned short frame_tail_;
} DrawClientCharData;

/********************** Robot Interactive data ----0x0302 *******************************************/
typedef struct {
    unsigned char *data;
} RobotInteractiveData;

/********************** Robot command data ----0x0303 ***********************************************/
typedef struct {
    float target_position_x;
    float target_position_y;
    float target_position_z;
    unsigned char command_keyboard;
    unsigned short target_robot_ID;
} RobotCommand;

struct RefereeData {
    GameStatus game_status_;
    GameResult game_result_;
    GameRobotHp game_robot_hp_;
    DartStatus dart_status_;
    IcraBuffDebuffZoneStatus icra_buff_debuff_zone_status;
    EventData event_data_;
    SupplyProjectileAction supply_projectile_action_;
    RefereeWarning referee_warning_;
    DartRemainingTime dart_remaining_time_;
    GameRobotStatus game_robot_status_;
    PowerHeatData power_heat_data_;
    GameRobotPos game_robot_pos_;
    Buff buff_;
    AerialRobotEnergy aerial_robot_energy_;
    RobotHurt robot_hurt_;
    ShootData shoot_data_;
    BulletRemaining bullet_remaining_;
    RfidStatus rfid_status_;
    DartClientCmd dart_client_cmd_;
    StudentInteractiveHeaderData student_interactive_header_data_;
    GraphicDataStruct graphic_data_struct_;
    RobotInteractiveData robot_interactive_data_;
    RobotCommand robot_command_;
    int performance_system_; // Performance level system
};

extern struct RefereeData referee_data_;
void Referee_init();
void Referee_unpack(unsigned char byte);

#endif //PROGRAM_DEVICES_REFREE_H_

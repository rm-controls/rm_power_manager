//
// Created by Lao·Zhu on 2021/3/26.
//

#include "refree.h"
#include "string.h"
#include "stdbool.h"

struct RefereeData referee_data_;
bool flag = false;
int rx_len_;
UnpackData referee_unpack_obj;
const int kProtocolFrameLength = 128, kProtocolHeaderLength = sizeof(FrameHeaderStruct),
    kProtocolCmdIdLength = sizeof(unsigned short), kProtocolTailLength = 2;

unsigned char getCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length, unsigned char ucCRC8);
unsigned int verifyCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length);
void appendCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength);
unsigned int verifyCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);
unsigned short getCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength, unsigned short wCRC);
void appendCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength);

void Referee_getData(unsigned char *frame);
void Referee_unpack(unsigned char *rx_buffer);

void Referee_init() {
    referee_unpack_obj.index = 0;
    referee_unpack_obj.unpack_step = kStepHeaderSof;
}

void Referee_read() {
//    rx_len_ = rx_buffer.size();
//    Referee_unpack(rx_buffer);
}

void Referee_unpack(unsigned char *rx_buffer) {
    int num = 0;
    unsigned char byte;
    while (rx_len_) {
        byte = rx_buffer[num];
        switch (referee_unpack_obj.unpack_step) {
            case kStepHeaderSof:
                if (byte == 0xA5) {
                    referee_unpack_obj.unpack_step = kStepLengthLow;
                    referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                } else
                    referee_unpack_obj.index = 0;
                break;
            case kStepLengthLow:referee_unpack_obj.data_len = byte;
                referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                referee_unpack_obj.unpack_step = kStepLengthHigh;
                break;
            case kStepLengthHigh:referee_unpack_obj.data_len |= (byte << 8);
                referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                if (referee_unpack_obj.data_len < kProtocolFrameLength - kProtocolCmdIdLength - kProtocolCmdIdLength)
                    referee_unpack_obj.unpack_step = kStepFrameSeq;
                else {
                    referee_unpack_obj.unpack_step = kStepHeaderSof;
                    referee_unpack_obj.index = 0;
                }
                break;
            case kStepFrameSeq:referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                referee_unpack_obj.unpack_step = kStepHeaderCrc8;
                break;
            case kStepHeaderCrc8:referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                if (referee_unpack_obj.index == (kProtocolHeaderLength - 1)) {
                    if (verifyCRC8CheckSum(referee_unpack_obj.protocol_packet, kProtocolHeaderLength))
                        referee_unpack_obj.unpack_step = kStepDataCrc16;
                    else {
                        referee_unpack_obj.unpack_step = kStepHeaderSof;
                        referee_unpack_obj.index = 0;
                    }
                }
                break;
            case kStepDataCrc16:
                if (referee_unpack_obj.index
                    < (kProtocolHeaderLength + kProtocolTailLength + kProtocolCmdIdLength + referee_unpack_obj.data_len
                        - 1))
                    referee_unpack_obj.protocol_packet[referee_unpack_obj.index++] = byte;
                if (referee_unpack_obj.index
                    >= (kProtocolHeaderLength + kProtocolTailLength + kProtocolCmdIdLength
                        + referee_unpack_obj.data_len - 1)) {
                    referee_unpack_obj.unpack_step = kStepHeaderSof;
                    referee_unpack_obj.index = 0;
                    if (verifyCRC16CheckSum(referee_unpack_obj.protocol_packet, kProtocolHeaderLength +
                        kProtocolTailLength + kProtocolCmdIdLength
                        + referee_unpack_obj.data_len)) {
                        Referee_getData(referee_unpack_obj.protocol_packet);
                        memset(referee_unpack_obj.protocol_packet, 0, sizeof(referee_unpack_obj.protocol_packet));
                        referee_unpack_obj.unpack_step = kStepHeaderSof;
                    }
                }
                break;
            default:referee_unpack_obj.unpack_step = kStepHeaderSof;
                memset(referee_unpack_obj.protocol_packet, 0, sizeof(referee_unpack_obj.protocol_packet));
                referee_unpack_obj.index = 0;
                num = 0;
                break;
        }
        num++;
        rx_len_--;
    }
    referee_unpack_obj.unpack_step = kStepHeaderSof;
    memset(referee_unpack_obj.protocol_packet, 0, sizeof(referee_unpack_obj.protocol_packet));
}

void Referee_getData(unsigned char *frame) {
    unsigned short cmd_id = 0;
    unsigned char index = 0;
    index += (sizeof(FrameHeaderStruct) - 1);
    memcpy(&cmd_id, frame + index, sizeof(unsigned short));
    index += sizeof(unsigned short);
    switch (cmd_id) {
        case kGameStatusCmdId:memcpy(&referee_data_.game_status_, frame + index, sizeof(GameStatus));
            break;
        case kGameResultCmdId:memcpy(&referee_data_.game_result_, frame + index, sizeof(GameResult));
            break;
        case kGameRobotHpCmdId:memcpy(&referee_data_.game_robot_hp_, frame + index, sizeof(GameRobotHp));
            break;
        case kDartStatusCmdId:memcpy(&referee_data_.dart_status_, frame + index, sizeof(DartStatus));
            break;
        case kIcraZoneStatusCmdId:
            memcpy(&referee_data_.icra_buff_debuff_zone_status,
                   frame + index,
                   sizeof(IcraBuffDebuffZoneStatus));
            break;
        case kFieldEventsCmdId:memcpy(&referee_data_.event_data_, frame + index, sizeof(EventData));
            break;
        case kSupplyProjectileActionCmdId:
            memcpy(&referee_data_.supply_projectile_action_,
                   frame + index,
                   sizeof(SupplyProjectileAction));
            break;
        case kRefereeWarningCmdId:memcpy(&referee_data_.referee_warning_, frame + index, sizeof(RefereeWarning));
            break;
        case kDartRemainingCmdId:
            memcpy(&referee_data_.dart_remaining_time_,
                   frame + index,
                   sizeof(DartRemainingTime));
            break;
        case kRobotStatusCmdId:
            memcpy(&referee_data_.game_robot_status_,
                   frame + index,
                   sizeof(GameRobotStatus));
            break;
        case kPowerHeatDataCmdId:memcpy(&referee_data_.power_heat_data_, frame + index, sizeof(PowerHeatData));
            referee_data_.power_heat_data_.chassis_volt =
                referee_data_.power_heat_data_.chassis_volt / 1000;
            referee_data_.power_heat_data_.chassis_current =
                referee_data_.power_heat_data_.chassis_current / 1000;
            break;
        case kRobotPosCmdId:memcpy(&referee_data_.game_robot_pos_, frame + index, sizeof(GameRobotPos));
            break;
        case kBuffCmdId:memcpy(&referee_data_.buff_, frame + index, sizeof(Buff));
            break;
        case kAerialRobotEnergyCmdId:
            memcpy(&referee_data_.aerial_robot_energy_,
                   frame + index,
                   sizeof(AerialRobotEnergy));
            break;
        case kRobotHurtCmdId:memcpy(&referee_data_.robot_hurt_, frame + index, sizeof(RobotHurt));
            break;
        case kShootDataCmdId:memcpy(&referee_data_.shoot_data_, frame + index, sizeof(ShootData));
            break;
        case kBulletRemainingCmdId:memcpy(&referee_data_.bullet_remaining_, frame + index, sizeof(BulletRemaining));
            break;
        case kRobotRfidStatusCmdId:memcpy(&referee_data_.rfid_status_, frame + index, sizeof(RfidStatus));
            break;
        case kDartClientCmdId:memcpy(&referee_data_.dart_client_cmd_, frame + index, sizeof(DartClientCmd));
            break;
        case kRobotCommandCmdId:memcpy(&referee_data_.robot_command_, frame + index, sizeof(RobotCommand));
            break;
        default:break;
    }
}

/******************* CRC Verify *************************/
unsigned char getCRC8CheckSum(unsigned char *pch_message,
                              unsigned int dw_length,
                              unsigned char ucCRC8) {
    unsigned char uc_index;
    while (dw_length--) {
        uc_index = ucCRC8 ^ (*pch_message++);
        ucCRC8 = CRC8_table[uc_index];
    }
    return (ucCRC8);
}

/*
** Descriptions: CRC8 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
unsigned int verifyCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length) {
    unsigned char ucExpected;
    if ((pch_message == NULL) || (dw_length <= 2)) {
        return 0;
    }
    ucExpected = getCRC8CheckSum(pch_message, dw_length - 1, CRC8_INIT);
    return (ucExpected == pch_message[dw_length - 1]);
}

/*
** Descriptions: append CRC8 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void appendCRC8CheckSum(unsigned char *pchMessage, unsigned int dwLength) {
    unsigned char ucCRC;
    if ((pchMessage == NULL) || (dwLength <= 2)) return;
    ucCRC = getCRC8CheckSum((unsigned char *) pchMessage, dwLength - 1, CRC8_INIT);
    pchMessage[dwLength - 1] = ucCRC;
}

/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
unsigned short getCRC16CheckSum(unsigned char *pchMessage,
                                unsigned int dwLength,
                                unsigned short wCRC) {
    unsigned char chData;
    if (pchMessage == NULL) {
        return 0xFFFF;
    }
    while (dwLength--) {
        chData = *pchMessage++;
        (wCRC) =
            ((unsigned short) (wCRC) >> 8)
                ^ wCRC_table[((unsigned short) (wCRC) ^ (unsigned short) (chData))
                    & 0x00ff];
    }
    return wCRC;
}

/*
** Descriptions: CRC16 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
unsigned int verifyCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength) {
    unsigned short wExpected;
    if ((pchMessage == NULL) || (dwLength <= 2)) {
        return 0;
    }
    wExpected = getCRC16CheckSum(pchMessage, dwLength - 2, CRC16_INIT);
    return ((wExpected & 0xff) == pchMessage[dwLength - 2]
        && ((wExpected >> 8) & 0xff) == pchMessage[dwLength - 1]);
}

/*
** Descriptions: append CRC16 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void appendCRC16CheckSum(unsigned char *pchMessage, unsigned int dwLength) {
    unsigned short wCRC;
    if ((pchMessage == NULL) || (dwLength <= 2)) {
        return;
    }
    wCRC = getCRC16CheckSum((unsigned char *) pchMessage, dwLength - 2, CRC16_INIT);
    pchMessage[dwLength - 2] = (unsigned char) (wCRC & 0x00ff);
    pchMessage[dwLength - 1] = (unsigned char) ((wCRC >> 8) & 0x00ff);
}

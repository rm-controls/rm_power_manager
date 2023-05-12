//
// Created by LaoZhu on 2023/5/3.
//

#include "referee.h"
#include "system.h"
#include "verify.h"
#include "string.h"
#include "dma.h"

typedef struct {
  unsigned char dummy_byte;
  unsigned char package_sequence;
  unsigned short data_length;
} referee_package_header_t;

typedef struct {
  referee_package_header_t header;
  unsigned short cmd_id;
  unsigned short data_counter;
  unsigned char data[127];
  unsigned char crc16_low;
} referee_package_t;

static unsigned char referee_process_status = 0, referee_header_buffer[5] = {0xA5, [1 ... 4]=0};
static referee_package_t current_package = {0};
referee_info_t referee_info;

static void referee_process_struct(referee_package_t *package);

const unsigned short k_game_status_cmd = 0x0001;
const unsigned short k_game_robot_status_cmd = 0x0201;
const unsigned short k_power_heat_data_cmd = 0x0202;

const unsigned int k_game_status_size = 11;
const unsigned int k_game_robot_status_size = 27;
const unsigned int k_power_heat_data_size = 16;

static struct GameStatus_t {
  uint8_t game_type: 4;
  uint8_t game_progress: 4;
  uint16_t stage_remain_time;
  uint64_t time_stamp;
} game_status;

static struct GameRobotStatus_t {
  uint8_t robot_id;
  uint8_t robot_level;
  uint16_t remain_hp;
  uint16_t max_hp;
  uint16_t shooter_id1_17mm_cooling_rate;
  uint16_t shooter_id1_17mm_cooling_limit;
  uint16_t shooter_id1_17mm_speed_limit;
  uint16_t shooter_id2_17mm_cooling_rate;
  uint16_t shooter_id2_17mm_cooling_limit;
  uint16_t shooter_id2_17mm_speed_limit;
  uint16_t shooter_id1_42mm_cooling_rate;
  uint16_t shooter_id1_42mm_cooling_limit;
  uint16_t shooter_id1_42mm_speed_limit;
  uint16_t chassis_power_limit;
  uint8_t mains_power_gimbal_output: 1;
  uint8_t mains_power_chassis_output: 1;
  uint8_t mains_power_shooter_output: 1;
} game_robot_status;

static struct PowerHeatData_t {
  uint16_t chassis_volt;
  uint16_t chassis_current;
  float chassis_power;
  uint16_t chassis_power_buffer;
  uint16_t shooter_id1_17mm_cooling_heat;
  uint16_t shooter_id2_17mm_cooling_heat;
  uint16_t shooter_id1_42mm_cooling_heat;
} power_heat_data;

static unsigned int last_time_tick = 0;
unsigned int referee_available(void) {
    return ((HAL_GetTick() - last_time_tick) < 1000);
}

void referee_process_buffer(const unsigned char *buffer) {
    int frame_header_position = -1;
    unsigned short crc16_expected;
    for (int counter = 0; counter < UART_DMA_BUFFER_SIZE; ++counter) {
        switch (referee_process_status) {
            case 0:if (buffer[counter] == 0xA5) referee_process_status = 1;
                break;
            case 1:current_package.header.data_length = buffer[counter];
                referee_header_buffer[1] = buffer[counter];
                referee_process_status = 2;
                break;
            case 2:current_package.header.data_length |= (buffer[counter] << 8);
                current_package.header.data_length = current_package.header.data_length;
                referee_header_buffer[2] = buffer[counter];
                referee_process_status = 3;
                break;
            case 3:current_package.header.package_sequence = buffer[counter];
                referee_header_buffer[3] = buffer[counter];
                referee_process_status = 4;
                break;
            case 4:
                if (get_crc8_value(referee_header_buffer, 4) == buffer[counter]) {
                    referee_process_status = 5;
                    frame_header_position = counter - 4;
                    referee_header_buffer[4] = buffer[counter];
                } else
                    goto clear_all_referee_data;
                break;
            case 5:current_package.cmd_id = buffer[counter];
                referee_process_status = 6;
                break;
            case 6:current_package.cmd_id |= (buffer[counter] << 8);
                current_package.data_counter = 0;
                referee_process_status = 7;
                if (current_package.cmd_id != k_game_robot_status_cmd ||
                    current_package.cmd_id != k_game_status_cmd ||
                    current_package.cmd_id != k_power_heat_data_cmd)
                    referee_process_status = 10;
                break;
            case 7:current_package.data[current_package.data_counter] = buffer[counter];
                current_package.data_counter++;
                if (current_package.data_counter == current_package.header.data_length)
                    referee_process_status = 8;
                break;
            case 8:current_package.crc16_low = buffer[counter];
                referee_process_status = 9;
                break;
            case 9:
                crc16_expected = get_crc16_value(referee_header_buffer,
                                                 current_package.cmd_id,
                                                 current_package.data,
                                                 current_package.header.data_length);
                if ((crc16_expected & 0xff) == current_package.crc16_low
                    && ((crc16_expected >> 8) & 0xff) == buffer[counter])
                    referee_process_struct(&current_package);
                goto clear_all_referee_data;
            case 10:current_package.data_counter++;
                if (current_package.data_counter == current_package.header.data_length + 2)
                    goto clear_all_referee_data;
                break;
            default:
            clear_all_referee_data:
                referee_process_status = 0;
                frame_header_position = -1;
                memset(&referee_header_buffer[1], 0x00, 4);
                memset(&current_package, 0x00, sizeof(referee_package_t));
                break;
        }
    }
    mdma_transmit_buffer(buffer, frame_header_position);
}

static void referee_process_struct(referee_package_t *package) {
    switch (package->cmd_id) {
        case k_game_status_cmd:memcpy(&game_status, package->data, k_game_status_size);
            referee_info.game_progress = game_status.game_progress;
            referee_info.timestamp = game_status.time_stamp;
            break;
        case k_game_robot_status_cmd:memcpy(&game_robot_status, package->data, k_game_robot_status_size);
            referee_info.chassis_power_limit = game_robot_status.chassis_power_limit;
            break;
        case k_power_heat_data_cmd:memcpy(&power_heat_data, package->data, k_power_heat_data_size);
            referee_info.chassis_power = power_heat_data.chassis_power;
            referee_info.chassis_voltage = (float) power_heat_data.chassis_volt / 1000.0f;
            referee_info.chassis_current = (float) power_heat_data.chassis_current / 1000.0f;
            referee_info.chassis_power_buffer = power_heat_data.chassis_power_buffer;
            break;
        default:break;
    }
    last_time_tick = HAL_GetTick();
}

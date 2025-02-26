/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-10 21:20
 * @FilePath: /ESP32_Project/main/Init_Config/BaseConfig.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#ifndef _INITCONFIG_H
#define _INITCONFIG_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"

//add by tian
#include "web_config.h"
#include "app_task.h"
#include "speech_if.h"
#include "cam_task.h"

#define ESP_FS_PATH        "/spiffs"
#define ESP_SD_FS_PATH     "/sdcard"
//#define ESP_FS_PIC_PATH    "/sdcard/picture"

typedef struct {
    char ssid[32];
    uint8_t ssidlen;
    char pass[32];
    uint8_t passlen;
} read_wifi_buf_t;

read_wifi_buf_t read_wifi_buf;

typedef struct {
    wifi_sta_config_t sta;
    wifi_ap_config_t ap;
} sta_ap_wifi_config_t;

extern read_wifi_buf_t read_wifi_buf;
extern sta_ap_wifi_config_t sta_ap_wifi_config;

void Init_Config(void);
esp_err_t mount_storage(const char *base_path);
esp_err_t sdcard_init(const char* base_path);
esp_err_t read_wifi_from_nvs(void);
esp_err_t wifi_ap_sta_init(void);

#endif

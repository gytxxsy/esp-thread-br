/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_check.h"

esp_err_t wifi_config_init(void);
esp_err_t wifi_config_set_ssid(const char *ssid);
esp_err_t wifi_config_get_ssid(char *ssid);
esp_err_t wifi_config_set_password(const char *password);
esp_err_t wifi_config_get_password(char *password);
esp_err_t wifi_config_command_register(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

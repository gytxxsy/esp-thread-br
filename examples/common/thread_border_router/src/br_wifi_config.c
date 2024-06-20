/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "esp_check.h"
#include "esp_console.h"
#include "br_wifi_config.h"
#include "nvs.h"
#include "argtable3/argtable3.h"

#define TAG "wifi_config"
#define SSIDKEY "ssid"
#define PASSWORDKEY "password"
#define SSIDMAXLEN 32
#define PASSWORDMAXLEN 64

static nvs_handle_t s_wifi_config_nvs_handle;

esp_err_t wifi_config_init(void)
{
    esp_err_t err = nvs_open("wifi_config", NVS_READWRITE, &s_wifi_config_nvs_handle);
    ESP_RETURN_ON_ERROR(err, TAG, "Failed to open wifi_config NVS namespace (0x%x)", err);
    return ESP_OK;
}

esp_err_t wifi_config_set_ssid(const char *ssid)
{
    ESP_RETURN_ON_FALSE((s_wifi_config_nvs_handle != 0), ESP_FAIL, TAG, "wifi_config NVS handle is invalid.");
    ESP_RETURN_ON_ERROR(nvs_set_str(s_wifi_config_nvs_handle, SSIDKEY, ssid), TAG, "No buffers for ssid");
    ESP_RETURN_ON_ERROR(nvs_commit(s_wifi_config_nvs_handle), TAG, "wifi_config NVS handle shut down when setting ssid");
    return ESP_OK;
}

esp_err_t wifi_config_get_ssid(char *ssid)
{
    
    ESP_RETURN_ON_FALSE((sizeof(ssid) != SSIDMAXLEN), ESP_FAIL, TAG, "The buffer size is not enough to get ssid");

    char ssid_str[SSIDMAXLEN] = {0};
    size_t length = SSIDMAXLEN;
    ESP_RETURN_ON_FALSE((s_wifi_config_nvs_handle != 0), ESP_FAIL, TAG, "wifi_config NVS handle is invalid.");
    ESP_RETURN_ON_ERROR(nvs_get_str(s_wifi_config_nvs_handle, SSIDKEY, ssid_str, &length), TAG, "Fail to get ssid");
    strcpy(ssid, ssid_str);
    return ESP_OK;
}

esp_err_t wifi_config_set_password(const char *password)
{
    ESP_RETURN_ON_FALSE((s_wifi_config_nvs_handle != 0), ESP_FAIL, TAG, "wifi_config NVS handle is invalid.");
    ESP_RETURN_ON_ERROR(nvs_set_str(s_wifi_config_nvs_handle, PASSWORDKEY, password), TAG, "No buffers for password");
    ESP_RETURN_ON_ERROR(nvs_commit(s_wifi_config_nvs_handle), TAG, "wifi_config NVS handle shut down when setting password");
    return ESP_OK;
}

esp_err_t wifi_config_get_password(char *password)
{
    ESP_RETURN_ON_FALSE((sizeof(password) != PASSWORDMAXLEN), ESP_FAIL, TAG, "The buffer size is not enough to get password");

    char password_str[PASSWORDMAXLEN] = {0};
    size_t length = PASSWORDMAXLEN;
    ESP_RETURN_ON_FALSE((s_wifi_config_nvs_handle != 0), ESP_FAIL, TAG, "wifi_config NVS handle is invalid.");
    ESP_RETURN_ON_ERROR(nvs_get_str(s_wifi_config_nvs_handle, PASSWORDKEY, password_str, &length), TAG, "Fail to get password");
    strcpy(password, password_str);
    return ESP_OK;
}

static int wifi_config_command(int argc, char **argv)
{
    if (argc != 3) {
        ESP_LOGE(TAG, "Invalid args");
        return -1;
    }
    if (wifi_config_set_ssid(argv[1]) == ESP_OK && wifi_config_set_password(argv[2]) == ESP_OK) {
        ESP_LOGI(TAG, "Set new wifi ssid and password successfully, restarting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "Fail to set new wifi ssid and password, please try again");
        return -1;
    }
    
    return 0;
}

static struct {
    struct arg_str *ssid;
    struct arg_str *password;
    struct arg_end *end;
} wifi_config_args;

esp_err_t wifi_config_command_register(void)
{
    wifi_config_args.ssid = arg_str1(NULL, NULL, "<ssid>", "ssid of Wi-Fi");
    wifi_config_args.password = arg_str1(NULL, NULL, "<password>", "password of Wi-Fi");
    wifi_config_args.end = arg_end(2);

    esp_console_cmd_t command = {
        .command = "newwifi",
        .help = "Set the ssid and password of Wi-Fi, format: esp newwifi <ssid> <password>",
        .func = &wifi_config_command,
        .argtable = &wifi_config_args
    };
    return esp_console_cmd_register(&command);
}

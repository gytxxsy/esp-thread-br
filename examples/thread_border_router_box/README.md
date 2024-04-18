# OpenThread Border Router Example

## Overview

This example is an extension based on the `basic_thread_border_router` example, demonstrating the Thread credential sharing feature. The example will connect to a pre-configured Wi-Fi network and form a Thread network automatically. The epskc could be geneated from the touch screen, which can be used to establish a dtls session with a Thread commissioner, The Thread credentials could be fetched or configured over the dtls session.

## How to use example

### Hardware Required

This example requires a ESP32-S3-BOX-3B and a ESP32-H2, the hardware connections are as follows:

ESP32-S3-BOX-3B pin | ESP32-H2 pin
--------------------|-------------
           3V3      |      3V3
           GND      |      G
           G38      |      TX
           G41      |      RX
           G42      |      RST
           G40      |      BOOT(GPIO9)

### Configure the project

ESP32-S3 is the SoC on ESP32-S3-BOX-3B, so set target to esp32s3:

```
idf.py set-target esp32s3
```

The example has been configured with the necessary settings in the sdkconfig.defaults, and most configurations do not need to be modified through idf.py menuconfig. You can modify the following parameters according to your actual situation:

1. Wi-Fi SSID and PSK.
2. Initial OpenThread dataset, such as the `networkkey` and `channel`.

### Create the RCP firmware image

The border router supports updating the RCP upon boot.

First build the [ot_rcp](https://github.com/gytxxsy/esp-idf/tree/demo/demo_for_ot_epskc/examples/openthread/ot_rcp) example in IDF. In the building process, the built RCP image will be automatically packed into the border router firmware.

### Build, Flash, and Run

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT build flash monitor
```

Upon startup, the device will join the pre-configured WiFi network, and join the Thread network based on the current OpenThread NVS data. Subsequently, it will publish the meshcop service in the Wi-Fi network. Once these work are completed, the device will display its web server URL on the main interface (please refer to [ESP Thread Border Router WEB GUI](https://docs.espressif.com/projects/esp-thread-br/en/latest/codelab/web-gui.html)) and have the following two clickable buttons:

1. `factoryreset`: This button is used to clear the current OpenThread NVS data and restart the device. After rebooting, the device will join the OpenThread network configured by the user through menuconfig.

2. `generate epskc`: This button is used to generate an OpenThread ephemeral key and publish the meshcop-e service in the Wi-Fi network. The meshcop-e service will include the device's IP address and port number.

The ephemeral key can be used to connect an external commissioner to a Thread Border Router via a WiFi network. Once the connection is established, the commissioner will be able to obtain or configure parameters for the Thread Border Router and its child nodes through the WiFi network.

### Use a pre-built binary file

This example provides a pre-built binary file `br_box_merged_flash.bin`. You can directly use the esptool tool to flash it onto the device using the following command:

```
esptool.py write_flash 0x0 br_box_merged_flash.bin
```
Pre-configured Wi-Fi and Thread parameters of this binary file can be found in sdkconfig.defaults.

**Note**: This example is an test version intended for experimental purposes only. We will provide official versions based on ESP32-S3-BOX-3B and ESP32-H2 in future updates.
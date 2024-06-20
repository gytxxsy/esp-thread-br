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

### IDF Required

Please use this IDF version: https://github.com/gytxxsy/esp-idf/tree/demo/demo_for_ot_epskc.

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

## Usage Example
### Device and Testing Environment Requirements
1. A phone that supports `Google Play Services` and has [ot_config.apk](../tools_for_Android/ot_config.apk) and [ot-helper.apk](../tools_for_Android/ot-helper.apk) installed.

2. Wi-Fi network that supports Internet access and 2.4G. In the following text, we assume the Wi-Fi name is `ESP-WIFI`.

3. A computer with a web browser installed (Google Chrome is recommended).

4. Two sets of ESP32-S3-BOX-3B + ESP32-H2 devices, flashed with the thread_border_router_box example program (assuming ESP-WIFI and its password are used as the Wi-Fi configuration, and different default Thread network parameters are used). In the following text, we will refer to these two sets of devices as `BR1` and `BR2`.

### Standard Usage Procedure
***Note: This procedure is only a reference. Please use it according to your actual needs.***

1. Connect both your phone and computer to `ESP-WIFI`.

2. Start `BR1` and `BR2`. They will automatically connect to `ESP-WIFI` and form Thread networks(the network parameters are determined by the parameters stored in the devices). It is recommended to open connections with BR1 and BR2 on the computer using monitor (referred to as `monitor BR1` and `monitor BR2` below) to facilitate the observation of necessary information.

3. After waiting for a while (normally no more than 1 minute), the device initialization will be completed. At this time, click the button `factoryreset` that appears at the top right corner of the screens of BR1 and BR2 to reboot the devices.

4. After rebooting, the devices will automatically connect to `ESP-WIFI` and form different Thread networks (the network parameters are determined by the default Thread network parameters). When `generate epskc` appears on the device screens, the initialization is complete. At this point, enter `dataset active` in `monitor BR1` and `monitor BR2` to see that `BR1` and `BR2` have different Thread datasets.

5. Now, you can discover the `_meshcop._udp` service on your computer via `avahi-browse -rt _meshcop._udp`(both `BR1` and `BR2` will publish this service on the Wi-Fi network), but the `_meshcop-e._udp` service will not be discoverable (because `BR1` and `BR2` have not yet published this service on the Wi-Fi network).

6. Open a web browser and visit the `ESP Border Router WEB GUI` using the URL displayed on `BR1`: "http://xxx.xxx.xxx.xxx:80/index.html". Click `Start Topology` to find that only `BR1` is present in the Thread network.

7. On your phone, clear the cache data of Google Play Services: `Settings > Apps > All Apps > Google Play services > Storage & cache > Manage space > CLEAR ALL DATA`.

8. Open the installed `ot-helper.apk` and click button `Get Preferred Network Credentials`. The returned information will indicate that there is no stored dataset on the device.

9. Open the installed `ot-config.apk`. You will find two devices (`BR1` and `BR2`) in the device list, both displaying `IN Administration Mode: NO`.

10. Click `generate epskc` on the screen of BR1. A password (9 random digits) will appear on BR1's screen. Now, you can discover the `_meshcop-e._udp` service published by `BR1` on your computer. Then the ot-config.apk device list on your phone will show BR1 as `In Administration Mode: YES`.

11. In the ot-config.apk device list, select BR1 and click `RETRIEVE DATASET`. In the input box that appears, enter the password shown on BR1's screen and click `RETRIEVE DATASET`. The phone will retrieve the dataset stored on `BR1`. BR1's screen will then re-display `generate epskc`.

12. Open `ot-helper.apk` and click button `Get Preferred Network Credentials`. The returned prompt will indicate that a dataset has been stored on the device, and it matches BR1's dataset.

13. Click `generate epskc` on the screen of BR2. A password (9 random digits) will appear on BR2's screen. Now, you can discover the `_meshcop-e._udp` service published by `BR2` on your computer. Then the ot-config.apk device list on your phone will show BR2 as `In Administration Mode: YES`.

14. In the ot-config.apk device list, select BR2 and click `SET DATASET`. In the input box that appears, enter the password shown on BR2's screen and click confirm. Then click the `SET DATASET`, `USE PREFERRED CREDENTIALS` and wait. The phone will set the stored dataset into `BR2`. BR2's screen will then re-display `generate epskc`.

15. At this point, enter `dataset pending` in `monitor BR2`. You will see that BR2's pending dataset has been set and matches BR1's active dataset. After about 30 seconds, re-enter `dataset pending` to see that BR2's pending dataset has disappeared. Enter `dataset active` to find that BR2's active dataset has been reset and matches BR1's active dataset.

16. Now, `BR1` and `BR2` are in the same Thread network. However, due to different partition IDs, it will take some time for partition merging. After merging is complete, `BR1` or `BR2` will become a `child` or `router`.

17. In the `ESP Border Router WEB GUI`, click `Start Topology` again to see that there are now two devices (`BR1` and `BR2`) in the Thread network.


### Notes
1. It is recommended to click `factoryreset` on the BR device screen before each formal test to prevent the devices from directly joining the same Thread network when starting up.

2. Before each formal test, make sure to clear the cached Thread dataset on the phone. To do this: go to `Settings > Apps > All Apps > Google Play services > Storage & cache > Manage space > CLEAR ALL DATA`.

3. If the BR device screen fails to initialize and displays "Invalid wifi", it means that it cannot connect to the Wi-Fi network. In this case, open the monitor connected to the device and enter `esp newwifi <ssid> <password>` to set up a usable Wi-Fi network.

4. `factoryreset` will only clear the Thread dataset and will not delete the saved Wi-Fi SSID and password. Wi-Fi can only be changed via the command `esp newwifi <ssid> <password>`.

5. If you encounter any serious issues, you can reflash the devices with `br_box_device1.bin` and `br_box_device2.bin` using the following command:

```bash
> esptool.py --port <PORT> write_flash 0x0 br_box_device1.bin
> esptool.py --port <PORT> write_flash 0x0 br_box_device2.bin
```

The default Wi-Fi configurations are as follows:
* br_box_device1.bin and br_box_device2.bin: 
```
ssid:       ESP-WIFI
password:   espressif
```


The default Thread dataset are as follows:

* br_box_device1.bin: 
```
Active Timestamp: 1
Channel: 21
Channel Mask: 0x07fff800
Ext PAN ID: dead00beef00cafe
Mesh Local Prefix: fd00:db8:a0:0::/64
Network Key: cdc0de5d3b8777d9f6e272f209dfca38
Network Name: ESP-OTBR1
PAN ID: 0x1234
PSKc: 104810e2315100afd6bc9215a6bfac53
Security Policy: 672 onrc 0
```

* br_box_device2.bin: 
```
Active Timestamp: 1
Channel: 15
Channel Mask: 0x07fff800
Ext PAN ID: 26fab18486b45745
Mesh Local Prefix: fdec:62de:52c7:ff45::/64
Network Key: e4781660e18f51697824f4e176fd16fd
Network Name: ESP-OTBR2
PAN ID: 0x4321
PSKc: 4efee0f6bbb7a2c4b355f1a30a42d5bb
Security Policy: 672 onrc 0
```
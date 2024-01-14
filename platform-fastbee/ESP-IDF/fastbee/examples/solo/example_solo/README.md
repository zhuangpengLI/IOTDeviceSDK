# 通过串口命令配置解决方案

### 介绍
`Example Solo` 为客户提供串口命令配置模组连接蜂信物联网平台, 即可以:  
- 支持通过命令设置一键配网（零配）或者设备热点配网
- 支持通过命令使模组恢复出厂设置
- 支持通过命令配置模组四元组信息
- 支持通过命令擦除动态注册的 DeviceSecret 信息
- 支持通过命令重启模组
- 支持通过命令配置模组无线 SSID 和 Password

另外该示例还支持如下功能：
- 支持控制设备
- 支持 <天猫精灵智能音箱> 控制设备
- 支持 <天猫精灵智能音箱> 配网并控制设备
- 支持 LED 控制(开关)
- 支持 OTA 升级

需要定制相关功能可以联系微信：a7491772
邮箱：908349383@qq.com

### 解决方案部署
#### 1.参考 [README](../../../README.md) 文档进行硬件准备、环境搭建、SDK 准备


#### 2.下载本工程
   ```
    git clone https://gitee.com/zhuangpengli/IOTDeviceSDK
    cd IOTDeviceSDK/platform-fastbee/ESP-IDF/fastbee
   ```

#### 3.编译 `example solo` 并烧录运行
```
cd examples/solo/example_solo
make chip=esp32 defconfig 或者 make chip=esp8266 defconfig
make -j8 flash monitor
```
使用 esp32s2 请参考根目录 README。

#### 4.命令手册

```
#配置四元组信息
linkkey <ProductKey> <DeviceName> <DeviceSecret> <ProductSecret>
示例：linkkey a1QdKm5axuO test_006 J4R9yh47YjAHdcIIo6M7P01MSfErusWG sUBuz3XD13sP083P
注意：顺序不能颠倒，每个参数之间使用空格隔开。如果不想设置其中的某一个，请用空格代替该参数值。
      设备第一次上电后，必须先配置四元组信息。
```

```
#配置一键配网，零配
active_awss
注意：设备默认是一键配网，配网有效时间为60s, 配网过期后，可以执行该命令重新进行配网。
```

```
#配置设备热点配网
dev_ap
注意：设备热点配网无有时间，可以一直配网，如果要切换一键配网，执行 active_awss。
```

```
#绕过配网配置无线 SSID 和 Password
netmgr connect <SSID> <Password>
示例： netmgr connect myssid 12345678
```

```
#恢复出厂设置
reset
注意：恢复出厂设置只会清除配网信息，不会擦除四元组信息。
```

```
#重启模组
reboot
注意：重启模组不会清除配网信息，不会擦除四元组信息。
```

```
#擦除动态注册的DeviceSecret
kv_clear
注意：该功能是为了动态注册后的模组重新写入其他四元组后可以正常使用，而不需要擦写模组。
```

#### 5.串口配置

||ESP32 & ESP32S2|ESP8266|
|:-----:|:-----:|:-----:|
|CMD_RX|RXD|RXD|
|CMD_TX|TXD|TXD|
|LOG_RX|RXD|RXD|
|LOG_TX|TXD|TXD|
|BAUD_RATE|115200|115200|

按照上表PIN脚接入对应串口，VCC 3.3V, GND, TX, RX。

#### 6.设备第一次运行时

配置四元组信息

```
[17:04:50.249][0;32mI (367) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE[0m
[17:04:50.249][0;32mI (367) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE[0m
[17:04:50.260]I (397) wifi:wifi firmware version: 29701b4
[17:04:50.282]I (397) wifi:config NVS flash: enabled
[17:04:50.282]I (397) wifi:config nano formating: disabled
[17:04:50.293]I (397) wifi:Init dynamic tx buffer num: 32
[17:04:50.293]I (407) wifi:Init data frame dynamic rx buffer num: 32
[17:04:50.293]I (407) wifi:Init management frame dynamic rx buffer num: 32
[17:04:50.301]I (417) wifi:Init management short buffer num: 32
[17:04:50.310]I (417) wifi:Init static rx buffer size: 1600
[17:04:50.310]I (427) wifi:Init static rx buffer num: 10
[17:04:50.310]I (427) wifi:Init dynamic rx buffer num: 32
[17:04:50.322][0;32mI (567) phy: phy_version: 4180, cb3948e, Sep 12 2019, 16:39:13, 0, 0[0m
[17:04:50.459]I (567) wifi:mode : sta (24:0a:c4:d6:d6:60)
[17:04:50.459][prt] log level set as: [ 4 ]
[17:04:50.459][0;32mI (567) uart: queue free spaces: 1[0m
[17:04:50.459]....................................................
[17:04:50.482]          DeviceName : 
[17:04:50.482]        DeviceSecret : 
[17:04:50.482]          ProductKey : 
[17:04:50.482]       ProductSecret : 
[17:04:50.482]....................................................
[17:04:50.482][0;31mE (597) app_entry: Please first input four config[0m
[17:04:50.482]
[17:07:48.598][0;32mI (178717) app_entry: ProductKey: a1QdKm5axuO[0m
[17:07:48.598][0;32mI (178717) app_entry: DeviceName: test_006[0m
[17:07:48.598][0;32mI (178717) app_entry: DeviceSecret: J4R9yh47YjAHdcIIo6M7P01MSfErusWG[0m
[17:07:48.612][0;32mI (178727) app_entry: ProductSecret: sUBuz3XD13sP083P[0m
[17:07:48.612]
```

串口执行：`linkkey a1QdKm5axuO test_006 J4R9yh47YjAHdcIIo6M7P01MSfErusWG sUBuz3XD13sP083P`

配置设备热点配网

```
[17:07:48.598][0;32mI (178717) app_entry: ProductKey: a1QdKm5axuO[0m
[17:07:48.598][0;32mI (178717) app_entry: DeviceName: test_006[0m
[17:07:48.598][0;32mI (178717) app_entry: DeviceSecret: J4R9yh47YjAHdcIIo6M7P01MSfErusWG[0m
[17:07:48.612][0;32mI (178727) app_entry: ProductSecret: sUBuz3XD13sP083P[0m
[17:07:48.612][0;33mW (217177) wrapper_kv: nvs get blob stassid failed with 1102[0m
[17:08:27.062][0;32mI (217177) app_entry: Set softap config[0m
[17:08:27.062][0;33mW (217177) wrapper_kv: nvs get blob stassid failed with 1102[0m
[17:08:27.068][1;31m[crt] awss_config_press(180): enable awss
[17:08:27.068][0m[0;32mI (217187) app main: IOTX_AWSS_ENABLE[0m
[17:08:27.080][1;31m[crt] awss_dev_ap_setup(36): ssid:adh_a1QdKm5axuO_D6D660
[17:08:27.080][0m[0;32mI (217197) awss: ssid: adh_a1QdKm5axuO_D6D660[0m
[17:08:27.089]I (217197) wifi:mode : softAP (24:0a:c4:d6:d6:61)
[17:08:27.089]I (217207) wifi:Total power save buffer number: 16
[17:08:27.101]I (217207) wifi:Init max length of beacon: 752/752
[17:08:27.101]I (217217) wifi:Init max length of beacon: 752/752
[17:08:27.101]I (217217) wifi:Total power save buffer number: 16
[17:08:27.109][0;32mI (218227) udp: success to establish udp, fd=54[0m
[17:08:28.104]
```

串口执行：`dev_ap`

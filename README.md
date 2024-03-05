# IOTDeviceSDK——物联网设备上云提供开箱即用接入SDK
## 一、项目介绍
IOTDeviceSDK是物联网平台提供的设备端软件开发工具包，可简化开发过程，实现设备快速接入各大物联网平台。
设备厂商获取SDK后，根据需要选择相应功能进行移植，即可快速集成IOTDeviceSDK，实现设备的接入。
- 支持mqtt,coap,modbus,cwmp(tr069),usp(tr369),snmp等协议 
- 支持Arduino，ESP-IDF，RaspberryPi，合宙等各种硬件平台 
- 支持openwrt，RT-Thread等OS
- 减少重复开发，快速接入各种云平台

> gitee: [IOTDeviceSDK](https://gitee.com/zhuangpengli/IOTDeviceSDK)

> github: [IOTDeviceSDK](https://github.com/zhuangpengLI/IOTDeviceSDK)

## 二、项目目录
* platform-generic ---------------------  适配通用平台
* platform-fastbee ---------------------  适配蜂信物联网平台
* platform-aliyun ----------------------- 适配阿里物联网平台

## 三、集成方式
据设备类型的不同，集成IOTDeviceSDK的方式也有差异：

- IP设备：指支持TCP/IP协议栈的设备，例如Wi-Fi设备，可以直接集成IOTDeviceSDK。
- 非IP设备：指不支持TCP/IP协议栈的设备，例如ZigBee、BT、KNX、ModBus等，需要通过集成了IOTDeviceSDK的网关设备，
再接入到各大物联网平台。子设备需要通过网关代理建连，网关集成IOTDeviceSDK，子设备与网关之间的通信方式由用户自定义，IOTDeviceSDK子设备接入，
## 四、设备功能开发
### 4.1连接与消息
  物联网平台为满足不同的设备接入需求，支持多种协议和多种认证方式，下面介绍设备接入物联网平台的几种方案。 

| 功能      | 说明                                                             |
|----------|-------------------------------------------------------------------|
| 多协议接入 | 支持MQTT、CoAP、HTTP等协议接入，大部分设备使用MQTT协议。               |
| 一机一密   | 每个设备使用单独的密钥进行认证。                                      |
| 一型一密   | 每个型号设备使用共同的密钥，简化设备烧录的工作。                          |
| 网关子设备 | 网关代理子设备接入物联网平台，子设备接入后可以使用子设备的topic与云平台通信。 |

### 4.2业务功能
设备在接入物联网平台后，可以通过物联网平台收发消息。设备将业务数据上报到物联网平台，可以通过以下方式。
- 自定义：用户完成自定义消息的topic和Payload，。
- 物模型：用户基于物模型定义设备属性、事件、服务，通过物模型的格式上报/下发，物模型是物联网平台的核心能力，使用物模型的优势：
- 数据结构化存储：简化后续数据开发流程。
- 状态缓存（设备影子）：设备/服务端在任意时间都可查询设备状态/数据。
- 数据可视化：在物联网平台页面可以查看数据。
- 在线调试：支持在线调试设备。
### 4.3运维功能
设备在完成业务功能开发后，在正式量产前，建议再增加运维功能，方便设备后续的迭代、配置及问题排查。物联网平台主要提供以下运维能力。

| 功能      | 说明                                                             |
|----------|-------------------------------------------------------------------|
| OTA | 设备升级，提供设备远程固件升级的能力。               |
| 远程登录   | 基于安全隧道，提供控制台通过SSH协议远程登录设备的能力。                                      |
| 日志上报   | 设备上报运行日志到物联网平台，通过该日志可进行设备问题排查。                          |
| 远程配置 | 远程配置设备。 |
| NTP服务 | 提供设备时间同步的能力。 |

## 五、参考资料

### 5.1 ESP-IDF 编程指南
[ESP-IDF 编程指南>>](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/index.html)

### 5.2 iRTU 开源DTU/RTU解决方案
[iRTU 开源DTU/RTU解决方案>>](https://wiki.openluat.com/doc/luatGuide/)

### 5.3 FastBee
[FastBee开源物联网平台 >>](https://gitee.com/kerwincui/wumei-smart)

### 5.4 Openwrt lede
[Openwrt lede >>](https://github.com/coolsnowwolf/lede.git)

### 5.5 RT-Thread
[RT-Thread IoT-Board SDK>>](https://gitee.com/Armink/IoT_Board)

### 5.6 Arduino
[Arduino入门教程>>](https://arduino.me/s/arduino-getting-started)

## 六、社区交流 & 代码贡献
- 欢迎设备厂商和平台小伙伴来贡献代码
- 使用过程有啥问题可以通过以下方式联系
- 微信: `a7491772`
- QQ: `908349383` (需备注“来自gitee”)
- 提供付费咨询和固件定制等服务
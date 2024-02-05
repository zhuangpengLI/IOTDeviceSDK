# 添加LinkSDK自定义组件。

1. 将LinkSDK复制到$IDF_PATH/components
2. 将examples/wifi复制到$IDF_PATH/examples


# 配置工程。

- 使用idf.py menuconfig配置工程，演示中使用了默认配置。
- 进入Example Configuration菜单。
- 修改WiFi SSID， WiFi Password和Maximum retry（最大重连次数）3个参数，保存并退出配置。

# 编译、烧写、串口监视。

- 进入examples/wifi/getting_started/station项目路径，使用idf.py build编译工程。
- 编译完成后使用idf.py -p PORT flash命令烧写固件（请用实际的USB端口名替换PORT）。
- 烧写成功后可运行idf.py -p PORT monitor打开串口打印信息监视器。
- 您也可使用idf.py -p PORT flash monitor连续执行这两条命令。

## 社区交流
- 使用过程有啥问题可以通过以下方式联系
- 微信: `a7491772` 
- QQ: `908349383` (需备注“来自gitee&github”)
- 提供付费咨询和固件定制等服务
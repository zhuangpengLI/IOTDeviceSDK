# 量产配置说明
物联网平台 在使用一机一密的情况下，
aliyun平台 需要在每个设备上烧写与设备对应的 ProductKey、ProductSecret、DeviceName、DeviceSecret。

蜂信物联网平台 
需要在设备上设置：认证类型 + 设备编号 + 产品编号 + 用户ID 作为mqtt clientId
如：clientId = S&D6329VL5668888&41&1
还需要设置MQTT的连接ip，端口，MQTT连接账号和密码

一般来说：认证类型，设备编号，产品编号为出厂固定值，不会随系统部署而发展变化

设备用户ID,MQTT服务相关配置可由部署环境的变化而动态切换

需要定制相关量产工具可以联系微信：a7491772
邮箱：908349383@qq.com
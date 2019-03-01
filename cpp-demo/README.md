# CTP Demo
上期所CTP接口控制台应用程序示例

* md: 即 `market data`, 对应行情订阅
* td: 即 `trade`, 交易相关
可以从[CTP官网](http://www.sfit.com.cn/5_2_DocumentDown.htm)下载。(simnow的SDK有点落后)
* 需安装 Anaconda Python3.6_x64
* 需安装VS2015, 网络上下载社区中文版离线安装包，所需硬盘空间约10G。安装选项仅选择C++部分，不需C#,.Net,Web Tools等
* 需安装 CMake最新版本 v3.x


## 编译
安装完 VS2017 + CMake，使用VS2017打开项目根路径的 CMakeLists.txt 作为 project。

执行 `md.exe`。仅在交易时段可以订阅到数据。

## todo:
* trade

## 依赖
* https://github.com/gabime/spdlog, header only logging

## 测试帐号
1. 注册测试帐号：http://www.simnow.com.cn
1. 账号重置： http://www.simnow.com.cn/static/resetPWDPage.action

# CTP Demo
上期所CTP接口控制台应用程序示例

* md: market data, 对应行情订阅
* trade: 对应订单提交；
* ctp_sdk: 为CTP接口所需的头文件和库文件，本项目使用版本: 6.3.8_20171123_traderapi.rar x64。
可以从[CTP官网](http://www.sfit.com.cn/5_2_DocumentDown.htm)下载。(simnow的SDK有点落后)
* 需安装 Anaconda Python3.6_x64
* 需安装VS2015, 网络上下载社区中文版离线安装包，所需硬盘空间约10G。安装选项仅选择C++部分，不需C#,.Net,Web Tools等
* 需安装 CMake最新版本 v3.x


## 编译
安装完 VS2015 + CMake，使用资源管理器打开项目根路径 `ctp_demo`, 双击执行批处理 `gan.bat`。

该批处理将创建 `build` 文件夹, 进入该文件夹，双击解决方案文件 `*.sln`，按键盘F7生成exe执行文件。

如果build(生成)成功，打开目录 `ctp_demo\build\md\install`, 执行 `md.exe`。仅在交易时段可以订阅到数据。

## todo:
* cmake 自动执行编译，不要每次都打开VS
* trade

## 备注：
1. md: 链接到thostmduserapi.lib
1. trade：要链接到thosttraderapi.lib

## 依赖

TODO: add __LINE__, __FILE__ macro
* https://github.com/gabime/spdlog, header only logging

## 测试帐号
1. 注册测试帐号：http://www.simnow.com.cn
1. 账号重置： http://www.simnow.com.cn/static/resetPWDPage.action








#include "ThostFtdcMdApi.h"
#include "md_spi.h"
#include "spdlog.h"

namespace spd = spdlog;

log_ptr lg = spd::stdout_color_mt("console");

CThostFtdcMdApi* pUserApi; // UserApi对象

char FRONT_ADDR[] = "tcp://180.168.146.187:10011";		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "9999";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "059979";		// 投资者代码
TThostFtdcPasswordType  PASSWORD = "123456";			// 用户密码
char *ppInstrumentID[] = { const_cast<char*>("cu1710") };			// 行情订阅列表
int iInstrumentID = 2;									// 行情订阅数量
int iRequestID = 0;                                     // 请求编号

int main(void)
{
	spd::set_pattern("%Y-%m-%d %H:%M:%S.%e - %L - %v");
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi

	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	pUserApi->Init();
	pUserApi->Join();
    pUserApi->Release();

    return 0;
}

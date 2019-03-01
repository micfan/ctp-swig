#include "md_spi.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono_literals;

extern CThostFtdcMdApi* pUserApi;
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];
extern int iInstrumentID;
extern int iRequestID;

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	lg->error("OnRspError()");
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	lg->error("OnFrontDisconnected, Reason: {}", nReason);
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	lg->warn("OnHeartBeatWarning, nTimerLapse: {} ", nTimeLapse);
}

void CMdSpi::OnFrontConnected()
{
	lg->info("OnFrontConnected");
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	lg->info("Send Request for user login: {}", iResult == 0 ? "success" : "failed");
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
							CThostFtdcRspInfoField *pRspInfo,
							int nRequestID,
							bool bIsLast)
{
	lg->info("OnRspUserLogin");
	if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
		lg->info("current trading day: {}", pUserApi->GetTradingDay());
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	lg->info("subscribing market data: {}", iResult == 0 ? "success" : "failed");
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    lg->info("OnRspSubMarketData instrumentId is {}", pSpecificInstrument->InstrumentID);
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspUnSubMarketData");
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	lg->info("OnRtnDepthMarketData, instrumentId: {}, Lastprice: {}",
		pDepthMarketData->InstrumentID,
		pDepthMarketData->LastPrice);
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		lg->info("ErrorID: {}, ErrorMsg: {}", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}

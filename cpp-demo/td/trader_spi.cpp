#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono_literals;

#include "trader_spi.h"

extern CThostFtdcTraderApi* pUserApi;

extern char FRONT_ADDR[];		// 前置地址
extern char BROKER_ID[];		// 经纪公司代码
extern char INVESTOR_ID[];		// 投资者代码
extern char PASSWORD[];			// 用户密码
extern char INSTRUMENT_ID[];	// 合约代码
extern TThostFtdcPriceType	LIMIT_PRICE;	// 价格
extern TThostFtdcDirectionType	DIRECTION;	// 买卖方向
extern int iRequestID; // 请求编号

// 会话参数
TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

// 流控判断
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	lg->info("OnFrontConnected");
	///用户登录请求
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	lg->info("发送用户登录请求, iResult: {} -> {}", iResult, ((iResult == 0) ? "成功" : "失败"));
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspUserLogin");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		lg->info("ORDER_REF: {}, {}", ORDER_REF, iNextOrderRef);
		///获取当前交易日
		lg->info("获取当前交易日: {}", pUserApi->GetTradingDay());
		///投资者结算结果确认
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	lg->info("投资者结算结果确认: {}, {}", iResult, ((iResult == 0) ? ", 成功" : ", 失败"));
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspSettlementInfoConfirm");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约
		ReqQryInstrument();
	}
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			lg->info("请求查询合约: {}, {}", iResult, ((iResult == 0) ? ", 成功" : ", 失败"));
			break;
		}
		else
		{
			lg->info("请求查询合约: {} 受到流控", iResult);
            this_thread::sleep_for(1s);
		}
	} // while
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspQryInstrument");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约
		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			lg->info("请求查询资金账户: {}", iResult + ((iResult == 0) ? ", 成功" : ", 失败"));
			break;
		}
		else
		{
			lg->info("请求查询资金账户: {}", iResult);
            this_thread::sleep_for(2s);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspQryTradingAccount");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询投资者持仓
		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			lg->info("请求查询投资者持仓: {}", iResult + ((iResult == 0) ? ", 成功" : ", 失败"));
			break;
		}
		else
		{
			lg->info("请求查询投资者持仓: {}", iResult);
            this_thread::sleep_for(2s);
		}
	} // while
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspQryInvestorPosition");
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///报单录入请求
		ReqOrderInsert();
	}
}

void CTraderSpi::ReqOrderInsert()
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
    strcpy(req.BrokerID, BROKER_ID);
	///投资者代码
    strcpy(req.InvestorID, INVESTOR_ID);
	///合约代码
    strcpy(req.InstrumentID, INSTRUMENT_ID);
	///报单引用
    strcpy(req.OrderRef, ORDER_REF);
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
//    req.OrderPriceType = THOST_FTDC_OPT_AnyPrice; //shijia
	///买卖方向: 
	req.Direction = DIRECTION;
	///组合开平标志: 开仓
    // req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;//  THOST_FTDC_OF_Close
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
    req.LimitPrice = LIMIT_PRICE;
    //req.LimitPrice = 0; // shijia
	///数量: 1
	req.VolumeTotalOriginal = 1;
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	lg->info("报单录入请求: {}", iResult + ((iResult == 0) ? ", 成功" : ", 失败"));
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspOrderInsert");
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//是否发送了报单
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
    strcpy(req.BrokerID, pOrder->BrokerID);
	///投资者代码
    strcpy(req.InvestorID, pOrder->InvestorID);
	///报单操作引用
//	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
    strcpy(req.OrderRef, pOrder->OrderRef);
	///请求编号
//	TThostFtdcRequestIDType	RequestID;
	///前置编号
	req.FrontID = FRONT_ID;
	///会话编号
	req.SessionID = SESSION_ID;
	///交易所代码
//	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
//	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///价格
//	TThostFtdcPriceType	LimitPrice;
	///数量变化
//	TThostFtdcVolumeType	VolumeChange;
	///用户代码
//	TThostFtdcUserIDType	UserID;
	///合约代码
    strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
	lg->info("报单操作请求: {}, {}", iResult, ((iResult == 0) ? ", 成功" : ", 失败"));
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspOrderAction");
	IsErrorRspInfo(pRspInfo);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	lg->info("OnRtnOrder");
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			lg->info("撤单成功"); 
	}
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	lg->info("OnRtnTrade");
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	lg->info("OnFrontDisconnected");
	lg->info("Reason: {}", nReason);
}
		
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	lg->info("OnHeartBeatWarning");
	lg->info("nTimerLapse: {}", nTimeLapse);
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	lg->info("OnRspError");
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		lg->info("ErrorID: {}, ErrorMsg: {}", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

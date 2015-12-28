#include "SrvLib.h"
#include "readerwriter.h"
#include "ModuleSupport.h"
#include "LogicServer.h"

CLIENT_COMMAND_EX_BEGIN(cLogicServer)
	CLIENT_COMMAND_EX(gt2lg_Packet,		OnRcvGatePacket)		//网关转发给逻辑处理服务器的包
	CLIENT_COMMAND_EX(gt2lg_NewPlayer,	OnRcvNewPlayer)			//新进来的玩家信息
	CLIENT_COMMAND_EX(gt2lg_Dropped,	OnRcvDelPlayer)			//断线了
CLIENT_COMMAND_EX_END(cOtherServer)

void cLogicServer::OnRcvGatePacket(Session * pSession)
{
}
void cLogicServer::OnRcvNewPlayer(Session * pSession)
{
}
void cLogicServer::OnRcvDelPlayer(Session * pSession)
{
}
void cLogicServer::OnPlayerPacket(int nCmd,UINT nPlayerID,size_t nSize)
{
}

#include "EnumToString.h"

BEGIN_ENUM(ServerCommand)
{
	// 客户端<->服务器组
	ENUM_ITEM(cl2sr_packet),				// 客户端发消息给服务器组
	ENUM_ITEM(sr2cl_packet),				// 服务器组发消息给客户端

	// 区域管理<->其他服务器
	ENUM_ITEM(mg2ot_ServerID),				// 分配到的服务器ID					参数 UINT nServerID
	ENUM_ITEM(mg2ot_SessionInfo),			// 已经登陆到管理者的服务器的信息	参数 NetIOConnect
	ENUM_ITEM(mg2ot_Transmit),				// 转发消息							参数 ServerID 长度 实际消息
	ENUM_ITEM(mg2ot_ReportLoad),			// 告知服务器是否要汇报负载变化		参数 INT nNeedReport
	ENUM_ITEM(ot2mg_MyInfo),				// 已经登陆到管理者的服务器的信息	参数 SessionInfo
	ENUM_ITEM(ot2mg_Transmit),				// 通过管理者向其他服务器转发消息	参数 长度 实际消息
	ENUM_ITEM(ot2ot_MyInfo),				// 连接其他服务器后,发送自己的信息	参数 SessionInfo

	// 网关<->逻辑服务器
	ENUM_ITEM(gt2lg_Packet),				// 网关转发给逻辑处理服务器的包		参数 来源客户端ID	命令+来源+子命令+参数
	ENUM_ITEM(gt2lg_NewPlayer),				// 新进来的玩家信息
	ENUM_ITEM(gt2lg_Dropped),				// 断线了							参数 来源客户端ID

	ENUM_ITEM(lg2gt_Packet),				// 参数 目的客户端ID	命令+目的+子命令+参数
	ENUM_ITEM(lg2gt_Broadcast_ByList),		// 逻辑服务器到网关的客户端列表广播
	ENUM_ITEM(lg2gt_Broadcast_All),			// 逻辑服务器到网关的全体客户端广播

	ENUM_ITEM(lg2gt_ServerPing),			// 返回其他server的ping float gate iocp层发送时间 float lg server iocp层接收时间 float lg server发送时间
	ENUM_ITEM(lg2gt_Kick),					// 踢掉网关上的连接					参数 目的客户端ID
	ENUM_ITEM(lg2gt_KickAll),				// 断开网关上所有连接。
	ENUM_ITEM(lg2gt_KickByServerID),		// 踢掉某个特定服务器的链接
	ENUM_ITEM(lg2gt_LimitIp),				// 通知网关打开/关闭IP限制
	
	ENUM_ITEM(lg2gt_Login),					// 登录完成
	ENUM_ITEM(lg2gt_Character),				// 角色选择完成
	ENUM_ITEM(lg2gt_ChangeMap),				// 转跳到相应的地图

	// 登录服务器->网关
	ENUM_ITEM(li2gt_LogonOtherwise),		// 通知某连接在其他地方登录了.
	ENUM_ITEM(li2gm_DefendIndulge),			// 防沉迷提示

	ENUM_ITEM(ServerCommand_Max)
}END_ENUM(ServerCommand)

BEGIN_ENUM(DbCommand)
{
	ENUM_ITEM(db_QueryObject),
	ENUM_ITEM(db_NewObject),
	ENUM_ITEM(db_DeleteObject),
	ENUM_ITEM(db_UpdateObject),
	ENUM_ITEM(db_Max)
}END_ENUM(DbCommand)
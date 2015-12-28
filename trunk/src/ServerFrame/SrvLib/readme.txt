cBaseServer
class cOtherServer: public cBaseServer
class cGateServer : public cOtherServer
class cLogicServer: public cOtherServer
class cManagerServer: public cBaseServer

.Manager
Other
	.Gate
	Logic
		.Login
		.Game
		.AI
		.Calc

Manager<->Other
Gate<->Logic


GateServer
	CCmdFilter	保证玩家的命令是符合预定时序的。
				保证某些命令不能发送过于频繁
	cFlowCounter 消息长度优化分析。统计各消息流量，便于针对流量过大的消息进行优化。
	IP限制		只允许规定的IP登录（可用于不对外网开发的内网测试）
				IP黑名单/白名单
	版本校验
	客户端MD5校验			
	变速器校验
	定期ping客户端
	
	如何判别哪些命令交给哪个服务器去处理？
		1.对PB的消息采取枚举编号
		2.对PB的service名进行长列表列举
		3.对消息进行范围性地编号
			1-校验类消息
			2-登录类消息
			3-游戏类消息
			4-房间类消息
	如何把PB的各种名，兑换成相对短的ID.
		因为要区分File.

房间服务器通用化
	1、每个游戏的玩家信息和房间信息都会不一样，如何处理
		写成基类和派生类，管理器只保存指针。
		每个玩家都有一个 ID 作为唯一的标志。
	2、固定下房间服务器与客户端的消息定义。作为基础协议。
	3、玩家列表到底由逻辑服务器来做，还是由房间服务器来做
		把玩家查询缓存分离开来做
	4、通用逻辑服务器的功能设计。
		维护玩家列表
		基本的消息机制

房间列表
	房间信息
		指针
		长度
	房主ID
	玩家列表
	房间状态
		等待加入
		等待分配游戏服务器
		等待游戏服务器创建房间
		游戏中
	所在游戏世界的服务器ID		

玩家列表
	玩家信息
		指针
		长度
	服务器组数据
		玩家所在的Gate信息
		Gate 产生的玩家ID
		在 Client 链表中的位置

	所在房间的ID
	房间查询缓存
		当前的查询条件
		当前查询的房间ID
		当前房间数量
		当前查询页
		
查询缓存
	在可组合条件不多的情况下，可以把各种查询条件的结果缓存。
	
cPlayer
cRoomPlayer
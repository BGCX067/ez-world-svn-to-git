.测试用例编写
.c++调用c#界面 机器人状况绑定
	客户端基本状态，无法定制。client.
	ID			LIST_ID,
	状态		LIST_STATE,
	掉线次数	LIST_DROPTIME,

	客户端游戏数据，必须定制
	帐号名	LIST_ACCOUNT,
	地图	LIST_MAP,
	级别	LIST_LEVEL,
	坐标	LIST_XY,

	机器人或者挂机的AI状态，必须定制
	AI状态名称	LIST_SUBSTATE_NAME,
	AI状态持续	LIST_SUBSTATE_TIME,
	AI持续标记	LIST_SIGN,
	AI详细信息	LIST_REALTIME,

	向表现层传递 单个 还是 多个？

	表现层接口
		还是数据结构的组织问题 参考xml的结构

		相对于上一级节点，无论是树节点和叶节点，都可能是：
			唯一的固定名节点
			固定索引节点
			实例ID节点
		
		需要一个数据结构，能够迅速定位到对应的叶节点。
			被添加的，可能是一个子树。
		
		ROOT
			角色
				玩家
					
				NPC
				


    
.c++调用flash界面 更新状况绑定
.去掉win32api的直接调用，去掉windows头文件的直接包含
.

机器人(先做控制台界面，可绑定到任何界面)
	机器人的配置文件
		数量
		Support的参数

标准客户端(标准windows窗口)
	
	
	ClientFrame
		唯一的设备
		ClientCores
		


客户端脱离
	图形&声音引擎
	GUI
	程序窗口

Support提供的功能如何访问
	CModuleSupport
因为与图形分开，是否还需要效率统计？只在主线程中做效率统计
	目前效率统计是单线程的。无法同时统计渲染线程和逻辑线程……
如何与渲染线程协作？何时同步？暂时不考虑
消息分类 
	1.rpc请求的回应	需要处理错误
		可以统一集中处理的：
		要交给具体系统去处理的：
	2.服务器主动推送的消息 不需要处理错误

Module,State:OnMessageBox,OnWndMessage,OnGuiMessage,OnGuiMessageDefault,FormDialog,OnRender(GUI消息),属于与图形客户端绑定之后的产生物
OnConnected时做版本检测
加速器检测，作为客户端定时处理的系统
版本号单独出一个类
窗口最小化处理只在真实客户端
MD5检测只在真实客户端
客户端字符串资源转移到上一层的类
特殊消息的处理(绝不准在这里硬编码直接处理任何消息了！)
	s2c_Gate_LogonOtherwise 别人用此帐号在其他地点登录了。需要回到登录状态，重新登录。（服务器推送的）
	s2c_Login_GatePing		直接回应 Gate的Ping（服务器推送的）
	s2c_Account_Freezed		登录时发现帐号被冻结（请求登录时的回应）
特殊消息的错误处理(绝不准在这里硬编码直接处理任何消息的错误了！)
	s2c_Version_Verify		退出客户端
	s2c_Login_Logon			断开连接
	s2c_Mail_SendMail		禁止相应界面
	s2c_Map_NewbieSelect	禁止相应界面
	s2c_Mail_ReceiveAdjunct	禁止相应界面
	s2c_Role_Create			禁止相应界面
	s2c_Role_Delete			禁止相应界面
	以下消息同时交由相应系统做进一步处理
	s2c_GemInset_Perforate:
	s2c_GemInset_Upgrade:
	s2c_GemInset_Inlay:
	s2c_Login_Logon:
	s2c_Role_List:
	s2c_MakeEquip:
	s2c_Equip_SoulLV:
	s2c_Equip_SoulQL:
	s2c_Consortia_Create:
	s2c_Role_Ready:
	s2c_Equip_Binding:
	s2c_UpdateGem:



需要文件来描述客户端消息的触发方式
	1.界面操作
	2.时钟触发
	3.接到服务器消息
	4.游戏操作
客户端消息的参数来源
	1.玩家游戏操作
	2.界面绑定
	3.时钟绑定
	4.文件内容绑定

Gate认为是框架中唯一的服务器。

1.服务器框架程序
2.客户端框架程序
3.数据库模块
	能登陆
	能行走
	能互见

Gate
	转发
	广播
	踢除链接
	状态转换
		登录
		选择角色
		游戏状态
	地图转换

	流量统计
	命令过滤
	黑白IP名单
	Ping检测
	加速器检测	

	版本校验
	MD5校验

Login
	

GameFrame	网络设备
	Client		单独的客户端数据
		CClientState * m_pState;	每个客户端的游戏状态
MyApp		网络设备
		CClientState * m_pState;


	

1.manager的依赖表 由pb参数化
2.log配置文件，除了指定配置文件，应该还制定输出文件，或者可以包含。
3.log配置文件的详细语法

先尽可能让框架完整 vs 先尽可能让测试完整
可扩充 vs 不可扩充
	
Server框架
	管理服务器
	网关服务器
	帐号/角色服务器
	计算服务器	进行关系加成运算。不做任何玩家操作逻辑处理。
	逻辑服务器
	AI服务器
	数据库通用模块
	
多语种互相调用Interface层

GameProto层
Fiber库

最简化的可测系统
	多个玩家对象
	如何模拟更多的函数调用？
		
		


从消息队列转换为纤程池。
	当纤程池中的纤程已经被用尽，消息队列中又出现新消息时
		无空闲纤程就不要接收。让消息队列去堆积。
		使用全局的读写缓存类
		
需要一个网络引擎的界面。以便迅速建立消息的测试机制。
	
从网络引擎中接受到 请求数据必须要被暂存在某个队列中。
	

1.把GameProto归结为公共库。log接口如何工作
2.模板放在一起 vs 模板放在单独系统中
3.整个世界需要一个根
	root
		template
		weapon
		role
		instance
	联系表不再从逻辑引擎中导出了。得不偿失。	
5.加入api
	c2s_login_api
		登录
		获取角色列表
		创建角色
		删除角色
		进入游戏
	c2s_world_api
		GM命令设置玩家属性
		物品操作

		行走/停止
		施放技能
  加入登录Session数据类型
		帐号信息 + 角色列表
  加入服务器的返回数据描述
		玩家本身的数据 在客户端的映像
			完全数据吗？
			物品也采取完全数据？
		客户端需要把自己转换成可视角色的函数？
	

4.为了最小化更新，一个数据表一个Data文件。
  为了版本工具管理方便，基于json方式来编辑。发行时再转换成Data文件。
	需要一个json文件来描述数据表与文件的对应关系。


6.数据库存储机制
7.地图上物体的互见性
			
					

	玩家填写的参数
		CHAR		szRoleName[MAX_NICKNAME+1];		// 角色名称
		typedef struct tRoleAvatar
		{
			UINT		nProfession;			// 职业
			UINT		nSex;					// 性别
			UINT		nFacialForm;			// 脸型
			UINT		nFacialTexture;			// 脸贴图
			UINT		nHair;					// 头发模型
		}RoleAvatar,*PRoleAvatar;		
	
	系统填写的参数
		普通攻击的快捷方式
		初始位置 地图编号 x,y
		ForceField,Agility,Physique,Habitus,Brightness
		BagNum,DepotPage
		新手装
		

不需要c++的源码？


世界描述库	既能当做客户端又能当做服务器。只是在客户端没有实际的函数响应插件，无法做出实际的响应。
	发起远程调用
	响应远程调用
	管理消息实例？
服务器逻辑响应库	可以使用任何脚本语言或者原生c语言来实现逻辑。
客户端数据绑定库


一、CoreSupport
OK.修改dllframe,让所有的dll继承自同一接口。
OK.printf类型的log改到support
LATER.setlocale问题
	<locale.h>的问题？express版本+stlport的问题
	在support时都初始化好。
	只解决了wcout,未解决
	更好的做法应该是使用utf8做输出，与具体的字符集无关。
OK.修改ezworld.vsprops
	debug /release 的执行目录分开
	配置文件只写一套。
	如果dll装载可以通过设置path来进行，就不需要设置用装载路径了。
OK.代码细分目录
todo.因为不是静态链接了，需要一个传递全局变量来使用support的方式

.单元测试
	support
		要检测在release版本时是否正确。
		在其他动态链接库里使用非英语种输出log.
	util
	appframe
	dllframe
二、客户端框架
	服务器框架
	netlib
	
	ClientCore
		ClientCore接口
			
			
		装载client.xml
	ClientCleanBind
	ClientOgreBind
	ClientRobotBind
	
	3DEngine
	UiPlugins
	

	状态，到底是应该给谁管理？
		谁来初始化一个状态
		定义游戏客户端状态的方式
			一个状态列表
		如何定义客户端状态的切换条件？
			ClientCore
				可以为任意函数调用添加脚本回调。
					进行客户端的状态切换。

		系统
			数据
			请求								
		每个消息类型都有一个ID吗？
		
		引用和实体的问题。
			绑定到一个引用
			绑定到一个实体？
			同类型的实体可能出现在一棵树的多处，

			描述树+客户端映像树+服务器实例树
			定义一系列针对实例树的操作。（每个游戏都适用的系统级别消息）
			假设把玩家定义成一个消息。
				玩家
					基本角色属性
					基本玩家属性
					背包容器
			
			客户端
				登录	
				选人
				创建
				游戏世界
					地图
					自己
						玩家属性
						物品集合
					其他角色列表
						
	
三、protobuf
四、mongodb
五、原生c语言写逻辑。
.不同stl版本的hash不兼容问题。



1.不跨平台
2.不使用libevent
3.不改造protobuf

1.是否跨平台。
2.是否使用libevent
3.是否改造protobuf


AppFrame
DllFrame
1.support
	ziparchive
	log4cplus

网络库
服务器构架库
脚本层绑定
客户端构架
表现层绑定
	


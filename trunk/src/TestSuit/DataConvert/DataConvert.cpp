// apptest.cpp : 定义控制台应用程序的入口点。
//

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <fstream>

#include "support.h"
#include "SupportLoader.h"
#include "LogicInterface.h"
#include "GameProto.h"
#include "Logic2Msg.h"

#define OVERLOAD
#include "overload.h"

#define LOGIC_DLL	_T("DotNetInterface.dll")
IMemManager * pMemManager = NULL;

// 如何让
class CTestFunction : public iFunctionCall
{
public:
	int Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams){return 0;}
};

const char * g_lpszEquipFields[] = 
{
	"item_info.id",			//base_ID
	"item_info.name",		//base_Name	
	NULL,					//base_Des	
	"item_info.buy_able",	//item_Buy	
	"item_info.sell_able",	//item_Sell	
	"item_info.exchange_able",	//item_Change	
	"item_info.destroy_able",	//item_Destory	
	NULL,					//item_Overlap	
	"item_info.time_max",	//item_Time	
	"item_info.money_type",	//item_Moneytype
	NULL,					//item_Buyprice
	NULL,					//item_Sellprice
	"item_info.bind_type",	//item_BandType
	NULL,					//item_BandFlag
	NULL,					//item_DeleFlag
	NULL,					//equip_Durabilit
	"item_info.class",		//equip_class
	NULL,					//equip_level
	"levels.max_durabilit",	//equip_maxdurabilit
	NULL,					//equip_socket
	NULL,					//equip_bangding
	"body",					//equip_body
	NULL,					//equip_moneyup
	"levels.soulup",		//equip_soulup
	NULL,					//equip_job
	"set_id",				//equip_setnum
	"item_info.icon_id",	//equip_modnum
	NULL,					//equip_gem1
	NULL,					//equip_gem2
	NULL,					//equip_gem3
	NULL,					//equip_gem4
	"levels.upchance",		//equip_upchance
	"levels.socketchan",	//equip_socketchan
	NULL,					//equip_finesign
	"equiplv",				//equip_equiplv
	"xzcsone",				//equip_xzcsone
	"xzcstwo",				//equip_xzcstwo
	NULL,					//equip_posterid
	NULL,					//equip_xysjone
	NULL,					//equip_xysjtow
	NULL,					//equip_sjxszl
	"levels.stilettoprice",	//equip_stilettoprice
	"sfbxyfjsjxs"			//equip_sfbxyfjsjxs
};
const char * g_lpszRoleFields[]=
{
	"charactor.leastdc",	//charactor_leastdc = 0,	//最小物理攻击力
	"charactor.maxdc",	//charactor_maxdc = 1,	//最大物理攻击力
	"charactor.ac",		//charactor_ac = 2,	//物理防御力
	"charactor.mac",		//charactor_mac = 3,	//法术防御力
	"charactor.crit",	//charactor_crit = 4,	//物理重击率
	"charactor.skillcrit",//charactor_skillcrit = 5,	//技能重击率
	"charactor.hitchance",//charactor_hitchance = 6,	//命中
	"charactor.hedge",	//charactor_hedge = 7,	//躲闪
	"charactor.hpreg",	//charactor_hpreg = 8,	//生命恢复速度
	"charactor.mpreg",	//charactor_mpreg = 9,	//法力恢复速度
	"charactor.attspeed",	//charactor_attspeed = 10,	//攻击速度
	"charactor.movespeed",	//charactor_movespeed = 11,	//移动速度
	"charactor.maxhp",	//charactor_maxhp = 12,	//最大生命值
	"charactor.maxmp",	//charactor_maxmp = 13,	//最大法力值
	"charactor.HP",		//charactor_HP = 14,	//当前生命值
	"charactor.MP",		//charactor_MP = 15,	//当前法力值
	"charactor.state",	//charactor_State = 16,	//当前状态
	"charactor.silent",	//charactor_silent = 17,	//沉默
	"charactor.Chaotic",	//charactor_Chaotic = 18,	//混乱
	"charactor.runspeed",//charactor_runspeed = 19,	//跑步速度
	"charactor.level",	//charactor_level = 20,	//等级
	"str",				//charactor_str = 21,	//力量
	"agi",				//charactor_agi = 22,	//敏捷
	"phy",				//charactor_phy = 23,	//灵力
	"con",				//charactor_con = 24,	//体质
	"int",				//charactor_int = 25,	//智慧
	"charactor.skilleff",//charactor_skilleff = 26,	//法术伤害
	"charactor.attdistance",	//charactor_attdistance = 27,	//普通攻击范围
	"charactor.dmgweek",		//charactor_dmgweek = 28,	//伤害减免
	"charactor.dmgreturn",	//charactor_dmgreturn = 29,	//伤害反弹
	"charactor.double_rate",	//charactor_double = 30,	//利刃几率
	"charactor.crazy",	//charactor_crazy = 31,	//狂战士几率
	"charactor.dizzy",	//charactor_dizzy = 32,	//击晕几率
	"charactor.EatMp",	//charactor_EatMp = 33,	//嗜魔几率
	"charactor.EatHp",	//charactor_EatHp = 34,	//嗜血几率
	"charactor.Mpreturn",//charactor_Mpreturn = 35,	//宁神几率
	"charactor.qx",		//charactor_qx = 36,	//潜行
	"charactor.stand",		//charactor_stand = 37,	//站立半径
	"charactor.camouflage",	//charactor_camouflage = 38,	//伪装
	"charactor.sense",		//charactor_sense = 39,	//侦测
	"charactor.isolation",	//charactor_isolation = 40,	//隔绝
	"charactor.immunity",	//charactor_immunity = 41,	//免疫
	"charactor.predictmarker",	//charactor_predictmarker = 42,	//预言术标记
	"charactor.manaburnmarker",	//charactor_manaburnmarker = 43,	//法力灼烧标记
	NULL,	//charactor_skillriseID = 44,		//技能复活ID
	NULL,	//charactor_skillrisepunish = 45,	//技能复活惩罚倍率
	NULL,	//charactor_skillriseHP = 46,		//技能复活HP
	NULL,	//charactor_skillriseMP = 47,		//技能复活MP
	NULL,	//charactor_propriseID = 48,		//道具复活ID
	NULL,	//charactor_proprisepunish = 49,	//道具复活惩罚倍率
	NULL,	//charactor_propriseHP = 50,		//道具复活HP
	NULL,	//charactor_propriseMP = 51,		//道具复活MP
	"charactor.manaburnmarker",	//charactor_poseID = 52,	//伪装ID
	NULL,	//charactor_exppunish = 53,	//经验惩罚
	"charactor.manaburnmarker",	//charactor_nomove = 54,	//定身
	NULL,	//charactor_zhsleastdcappend = 55,	//召唤兽最小物理攻击力附加
	NULL,	//charactor_zhsmaxdcappend = 56,	//召唤兽最大物理攻击力附加
	NULL,	//charactor_zhsacappend = 57,		//召唤兽物理防御力附加
	NULL,	//charactor_sunmmonsmarker = 58,	//召唤兽标记
	NULL,	//player_dbid = 59,	//帐号ID
	"nickname",				//player_nickname = 60,	//昵称
	"exp",					//player_exp = 61,	//经验值
	"title",				//player_title = 62,	//称号
	"job",					//player_job = 63,	//职业
	NULL,	//player_money = 64,	//未使用
	NULL,	//player_gold = 65,	//元宝
	"nExpFactor",			//player_ExpFactor = 66,	//经验倍率
	"bag_num",				//player_BagNum = 67,	//背包栏数
	"depot_num",			//player_DepotPage = 68,	//仓库栏数
	"pk",					//player_PK = 69,	//PK值
	NULL,	//player_mailbox = 70,	//邮箱
	NULL,	//player_party = 71,	//所在队伍ID
	NULL,	//player_guild = 72,	//所在公会ID
	NULL,	//player_marry = 73,	//夫妻ID
	NULL,	//player_study = 74,	//师徒ID
	NULL,	//player_brothor = 75,	//结拜ID
	"pk_mode",		//player_PKmode = 76,	//PK模式
	"soul",			//player_soul = 77,	//魂
	"unusepoint",	//player_unusepoint = 78,	//未分配点数
	"sex",			//player_sexy = 79,	//性别
	"nuplevelexp",	//player_uplevelexp = 80,	//升级所需经验
	"magicfind",	//player_magicfind = 81,	//寻宝率
	NULL,	//player_rwtimes = 82,	//任务次数
	NULL,	//player_wallowstate = 83,	//防沉迷状态
	NULL,	//player_count_call1 = 84,	//称号计数器1
	NULL,	//player_count_call2 = 85,	//称号计数器2
	NULL,	//player_count_call3 = 86,	//称号计数器3
	NULL,	//player_count_call4 = 87,	//称号计数器4
	NULL,	//player_count_call5 = 88,	//称号计数器5
	NULL,	//player_count_call6 = 89,	//称号计数器6
	NULL,	//player_count_call7 = 90,	//称号计数器7
	NULL,	//player_count_call8 = 91,	//称号计数器8
	NULL,	//player_count_call9 = 92,	//称号计数器9
	NULL,	//player_count_call10 = 93,	//称号计数器10
	NULL,	//player_count_call11 = 94,	//称号计数器11
	"slient",			//player_slient = 95,	//禁言
	NULL,	//player_gminvisible = 96,	//GM隐身
	NULL,	//player_gmunbreakable = 97,	//GM无敌
	NULL,	//player_fbcd = 98,	//法宝冷却时间
	NULL,	//player_ridelv = 99,	//骑乘级别
	NULL,	//player_ridetype = 100,	//骑乘类型
	NULL,	//player_standingroleintegral = 101,	//日常任务积分
	"gongde",			//player_gongde = 102,	//功德
	"aixinzhi",			//player_aixinzhi = 103,	//爱心值
};

UnitField g_UnitFields[] =
{
	{Logic::Unit_player,g_lpszRoleFields,Logic::player_Max},
	{Logic::Unit_equip,g_lpszEquipFields,Logic::equip_Max},
	{-1,NULL,0}
};

const char * g_lpszEquipMakeIndices[] = 
{
	"equip_makes",
	"pinzhi",	//equipmake_ID = 0,	//职业
	"adder",	//equipmake_pinzhi = 1,	//品质
};
const char * g_lpszEquipMakeFields[] = 
{
	"jiazhi",	//equipmake_jiazhi = 2,	//加值
	"wumian",	//equipmake_wumian = 3,	//物免
	"momian",	//equipmake_momian = 4,	//魔免
	"pingjunshangmian",	//equipmake_pingjunshangmian = 5,	//平均伤免
	"desc",		//equipmake_kanzhefangbian = 6,	//看着方便
};
const char * g_lpszEquipBuWeiIndices[] = 
{
	"equip_buwei"
};
const char * g_lpszEquipBuWeiFields[] = 
{
	"buweixishu",	//buweixishu_buweixishu = 1,	//部位系数
	"pricexishu"	//buweixishu_pricexishu = 2,	//价格系数
};










int _tmain(int argc, _TCHAR* argv[])
{
	// 支持库
	CSupportLoader support;
	if(!support.Load(_T("../bin/log4cplus.properties"),_T("data"),_T("client_update"),NULL))
		return -1;
	pMemManager = support.GetSystem()->GetSysMem();

	CModuleLoader<iLogicInterface> logiclib;
	if(!logiclib.Load(LOGIC_DLL,NULL,support.GetSystem()))
		return -1;
	
#ifdef _DEBUG
	const char * lpszDotNetDll = ".\\debug\\DotNetLogicTest.dll";
	const TCHAR * lpszLogicEngin = _T(".\\debug\\LogicEngine.dll");
#else
	const char * lpszDotNetDll = ".\\release\\DotNetLogicTest.dll";
	const TCHAR * lpszLogicEngin = _T(".\\release\\LogicEngine.dll");
#endif

	CTestFunction testFunctions;
	if(!logiclib.GetInterface()->Initialize(lpszDotNetDll,&testFunctions))
		return -1;
	
	BOOL bRet = SetEnvironmentVariable(_T("LogicDefine"),_T("..\\..\\data\\WorldDefine_gb2312.xml"));
	LogicEngin logicEngine;
	if(!logicEngine.Init(lpszLogicEngin,"..\\..\\data\\world.logic"))
		return -1;
	
	google::protobuf::GameProto gp(support);
	if(!gp.Init(_T("overlord.desc")))
		return -1;

	// 数据根节点
	google::protobuf::Message * pRoot = gp.New("world.Root");
	if(pRoot)
	{
		const google::protobuf::ServiceDescriptor * pServiceDescriptor = gp.GetDescriptorPool()->FindServiceByName("world.LoginService");
		int nMethods =  pServiceDescriptor->method_count();
		for(int i=0;i<nMethods;i++)
		{
			const google::protobuf::MethodDescriptor * pMethodDescriptor = pServiceDescriptor->method(i);
			std::string m_name = pMethodDescriptor->full_name();
			const google::protobuf::Descriptor * pInputDesc = pMethodDescriptor->input_type();
			const google::protobuf::Descriptor * pOutDesc = pMethodDescriptor->output_type();
			int x = 0;
		}
		
		// 模板节点	一个表一个json文件
		const google::protobuf::FieldDescriptor * df = pRoot->GetDescriptor()->FindFieldByName("templates");
		google::protobuf::Message * pTemplateRoot = pRoot->GetReflection()->MutableMessage(pRoot,df);
		if(pTemplateRoot)
		{
			// 装备模板
			//ImportTemplate(logicEngine.GetWorld(),pTemplateRoot,"equips",Logic::Unit_equip,g_lpszEquipFields,Logic::equip_level);

		}

		// 一个系统一个json文件
		// 武器系统
		df = pRoot->GetDescriptor()->FindFieldByName("equip_tables");
		google::protobuf::Message * pEquipTemplate = pRoot->GetReflection()->MutableMessage(pRoot,df);
		if(pEquipTemplate)
		{
			ImportContact(logicEngine.GetWorld(),pEquipTemplate,Logic::Contact_equipmake,g_lpszEquipMakeIndices,g_lpszEquipMakeFields,2);
			ImportContact(logicEngine.GetWorld(),pEquipTemplate,Logic::Contact_buweixishu,g_lpszEquipBuWeiIndices,g_lpszEquipBuWeiFields,0);

			int nByteSize = pEquipTemplate->ByteSize();
			std::ofstream os( "..\\..\\data\\equip.data" );
			bool b = pEquipTemplate->SerializeToOstream(&os);
			os.close();
			
			Json::Value root(Json::objectValue);
			google::protobuf::GameProto::Pb2Json(root,pEquipTemplate);
			Json::StyledStreamWriter writer;
			std::ofstream jsonos( "..\\..\\data\\equip.json" );
			writer.write( jsonos,root );
			jsonos.close();
		}

		delete pRoot;
	}
	return 0;
}


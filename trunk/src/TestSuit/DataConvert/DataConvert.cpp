// apptest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif
#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
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

// �����
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
	"charactor.leastdc",	//charactor_leastdc = 0,	//��С��������
	"charactor.maxdc",	//charactor_maxdc = 1,	//�����������
	"charactor.ac",		//charactor_ac = 2,	//���������
	"charactor.mac",		//charactor_mac = 3,	//����������
	"charactor.crit",	//charactor_crit = 4,	//�����ػ���
	"charactor.skillcrit",//charactor_skillcrit = 5,	//�����ػ���
	"charactor.hitchance",//charactor_hitchance = 6,	//����
	"charactor.hedge",	//charactor_hedge = 7,	//����
	"charactor.hpreg",	//charactor_hpreg = 8,	//�����ָ��ٶ�
	"charactor.mpreg",	//charactor_mpreg = 9,	//�����ָ��ٶ�
	"charactor.attspeed",	//charactor_attspeed = 10,	//�����ٶ�
	"charactor.movespeed",	//charactor_movespeed = 11,	//�ƶ��ٶ�
	"charactor.maxhp",	//charactor_maxhp = 12,	//�������ֵ
	"charactor.maxmp",	//charactor_maxmp = 13,	//�����ֵ
	"charactor.HP",		//charactor_HP = 14,	//��ǰ����ֵ
	"charactor.MP",		//charactor_MP = 15,	//��ǰ����ֵ
	"charactor.state",	//charactor_State = 16,	//��ǰ״̬
	"charactor.silent",	//charactor_silent = 17,	//��Ĭ
	"charactor.Chaotic",	//charactor_Chaotic = 18,	//����
	"charactor.runspeed",//charactor_runspeed = 19,	//�ܲ��ٶ�
	"charactor.level",	//charactor_level = 20,	//�ȼ�
	"str",				//charactor_str = 21,	//����
	"agi",				//charactor_agi = 22,	//����
	"phy",				//charactor_phy = 23,	//����
	"con",				//charactor_con = 24,	//����
	"int",				//charactor_int = 25,	//�ǻ�
	"charactor.skilleff",//charactor_skilleff = 26,	//�����˺�
	"charactor.attdistance",	//charactor_attdistance = 27,	//��ͨ������Χ
	"charactor.dmgweek",		//charactor_dmgweek = 28,	//�˺�����
	"charactor.dmgreturn",	//charactor_dmgreturn = 29,	//�˺�����
	"charactor.double_rate",	//charactor_double = 30,	//���м���
	"charactor.crazy",	//charactor_crazy = 31,	//��սʿ����
	"charactor.dizzy",	//charactor_dizzy = 32,	//���μ���
	"charactor.EatMp",	//charactor_EatMp = 33,	//��ħ����
	"charactor.EatHp",	//charactor_EatHp = 34,	//��Ѫ����
	"charactor.Mpreturn",//charactor_Mpreturn = 35,	//������
	"charactor.qx",		//charactor_qx = 36,	//Ǳ��
	"charactor.stand",		//charactor_stand = 37,	//վ���뾶
	"charactor.camouflage",	//charactor_camouflage = 38,	//αװ
	"charactor.sense",		//charactor_sense = 39,	//���
	"charactor.isolation",	//charactor_isolation = 40,	//����
	"charactor.immunity",	//charactor_immunity = 41,	//����
	"charactor.predictmarker",	//charactor_predictmarker = 42,	//Ԥ�������
	"charactor.manaburnmarker",	//charactor_manaburnmarker = 43,	//�������ձ��
	NULL,	//charactor_skillriseID = 44,		//���ܸ���ID
	NULL,	//charactor_skillrisepunish = 45,	//���ܸ���ͷ�����
	NULL,	//charactor_skillriseHP = 46,		//���ܸ���HP
	NULL,	//charactor_skillriseMP = 47,		//���ܸ���MP
	NULL,	//charactor_propriseID = 48,		//���߸���ID
	NULL,	//charactor_proprisepunish = 49,	//���߸���ͷ�����
	NULL,	//charactor_propriseHP = 50,		//���߸���HP
	NULL,	//charactor_propriseMP = 51,		//���߸���MP
	"charactor.manaburnmarker",	//charactor_poseID = 52,	//αװID
	NULL,	//charactor_exppunish = 53,	//����ͷ�
	"charactor.manaburnmarker",	//charactor_nomove = 54,	//����
	NULL,	//charactor_zhsleastdcappend = 55,	//�ٻ�����С������������
	NULL,	//charactor_zhsmaxdcappend = 56,	//�ٻ������������������
	NULL,	//charactor_zhsacappend = 57,		//�ٻ����������������
	NULL,	//charactor_sunmmonsmarker = 58,	//�ٻ��ޱ��
	NULL,	//player_dbid = 59,	//�ʺ�ID
	"nickname",				//player_nickname = 60,	//�ǳ�
	"exp",					//player_exp = 61,	//����ֵ
	"title",				//player_title = 62,	//�ƺ�
	"job",					//player_job = 63,	//ְҵ
	NULL,	//player_money = 64,	//δʹ��
	NULL,	//player_gold = 65,	//Ԫ��
	"nExpFactor",			//player_ExpFactor = 66,	//���鱶��
	"bag_num",				//player_BagNum = 67,	//��������
	"depot_num",			//player_DepotPage = 68,	//�ֿ�����
	"pk",					//player_PK = 69,	//PKֵ
	NULL,	//player_mailbox = 70,	//����
	NULL,	//player_party = 71,	//���ڶ���ID
	NULL,	//player_guild = 72,	//���ڹ���ID
	NULL,	//player_marry = 73,	//����ID
	NULL,	//player_study = 74,	//ʦͽID
	NULL,	//player_brothor = 75,	//���ID
	"pk_mode",		//player_PKmode = 76,	//PKģʽ
	"soul",			//player_soul = 77,	//��
	"unusepoint",	//player_unusepoint = 78,	//δ�������
	"sex",			//player_sexy = 79,	//�Ա�
	"nuplevelexp",	//player_uplevelexp = 80,	//�������辭��
	"magicfind",	//player_magicfind = 81,	//Ѱ����
	NULL,	//player_rwtimes = 82,	//�������
	NULL,	//player_wallowstate = 83,	//������״̬
	NULL,	//player_count_call1 = 84,	//�ƺż�����1
	NULL,	//player_count_call2 = 85,	//�ƺż�����2
	NULL,	//player_count_call3 = 86,	//�ƺż�����3
	NULL,	//player_count_call4 = 87,	//�ƺż�����4
	NULL,	//player_count_call5 = 88,	//�ƺż�����5
	NULL,	//player_count_call6 = 89,	//�ƺż�����6
	NULL,	//player_count_call7 = 90,	//�ƺż�����7
	NULL,	//player_count_call8 = 91,	//�ƺż�����8
	NULL,	//player_count_call9 = 92,	//�ƺż�����9
	NULL,	//player_count_call10 = 93,	//�ƺż�����10
	NULL,	//player_count_call11 = 94,	//�ƺż�����11
	"slient",			//player_slient = 95,	//����
	NULL,	//player_gminvisible = 96,	//GM����
	NULL,	//player_gmunbreakable = 97,	//GM�޵�
	NULL,	//player_fbcd = 98,	//������ȴʱ��
	NULL,	//player_ridelv = 99,	//��˼���
	NULL,	//player_ridetype = 100,	//�������
	NULL,	//player_standingroleintegral = 101,	//�ճ��������
	"gongde",			//player_gongde = 102,	//����
	"aixinzhi",			//player_aixinzhi = 103,	//����ֵ
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
	"pinzhi",	//equipmake_ID = 0,	//ְҵ
	"adder",	//equipmake_pinzhi = 1,	//Ʒ��
};
const char * g_lpszEquipMakeFields[] = 
{
	"jiazhi",	//equipmake_jiazhi = 2,	//��ֵ
	"wumian",	//equipmake_wumian = 3,	//����
	"momian",	//equipmake_momian = 4,	//ħ��
	"pingjunshangmian",	//equipmake_pingjunshangmian = 5,	//ƽ������
	"desc",		//equipmake_kanzhefangbian = 6,	//���ŷ���
};
const char * g_lpszEquipBuWeiIndices[] = 
{
	"equip_buwei"
};
const char * g_lpszEquipBuWeiFields[] = 
{
	"buweixishu",	//buweixishu_buweixishu = 1,	//��λϵ��
	"pricexishu"	//buweixishu_pricexishu = 2,	//�۸�ϵ��
};










int _tmain(int argc, _TCHAR* argv[])
{
	// ֧�ֿ�
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

	// ���ݸ��ڵ�
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
		
		// ģ��ڵ�	һ����һ��json�ļ�
		const google::protobuf::FieldDescriptor * df = pRoot->GetDescriptor()->FindFieldByName("templates");
		google::protobuf::Message * pTemplateRoot = pRoot->GetReflection()->MutableMessage(pRoot,df);
		if(pTemplateRoot)
		{
			// װ��ģ��
			//ImportTemplate(logicEngine.GetWorld(),pTemplateRoot,"equips",Logic::Unit_equip,g_lpszEquipFields,Logic::equip_level);

		}

		// һ��ϵͳһ��json�ļ�
		// ����ϵͳ
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


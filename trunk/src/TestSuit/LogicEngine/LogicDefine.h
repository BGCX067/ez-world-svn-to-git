/////////////////////////////////////////////////////////////////////////////
// �˴������߼��༭������,�벻Ҫ�ֹ��༭�ⲿ�ִ���
//	��������:2012-04-28 11:02:22
/////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(1)
namespace Logic
{
	// ��ԪIDö��
	enum Unit_IDs {
		Unit_base = 1,	//������Ԫ
		Unit_guild = 11,	//����
		Unit_charactor = 13,	//��ɫ
		Unit_exp = 14,	//�ȼ������
		Unit_set = 15,	//��װ
		Unit_worldnum = 16,	//ȫ�ֱ���
		Unit_playertitle = 18,	//�ƺ�
		Unit_faceact = 21,	//���鶯��
		Unit_currencyact = 22,	//ͨ�ö���
		Unit_item = 2,	//��Ʒ��Ԫ
		Unit_map = 9,	//��ͼ
		Unit_mine = 10,	//��
		Unit_skill = 12,	//��Ҽ���
		Unit_brotherbuff = 24,	//���Buff
		Unit_petadd = 26,	//��������
		Unit_petskill = 28,	//�����ؼ�
		Unit_equip = 5,	//װ��
		Unit_prop = 6,	//����
		Unit_gem = 7,	//��ʯ
		Unit_talisman = 8,	//����
		Unit_jinglian = 17,	//����
		Unit_firstnamefb = 19,	//����ǰ׺
		Unit_lastnamefb = 20,	//������׺
		Unit_pet = 25,	//����
		Unit_pethabit = 27,	//����ϰ��
		Unit_dress = 23,	//ʱװ
		Unit_player = 3,	//���
		Unit_NPC = 4,	//NPC
	};
	//////////////////////////////////////////////////////////////
	// ��Ԫ ������Ԫ ID = 1
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum base_Fields {
		base_ID = 0,	//ID
		base_Name = 1,	//����
		base_Des = 2,	//����
		base_Max = 3
	};
	// ����ģ��ṹ
	struct base_t {
		int	m_nID;	//ID
		char	m_szName[32];	//����
		char	m_szDes[128];	//����
	};
	// ����ʵ���ṹ
	struct base_i {
	};
#ifdef base_instance 
	int nbase_iOffset[3] = {
		-1,-1,-1	};
#endif //base_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���� ID = 11
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum guild_Fields {
		guild_ID = 0,	//ID
		guild_NAME = 1,	//����
		guild_miaoshu = 2,	//����
		guild_LV = 3,	//����
		guild_NEXTLV = 4,	//��һ����
		guild_Max = 5
	};
	// ����ģ��ṹ
	struct guild_t {
		int	m_nID;	//ID
		char	m_szNAME[32];	//����
		char	m_szmiaoshu[128];	//����
		int	m_nLV;	//����
		int	m_nNEXTLV;	//��һ����
	};
	// ����ʵ���ṹ
	struct guild_i {
	};
#ifdef guild_instance 
	int nguild_iOffset[5] = {
		-1,-1,-1,-1,-1	};
#endif //guild_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��ɫ ID = 13
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum charactor_Fields {
		charactor_leastdc = 0,	//��С��������
		charactor_maxdc = 1,	//�����������
		charactor_ac = 2,	//���������
		charactor_mac = 3,	//����������
		charactor_crit = 4,	//�����ػ���
		charactor_skillcrit = 5,	//�����ػ���
		charactor_hitchance = 6,	//����
		charactor_hedge = 7,	//����
		charactor_hpreg = 8,	//�����ָ��ٶ�
		charactor_mpreg = 9,	//�����ָ��ٶ�
		charactor_attspeed = 10,	//�����ٶ�
		charactor_movespeed = 11,	//�ƶ��ٶ�
		charactor_maxhp = 12,	//�������ֵ
		charactor_maxmp = 13,	//�����ֵ
		charactor_HP = 14,	//��ǰ����ֵ
		charactor_MP = 15,	//��ǰ����ֵ
		charactor_State = 16,	//��ǰ״̬
		charactor_silent = 17,	//��Ĭ
		charactor_Chaotic = 18,	//����
		charactor_runspeed = 19,	//�ܲ��ٶ�
		charactor_level = 20,	//�ȼ�
		charactor_str = 21,	//����
		charactor_agi = 22,	//����
		charactor_phy = 23,	//����
		charactor_con = 24,	//����
		charactor_int = 25,	//�ǻ�
		charactor_skilleff = 26,	//�����˺�
		charactor_attdistance = 27,	//��ͨ������Χ
		charactor_dmgweek = 28,	//�˺�����
		charactor_dmgreturn = 29,	//�˺�����
		charactor_double = 30,	//���м���
		charactor_crazy = 31,	//��սʿ����
		charactor_dizzy = 32,	//���μ���
		charactor_EatMp = 33,	//��ħ����
		charactor_EatHp = 34,	//��Ѫ����
		charactor_Mpreturn = 35,	//������
		charactor_qx = 36,	//Ǳ��
		charactor_stand = 37,	//վ���뾶
		charactor_camouflage = 38,	//αװ
		charactor_sense = 39,	//���
		charactor_isolation = 40,	//����
		charactor_immunity = 41,	//����
		charactor_predictmarker = 42,	//Ԥ�������
		charactor_manaburnmarker = 43,	//�������ձ��
		charactor_skillriseID = 44,	//���ܸ���ID
		charactor_skillrisepunish = 45,	//���ܸ���ͷ�����
		charactor_skillriseHP = 46,	//���ܸ���HP
		charactor_skillriseMP = 47,	//���ܸ���MP
		charactor_propriseID = 48,	//���߸���ID
		charactor_proprisepunish = 49,	//���߸���ͷ�����
		charactor_propriseHP = 50,	//���߸���HP
		charactor_propriseMP = 51,	//���߸���MP
		charactor_poseID = 52,	//αװID
		charactor_exppunish = 53,	//����ͷ�
		charactor_nomove = 54,	//����
		charactor_zhsleastdcappend = 55,	//�ٻ�����С������������
		charactor_zhsmaxdcappend = 56,	//�ٻ������������������
		charactor_zhsacappend = 57,	//�ٻ����������������
		charactor_sunmmonsmarker = 58,	//�ٻ��ޱ��
		charactor_Max = 59
	};
	// ������ ��ǰ״̬ ö��ֵ����
	enum charactor_State_Enum {
		charactor_State_0,	//վ��
		charactor_State_1,	//����
		charactor_State_2,	//��Ծ
		charactor_State_3,	//����
		charactor_State_4,	//��������
		charactor_State_5,	//����ʩ��
		charactor_State_6,	//������
		charactor_State_7,	//����
		charactor_State_8,	//���鶯��
		charactor_State_9,	//����
		charactor_State_10,	//������
		charactor_State_11,	//վ��
		charactor_State_12,	//����
		charactor_State_13,	//ѣ��
		charactor_State_14,	//����ײ
		charactor_State_15,	//�ٻ����
		charactor_State_16,	//��˯
		charactor_State_17,	//����
		charactor_StateMax = 18
	};
	// ����ģ��ṹ
	struct charactor_t {
		float	m_fmovespeed;	//�ƶ��ٶ�
		float	m_frunspeed;	//�ܲ��ٶ�
		int	m_nlevel;	//�ȼ�
	};
	// ����ʵ���ṹ
	struct charactor_i {
		int	m_nleastdc;	//��С��������
		int	m_nmaxdc;	//�����������
		int	m_nac;	//���������
		int	m_nmac;	//����������
		int	m_ncrit;	//�����ػ���
		int	m_nskillcrit;	//�����ػ���
		int	m_nhitchance;	//����
		int	m_nhedge;	//����
		int	m_nhpreg;	//�����ָ��ٶ�
		int	m_nmpreg;	//�����ָ��ٶ�
		float	m_fattspeed;	//�����ٶ�
		float	m_fmovespeed;	//�ƶ��ٶ�
		int	m_nmaxhp;	//�������ֵ
		int	m_nmaxmp;	//�����ֵ
		int	m_nHP;	//��ǰ����ֵ
		int	m_nMP;	//��ǰ����ֵ
		charactor_State_Enum	m_eState;	//��ǰ״̬
		BOOL	m_bsilent;	//��Ĭ
		BOOL	m_bChaotic;	//����
		float	m_frunspeed;	//�ܲ��ٶ�
		int	m_nlevel;	//�ȼ�
		int	m_nstr;	//����
		int	m_nagi;	//����
		int	m_nphy;	//����
		int	m_ncon;	//����
		int	m_nint;	//�ǻ�
		int	m_nskilleff;	//�����˺�
		int	m_nattdistance;	//��ͨ������Χ
		int	m_ndmgweek;	//�˺�����
		int	m_ndmgreturn;	//�˺�����
		int	m_ndouble;	//���м���
		int	m_ncrazy;	//��սʿ����
		int	m_ndizzy;	//���μ���
		int	m_nEatMp;	//��ħ����
		int	m_nEatHp;	//��Ѫ����
		int	m_nMpreturn;	//������
		int	m_nqx;	//Ǳ��
		int	m_nstand;	//վ���뾶
		int	m_ncamouflage;	//αװ
		int	m_nsense;	//���
		BOOL	m_bisolation;	//����
		BOOL	m_bimmunity;	//����
		int	m_npredictmarker;	//Ԥ�������
		int	m_nmanaburnmarker;	//�������ձ��
		int	m_nskillriseID;	//���ܸ���ID
		int	m_nskillrisepunish;	//���ܸ���ͷ�����
		int	m_nskillriseHP;	//���ܸ���HP
		int	m_nskillriseMP;	//���ܸ���MP
		int	m_npropriseID;	//���߸���ID
		int	m_nproprisepunish;	//���߸���ͷ�����
		int	m_npropriseHP;	//���߸���HP
		int	m_npropriseMP;	//���߸���MP
		int	m_nposeID;	//αװID
		int	m_nexppunish;	//����ͷ�
		int	m_nnomove;	//����
		int	m_nzhsleastdcappend;	//�ٻ�����С������������
		int	m_nzhsmaxdcappend;	//�ٻ������������������
		int	m_nzhsacappend;	//�ٻ����������������
		int	m_nsunmmonsmarker;	//�ٻ��ޱ��
	};
#ifdef charactor_instance 
	int ncharactor_iOffset[59] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232	};
#endif //charactor_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ �ȼ������ ID = 14
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum exp_Fields {
		exp_level = 0,	//�ȼ�
		exp_exp = 1,	//����ֵ
		exp_gongde = 2,	//����
		exp_Max = 3
	};
	// ����ģ��ṹ
	struct exp_t {
		int	m_nlevel;	//�ȼ�
		int	m_nexp;	//����ֵ
		int	m_ngongde;	//����
	};
	// ����ʵ���ṹ
	struct exp_i {
	};
#ifdef exp_instance 
	int nexp_iOffset[3] = {
		-1,-1,-1	};
#endif //exp_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��װ ID = 15
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum set_Fields {
		set_id = 0,	//ID
		set_name = 1,	//��װ��
		set_Wq = 2,	//����
		set_Yf = 3,	//�·�
		set_Kz = 4,	//����
		set_Tk = 5,	//ͷ��
		set_Jb = 6,	//���
		set_Xz = 7,	//Ь��
		set_St = 8,	//����
		set_Yd = 9,	//����
		set_Sw = 10,	//����
		set_PF = 11,	//����
		set_JZ1 = 12,	//��ָ1
		set_JZ2 = 13,	//��ָ2
		set_XL = 14,	//����
		set_activation1 = 15,	//��������1
		set_activation2 = 16,	//��������2
		set_activation3 = 17,	//��������3
		set_activation4 = 18,	//��������4
		set_effect1 = 19,	//Ч��˵��1
		set_effect2 = 20,	//Ч��˵��2
		set_effect3 = 21,	//Ч��˵��3
		set_effect4 = 22,	//Ч��˵��4
		set_Max = 23
	};
	// ����ģ��ṹ
	struct set_t {
		int	m_nid;	//ID
		char	m_szname[16];	//��װ��
		int	m_nWq;	//����
		int	m_nYf;	//�·�
		int	m_nKz;	//����
		int	m_nTk;	//ͷ��
		int	m_nJb;	//���
		int	m_nXz;	//Ь��
		int	m_nSt;	//����
		int	m_nYd;	//����
		int	m_nSw;	//����
		int	m_nPF;	//����
		int	m_nJZ1;	//��ָ1
		int	m_nJZ2;	//��ָ2
		int	m_nXL;	//����
		int	m_nactivation1;	//��������1
		int	m_nactivation2;	//��������2
		int	m_nactivation3;	//��������3
		int	m_nactivation4;	//��������4
		char	m_szeffect1[128];	//Ч��˵��1
		char	m_szeffect2[128];	//Ч��˵��2
		char	m_szeffect3[128];	//Ч��˵��3
		char	m_szeffect4[128];	//Ч��˵��4
	};
	// ����ʵ���ṹ
	struct set_i {
	};
#ifdef set_instance 
	int nset_iOffset[23] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //set_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ȫ�ֱ��� ID = 16
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum worldnum_Fields {
		worldnum_dkxh = 0,	//������Ľ�Ǯ
		worldnum_bluezq = 1,	//��װ��ǿ
		worldnum_greenzq = 2,	//��װ��ǿ
		worldnum_zisezq = 3,	//��װ��ǿ
		worldnum_pubcooldown = 4,	//������ȴʱ��
		worldnum_mapid = 5,	//�������ͼ
		worldnum_bornx = 6,	//������x
		worldnum_borny = 7,	//������y
		worldnum_dummydays = 8,	//����ʱ��
		worldnum_moneydroprate = 9,	//��Ǯ������
		worldnum_moneydroptimes = 10,	//��Ǯ���䱶��
		worldnum_itemdroprate = 11,	//��Ʒ������
		worldnum_trumpdroprate = 12,	//����������
		worldnum_boxchange = 13,	//���ӱ��Ȩ��ʱ��
		worldnum_boxlast = 14,	//������ʧʱ��
		worldnum_alloctime = 15,	//����ʱ��
		worldnum_fbstr = 16,	//��������
		worldnum_fbdex = 17,	//��������
		worldnum_fbint = 18,	//�����ǻ�
		worldnum_fbcon = 19,	//��������
		worldnum_fbphy = 20,	//��������
		worldnum_fbmatt = 21,	//��������Ч��
		worldnum_fblifere = 22,	//���������ָ�
		worldnum_fbmanare = 23,	//���������ָ�
		worldnum_fbskillcr = 24,	//���������ػ���
		worldnum_diaobaoxishu1 = 25,	//������ϵ��1
		worldnum_diaobaoxishu2 = 26,	//������ϵ��2
		worldnum_healthgametime = 27,	//������Ϸʱ��
		worldnum_faggametime = 28,	//ƣ����Ϸʱ��
		worldnum_healthspacetime = 29,	//������ʾ���ʱ��
		worldnum_fagspacetime = 30,	//ƣ����ʾ���ʱ��
		worldnum_unhealthspacetime = 31,	//�ǽ�����ʾ���ʱ��
		worldnum_leveldropclique = 32,	//�ȼ�������ֵ
		worldnum_zuduifanwei = 33,	//��ӷ�Χ
		worldnum_youzi = 34,	//����
		worldnum_GOLDshangxian = 35,	//�����Ͳֿ��Ǯ����
		worldnum_jingliandroprate = 36,	//�������������
		worldnum_Gemdroprate = 37,	//��ʯ������
		worldnum_daojudroprate = 38,	//���ߵ�����
		worldnum_smithingdroprate = 39,	//������������
		worldnum_protectLV = 40,	//���ֱ�������
		worldnum_guardpatrolradius = 41,	//����Ѳ�߰뾶
		worldnum_randomgreen = 42,	//�Ĳ������
		worldnum_randomblue = 43,	//�Ĳ������
		worldnum_randompurple = 44,	//�Ĳ������
		worldnum_valueparameter = 45,	//�Ĳ��۸����
		worldnum_pcrunspeed = 46,	//����ܲ��ٶ�
		worldnum_pricewhite = 47,	//�۸�ϵ����
		worldnum_pricegreen = 48,	//�۸�ϵ����
		worldnum_priceblue = 49,	//�۸�ϵ����
		worldnum_pricepurple = 50,	//�۸�ϵ����
		worldnum_priceorange = 51,	//�۸�ϵ����
		worldnum_soulshangxian = 52,	//������
		worldnum_xysjsxglone = 53,	//ϡ��������Ը���1
		worldnum_xysjsxgltow = 54,	//ϡ��������Ը���2
		worldnum_infotimes = 55,	//��ʾ��Ϣ����ʱ��
		worldnum_deadnom = 56,	//��������ͷ���ͨ
		worldnum_deadred = 57,	//��������ͷ�����
		worldnum_deadpve = 58,	//�����;óͷ�PVE
		worldnum_deadpvp = 59,	//�����;óͷ�PVP
		worldnum_arearesettime = 60,	//��������ʱ��
		worldnum_changetargetadds = 61,	//�л�Ŀ�꼸��
		worldnum_tabdistance = 62,	//TABѡ�����
		worldnum_pugreen = 63,	//����
		worldnum_pubule = 64,	//����
		worldnum_pupurple = 65,	//����
		worldnum_jingbule = 66,	//����
		worldnum_jingpurple = 67,	//����
		worldnum_bossbule = 68,	//BOSS��
		worldnum_bosszpurple = 69,	//BOSS��
		worldnum_greensoul = 70,	//�̻걶��
		worldnum_bluesoul = 71,	//���걶��
		worldnum_purplesoul = 72,	//�ϻ걶��
		worldnum_equipsale = 73,	//װ������
		worldnum_equipup = 74,	//װ������
		worldnum_equiprepair = 75,	//װ������
		worldnum_lvzsjxs = 76,	//��װ�������
		worldnum_lzsjxs = 77,	//��װ�������
		worldnum_zzsjxs = 78,	//��װ�������
		worldnum_czsjxs = 79,	//��װ�������
		worldnum_chatarea = 80,	//���췶Χ
		worldnum_monestchangetarget = 81,	//�����л�Ŀ�꼸��
		worldnum_bbmoveradii = 82,	//�ٻ��޻�뾶
		worldnum_bbwaitradii = 83,	//�ٻ��޴����뾶
		worldnum_bbstopfollowradii = 84,	//�ٻ���ֹͣ����뾶
		worldnum_targetholdtime = 85,	//Ŀ�걣��ʱ��
		worldnum_severbattletime = 86,	//����ս��ʱ��
		worldnum_ksjysqgxxczsj = 87,	//��ʯ����������Ϣ����ʱ��
		worldnum_qxdzjsj = 88,	//���ܶ�����ʱ��
		worldnum_kcxxdrdc = 89,	//����Ϣ���뵭��
		worldnum_gwsysj = 90,	//��λ˲��ʱ��
		worldnum_kcswsxjg = 91,	//������ˢ�¼��
		worldnum_fbbcsj = 92,	//��������ʱ��
		worldnum_fbdjs = 93,	//��������ʱ
		worldnum_monsterstandtime = 94,	//����վ��ʱ��
		worldnum_monstermovetime = 95,	//�����߶�ʱ��
		worldnum_newplayerlevel = 96,	//���ֽ����ȼ�
		worldnum_ptspeed = 97,	//��ͨ���������ٶ�
		worldnum_rmbspeed = 98,	//�շ����������ٶ�
		worldnum_shaguaigongde = 99,	//ɱ�ֹ���
		worldnum_baishidengji = 100,	//��ʦ�ȼ�
		worldnum_expsize = 101,	//���鱶��
		worldnum_zbsjcgbl = 102,	//װ�������ɹ�����
		worldnum_dkcgbl = 103,	//��׳ɹ�����
		worldnum_dlglbl = 104,	//������ʱ���
		worldnum_petdroprate = 105,	//����������
		worldnum_petpublue = 106,	//������
		worldnum_petpupurple = 107,	//������
		worldnum_petjingbule = 108,	//�辫��
		worldnum_petjinggreen = 109,	//�辫��
		worldnum_petbosspurple = 110,	//��BOSS��
		worldnum_petbossorange = 111,	//��BOSS��
		worldnum_petmoveradii = 112,	//�����뾶
		worldnum_petwaitradii = 113,	//��������뾶
		worldnum_petstopfollowradii = 114,	//����ֹͣ����뾶
		worldnum_aixinzhizhuanhuanlv = 115,	//��Ұ���ֵת����
		worldnum_chongwushengjixishu = 116,	//��������ϵ��
		worldnum_Max = 117
	};
	// ����ģ��ṹ
	struct worldnum_t {
		int	m_ndkxh;	//������Ľ�Ǯ
		int	m_nbluezq;	//��װ��ǿ
		int	m_ngreenzq;	//��װ��ǿ
		int	m_nzisezq;	//��װ��ǿ
		int	m_npubcooldown;	//������ȴʱ��
		int	m_nmapid;	//�������ͼ
		int	m_nbornx;	//������x
		int	m_nborny;	//������y
		int	m_ndummydays;	//����ʱ��
		int	m_nmoneydroprate;	//��Ǯ������
		int	m_nmoneydroptimes;	//��Ǯ���䱶��
		int	m_nitemdroprate;	//��Ʒ������
		int	m_ntrumpdroprate;	//����������
		int	m_nboxchange;	//���ӱ��Ȩ��ʱ��
		int	m_nboxlast;	//������ʧʱ��
		int	m_nalloctime;	//����ʱ��
		float	m_ffbstr;	//��������
		float	m_ffbdex;	//��������
		float	m_ffbint;	//�����ǻ�
		float	m_ffbcon;	//��������
		float	m_ffbphy;	//��������
		float	m_ffbmatt;	//��������Ч��
		float	m_ffblifere;	//���������ָ�
		float	m_ffbmanare;	//���������ָ�
		float	m_ffbskillcr;	//���������ػ���
		float	m_fdiaobaoxishu1;	//������ϵ��1
		float	m_fdiaobaoxishu2;	//������ϵ��2
		int	m_nhealthgametime;	//������Ϸʱ��
		int	m_nfaggametime;	//ƣ����Ϸʱ��
		int	m_nhealthspacetime;	//������ʾ���ʱ��
		int	m_nfagspacetime;	//ƣ����ʾ���ʱ��
		int	m_nunhealthspacetime;	//�ǽ�����ʾ���ʱ��
		int	m_nleveldropclique;	//�ȼ�������ֵ
		int	m_nzuduifanwei;	//��ӷ�Χ
		int	m_nyouzi;	//����
		int	m_nGOLDshangxian;	//�����Ͳֿ��Ǯ����
		int	m_njingliandroprate;	//�������������
		int	m_nGemdroprate;	//��ʯ������
		int	m_ndaojudroprate;	//���ߵ�����
		int	m_nsmithingdroprate;	//������������
		int	m_nprotectLV;	//���ֱ�������
		int	m_nguardpatrolradius;	//����Ѳ�߰뾶
		int	m_nrandomgreen;	//�Ĳ������
		int	m_nrandomblue;	//�Ĳ������
		int	m_nrandompurple;	//�Ĳ������
		int	m_nvalueparameter;	//�Ĳ��۸����
		float	m_fpcrunspeed;	//����ܲ��ٶ�
		int	m_npricewhite;	//�۸�ϵ����
		int	m_npricegreen;	//�۸�ϵ����
		int	m_npriceblue;	//�۸�ϵ����
		int	m_npricepurple;	//�۸�ϵ����
		int	m_npriceorange;	//�۸�ϵ����
		int	m_nsoulshangxian;	//������
		int	m_nxysjsxglone;	//ϡ��������Ը���1
		int	m_nxysjsxgltow;	//ϡ��������Ը���2
		int	m_ninfotimes;	//��ʾ��Ϣ����ʱ��
		int	m_ndeadnom;	//��������ͷ���ͨ
		int	m_ndeadred;	//��������ͷ�����
		int	m_ndeadpve;	//�����;óͷ�PVE
		int	m_ndeadpvp;	//�����;óͷ�PVP
		char	m_szarearesettime[4];	//��������ʱ��
		char	m_szchangetargetadds[4];	//�л�Ŀ�꼸��
		int	m_ntabdistance;	//TABѡ�����
		float	m_fpugreen;	//����
		float	m_fpubule;	//����
		float	m_fpupurple;	//����
		float	m_fjingbule;	//����
		float	m_fjingpurple;	//����
		float	m_fbossbule;	//BOSS��
		float	m_fbosszpurple;	//BOSS��
		int	m_ngreensoul;	//�̻걶��
		int	m_nbluesoul;	//���걶��
		int	m_npurplesoul;	//�ϻ걶��
		float	m_fequipsale;	//װ������
		float	m_fequipup;	//װ������
		float	m_fequiprepair;	//װ������
		int	m_nlvzsjxs;	//��װ�������
		int	m_nlzsjxs;	//��װ�������
		int	m_nzzsjxs;	//��װ�������
		int	m_nczsjxs;	//��װ�������
		int	m_nchatarea;	//���췶Χ
		int	m_nmonestchangetarget;	//�����л�Ŀ�꼸��
		int	m_nbbmoveradii;	//�ٻ��޻�뾶
		int	m_nbbwaitradii;	//�ٻ��޴����뾶
		int	m_nbbstopfollowradii;	//�ٻ���ֹͣ����뾶
		int	m_ntargetholdtime;	//Ŀ�걣��ʱ��
		int	m_nseverbattletime;	//����ս��ʱ��
		int	m_nksjysqgxxczsj;	//��ʯ����������Ϣ����ʱ��
		int	m_nqxdzjsj;	//���ܶ�����ʱ��
		int	m_nkcxxdrdc;	//����Ϣ���뵭��
		int	m_ngwsysj;	//��λ˲��ʱ��
		int	m_nkcswsxjg;	//������ˢ�¼��
		int	m_nfbbcsj;	//��������ʱ��
		int	m_nfbdjs;	//��������ʱ
		int	m_nmonsterstandtime;	//����վ��ʱ��
		int	m_nmonstermovetime;	//�����߶�ʱ��
		int	m_nnewplayerlevel;	//���ֽ����ȼ�
		int	m_nptspeed;	//��ͨ���������ٶ�
		int	m_nrmbspeed;	//�շ����������ٶ�
		int	m_nshaguaigongde;	//ɱ�ֹ���
		int	m_nbaishidengji;	//��ʦ�ȼ�
		int	m_nexpsize;	//���鱶��
		int	m_nzbsjcgbl;	//װ�������ɹ�����
		int	m_ndkcgbl;	//��׳ɹ�����
		int	m_ndlglbl;	//������ʱ���
		int	m_npetdroprate;	//����������
		int	m_npetpublue;	//������
		int	m_npetpupurple;	//������
		int	m_npetjingbule;	//�辫��
		int	m_npetjinggreen;	//�辫��
		int	m_npetbosspurple;	//��BOSS��
		int	m_npetbossorange;	//��BOSS��
		int	m_npetmoveradii;	//�����뾶
		int	m_npetwaitradii;	//��������뾶
		int	m_npetstopfollowradii;	//����ֹͣ����뾶
		int	m_naixinzhizhuanhuanlv;	//��Ұ���ֵת����
		int	m_nchongwushengjixishu;	//��������ϵ��
	};
	// ����ʵ���ṹ
	struct worldnum_i {
	};
#ifdef worldnum_instance 
	int nworldnum_iOffset[117] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //worldnum_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ �ƺ� ID = 18
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum playertitle_Fields {
		playertitle_ID = 0,	//ID
		playertitle_Name = 1,	//�ƺ�����
		playertitle_Replace = 2,	//�ƺ��滻��
		playertitle_Color = 3,	//�ƺ���ɫ
		playertitle_Explain = 4,	//�ƺ�����
		playertitle_shifouyongjiu = 5,	//�Ƿ�����
		playertitle_chenghaolv = 6,	//�ƺŵȼ�
		playertitle_suoshubiaoqianye = 7,	//������ǩҳ
		playertitle_cfms = 8,	//��������
		playertitle_xgms = 9,	//Ч������
		playertitle_Max = 10
	};
	// ������ ������ǩҳ ö��ֵ����
	enum playertitle_suoshubiaoqianye_Enum {
		playertitle_suoshubiaoqianye_0,	//һ��
		playertitle_suoshubiaoqianye_1,	//����
		playertitle_suoshubiaoqianye_2,	//ս��
		playertitle_suoshubiaoqianye_3,	//����
		playertitle_suoshubiaoqianyeMax = 4
	};
	// ����ģ��ṹ
	struct playertitle_t {
		int	m_nID;	//ID
		char	m_szName[64];	//�ƺ�����
		char	m_szColor[9];	//�ƺ���ɫ
		char	m_szExplain[128];	//�ƺ�����
		BOOL	m_bshifouyongjiu;	//�Ƿ�����
		int	m_nchenghaolv;	//�ƺŵȼ�
		playertitle_suoshubiaoqianye_Enum	m_esuoshubiaoqianye;	//������ǩҳ
		char	m_szcfms[128];	//��������
		char	m_szxgms[128];	//Ч������
	};
	// ����ʵ���ṹ
	struct playertitle_i {
		char	m_szReplace[32];	//�ƺ��滻��
	};
#ifdef playertitle_instance 
	int nplayertitle_iOffset[10] = {
		-1,-1,0,-1,-1,-1,-1,-1,-1,-1	};
#endif //playertitle_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���鶯�� ID = 21
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum faceact_Fields {
		faceact_idname = 0,	//ID
		faceact_name = 1,	//����
		faceact_depict = 2,	//����
		faceact_Max = 3
	};
	// ����ģ��ṹ
	struct faceact_t {
		int	m_nidname;	//ID
		char	m_szname[16];	//����
		char	m_szdepict[128];	//����
	};
	// ����ʵ���ṹ
	struct faceact_i {
	};
#ifdef faceact_instance 
	int nfaceact_iOffset[3] = {
		-1,-1,-1	};
#endif //faceact_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ͨ�ö��� ID = 22
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum currencyact_Fields {
		currencyact_idname = 0,	//ID
		currencyact_name = 1,	//����
		currencyact_depict = 2,	//����
		currencyact_Max = 3
	};
	// ����ģ��ṹ
	struct currencyact_t {
		int	m_nidname;	//ID
		char	m_szname[16];	//����
		char	m_szdepict[128];	//����
	};
	// ����ʵ���ṹ
	struct currencyact_i {
	};
#ifdef currencyact_instance 
	int ncurrencyact_iOffset[3] = {
		-1,-1,-1	};
#endif //currencyact_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��Ʒ��Ԫ ID = 2
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum item_Fields {
		item_Buy = 3,	//����
		item_Sell = 4,	//����
		item_Change = 5,	//����
		item_Destory = 6,	//����
		item_Overlap = 7,	//����
		item_Time = 8,	//ӵ��ʱ��
		item_Moneytype = 9,	//����ҵ���
		item_Buyprice = 10,	//�����
		item_Sellprice = 11,	//���ۼ�
		item_BandType = 12,	//������
		item_BandFlag = 13,	//�󶨱��
		item_DeleFlag = 14,	//ɾ�����
		item_Max = 15
	};
	// ������ ����ҵ��� ö��ֵ����
	enum item_Moneytype_Enum {
		item_Moneytype_0,	//��ͨ
		item_Moneytype_1,	//�����
		item_Moneytype_2,	//����
		item_Moneytype_3,	//����
		item_MoneytypeMax = 4
	};
	// ������ ������ ö��ֵ����
	enum item_BandType_Enum {
		item_BandType_0,	//���ܰ�
		item_BandType_1,	//�ֶ���
		item_BandType_2,	//ʹ�ð�
		item_BandTypeMax = 3
	};
	// ����ģ��ṹ
	struct item_t : public base_t {
		BOOL	m_bBuy;	//����
		BOOL	m_bSell;	//����
		BOOL	m_bChange;	//����
		BOOL	m_bDestory;	//����
		int	m_nOverlap;	//����
		int	m_nTime;	//ӵ��ʱ��
		item_Moneytype_Enum	m_eMoneytype;	//����ҵ���
		int	m_nBuyprice;	//�����
		int	m_nSellprice;	//���ۼ�
		item_BandType_Enum	m_eBandType;	//������
	};
	// ����ʵ���ṹ
	struct item_i : public base_i {
		int	m_nTime;	//ӵ��ʱ��
		int	m_nBuyprice;	//�����
		int	m_nSellprice;	//���ۼ�
		BOOL	m_bBandFlag;	//�󶨱��
		BOOL	m_bDeleFlag;	//ɾ�����
	};
#ifdef item_instance 
	int nitem_iOffset[15] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16	};
#endif //item_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��ͼ ID = 9
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum map_Fields {
		map_file = 3,	//�ļ���ַ
		map_mappath = 4,	//С��ͼ·��
		map_width = 5,	//С��ͼ��
		map_height = 6,	//С��ͼ��
		map_cell = 7,	//С��ͼcell
		map_type = 8,	//��ͼ����
		map_serverID = 9,	//����������
		map_jinrucishu = 10,	//����ÿ�ս����������
		map_Max = 11
	};
	// ������ ��ͼ���� ö��ֵ����
	enum map_type_Enum {
		map_type_0,	//��ͨ
		map_type_1,	//����
		map_type_2,	//δ����
		map_typeMax = 3
	};
	// ����ģ��ṹ
	struct map_t : public base_t {
		char	m_szfile[128];	//�ļ���ַ
		char	m_szmappath[128];	//С��ͼ·��
		int	m_nwidth;	//С��ͼ��
		int	m_nheight;	//С��ͼ��
		int	m_ncell;	//С��ͼcell
		map_type_Enum	m_etype;	//��ͼ����
		int	m_njinrucishu;	//����ÿ�ս����������
	};
	// ����ʵ���ṹ
	struct map_i : public base_i {
		int	m_nserverID;	//����������
	};
#ifdef map_instance 
	int nmap_iOffset[11] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1	};
#endif //map_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ �� ID = 10
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum mine_Fields {
		mine_chanpin = 3,	//��Ʒ
		mine_chanliang = 4,	//����
		mine_level = 5,	//����
		mine_nextid = 6,	//��һ��ID
		mine_kuangcname = 7,	//������
		mine_MapID = 8,	//��ͼID
		mine_areaID = 9,	//������
		mine_X = 10,	//x����
		mine_Y = 11,	//y����
		mine_W = 12,	//���
		mine_H = 13,	//�߶�
		mine_GuildID = 14,	//��������
		mine_CountDown = 15,	//��������ʱ
		mine_NpcCnt = 16,	//��ǰ����NPC����
		mine_MaxNpc = 17,	//����NPC��������
		mine_LVUPGOLD = 18,	//������Ǯ
		mine_QX = 19,	//����X����
		mine_QY = 20,	//����Y����
		mine_Max = 21
	};
	// ����ģ��ṹ
	struct mine_t : public base_t {
		int	m_nchanpin;	//��Ʒ
		int	m_nchanliang;	//����
		int	m_nlevel;	//����
		int	m_nnextid;	//��һ��ID
		char	m_szkuangcname[16];	//������
		int	m_nMapID;	//��ͼID
		int	m_nareaID;	//������
		int	m_nX;	//x����
		int	m_nY;	//y����
		int	m_nW;	//���
		int	m_nH;	//�߶�
		int	m_nGuildID;	//��������
		int	m_nNpcCnt;	//��ǰ����NPC����
		int	m_nMaxNpc;	//����NPC��������
		int	m_nLVUPGOLD;	//������Ǯ
		int	m_nQX;	//����X����
		int	m_nQY;	//����Y����
	};
	// ����ʵ���ṹ
	struct mine_i : public base_i {
		int	m_nchanliang;	//����
		int	m_nGuildID;	//��������
		int	m_nCountDown;	//��������ʱ
		int	m_nNpcCnt;	//��ǰ����NPC����
	};
#ifdef mine_instance 
	int nmine_iOffset[21] = {
		-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,8,12,-1,-1,-1,-1	};
#endif //mine_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��Ҽ��� ID = 12
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum skill_Fields {
		skill_skilltarget = 3,	//����Ŀ��
		skill_skilltype = 4,	//��������
		skill_skillcrit = 5,	//�Ƿ��ػ�
		skill_singtime = 6,	//����ʱ��
		skill_cooldown = 7,	//��ȴʱ��
		skill_freetime = 8,	//�ͷ�ʱ��
		skill_freedistance = 9,	//�ͷž���
		skill_fly = 10,	//�Ƿ����
		skill_exp = 11,	//����ѧϰ����
		skill_skillbz = 12,	//���ܱ�־
		skill_skillcs = 13,	//��������
		skill_skillcast = 14,	//��������
		skill_group = 15,	//����
		skill_dztx = 16,	//������Ч
		skill_castway = 17,	//������ʽ
		skill_lazztime = 18,	//�ӳ�ʩ��
		skill_castrange = 19,	//�����뾶
		skill_levelask = 20,	//Ҫ��ȼ�
		skill_arealasttime = 21,	//����ά��ʱ��
		skill_arealastpre = 22,	//����ά��Ƶ��
		skill_repeatreleasetime = 23,	//�ظ��ͷ�ʱ��
		skill_autoatk = 24,	//�Զ�����
		skill_skillinterval = 25,	//���ܼ��
		skill_noact = 26,	//�޶���
		skill_Max = 27
	};
	// ������ ����Ŀ�� ö��ֵ����
	enum skill_skilltarget_Enum {
		skill_skilltarget_0,	//����
		skill_skilltarget_1,	//��������
		skill_skilltarget_2,	//Ⱥ��
		skill_skilltarget_3,	//����
		skill_skilltarget_4,	//����
		skill_skilltargetMax = 5
	};
	// ������ �������� ö��ֵ����
	enum skill_skilltype_Enum {
		skill_skilltype_0,	//����
		skill_skilltype_1,	//����
		skill_skilltypeMax = 2
	};
	// ������ ���ܱ�־ ö��ֵ����
	enum skill_skillbz_Enum {
		skill_skillbz_0,	//ս������
		skill_skillbz_1,	//��������
		skill_skillbz_2,	//���Ἴ��
		skill_skillbz_3,	//���＼��
		skill_skillbz_4,	//���＼��
		skill_skillbzMax = 5
	};
	// ������ ������ʽ ö��ֵ����
	enum skill_castway_Enum {
		skill_castway_0,	//ʱ�䴥��
		skill_castway_1,	//�ռ䴥��
		skill_castway_2,	//˫�ش���
		skill_castwayMax = 3
	};
	// ������ �Զ����� ö��ֵ����
	enum skill_autoatk_Enum {
		skill_autoatk_0,	//��Ӱ��
		skill_autoatk_1,	//����
		skill_autoatk_2,	//���
		skill_autoatkMax = 3
	};
	// ����ģ��ṹ
	struct skill_t : public base_t {
		skill_skilltarget_Enum	m_eskilltarget;	//����Ŀ��
		skill_skilltype_Enum	m_eskilltype;	//��������
		BOOL	m_bskillcrit;	//�Ƿ��ػ�
		int	m_nsingtime;	//����ʱ��
		int	m_ncooldown;	//��ȴʱ��
		int	m_nfreetime;	//�ͷ�ʱ��
		int	m_nfreedistance;	//�ͷž���
		BOOL	m_bfly;	//�Ƿ����
		int	m_nexp;	//����ѧϰ����
		skill_skillbz_Enum	m_eskillbz;	//���ܱ�־
		int	m_nskillcs;	//��������
		int	m_nskillcast;	//��������
		int	m_ngroup;	//����
		int	m_ndztx;	//������Ч
		skill_castway_Enum	m_ecastway;	//������ʽ
		int	m_nlazztime;	//�ӳ�ʩ��
		int	m_ncastrange;	//�����뾶
		int	m_nlevelask;	//Ҫ��ȼ�
		int	m_narealasttime;	//����ά��ʱ��
		int	m_narealastpre;	//����ά��Ƶ��
		int	m_nrepeatreleasetime;	//�ظ��ͷ�ʱ��
		skill_autoatk_Enum	m_eautoatk;	//�Զ�����
		int	m_nskillinterval;	//���ܼ��
		BOOL	m_bnoact;	//�޶���
	};
	// ����ʵ���ṹ
	struct skill_i : public base_i {
		skill_skilltarget_Enum	m_eskilltarget;	//����Ŀ��
		skill_skilltype_Enum	m_eskilltype;	//��������
		BOOL	m_bskillcrit;	//�Ƿ��ػ�
		int	m_nsingtime;	//����ʱ��
		int	m_ncooldown;	//��ȴʱ��
		int	m_nfreetime;	//�ͷ�ʱ��
		int	m_nfreedistance;	//�ͷž���
		BOOL	m_bfly;	//�Ƿ����
		int	m_nexp;	//����ѧϰ����
		skill_skillbz_Enum	m_eskillbz;	//���ܱ�־
		int	m_nskillcast;	//��������
	};
#ifdef skill_instance 
	int nskill_iOffset[27] = {
		-1,-1,-1,0,4,8,12,16,20,24,28,32,36,-1,40,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //skill_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���Buff ID = 24
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum brotherbuff_Fields {
		brotherbuff_Max = 3
	};
	// ����ģ��ṹ
	struct brotherbuff_t : public base_t {
	};
	// ����ʵ���ṹ
	struct brotherbuff_i : public base_i {
	};
#ifdef brotherbuff_instance 
	int nbrotherbuff_iOffset[3] = {
		-1,-1,-1	};
#endif //brotherbuff_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ �������� ID = 26
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petadd_Fields {
		petadd_Max = 3
	};
	// ����ģ��ṹ
	struct petadd_t : public base_t {
	};
	// ����ʵ���ṹ
	struct petadd_i : public base_i {
	};
#ifdef petadd_instance 
	int npetadd_iOffset[3] = {
		-1,-1,-1	};
#endif //petadd_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ �����ؼ� ID = 28
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petskill_Fields {
		petskill_skilltarget = 3,	//����Ŀ��
		petskill_cooldowntime = 4,	//��ȴʱ��
		petskill_casttype = 5,	//��������
		petskill_castprobility = 6,	//��������
		petskill_freedistance = 7,	//�ͷž���
		petskill_Max = 8
	};
	// ������ ����Ŀ�� ö��ֵ����
	enum petskill_skilltarget_Enum {
		petskill_skilltarget_0,	//����
		petskill_skilltarget_1,	//Ⱥ��
		petskill_skilltarget_2,	//����Ŀ��
		petskill_skilltarget_3,	//����
		petskill_skilltargetMax = 4
	};
	// ������ �������� ö��ֵ����
	enum petskill_casttype_Enum {
		petskill_casttype_0,	//��ɫ������
		petskill_casttype_1,	//��ɫ���ܹ���
		petskill_casttype_2,	//��ɫ�ܵ�����ͼ��ܹ���
		petskill_casttypeMax = 3
	};
	// ����ģ��ṹ
	struct petskill_t : public base_t {
		petskill_skilltarget_Enum	m_eskilltarget;	//����Ŀ��
		int	m_ncooldowntime;	//��ȴʱ��
		petskill_casttype_Enum	m_ecasttype;	//��������
		int	m_ncastprobility;	//��������
		int	m_nfreedistance;	//�ͷž���
	};
	// ����ʵ���ṹ
	struct petskill_i : public base_i {
		int	m_ncooldowntime;	//��ȴʱ��
		petskill_casttype_Enum	m_ecasttype;	//��������
		int	m_ncastprobility;	//��������
		int	m_nfreedistance;	//�ͷž���
	};
#ifdef petskill_instance 
	int npetskill_iOffset[8] = {
		-1,-1,-1,-1,0,4,8,12	};
#endif //petskill_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ װ�� ID = 5
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum equip_Fields {
		equip_Durabilit = 15,	//��ǰ�;ö�
		equip_class = 16,	//Ʒ��
		equip_level = 17,	//�ȼ�
		equip_maxdurabilit = 18,	//����;ö�
		equip_socket = 19,	//��Ƕ��
		equip_bangding = 20,	//�Ƿ��
		equip_body = 21,	//װ����λ
		equip_moneyup = 22,	//������Ǯ
		equip_soulup = 23,	//������
		equip_job = 24,	//ʹ��ְҵ
		equip_setnum = 25,	//��װ���
		equip_modnum = 26,	//ʹ��ģ��
		equip_gem1 = 27,	//��ʯ��1
		equip_gem2 = 28,	//��ʯ��2
		equip_gem3 = 29,	//��ʯ��3
		equip_gem4 = 30,	//��ʯ��4
		equip_upchance = 31,	//�����ɹ���
		equip_socketchan = 32,	//��׳ɹ���
		equip_finesign = 33,	//�������
		equip_equiplv = 34,	//��Ʒ�ȼ�
		equip_xzcsone = 35,	//��������1
		equip_xzcstwo = 36,	//��������2
		equip_posterid = 37,	//����ID
		equip_xysjone = 38,	//ϡ�����1
		equip_xysjtow = 39,	//ϡ�����2
		equip_sjxszl = 40,	//�����������
		equip_stilettoprice = 41,	//��׼۸�
		equip_sfbxyfjsjxs = 42,	//�Ƿ���Ҫ�����������
		equip_Max = 43
	};
	// ������ Ʒ�� ö��ֵ����
	enum equip_class_Enum {
		equip_class_0,	//��ͨ
		equip_class_1,	//����
		equip_class_2,	//����
		equip_class_3,	//����
		equip_class_4,	//��
		equip_class_5,	//����
		equip_class_6,	//����-2
		equip_class_7,	//����-3
		equip_class_8,	//���
		equip_classMax = 9
	};
	// ������ װ����λ ö��ֵ����
	enum equip_body_Enum {
		equip_body_0,	//ͷ��
		equip_body_1,	//����
		equip_body_2,	//����
		equip_body_3,	//�·�
		equip_body_4,	//����
		equip_body_5,	//����
		equip_body_6,	//����
		equip_body_7,	//����
		equip_body_8,	//Ь��
		equip_body_9,	//����
		equip_body_10,	//����
		equip_body_11,	//��ָ
		equip_body_12,	//ʱװ
		equip_bodyMax = 13
	};
	// ������ ʹ��ְҵ ö��ֵ����
	enum equip_job_Enum {
		equip_job_0,	//�佫
		equip_job_1,	//�̿�
		equip_job_2,	//��ʿ
		equip_job_3,	//��ʦ
		equip_job_4,	//ȫְҵ
		equip_jobMax = 5
	};
	// ����ģ��ṹ
	struct equip_t : public item_t {
		equip_class_Enum	m_eclass;	//Ʒ��
		int	m_nlevel;	//�ȼ�
		int	m_nmaxdurabilit;	//����;ö�
		equip_body_Enum	m_ebody;	//װ����λ
		int	m_nmoneyup;	//������Ǯ
		int	m_nsoulup;	//������
		equip_job_Enum	m_ejob;	//ʹ��ְҵ
		int	m_nsetnum;	//��װ���
		int	m_nmodnum;	//ʹ��ģ��
		int	m_nupchance;	//�����ɹ���
		int	m_nsocketchan;	//��׳ɹ���
		int	m_nequiplv;	//��Ʒ�ȼ�
		float	m_fxzcsone;	//��������1
		float	m_fxzcstwo;	//��������2
		int	m_nposterid;	//����ID
		int	m_nstilettoprice;	//��׼۸�
		BOOL	m_bsfbxyfjsjxs;	//�Ƿ���Ҫ�����������
	};
	// ����ʵ���ṹ
	struct equip_i : public item_i {
		int	m_nDurabilit;	//��ǰ�;ö�
		equip_class_Enum	m_eclass;	//Ʒ��
		int	m_nlevel;	//�ȼ�
		int	m_nmaxdurabilit;	//����;ö�
		int	m_nsocket;	//��Ƕ��
		BOOL	m_bbangding;	//�Ƿ��
		equip_body_Enum	m_ebody;	//װ����λ
		int	m_nmoneyup;	//������Ǯ
		int	m_nsoulup;	//������
		int	m_ngem1;	//��ʯ��1
		int	m_ngem2;	//��ʯ��2
		int	m_ngem3;	//��ʯ��3
		int	m_ngem4;	//��ʯ��4
		int	m_nupchance;	//�����ɹ���
		int	m_nsocketchan;	//��׳ɹ���
		int	m_nfinesign;	//�������
		int	m_nxysjone;	//ϡ�����1
		int	m_nxysjtow;	//ϡ�����2
		int	m_nsjxszl;	//�����������
		int	m_nstilettoprice;	//��׼۸�
	};
#ifdef equip_instance 
	int nequip_iOffset[43] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,44,48,52,-1,-1,-1,56,60,64,68,72,76,80,-1,-1,-1,-1,84,88,92,96,-1	};
#endif //equip_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���� ID = 6
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum prop_Fields {
		prop_colddown = 15,	//��ȴʱ��
		prop_group = 16,	//����
		prop_type = 17,	//����
		prop_class = 18,	//Ʒ��
		prop_icon = 19,	//ͼ��
		prop_useable = 20,	//��ʹ����Ʒ
		prop_onlyeffects = 21,	//Ψһ��Ʒ
		prop_nodisappear = 22,	//����ʧ
		prop_posterid = 23,	//����ID
		prop_act = 24,	//�Ƿ񴥷�����
		prop_Max = 25
	};
	// ������ ���� ö��ֵ����
	enum prop_type_Enum {
		prop_type_0,	//ҩƷ
		prop_type_1,	//��ʯ
		prop_type_2,	//����ͼֽ
		prop_type_3,	//����
		prop_type_4,	//������Ʒ
		prop_type_5,	//���˷���
		prop_type_6,	//�������
		prop_type_7,	//�سǾ���
		prop_type_8,	//����
		prop_type_9,	//�ӻ�
		prop_type_10,	//��������
		prop_type_11,	//����������
		prop_type_12,	//��������
		prop_type_13,	//�������
		prop_type_14,	//������չ��
		prop_type_15,	//�ֿ���չ��
		prop_type_16,	//Ѱ������
		prop_type_17,	//��׷���
		prop_type_18,	//���
		prop_type_19,	//��������
		prop_type_20,	//ǧ�ﴫ��
		prop_type_21,	//�̻�
		prop_type_22,	//��������
		prop_type_23,	//����ˮ
		prop_type_24,	//����ʯ
		prop_type_25,	//��������
		prop_type_26,	//����
		prop_type_27,	//����л�
		prop_type_28,	//�����
		prop_type_29,	//���׷�
		prop_type_30,	//����������
		prop_type_31,	//����ˢ�·�
		prop_type_32,	//�󶨷�
		prop_type_33,	//�����������
		prop_type_34,	//ϰ�Թ�ʵ
		prop_type_35,	//�������
		prop_type_36,	//�����ʵ���
		prop_type_37,	//���Ĵ����
		prop_typeMax = 38
	};
	// ������ Ʒ�� ö��ֵ����
	enum prop_class_Enum {
		prop_class_0,	//��ͨ
		prop_class_1,	//����
		prop_class_2,	//����
		prop_class_3,	//����
		prop_class_4,	//��
		prop_class_5,	//����
		prop_classMax = 6
	};
	// ����ģ��ṹ
	struct prop_t : public item_t {
		int	m_ncolddown;	//��ȴʱ��
		int	m_ngroup;	//����
		prop_type_Enum	m_etype;	//����
		prop_class_Enum	m_eclass;	//Ʒ��
		int	m_nicon;	//ͼ��
		BOOL	m_buseable;	//��ʹ����Ʒ
		BOOL	m_bonlyeffects;	//Ψһ��Ʒ
		BOOL	m_bnodisappear;	//����ʧ
		int	m_nposterid;	//����ID
		BOOL	m_bact;	//�Ƿ񴥷�����
	};
	// ����ʵ���ṹ
	struct prop_i : public item_i {
		int	m_ncolddown;	//��ȴʱ��
	};
#ifdef prop_instance 
	int nprop_iOffset[25] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //prop_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��ʯ ID = 7
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gem_Fields {
		gem_class = 15,	//Ʒ��
		gem_icon = 16,	//ͼ��
		gem_posterid = 17,	//����ID
		gem_huiguangdangci = 18,	//�Թ⵵��
		gem_Max = 19
	};
	// ������ Ʒ�� ö��ֵ����
	enum gem_class_Enum {
		gem_class_0,	//��ͨ
		gem_class_1,	//����
		gem_class_2,	//����
		gem_class_3,	//����
		gem_class_4,	//��
		gem_class_5,	//����
		gem_classMax = 6
	};
	// ����ģ��ṹ
	struct gem_t : public item_t {
		gem_class_Enum	m_eclass;	//Ʒ��
		int	m_nicon;	//ͼ��
		int	m_nposterid;	//����ID
		int	m_nhuiguangdangci;	//�Թ⵵��
	};
	// ����ʵ���ṹ
	struct gem_i : public item_i {
	};
#ifdef gem_instance 
	int ngem_iOffset[19] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,-1,-1	};
#endif //gem_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���� ID = 8
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum talisman_Fields {
		talisman_Prefix1 = 15,	//����ǰ׺1
		talisman_Prefix2 = 16,	//����ǰ׺2
		talisman_Postfix = 17,	//������׺
		talisman_Class = 18,	//��������
		talisman_Level = 19,	//��������
		talisman_class = 20,	//Ʒ��
		talisman_soulup = 21,	//������
		talisman_Max = 22
	};
	// ������ ����ǰ׺1 ö��ֵ����
	enum talisman_Prefix1_Enum {
		talisman_Prefix1_0,	//����
		talisman_Prefix1_1,	//����
		talisman_Prefix1_2,	//����
		talisman_Prefix1_3,	//����
		talisman_Prefix1_4,	//����
		talisman_Prefix1_5,	//����
		talisman_Prefix1_6,	//��ɽ
		talisman_Prefix1_7,	//����
		talisman_Prefix1_8,	//��Ԫ
		talisman_Prefix1Max = 9
	};
	// ������ ����ǰ׺2 ö��ֵ����
	enum talisman_Prefix2_Enum {
		talisman_Prefix2_0,	//����
		talisman_Prefix2_1,	//����
		talisman_Prefix2_2,	//����
		talisman_Prefix2_3,	//����
		talisman_Prefix2_4,	//����
		talisman_Prefix2_5,	//����
		talisman_Prefix2_6,	//��ɽ
		talisman_Prefix2_7,	//����
		talisman_Prefix2_8,	//��Ԫ
		talisman_Prefix2Max = 9
	};
	// ������ ������׺ ö��ֵ����
	enum talisman_Postfix_Enum {
		talisman_Postfix_0,	//����
		talisman_Postfix_1,	//��ħ
		talisman_Postfix_2,	//�ѵ�
		talisman_Postfix_3,	//ͨ��
		talisman_Postfix_4,	//�޳�
		talisman_Postfix_5,	//Ǭ��
		talisman_PostfixMax = 6
	};
	// ������ �������� ö��ֵ����
	enum talisman_Class_Enum {
		talisman_Class_0,	//��
		talisman_Class_1,	//��
		talisman_Class_2,	//��
		talisman_Class_3,	//ӡ
		talisman_Class_4,	//��
		talisman_Class_5,	//��
		talisman_Class_6,	//�
		talisman_ClassMax = 7
	};
	// ������ Ʒ�� ö��ֵ����
	enum talisman_class_Enum {
		talisman_class_0,	//��ͨ
		talisman_classMax = 1
	};
	// ����ģ��ṹ
	struct talisman_t : public item_t {
		int	m_nLevel;	//��������
		talisman_class_Enum	m_eclass;	//Ʒ��
		int	m_nsoulup;	//������
	};
	// ����ʵ���ṹ
	struct talisman_i : public item_i {
		talisman_Prefix1_Enum	m_ePrefix1;	//����ǰ׺1
		talisman_Prefix2_Enum	m_ePrefix2;	//����ǰ׺2
		talisman_Postfix_Enum	m_ePostfix;	//������׺
		talisman_Class_Enum	m_eClass;	//��������
		int	m_nLevel;	//��������
		talisman_class_Enum	m_eclass;	//Ʒ��
	};
#ifdef talisman_instance 
	int ntalisman_iOffset[22] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,-1	};
#endif //talisman_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���� ID = 17
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum jinglian_Fields {
		jinglian_butian = 15,	//��������
		jinglian_jinggang = 16,	//��ʯ����
		jinglian_jin = 17,	//������
		jinglian_yin = 18,	//������
		jinglian_tong = 19,	//ͭ����
		jinglian_tie = 20,	//������
		jinglian_buwei = 21,	//��λ
		jinglian_level = 22,	//�ȼ��ο�
		jinglian_huiguangyanse = 23,	//�Թ���ɫ
		jinglian_Max = 24
	};
	// ������ ��λ ö��ֵ����
	enum jinglian_buwei_Enum {
		jinglian_buwei_0,	//����
		jinglian_buwei_1,	//�ϰ���
		jinglian_buwei_2,	//�°���
		jinglian_buwei_3,	//����
		jinglian_buweiMax = 4
	};
	// ����ģ��ṹ
	struct jinglian_t : public item_t {
		int	m_nbutian;	//��������
		int	m_njinggang;	//��ʯ����
		int	m_njin;	//������
		int	m_nyin;	//������
		int	m_ntong;	//ͭ����
		int	m_ntie;	//������
		jinglian_buwei_Enum	m_ebuwei;	//��λ
		int	m_nlevel;	//�ȼ��ο�
		int	m_nhuiguangyanse;	//�Թ���ɫ
	};
	// ����ʵ���ṹ
	struct jinglian_i : public item_i {
	};
#ifdef jinglian_instance 
	int njinglian_iOffset[24] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //jinglian_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ����ǰ׺ ID = 19
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum firstnamefb_Fields {
		firstnamefb_chuxianjilv = 15,	//���ּ���
		firstnamefb_color = 16,	//��ɫ
		firstnamefb_Max = 17
	};
	// ����ģ��ṹ
	struct firstnamefb_t : public item_t {
		int	m_nchuxianjilv;	//���ּ���
		char	m_szcolor[9];	//��ɫ
	};
	// ����ʵ���ṹ
	struct firstnamefb_i : public item_i {
	};
#ifdef firstnamefb_instance 
	int nfirstnamefb_iOffset[17] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1	};
#endif //firstnamefb_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ������׺ ID = 20
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum lastnamefb_Fields {
		lastnamefb_Max = 15
	};
	// ����ģ��ṹ
	struct lastnamefb_t : public item_t {
	};
	// ����ʵ���ṹ
	struct lastnamefb_i : public item_i {
	};
#ifdef lastnamefb_instance 
	int nlastnamefb_iOffset[15] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16	};
#endif //lastnamefb_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ���� ID = 25
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum pet_Fields {
		pet_icon = 15,	//ͼ��
		pet_modnum = 16,	//ʹ��ģ��
		pet_class = 17,	//Ʒ��
		pet_level = 18,	//�ȼ�
		pet_exp = 19,	//����ֵ
		pet_active = 20,	//��Ծ��
		pet_activespd = 21,	//��Ծ�������ٶ�
		pet_str = 22,	//����
		pet_agi = 23,	//����
		pet_con = 24,	//����
		pet_int = 25,	//����
		pet_phy = 26,	//����
		pet_habit = 27,	//ϰ��
		pet_pointplus = 28,	//��������
		pet_fruit1 = 29,	//��ʵ1
		pet_fruit2 = 30,	//��ʵ2
		pet_fruit3 = 31,	//��ʵ3
		pet_fruit4 = 32,	//��ʵ4
		pet_skill = 33,	//�ؼ�
		pet_strpplus = 34,	//��������
		pet_agipplus = 35,	//���ݸ���
		pet_conpplus = 36,	//��������
		pet_intpplus = 37,	//��������
		pet_phypplus = 38,	//��������
		pet_f1pplus = 39,	//��ʵ1����
		pet_f2pplus = 40,	//��ʵ2����
		pet_f3pplus = 41,	//��ʵ3����
		pet_f4pplus = 42,	//��ʵ4����
		pet_pstatus = 43,	//�Ƿ��ս
		pet_pnickname = 44,	//�����ǳ�
		pet_autoictv = 45,	//�Զ�����
		pet_petexlvup = 46,	//���������ӳ�
		pet_pointchanging = 47,	//����ת����
		pet_activechanging = 48,	//��Ծ�Ȳ���Ч��
		pet_activepermin = 49,	//�����Ծ������
		pet_petexplus = 50,	//���ﾭ��ӳ�
		pet_Max = 51
	};
	// ������ Ʒ�� ö��ֵ����
	enum pet_class_Enum {
		pet_class_0,	//����
		pet_class_1,	//����
		pet_class_2,	//����
		pet_class_3,	//��
		pet_classMax = 4
	};
	// ����ģ��ṹ
	struct pet_t : public item_t {
		int	m_nicon;	//ͼ��
		int	m_nmodnum;	//ʹ��ģ��
	};
	// ����ʵ���ṹ
	struct pet_i : public item_i {
		pet_class_Enum	m_eclass;	//Ʒ��
		int	m_nlevel;	//�ȼ�
		int	m_nexp;	//����ֵ
		int	m_nactive;	//��Ծ��
		int	m_nactivespd;	//��Ծ�������ٶ�
		int	m_nstr;	//����
		int	m_nagi;	//����
		int	m_ncon;	//����
		int	m_nint;	//����
		int	m_nphy;	//����
		int	m_nhabit;	//ϰ��
		int	m_npointplus;	//��������
		int	m_nfruit1;	//��ʵ1
		int	m_nfruit2;	//��ʵ2
		int	m_nfruit3;	//��ʵ3
		int	m_nfruit4;	//��ʵ4
		int	m_nskill;	//�ؼ�
		int	m_nstrpplus;	//��������
		int	m_nagipplus;	//���ݸ���
		int	m_nconpplus;	//��������
		int	m_nintpplus;	//��������
		int	m_nphypplus;	//��������
		int	m_nf1pplus;	//��ʵ1����
		int	m_nf2pplus;	//��ʵ2����
		int	m_nf3pplus;	//��ʵ3����
		int	m_nf4pplus;	//��ʵ4����
		BOOL	m_bpstatus;	//�Ƿ��ս
		char	m_szpnickname[17];	//�����ǳ�
		BOOL	m_bautoictv;	//�Զ�����
		int	m_npetexlvup;	//���������ӳ�
		int	m_npointchanging;	//����ת����
		int	m_nactivechanging;	//��Ծ�Ȳ���Ч��
		int	m_nactivepermin;	//�����Ծ������
		int	m_npetexplus;	//���ﾭ��ӳ�
	};
#ifdef pet_instance 
	int npet_iOffset[51] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,145,149,153,157,161,165	};
#endif //pet_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ����ϰ�� ID = 27
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum pethabit_Fields {
		pethabit_OtherName = 15,	//����
		pethabit_Max = 16
	};
	// ����ģ��ṹ
	struct pethabit_t : public item_t {
		char	m_szOtherName[32];	//����
	};
	// ����ʵ���ṹ
	struct pethabit_i : public item_i {
	};
#ifdef pethabit_instance 
	int npethabit_iOffset[16] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1	};
#endif //pethabit_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ʱװ ID = 23
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum dress_Fields {
		dress_Max = 43
	};
	// ����ģ��ṹ
	struct dress_t : public equip_t {
	};
	// ����ʵ���ṹ
	struct dress_i : public equip_i {
	};
#ifdef dress_instance 
	int ndress_iOffset[43] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,44,48,52,-1,-1,-1,56,60,64,68,72,76,80,-1,-1,-1,-1,84,88,92,96,-1	};
#endif //dress_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ ��� ID = 3
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum player_Fields {
		player_dbid = 59,	//�ʺ�ID
		player_nickname = 60,	//�ǳ�
		player_exp = 61,	//����ֵ
		player_title = 62,	//�ƺ�
		player_job = 63,	//ְҵ
		player_money = 64,	//δʹ��
		player_gold = 65,	//Ԫ��
		player_ExpFactor = 66,	//���鱶��
		player_BagNum = 67,	//��������
		player_DepotPage = 68,	//�ֿ�����
		player_PK = 69,	//PKֵ
		player_mailbox = 70,	//����
		player_party = 71,	//���ڶ���ID
		player_guild = 72,	//���ڹ���ID
		player_marry = 73,	//����ID
		player_study = 74,	//ʦͽID
		player_brothor = 75,	//���ID
		player_PKmode = 76,	//PKģʽ
		player_soul = 77,	//��
		player_unusepoint = 78,	//δ�������
		player_sexy = 79,	//�Ա�
		player_uplevelexp = 80,	//�������辭��
		player_magicfind = 81,	//Ѱ����
		player_rwtimes = 82,	//�������
		player_wallowstate = 83,	//������״̬
		player_count_call1 = 84,	//�ƺż�����1
		player_count_call2 = 85,	//�ƺż�����2
		player_count_call3 = 86,	//�ƺż�����3
		player_count_call4 = 87,	//�ƺż�����4
		player_count_call5 = 88,	//�ƺż�����5
		player_count_call6 = 89,	//�ƺż�����6
		player_count_call7 = 90,	//�ƺż�����7
		player_count_call8 = 91,	//�ƺż�����8
		player_count_call9 = 92,	//�ƺż�����9
		player_count_call10 = 93,	//�ƺż�����10
		player_count_call11 = 94,	//�ƺż�����11
		player_slient = 95,	//����
		player_gminvisible = 96,	//GM����
		player_gmunbreakable = 97,	//GM�޵�
		player_fbcd = 98,	//������ȴʱ��
		player_ridelv = 99,	//��˼���
		player_ridetype = 100,	//�������
		player_standingroleintegral = 101,	//�ճ��������
		player_gongde = 102,	//����
		player_aixinzhi = 103,	//����ֵ
		player_Max = 104
	};
	// ������ ְҵ ö��ֵ����
	enum player_job_Enum {
		player_job_0,	//�佫
		player_job_1,	//�̿�
		player_job_2,	//��ʿ
		player_job_3,	//��ʦ
		player_jobMax = 4
	};
	// ������ PKģʽ ö��ֵ����
	enum player_PKmode_Enum {
		player_PKmode_0,	//��ͨ
		player_PKmode_1,	//PK
		player_PKmode_2,	//ǿɱ
		player_PKmodeMax = 3
	};
	// ������ ������״̬ ö��ֵ����
	enum player_wallowstate_Enum {
		player_wallowstate_0,	//������Ϸʱ��
		player_wallowstate_1,	//ƣ����Ϸʱ��
		player_wallowstate_2,	//�ǽ�����Ϸʱ��
		player_wallowstateMax = 3
	};
	// ����ģ��ṹ
	struct player_t : public charactor_t {
	};
	// ����ʵ���ṹ
	struct player_i : public charactor_i {
		int	m_ndbid;	//�ʺ�ID
		char	m_sznickname[17];	//�ǳ�
		int	m_nexp;	//����ֵ
		int	m_ntitle;	//�ƺ�
		player_job_Enum	m_ejob;	//ְҵ
		int	m_nmoney;	//δʹ��
		int	m_ngold;	//Ԫ��
		int	m_nExpFactor;	//���鱶��
		int	m_nBagNum;	//��������
		int	m_nDepotPage;	//�ֿ�����
		int	m_nPK;	//PKֵ
		int	m_nmailbox;	//����
		int	m_nparty;	//���ڶ���ID
		int	m_nguild;	//���ڹ���ID
		int	m_nmarry;	//����ID
		int	m_nstudy;	//ʦͽID
		int	m_nbrothor;	//���ID
		player_PKmode_Enum	m_ePKmode;	//PKģʽ
		int	m_nsoul;	//��
		int	m_nunusepoint;	//δ�������
		BOOL	m_bsexy;	//�Ա�
		int	m_nuplevelexp;	//�������辭��
		int	m_nmagicfind;	//Ѱ����
		int	m_nrwtimes;	//�������
		player_wallowstate_Enum	m_ewallowstate;	//������״̬
		int	m_ncount_call1;	//�ƺż�����1
		char	m_szcount_call2[16];	//�ƺż�����2
		int	m_ncount_call3;	//�ƺż�����3
		int	m_ncount_call4;	//�ƺż�����4
		int	m_ncount_call5;	//�ƺż�����5
		char	m_szcount_call6[16];	//�ƺż�����6
		int	m_ncount_call7;	//�ƺż�����7
		int	m_ncount_call8;	//�ƺż�����8
		int	m_ncount_call9;	//�ƺż�����9
		int	m_ncount_call10;	//�ƺż�����10
		int	m_ncount_call11;	//�ƺż�����11
		int	m_nslient;	//����
		BOOL	m_bgminvisible;	//GM����
		BOOL	m_bgmunbreakable;	//GM�޵�
		int	m_nfbcd;	//������ȴʱ��
		int	m_nridelv;	//��˼���
		int	m_nridetype;	//�������
		int	m_nstandingroleintegral;	//�ճ��������
		int	m_ngongde;	//����
		int	m_naixinzhi;	//����ֵ
	};
#ifdef player_instance 
	int nplayer_iOffset[104] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,257,261,265,269,273,277,281,285,289,293,297,301,305,309,313,317,321,325,329,333,337,341,345,349,353,369,373,377,381,397,401,405,409,413,417,421,425,429,433,437,441,445,449	};
#endif //player_instance 
	//////////////////////////////////////////////////////////////
	// ��Ԫ NPC ID = 4
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum NPC_Fields {
		NPC_id = 59,	//ID
		NPC_name = 60,	//����
		NPC_mobclass = 61,	//����Ʒ��
		NPC_offerexp = 62,	//�ṩ����ֵ
		NPC_isnpc = 63,	//NPC����
		NPC_AI = 64,	//AI���
		NPC_zoom = 65,	//��Ұ
		NPC_follow = 66,	//׷����Χ
		NPC_mod = 67,	//ģ��ʹ��
		NPC_nobeatfly = 68,	//��������
		NPC_moneydrop = 69,	//Ǯ����
		NPC_nomdrop = 70,	//��ͨ��Ʒ����
		NPC_spdrop = 71,	//������Ʒ����
		NPC_job = 72,	//ְҵ
		NPC_roundable = 73,	//�Ƿ���ת
		NPC_nobeatxuanyun = 74,	//ѣ������
		NPC_nobeatslow = 75,	//��������
		NPC_nobeatsleep = 76,	//��˯����
		NPC_nobeatchenmo = 77,	//��Ĭ����
		NPC_nobeatchongfeng = 78,	//�������
		NPC_tietuid = 79,	//��ͼID
		NPC_attdistance = 80,	//��ͨ������Χ
		NPC_attkind = 81,	//��������
		NPC_birthexcursion = 82,	//����ƫ��
		NPC_patrolarea = 83,	//Ѳ�߷�Χ
		NPC_bodybetime = 84,	//ʬ�����ʱ��
		NPC_jinengidone = 85,	//����ID1
		NPC_jinengidtwo = 86,	//����ID2
		NPC_diermingcheng = 87,	//�ڶ�����
		NPC_Max = 88
	};
	// ������ ����Ʒ�� ö��ֵ����
	enum NPC_mobclass_Enum {
		NPC_mobclass_0,	//��ͨ��-��׼
		NPC_mobclass_1,	//��ͨ��-ǿ׳
		NPC_mobclass_2,	//��ͨ��-ʩ����
		NPC_mobclass_3,	//��ͨ��-����
		NPC_mobclass_4,	//��ͨ��-����
		NPC_mobclass_5,	//��Ӣ��-��׼
		NPC_mobclass_6,	//��Ӣ��-ǿ׳
		NPC_mobclass_7,	//��Ӣ��-ʩ����
		NPC_mobclass_8,	//��Ӣ��-����
		NPC_mobclass_9,	//��Ӣ��-����
		NPC_mobclass_10,	//BOSS
		NPC_mobclass_11,	//BOSS-2
		NPC_mobclass_12,	//BOSS-3
		NPC_mobclass_13,	//BOSS-4
		NPC_mobclass_14,	//BOSS-5
		NPC_mobclass_15,	//BOSS-6
		NPC_mobclass_16,	//BOSS-7
		NPC_mobclass_17,	//BOSS-8
		NPC_mobclass_18,	//����
		NPC_mobclass_19,	//��ͨ��-����
		NPC_mobclassMax = 20
	};
	// ������ NPC���� ö��ֵ����
	enum NPC_isnpc_Enum {
		NPC_isnpc_0,	//����
		NPC_isnpc_1,	//NPC
		NPC_isnpc_2,	//����
		NPC_isnpc_3,	//����
		NPC_isnpcMax = 4
	};
	// ������ ְҵ ö��ֵ����
	enum NPC_job_Enum {
		NPC_job_0,	//�ֿ�
		NPC_job_1,	//����
		NPC_job_2,	//�����Ƕ
		NPC_job_3,	//��������
		NPC_job_4,	//���߳���
		NPC_job_5,	//����
		NPC_job_6,	//����
		NPC_job_7,	//��
		NPC_job_8,	//����
		NPC_job_9,	//����
		NPC_job_10,	//�ճ�����
		NPC_job_11,	//���γ���
		NPC_job_12,	//��������
		NPC_job_13,	//ҩƷ����
		NPC_job_14,	//װ��ǿ��
		NPC_job_15,	//��ְҵ
		NPC_job_16,	//����
		NPC_job_17,	//������������
		NPC_job_18,	//������߳���
		NPC_job_19,	//��ʯ��
		NPC_jobMax = 20
	};
	// ������ �ڶ����� ö��ֵ����
	enum NPC_diermingcheng_Enum {
		NPC_diermingcheng_0,	//<�ֿ�>
		NPC_diermingcheng_1,	//<����>
		NPC_diermingcheng_2,	//<�����Ƕ>
		NPC_diermingcheng_3,	//<��������>
		NPC_diermingcheng_4,	//<���߳���>
		NPC_diermingcheng_5,	//<����>
		NPC_diermingcheng_6,	//<����>
		NPC_diermingcheng_7,	//<��>
		NPC_diermingcheng_8,	//<����>
		NPC_diermingcheng_9,	//<����>
		NPC_diermingcheng_10,	//<�ճ�����>
		NPC_diermingcheng_11,	//<���γ���>
		NPC_diermingcheng_12,	//<��������>
		NPC_diermingcheng_13,	//<ҩƷ����>
		NPC_diermingcheng_14,	//<����/����>
		NPC_diermingcheng_15,	//��
		NPC_diermingcheng_16,	//<������������>
		NPC_diermingcheng_17,	//<������߳���>
		NPC_diermingcheng_18,	//<��ʯ��>
		NPC_diermingcheng_19,	//<ϡ��BOSS>
		NPC_diermingcheng_20,	//<����BOSS>
		NPC_diermingcheng_21,	//<����ͷĿ>
		NPC_diermingcheng_22,	//<������>
		NPC_diermingcheng_23,	//<����>
		NPC_diermingcheng_24,	//<װ����>
		NPC_diermingcheng_25,	//<�߼�������������>
		NPC_diermingcheng_26,	//<�߼�������߳���>
		NPC_diermingcheng_27,	//<�߼���������>
		NPC_diermingcheng_28,	//<�߼����߳���>
		NPC_diermingchengMax = 29
	};
	// ����ģ��ṹ
	struct NPC_t : public charactor_t {
		int	m_nid;	//ID
		char	m_szname[32];	//����
		NPC_mobclass_Enum	m_emobclass;	//����Ʒ��
		NPC_isnpc_Enum	m_eisnpc;	//NPC����
		int	m_nAI;	//AI���
		int	m_nmod;	//ģ��ʹ��
		BOOL	m_bnobeatfly;	//��������
		BOOL	m_bmoneydrop;	//Ǯ����
		BOOL	m_bnomdrop;	//��ͨ��Ʒ����
		BOOL	m_bspdrop;	//������Ʒ����
		NPC_job_Enum	m_ejob;	//ְҵ
		BOOL	m_broundable;	//�Ƿ���ת
		BOOL	m_bnobeatxuanyun;	//ѣ������
		BOOL	m_bnobeatslow;	//��������
		BOOL	m_bnobeatsleep;	//��˯����
		BOOL	m_bnobeatchenmo;	//��Ĭ����
		BOOL	m_bnobeatchongfeng;	//�������
		char	m_sztietuid[16];	//��ͼID
		int	m_nattdistance;	//��ͨ������Χ
		int	m_nattkind;	//��������
		char	m_szbirthexcursion[4];	//����ƫ��
		char	m_szpatrolarea[4];	//Ѳ�߷�Χ
		char	m_szbodybetime[4];	//ʬ�����ʱ��
		int	m_njinengidone;	//����ID1
		int	m_njinengidtwo;	//����ID2
		NPC_diermingcheng_Enum	m_ediermingcheng;	//�ڶ�����
	};
	// ����ʵ���ṹ
	struct NPC_i : public charactor_i {
		char	m_szname[32];	//����
		int	m_nofferexp;	//�ṩ����ֵ
		int	m_nzoom;	//��Ұ
		int	m_nfollow;	//׷����Χ
		char	m_szbirthexcursion[4];	//����ƫ��
		char	m_szpatrolarea[4];	//Ѳ�߷�Χ
		char	m_szbodybetime[4];	//ʬ�����ʱ��
		NPC_diermingcheng_Enum	m_ediermingcheng;	//�ڶ�����
	};
#ifdef NPC_instance 
	int nNPC_iOffset[88] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,-1,236,-1,268,-1,-1,272,276,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,280,284,288,-1,-1,292	};
#endif //NPC_instance 
	// ��ϵIDö��
	enum Contact_IDs {
		Contact_EffectOpScript = 1,	//Ӱ�����
		Contact_EffectValueScript = 2,	//Ӱ��ֵ����ű�
		Contact_SkillType = 3,	//��������
		Contact_actionCondition = 4,	//��������
		Contact_mobtype = 5,	//����ģ��
		Contact_gonghuiskillstudy = 7,	//���Ἴ��ѧϰ
		Contact_lastnamefbbaoliu = 8,	//������׺��������
		Contact_typefb = 9,	//��������
		Contact_moblevelroom = 10,	//����ȼ�����
		Contact_drop = 11,	//��Ʒ���伯��
		Contact_makelist = 12,	//��Ʒ���쵥
		Contact_drpplist = 13,	//�����������
		Contact_npcgn = 14,	//NPC��Ӧ����
		Contact_selllist = 15,	//�����б�
		Contact_spyx = 16,	//����Ӱ��
		Contact_gnnpc = 17,	//����NPC��Ӧ
		Contact_spskill = 18,	//���⼼�ܲ���
		Contact_forginghort = 19,	//����װ��������Ӧ
		Contact_StudySkillDepend = 20,	//����ѧϰ����
		Contact_mapoutport = 21,	//��ͼ���ӵ�
		Contact_shuxingjiazhi = 22,	//���Լ�ֵ
		Contact_jinglianduiying = 23,	//������Ӧ
		Contact_xiaoxiilebiao = 24,	//��Ϣ�б�
		Contact_title = 25,	//�ƺ����
		Contact_ditufuhuodian = 26,	//��ͼ�����
		Contact_kuangchangshouwei = 27,	//������
		Contact_gonghui = 28,	//����
		Contact_charmodetype = 29,	//���ģ��ģ��
		Contact_KCshouweiduiying = 30,	//�󳡾����Ӧ
		Contact_NPCskillscontact = 31,	//NPC������ϵ
		Contact_NPCAI = 32,	//����AI
		Contact_jingliandrop = 33,	//��������
		Contact_gemdrop = 34,	//��ʯ����
		Contact_itemdrop = 35,	//���ߵ���
		Contact_smithingdrop = 36,	//�������
		Contact_maptransmissionlist = 37,	//�����б�
		Contact_transmissionprice = 38,	//���ͼ۸��
		Contact_fbmake = 39,	//��ǰ׺��������
		Contact_doublefbmake = 40,	//˫ǰ׺��������
		Contact_LurkShape = 41,	//����ģ��
		Contact_skillspecialpercent = 42,	//��������ٷֱ�
		Contact_mapway = 43,	//��ͼѰ·
		Contact_jinglianlianxibiao = 44,	//������λ��ϵ��
		Contact_equipmake = 45,	//װ������
		Contact_buweixishu = 47,	//��λϵ����
		Contact_sjsxxishubiao = 48,	//������Զ��ձ�
		Contact_gdsxduizhaobiao = 49,	//�̶����Զ��ձ�
		Contact_canpreviewlist = 50,	//��Ԥ���б�
		Contact_bagcontactlist = 51,	//�����ϵ��
		Contact_xysjsxlist = 52,	//ϡ��������Զ��ձ�
		Contact_smeltstone = 53,	//����
		Contact_stonelvuplist = 54,	//�����������ձ�
		Contact_dtncslist = 55,	//��ͼ�ڴ����б�
		Contact_resourcelist = 56,	//����ֲ���
		Contact_richangrenwulvarea = 57,	//�ճ�����ȼ�����
		Contact_sjsxdjssxysb = 58,	//������Ե���ɫ����ӳ���
		Contact_csnpclb = 59,	//����NPC�б�
		Contact_roletypelist = 60,	//�������ͱ�
		Contact_rolelvareaanddepotid = 61,	//����ȼ�����Ͷ�Ӧ��
		Contact_depotidandroleidlist = 62,	//��������Ӧ��
		Contact_replacerolelist = 63,	//�ɽ������б�
		Contact_integralaward = 64,	//���ֽ�����
		Contact_shitujianglilist = 65,	//ʦͽ������
		Contact_jiebaiqinmidu = 66,	//������ܶ�
		Contact_huiguangdangci = 67,	//�Թ⵵��
		Contact_huiguangyanse = 68,	//�Թ���ɫ
		Contact_xinshouzhuantiao = 69,	//���ֵ�ͼת��
		Contact_fubenboss = 70,	//����BOSS���
		Contact_npczidongxunlufenlei = 71,	//NPC�Զ�Ѱ·����
		Contact_baoshihecheng = 72,	//��ʯ�ϳ�
		Contact_petclass = 73,	//����Ʒ��
		Contact_petfruit = 74,	//ϰ�Թ�ʵ
		Contact_petdrop = 75,	//�������
		Contact_petfruitbagcontactlist = 76,	//�����ʵ�����ϵ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ Ӱ����� ID = 1
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum EffectOpScript_Fields {
		EffectOpScript_ID = 0,	//ID
		EffectOpScript_Name = 1,	//����
		EffectOpScript_Alias = 2,	//Ӣ�ı���
		EffectOpScript_Max = 3
	};
	// ��ϵ���ݽṹ
	struct EffectOpScript_t {
		int	m_nID;	//ID
		char	m_szName[16];	//����
		char	m_szAlias[16];	//Ӣ�ı���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ Ӱ��ֵ����ű� ID = 2
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum EffectValueScript_Fields {
		EffectValueScript_id = 0,	//ID
		EffectValueScript_name = 1,	//����
		EffectValueScript_Alias = 2,	//����
		EffectValueScript_param1 = 3,	//����1
		EffectValueScript_param2 = 4,	//����2
		EffectValueScript_param3 = 5,	//����3
		EffectValueScript_param4 = 6,	//����4
		EffectValueScript_Max = 7
	};
	// ������ (null) ö��ֵ����
	enum EffectValueScript_param1_Enum {
		EffectValueScript_param1_0,	//
		EffectValueScript_param1_1,	//
		EffectValueScript_param1_2,	//
		EffectValueScript_param1Max = 3
	};
	// ������ (null) ö��ֵ����
	enum EffectValueScript_param2_Enum {
		EffectValueScript_param2_0,	//
		EffectValueScript_param2_1,	//
		EffectValueScript_param2_2,	//
		EffectValueScript_param2Max = 3
	};
	// ������ (null) ö��ֵ����
	enum EffectValueScript_param3_Enum {
		EffectValueScript_param3_0,	//
		EffectValueScript_param3_1,	//
		EffectValueScript_param3_2,	//
		EffectValueScript_param3Max = 3
	};
	// ������ (null) ö��ֵ����
	enum EffectValueScript_param4_Enum {
		EffectValueScript_param4_0,	//
		EffectValueScript_param4_1,	//
		EffectValueScript_param4_2,	//
		EffectValueScript_param4Max = 3
	};
	// ��ϵ���ݽṹ
	struct EffectValueScript_t {
		int	m_nid;	//ID
		char	m_szname[32];	//����
		char	m_szAlias[16];	//����
		EffectValueScript_param1_Enum	m_eparam1;	//����1
		EffectValueScript_param2_Enum	m_eparam2;	//����2
		EffectValueScript_param3_Enum	m_eparam3;	//����3
		EffectValueScript_param4_Enum	m_eparam4;	//����4
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �������� ID = 3
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum SkillType_Fields {
		SkillType_ID = 0,	//id
		SkillType_Name = 1,	//����
		SkillType_Alias = 2,	//Ӣ�ı���
		SkillType_Max = 3
	};
	// ��ϵ���ݽṹ
	struct SkillType_t {
		int	m_nID;	//id
		char	m_szName[32];	//����
		char	m_szAlias[16];	//Ӣ�ı���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �������� ID = 4
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum actionCondition_Fields {
		actionCondition_Max = 0
	};
	// ��ϵ���ݽṹ
	struct actionCondition_t {
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ģ�� ID = 5
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum mobtype_Fields {
		mobtype_level = 0,	//�ȼ�
		mobtype_mobclass = 1,	//��������
		mobtype_leastdc = 2,	//��С��������
		mobtype_maxdc = 3,	//�����������
		mobtype_ac = 4,	//���������
		mobtype_mac = 5,	//����������
		mobtype_crit = 6,	//�����ػ���
		mobtype_skillcrit = 7,	//�����ػ���
		mobtype_hitchance = 8,	//����
		mobtype_hedge = 9,	//����
		mobtype_hpreg = 10,	//�����ָ��ٶ�
		mobtype_mpreg = 11,	//�����ָ��ٶ�
		mobtype_attspeed = 12,	//�����ٶ�
		mobtype_maxhp = 13,	//�������ֵ
		mobtype_maxmp = 14,	//�����ֵ
		mobtype_offerexp = 15,	//�ṩ����ֵ
		mobtype_zoom = 16,	//��Ұ
		mobtype_follow = 17,	//׷����Χ
		mobtype_magicatt = 18,	//����������
		mobtype_Max = 19
	};
	// ������ (null) ö��ֵ����
	enum mobtype_mobclass_Enum {
		mobtype_mobclass_0,	//
		mobtype_mobclass_1,	//
		mobtype_mobclass_2,	//
		mobtype_mobclass_3,	//
		mobtype_mobclass_4,	//
		mobtype_mobclass_5,	//
		mobtype_mobclass_6,	//
		mobtype_mobclass_7,	//
		mobtype_mobclass_8,	//
		mobtype_mobclass_9,	//
		mobtype_mobclass_10,	//
		mobtype_mobclass_11,	//
		mobtype_mobclass_12,	//
		mobtype_mobclass_13,	//
		mobtype_mobclass_14,	//
		mobtype_mobclass_15,	//
		mobtype_mobclass_16,	//
		mobtype_mobclass_17,	//
		mobtype_mobclass_18,	//
		mobtype_mobclass_19,	//
		mobtype_mobclassMax = 20
	};
	// ��ϵ���ݽṹ
	struct mobtype_t {
		int	m_nlevel;	//�ȼ�
		mobtype_mobclass_Enum	m_emobclass;	//��������
		int	m_nleastdc;	//��С��������
		int	m_nmaxdc;	//�����������
		int	m_nac;	//���������
		int	m_nmac;	//����������
		int	m_ncrit;	//�����ػ���
		int	m_nskillcrit;	//�����ػ���
		int	m_nhitchance;	//����
		int	m_nhedge;	//����
		int	m_nhpreg;	//�����ָ��ٶ�
		int	m_nmpreg;	//�����ָ��ٶ�
		float	m_fattspeed;	//�����ٶ�
		int	m_nmaxhp;	//�������ֵ
		int	m_nmaxmp;	//�����ֵ
		int	m_nofferexp;	//�ṩ����ֵ
		int	m_nzoom;	//��Ұ
		int	m_nfollow;	//׷����Χ
		int	m_nmagicatt;	//����������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���Ἴ��ѧϰ ID = 7
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gonghuiskillstudy_Fields {
		gonghuiskillstudy_gonghuiskilskillname = 0,	//��������
		gonghuiskillstudy_skillid = 1,	//����ID
		gonghuiskillstudy_gonghuiLV = 2,	//����ȼ�
		gonghuiskillstudy_money = 3,	//�з���Ǯ
		gonghuiskillstudy_Max = 4
	};
	// ��ϵ���ݽṹ
	struct gonghuiskillstudy_t {
		char	m_szgonghuiskilskillname[16];	//��������
		int	m_nskillid;	//����ID
		int	m_ngonghuiLV;	//����ȼ�
		int	m_nmoney;	//�з���Ǯ
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������׺�������� ID = 8
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum lastnamefbbaoliu_Fields {
		lastnamefbbaoliu_Max = 0
	};
	// ��ϵ���ݽṹ
	struct lastnamefbbaoliu_t {
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �������� ID = 9
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum typefb_Fields {
		typefb_typename = 0,	//������
		typefb_skillas = 1,	//��Ӧ����ID
		typefb_firenum = 2,	//��������
		typefb_Max = 3
	};
	// ��ϵ���ݽṹ
	struct typefb_t {
		char	m_sztypename[16];	//������
		int	m_nskillas;	//��Ӧ����ID
		int	m_nfirenum;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ȼ����� ID = 10
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum moblevelroom_Fields {
		moblevelroom_ID = 0,	//ID
		moblevelroom_levelmin = 1,	//��Ӧ�ȼ�����
		moblevelroom_levelmax = 2,	//��Ӧ�ȼ�����
		moblevelroom_Max = 3
	};
	// ��ϵ���ݽṹ
	struct moblevelroom_t {
		int	m_nID;	//ID
		int	m_nlevelmin;	//��Ӧ�ȼ�����
		int	m_nlevelmax;	//��Ӧ�ȼ�����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��Ʒ���伯�� ID = 11
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum drop_Fields {
		drop_room = 0,	//��������
		drop_equipnum = 1,	//װ�����
		drop_dorpclass = 2,	//�������
		drop_dropprobability = 3,	//�������
		drop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum drop_dorpclass_Enum {
		drop_dorpclass_0,	//
		drop_dorpclass_1,	//
		drop_dorpclass_2,	//
		drop_dorpclassMax = 3
	};
	// ��ϵ���ݽṹ
	struct drop_t {
		int	m_nroom;	//��������
		int	m_nequipnum;	//װ�����
		drop_dorpclass_Enum	m_edorpclass;	//�������
		int	m_ndropprobability;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��Ʒ���쵥 ID = 12
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum makelist_Fields {
		makelist_ID = 0,	//���
		makelist_paper = 1,	//����ͼֽ
		makelist_eqid = 2,	//װ�����
		makelist_class1 = 3,	//����1����
		makelist_id1 = 4,	//����1���
		makelist_num1 = 5,	//����1����
		makelist_class2 = 6,	//����2����
		makelist_id2 = 7,	//����2���
		makelist_num2 = 8,	//����2����
		makelist_class3 = 9,	//����3����
		makelist_id3 = 10,	//����3���
		makelist_num3 = 11,	//����3����
		makelist_class4 = 12,	//����4����
		makelist_id4 = 13,	//����4���
		makelist_num4 = 14,	//����4����
		makelist_class5 = 15,	//����5����
		makelist_id5 = 16,	//����5���
		makelist_num5 = 17,	//����5����
		makelist_class6 = 18,	//����6����
		makelist_id6 = 19,	//����6���
		makelist_num6 = 20,	//����6����
		makelist_papertype = 21,	//ͼֽ����
		makelist_hortpro = 22,	//��������
		makelist_Max = 23
	};
	// ������ (null) ö��ֵ����
	enum makelist_class1_Enum {
		makelist_class1_0,	//
		makelist_class1_1,	//
		makelist_class1_2,	//
		makelist_class1_3,	//
		makelist_class1Max = 4
	};
	// ������ (null) ö��ֵ����
	enum makelist_class2_Enum {
		makelist_class2_0,	//
		makelist_class2_1,	//
		makelist_class2_2,	//
		makelist_class2_3,	//
		makelist_class2Max = 4
	};
	// ������ (null) ö��ֵ����
	enum makelist_class3_Enum {
		makelist_class3_0,	//
		makelist_class3_1,	//
		makelist_class3_2,	//
		makelist_class3_3,	//
		makelist_class3Max = 4
	};
	// ������ (null) ö��ֵ����
	enum makelist_class4_Enum {
		makelist_class4_0,	//
		makelist_class4_1,	//
		makelist_class4_2,	//
		makelist_class4_3,	//
		makelist_class4Max = 4
	};
	// ������ (null) ö��ֵ����
	enum makelist_class5_Enum {
		makelist_class5_0,	//
		makelist_class5_1,	//
		makelist_class5_2,	//
		makelist_class5_3,	//
		makelist_class5Max = 4
	};
	// ������ (null) ö��ֵ����
	enum makelist_class6_Enum {
		makelist_class6_0,	//
		makelist_class6_1,	//
		makelist_class6_2,	//
		makelist_class6_3,	//
		makelist_class6Max = 4
	};
	// ��ϵ���ݽṹ
	struct makelist_t {
		int	m_nID;	//���
		int	m_npaper;	//����ͼֽ
		int	m_neqid;	//װ�����
		makelist_class1_Enum	m_eclass1;	//����1����
		int	m_nid1;	//����1���
		int	m_nnum1;	//����1����
		makelist_class2_Enum	m_eclass2;	//����2����
		int	m_nid2;	//����2���
		int	m_nnum2;	//����2����
		makelist_class3_Enum	m_eclass3;	//����3����
		int	m_nid3;	//����3���
		int	m_nnum3;	//����3����
		makelist_class4_Enum	m_eclass4;	//����4����
		int	m_nid4;	//����4���
		int	m_nnum4;	//����4����
		makelist_class5_Enum	m_eclass5;	//����5����
		int	m_nid5;	//����5���
		int	m_nnum5;	//����5����
		makelist_class6_Enum	m_eclass6;	//����6����
		int	m_nid6;	//����6���
		int	m_nnum6;	//����6����
		int	m_npapertype;	//ͼֽ����
		int	m_nhortpro;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����������� ID = 13
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum drpplist_Fields {
		drpplist_ID = 0,	//ID
		drpplist_mobid = 1,	//������
		drpplist_droptype = 2,	//��������
		drpplist_equid = 3,	//������
		drpplist_dropmey = 4,	//���伸��
		drpplist_droptimes = 5,	//�������
		drpplist_Max = 6
	};
	// ������ (null) ö��ֵ����
	enum drpplist_droptype_Enum {
		drpplist_droptype_0,	//
		drpplist_droptype_1,	//
		drpplist_droptype_2,	//
		drpplist_droptypeMax = 3
	};
	// ��ϵ���ݽṹ
	struct drpplist_t {
		int	m_nID;	//ID
		int	m_nmobid;	//������
		drpplist_droptype_Enum	m_edroptype;	//��������
		int	m_nequid;	//������
		float	m_fdropmey;	//���伸��
		int	m_ndroptimes;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ NPC��Ӧ���� ID = 14
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum npcgn_Fields {
		npcgn_ID = 0,	//ID
		npcgn_dygn = 1,	//��Ӧ����
		npcgn_sign = 2,	//���ܱ�ǩ
		npcgn_gnlist = 3,	//�����б�
		npcgn_sfsm = 4,	//�Ƿ�����
		npcgn_Max = 5
	};
	// ������ (null) ö��ֵ����
	enum npcgn_dygn_Enum {
		npcgn_dygn_0,	//
		npcgn_dygn_1,	//
		npcgn_dygn_2,	//
		npcgn_dygn_3,	//
		npcgn_dygn_4,	//
		npcgn_dygn_5,	//
		npcgn_dygn_6,	//
		npcgn_dygn_7,	//
		npcgn_dygn_8,	//
		npcgn_dygn_9,	//
		npcgn_dygn_10,	//
		npcgn_dygn_11,	//
		npcgn_dygn_12,	//
		npcgn_dygn_13,	//
		npcgn_dygn_14,	//
		npcgn_dygn_15,	//
		npcgn_dygn_16,	//
		npcgn_dygn_17,	//
		npcgn_dygn_18,	//
		npcgn_dygn_19,	//
		npcgn_dygn_20,	//
		npcgn_dygn_21,	//
		npcgn_dygn_22,	//
		npcgn_dygn_23,	//
		npcgn_dygn_24,	//
		npcgn_dygn_25,	//
		npcgn_dygn_26,	//
		npcgn_dygn_27,	//
		npcgn_dygn_28,	//
		npcgn_dygn_29,	//
		npcgn_dygn_30,	//
		npcgn_dygn_31,	//
		npcgn_dygn_32,	//
		npcgn_dygn_33,	//
		npcgn_dygn_34,	//
		npcgn_dygn_35,	//
		npcgn_dygn_36,	//
		npcgn_dygn_37,	//
		npcgn_dygn_38,	//
		npcgn_dygn_39,	//
		npcgn_dygn_40,	//
		npcgn_dygn_41,	//
		npcgn_dygn_42,	//
		npcgn_dygn_43,	//
		npcgn_dygn_44,	//
		npcgn_dygn_45,	//
		npcgn_dygn_46,	//
		npcgn_dygn_47,	//
		npcgn_dygn_48,	//
		npcgn_dygn_49,	//
		npcgn_dygn_50,	//
		npcgn_dygn_51,	//
		npcgn_dygn_52,	//
		npcgn_dygn_53,	//
		npcgn_dygn_54,	//
		npcgn_dygn_55,	//
		npcgn_dygn_56,	//
		npcgn_dygn_57,	//
		npcgn_dygn_58,	//
		npcgn_dygn_59,	//
		npcgn_dygn_60,	//
		npcgn_dygn_61,	//
		npcgn_dygn_62,	//
		npcgn_dygn_63,	//
		npcgn_dygn_64,	//
		npcgn_dygn_65,	//
		npcgn_dygn_66,	//
		npcgn_dygn_67,	//
		npcgn_dygn_68,	//
		npcgn_dygn_69,	//
		npcgn_dygn_70,	//
		npcgn_dygn_71,	//
		npcgn_dygn_72,	//
		npcgn_dygn_73,	//
		npcgn_dygnMax = 74
	};
	// ��ϵ���ݽṹ
	struct npcgn_t {
		int	m_nID;	//ID
		npcgn_dygn_Enum	m_edygn;	//��Ӧ����
		char	m_szsign[32];	//���ܱ�ǩ
		int	m_ngnlist;	//�����б�
		BOOL	m_bsfsm;	//�Ƿ�����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �����б� ID = 15
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum selllist_Fields {
		selllist_Listid = 0,	//���������
		selllist_wpclass = 1,	//������Ʒ����
		selllist_wpid = 2,	//������Ʒ���
		selllist_class = 3,	//��ƷƷ��
		selllist_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum selllist_wpclass_Enum {
		selllist_wpclass_0,	//
		selllist_wpclass_1,	//
		selllist_wpclass_2,	//
		selllist_wpclass_3,	//
		selllist_wpclassMax = 4
	};
	// ������ (null) ö��ֵ����
	enum selllist_class_Enum {
		selllist_class_0,	//
		selllist_class_1,	//
		selllist_class_2,	//
		selllist_class_3,	//
		selllist_class_4,	//
		selllist_class_5,	//
		selllist_class_6,	//
		selllist_class_7,	//
		selllist_class_8,	//
		selllist_classMax = 9
	};
	// ��ϵ���ݽṹ
	struct selllist_t {
		int	m_nListid;	//���������
		selllist_wpclass_Enum	m_ewpclass;	//������Ʒ����
		int	m_nwpid;	//������Ʒ���
		selllist_class_Enum	m_eclass;	//��ƷƷ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����Ӱ�� ID = 16
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum spyx_Fields {
		spyx_Max = 0
	};
	// ��ϵ���ݽṹ
	struct spyx_t {
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����NPC��Ӧ ID = 17
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gnnpc_Fields {
		gnnpc_NPCID = 0,	//NPC���
		gnnpc_GNID = 1,	//��Ӧ���ܱ��
		gnnpc_Max = 2
	};
	// ��ϵ���ݽṹ
	struct gnnpc_t {
		int	m_nNPCID;	//NPC���
		int	m_nGNID;	//��Ӧ���ܱ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���⼼�ܲ��� ID = 18
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum spskill_Fields {
		spskill_skillid = 0,	//���ܱ��
		spskill_jgtype = 1,	//�������
		spskill_jg1 = 2,	//�������a
		spskill_jg2 = 3,	//�������b
		spskill_jg3 = 4,	//�������c
		spskill_jntxt = 5,	//��������
		spskill_sfjssx = 6,	//�Ƿ�ʱ��Ч
		spskill_Max = 7
	};
	// ������ (null) ö��ֵ����
	enum spskill_jgtype_Enum {
		spskill_jgtype_0,	//
		spskill_jgtype_1,	//
		spskill_jgtype_2,	//
		spskill_jgtype_3,	//
		spskill_jgtype_4,	//
		spskill_jgtype_5,	//
		spskill_jgtype_6,	//
		spskill_jgtype_7,	//
		spskill_jgtype_8,	//
		spskill_jgtypeMax = 9
	};
	// ��ϵ���ݽṹ
	struct spskill_t {
		int	m_nskillid;	//���ܱ��
		spskill_jgtype_Enum	m_ejgtype;	//�������
		int	m_njg1;	//�������a
		int	m_njg2;	//�������b
		int	m_njg3;	//�������c
		char	m_szjntxt[128];	//��������
		BOOL	m_bsfjssx;	//�Ƿ�ʱ��Ч
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����װ��������Ӧ ID = 19
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum forginghort_Fields {
		forginghort_horttype = 0,	//��������
		forginghort_gainpro = 1,	//��ü���
		forginghort_hortpro1 = 2,	//��������1
		forginghort_hortcost1 = 3,	//����ֵ1
		forginghort_hortpro2 = 4,	//��������2
		forginghort_hortcost2 = 5,	//����ֵ2
		forginghort_hortpro3 = 6,	//��������3
		forginghort_hortcost3 = 7,	//����ֵ3
		forginghort_Max = 8
	};
	// ��ϵ���ݽṹ
	struct forginghort_t {
		int	m_nhorttype;	//��������
		int	m_ngainpro;	//��ü���
		int	m_nhortpro1;	//��������1
		int	m_nhortcost1;	//����ֵ1
		int	m_nhortpro2;	//��������2
		int	m_nhortcost2;	//����ֵ2
		int	m_nhortpro3;	//��������3
		int	m_nhortcost3;	//����ֵ3
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ѧϰ���� ID = 20
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum StudySkillDepend_Fields {
		StudySkillDepend_SkillId = 0,	//����
		StudySkillDepend_DependSkillId = 1,	//��������
		StudySkillDepend_Max = 2
	};
	// ��ϵ���ݽṹ
	struct StudySkillDepend_t {
		int	m_nSkillId;	//����
		int	m_nDependSkillId;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ͼ���ӵ� ID = 21
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum mapoutport_Fields {
		mapoutport_mapid = 0,	//������ͼid
		mapoutport_x = 1,	//x
		mapoutport_y = 2,	//y
		mapoutport_w = 3,	//w
		mapoutport_h = 4,	//h
		mapoutport_tomap = 5,	//tomap
		mapoutport_tox = 6,	//tox
		mapoutport_toy = 7,	//toy
		mapoutport_tow = 8,	//tow
		mapoutport_toh = 9,	//toh
		mapoutport_degree = 10,	//�Ƕ�
		mapoutport_Max = 11
	};
	// ��ϵ���ݽṹ
	struct mapoutport_t {
		int	m_nmapid;	//������ͼid
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
		int	m_ntomap;	//tomap
		int	m_ntox;	//tox
		int	m_ntoy;	//toy
		int	m_ntow;	//tow
		int	m_ntoh;	//toh
		int	m_ndegree;	//�Ƕ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���Լ�ֵ ID = 22
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum shuxingjiazhi_Fields {
		shuxingjiazhi_ID = 0,	//ID
		shuxingjiazhi_levelmin = 1,	//�ȼ���������
		shuxingjiazhi_levelmax = 2,	//�ȼ���������
		shuxingjiazhi_strmin = 3,	//��������
		shuxingjiazhi_strmax = 4,	//��������
		shuxingjiazhi_dexmin = 5,	//��������
		shuxingjiazhi_dexmax = 6,	//��������
		shuxingjiazhi_intmin = 7,	//�ǻ�����
		shuxingjiazhi_intmax = 8,	//�ǻ�����
		shuxingjiazhi_conmin = 9,	//��������
		shuxingjiazhi_conmax = 10,	//��������
		shuxingjiazhi_phymin = 11,	//��������
		shuxingjiazhi_phymax = 12,	//��������
		shuxingjiazhi_attmin = 13,	//������������
		shuxingjiazhi_attmax = 14,	//������������
		shuxingjiazhi_defmin = 15,	//�������������
		shuxingjiazhi_defmax = 16,	//�������������
		shuxingjiazhi_mattmin = 17,	//����Ч������
		shuxingjiazhi_mattmax = 18,	//����Ч������
		shuxingjiazhi_mdefmin = 19,	//��������������
		shuxingjiazhi_mdefmax = 20,	//��������������
		shuxingjiazhi_lifemin = 21,	//�������ֵ����
		shuxingjiazhi_lifemax = 22,	//�������ֵ����
		shuxingjiazhi_manamin = 23,	//�������ֵ����
		shuxingjiazhi_manamax = 24,	//�������ֵ����
		shuxingjiazhi_hitmin = 25,	//��������
		shuxingjiazhi_hitmax = 26,	//��������
		shuxingjiazhi_dodgemin = 27,	//��������
		shuxingjiazhi_dodgemax = 28,	//��������
		shuxingjiazhi_phycrmin = 29,	//�����ػ�������
		shuxingjiazhi_phycrmax = 30,	//�����ػ�������
		shuxingjiazhi_skicrmin = 31,	//�����ػ�������
		shuxingjiazhi_skicrmax = 32,	//�����ػ�������
		shuxingjiazhi_liferemin = 33,	//�����ָ��ٶ�����
		shuxingjiazhi_liferemax = 34,	//�����ָ��ٶ�����
		shuxingjiazhi_manaremin = 35,	//�����ָ��ٶ�����
		shuxingjiazhi_manaremax = 36,	//�����ָ��ٶ�����
		shuxingjiazhi_rpmin = 37,	//����������
		shuxingjiazhi_rpmax = 38,	//����������
		shuxingjiazhi_Max = 39
	};
	// ��ϵ���ݽṹ
	struct shuxingjiazhi_t {
		int	m_nID;	//ID
		int	m_nlevelmin;	//�ȼ���������
		int	m_nlevelmax;	//�ȼ���������
		int	m_nstrmin;	//��������
		int	m_nstrmax;	//��������
		int	m_ndexmin;	//��������
		int	m_ndexmax;	//��������
		int	m_nintmin;	//�ǻ�����
		int	m_nintmax;	//�ǻ�����
		int	m_nconmin;	//��������
		int	m_nconmax;	//��������
		int	m_nphymin;	//��������
		int	m_nphymax;	//��������
		int	m_nattmin;	//������������
		int	m_nattmax;	//������������
		int	m_ndefmin;	//�������������
		int	m_ndefmax;	//�������������
		int	m_nmattmin;	//����Ч������
		int	m_nmattmax;	//����Ч������
		int	m_nmdefmin;	//��������������
		int	m_nmdefmax;	//��������������
		int	m_nlifemin;	//�������ֵ����
		int	m_nlifemax;	//�������ֵ����
		int	m_nmanamin;	//�������ֵ����
		int	m_nmanamax;	//�������ֵ����
		int	m_nhitmin;	//��������
		int	m_nhitmax;	//��������
		int	m_ndodgemin;	//��������
		int	m_ndodgemax;	//��������
		int	m_nphycrmin;	//�����ػ�������
		int	m_nphycrmax;	//�����ػ�������
		int	m_nskicrmin;	//�����ػ�������
		int	m_nskicrmax;	//�����ػ�������
		int	m_nliferemin;	//�����ָ��ٶ�����
		int	m_nliferemax;	//�����ָ��ٶ�����
		int	m_nmanaremin;	//�����ָ��ٶ�����
		int	m_nmanaremax;	//�����ָ��ٶ�����
		int	m_nrpmin;	//����������
		int	m_nrpmax;	//����������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������Ӧ ID = 23
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum jinglianduiying_Fields {
		jinglianduiying_itemID = 0,	//����ID
		jinglianduiying_fineID = 1,	//����ID
		jinglianduiying_Max = 2
	};
	// ��ϵ���ݽṹ
	struct jinglianduiying_t {
		int	m_nitemID;	//����ID
		int	m_nfineID;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��Ϣ�б� ID = 24
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum xiaoxiilebiao_Fields {
		xiaoxiilebiao_ID = 0,	//ID
		xiaoxiilebiao_type = 1,	//��Ϣ����
		xiaoxiilebiao_message = 2,	//��Ϣ����
		xiaoxiilebiao_explain = 3,	//��Ϣ����
		xiaoxiilebiao_anothername = 4,	//��Ϣ����
		xiaoxiilebiao_Max = 5
	};
	// ������ (null) ö��ֵ����
	enum xiaoxiilebiao_type_Enum {
		xiaoxiilebiao_type_0,	//
		xiaoxiilebiao_type_1,	//
		xiaoxiilebiao_type_2,	//
		xiaoxiilebiao_type_3,	//
		xiaoxiilebiao_type_4,	//
		xiaoxiilebiao_type_5,	//
		xiaoxiilebiao_type_6,	//
		xiaoxiilebiao_type_7,	//
		xiaoxiilebiao_type_8,	//
		xiaoxiilebiao_type_9,	//
		xiaoxiilebiao_typeMax = 10
	};
	// ��ϵ���ݽṹ
	struct xiaoxiilebiao_t {
		int	m_nID;	//ID
		xiaoxiilebiao_type_Enum	m_etype;	//��Ϣ����
		char	m_szmessage[64];	//��Ϣ����
		char	m_szexplain[64];	//��Ϣ����
		char	m_szanothername[64];	//��Ϣ����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �ƺ���� ID = 25
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum title_Fields {
		title_newcall = 0,	//�³ƺ�
		title_membercall = 1,	//��Ա�ƺ�
		title_Max = 2
	};
	// ��ϵ���ݽṹ
	struct title_t {
		int	m_nnewcall;	//�³ƺ�
		int	m_nmembercall;	//��Ա�ƺ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ͼ����� ID = 26
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum ditufuhuodian_Fields {
		ditufuhuodian_mapid = 0,	//������ͼid
		ditufuhuodian_Type = 1,	//���������
		ditufuhuodian_x = 2,	//x
		ditufuhuodian_y = 3,	//y
		ditufuhuodian_w = 4,	//w
		ditufuhuodian_h = 5,	//h
		ditufuhuodian_job = 6,	//ְҵ
		ditufuhuodian_deadmapid = 7,	//����ʱ��ͼID
		ditufuhuodian_deadkcid = 8,	//����ʱ��ID
		ditufuhuodian_Max = 9
	};
	// ������ (null) ö��ֵ����
	enum ditufuhuodian_Type_Enum {
		ditufuhuodian_Type_0,	//
		ditufuhuodian_Type_1,	//
		ditufuhuodian_Type_2,	//
		ditufuhuodian_Type_3,	//
		ditufuhuodian_Type_4,	//
		ditufuhuodian_TypeMax = 5
	};
	// ������ (null) ö��ֵ����
	enum ditufuhuodian_job_Enum {
		ditufuhuodian_job_0,	//
		ditufuhuodian_job_1,	//
		ditufuhuodian_job_2,	//
		ditufuhuodian_job_3,	//
		ditufuhuodian_job_4,	//
		ditufuhuodian_jobMax = 5
	};
	// ��ϵ���ݽṹ
	struct ditufuhuodian_t {
		int	m_nmapid;	//������ͼid
		ditufuhuodian_Type_Enum	m_eType;	//���������
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
		ditufuhuodian_job_Enum	m_ejob;	//ְҵ
		int	m_ndeadmapid;	//����ʱ��ͼID
		int	m_ndeadkcid;	//����ʱ��ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������ ID = 27
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum kuangchangshouwei_Fields {
		kuangchangshouwei_kuangchangid = 0,	//��ID
		kuangchangshouwei_kuangchangmingcheng = 1,	//������
		kuangchangshouwei_npcid = 2,	//NPCID
		kuangchangshouwei_X = 3,	//x
		kuangchangshouwei_Y = 4,	//y
		kuangchangshouwei_shouweifanhao = 5,	//��������
		kuangchangshouwei_Max = 6
	};
	// ��ϵ���ݽṹ
	struct kuangchangshouwei_t {
		int	m_nkuangchangid;	//��ID
		char	m_szkuangchangmingcheng[16];	//������
		int	m_nnpcid;	//NPCID
		int	m_nX;	//x
		int	m_nY;	//y
		int	m_nshouweifanhao;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���� ID = 28
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gonghui_Fields {
		gonghui_Lv = 0,	//�ȼ�
		gonghui_zuidarenkou = 1,	//����˿�
		gonghui_tie = 2,	//��
		gonghui_tong = 3,	//ͭ
		gonghui_yin = 4,	//��
		gonghui_jin = 5,	//��
		gonghui_yushi = 6,	//��ʯ
		gonghui_butian = 7,	//����
		gonghui_jinku = 8,	//���
		gonghui_LvupGold = 9,	//������Ǯ
		gonghui_Max = 10
	};
	// ��ϵ���ݽṹ
	struct gonghui_t {
		int	m_nLv;	//�ȼ�
		int	m_nzuidarenkou;	//����˿�
		int	m_ntie;	//��
		int	m_ntong;	//ͭ
		int	m_nyin;	//��
		int	m_njin;	//��
		int	m_nyushi;	//��ʯ
		int	m_nbutian;	//����
		int	m_njinku;	//���
		int	m_nLvupGold;	//������Ǯ
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���ģ��ģ�� ID = 29
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum charmodetype_Fields {
		charmodetype_attdistance = 0,	//��ͨ������Χ
		charmodetype_movespeed = 1,	//�ƶ��ٶ�
		charmodetype_runspeed = 2,	//�ܲ��ٶ�
		charmodetype_Max = 3
	};
	// ��ϵ���ݽṹ
	struct charmodetype_t {
		int	m_nattdistance;	//��ͨ������Χ
		int	m_nmovespeed;	//�ƶ��ٶ�
		int	m_nrunspeed;	//�ܲ��ٶ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �󳡾����Ӧ ID = 30
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum KCshouweiduiying_Fields {
		KCshouweiduiying_kcjzID = 0,	//�󳡾���ID
		KCshouweiduiying_swID = 1,	//����ID
		KCshouweiduiying_Max = 2
	};
	// ��ϵ���ݽṹ
	struct KCshouweiduiying_t {
		int	m_nkcjzID;	//�󳡾���ID
		int	m_nswID;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ NPC������ϵ ID = 31
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum NPCskillscontact_Fields {
		NPCskillscontact_NPCID = 0,	//NPCID
		NPCskillscontact_SlillsID = 1,	//SkillsID
		NPCskillscontact_releaseprobability = 2,	//�ͷŸ���
		NPCskillscontact_Max = 3
	};
	// ��ϵ���ݽṹ
	struct NPCskillscontact_t {
		int	m_nNPCID;	//NPCID
		int	m_nSlillsID;	//SkillsID
		int	m_nreleaseprobability;	//�ͷŸ���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����AI ID = 32
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum NPCAI_Fields {
		NPCAI_AIid = 0,	//AI���
		NPCAI_suijiyidong = 1,	//����ƶ�
		NPCAI_zhudonggongji = 2,	//��������
		NPCAI_bangzhutonglei = 3,	//����ͬ��
		NPCAI_tiaozhengmubiao = 4,	//����Ŀ��
		NPCAI_taopao = 5,	//����
		NPCAI_taopaogailv = 6,	//���ܸ���
		NPCAI_dili = 7,	//����
		NPCAI_beishanghai = 8,	//����ʱ�ɱ��˺�
		NPCAI_yuanchenggongji = 9,	//Զ�̹���
		NPCAI_zibao = 10,	//�Ա�
		NPCAI_zhaohuan = 11,	//�ٻ�
		NPCAI_beizhaohuan = 12,	//���ٻ�
		NPCAI_huixue = 13,	//�Զ��ָ�����
		NPCAI_shunyi = 14,	//˲��
		NPCAI_Max = 15
	};
	// ��ϵ���ݽṹ
	struct NPCAI_t {
		int	m_nAIid;	//AI���
		BOOL	m_bsuijiyidong;	//����ƶ�
		BOOL	m_bzhudonggongji;	//��������
		BOOL	m_bbangzhutonglei;	//����ͬ��
		BOOL	m_btiaozhengmubiao;	//����Ŀ��
		BOOL	m_btaopao;	//����
		int	m_ntaopaogailv;	//���ܸ���
		BOOL	m_bdili;	//����
		BOOL	m_bbeishanghai;	//����ʱ�ɱ��˺�
		BOOL	m_byuanchenggongji;	//Զ�̹���
		BOOL	m_bzibao;	//�Ա�
		BOOL	m_bzhaohuan;	//�ٻ�
		BOOL	m_bbeizhaohuan;	//���ٻ�
		BOOL	m_bhuixue;	//�Զ��ָ�����
		BOOL	m_bshunyi;	//˲��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �������� ID = 33
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum jingliandrop_Fields {
		jingliandrop_room = 0,	//��������
		jingliandrop_itemnum = 1,	//��Ʒ���
		jingliandrop_dorpclass = 2,	//�������
		jingliandrop_dropprobability = 3,	//�������
		jingliandrop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum jingliandrop_dorpclass_Enum {
		jingliandrop_dorpclass_0,	//
		jingliandrop_dorpclassMax = 1
	};
	// ��ϵ���ݽṹ
	struct jingliandrop_t {
		int	m_nroom;	//��������
		int	m_nitemnum;	//��Ʒ���
		jingliandrop_dorpclass_Enum	m_edorpclass;	//�������
		int	m_ndropprobability;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ʯ���� ID = 34
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gemdrop_Fields {
		gemdrop_room = 0,	//��������
		gemdrop_itemnum = 1,	//��Ʒ���
		gemdrop_dorpclass = 2,	//�������
		gemdrop_dropprobability = 3,	//�������
		gemdrop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum gemdrop_dorpclass_Enum {
		gemdrop_dorpclass_0,	//
		gemdrop_dorpclassMax = 1
	};
	// ��ϵ���ݽṹ
	struct gemdrop_t {
		int	m_nroom;	//��������
		int	m_nitemnum;	//��Ʒ���
		gemdrop_dorpclass_Enum	m_edorpclass;	//�������
		int	m_ndropprobability;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���ߵ��� ID = 35
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum itemdrop_Fields {
		itemdrop_room = 0,	//��������
		itemdrop_itemnum = 1,	//��Ʒ���
		itemdrop_dorpclass = 2,	//�������
		itemdrop_dropprobability = 3,	//�������
		itemdrop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum itemdrop_dorpclass_Enum {
		itemdrop_dorpclass_0,	//
		itemdrop_dorpclassMax = 1
	};
	// ��ϵ���ݽṹ
	struct itemdrop_t {
		int	m_nroom;	//��������
		int	m_nitemnum;	//��Ʒ���
		itemdrop_dorpclass_Enum	m_edorpclass;	//�������
		int	m_ndropprobability;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������� ID = 36
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum smithingdrop_Fields {
		smithingdrop_room = 0,	//��������
		smithingdrop_itemnum = 1,	//��Ʒ���
		smithingdrop_dorpclass = 2,	//�������
		smithingdrop_dropprobability = 3,	//�������
		smithingdrop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum smithingdrop_dorpclass_Enum {
		smithingdrop_dorpclass_0,	//
		smithingdrop_dorpclassMax = 1
	};
	// ��ϵ���ݽṹ
	struct smithingdrop_t {
		int	m_nroom;	//��������
		int	m_nitemnum;	//��Ʒ���
		smithingdrop_dorpclass_Enum	m_edorpclass;	//�������
		int	m_ndropprobability;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �����б� ID = 37
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum maptransmissionlist_Fields {
		maptransmissionlist_mapID = 0,	//������ͼ
		maptransmissionlist_mapname = 1,	//��ͼ����
		maptransmissionlist_x = 2,	//x
		maptransmissionlist_y = 3,	//y
		maptransmissionlist_w = 4,	//w
		maptransmissionlist_h = 5,	//h
		maptransmissionlist_Max = 6
	};
	// ��ϵ���ݽṹ
	struct maptransmissionlist_t {
		int	m_nmapID;	//������ͼ
		char	m_szmapname[16];	//��ͼ����
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���ͼ۸�� ID = 38
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum transmissionprice_Fields {
		transmissionprice_mapid = 0,	//��ͼ���
		transmissionprice_mapname = 1,	//��ͼ����
		transmissionprice_targetmapid = 2,	//Ŀ���ͼ���
		transmissionprice_targetmapname = 3,	//Ŀ���ͼ����
		transmissionprice_price = 4,	//�۸�
		transmissionprice_Max = 5
	};
	// ��ϵ���ݽṹ
	struct transmissionprice_t {
		int	m_nmapid;	//��ͼ���
		char	m_szmapname[16];	//��ͼ����
		int	m_ntargetmapid;	//Ŀ���ͼ���
		char	m_sztargetmapname[16];	//Ŀ���ͼ����
		int	m_nprice;	//�۸�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ǰ׺�������� ID = 39
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum fbmake_Fields {
		fbmake_odds = 0,	//����
		fbmake_square = 1,	//�Գ�
		fbmake_Max = 2
	};
	// ��ϵ���ݽṹ
	struct fbmake_t {
		int	m_nodds;	//����
		int	m_nsquare;	//�Գ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ˫ǰ׺�������� ID = 40
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum doublefbmake_Fields {
		doublefbmake_odds = 0,	//����
		doublefbmake_square = 1,	//�Գ�
		doublefbmake_othersquare = 2,	//����
		doublefbmake_Max = 3
	};
	// ��ϵ���ݽṹ
	struct doublefbmake_t {
		int	m_nodds;	//����
		int	m_nsquare;	//�Գ�
		int	m_nothersquare;	//����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ģ�� ID = 41
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum LurkShape_Fields {
		LurkShape_MapID = 0,	//��ͼID
		LurkShape_ModelFile = 1,	//ģ����
		LurkShape_Max = 2
	};
	// ��ϵ���ݽṹ
	struct LurkShape_t {
		int	m_nMapID;	//��ͼID
		char	m_szModelFile[256];	//ģ����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��������ٷֱ� ID = 42
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum skillspecialpercent_Fields {
		skillspecialpercent_signname = 0,	//�������
		skillspecialpercent_signlv = 1,	//��ǵȼ�
		skillspecialpercent_percent = 2,	//�ٷֱ�
		skillspecialpercent_Max = 3
	};
	// ������ (null) ö��ֵ����
	enum skillspecialpercent_signname_Enum {
		skillspecialpercent_signname_0,	//
		skillspecialpercent_signname_1,	//
		skillspecialpercent_signnameMax = 2
	};
	// ��ϵ���ݽṹ
	struct skillspecialpercent_t {
		skillspecialpercent_signname_Enum	m_esignname;	//�������
		int	m_nsignlv;	//��ǵȼ�
		int	m_npercent;	//�ٷֱ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ͼѰ· ID = 43
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum mapway_Fields {
		mapway_Max = 0
	};
	// ��ϵ���ݽṹ
	struct mapway_t {
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������λ��ϵ�� ID = 44
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum jinglianlianxibiao_Fields {
		jinglianlianxibiao_part = 0,	//��λ
		jinglianlianxibiao_equippart = 1,	//��Ӧװ����λ
		jinglianlianxibiao_Max = 2
	};
	// ������ (null) ö��ֵ����
	enum jinglianlianxibiao_part_Enum {
		jinglianlianxibiao_part_0,	//
		jinglianlianxibiao_part_1,	//
		jinglianlianxibiao_part_2,	//
		jinglianlianxibiao_part_3,	//
		jinglianlianxibiao_partMax = 4
	};
	// ������ (null) ö��ֵ����
	enum jinglianlianxibiao_equippart_Enum {
		jinglianlianxibiao_equippart_0,	//
		jinglianlianxibiao_equippart_1,	//
		jinglianlianxibiao_equippart_2,	//
		jinglianlianxibiao_equippart_3,	//
		jinglianlianxibiao_equippart_4,	//
		jinglianlianxibiao_equippart_5,	//
		jinglianlianxibiao_equippart_6,	//
		jinglianlianxibiao_equippart_7,	//
		jinglianlianxibiao_equippart_8,	//
		jinglianlianxibiao_equippart_9,	//
		jinglianlianxibiao_equippart_10,	//
		jinglianlianxibiao_equippart_11,	//
		jinglianlianxibiao_equippartMax = 12
	};
	// ��ϵ���ݽṹ
	struct jinglianlianxibiao_t {
		jinglianlianxibiao_part_Enum	m_epart;	//��λ
		jinglianlianxibiao_equippart_Enum	m_eequippart;	//��Ӧװ����λ
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ װ������ ID = 45
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum equipmake_Fields {
		equipmake_ID = 0,	//ְҵ
		equipmake_pinzhi = 1,	//Ʒ��
		equipmake_jiazhi = 2,	//��ֵ
		equipmake_wumian = 3,	//����
		equipmake_momian = 4,	//ħ��
		equipmake_pingjunshangmian = 5,	//ƽ������
		equipmake_kanzhefangbian = 6,	//���ŷ���
		equipmake_Max = 7
	};
	// ������ (null) ö��ֵ����
	enum equipmake_ID_Enum {
		equipmake_ID_0,	//
		equipmake_ID_1,	//
		equipmake_ID_2,	//
		equipmake_ID_3,	//
		equipmake_ID_4,	//
		equipmake_IDMax = 5
	};
	// ������ (null) ö��ֵ����
	enum equipmake_pinzhi_Enum {
		equipmake_pinzhi_0,	//
		equipmake_pinzhi_1,	//
		equipmake_pinzhi_2,	//
		equipmake_pinzhi_3,	//
		equipmake_pinzhi_4,	//
		equipmake_pinzhi_5,	//
		equipmake_pinzhi_6,	//
		equipmake_pinzhi_7,	//
		equipmake_pinzhi_8,	//
		equipmake_pinzhiMax = 9
	};
	// ��ϵ���ݽṹ
	struct equipmake_t {
		equipmake_ID_Enum	m_eID;	//ְҵ
		equipmake_pinzhi_Enum	m_epinzhi;	//Ʒ��
		int	m_njiazhi;	//��ֵ
		float	m_fwumian;	//����
		float	m_fmomian;	//ħ��
		float	m_fpingjunshangmian;	//ƽ������
		char	m_szkanzhefangbian[16];	//���ŷ���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��λϵ���� ID = 47
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum buweixishu_Fields {
		buweixishu_buwei = 0,	//��λ
		buweixishu_buweixishu = 1,	//��λϵ��
		buweixishu_pricexishu = 2,	//�۸�ϵ��
		buweixishu_Max = 3
	};
	// ������ (null) ö��ֵ����
	enum buweixishu_buwei_Enum {
		buweixishu_buwei_0,	//
		buweixishu_buwei_1,	//
		buweixishu_buwei_2,	//
		buweixishu_buwei_3,	//
		buweixishu_buwei_4,	//
		buweixishu_buwei_5,	//
		buweixishu_buwei_6,	//
		buweixishu_buwei_7,	//
		buweixishu_buwei_8,	//
		buweixishu_buwei_9,	//
		buweixishu_buwei_10,	//
		buweixishu_buwei_11,	//
		buweixishu_buweiMax = 12
	};
	// ��ϵ���ݽṹ
	struct buweixishu_t {
		buweixishu_buwei_Enum	m_ebuwei;	//��λ
		int	m_nbuweixishu;	//��λϵ��
		float	m_fpricexishu;	//�۸�ϵ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������Զ��ձ� ID = 48
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum sjsxxishubiao_Fields {
		sjsxxishubiao_id = 0,	//ID
		sjsxxishubiao_class = 1,	//ְҵ
		sjsxxishubiao_equiplv = 2,	//��Ʒ�ȼ�����
		sjsxxishubiao_addvalue = 3,	//��ֵ
		sjsxxishubiao_strmin = 4,	//��������
		sjsxxishubiao_strmax = 5,	//��������
		sjsxxishubiao_dexmin = 6,	//��������
		sjsxxishubiao_dexmax = 7,	//��������
		sjsxxishubiao_conmin = 8,	//��������
		sjsxxishubiao_conmax = 9,	//��������
		sjsxxishubiao_powermin = 10,	//��������
		sjsxxishubiao_powermax = 11,	//��������
		sjsxxishubiao_intmin = 12,	//�ǻ�����
		sjsxxishubiao_intmax = 13,	//�ǻ�����
		sjsxxishubiao_minattmin = 14,	//��С�﹥����
		sjsxxishubiao_minattmax = 15,	//��С�﹥����
		sjsxxishubiao_maxattmin = 16,	//����﹥����
		sjsxxishubiao_maxattmax = 17,	//����﹥����
		sjsxxishubiao_defmin = 18,	//�������
		sjsxxishubiao_defmax = 19,	//�������
		sjsxxishubiao_mdefmin = 20,	//��������
		sjsxxishubiao_mdefmax = 21,	//��������
		sjsxxishubiao_mattmin = 22,	//��������
		sjsxxishubiao_mattmax = 23,	//��������
		sjsxxishubiao_hpmin = 24,	//�������ֵ����
		sjsxxishubiao_homax = 25,	//�������ֵ����
		sjsxxishubiao_mpmin = 26,	//�����ֵ����
		sjsxxishubiao_mpmax = 27,	//�����ֵ����
		sjsxxishubiao_hitmin = 28,	//��������
		sjsxxishubiao_hitmax = 29,	//��������
		sjsxxishubiao_dodgemin = 30,	//��������
		sjsxxishubiao_dodgemax = 31,	//��������
		sjsxxishubiao_phycrmin = 32,	//�ﱬ����
		sjsxxishubiao_phycrmax = 33,	//�ﱬ����
		sjsxxishubiao_skicrmin = 34,	//��������
		sjsxxishubiao_skicrmax = 35,	//��������
		sjsxxishubiao_hpremin = 36,	//�����ָ��ٶ�����
		sjsxxishubiao_hpremax = 37,	//�����ָ��ٶ�����
		sjsxxishubiao_mpremin = 38,	//�����ָ��ٶ�����
		sjsxxishubiao_moremax = 39,	//�����ָ��ٶ�����
		sjsxxishubiao_rpmin = 40,	//��������
		sjsxxishubiao_rpmax = 41,	//��������
		sjsxxishubiao_Max = 42
	};
	// ������ (null) ö��ֵ����
	enum sjsxxishubiao_class_Enum {
		sjsxxishubiao_class_0,	//
		sjsxxishubiao_class_1,	//
		sjsxxishubiao_class_2,	//
		sjsxxishubiao_class_3,	//
		sjsxxishubiao_classMax = 4
	};
	// ��ϵ���ݽṹ
	struct sjsxxishubiao_t {
		int	m_nid;	//ID
		sjsxxishubiao_class_Enum	m_eclass;	//ְҵ
		int	m_nequiplv;	//��Ʒ�ȼ�����
		int	m_naddvalue;	//��ֵ
		int	m_nstrmin;	//��������
		int	m_nstrmax;	//��������
		int	m_ndexmin;	//��������
		int	m_ndexmax;	//��������
		int	m_nconmin;	//��������
		int	m_nconmax;	//��������
		int	m_npowermin;	//��������
		int	m_npowermax;	//��������
		int	m_nintmin;	//�ǻ�����
		int	m_nintmax;	//�ǻ�����
		int	m_nminattmin;	//��С�﹥����
		int	m_nminattmax;	//��С�﹥����
		int	m_nmaxattmin;	//����﹥����
		int	m_nmaxattmax;	//����﹥����
		int	m_ndefmin;	//�������
		int	m_ndefmax;	//�������
		int	m_nmdefmin;	//��������
		int	m_nmdefmax;	//��������
		int	m_nmattmin;	//��������
		int	m_nmattmax;	//��������
		int	m_nhpmin;	//�������ֵ����
		int	m_nhomax;	//�������ֵ����
		int	m_nmpmin;	//�����ֵ����
		int	m_nmpmax;	//�����ֵ����
		int	m_nhitmin;	//��������
		int	m_nhitmax;	//��������
		int	m_ndodgemin;	//��������
		int	m_ndodgemax;	//��������
		int	m_nphycrmin;	//�ﱬ����
		int	m_nphycrmax;	//�ﱬ����
		int	m_nskicrmin;	//��������
		int	m_nskicrmax;	//��������
		int	m_nhpremin;	//�����ָ��ٶ�����
		int	m_nhpremax;	//�����ָ��ٶ�����
		int	m_nmpremin;	//�����ָ��ٶ�����
		int	m_nmoremax;	//�����ָ��ٶ�����
		int	m_nrpmin;	//��������
		int	m_nrpmax;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �̶����Զ��ձ� ID = 49
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum gdsxduizhaobiao_Fields {
		gdsxduizhaobiao_part = 0,	//��λ
		gdsxduizhaobiao_job = 1,	//ְҵ
		gdsxduizhaobiao_shuxingID = 2,	//����ID
		gdsxduizhaobiao_Max = 3
	};
	// ������ (null) ö��ֵ����
	enum gdsxduizhaobiao_part_Enum {
		gdsxduizhaobiao_part_0,	//
		gdsxduizhaobiao_part_1,	//
		gdsxduizhaobiao_part_2,	//
		gdsxduizhaobiao_part_3,	//
		gdsxduizhaobiao_part_4,	//
		gdsxduizhaobiao_part_5,	//
		gdsxduizhaobiao_part_6,	//
		gdsxduizhaobiao_part_7,	//
		gdsxduizhaobiao_part_8,	//
		gdsxduizhaobiao_part_9,	//
		gdsxduizhaobiao_part_10,	//
		gdsxduizhaobiao_part_11,	//
		gdsxduizhaobiao_partMax = 12
	};
	// ������ (null) ö��ֵ����
	enum gdsxduizhaobiao_job_Enum {
		gdsxduizhaobiao_job_0,	//
		gdsxduizhaobiao_job_1,	//
		gdsxduizhaobiao_job_2,	//
		gdsxduizhaobiao_job_3,	//
		gdsxduizhaobiao_job_4,	//
		gdsxduizhaobiao_jobMax = 5
	};
	// ��ϵ���ݽṹ
	struct gdsxduizhaobiao_t {
		gdsxduizhaobiao_part_Enum	m_epart;	//��λ
		gdsxduizhaobiao_job_Enum	m_ejob;	//ְҵ
		int	m_nshuxingID;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��Ԥ���б� ID = 50
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum canpreviewlist_Fields {
		canpreviewlist_cellname = 0,	//��Ԫ����
		canpreviewlist_cellid = 1,	//��ԪID
		canpreviewlist_kindid = 2,	//����ID
		canpreviewlist_Max = 3
	};
	// ��ϵ���ݽṹ
	struct canpreviewlist_t {
		char	m_szcellname[16];	//��Ԫ����
		int	m_ncellid;	//��ԪID
		int	m_nkindid;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �����ϵ�� ID = 51
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum bagcontactlist_Fields {
		bagcontactlist_bagid = 0,	//���ID
		bagcontactlist_goodscell = 1,	//���ﵥԪ
		bagcontactlist_goodsid = 2,	//����ID
		bagcontactlist_goodsnumber = 3,	//��������
		bagcontactlist_appearprobability = 4,	//���ָ���
		bagcontactlist_Max = 5
	};
	// ��ϵ���ݽṹ
	struct bagcontactlist_t {
		int	m_nbagid;	//���ID
		int	m_ngoodscell;	//���ﵥԪ
		int	m_ngoodsid;	//����ID
		int	m_ngoodsnumber;	//��������
		int	m_nappearprobability;	//���ָ���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ϡ��������Զ��ձ� ID = 52
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum xysjsxlist_Fields {
		xysjsxlist_id = 0,	//ID
		xysjsxlist_buwei = 1,	//��λ
		xysjsxlist_strmin = 2,	//��������
		xysjsxlist_strmax = 3,	//��������
		xysjsxlist_dexmin = 4,	//��������
		xysjsxlist_dexmax = 5,	//��������
		xysjsxlist_poewrmin = 6,	//��������
		xysjsxlist_powermax = 7,	//��������
		xysjsxlist_wismin = 8,	//�ǻ�����
		xysjsxlist_wismax = 9,	//�ǻ�����
		xysjsxlist_hpmin = 10,	//�������ֵ����
		xysjsxlist_hpmax = 11,	//�������ֵ����
		xysjsxlist_mpmin = 12,	//�����ֵ����
		xysjsxlist_mpmax = 13,	//�����ֵ����
		xysjsxlist_shjmmin = 14,	//�˺���������
		xysjsxlist_shjmmax = 15,	//�˺���������
		xysjsxlist_conmin = 16,	//��������
		xysjsxlist_conmax = 17,	//��������
		xysjsxlist_defmin = 18,	//�������������
		xysjsxlist_defmax = 19,	//�������������
		xysjsxlist_mdefmin = 20,	//��������������
		xysjsxlist_mdefmax = 21,	//��������������
		xysjsxlist_shftmin = 22,	//�˺���������
		xysjsxlist_shftmax = 23,	//�˺���������
		xysjsxlist_atkmin = 24,	//��󹥻�������
		xysjsxlist_atkmax = 25,	//��󹥻�������
		xysjsxlist_matkmin = 26,	//��������
		xysjsxlist_matkmax = 27,	//��������
		xysjsxlist_Max = 28
	};
	// ������ (null) ö��ֵ����
	enum xysjsxlist_buwei_Enum {
		xysjsxlist_buwei_0,	//
		xysjsxlist_buwei_1,	//
		xysjsxlist_buwei_2,	//
		xysjsxlist_buwei_3,	//
		xysjsxlist_buwei_4,	//
		xysjsxlist_buwei_5,	//
		xysjsxlist_buwei_6,	//
		xysjsxlist_buwei_7,	//
		xysjsxlist_buwei_8,	//
		xysjsxlist_buwei_9,	//
		xysjsxlist_buwei_10,	//
		xysjsxlist_buwei_11,	//
		xysjsxlist_buweiMax = 12
	};
	// ��ϵ���ݽṹ
	struct xysjsxlist_t {
		int	m_nid;	//ID
		xysjsxlist_buwei_Enum	m_ebuwei;	//��λ
		int	m_nstrmin;	//��������
		int	m_nstrmax;	//��������
		int	m_ndexmin;	//��������
		int	m_ndexmax;	//��������
		int	m_npoewrmin;	//��������
		int	m_npowermax;	//��������
		int	m_nwismin;	//�ǻ�����
		int	m_nwismax;	//�ǻ�����
		int	m_nhpmin;	//�������ֵ����
		int	m_nhpmax;	//�������ֵ����
		int	m_nmpmin;	//�����ֵ����
		int	m_nmpmax;	//�����ֵ����
		int	m_nshjmmin;	//�˺���������
		int	m_nshjmmax;	//�˺���������
		int	m_nconmin;	//��������
		int	m_nconmax;	//��������
		int	m_ndefmin;	//�������������
		int	m_ndefmax;	//�������������
		int	m_nmdefmin;	//��������������
		int	m_nmdefmax;	//��������������
		int	m_nshftmin;	//�˺���������
		int	m_nshftmax;	//�˺���������
		int	m_natkmin;	//��󹥻�������
		int	m_natkmax;	//��󹥻�������
		int	m_nmatkmin;	//��������
		int	m_nmatkmax;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���� ID = 53
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum smeltstone_Fields {
		smeltstone_character = 0,	//Ʒ��
		smeltstone_equiplv = 1,	//��Ʒ�ȼ�
		smeltstone_goodsid = 2,	//��Ʒ���
		smeltstone_fusionprice = 3,	//�����۸�
		smeltstone_composenumber = 4,	//�ϳ���
		smeltstone_composeid = 5,	//�ϳɱ��
		smeltstone_fuseprice = 6,	//�ںϼ۸�
		smeltstone_Max = 7
	};
	// ������ (null) ö��ֵ����
	enum smeltstone_character_Enum {
		smeltstone_character_0,	//
		smeltstone_character_1,	//
		smeltstone_character_2,	//
		smeltstone_character_3,	//
		smeltstone_character_4,	//
		smeltstone_character_5,	//
		smeltstone_characterMax = 6
	};
	// ��ϵ���ݽṹ
	struct smeltstone_t {
		smeltstone_character_Enum	m_echaracter;	//Ʒ��
		int	m_nequiplv;	//��Ʒ�ȼ�
		int	m_ngoodsid;	//��Ʒ���
		int	m_nfusionprice;	//�����۸�
		int	m_ncomposenumber;	//�ϳ���
		int	m_ncomposeid;	//�ϳɱ��
		int	m_nfuseprice;	//�ںϼ۸�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �����������ձ� ID = 54
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum stonelvuplist_Fields {
		stonelvuplist_lvuotype = 0,	//��������
		stonelvuplist_id = 1,	//ID
		stonelvuplist_num = 2,	//��Ҫ����
		stonelvuplist_scid = 3,	//����ID
		stonelvuplist_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum stonelvuplist_lvuotype_Enum {
		stonelvuplist_lvuotype_0,	//
		stonelvuplist_lvuotype_1,	//
		stonelvuplist_lvuotypeMax = 2
	};
	// ��ϵ���ݽṹ
	struct stonelvuplist_t {
		stonelvuplist_lvuotype_Enum	m_elvuotype;	//��������
		int	m_nid;	//ID
		int	m_nnum;	//��Ҫ����
		int	m_nscid;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ͼ�ڴ����б� ID = 55
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum dtncslist_Fields {
		dtncslist_mapid = 0,	//��ͼID
		dtncslist_id = 1,	//���͵�ID
		dtncslist_name = 2,	//���͵�����
		dtncslist_x = 3,	//����X
		dtncslist_y = 4,	//����Y
		dtncslist_Max = 5
	};
	// ��ϵ���ݽṹ
	struct dtncslist_t {
		int	m_nmapid;	//��ͼID
		int	m_nid;	//���͵�ID
		char	m_szname[16];	//���͵�����
		int	m_nx;	//����X
		int	m_ny;	//����Y
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ֲ��� ID = 56
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum resourcelist_Fields {
		resourcelist_day = 0,	//����
		resourcelist_hour = 1,	//�ӵ�
		resourcelist_frequency = 2,	//����Ƶ��
		resourcelist_magnification = 3,	//��������
		resourcelist_Max = 4
	};
	// ��ϵ���ݽṹ
	struct resourcelist_t {
		int	m_nday;	//����
		int	m_nhour;	//�ӵ�
		int	m_nfrequency;	//����Ƶ��
		int	m_nmagnification;	//��������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �ճ�����ȼ����� ID = 57
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum richangrenwulvarea_Fields {
		richangrenwulvarea_Max = 0
	};
	// ��ϵ���ݽṹ
	struct richangrenwulvarea_t {
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������Ե���ɫ����ӳ��� ID = 58
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum sjsxdjssxysb_Fields {
		sjsxdjssxysb_kinds = 0,	//����
		sjsxdjssxysb_id = 1,	//ID
		sjsxdjssxysb_target = 2,	//Ŀ��
		sjsxdjssxysb_Max = 3
	};
	// ������ (null) ö��ֵ����
	enum sjsxdjssxysb_kinds_Enum {
		sjsxdjssxysb_kinds_0,	//
		sjsxdjssxysb_kinds_1,	//
		sjsxdjssxysb_kindsMax = 2
	};
	// ��ϵ���ݽṹ
	struct sjsxdjssxysb_t {
		sjsxdjssxysb_kinds_Enum	m_ekinds;	//����
		int	m_nid;	//ID
		int	m_ntarget;	//Ŀ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����NPC�б� ID = 59
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum csnpclb_Fields {
		csnpclb_npcid = 0,	//npcid
		csnpclb_csdname = 1,	//���͵�����
		csnpclb_mapid = 2,	//������ͼid
		csnpclb_meishuX = 3,	//����X
		csnpclb_meishuY = 4,	//����Y
		csnpclb_tips = 5,	//tips
		csnpclb_buttonid = 6,	//��ťid
		csnpclb_Max = 7
	};
	// ��ϵ���ݽṹ
	struct csnpclb_t {
		int	m_nnpcid;	//npcid
		char	m_szcsdname[16];	//���͵�����
		int	m_nmapid;	//������ͼid
		int	m_nmeishuX;	//����X
		int	m_nmeishuY;	//����Y
		char	m_sztips[128];	//tips
		int	m_nbuttonid;	//��ťid
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �������ͱ� ID = 60
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum roletypelist_Fields {
		roletypelist_roletypeid = 0,	//��������ID
		roletypelist_type = 1,	//����
		roletypelist_zqqs = 2,	//������ʼ��
		roletypelist_zqsc = 3,	//����ʱ��
		roletypelist_kjsdqs = 4,	//�ɽ�ʱ����ʼ
		roletypelist_kjsdzj = 5,	//�ɽ�ʱ���ս�
		roletypelist_ls = 6,	//����
		roletypelist_hs = 7,	//����
		roletypelist_jfzdz = 8,	//�������ֵ
		roletypelist_Max = 9
	};
	// ������ (null) ö��ֵ����
	enum roletypelist_type_Enum {
		roletypelist_type_0,	//
		roletypelist_type_1,	//
		roletypelist_type_2,	//
		roletypelist_typeMax = 3
	};
	// ��ϵ���ݽṹ
	struct roletypelist_t {
		int	m_nroletypeid;	//��������ID
		roletypelist_type_Enum	m_etype;	//����
		int	m_nzqqs;	//������ʼ��
		int	m_nzqsc;	//����ʱ��
		int	m_nkjsdqs;	//�ɽ�ʱ����ʼ
		int	m_nkjsdzj;	//�ɽ�ʱ���ս�
		int	m_nls;	//����
		int	m_nhs;	//����
		int	m_njfzdz;	//�������ֵ
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����ȼ�����Ͷ�Ӧ�� ID = 61
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum rolelvareaanddepotid_Fields {
		rolelvareaanddepotid_roletypeid = 0,	//��������ID
		rolelvareaanddepotid_lvmin = 1,	//�ȼ�����
		rolelvareaanddepotid_lvmax = 2,	//�ȼ�����
		rolelvareaanddepotid_roledepotid = 3,	//�����ID
		rolelvareaanddepotid_Max = 4
	};
	// ��ϵ���ݽṹ
	struct rolelvareaanddepotid_t {
		int	m_nroletypeid;	//��������ID
		int	m_nlvmin;	//�ȼ�����
		int	m_nlvmax;	//�ȼ�����
		int	m_nroledepotid;	//�����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��������Ӧ�� ID = 62
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum depotidandroleidlist_Fields {
		depotidandroleidlist_roletypeid = 0,	//�����ID
		depotidandroleidlist_roleid = 1,	//����ID
		depotidandroleidlist_Max = 2
	};
	// ��ϵ���ݽṹ
	struct depotidandroleidlist_t {
		int	m_nroletypeid;	//�����ID
		int	m_nroleid;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �ɽ������б� ID = 63
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum replacerolelist_Fields {
		replacerolelist_roletypeid = 0,	//��������ID
		replacerolelist_option = 1,	//����
		replacerolelist_oddswhite = 2,	//���ʰ�
		replacerolelist_oddsgreen = 3,	//������
		replacerolelist_oddsbule = 4,	//������
		replacerolelist_oddspurple = 5,	//������
		replacerolelist_oddsorange = 6,	//���ʳ�
		replacerolelist_Max = 7
	};
	// ��ϵ���ݽṹ
	struct replacerolelist_t {
		int	m_nroletypeid;	//��������ID
		int	m_noption;	//����
		int	m_noddswhite;	//���ʰ�
		int	m_noddsgreen;	//������
		int	m_noddsbule;	//������
		int	m_noddspurple;	//������
		int	m_noddsorange;	//���ʳ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���ֽ����� ID = 64
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum integralaward_Fields {
		integralaward_rolekind = 0,	//��������
		integralaward_lvmin = 1,	//�ȼ�����
		integralaward_lvmax = 2,	//�ȼ�����
		integralaward_integralmax = 3,	//�������ֵ
		integralaward_integralvalueone = 4,	//���ֽ���ֵһ
		integralaward_integralawardonekind = 5,	//���ֽ�Ʒһ����
		integralaward_integralawardoneid = 6,	//���ֽ�ƷһID
		integralaward_integralawardonenum = 7,	//���ֽ�Ʒһ����
		integralaward_integralvaluetwo = 8,	//���ֽ���ֵ��
		integralaward_integralawardtwokind = 9,	//���ֽ�Ʒ������
		integralaward_integralawardtwoid = 10,	//���ֽ�Ʒ��ID
		integralaward_integralawardtwonum = 11,	//���ֽ�Ʒ������
		integralaward_integralvaluethree = 12,	//���ֽ���ֵ��
		integralaward_integralawardthreekind = 13,	//���ֽ�Ʒ������
		integralaward_integralawardthreeid = 14,	//���ֽ�Ʒ��ID
		integralaward_integralawardthreenum = 15,	//���ֽ�Ʒ������
		integralaward_integralvaluefour = 16,	//���ֽ���ֵ��
		integralaward_integralawardfourkind = 17,	//���ֽ�Ʒ������
		integralaward_integralawardfourid = 18,	//���ֽ�Ʒ��ID
		integralaward_integralawardfournum = 19,	//���ֽ�Ʒ������
		integralaward_integralvaluefive = 20,	//���ֽ���ֵ��
		integralaward_integralawardfivekind = 21,	//���ֽ�Ʒ������
		integralaward_integralawardfiveid = 22,	//���ֽ�Ʒ��ID
		integralaward_integralawardfivenum = 23,	//���ֽ�Ʒ������
		integralaward_Max = 24
	};
	// ��ϵ���ݽṹ
	struct integralaward_t {
		int	m_nrolekind;	//��������
		int	m_nlvmin;	//�ȼ�����
		int	m_nlvmax;	//�ȼ�����
		int	m_nintegralmax;	//�������ֵ
		int	m_nintegralvalueone;	//���ֽ���ֵһ
		int	m_nintegralawardonekind;	//���ֽ�Ʒһ����
		int	m_nintegralawardoneid;	//���ֽ�ƷһID
		int	m_nintegralawardonenum;	//���ֽ�Ʒһ����
		int	m_nintegralvaluetwo;	//���ֽ���ֵ��
		int	m_nintegralawardtwokind;	//���ֽ�Ʒ������
		int	m_nintegralawardtwoid;	//���ֽ�Ʒ��ID
		int	m_nintegralawardtwonum;	//���ֽ�Ʒ������
		int	m_nintegralvaluethree;	//���ֽ���ֵ��
		int	m_nintegralawardthreekind;	//���ֽ�Ʒ������
		int	m_nintegralawardthreeid;	//���ֽ�Ʒ��ID
		int	m_nintegralawardthreenum;	//���ֽ�Ʒ������
		int	m_nintegralvaluefour;	//���ֽ���ֵ��
		int	m_nintegralawardfourkind;	//���ֽ�Ʒ������
		int	m_nintegralawardfourid;	//���ֽ�Ʒ��ID
		int	m_nintegralawardfournum;	//���ֽ�Ʒ������
		int	m_nintegralvaluefive;	//���ֽ���ֵ��
		int	m_nintegralawardfivekind;	//���ֽ�Ʒ������
		int	m_nintegralawardfiveid;	//���ֽ�Ʒ��ID
		int	m_nintegralawardfivenum;	//���ֽ�Ʒ������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ʦͽ������ ID = 65
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum shitujianglilist_Fields {
		shitujianglilist_lv = 0,	//ͽ�ܵȼ�
		shitujianglilist_gdjfjl = 1,	//���»��ֽ���
		shitujianglilist_Max = 2
	};
	// ��ϵ���ݽṹ
	struct shitujianglilist_t {
		int	m_nlv;	//ͽ�ܵȼ�
		int	m_ngdjfjl;	//���»��ֽ���
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������ܶ� ID = 66
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum jiebaiqinmidu_Fields {
		jiebaiqinmidu_id = 0,	//ID
		jiebaiqinmidu_name = 1,	//����
		jiebaiqinmidu_lvupqinmidu = 2,	//�����������ܶ�
		jiebaiqinmidu_qingyidu = 3,	//�����
		jiebaiqinmidu_Max = 4
	};
	// ��ϵ���ݽṹ
	struct jiebaiqinmidu_t {
		int	m_nid;	//ID
		char	m_szname[16];	//����
		int	m_nlvupqinmidu;	//�����������ܶ�
		int	m_nqingyidu;	//�����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �Թ⵵�� ID = 67
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum huiguangdangci_Fields {
		huiguangdangci_id = 0,	//ID
		huiguangdangci_houdu = 1,	//���
		huiguangdangci_qiangdu = 2,	//ǿ��
		huiguangdangci_Max = 3
	};
	// ��ϵ���ݽṹ
	struct huiguangdangci_t {
		int	m_nid;	//ID
		float	m_fhoudu;	//���
		float	m_fqiangdu;	//ǿ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �Թ���ɫ ID = 68
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum huiguangyanse_Fields {
		huiguangyanse_id = 0,	//id
		huiguangyanse_rgb = 1,	//RGB
		huiguangyanse_Max = 2
	};
	// ��ϵ���ݽṹ
	struct huiguangyanse_t {
		int	m_nid;	//id
		char	m_szrgb[16];	//RGB
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ���ֵ�ͼת�� ID = 69
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum xinshouzhuantiao_Fields {
		xinshouzhuantiao_maptiaozhuan = 0,	//��ͼת��
		xinshouzhuantiao_mapid = 1,	//��ͼID
		xinshouzhuantiao_zhiye = 2,	//ְҵ
		xinshouzhuantiao_x = 3,	//X����
		xinshouzhuantiao_y = 4,	//Y����
		xinshouzhuantiao_Max = 5
	};
	// ������ (null) ö��ֵ����
	enum xinshouzhuantiao_maptiaozhuan_Enum {
		xinshouzhuantiao_maptiaozhuan_0,	//
		xinshouzhuantiao_maptiaozhuan_1,	//
		xinshouzhuantiao_maptiaozhuanMax = 2
	};
	// ������ (null) ö��ֵ����
	enum xinshouzhuantiao_zhiye_Enum {
		xinshouzhuantiao_zhiye_0,	//
		xinshouzhuantiao_zhiye_1,	//
		xinshouzhuantiao_zhiye_2,	//
		xinshouzhuantiao_zhiye_3,	//
		xinshouzhuantiao_zhiyeMax = 4
	};
	// ��ϵ���ݽṹ
	struct xinshouzhuantiao_t {
		xinshouzhuantiao_maptiaozhuan_Enum	m_emaptiaozhuan;	//��ͼת��
		int	m_nmapid;	//��ͼID
		xinshouzhuantiao_zhiye_Enum	m_ezhiye;	//ְҵ
		int	m_nx;	//X����
		int	m_ny;	//Y����
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����BOSS��� ID = 70
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum fubenboss_Fields {
		fubenboss_bossid = 0,	//bossid
		fubenboss_mapid = 1,	//������ͼid
		fubenboss_meishuidX = 2,	//����X
		fubenboss_meishuidY = 3,	//����Y
		fubenboss_tips = 4,	//tips
		fubenboss_buttonid = 5,	//��ťid
		fubenboss_Max = 6
	};
	// ��ϵ���ݽṹ
	struct fubenboss_t {
		int	m_nbossid;	//bossid
		int	m_nmapid;	//������ͼid
		int	m_nmeishuidX;	//����X
		int	m_nmeishuidY;	//����Y
		char	m_sztips[128];	//tips
		int	m_nbuttonid;	//��ťid
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ NPC�Զ�Ѱ·���� ID = 71
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum npczidongxunlufenlei_Fields {
		npczidongxunlufenlei_npcleixing = 0,	//NPC����
		npczidongxunlufenlei_npczhiye = 1,	//NPCְҵ
		npczidongxunlufenlei_Max = 2
	};
	// ������ (null) ö��ֵ����
	enum npczidongxunlufenlei_npcleixing_Enum {
		npczidongxunlufenlei_npcleixing_0,	//
		npczidongxunlufenlei_npcleixing_1,	//
		npczidongxunlufenlei_npcleixing_2,	//
		npczidongxunlufenlei_npcleixing_3,	//
		npczidongxunlufenlei_npcleixing_4,	//
		npczidongxunlufenlei_npcleixing_5,	//
		npczidongxunlufenlei_npcleixingMax = 6
	};
	// ������ (null) ö��ֵ����
	enum npczidongxunlufenlei_npczhiye_Enum {
		npczidongxunlufenlei_npczhiye_0,	//
		npczidongxunlufenlei_npczhiye_1,	//
		npczidongxunlufenlei_npczhiye_2,	//
		npczidongxunlufenlei_npczhiye_3,	//
		npczidongxunlufenlei_npczhiye_4,	//
		npczidongxunlufenlei_npczhiye_5,	//
		npczidongxunlufenlei_npczhiye_6,	//
		npczidongxunlufenlei_npczhiye_7,	//
		npczidongxunlufenlei_npczhiye_8,	//
		npczidongxunlufenlei_npczhiye_9,	//
		npczidongxunlufenlei_npczhiye_10,	//
		npczidongxunlufenlei_npczhiye_11,	//
		npczidongxunlufenlei_npczhiye_12,	//
		npczidongxunlufenlei_npczhiye_13,	//
		npczidongxunlufenlei_npczhiye_14,	//
		npczidongxunlufenlei_npczhiye_15,	//
		npczidongxunlufenlei_npczhiye_16,	//
		npczidongxunlufenlei_npczhiyeMax = 17
	};
	// ��ϵ���ݽṹ
	struct npczidongxunlufenlei_t {
		npczidongxunlufenlei_npcleixing_Enum	m_enpcleixing;	//NPC����
		npczidongxunlufenlei_npczhiye_Enum	m_enpczhiye;	//NPCְҵ
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ��ʯ�ϳ� ID = 72
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum baoshihecheng_Fields {
		baoshihecheng_id = 0,	//ID
		baoshihecheng_scid = 1,	//����ID
		baoshihecheng_xiaohaojinbi = 2,	//���Ľ��
		baoshihecheng_xiaohaohun = 3,	//���Ļ�
		baoshihecheng_Max = 4
	};
	// ��ϵ���ݽṹ
	struct baoshihecheng_t {
		int	m_nid;	//ID
		int	m_nscid;	//����ID
		int	m_nxiaohaojinbi;	//���Ľ��
		int	m_nxiaohaohun;	//���Ļ�
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ����Ʒ�� ID = 73
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petclass_Fields {
		petclass_id = 0,	//ID
		petclass_class = 1,	//Ʒ��
		petclass_gpoint = 2,	//�ɳ�ֵ
		petclass_huoyueduxishu = 3,	//��Ծ��Ʒ��ϵ��
		petclass_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum petclass_class_Enum {
		petclass_class_0,	//
		petclass_class_1,	//
		petclass_class_2,	//
		petclass_class_3,	//
		petclass_classMax = 4
	};
	// ��ϵ���ݽṹ
	struct petclass_t {
		int	m_nid;	//ID
		petclass_class_Enum	m_eclass;	//Ʒ��
		float	m_fgpoint;	//�ɳ�ֵ
		float	m_fhuoyueduxishu;	//��Ծ��Ʒ��ϵ��
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ϰ�Թ�ʵ ID = 74
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petfruit_Fields {
		petfruit_id = 0,	//ID
		petfruit_fruit = 1,	//ϰ�Թ�ʵ
		petfruit_habit = 2,	//ϰ��
		petfruit_pid = 3,	//����ID
		petfruit_Max = 4
	};
	// ��ϵ���ݽṹ
	struct petfruit_t {
		int	m_nid;	//ID
		char	m_szfruit[16];	//ϰ�Թ�ʵ
		char	m_szhabit[16];	//ϰ��
		int	m_npid;	//����ID
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ ������� ID = 75
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petdrop_Fields {
		petdrop_room = 0,	//��������
		petdrop_itemnum = 1,	//��Ʒ���
		petdrop_dropclass = 2,	//�������
		petdrop_dropprobablity = 3,	//�������
		petdrop_Max = 4
	};
	// ������ (null) ö��ֵ����
	enum petdrop_dropclass_Enum {
		petdrop_dropclass_0,	//
		petdrop_dropclassMax = 1
	};
	// ��ϵ���ݽṹ
	struct petdrop_t {
		int	m_nroom;	//��������
		int	m_nitemnum;	//��Ʒ���
		petdrop_dropclass_Enum	m_edropclass;	//�������
		int	m_ndropprobablity;	//�������
	};
	//////////////////////////////////////////////////////////////
	// ��ϵ �����ʵ�����ϵ�� ID = 76
	//////////////////////////////////////////////////////////////
	// ������ö��
	enum petfruitbagcontactlist_Fields {
		petfruitbagcontactlist_petbagid = 0,	//�������ID
		petfruitbagcontactlist_fruitclass = 1,	//��ʵƷ��
		petfruitbagcontactlist_fruitclassprobability = 2,	//��ʵƷ�ʸ���
		petfruitbagcontactlist_Max = 3
	};
	// ������ (null) ö��ֵ����
	enum petfruitbagcontactlist_fruitclass_Enum {
		petfruitbagcontactlist_fruitclass_0,	//
		petfruitbagcontactlist_fruitclass_1,	//
		petfruitbagcontactlist_fruitclass_2,	//
		petfruitbagcontactlist_fruitclassMax = 3
	};
	// ��ϵ���ݽṹ
	struct petfruitbagcontactlist_t {
		int	m_npetbagid;	//�������ID
		petfruitbagcontactlist_fruitclass_Enum	m_efruitclass;	//��ʵƷ��
		int	m_nfruitclassprobability;	//��ʵƷ�ʸ���
	};
}
#pragma pack()
// �������

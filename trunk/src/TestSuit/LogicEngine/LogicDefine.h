/////////////////////////////////////////////////////////////////////////////
// 此代码由逻辑编辑器生成,请不要手工编辑这部分代码
//	生成日期:2012-04-28 11:02:22
/////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(1)
namespace Logic
{
	// 单元ID枚举
	enum Unit_IDs {
		Unit_base = 1,	//基本单元
		Unit_guild = 11,	//公会
		Unit_charactor = 13,	//角色
		Unit_exp = 14,	//等级经验表
		Unit_set = 15,	//套装
		Unit_worldnum = 16,	//全局变量
		Unit_playertitle = 18,	//称号
		Unit_faceact = 21,	//表情动作
		Unit_currencyact = 22,	//通用动作
		Unit_item = 2,	//物品单元
		Unit_map = 9,	//地图
		Unit_mine = 10,	//矿场
		Unit_skill = 12,	//玩家技能
		Unit_brotherbuff = 24,	//结拜Buff
		Unit_petadd = 26,	//宠物属性
		Unit_petskill = 28,	//宠物特技
		Unit_equip = 5,	//装备
		Unit_prop = 6,	//道具
		Unit_gem = 7,	//宝石
		Unit_talisman = 8,	//法宝
		Unit_jinglian = 17,	//精炼
		Unit_firstnamefb = 19,	//法宝前缀
		Unit_lastnamefb = 20,	//法宝后缀
		Unit_pet = 25,	//宠物
		Unit_pethabit = 27,	//宠物习性
		Unit_dress = 23,	//时装
		Unit_player = 3,	//玩家
		Unit_NPC = 4,	//NPC
	};
	//////////////////////////////////////////////////////////////
	// 单元 基本单元 ID = 1
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum base_Fields {
		base_ID = 0,	//ID
		base_Name = 1,	//名称
		base_Des = 2,	//描述
		base_Max = 3
	};
	// 数据模板结构
	struct base_t {
		int	m_nID;	//ID
		char	m_szName[32];	//名称
		char	m_szDes[128];	//描述
	};
	// 数据实例结构
	struct base_i {
	};
#ifdef base_instance 
	int nbase_iOffset[3] = {
		-1,-1,-1	};
#endif //base_instance 
	//////////////////////////////////////////////////////////////
	// 单元 公会 ID = 11
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum guild_Fields {
		guild_ID = 0,	//ID
		guild_NAME = 1,	//名字
		guild_miaoshu = 2,	//描述
		guild_LV = 3,	//级别
		guild_NEXTLV = 4,	//下一级别
		guild_Max = 5
	};
	// 数据模板结构
	struct guild_t {
		int	m_nID;	//ID
		char	m_szNAME[32];	//名字
		char	m_szmiaoshu[128];	//描述
		int	m_nLV;	//级别
		int	m_nNEXTLV;	//下一级别
	};
	// 数据实例结构
	struct guild_i {
	};
#ifdef guild_instance 
	int nguild_iOffset[5] = {
		-1,-1,-1,-1,-1	};
#endif //guild_instance 
	//////////////////////////////////////////////////////////////
	// 单元 角色 ID = 13
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum charactor_Fields {
		charactor_leastdc = 0,	//最小物理攻击力
		charactor_maxdc = 1,	//最大物理攻击力
		charactor_ac = 2,	//物理防御力
		charactor_mac = 3,	//法术防御力
		charactor_crit = 4,	//物理重击率
		charactor_skillcrit = 5,	//技能重击率
		charactor_hitchance = 6,	//命中
		charactor_hedge = 7,	//躲闪
		charactor_hpreg = 8,	//生命恢复速度
		charactor_mpreg = 9,	//法力恢复速度
		charactor_attspeed = 10,	//攻击速度
		charactor_movespeed = 11,	//移动速度
		charactor_maxhp = 12,	//最大生命值
		charactor_maxmp = 13,	//最大法力值
		charactor_HP = 14,	//当前生命值
		charactor_MP = 15,	//当前法力值
		charactor_State = 16,	//当前状态
		charactor_silent = 17,	//沉默
		charactor_Chaotic = 18,	//混乱
		charactor_runspeed = 19,	//跑步速度
		charactor_level = 20,	//等级
		charactor_str = 21,	//力量
		charactor_agi = 22,	//敏捷
		charactor_phy = 23,	//灵力
		charactor_con = 24,	//体质
		charactor_int = 25,	//智慧
		charactor_skilleff = 26,	//法术伤害
		charactor_attdistance = 27,	//普通攻击范围
		charactor_dmgweek = 28,	//伤害减免
		charactor_dmgreturn = 29,	//伤害反弹
		charactor_double = 30,	//利刃几率
		charactor_crazy = 31,	//狂战士几率
		charactor_dizzy = 32,	//击晕几率
		charactor_EatMp = 33,	//嗜魔几率
		charactor_EatHp = 34,	//嗜血几率
		charactor_Mpreturn = 35,	//宁神几率
		charactor_qx = 36,	//潜行
		charactor_stand = 37,	//站立半径
		charactor_camouflage = 38,	//伪装
		charactor_sense = 39,	//侦测
		charactor_isolation = 40,	//隔绝
		charactor_immunity = 41,	//免疫
		charactor_predictmarker = 42,	//预言术标记
		charactor_manaburnmarker = 43,	//法力灼烧标记
		charactor_skillriseID = 44,	//技能复活ID
		charactor_skillrisepunish = 45,	//技能复活惩罚倍率
		charactor_skillriseHP = 46,	//技能复活HP
		charactor_skillriseMP = 47,	//技能复活MP
		charactor_propriseID = 48,	//道具复活ID
		charactor_proprisepunish = 49,	//道具复活惩罚倍率
		charactor_propriseHP = 50,	//道具复活HP
		charactor_propriseMP = 51,	//道具复活MP
		charactor_poseID = 52,	//伪装ID
		charactor_exppunish = 53,	//经验惩罚
		charactor_nomove = 54,	//定身
		charactor_zhsleastdcappend = 55,	//召唤兽最小物理攻击力附加
		charactor_zhsmaxdcappend = 56,	//召唤兽最大物理攻击力附加
		charactor_zhsacappend = 57,	//召唤兽物理防御力附加
		charactor_sunmmonsmarker = 58,	//召唤兽标记
		charactor_Max = 59
	};
	// 属性域 当前状态 枚举值定义
	enum charactor_State_Enum {
		charactor_State_0,	//站立
		charactor_State_1,	//行走
		charactor_State_2,	//跳跃
		charactor_State_3,	//攻击
		charactor_State_4,	//技能吟唱
		charactor_State_5,	//技能施放
		charactor_State_6,	//开箱子
		charactor_State_7,	//升旗
		charactor_State_8,	//表情动作
		charactor_State_9,	//被击
		charactor_State_10,	//被击飞
		charactor_State_11,	//站起
		charactor_State_12,	//死亡
		charactor_State_13,	//眩晕
		charactor_State_14,	//被冲撞
		charactor_State_15,	//召唤骑乘
		charactor_State_16,	//沉睡
		charactor_State_17,	//隔离
		charactor_StateMax = 18
	};
	// 数据模板结构
	struct charactor_t {
		float	m_fmovespeed;	//移动速度
		float	m_frunspeed;	//跑步速度
		int	m_nlevel;	//等级
	};
	// 数据实例结构
	struct charactor_i {
		int	m_nleastdc;	//最小物理攻击力
		int	m_nmaxdc;	//最大物理攻击力
		int	m_nac;	//物理防御力
		int	m_nmac;	//法术防御力
		int	m_ncrit;	//物理重击率
		int	m_nskillcrit;	//技能重击率
		int	m_nhitchance;	//命中
		int	m_nhedge;	//躲闪
		int	m_nhpreg;	//生命恢复速度
		int	m_nmpreg;	//法力恢复速度
		float	m_fattspeed;	//攻击速度
		float	m_fmovespeed;	//移动速度
		int	m_nmaxhp;	//最大生命值
		int	m_nmaxmp;	//最大法力值
		int	m_nHP;	//当前生命值
		int	m_nMP;	//当前法力值
		charactor_State_Enum	m_eState;	//当前状态
		BOOL	m_bsilent;	//沉默
		BOOL	m_bChaotic;	//混乱
		float	m_frunspeed;	//跑步速度
		int	m_nlevel;	//等级
		int	m_nstr;	//力量
		int	m_nagi;	//敏捷
		int	m_nphy;	//灵力
		int	m_ncon;	//体质
		int	m_nint;	//智慧
		int	m_nskilleff;	//法术伤害
		int	m_nattdistance;	//普通攻击范围
		int	m_ndmgweek;	//伤害减免
		int	m_ndmgreturn;	//伤害反弹
		int	m_ndouble;	//利刃几率
		int	m_ncrazy;	//狂战士几率
		int	m_ndizzy;	//击晕几率
		int	m_nEatMp;	//嗜魔几率
		int	m_nEatHp;	//嗜血几率
		int	m_nMpreturn;	//宁神几率
		int	m_nqx;	//潜行
		int	m_nstand;	//站立半径
		int	m_ncamouflage;	//伪装
		int	m_nsense;	//侦测
		BOOL	m_bisolation;	//隔绝
		BOOL	m_bimmunity;	//免疫
		int	m_npredictmarker;	//预言术标记
		int	m_nmanaburnmarker;	//法力灼烧标记
		int	m_nskillriseID;	//技能复活ID
		int	m_nskillrisepunish;	//技能复活惩罚倍率
		int	m_nskillriseHP;	//技能复活HP
		int	m_nskillriseMP;	//技能复活MP
		int	m_npropriseID;	//道具复活ID
		int	m_nproprisepunish;	//道具复活惩罚倍率
		int	m_npropriseHP;	//道具复活HP
		int	m_npropriseMP;	//道具复活MP
		int	m_nposeID;	//伪装ID
		int	m_nexppunish;	//经验惩罚
		int	m_nnomove;	//定身
		int	m_nzhsleastdcappend;	//召唤兽最小物理攻击力附加
		int	m_nzhsmaxdcappend;	//召唤兽最大物理攻击力附加
		int	m_nzhsacappend;	//召唤兽物理防御力附加
		int	m_nsunmmonsmarker;	//召唤兽标记
	};
#ifdef charactor_instance 
	int ncharactor_iOffset[59] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232	};
#endif //charactor_instance 
	//////////////////////////////////////////////////////////////
	// 单元 等级经验表 ID = 14
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum exp_Fields {
		exp_level = 0,	//等级
		exp_exp = 1,	//经验值
		exp_gongde = 2,	//功德
		exp_Max = 3
	};
	// 数据模板结构
	struct exp_t {
		int	m_nlevel;	//等级
		int	m_nexp;	//经验值
		int	m_ngongde;	//功德
	};
	// 数据实例结构
	struct exp_i {
	};
#ifdef exp_instance 
	int nexp_iOffset[3] = {
		-1,-1,-1	};
#endif //exp_instance 
	//////////////////////////////////////////////////////////////
	// 单元 套装 ID = 15
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum set_Fields {
		set_id = 0,	//ID
		set_name = 1,	//套装名
		set_Wq = 2,	//武器
		set_Yf = 3,	//衣服
		set_Kz = 4,	//裤子
		set_Tk = 5,	//头盔
		set_Jb = 6,	//肩膀
		set_Xz = 7,	//鞋子
		set_St = 8,	//手套
		set_Yd = 9,	//腰带
		set_Sw = 10,	//手腕
		set_PF = 11,	//披风
		set_JZ1 = 12,	//戒指1
		set_JZ2 = 13,	//戒指2
		set_XL = 14,	//项链
		set_activation1 = 15,	//激活数量1
		set_activation2 = 16,	//激活数量2
		set_activation3 = 17,	//激活数量3
		set_activation4 = 18,	//激活数量4
		set_effect1 = 19,	//效果说明1
		set_effect2 = 20,	//效果说明2
		set_effect3 = 21,	//效果说明3
		set_effect4 = 22,	//效果说明4
		set_Max = 23
	};
	// 数据模板结构
	struct set_t {
		int	m_nid;	//ID
		char	m_szname[16];	//套装名
		int	m_nWq;	//武器
		int	m_nYf;	//衣服
		int	m_nKz;	//裤子
		int	m_nTk;	//头盔
		int	m_nJb;	//肩膀
		int	m_nXz;	//鞋子
		int	m_nSt;	//手套
		int	m_nYd;	//腰带
		int	m_nSw;	//手腕
		int	m_nPF;	//披风
		int	m_nJZ1;	//戒指1
		int	m_nJZ2;	//戒指2
		int	m_nXL;	//项链
		int	m_nactivation1;	//激活数量1
		int	m_nactivation2;	//激活数量2
		int	m_nactivation3;	//激活数量3
		int	m_nactivation4;	//激活数量4
		char	m_szeffect1[128];	//效果说明1
		char	m_szeffect2[128];	//效果说明2
		char	m_szeffect3[128];	//效果说明3
		char	m_szeffect4[128];	//效果说明4
	};
	// 数据实例结构
	struct set_i {
	};
#ifdef set_instance 
	int nset_iOffset[23] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //set_instance 
	//////////////////////////////////////////////////////////////
	// 单元 全局变量 ID = 16
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum worldnum_Fields {
		worldnum_dkxh = 0,	//打孔消耗金钱
		worldnum_bluezq = 1,	//蓝装增强
		worldnum_greenzq = 2,	//绿装增强
		worldnum_zisezq = 3,	//紫装增强
		worldnum_pubcooldown = 4,	//公用冷却时间
		worldnum_mapid = 5,	//初生点地图
		worldnum_bornx = 6,	//初生点x
		worldnum_borny = 7,	//初生点y
		worldnum_dummydays = 8,	//虚拟时长
		worldnum_moneydroprate = 9,	//金钱掉落率
		worldnum_moneydroptimes = 10,	//金钱掉落倍率
		worldnum_itemdroprate = 11,	//物品掉落率
		worldnum_trumpdroprate = 12,	//法宝掉落率
		worldnum_boxchange = 13,	//箱子变更权限时间
		worldnum_boxlast = 14,	//箱子消失时间
		worldnum_alloctime = 15,	//分配时间
		worldnum_fbstr = 16,	//法宝力量
		worldnum_fbdex = 17,	//法宝敏捷
		worldnum_fbint = 18,	//法宝智慧
		worldnum_fbcon = 19,	//法宝体质
		worldnum_fbphy = 20,	//法宝体魄
		worldnum_fbmatt = 21,	//法宝法术效果
		worldnum_fblifere = 22,	//法宝生命恢复
		worldnum_fbmanare = 23,	//法宝法力恢复
		worldnum_fbskillcr = 24,	//法宝技能重击率
		worldnum_diaobaoxishu1 = 25,	//掉宝率系数1
		worldnum_diaobaoxishu2 = 26,	//掉宝率系数2
		worldnum_healthgametime = 27,	//健康游戏时间
		worldnum_faggametime = 28,	//疲劳游戏时间
		worldnum_healthspacetime = 29,	//健康提示间隔时间
		worldnum_fagspacetime = 30,	//疲劳提示间隔时间
		worldnum_unhealthspacetime = 31,	//非健康提示间隔时间
		worldnum_leveldropclique = 32,	//等级掉宝阀值
		worldnum_zuduifanwei = 33,	//组队范围
		worldnum_youzi = 34,	//邮资
		worldnum_GOLDshangxian = 35,	//背包和仓库金钱上限
		worldnum_jingliandroprate = 36,	//精炼卷轴掉落率
		worldnum_Gemdroprate = 37,	//宝石掉落率
		worldnum_daojudroprate = 38,	//道具掉落率
		worldnum_smithingdroprate = 39,	//锻造卷轴掉落率
		worldnum_protectLV = 40,	//新手保护级别
		worldnum_guardpatrolradius = 41,	//守卫巡逻半径
		worldnum_randomgreen = 42,	//赌博随机绿
		worldnum_randomblue = 43,	//赌博随机蓝
		worldnum_randompurple = 44,	//赌博随机紫
		worldnum_valueparameter = 45,	//赌博价格参数
		worldnum_pcrunspeed = 46,	//玩家跑步速度
		worldnum_pricewhite = 47,	//价格系数白
		worldnum_pricegreen = 48,	//价格系数绿
		worldnum_priceblue = 49,	//价格系数蓝
		worldnum_pricepurple = 50,	//价格系数紫
		worldnum_priceorange = 51,	//价格系数橙
		worldnum_soulshangxian = 52,	//魂上限
		worldnum_xysjsxglone = 53,	//稀有随机属性概率1
		worldnum_xysjsxgltow = 54,	//稀有随机属性概率2
		worldnum_infotimes = 55,	//提示信息存留时间
		worldnum_deadnom = 56,	//死亡经验惩罚普通
		worldnum_deadred = 57,	//死亡经验惩罚红名
		worldnum_deadpve = 58,	//死亡耐久惩罚PVE
		worldnum_deadpvp = 59,	//死亡耐久惩罚PVP
		worldnum_arearesettime = 60,	//区域重置时间
		worldnum_changetargetadds = 61,	//切换目标几率
		worldnum_tabdistance = 62,	//TAB选择距离
		worldnum_pugreen = 63,	//普绿
		worldnum_pubule = 64,	//普蓝
		worldnum_pupurple = 65,	//普紫
		worldnum_jingbule = 66,	//精蓝
		worldnum_jingpurple = 67,	//精紫
		worldnum_bossbule = 68,	//BOSS蓝
		worldnum_bosszpurple = 69,	//BOSS紫
		worldnum_greensoul = 70,	//绿魂倍率
		worldnum_bluesoul = 71,	//蓝魂倍率
		worldnum_purplesoul = 72,	//紫魂倍率
		worldnum_equipsale = 73,	//装备出售
		worldnum_equipup = 74,	//装备升级
		worldnum_equiprepair = 75,	//装备修理
		worldnum_lvzsjxs = 76,	//绿装随机项数
		worldnum_lzsjxs = 77,	//蓝装随机项数
		worldnum_zzsjxs = 78,	//紫装随机项数
		worldnum_czsjxs = 79,	//橙装随机项数
		worldnum_chatarea = 80,	//聊天范围
		worldnum_monestchangetarget = 81,	//怪物切换目标几率
		worldnum_bbmoveradii = 82,	//召唤兽活动半径
		worldnum_bbwaitradii = 83,	//召唤兽待机半径
		worldnum_bbstopfollowradii = 84,	//召唤兽停止跟随半径
		worldnum_targetholdtime = 85,	//目标保留时间
		worldnum_severbattletime = 86,	//脱离战斗时间
		worldnum_ksjysqgxxczsj = 87,	//矿石交易所求购信息存在时间
		worldnum_qxdzjsj = 88,	//亲密度增加时间
		worldnum_kcxxdrdc = 89,	//矿场信息淡入淡出
		worldnum_gwsysj = 90,	//归位瞬移时间
		worldnum_kcswsxjg = 91,	//矿场守卫刷新间隔
		worldnum_fbbcsj = 92,	//副本保存时间
		worldnum_fbdjs = 93,	//副本倒计时
		worldnum_monsterstandtime = 94,	//怪物站立时间
		worldnum_monstermovetime = 95,	//怪物走动时间
		worldnum_newplayerlevel = 96,	//新手结束等级
		worldnum_ptspeed = 97,	//普通坐骑上马速度
		worldnum_rmbspeed = 98,	//收费坐骑上马速度
		worldnum_shaguaigongde = 99,	//杀怪功德
		worldnum_baishidengji = 100,	//拜师等级
		worldnum_expsize = 101,	//经验倍率
		worldnum_zbsjcgbl = 102,	//装备升级成功倍率
		worldnum_dkcgbl = 103,	//打孔成功倍率
		worldnum_dlglbl = 104,	//掉落概率倍率
		worldnum_petdroprate = 105,	//宠物掉落概率
		worldnum_petpublue = 106,	//宠普蓝
		worldnum_petpupurple = 107,	//宠普紫
		worldnum_petjingbule = 108,	//宠精蓝
		worldnum_petjinggreen = 109,	//宠精紫
		worldnum_petbosspurple = 110,	//宠BOSS紫
		worldnum_petbossorange = 111,	//宠BOSS橙
		worldnum_petmoveradii = 112,	//宠物活动半径
		worldnum_petwaitradii = 113,	//宠物待机半径
		worldnum_petstopfollowradii = 114,	//宠物停止跟随半径
		worldnum_aixinzhizhuanhuanlv = 115,	//玩家爱心值转换率
		worldnum_chongwushengjixishu = 116,	//宠物升级系数
		worldnum_Max = 117
	};
	// 数据模板结构
	struct worldnum_t {
		int	m_ndkxh;	//打孔消耗金钱
		int	m_nbluezq;	//蓝装增强
		int	m_ngreenzq;	//绿装增强
		int	m_nzisezq;	//紫装增强
		int	m_npubcooldown;	//公用冷却时间
		int	m_nmapid;	//初生点地图
		int	m_nbornx;	//初生点x
		int	m_nborny;	//初生点y
		int	m_ndummydays;	//虚拟时长
		int	m_nmoneydroprate;	//金钱掉落率
		int	m_nmoneydroptimes;	//金钱掉落倍率
		int	m_nitemdroprate;	//物品掉落率
		int	m_ntrumpdroprate;	//法宝掉落率
		int	m_nboxchange;	//箱子变更权限时间
		int	m_nboxlast;	//箱子消失时间
		int	m_nalloctime;	//分配时间
		float	m_ffbstr;	//法宝力量
		float	m_ffbdex;	//法宝敏捷
		float	m_ffbint;	//法宝智慧
		float	m_ffbcon;	//法宝体质
		float	m_ffbphy;	//法宝体魄
		float	m_ffbmatt;	//法宝法术效果
		float	m_ffblifere;	//法宝生命恢复
		float	m_ffbmanare;	//法宝法力恢复
		float	m_ffbskillcr;	//法宝技能重击率
		float	m_fdiaobaoxishu1;	//掉宝率系数1
		float	m_fdiaobaoxishu2;	//掉宝率系数2
		int	m_nhealthgametime;	//健康游戏时间
		int	m_nfaggametime;	//疲劳游戏时间
		int	m_nhealthspacetime;	//健康提示间隔时间
		int	m_nfagspacetime;	//疲劳提示间隔时间
		int	m_nunhealthspacetime;	//非健康提示间隔时间
		int	m_nleveldropclique;	//等级掉宝阀值
		int	m_nzuduifanwei;	//组队范围
		int	m_nyouzi;	//邮资
		int	m_nGOLDshangxian;	//背包和仓库金钱上限
		int	m_njingliandroprate;	//精炼卷轴掉落率
		int	m_nGemdroprate;	//宝石掉落率
		int	m_ndaojudroprate;	//道具掉落率
		int	m_nsmithingdroprate;	//锻造卷轴掉落率
		int	m_nprotectLV;	//新手保护级别
		int	m_nguardpatrolradius;	//守卫巡逻半径
		int	m_nrandomgreen;	//赌博随机绿
		int	m_nrandomblue;	//赌博随机蓝
		int	m_nrandompurple;	//赌博随机紫
		int	m_nvalueparameter;	//赌博价格参数
		float	m_fpcrunspeed;	//玩家跑步速度
		int	m_npricewhite;	//价格系数白
		int	m_npricegreen;	//价格系数绿
		int	m_npriceblue;	//价格系数蓝
		int	m_npricepurple;	//价格系数紫
		int	m_npriceorange;	//价格系数橙
		int	m_nsoulshangxian;	//魂上限
		int	m_nxysjsxglone;	//稀有随机属性概率1
		int	m_nxysjsxgltow;	//稀有随机属性概率2
		int	m_ninfotimes;	//提示信息存留时间
		int	m_ndeadnom;	//死亡经验惩罚普通
		int	m_ndeadred;	//死亡经验惩罚红名
		int	m_ndeadpve;	//死亡耐久惩罚PVE
		int	m_ndeadpvp;	//死亡耐久惩罚PVP
		char	m_szarearesettime[4];	//区域重置时间
		char	m_szchangetargetadds[4];	//切换目标几率
		int	m_ntabdistance;	//TAB选择距离
		float	m_fpugreen;	//普绿
		float	m_fpubule;	//普蓝
		float	m_fpupurple;	//普紫
		float	m_fjingbule;	//精蓝
		float	m_fjingpurple;	//精紫
		float	m_fbossbule;	//BOSS蓝
		float	m_fbosszpurple;	//BOSS紫
		int	m_ngreensoul;	//绿魂倍率
		int	m_nbluesoul;	//蓝魂倍率
		int	m_npurplesoul;	//紫魂倍率
		float	m_fequipsale;	//装备出售
		float	m_fequipup;	//装备升级
		float	m_fequiprepair;	//装备修理
		int	m_nlvzsjxs;	//绿装随机项数
		int	m_nlzsjxs;	//蓝装随机项数
		int	m_nzzsjxs;	//紫装随机项数
		int	m_nczsjxs;	//橙装随机项数
		int	m_nchatarea;	//聊天范围
		int	m_nmonestchangetarget;	//怪物切换目标几率
		int	m_nbbmoveradii;	//召唤兽活动半径
		int	m_nbbwaitradii;	//召唤兽待机半径
		int	m_nbbstopfollowradii;	//召唤兽停止跟随半径
		int	m_ntargetholdtime;	//目标保留时间
		int	m_nseverbattletime;	//脱离战斗时间
		int	m_nksjysqgxxczsj;	//矿石交易所求购信息存在时间
		int	m_nqxdzjsj;	//亲密度增加时间
		int	m_nkcxxdrdc;	//矿场信息淡入淡出
		int	m_ngwsysj;	//归位瞬移时间
		int	m_nkcswsxjg;	//矿场守卫刷新间隔
		int	m_nfbbcsj;	//副本保存时间
		int	m_nfbdjs;	//副本倒计时
		int	m_nmonsterstandtime;	//怪物站立时间
		int	m_nmonstermovetime;	//怪物走动时间
		int	m_nnewplayerlevel;	//新手结束等级
		int	m_nptspeed;	//普通坐骑上马速度
		int	m_nrmbspeed;	//收费坐骑上马速度
		int	m_nshaguaigongde;	//杀怪功德
		int	m_nbaishidengji;	//拜师等级
		int	m_nexpsize;	//经验倍率
		int	m_nzbsjcgbl;	//装备升级成功倍率
		int	m_ndkcgbl;	//打孔成功倍率
		int	m_ndlglbl;	//掉落概率倍率
		int	m_npetdroprate;	//宠物掉落概率
		int	m_npetpublue;	//宠普蓝
		int	m_npetpupurple;	//宠普紫
		int	m_npetjingbule;	//宠精蓝
		int	m_npetjinggreen;	//宠精紫
		int	m_npetbosspurple;	//宠BOSS紫
		int	m_npetbossorange;	//宠BOSS橙
		int	m_npetmoveradii;	//宠物活动半径
		int	m_npetwaitradii;	//宠物待机半径
		int	m_npetstopfollowradii;	//宠物停止跟随半径
		int	m_naixinzhizhuanhuanlv;	//玩家爱心值转换率
		int	m_nchongwushengjixishu;	//宠物升级系数
	};
	// 数据实例结构
	struct worldnum_i {
	};
#ifdef worldnum_instance 
	int nworldnum_iOffset[117] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //worldnum_instance 
	//////////////////////////////////////////////////////////////
	// 单元 称号 ID = 18
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum playertitle_Fields {
		playertitle_ID = 0,	//ID
		playertitle_Name = 1,	//称号名称
		playertitle_Replace = 2,	//称号替换项
		playertitle_Color = 3,	//称号颜色
		playertitle_Explain = 4,	//称号描述
		playertitle_shifouyongjiu = 5,	//是否永久
		playertitle_chenghaolv = 6,	//称号等级
		playertitle_suoshubiaoqianye = 7,	//所属标签页
		playertitle_cfms = 8,	//触发描述
		playertitle_xgms = 9,	//效果描述
		playertitle_Max = 10
	};
	// 属性域 所属标签页 枚举值定义
	enum playertitle_suoshubiaoqianye_Enum {
		playertitle_suoshubiaoqianye_0,	//一般
		playertitle_suoshubiaoqianye_1,	//生活
		playertitle_suoshubiaoqianye_2,	//战斗
		playertitle_suoshubiaoqianye_3,	//隐藏
		playertitle_suoshubiaoqianyeMax = 4
	};
	// 数据模板结构
	struct playertitle_t {
		int	m_nID;	//ID
		char	m_szName[64];	//称号名称
		char	m_szColor[9];	//称号颜色
		char	m_szExplain[128];	//称号描述
		BOOL	m_bshifouyongjiu;	//是否永久
		int	m_nchenghaolv;	//称号等级
		playertitle_suoshubiaoqianye_Enum	m_esuoshubiaoqianye;	//所属标签页
		char	m_szcfms[128];	//触发描述
		char	m_szxgms[128];	//效果描述
	};
	// 数据实例结构
	struct playertitle_i {
		char	m_szReplace[32];	//称号替换项
	};
#ifdef playertitle_instance 
	int nplayertitle_iOffset[10] = {
		-1,-1,0,-1,-1,-1,-1,-1,-1,-1	};
#endif //playertitle_instance 
	//////////////////////////////////////////////////////////////
	// 单元 表情动作 ID = 21
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum faceact_Fields {
		faceact_idname = 0,	//ID
		faceact_name = 1,	//名称
		faceact_depict = 2,	//描述
		faceact_Max = 3
	};
	// 数据模板结构
	struct faceact_t {
		int	m_nidname;	//ID
		char	m_szname[16];	//名称
		char	m_szdepict[128];	//描述
	};
	// 数据实例结构
	struct faceact_i {
	};
#ifdef faceact_instance 
	int nfaceact_iOffset[3] = {
		-1,-1,-1	};
#endif //faceact_instance 
	//////////////////////////////////////////////////////////////
	// 单元 通用动作 ID = 22
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum currencyact_Fields {
		currencyact_idname = 0,	//ID
		currencyact_name = 1,	//名称
		currencyact_depict = 2,	//描述
		currencyact_Max = 3
	};
	// 数据模板结构
	struct currencyact_t {
		int	m_nidname;	//ID
		char	m_szname[16];	//名称
		char	m_szdepict[128];	//描述
	};
	// 数据实例结构
	struct currencyact_i {
	};
#ifdef currencyact_instance 
	int ncurrencyact_iOffset[3] = {
		-1,-1,-1	};
#endif //currencyact_instance 
	//////////////////////////////////////////////////////////////
	// 单元 物品单元 ID = 2
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum item_Fields {
		item_Buy = 3,	//购买
		item_Sell = 4,	//出售
		item_Change = 5,	//交易
		item_Destory = 6,	//销毁
		item_Overlap = 7,	//叠加
		item_Time = 8,	//拥有时间
		item_Moneytype = 9,	//人民币道具
		item_Buyprice = 10,	//购买价
		item_Sellprice = 11,	//出售价
		item_BandType = 12,	//绑定类型
		item_BandFlag = 13,	//绑定标记
		item_DeleFlag = 14,	//删除标记
		item_Max = 15
	};
	// 属性域 人民币道具 枚举值定义
	enum item_Moneytype_Enum {
		item_Moneytype_0,	//普通
		item_Moneytype_1,	//人民币
		item_Moneytype_2,	//积分
		item_Moneytype_3,	//功德
		item_MoneytypeMax = 4
	};
	// 属性域 绑定类型 枚举值定义
	enum item_BandType_Enum {
		item_BandType_0,	//不能绑定
		item_BandType_1,	//手动绑定
		item_BandType_2,	//使用绑定
		item_BandTypeMax = 3
	};
	// 数据模板结构
	struct item_t : public base_t {
		BOOL	m_bBuy;	//购买
		BOOL	m_bSell;	//出售
		BOOL	m_bChange;	//交易
		BOOL	m_bDestory;	//销毁
		int	m_nOverlap;	//叠加
		int	m_nTime;	//拥有时间
		item_Moneytype_Enum	m_eMoneytype;	//人民币道具
		int	m_nBuyprice;	//购买价
		int	m_nSellprice;	//出售价
		item_BandType_Enum	m_eBandType;	//绑定类型
	};
	// 数据实例结构
	struct item_i : public base_i {
		int	m_nTime;	//拥有时间
		int	m_nBuyprice;	//购买价
		int	m_nSellprice;	//出售价
		BOOL	m_bBandFlag;	//绑定标记
		BOOL	m_bDeleFlag;	//删除标记
	};
#ifdef item_instance 
	int nitem_iOffset[15] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16	};
#endif //item_instance 
	//////////////////////////////////////////////////////////////
	// 单元 地图 ID = 9
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum map_Fields {
		map_file = 3,	//文件地址
		map_mappath = 4,	//小地图路径
		map_width = 5,	//小地图宽
		map_height = 6,	//小地图高
		map_cell = 7,	//小地图cell
		map_type = 8,	//地图类型
		map_serverID = 9,	//所属服务器
		map_jinrucishu = 10,	//副本每日进入次数限制
		map_Max = 11
	};
	// 属性域 地图类型 枚举值定义
	enum map_type_Enum {
		map_type_0,	//普通
		map_type_1,	//副本
		map_type_2,	//未开放
		map_typeMax = 3
	};
	// 数据模板结构
	struct map_t : public base_t {
		char	m_szfile[128];	//文件地址
		char	m_szmappath[128];	//小地图路径
		int	m_nwidth;	//小地图宽
		int	m_nheight;	//小地图高
		int	m_ncell;	//小地图cell
		map_type_Enum	m_etype;	//地图类型
		int	m_njinrucishu;	//副本每日进入次数限制
	};
	// 数据实例结构
	struct map_i : public base_i {
		int	m_nserverID;	//所属服务器
	};
#ifdef map_instance 
	int nmap_iOffset[11] = {
		-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1	};
#endif //map_instance 
	//////////////////////////////////////////////////////////////
	// 单元 矿场 ID = 10
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum mine_Fields {
		mine_chanpin = 3,	//产品
		mine_chanliang = 4,	//产量
		mine_level = 5,	//级别
		mine_nextid = 6,	//下一级ID
		mine_kuangcname = 7,	//矿场名称
		mine_MapID = 8,	//地图ID
		mine_areaID = 9,	//区域编号
		mine_X = 10,	//x坐标
		mine_Y = 11,	//y坐标
		mine_W = 12,	//宽度
		mine_H = 13,	//高度
		mine_GuildID = 14,	//所属公会
		mine_CountDown = 15,	//产出倒计时
		mine_NpcCnt = 16,	//当前看守NPC数量
		mine_MaxNpc = 17,	//看守NPC数量上限
		mine_LVUPGOLD = 18,	//升级金钱
		mine_QX = 19,	//旗帜X坐标
		mine_QY = 20,	//旗帜Y坐标
		mine_Max = 21
	};
	// 数据模板结构
	struct mine_t : public base_t {
		int	m_nchanpin;	//产品
		int	m_nchanliang;	//产量
		int	m_nlevel;	//级别
		int	m_nnextid;	//下一级ID
		char	m_szkuangcname[16];	//矿场名称
		int	m_nMapID;	//地图ID
		int	m_nareaID;	//区域编号
		int	m_nX;	//x坐标
		int	m_nY;	//y坐标
		int	m_nW;	//宽度
		int	m_nH;	//高度
		int	m_nGuildID;	//所属公会
		int	m_nNpcCnt;	//当前看守NPC数量
		int	m_nMaxNpc;	//看守NPC数量上限
		int	m_nLVUPGOLD;	//升级金钱
		int	m_nQX;	//旗帜X坐标
		int	m_nQY;	//旗帜Y坐标
	};
	// 数据实例结构
	struct mine_i : public base_i {
		int	m_nchanliang;	//产量
		int	m_nGuildID;	//所属公会
		int	m_nCountDown;	//产出倒计时
		int	m_nNpcCnt;	//当前看守NPC数量
	};
#ifdef mine_instance 
	int nmine_iOffset[21] = {
		-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,8,12,-1,-1,-1,-1	};
#endif //mine_instance 
	//////////////////////////////////////////////////////////////
	// 单元 玩家技能 ID = 12
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum skill_Fields {
		skill_skilltarget = 3,	//技能目标
		skill_skilltype = 4,	//技能类型
		skill_skillcrit = 5,	//是否重击
		skill_singtime = 6,	//吟唱时间
		skill_cooldown = 7,	//冷却时间
		skill_freetime = 8,	//释放时间
		skill_freedistance = 9,	//释放距离
		skill_fly = 10,	//是否飞行
		skill_exp = 11,	//技能学习经验
		skill_skillbz = 12,	//技能标志
		skill_skillcs = 13,	//从属技能
		skill_skillcast = 14,	//技能消耗
		skill_group = 15,	//组标记
		skill_dztx = 16,	//动作特效
		skill_castway = 17,	//触发方式
		skill_lazztime = 18,	//延迟施放
		skill_castrange = 19,	//触发半径
		skill_levelask = 20,	//要求等级
		skill_arealasttime = 21,	//区域维持时间
		skill_arealastpre = 22,	//区域维持频率
		skill_repeatreleasetime = 23,	//重复释放时间
		skill_autoatk = 24,	//自动攻击
		skill_skillinterval = 25,	//技能间隔
		skill_noact = 26,	//无动作
		skill_Max = 27
	};
	// 属性域 技能目标 枚举值定义
	enum skill_skilltarget_Enum {
		skill_skilltarget_0,	//自身
		skill_skilltarget_1,	//其他个体
		skill_skilltarget_2,	//群体
		skill_skilltarget_3,	//区域
		skill_skilltarget_4,	//陷阱
		skill_skilltargetMax = 5
	};
	// 属性域 技能类型 枚举值定义
	enum skill_skilltype_Enum {
		skill_skilltype_0,	//主动
		skill_skilltype_1,	//被动
		skill_skilltypeMax = 2
	};
	// 属性域 技能标志 枚举值定义
	enum skill_skillbz_Enum {
		skill_skillbz_0,	//战斗技能
		skill_skillbz_1,	//法宝技能
		skill_skillbz_2,	//公会技能
		skill_skillbz_3,	//怪物技能
		skill_skillbz_4,	//宠物技能
		skill_skillbzMax = 5
	};
	// 属性域 触发方式 枚举值定义
	enum skill_castway_Enum {
		skill_castway_0,	//时间触发
		skill_castway_1,	//空间触发
		skill_castway_2,	//双重触发
		skill_castwayMax = 3
	};
	// 属性域 自动攻击 枚举值定义
	enum skill_autoatk_Enum {
		skill_autoatk_0,	//无影响
		skill_autoatk_1,	//接续
		skill_autoatk_2,	//打断
		skill_autoatkMax = 3
	};
	// 数据模板结构
	struct skill_t : public base_t {
		skill_skilltarget_Enum	m_eskilltarget;	//技能目标
		skill_skilltype_Enum	m_eskilltype;	//技能类型
		BOOL	m_bskillcrit;	//是否重击
		int	m_nsingtime;	//吟唱时间
		int	m_ncooldown;	//冷却时间
		int	m_nfreetime;	//释放时间
		int	m_nfreedistance;	//释放距离
		BOOL	m_bfly;	//是否飞行
		int	m_nexp;	//技能学习经验
		skill_skillbz_Enum	m_eskillbz;	//技能标志
		int	m_nskillcs;	//从属技能
		int	m_nskillcast;	//技能消耗
		int	m_ngroup;	//组标记
		int	m_ndztx;	//动作特效
		skill_castway_Enum	m_ecastway;	//触发方式
		int	m_nlazztime;	//延迟施放
		int	m_ncastrange;	//触发半径
		int	m_nlevelask;	//要求等级
		int	m_narealasttime;	//区域维持时间
		int	m_narealastpre;	//区域维持频率
		int	m_nrepeatreleasetime;	//重复释放时间
		skill_autoatk_Enum	m_eautoatk;	//自动攻击
		int	m_nskillinterval;	//技能间隔
		BOOL	m_bnoact;	//无动作
	};
	// 数据实例结构
	struct skill_i : public base_i {
		skill_skilltarget_Enum	m_eskilltarget;	//技能目标
		skill_skilltype_Enum	m_eskilltype;	//技能类型
		BOOL	m_bskillcrit;	//是否重击
		int	m_nsingtime;	//吟唱时间
		int	m_ncooldown;	//冷却时间
		int	m_nfreetime;	//释放时间
		int	m_nfreedistance;	//释放距离
		BOOL	m_bfly;	//是否飞行
		int	m_nexp;	//技能学习经验
		skill_skillbz_Enum	m_eskillbz;	//技能标志
		int	m_nskillcast;	//技能消耗
	};
#ifdef skill_instance 
	int nskill_iOffset[27] = {
		-1,-1,-1,0,4,8,12,16,20,24,28,32,36,-1,40,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //skill_instance 
	//////////////////////////////////////////////////////////////
	// 单元 结拜Buff ID = 24
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum brotherbuff_Fields {
		brotherbuff_Max = 3
	};
	// 数据模板结构
	struct brotherbuff_t : public base_t {
	};
	// 数据实例结构
	struct brotherbuff_i : public base_i {
	};
#ifdef brotherbuff_instance 
	int nbrotherbuff_iOffset[3] = {
		-1,-1,-1	};
#endif //brotherbuff_instance 
	//////////////////////////////////////////////////////////////
	// 单元 宠物属性 ID = 26
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petadd_Fields {
		petadd_Max = 3
	};
	// 数据模板结构
	struct petadd_t : public base_t {
	};
	// 数据实例结构
	struct petadd_i : public base_i {
	};
#ifdef petadd_instance 
	int npetadd_iOffset[3] = {
		-1,-1,-1	};
#endif //petadd_instance 
	//////////////////////////////////////////////////////////////
	// 单元 宠物特技 ID = 28
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petskill_Fields {
		petskill_skilltarget = 3,	//技能目标
		petskill_cooldowntime = 4,	//冷却时间
		petskill_casttype = 5,	//触发类型
		petskill_castprobility = 6,	//触发几率
		petskill_freedistance = 7,	//释放距离
		petskill_Max = 8
	};
	// 属性域 技能目标 枚举值定义
	enum petskill_skilltarget_Enum {
		petskill_skilltarget_0,	//自身
		petskill_skilltarget_1,	//群体
		petskill_skilltarget_2,	//个体目标
		petskill_skilltarget_3,	//区域
		petskill_skilltargetMax = 4
	};
	// 属性域 触发类型 枚举值定义
	enum petskill_casttype_Enum {
		petskill_casttype_0,	//角色物理攻击
		petskill_casttype_1,	//角色技能攻击
		petskill_casttype_2,	//角色受到物理和技能攻击
		petskill_casttypeMax = 3
	};
	// 数据模板结构
	struct petskill_t : public base_t {
		petskill_skilltarget_Enum	m_eskilltarget;	//技能目标
		int	m_ncooldowntime;	//冷却时间
		petskill_casttype_Enum	m_ecasttype;	//触发类型
		int	m_ncastprobility;	//触发几率
		int	m_nfreedistance;	//释放距离
	};
	// 数据实例结构
	struct petskill_i : public base_i {
		int	m_ncooldowntime;	//冷却时间
		petskill_casttype_Enum	m_ecasttype;	//触发类型
		int	m_ncastprobility;	//触发几率
		int	m_nfreedistance;	//释放距离
	};
#ifdef petskill_instance 
	int npetskill_iOffset[8] = {
		-1,-1,-1,-1,0,4,8,12	};
#endif //petskill_instance 
	//////////////////////////////////////////////////////////////
	// 单元 装备 ID = 5
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum equip_Fields {
		equip_Durabilit = 15,	//当前耐久度
		equip_class = 16,	//品质
		equip_level = 17,	//等级
		equip_maxdurabilit = 18,	//最大耐久度
		equip_socket = 19,	//镶嵌孔
		equip_bangding = 20,	//是否绑定
		equip_body = 21,	//装备部位
		equip_moneyup = 22,	//升级金钱
		equip_soulup = 23,	//升级魂
		equip_job = 24,	//使用职业
		equip_setnum = 25,	//套装编号
		equip_modnum = 26,	//使用模型
		equip_gem1 = 27,	//宝石孔1
		equip_gem2 = 28,	//宝石孔2
		equip_gem3 = 29,	//宝石孔3
		equip_gem4 = 30,	//宝石孔4
		equip_upchance = 31,	//升级成功率
		equip_socketchan = 32,	//打孔成功率
		equip_finesign = 33,	//精炼标记
		equip_equiplv = 34,	//物品等级
		equip_xzcsone = 35,	//修正参数1
		equip_xzcstwo = 36,	//修正参数2
		equip_posterid = 37,	//海报ID
		equip_xysjone = 38,	//稀有随机1
		equip_xysjtow = 39,	//稀有随机2
		equip_sjxszl = 40,	//随机项数增量
		equip_stilettoprice = 41,	//打孔价格
		equip_sfbxyfjsjxs = 42,	//是否不需要附加随机属性
		equip_Max = 43
	};
	// 属性域 品质 枚举值定义
	enum equip_class_Enum {
		equip_class_0,	//普通
		equip_class_1,	//精致
		equip_class_2,	//优秀
		equip_class_3,	//传奇
		equip_class_4,	//神话
		equip_class_5,	//逆天
		equip_class_6,	//逆天-2
		equip_class_7,	//逆天-3
		equip_class_8,	//随机
		equip_classMax = 9
	};
	// 属性域 装备部位 枚举值定义
	enum equip_body_Enum {
		equip_body_0,	//头盔
		equip_body_1,	//护肩
		equip_body_2,	//披风
		equip_body_3,	//衣服
		equip_body_4,	//腰带
		equip_body_5,	//护腿
		equip_body_6,	//手套
		equip_body_7,	//护腕
		equip_body_8,	//鞋子
		equip_body_9,	//武器
		equip_body_10,	//项链
		equip_body_11,	//戒指
		equip_body_12,	//时装
		equip_bodyMax = 13
	};
	// 属性域 使用职业 枚举值定义
	enum equip_job_Enum {
		equip_job_0,	//武将
		equip_job_1,	//刺客
		equip_job_2,	//术士
		equip_job_3,	//乐师
		equip_job_4,	//全职业
		equip_jobMax = 5
	};
	// 数据模板结构
	struct equip_t : public item_t {
		equip_class_Enum	m_eclass;	//品质
		int	m_nlevel;	//等级
		int	m_nmaxdurabilit;	//最大耐久度
		equip_body_Enum	m_ebody;	//装备部位
		int	m_nmoneyup;	//升级金钱
		int	m_nsoulup;	//升级魂
		equip_job_Enum	m_ejob;	//使用职业
		int	m_nsetnum;	//套装编号
		int	m_nmodnum;	//使用模型
		int	m_nupchance;	//升级成功率
		int	m_nsocketchan;	//打孔成功率
		int	m_nequiplv;	//物品等级
		float	m_fxzcsone;	//修正参数1
		float	m_fxzcstwo;	//修正参数2
		int	m_nposterid;	//海报ID
		int	m_nstilettoprice;	//打孔价格
		BOOL	m_bsfbxyfjsjxs;	//是否不需要附加随机属性
	};
	// 数据实例结构
	struct equip_i : public item_i {
		int	m_nDurabilit;	//当前耐久度
		equip_class_Enum	m_eclass;	//品质
		int	m_nlevel;	//等级
		int	m_nmaxdurabilit;	//最大耐久度
		int	m_nsocket;	//镶嵌孔
		BOOL	m_bbangding;	//是否绑定
		equip_body_Enum	m_ebody;	//装备部位
		int	m_nmoneyup;	//升级金钱
		int	m_nsoulup;	//升级魂
		int	m_ngem1;	//宝石孔1
		int	m_ngem2;	//宝石孔2
		int	m_ngem3;	//宝石孔3
		int	m_ngem4;	//宝石孔4
		int	m_nupchance;	//升级成功率
		int	m_nsocketchan;	//打孔成功率
		int	m_nfinesign;	//精炼标记
		int	m_nxysjone;	//稀有随机1
		int	m_nxysjtow;	//稀有随机2
		int	m_nsjxszl;	//随机项数增量
		int	m_nstilettoprice;	//打孔价格
	};
#ifdef equip_instance 
	int nequip_iOffset[43] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,44,48,52,-1,-1,-1,56,60,64,68,72,76,80,-1,-1,-1,-1,84,88,92,96,-1	};
#endif //equip_instance 
	//////////////////////////////////////////////////////////////
	// 单元 道具 ID = 6
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum prop_Fields {
		prop_colddown = 15,	//冷却时间
		prop_group = 16,	//组标记
		prop_type = 17,	//类型
		prop_class = 18,	//品质
		prop_icon = 19,	//图标
		prop_useable = 20,	//可使用物品
		prop_onlyeffects = 21,	//唯一物品
		prop_nodisappear = 22,	//不消失
		prop_posterid = 23,	//海报ID
		prop_act = 24,	//是否触发动作
		prop_Max = 25
	};
	// 属性域 类型 枚举值定义
	enum prop_type_Enum {
		prop_type_0,	//药品
		prop_type_1,	//矿石
		prop_type_2,	//锻造图纸
		prop_type_3,	//材料
		prop_type_4,	//任务物品
		prop_type_5,	//幸运符咒
		prop_type_6,	//经验卷轴
		prop_type_7,	//回城卷轴
		prop_type_8,	//坐骑
		prop_type_9,	//杂货
		prop_type_10,	//精炼卷轴
		prop_type_11,	//矿场守卫卷轴
		prop_type_12,	//矿场增产符
		prop_type_13,	//复活卷轴
		prop_type_14,	//背包扩展符
		prop_type_15,	//仓库扩展符
		prop_type_16,	//寻宝符咒
		prop_type_17,	//打孔符咒
		prop_type_18,	//礼包
		prop_type_19,	//替身娃娃
		prop_type_20,	//千里传音
		prop_type_21,	//烟花
		prop_type_22,	//表情喇叭
		prop_type_23,	//忘情水
		prop_type_24,	//精炼石
		prop_type_25,	//矿场侦测卷轴
		prop_type_26,	//器灵
		prop_type_27,	//器灵残魂
		prop_type_28,	//器灵符
		prop_type_29,	//保底符
		prop_type_30,	//法宝炼化符
		prop_type_31,	//任务刷新符
		prop_type_32,	//绑定符
		prop_type_33,	//副本进入卷轴
		prop_type_34,	//习性果实
		prop_type_35,	//宠物道具
		prop_type_36,	//宠物果实礼包
		prop_type_37,	//爱心储蓄罐
		prop_typeMax = 38
	};
	// 属性域 品质 枚举值定义
	enum prop_class_Enum {
		prop_class_0,	//普通
		prop_class_1,	//精致
		prop_class_2,	//优秀
		prop_class_3,	//传奇
		prop_class_4,	//神话
		prop_class_5,	//逆天
		prop_classMax = 6
	};
	// 数据模板结构
	struct prop_t : public item_t {
		int	m_ncolddown;	//冷却时间
		int	m_ngroup;	//组标记
		prop_type_Enum	m_etype;	//类型
		prop_class_Enum	m_eclass;	//品质
		int	m_nicon;	//图标
		BOOL	m_buseable;	//可使用物品
		BOOL	m_bonlyeffects;	//唯一物品
		BOOL	m_bnodisappear;	//不消失
		int	m_nposterid;	//海报ID
		BOOL	m_bact;	//是否触发动作
	};
	// 数据实例结构
	struct prop_i : public item_i {
		int	m_ncolddown;	//冷却时间
	};
#ifdef prop_instance 
	int nprop_iOffset[25] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //prop_instance 
	//////////////////////////////////////////////////////////////
	// 单元 宝石 ID = 7
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gem_Fields {
		gem_class = 15,	//品质
		gem_icon = 16,	//图标
		gem_posterid = 17,	//海报ID
		gem_huiguangdangci = 18,	//辉光档次
		gem_Max = 19
	};
	// 属性域 品质 枚举值定义
	enum gem_class_Enum {
		gem_class_0,	//普通
		gem_class_1,	//精致
		gem_class_2,	//优秀
		gem_class_3,	//传奇
		gem_class_4,	//神话
		gem_class_5,	//逆天
		gem_classMax = 6
	};
	// 数据模板结构
	struct gem_t : public item_t {
		gem_class_Enum	m_eclass;	//品质
		int	m_nicon;	//图标
		int	m_nposterid;	//海报ID
		int	m_nhuiguangdangci;	//辉光档次
	};
	// 数据实例结构
	struct gem_i : public item_i {
	};
#ifdef gem_instance 
	int ngem_iOffset[19] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,-1,-1	};
#endif //gem_instance 
	//////////////////////////////////////////////////////////////
	// 单元 法宝 ID = 8
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum talisman_Fields {
		talisman_Prefix1 = 15,	//法宝前缀1
		talisman_Prefix2 = 16,	//法宝前缀2
		talisman_Postfix = 17,	//法宝后缀
		talisman_Class = 18,	//法宝种类
		talisman_Level = 19,	//炼化次数
		talisman_class = 20,	//品质
		talisman_soulup = 21,	//升级魂
		talisman_Max = 22
	};
	// 属性域 法宝前缀1 枚举值定义
	enum talisman_Prefix1_Enum {
		talisman_Prefix1_0,	//九天
		talisman_Prefix1_1,	//八卦
		talisman_Prefix1_2,	//七星
		talisman_Prefix1_3,	//六洞
		talisman_Prefix1_4,	//五行
		talisman_Prefix1_5,	//四象
		talisman_Prefix1_6,	//三山
		talisman_Prefix1_7,	//两仪
		talisman_Prefix1_8,	//混元
		talisman_Prefix1Max = 9
	};
	// 属性域 法宝前缀2 枚举值定义
	enum talisman_Prefix2_Enum {
		talisman_Prefix2_0,	//九天
		talisman_Prefix2_1,	//八卦
		talisman_Prefix2_2,	//七星
		talisman_Prefix2_3,	//六洞
		talisman_Prefix2_4,	//五行
		talisman_Prefix2_5,	//四象
		talisman_Prefix2_6,	//三山
		talisman_Prefix2_7,	//两仪
		talisman_Prefix2_8,	//混元
		talisman_Prefix2Max = 9
	};
	// 属性域 法宝后缀 枚举值定义
	enum talisman_Postfix_Enum {
		talisman_Postfix_0,	//修罗
		talisman_Postfix_1,	//伏魔
		talisman_Postfix_2,	//裂地
		talisman_Postfix_3,	//通灵
		talisman_Postfix_4,	//无尘
		talisman_Postfix_5,	//乾坤
		talisman_PostfixMax = 6
	};
	// 属性域 法宝种类 枚举值定义
	enum talisman_Class_Enum {
		talisman_Class_0,	//剑
		talisman_Class_1,	//丹
		talisman_Class_2,	//铃
		talisman_Class_3,	//印
		talisman_Class_4,	//扇
		talisman_Class_5,	//灯
		talisman_Class_6,	//幡
		talisman_ClassMax = 7
	};
	// 属性域 品质 枚举值定义
	enum talisman_class_Enum {
		talisman_class_0,	//普通
		talisman_classMax = 1
	};
	// 数据模板结构
	struct talisman_t : public item_t {
		int	m_nLevel;	//炼化次数
		talisman_class_Enum	m_eclass;	//品质
		int	m_nsoulup;	//升级魂
	};
	// 数据实例结构
	struct talisman_i : public item_i {
		talisman_Prefix1_Enum	m_ePrefix1;	//法宝前缀1
		talisman_Prefix2_Enum	m_ePrefix2;	//法宝前缀2
		talisman_Postfix_Enum	m_ePostfix;	//法宝后缀
		talisman_Class_Enum	m_eClass;	//法宝种类
		int	m_nLevel;	//炼化次数
		talisman_class_Enum	m_eclass;	//品质
	};
#ifdef talisman_instance 
	int ntalisman_iOffset[22] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,-1	};
#endif //talisman_instance 
	//////////////////////////////////////////////////////////////
	// 单元 精炼 ID = 17
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum jinglian_Fields {
		jinglian_butian = 15,	//补天需求
		jinglian_jinggang = 16,	//玉石需求
		jinglian_jin = 17,	//金需求
		jinglian_yin = 18,	//银需求
		jinglian_tong = 19,	//铜需求
		jinglian_tie = 20,	//铁需求
		jinglian_buwei = 21,	//部位
		jinglian_level = 22,	//等级参考
		jinglian_huiguangyanse = 23,	//辉光颜色
		jinglian_Max = 24
	};
	// 属性域 部位 枚举值定义
	enum jinglian_buwei_Enum {
		jinglian_buwei_0,	//武器
		jinglian_buwei_1,	//上半身
		jinglian_buwei_2,	//下半身
		jinglian_buwei_3,	//首饰
		jinglian_buweiMax = 4
	};
	// 数据模板结构
	struct jinglian_t : public item_t {
		int	m_nbutian;	//补天需求
		int	m_njinggang;	//玉石需求
		int	m_njin;	//金需求
		int	m_nyin;	//银需求
		int	m_ntong;	//铜需求
		int	m_ntie;	//铁需求
		jinglian_buwei_Enum	m_ebuwei;	//部位
		int	m_nlevel;	//等级参考
		int	m_nhuiguangyanse;	//辉光颜色
	};
	// 数据实例结构
	struct jinglian_i : public item_i {
	};
#ifdef jinglian_instance 
	int njinglian_iOffset[24] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,-1,-1,-1,-1,-1,-1,-1	};
#endif //jinglian_instance 
	//////////////////////////////////////////////////////////////
	// 单元 法宝前缀 ID = 19
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum firstnamefb_Fields {
		firstnamefb_chuxianjilv = 15,	//出现几率
		firstnamefb_color = 16,	//颜色
		firstnamefb_Max = 17
	};
	// 数据模板结构
	struct firstnamefb_t : public item_t {
		int	m_nchuxianjilv;	//出现几率
		char	m_szcolor[9];	//颜色
	};
	// 数据实例结构
	struct firstnamefb_i : public item_i {
	};
#ifdef firstnamefb_instance 
	int nfirstnamefb_iOffset[17] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1	};
#endif //firstnamefb_instance 
	//////////////////////////////////////////////////////////////
	// 单元 法宝后缀 ID = 20
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum lastnamefb_Fields {
		lastnamefb_Max = 15
	};
	// 数据模板结构
	struct lastnamefb_t : public item_t {
	};
	// 数据实例结构
	struct lastnamefb_i : public item_i {
	};
#ifdef lastnamefb_instance 
	int nlastnamefb_iOffset[15] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16	};
#endif //lastnamefb_instance 
	//////////////////////////////////////////////////////////////
	// 单元 宠物 ID = 25
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum pet_Fields {
		pet_icon = 15,	//图标
		pet_modnum = 16,	//使用模型
		pet_class = 17,	//品质
		pet_level = 18,	//等级
		pet_exp = 19,	//经验值
		pet_active = 20,	//活跃度
		pet_activespd = 21,	//活跃度消耗速度
		pet_str = 22,	//力量
		pet_agi = 23,	//敏捷
		pet_con = 24,	//耐力
		pet_int = 25,	//智力
		pet_phy = 26,	//灵力
		pet_habit = 27,	//习性
		pet_pointplus = 28,	//附加属性
		pet_fruit1 = 29,	//果实1
		pet_fruit2 = 30,	//果实2
		pet_fruit3 = 31,	//果实3
		pet_fruit4 = 32,	//果实4
		pet_skill = 33,	//特技
		pet_strpplus = 34,	//力量附加
		pet_agipplus = 35,	//敏捷附加
		pet_conpplus = 36,	//耐力附加
		pet_intpplus = 37,	//智力附加
		pet_phypplus = 38,	//灵力附加
		pet_f1pplus = 39,	//果实1附加
		pet_f2pplus = 40,	//果实2附加
		pet_f3pplus = 41,	//果实3附加
		pet_f4pplus = 42,	//果实4附加
		pet_pstatus = 43,	//是否出战
		pet_pnickname = 44,	//宠物昵称
		pet_autoictv = 45,	//自动激励
		pet_petexlvup = 46,	//属性升级加成
		pet_pointchanging = 47,	//属性转换率
		pet_activechanging = 48,	//活跃度补充效率
		pet_activepermin = 49,	//宠物活跃度消耗
		pet_petexplus = 50,	//宠物经验加成
		pet_Max = 51
	};
	// 属性域 品质 枚举值定义
	enum pet_class_Enum {
		pet_class_0,	//精致
		pet_class_1,	//优秀
		pet_class_2,	//传奇
		pet_class_3,	//神话
		pet_classMax = 4
	};
	// 数据模板结构
	struct pet_t : public item_t {
		int	m_nicon;	//图标
		int	m_nmodnum;	//使用模型
	};
	// 数据实例结构
	struct pet_i : public item_i {
		pet_class_Enum	m_eclass;	//品质
		int	m_nlevel;	//等级
		int	m_nexp;	//经验值
		int	m_nactive;	//活跃度
		int	m_nactivespd;	//活跃度消耗速度
		int	m_nstr;	//力量
		int	m_nagi;	//敏捷
		int	m_ncon;	//耐力
		int	m_nint;	//智力
		int	m_nphy;	//灵力
		int	m_nhabit;	//习性
		int	m_npointplus;	//附加属性
		int	m_nfruit1;	//果实1
		int	m_nfruit2;	//果实2
		int	m_nfruit3;	//果实3
		int	m_nfruit4;	//果实4
		int	m_nskill;	//特技
		int	m_nstrpplus;	//力量附加
		int	m_nagipplus;	//敏捷附加
		int	m_nconpplus;	//耐力附加
		int	m_nintpplus;	//智力附加
		int	m_nphypplus;	//灵力附加
		int	m_nf1pplus;	//果实1附加
		int	m_nf2pplus;	//果实2附加
		int	m_nf3pplus;	//果实3附加
		int	m_nf4pplus;	//果实4附加
		BOOL	m_bpstatus;	//是否出战
		char	m_szpnickname[17];	//宠物昵称
		BOOL	m_bautoictv;	//自动激励
		int	m_npetexlvup;	//属性升级加成
		int	m_npointchanging;	//属性转换率
		int	m_nactivechanging;	//活跃度补充效率
		int	m_nactivepermin;	//宠物活跃度消耗
		int	m_npetexplus;	//宠物经验加成
	};
#ifdef pet_instance 
	int npet_iOffset[51] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1,-1,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,145,149,153,157,161,165	};
#endif //pet_instance 
	//////////////////////////////////////////////////////////////
	// 单元 宠物习性 ID = 27
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum pethabit_Fields {
		pethabit_OtherName = 15,	//别名
		pethabit_Max = 16
	};
	// 数据模板结构
	struct pethabit_t : public item_t {
		char	m_szOtherName[32];	//别名
	};
	// 数据实例结构
	struct pethabit_i : public item_i {
	};
#ifdef pethabit_instance 
	int npethabit_iOffset[16] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,-1	};
#endif //pethabit_instance 
	//////////////////////////////////////////////////////////////
	// 单元 时装 ID = 23
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum dress_Fields {
		dress_Max = 43
	};
	// 数据模板结构
	struct dress_t : public equip_t {
	};
	// 数据实例结构
	struct dress_i : public equip_i {
	};
#ifdef dress_instance 
	int ndress_iOffset[43] = {
		-1,-1,-1,-1,-1,-1,-1,-1,0,-1,4,8,-1,12,16,20,24,28,32,36,40,44,48,52,-1,-1,-1,56,60,64,68,72,76,80,-1,-1,-1,-1,84,88,92,96,-1	};
#endif //dress_instance 
	//////////////////////////////////////////////////////////////
	// 单元 玩家 ID = 3
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum player_Fields {
		player_dbid = 59,	//帐号ID
		player_nickname = 60,	//昵称
		player_exp = 61,	//经验值
		player_title = 62,	//称号
		player_job = 63,	//职业
		player_money = 64,	//未使用
		player_gold = 65,	//元宝
		player_ExpFactor = 66,	//经验倍率
		player_BagNum = 67,	//背包栏数
		player_DepotPage = 68,	//仓库栏数
		player_PK = 69,	//PK值
		player_mailbox = 70,	//邮箱
		player_party = 71,	//所在队伍ID
		player_guild = 72,	//所在公会ID
		player_marry = 73,	//夫妻ID
		player_study = 74,	//师徒ID
		player_brothor = 75,	//结拜ID
		player_PKmode = 76,	//PK模式
		player_soul = 77,	//魂
		player_unusepoint = 78,	//未分配点数
		player_sexy = 79,	//性别
		player_uplevelexp = 80,	//升级所需经验
		player_magicfind = 81,	//寻宝率
		player_rwtimes = 82,	//任务次数
		player_wallowstate = 83,	//防沉迷状态
		player_count_call1 = 84,	//称号计数器1
		player_count_call2 = 85,	//称号计数器2
		player_count_call3 = 86,	//称号计数器3
		player_count_call4 = 87,	//称号计数器4
		player_count_call5 = 88,	//称号计数器5
		player_count_call6 = 89,	//称号计数器6
		player_count_call7 = 90,	//称号计数器7
		player_count_call8 = 91,	//称号计数器8
		player_count_call9 = 92,	//称号计数器9
		player_count_call10 = 93,	//称号计数器10
		player_count_call11 = 94,	//称号计数器11
		player_slient = 95,	//禁言
		player_gminvisible = 96,	//GM隐身
		player_gmunbreakable = 97,	//GM无敌
		player_fbcd = 98,	//法宝冷却时间
		player_ridelv = 99,	//骑乘级别
		player_ridetype = 100,	//骑乘类型
		player_standingroleintegral = 101,	//日常任务积分
		player_gongde = 102,	//功德
		player_aixinzhi = 103,	//爱心值
		player_Max = 104
	};
	// 属性域 职业 枚举值定义
	enum player_job_Enum {
		player_job_0,	//武将
		player_job_1,	//刺客
		player_job_2,	//术士
		player_job_3,	//乐师
		player_jobMax = 4
	};
	// 属性域 PK模式 枚举值定义
	enum player_PKmode_Enum {
		player_PKmode_0,	//普通
		player_PKmode_1,	//PK
		player_PKmode_2,	//强杀
		player_PKmodeMax = 3
	};
	// 属性域 防沉迷状态 枚举值定义
	enum player_wallowstate_Enum {
		player_wallowstate_0,	//健康游戏时间
		player_wallowstate_1,	//疲劳游戏时间
		player_wallowstate_2,	//非健康游戏时间
		player_wallowstateMax = 3
	};
	// 数据模板结构
	struct player_t : public charactor_t {
	};
	// 数据实例结构
	struct player_i : public charactor_i {
		int	m_ndbid;	//帐号ID
		char	m_sznickname[17];	//昵称
		int	m_nexp;	//经验值
		int	m_ntitle;	//称号
		player_job_Enum	m_ejob;	//职业
		int	m_nmoney;	//未使用
		int	m_ngold;	//元宝
		int	m_nExpFactor;	//经验倍率
		int	m_nBagNum;	//背包栏数
		int	m_nDepotPage;	//仓库栏数
		int	m_nPK;	//PK值
		int	m_nmailbox;	//邮箱
		int	m_nparty;	//所在队伍ID
		int	m_nguild;	//所在公会ID
		int	m_nmarry;	//夫妻ID
		int	m_nstudy;	//师徒ID
		int	m_nbrothor;	//结拜ID
		player_PKmode_Enum	m_ePKmode;	//PK模式
		int	m_nsoul;	//魂
		int	m_nunusepoint;	//未分配点数
		BOOL	m_bsexy;	//性别
		int	m_nuplevelexp;	//升级所需经验
		int	m_nmagicfind;	//寻宝率
		int	m_nrwtimes;	//任务次数
		player_wallowstate_Enum	m_ewallowstate;	//防沉迷状态
		int	m_ncount_call1;	//称号计数器1
		char	m_szcount_call2[16];	//称号计数器2
		int	m_ncount_call3;	//称号计数器3
		int	m_ncount_call4;	//称号计数器4
		int	m_ncount_call5;	//称号计数器5
		char	m_szcount_call6[16];	//称号计数器6
		int	m_ncount_call7;	//称号计数器7
		int	m_ncount_call8;	//称号计数器8
		int	m_ncount_call9;	//称号计数器9
		int	m_ncount_call10;	//称号计数器10
		int	m_ncount_call11;	//称号计数器11
		int	m_nslient;	//禁言
		BOOL	m_bgminvisible;	//GM隐身
		BOOL	m_bgmunbreakable;	//GM无敌
		int	m_nfbcd;	//法宝冷却时间
		int	m_nridelv;	//骑乘级别
		int	m_nridetype;	//骑乘类型
		int	m_nstandingroleintegral;	//日常任务积分
		int	m_ngongde;	//功德
		int	m_naixinzhi;	//爱心值
	};
#ifdef player_instance 
	int nplayer_iOffset[104] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,257,261,265,269,273,277,281,285,289,293,297,301,305,309,313,317,321,325,329,333,337,341,345,349,353,369,373,377,381,397,401,405,409,413,417,421,425,429,433,437,441,445,449	};
#endif //player_instance 
	//////////////////////////////////////////////////////////////
	// 单元 NPC ID = 4
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum NPC_Fields {
		NPC_id = 59,	//ID
		NPC_name = 60,	//名称
		NPC_mobclass = 61,	//怪物品质
		NPC_offerexp = 62,	//提供经验值
		NPC_isnpc = 63,	//NPC种类
		NPC_AI = 64,	//AI编号
		NPC_zoom = 65,	//视野
		NPC_follow = 66,	//追击范围
		NPC_mod = 67,	//模型使用
		NPC_nobeatfly = 68,	//击飞免疫
		NPC_moneydrop = 69,	//钱掉落
		NPC_nomdrop = 70,	//普通物品掉落
		NPC_spdrop = 71,	//特殊物品掉落
		NPC_job = 72,	//职业
		NPC_roundable = 73,	//是否旋转
		NPC_nobeatxuanyun = 74,	//眩晕免疫
		NPC_nobeatslow = 75,	//减速免疫
		NPC_nobeatsleep = 76,	//沉睡免疫
		NPC_nobeatchenmo = 77,	//沉默免疫
		NPC_nobeatchongfeng = 78,	//冲锋免疫
		NPC_tietuid = 79,	//贴图ID
		NPC_attdistance = 80,	//普通攻击范围
		NPC_attkind = 81,	//攻击种类
		NPC_birthexcursion = 82,	//出生偏移
		NPC_patrolarea = 83,	//巡逻范围
		NPC_bodybetime = 84,	//尸体存在时间
		NPC_jinengidone = 85,	//技能ID1
		NPC_jinengidtwo = 86,	//技能ID2
		NPC_diermingcheng = 87,	//第二名称
		NPC_Max = 88
	};
	// 属性域 怪物品质 枚举值定义
	enum NPC_mobclass_Enum {
		NPC_mobclass_0,	//普通怪-标准
		NPC_mobclass_1,	//普通怪-强壮
		NPC_mobclass_2,	//普通怪-施法者
		NPC_mobclass_3,	//普通怪-敏锐
		NPC_mobclass_4,	//普通怪-弓手
		NPC_mobclass_5,	//精英怪-标准
		NPC_mobclass_6,	//精英怪-强壮
		NPC_mobclass_7,	//精英怪-施法者
		NPC_mobclass_8,	//精英怪-敏锐
		NPC_mobclass_9,	//精英怪-弓手
		NPC_mobclass_10,	//BOSS
		NPC_mobclass_11,	//BOSS-2
		NPC_mobclass_12,	//BOSS-3
		NPC_mobclass_13,	//BOSS-4
		NPC_mobclass_14,	//BOSS-5
		NPC_mobclass_15,	//BOSS-6
		NPC_mobclass_16,	//BOSS-7
		NPC_mobclass_17,	//BOSS-8
		NPC_mobclass_18,	//宠物
		NPC_mobclass_19,	//普通怪-观赏
		NPC_mobclassMax = 20
	};
	// 属性域 NPC种类 枚举值定义
	enum NPC_isnpc_Enum {
		NPC_isnpc_0,	//怪物
		NPC_isnpc_1,	//NPC
		NPC_isnpc_2,	//机关
		NPC_isnpc_3,	//卫兵
		NPC_isnpcMax = 4
	};
	// 属性域 职业 枚举值定义
	enum NPC_job_Enum {
		NPC_job_0,	//仓库
		NPC_job_1,	//传送
		NPC_job_2,	//打孔镶嵌
		NPC_job_3,	//法宝炼化
		NPC_job_4,	//防具出售
		NPC_job_5,	//公会
		NPC_job_6,	//婚姻
		NPC_job_7,	//矿场
		NPC_job_8,	//拍卖
		NPC_job_9,	//器灵
		NPC_job_10,	//日常任务
		NPC_job_11,	//首饰出售
		NPC_job_12,	//武器出售
		NPC_job_13,	//药品出售
		NPC_job_14,	//装备强化
		NPC_job_15,	//无职业
		NPC_job_16,	//坐骑
		NPC_job_17,	//特殊武器出售
		NPC_job_18,	//特殊防具出售
		NPC_job_19,	//宝石商
		NPC_jobMax = 20
	};
	// 属性域 第二名称 枚举值定义
	enum NPC_diermingcheng_Enum {
		NPC_diermingcheng_0,	//<仓库>
		NPC_diermingcheng_1,	//<传送>
		NPC_diermingcheng_2,	//<打孔镶嵌>
		NPC_diermingcheng_3,	//<法宝炼化>
		NPC_diermingcheng_4,	//<防具出售>
		NPC_diermingcheng_5,	//<公会>
		NPC_diermingcheng_6,	//<婚姻>
		NPC_diermingcheng_7,	//<矿场>
		NPC_diermingcheng_8,	//<拍卖>
		NPC_diermingcheng_9,	//<器灵>
		NPC_diermingcheng_10,	//<日常任务>
		NPC_diermingcheng_11,	//<首饰出售>
		NPC_diermingcheng_12,	//<武器出售>
		NPC_diermingcheng_13,	//<药品出售>
		NPC_diermingcheng_14,	//<升级/锻造>
		NPC_diermingcheng_15,	//无
		NPC_diermingcheng_16,	//<特殊武器出售>
		NPC_diermingcheng_17,	//<特殊防具出售>
		NPC_diermingcheng_18,	//<宝石商>
		NPC_diermingcheng_19,	//<稀有BOSS>
		NPC_diermingcheng_20,	//<任务BOSS>
		NPC_diermingcheng_21,	//<副本头目>
		NPC_diermingcheng_22,	//<矿场守卫>
		NPC_diermingcheng_23,	//<坐骑>
		NPC_diermingcheng_24,	//<装备绑定>
		NPC_diermingcheng_25,	//<高级特殊武器出售>
		NPC_diermingcheng_26,	//<高级特殊防具出售>
		NPC_diermingcheng_27,	//<高级武器出售>
		NPC_diermingcheng_28,	//<高级防具出售>
		NPC_diermingchengMax = 29
	};
	// 数据模板结构
	struct NPC_t : public charactor_t {
		int	m_nid;	//ID
		char	m_szname[32];	//名称
		NPC_mobclass_Enum	m_emobclass;	//怪物品质
		NPC_isnpc_Enum	m_eisnpc;	//NPC种类
		int	m_nAI;	//AI编号
		int	m_nmod;	//模型使用
		BOOL	m_bnobeatfly;	//击飞免疫
		BOOL	m_bmoneydrop;	//钱掉落
		BOOL	m_bnomdrop;	//普通物品掉落
		BOOL	m_bspdrop;	//特殊物品掉落
		NPC_job_Enum	m_ejob;	//职业
		BOOL	m_broundable;	//是否旋转
		BOOL	m_bnobeatxuanyun;	//眩晕免疫
		BOOL	m_bnobeatslow;	//减速免疫
		BOOL	m_bnobeatsleep;	//沉睡免疫
		BOOL	m_bnobeatchenmo;	//沉默免疫
		BOOL	m_bnobeatchongfeng;	//冲锋免疫
		char	m_sztietuid[16];	//贴图ID
		int	m_nattdistance;	//普通攻击范围
		int	m_nattkind;	//攻击种类
		char	m_szbirthexcursion[4];	//出生偏移
		char	m_szpatrolarea[4];	//巡逻范围
		char	m_szbodybetime[4];	//尸体存在时间
		int	m_njinengidone;	//技能ID1
		int	m_njinengidtwo;	//技能ID2
		NPC_diermingcheng_Enum	m_ediermingcheng;	//第二名称
	};
	// 数据实例结构
	struct NPC_i : public charactor_i {
		char	m_szname[32];	//名称
		int	m_nofferexp;	//提供经验值
		int	m_nzoom;	//视野
		int	m_nfollow;	//追击范围
		char	m_szbirthexcursion[4];	//出生偏移
		char	m_szpatrolarea[4];	//巡逻范围
		char	m_szbodybetime[4];	//尸体存在时间
		NPC_diermingcheng_Enum	m_ediermingcheng;	//第二名称
	};
#ifdef NPC_instance 
	int nNPC_iOffset[88] = {
		0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,-1,236,-1,268,-1,-1,272,276,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,280,284,288,-1,-1,292	};
#endif //NPC_instance 
	// 联系ID枚举
	enum Contact_IDs {
		Contact_EffectOpScript = 1,	//影响操作
		Contact_EffectValueScript = 2,	//影响值计算脚本
		Contact_SkillType = 3,	//技能类型
		Contact_actionCondition = 4,	//触发条件
		Contact_mobtype = 5,	//怪物模板
		Contact_gonghuiskillstudy = 7,	//公会技能学习
		Contact_lastnamefbbaoliu = 8,	//法宝后缀（保留）
		Contact_typefb = 9,	//法宝种类
		Contact_moblevelroom = 10,	//怪物等级区间
		Contact_drop = 11,	//物品掉落集合
		Contact_makelist = 12,	//物品制造单
		Contact_drpplist = 13,	//怪物特殊掉落
		Contact_npcgn = 14,	//NPC对应功能
		Contact_selllist = 15,	//售卖列表
		Contact_spyx = 16,	//特殊影响
		Contact_gnnpc = 17,	//功能NPC对应
		Contact_spskill = 18,	//特殊技能参数
		Contact_forginghort = 19,	//锻造装备奖励对应
		Contact_StudySkillDepend = 20,	//技能学习依赖
		Contact_mapoutport = 21,	//地图连接点
		Contact_shuxingjiazhi = 22,	//属性价值
		Contact_jinglianduiying = 23,	//精炼对应
		Contact_xiaoxiilebiao = 24,	//消息列表
		Contact_title = 25,	//称号组合
		Contact_ditufuhuodian = 26,	//地图复活点
		Contact_kuangchangshouwei = 27,	//矿场守卫
		Contact_gonghui = 28,	//公会
		Contact_charmodetype = 29,	//玩家模型模板
		Contact_KCshouweiduiying = 30,	//矿场卷轴对应
		Contact_NPCskillscontact = 31,	//NPC技能联系
		Contact_NPCAI = 32,	//怪物AI
		Contact_jingliandrop = 33,	//精练掉落
		Contact_gemdrop = 34,	//宝石掉落
		Contact_itemdrop = 35,	//道具掉落
		Contact_smithingdrop = 36,	//锻造掉落
		Contact_maptransmissionlist = 37,	//传送列表
		Contact_transmissionprice = 38,	//传送价格表
		Contact_fbmake = 39,	//单前缀法宝生成
		Contact_doublefbmake = 40,	//双前缀法宝生成
		Contact_LurkShape = 41,	//变形模型
		Contact_skillspecialpercent = 42,	//技能特殊百分比
		Contact_mapway = 43,	//地图寻路
		Contact_jinglianlianxibiao = 44,	//精炼部位联系表
		Contact_equipmake = 45,	//装备生成
		Contact_buweixishu = 47,	//部位系数表
		Contact_sjsxxishubiao = 48,	//随机属性对照表
		Contact_gdsxduizhaobiao = 49,	//固定属性对照表
		Contact_canpreviewlist = 50,	//可预览列表
		Contact_bagcontactlist = 51,	//礼包联系表
		Contact_xysjsxlist = 52,	//稀有随机属性对照表
		Contact_smeltstone = 53,	//器灵
		Contact_stonelvuplist = 54,	//器灵升级对照表
		Contact_dtncslist = 55,	//地图内传送列表
		Contact_resourcelist = 56,	//出矿分布表
		Contact_richangrenwulvarea = 57,	//日常任务等级区间
		Contact_sjsxdjssxysb = 58,	//随机属性到角色属性映射表
		Contact_csnpclb = 59,	//传送NPC列表
		Contact_roletypelist = 60,	//任务类型表
		Contact_rolelvareaanddepotid = 61,	//任务等级区间和对应库
		Contact_depotidandroleidlist = 62,	//库和任务对应表
		Contact_replacerolelist = 63,	//可接任务列表
		Contact_integralaward = 64,	//积分奖励表
		Contact_shitujianglilist = 65,	//师徒奖励表
		Contact_jiebaiqinmidu = 66,	//结拜亲密度
		Contact_huiguangdangci = 67,	//辉光档次
		Contact_huiguangyanse = 68,	//辉光颜色
		Contact_xinshouzhuantiao = 69,	//新手地图转跳
		Contact_fubenboss = 70,	//副本BOSS标记
		Contact_npczidongxunlufenlei = 71,	//NPC自动寻路分类
		Contact_baoshihecheng = 72,	//宝石合成
		Contact_petclass = 73,	//宠物品质
		Contact_petfruit = 74,	//习性果实
		Contact_petdrop = 75,	//宠物掉落
		Contact_petfruitbagcontactlist = 76,	//宠物果实礼包联系表
	};
	//////////////////////////////////////////////////////////////
	// 联系 影响操作 ID = 1
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum EffectOpScript_Fields {
		EffectOpScript_ID = 0,	//ID
		EffectOpScript_Name = 1,	//名称
		EffectOpScript_Alias = 2,	//英文别名
		EffectOpScript_Max = 3
	};
	// 联系数据结构
	struct EffectOpScript_t {
		int	m_nID;	//ID
		char	m_szName[16];	//名称
		char	m_szAlias[16];	//英文别名
	};
	//////////////////////////////////////////////////////////////
	// 联系 影响值计算脚本 ID = 2
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum EffectValueScript_Fields {
		EffectValueScript_id = 0,	//ID
		EffectValueScript_name = 1,	//名称
		EffectValueScript_Alias = 2,	//别名
		EffectValueScript_param1 = 3,	//参数1
		EffectValueScript_param2 = 4,	//参数2
		EffectValueScript_param3 = 5,	//参数3
		EffectValueScript_param4 = 6,	//参数4
		EffectValueScript_Max = 7
	};
	// 属性域 (null) 枚举值定义
	enum EffectValueScript_param1_Enum {
		EffectValueScript_param1_0,	//
		EffectValueScript_param1_1,	//
		EffectValueScript_param1_2,	//
		EffectValueScript_param1Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum EffectValueScript_param2_Enum {
		EffectValueScript_param2_0,	//
		EffectValueScript_param2_1,	//
		EffectValueScript_param2_2,	//
		EffectValueScript_param2Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum EffectValueScript_param3_Enum {
		EffectValueScript_param3_0,	//
		EffectValueScript_param3_1,	//
		EffectValueScript_param3_2,	//
		EffectValueScript_param3Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum EffectValueScript_param4_Enum {
		EffectValueScript_param4_0,	//
		EffectValueScript_param4_1,	//
		EffectValueScript_param4_2,	//
		EffectValueScript_param4Max = 3
	};
	// 联系数据结构
	struct EffectValueScript_t {
		int	m_nid;	//ID
		char	m_szname[32];	//名称
		char	m_szAlias[16];	//别名
		EffectValueScript_param1_Enum	m_eparam1;	//参数1
		EffectValueScript_param2_Enum	m_eparam2;	//参数2
		EffectValueScript_param3_Enum	m_eparam3;	//参数3
		EffectValueScript_param4_Enum	m_eparam4;	//参数4
	};
	//////////////////////////////////////////////////////////////
	// 联系 技能类型 ID = 3
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum SkillType_Fields {
		SkillType_ID = 0,	//id
		SkillType_Name = 1,	//名称
		SkillType_Alias = 2,	//英文别名
		SkillType_Max = 3
	};
	// 联系数据结构
	struct SkillType_t {
		int	m_nID;	//id
		char	m_szName[32];	//名称
		char	m_szAlias[16];	//英文别名
	};
	//////////////////////////////////////////////////////////////
	// 联系 触发条件 ID = 4
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum actionCondition_Fields {
		actionCondition_Max = 0
	};
	// 联系数据结构
	struct actionCondition_t {
	};
	//////////////////////////////////////////////////////////////
	// 联系 怪物模板 ID = 5
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum mobtype_Fields {
		mobtype_level = 0,	//等级
		mobtype_mobclass = 1,	//怪物种类
		mobtype_leastdc = 2,	//最小物理攻击力
		mobtype_maxdc = 3,	//最大物理攻击力
		mobtype_ac = 4,	//物理防御力
		mobtype_mac = 5,	//法术防御力
		mobtype_crit = 6,	//物理重击率
		mobtype_skillcrit = 7,	//技能重击率
		mobtype_hitchance = 8,	//命中
		mobtype_hedge = 9,	//躲闪
		mobtype_hpreg = 10,	//生命恢复速度
		mobtype_mpreg = 11,	//法力恢复速度
		mobtype_attspeed = 12,	//攻击速度
		mobtype_maxhp = 13,	//最大生命值
		mobtype_maxmp = 14,	//最大法力值
		mobtype_offerexp = 15,	//提供经验值
		mobtype_zoom = 16,	//视野
		mobtype_follow = 17,	//追击范围
		mobtype_magicatt = 18,	//法术攻击力
		mobtype_Max = 19
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct mobtype_t {
		int	m_nlevel;	//等级
		mobtype_mobclass_Enum	m_emobclass;	//怪物种类
		int	m_nleastdc;	//最小物理攻击力
		int	m_nmaxdc;	//最大物理攻击力
		int	m_nac;	//物理防御力
		int	m_nmac;	//法术防御力
		int	m_ncrit;	//物理重击率
		int	m_nskillcrit;	//技能重击率
		int	m_nhitchance;	//命中
		int	m_nhedge;	//躲闪
		int	m_nhpreg;	//生命恢复速度
		int	m_nmpreg;	//法力恢复速度
		float	m_fattspeed;	//攻击速度
		int	m_nmaxhp;	//最大生命值
		int	m_nmaxmp;	//最大法力值
		int	m_nofferexp;	//提供经验值
		int	m_nzoom;	//视野
		int	m_nfollow;	//追击范围
		int	m_nmagicatt;	//法术攻击力
	};
	//////////////////////////////////////////////////////////////
	// 联系 公会技能学习 ID = 7
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gonghuiskillstudy_Fields {
		gonghuiskillstudy_gonghuiskilskillname = 0,	//技能名称
		gonghuiskillstudy_skillid = 1,	//技能ID
		gonghuiskillstudy_gonghuiLV = 2,	//公会等级
		gonghuiskillstudy_money = 3,	//研发金钱
		gonghuiskillstudy_Max = 4
	};
	// 联系数据结构
	struct gonghuiskillstudy_t {
		char	m_szgonghuiskilskillname[16];	//技能名称
		int	m_nskillid;	//技能ID
		int	m_ngonghuiLV;	//公会等级
		int	m_nmoney;	//研发金钱
	};
	//////////////////////////////////////////////////////////////
	// 联系 法宝后缀（保留） ID = 8
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum lastnamefbbaoliu_Fields {
		lastnamefbbaoliu_Max = 0
	};
	// 联系数据结构
	struct lastnamefbbaoliu_t {
	};
	//////////////////////////////////////////////////////////////
	// 联系 法宝种类 ID = 9
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum typefb_Fields {
		typefb_typename = 0,	//种类名
		typefb_skillas = 1,	//对应技能ID
		typefb_firenum = 2,	//炼化次数
		typefb_Max = 3
	};
	// 联系数据结构
	struct typefb_t {
		char	m_sztypename[16];	//种类名
		int	m_nskillas;	//对应技能ID
		int	m_nfirenum;	//炼化次数
	};
	//////////////////////////////////////////////////////////////
	// 联系 怪物等级区间 ID = 10
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum moblevelroom_Fields {
		moblevelroom_ID = 0,	//ID
		moblevelroom_levelmin = 1,	//对应等级下限
		moblevelroom_levelmax = 2,	//对应等级上限
		moblevelroom_Max = 3
	};
	// 联系数据结构
	struct moblevelroom_t {
		int	m_nID;	//ID
		int	m_nlevelmin;	//对应等级下限
		int	m_nlevelmax;	//对应等级上限
	};
	//////////////////////////////////////////////////////////////
	// 联系 物品掉落集合 ID = 11
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum drop_Fields {
		drop_room = 0,	//从属区间
		drop_equipnum = 1,	//装备编号
		drop_dorpclass = 2,	//掉落类别
		drop_dropprobability = 3,	//掉落概率
		drop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum drop_dorpclass_Enum {
		drop_dorpclass_0,	//
		drop_dorpclass_1,	//
		drop_dorpclass_2,	//
		drop_dorpclassMax = 3
	};
	// 联系数据结构
	struct drop_t {
		int	m_nroom;	//从属区间
		int	m_nequipnum;	//装备编号
		drop_dorpclass_Enum	m_edorpclass;	//掉落类别
		int	m_ndropprobability;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 物品制造单 ID = 12
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum makelist_Fields {
		makelist_ID = 0,	//编号
		makelist_paper = 1,	//制作图纸
		makelist_eqid = 2,	//装备编号
		makelist_class1 = 3,	//需求1种类
		makelist_id1 = 4,	//需求1编号
		makelist_num1 = 5,	//需求1数量
		makelist_class2 = 6,	//需求2种类
		makelist_id2 = 7,	//需求2编号
		makelist_num2 = 8,	//需求2数量
		makelist_class3 = 9,	//需求3种类
		makelist_id3 = 10,	//需求3编号
		makelist_num3 = 11,	//需求3数量
		makelist_class4 = 12,	//需求4种类
		makelist_id4 = 13,	//需求4编号
		makelist_num4 = 14,	//需求4数量
		makelist_class5 = 15,	//需求5种类
		makelist_id5 = 16,	//需求5编号
		makelist_num5 = 17,	//需求5数量
		makelist_class6 = 18,	//需求6种类
		makelist_id6 = 19,	//需求6编号
		makelist_num6 = 20,	//需求6数量
		makelist_papertype = 21,	//图纸类型
		makelist_hortpro = 22,	//奖励几率
		makelist_Max = 23
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class1_Enum {
		makelist_class1_0,	//
		makelist_class1_1,	//
		makelist_class1_2,	//
		makelist_class1_3,	//
		makelist_class1Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class2_Enum {
		makelist_class2_0,	//
		makelist_class2_1,	//
		makelist_class2_2,	//
		makelist_class2_3,	//
		makelist_class2Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class3_Enum {
		makelist_class3_0,	//
		makelist_class3_1,	//
		makelist_class3_2,	//
		makelist_class3_3,	//
		makelist_class3Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class4_Enum {
		makelist_class4_0,	//
		makelist_class4_1,	//
		makelist_class4_2,	//
		makelist_class4_3,	//
		makelist_class4Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class5_Enum {
		makelist_class5_0,	//
		makelist_class5_1,	//
		makelist_class5_2,	//
		makelist_class5_3,	//
		makelist_class5Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum makelist_class6_Enum {
		makelist_class6_0,	//
		makelist_class6_1,	//
		makelist_class6_2,	//
		makelist_class6_3,	//
		makelist_class6Max = 4
	};
	// 联系数据结构
	struct makelist_t {
		int	m_nID;	//编号
		int	m_npaper;	//制作图纸
		int	m_neqid;	//装备编号
		makelist_class1_Enum	m_eclass1;	//需求1种类
		int	m_nid1;	//需求1编号
		int	m_nnum1;	//需求1数量
		makelist_class2_Enum	m_eclass2;	//需求2种类
		int	m_nid2;	//需求2编号
		int	m_nnum2;	//需求2数量
		makelist_class3_Enum	m_eclass3;	//需求3种类
		int	m_nid3;	//需求3编号
		int	m_nnum3;	//需求3数量
		makelist_class4_Enum	m_eclass4;	//需求4种类
		int	m_nid4;	//需求4编号
		int	m_nnum4;	//需求4数量
		makelist_class5_Enum	m_eclass5;	//需求5种类
		int	m_nid5;	//需求5编号
		int	m_nnum5;	//需求5数量
		makelist_class6_Enum	m_eclass6;	//需求6种类
		int	m_nid6;	//需求6编号
		int	m_nnum6;	//需求6数量
		int	m_npapertype;	//图纸类型
		int	m_nhortpro;	//奖励几率
	};
	//////////////////////////////////////////////////////////////
	// 联系 怪物特殊掉落 ID = 13
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum drpplist_Fields {
		drpplist_ID = 0,	//ID
		drpplist_mobid = 1,	//怪物编号
		drpplist_droptype = 2,	//掉落类型
		drpplist_equid = 3,	//掉落编号
		drpplist_dropmey = 4,	//掉落几率
		drpplist_droptimes = 5,	//掉落次数
		drpplist_Max = 6
	};
	// 属性域 (null) 枚举值定义
	enum drpplist_droptype_Enum {
		drpplist_droptype_0,	//
		drpplist_droptype_1,	//
		drpplist_droptype_2,	//
		drpplist_droptypeMax = 3
	};
	// 联系数据结构
	struct drpplist_t {
		int	m_nID;	//ID
		int	m_nmobid;	//怪物编号
		drpplist_droptype_Enum	m_edroptype;	//掉落类型
		int	m_nequid;	//掉落编号
		float	m_fdropmey;	//掉落几率
		int	m_ndroptimes;	//掉落次数
	};
	//////////////////////////////////////////////////////////////
	// 联系 NPC对应功能 ID = 14
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum npcgn_Fields {
		npcgn_ID = 0,	//ID
		npcgn_dygn = 1,	//对应功能
		npcgn_sign = 2,	//功能标签
		npcgn_gnlist = 3,	//功能列表
		npcgn_sfsm = 4,	//是否售卖
		npcgn_Max = 5
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct npcgn_t {
		int	m_nID;	//ID
		npcgn_dygn_Enum	m_edygn;	//对应功能
		char	m_szsign[32];	//功能标签
		int	m_ngnlist;	//功能列表
		BOOL	m_bsfsm;	//是否售卖
	};
	//////////////////////////////////////////////////////////////
	// 联系 售卖列表 ID = 15
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum selllist_Fields {
		selllist_Listid = 0,	//售卖单编号
		selllist_wpclass = 1,	//售卖物品种类
		selllist_wpid = 2,	//售卖物品编号
		selllist_class = 3,	//物品品质
		selllist_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum selllist_wpclass_Enum {
		selllist_wpclass_0,	//
		selllist_wpclass_1,	//
		selllist_wpclass_2,	//
		selllist_wpclass_3,	//
		selllist_wpclassMax = 4
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct selllist_t {
		int	m_nListid;	//售卖单编号
		selllist_wpclass_Enum	m_ewpclass;	//售卖物品种类
		int	m_nwpid;	//售卖物品编号
		selllist_class_Enum	m_eclass;	//物品品质
	};
	//////////////////////////////////////////////////////////////
	// 联系 特殊影响 ID = 16
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum spyx_Fields {
		spyx_Max = 0
	};
	// 联系数据结构
	struct spyx_t {
	};
	//////////////////////////////////////////////////////////////
	// 联系 功能NPC对应 ID = 17
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gnnpc_Fields {
		gnnpc_NPCID = 0,	//NPC编号
		gnnpc_GNID = 1,	//对应功能编号
		gnnpc_Max = 2
	};
	// 联系数据结构
	struct gnnpc_t {
		int	m_nNPCID;	//NPC编号
		int	m_nGNID;	//对应功能编号
	};
	//////////////////////////////////////////////////////////////
	// 联系 特殊技能参数 ID = 18
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum spskill_Fields {
		spskill_skillid = 0,	//技能编号
		spskill_jgtype = 1,	//结果类型
		spskill_jg1 = 2,	//结果参数a
		spskill_jg2 = 3,	//结果参数b
		spskill_jg3 = 4,	//结果参数c
		spskill_jntxt = 5,	//技能描述
		spskill_sfjssx = 6,	//是否即时生效
		spskill_Max = 7
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct spskill_t {
		int	m_nskillid;	//技能编号
		spskill_jgtype_Enum	m_ejgtype;	//结果类型
		int	m_njg1;	//结果参数a
		int	m_njg2;	//结果参数b
		int	m_njg3;	//结果参数c
		char	m_szjntxt[128];	//技能描述
		BOOL	m_bsfjssx;	//是否即时生效
	};
	//////////////////////////////////////////////////////////////
	// 联系 锻造装备奖励对应 ID = 19
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum forginghort_Fields {
		forginghort_horttype = 0,	//奖励类型
		forginghort_gainpro = 1,	//获得几率
		forginghort_hortpro1 = 2,	//奖励属性1
		forginghort_hortcost1 = 3,	//奖励值1
		forginghort_hortpro2 = 4,	//奖励属性2
		forginghort_hortcost2 = 5,	//奖励值2
		forginghort_hortpro3 = 6,	//奖励属性3
		forginghort_hortcost3 = 7,	//奖励值3
		forginghort_Max = 8
	};
	// 联系数据结构
	struct forginghort_t {
		int	m_nhorttype;	//奖励类型
		int	m_ngainpro;	//获得几率
		int	m_nhortpro1;	//奖励属性1
		int	m_nhortcost1;	//奖励值1
		int	m_nhortpro2;	//奖励属性2
		int	m_nhortcost2;	//奖励值2
		int	m_nhortpro3;	//奖励属性3
		int	m_nhortcost3;	//奖励值3
	};
	//////////////////////////////////////////////////////////////
	// 联系 技能学习依赖 ID = 20
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum StudySkillDepend_Fields {
		StudySkillDepend_SkillId = 0,	//技能
		StudySkillDepend_DependSkillId = 1,	//依赖技能
		StudySkillDepend_Max = 2
	};
	// 联系数据结构
	struct StudySkillDepend_t {
		int	m_nSkillId;	//技能
		int	m_nDependSkillId;	//依赖技能
	};
	//////////////////////////////////////////////////////////////
	// 联系 地图连接点 ID = 21
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum mapoutport_Fields {
		mapoutport_mapid = 0,	//所属地图id
		mapoutport_x = 1,	//x
		mapoutport_y = 2,	//y
		mapoutport_w = 3,	//w
		mapoutport_h = 4,	//h
		mapoutport_tomap = 5,	//tomap
		mapoutport_tox = 6,	//tox
		mapoutport_toy = 7,	//toy
		mapoutport_tow = 8,	//tow
		mapoutport_toh = 9,	//toh
		mapoutport_degree = 10,	//角度
		mapoutport_Max = 11
	};
	// 联系数据结构
	struct mapoutport_t {
		int	m_nmapid;	//所属地图id
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
		int	m_ntomap;	//tomap
		int	m_ntox;	//tox
		int	m_ntoy;	//toy
		int	m_ntow;	//tow
		int	m_ntoh;	//toh
		int	m_ndegree;	//角度
	};
	//////////////////////////////////////////////////////////////
	// 联系 属性价值 ID = 22
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum shuxingjiazhi_Fields {
		shuxingjiazhi_ID = 0,	//ID
		shuxingjiazhi_levelmin = 1,	//等级区间下限
		shuxingjiazhi_levelmax = 2,	//等级区间上限
		shuxingjiazhi_strmin = 3,	//力量下限
		shuxingjiazhi_strmax = 4,	//力量上限
		shuxingjiazhi_dexmin = 5,	//敏捷下限
		shuxingjiazhi_dexmax = 6,	//敏捷上限
		shuxingjiazhi_intmin = 7,	//智慧下限
		shuxingjiazhi_intmax = 8,	//智慧上限
		shuxingjiazhi_conmin = 9,	//体质下限
		shuxingjiazhi_conmax = 10,	//体质上限
		shuxingjiazhi_phymin = 11,	//体魄下限
		shuxingjiazhi_phymax = 12,	//体魄上限
		shuxingjiazhi_attmin = 13,	//物理攻击力下限
		shuxingjiazhi_attmax = 14,	//物理攻击力上限
		shuxingjiazhi_defmin = 15,	//物理防御力下限
		shuxingjiazhi_defmax = 16,	//物理防御力上限
		shuxingjiazhi_mattmin = 17,	//法术效果下限
		shuxingjiazhi_mattmax = 18,	//法术效果上限
		shuxingjiazhi_mdefmin = 19,	//法术防御力下限
		shuxingjiazhi_mdefmax = 20,	//法术防御力上限
		shuxingjiazhi_lifemin = 21,	//生命最大值下限
		shuxingjiazhi_lifemax = 22,	//生命最大值上限
		shuxingjiazhi_manamin = 23,	//法力最大值下限
		shuxingjiazhi_manamax = 24,	//法力最大值上限
		shuxingjiazhi_hitmin = 25,	//命中下限
		shuxingjiazhi_hitmax = 26,	//命中上限
		shuxingjiazhi_dodgemin = 27,	//躲闪下限
		shuxingjiazhi_dodgemax = 28,	//躲闪上限
		shuxingjiazhi_phycrmin = 29,	//物理重击率下限
		shuxingjiazhi_phycrmax = 30,	//物理重击率上限
		shuxingjiazhi_skicrmin = 31,	//技能重击率下限
		shuxingjiazhi_skicrmax = 32,	//技能重击率上限
		shuxingjiazhi_liferemin = 33,	//生命恢复速度下限
		shuxingjiazhi_liferemax = 34,	//生命恢复速度上限
		shuxingjiazhi_manaremin = 35,	//法力恢复速度下限
		shuxingjiazhi_manaremax = 36,	//法力恢复速度上限
		shuxingjiazhi_rpmin = 37,	//掉宝率下限
		shuxingjiazhi_rpmax = 38,	//掉宝率上限
		shuxingjiazhi_Max = 39
	};
	// 联系数据结构
	struct shuxingjiazhi_t {
		int	m_nID;	//ID
		int	m_nlevelmin;	//等级区间下限
		int	m_nlevelmax;	//等级区间上限
		int	m_nstrmin;	//力量下限
		int	m_nstrmax;	//力量上限
		int	m_ndexmin;	//敏捷下限
		int	m_ndexmax;	//敏捷上限
		int	m_nintmin;	//智慧下限
		int	m_nintmax;	//智慧上限
		int	m_nconmin;	//体质下限
		int	m_nconmax;	//体质上限
		int	m_nphymin;	//体魄下限
		int	m_nphymax;	//体魄上限
		int	m_nattmin;	//物理攻击力下限
		int	m_nattmax;	//物理攻击力上限
		int	m_ndefmin;	//物理防御力下限
		int	m_ndefmax;	//物理防御力上限
		int	m_nmattmin;	//法术效果下限
		int	m_nmattmax;	//法术效果上限
		int	m_nmdefmin;	//法术防御力下限
		int	m_nmdefmax;	//法术防御力上限
		int	m_nlifemin;	//生命最大值下限
		int	m_nlifemax;	//生命最大值上限
		int	m_nmanamin;	//法力最大值下限
		int	m_nmanamax;	//法力最大值上限
		int	m_nhitmin;	//命中下限
		int	m_nhitmax;	//命中上限
		int	m_ndodgemin;	//躲闪下限
		int	m_ndodgemax;	//躲闪上限
		int	m_nphycrmin;	//物理重击率下限
		int	m_nphycrmax;	//物理重击率上限
		int	m_nskicrmin;	//技能重击率下限
		int	m_nskicrmax;	//技能重击率上限
		int	m_nliferemin;	//生命恢复速度下限
		int	m_nliferemax;	//生命恢复速度上限
		int	m_nmanaremin;	//法力恢复速度下限
		int	m_nmanaremax;	//法力恢复速度上限
		int	m_nrpmin;	//掉宝率下限
		int	m_nrpmax;	//掉宝率上限
	};
	//////////////////////////////////////////////////////////////
	// 联系 精炼对应 ID = 23
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum jinglianduiying_Fields {
		jinglianduiying_itemID = 0,	//道具ID
		jinglianduiying_fineID = 1,	//精炼ID
		jinglianduiying_Max = 2
	};
	// 联系数据结构
	struct jinglianduiying_t {
		int	m_nitemID;	//道具ID
		int	m_nfineID;	//精炼ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 消息列表 ID = 24
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum xiaoxiilebiao_Fields {
		xiaoxiilebiao_ID = 0,	//ID
		xiaoxiilebiao_type = 1,	//消息类型
		xiaoxiilebiao_message = 2,	//消息内容
		xiaoxiilebiao_explain = 3,	//消息解释
		xiaoxiilebiao_anothername = 4,	//消息别名
		xiaoxiilebiao_Max = 5
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct xiaoxiilebiao_t {
		int	m_nID;	//ID
		xiaoxiilebiao_type_Enum	m_etype;	//消息类型
		char	m_szmessage[64];	//消息内容
		char	m_szexplain[64];	//消息解释
		char	m_szanothername[64];	//消息别名
	};
	//////////////////////////////////////////////////////////////
	// 联系 称号组合 ID = 25
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum title_Fields {
		title_newcall = 0,	//新称号
		title_membercall = 1,	//成员称号
		title_Max = 2
	};
	// 联系数据结构
	struct title_t {
		int	m_nnewcall;	//新称号
		int	m_nmembercall;	//成员称号
	};
	//////////////////////////////////////////////////////////////
	// 联系 地图复活点 ID = 26
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum ditufuhuodian_Fields {
		ditufuhuodian_mapid = 0,	//复活点地图id
		ditufuhuodian_Type = 1,	//复活点类型
		ditufuhuodian_x = 2,	//x
		ditufuhuodian_y = 3,	//y
		ditufuhuodian_w = 4,	//w
		ditufuhuodian_h = 5,	//h
		ditufuhuodian_job = 6,	//职业
		ditufuhuodian_deadmapid = 7,	//死亡时地图ID
		ditufuhuodian_deadkcid = 8,	//死亡时矿场ID
		ditufuhuodian_Max = 9
	};
	// 属性域 (null) 枚举值定义
	enum ditufuhuodian_Type_Enum {
		ditufuhuodian_Type_0,	//
		ditufuhuodian_Type_1,	//
		ditufuhuodian_Type_2,	//
		ditufuhuodian_Type_3,	//
		ditufuhuodian_Type_4,	//
		ditufuhuodian_TypeMax = 5
	};
	// 属性域 (null) 枚举值定义
	enum ditufuhuodian_job_Enum {
		ditufuhuodian_job_0,	//
		ditufuhuodian_job_1,	//
		ditufuhuodian_job_2,	//
		ditufuhuodian_job_3,	//
		ditufuhuodian_job_4,	//
		ditufuhuodian_jobMax = 5
	};
	// 联系数据结构
	struct ditufuhuodian_t {
		int	m_nmapid;	//复活点地图id
		ditufuhuodian_Type_Enum	m_eType;	//复活点类型
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
		ditufuhuodian_job_Enum	m_ejob;	//职业
		int	m_ndeadmapid;	//死亡时地图ID
		int	m_ndeadkcid;	//死亡时矿场ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 矿场守卫 ID = 27
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum kuangchangshouwei_Fields {
		kuangchangshouwei_kuangchangid = 0,	//矿场ID
		kuangchangshouwei_kuangchangmingcheng = 1,	//矿场名称
		kuangchangshouwei_npcid = 2,	//NPCID
		kuangchangshouwei_X = 3,	//x
		kuangchangshouwei_Y = 4,	//y
		kuangchangshouwei_shouweifanhao = 5,	//守卫番号
		kuangchangshouwei_Max = 6
	};
	// 联系数据结构
	struct kuangchangshouwei_t {
		int	m_nkuangchangid;	//矿场ID
		char	m_szkuangchangmingcheng[16];	//矿场名称
		int	m_nnpcid;	//NPCID
		int	m_nX;	//x
		int	m_nY;	//y
		int	m_nshouweifanhao;	//守卫番号
	};
	//////////////////////////////////////////////////////////////
	// 联系 公会 ID = 28
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gonghui_Fields {
		gonghui_Lv = 0,	//等级
		gonghui_zuidarenkou = 1,	//最大人口
		gonghui_tie = 2,	//铁
		gonghui_tong = 3,	//铜
		gonghui_yin = 4,	//银
		gonghui_jin = 5,	//金
		gonghui_yushi = 6,	//玉石
		gonghui_butian = 7,	//补天
		gonghui_jinku = 8,	//金库
		gonghui_LvupGold = 9,	//升级金钱
		gonghui_Max = 10
	};
	// 联系数据结构
	struct gonghui_t {
		int	m_nLv;	//等级
		int	m_nzuidarenkou;	//最大人口
		int	m_ntie;	//铁
		int	m_ntong;	//铜
		int	m_nyin;	//银
		int	m_njin;	//金
		int	m_nyushi;	//玉石
		int	m_nbutian;	//补天
		int	m_njinku;	//金库
		int	m_nLvupGold;	//升级金钱
	};
	//////////////////////////////////////////////////////////////
	// 联系 玩家模型模板 ID = 29
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum charmodetype_Fields {
		charmodetype_attdistance = 0,	//普通攻击范围
		charmodetype_movespeed = 1,	//移动速度
		charmodetype_runspeed = 2,	//跑步速度
		charmodetype_Max = 3
	};
	// 联系数据结构
	struct charmodetype_t {
		int	m_nattdistance;	//普通攻击范围
		int	m_nmovespeed;	//移动速度
		int	m_nrunspeed;	//跑步速度
	};
	//////////////////////////////////////////////////////////////
	// 联系 矿场卷轴对应 ID = 30
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum KCshouweiduiying_Fields {
		KCshouweiduiying_kcjzID = 0,	//矿场卷轴ID
		KCshouweiduiying_swID = 1,	//守卫ID
		KCshouweiduiying_Max = 2
	};
	// 联系数据结构
	struct KCshouweiduiying_t {
		int	m_nkcjzID;	//矿场卷轴ID
		int	m_nswID;	//守卫ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 NPC技能联系 ID = 31
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum NPCskillscontact_Fields {
		NPCskillscontact_NPCID = 0,	//NPCID
		NPCskillscontact_SlillsID = 1,	//SkillsID
		NPCskillscontact_releaseprobability = 2,	//释放概率
		NPCskillscontact_Max = 3
	};
	// 联系数据结构
	struct NPCskillscontact_t {
		int	m_nNPCID;	//NPCID
		int	m_nSlillsID;	//SkillsID
		int	m_nreleaseprobability;	//释放概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 怪物AI ID = 32
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum NPCAI_Fields {
		NPCAI_AIid = 0,	//AI编号
		NPCAI_suijiyidong = 1,	//随机移动
		NPCAI_zhudonggongji = 2,	//主动攻击
		NPCAI_bangzhutonglei = 3,	//帮助同类
		NPCAI_tiaozhengmubiao = 4,	//调整目标
		NPCAI_taopao = 5,	//逃跑
		NPCAI_taopaogailv = 6,	//逃跑概率
		NPCAI_dili = 7,	//底力
		NPCAI_beishanghai = 8,	//不动时可被伤害
		NPCAI_yuanchenggongji = 9,	//远程攻击
		NPCAI_zibao = 10,	//自爆
		NPCAI_zhaohuan = 11,	//召唤
		NPCAI_beizhaohuan = 12,	//被召唤
		NPCAI_huixue = 13,	//自动恢复生命
		NPCAI_shunyi = 14,	//瞬移
		NPCAI_Max = 15
	};
	// 联系数据结构
	struct NPCAI_t {
		int	m_nAIid;	//AI编号
		BOOL	m_bsuijiyidong;	//随机移动
		BOOL	m_bzhudonggongji;	//主动攻击
		BOOL	m_bbangzhutonglei;	//帮助同类
		BOOL	m_btiaozhengmubiao;	//调整目标
		BOOL	m_btaopao;	//逃跑
		int	m_ntaopaogailv;	//逃跑概率
		BOOL	m_bdili;	//底力
		BOOL	m_bbeishanghai;	//不动时可被伤害
		BOOL	m_byuanchenggongji;	//远程攻击
		BOOL	m_bzibao;	//自爆
		BOOL	m_bzhaohuan;	//召唤
		BOOL	m_bbeizhaohuan;	//被召唤
		BOOL	m_bhuixue;	//自动恢复生命
		BOOL	m_bshunyi;	//瞬移
	};
	//////////////////////////////////////////////////////////////
	// 联系 精练掉落 ID = 33
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum jingliandrop_Fields {
		jingliandrop_room = 0,	//从属区间
		jingliandrop_itemnum = 1,	//物品编号
		jingliandrop_dorpclass = 2,	//掉落类别
		jingliandrop_dropprobability = 3,	//掉落概率
		jingliandrop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum jingliandrop_dorpclass_Enum {
		jingliandrop_dorpclass_0,	//
		jingliandrop_dorpclassMax = 1
	};
	// 联系数据结构
	struct jingliandrop_t {
		int	m_nroom;	//从属区间
		int	m_nitemnum;	//物品编号
		jingliandrop_dorpclass_Enum	m_edorpclass;	//掉落类别
		int	m_ndropprobability;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 宝石掉落 ID = 34
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gemdrop_Fields {
		gemdrop_room = 0,	//从属区间
		gemdrop_itemnum = 1,	//物品编号
		gemdrop_dorpclass = 2,	//掉落类别
		gemdrop_dropprobability = 3,	//掉落概率
		gemdrop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum gemdrop_dorpclass_Enum {
		gemdrop_dorpclass_0,	//
		gemdrop_dorpclassMax = 1
	};
	// 联系数据结构
	struct gemdrop_t {
		int	m_nroom;	//从属区间
		int	m_nitemnum;	//物品编号
		gemdrop_dorpclass_Enum	m_edorpclass;	//掉落类别
		int	m_ndropprobability;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 道具掉落 ID = 35
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum itemdrop_Fields {
		itemdrop_room = 0,	//从属区间
		itemdrop_itemnum = 1,	//物品编号
		itemdrop_dorpclass = 2,	//掉落类别
		itemdrop_dropprobability = 3,	//掉落概率
		itemdrop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum itemdrop_dorpclass_Enum {
		itemdrop_dorpclass_0,	//
		itemdrop_dorpclassMax = 1
	};
	// 联系数据结构
	struct itemdrop_t {
		int	m_nroom;	//从属区间
		int	m_nitemnum;	//物品编号
		itemdrop_dorpclass_Enum	m_edorpclass;	//掉落类别
		int	m_ndropprobability;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 锻造掉落 ID = 36
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum smithingdrop_Fields {
		smithingdrop_room = 0,	//从属区间
		smithingdrop_itemnum = 1,	//物品编号
		smithingdrop_dorpclass = 2,	//掉落类别
		smithingdrop_dropprobability = 3,	//掉落概率
		smithingdrop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum smithingdrop_dorpclass_Enum {
		smithingdrop_dorpclass_0,	//
		smithingdrop_dorpclassMax = 1
	};
	// 联系数据结构
	struct smithingdrop_t {
		int	m_nroom;	//从属区间
		int	m_nitemnum;	//物品编号
		smithingdrop_dorpclass_Enum	m_edorpclass;	//掉落类别
		int	m_ndropprobability;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 传送列表 ID = 37
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum maptransmissionlist_Fields {
		maptransmissionlist_mapID = 0,	//所属地图
		maptransmissionlist_mapname = 1,	//地图名称
		maptransmissionlist_x = 2,	//x
		maptransmissionlist_y = 3,	//y
		maptransmissionlist_w = 4,	//w
		maptransmissionlist_h = 5,	//h
		maptransmissionlist_Max = 6
	};
	// 联系数据结构
	struct maptransmissionlist_t {
		int	m_nmapID;	//所属地图
		char	m_szmapname[16];	//地图名称
		int	m_nx;	//x
		int	m_ny;	//y
		int	m_nw;	//w
		int	m_nh;	//h
	};
	//////////////////////////////////////////////////////////////
	// 联系 传送价格表 ID = 38
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum transmissionprice_Fields {
		transmissionprice_mapid = 0,	//地图编号
		transmissionprice_mapname = 1,	//地图名称
		transmissionprice_targetmapid = 2,	//目标地图编号
		transmissionprice_targetmapname = 3,	//目标地图名称
		transmissionprice_price = 4,	//价格
		transmissionprice_Max = 5
	};
	// 联系数据结构
	struct transmissionprice_t {
		int	m_nmapid;	//地图编号
		char	m_szmapname[16];	//地图名称
		int	m_ntargetmapid;	//目标地图编号
		char	m_sztargetmapname[16];	//目标地图名称
		int	m_nprice;	//价格
	};
	//////////////////////////////////////////////////////////////
	// 联系 单前缀法宝生成 ID = 39
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum fbmake_Fields {
		fbmake_odds = 0,	//几率
		fbmake_square = 1,	//自乘
		fbmake_Max = 2
	};
	// 联系数据结构
	struct fbmake_t {
		int	m_nodds;	//几率
		int	m_nsquare;	//自乘
	};
	//////////////////////////////////////////////////////////////
	// 联系 双前缀法宝生成 ID = 40
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum doublefbmake_Fields {
		doublefbmake_odds = 0,	//几率
		doublefbmake_square = 1,	//自乘
		doublefbmake_othersquare = 2,	//他乘
		doublefbmake_Max = 3
	};
	// 联系数据结构
	struct doublefbmake_t {
		int	m_nodds;	//几率
		int	m_nsquare;	//自乘
		int	m_nothersquare;	//他乘
	};
	//////////////////////////////////////////////////////////////
	// 联系 变形模型 ID = 41
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum LurkShape_Fields {
		LurkShape_MapID = 0,	//地图ID
		LurkShape_ModelFile = 1,	//模型名
		LurkShape_Max = 2
	};
	// 联系数据结构
	struct LurkShape_t {
		int	m_nMapID;	//地图ID
		char	m_szModelFile[256];	//模型名
	};
	//////////////////////////////////////////////////////////////
	// 联系 技能特殊百分比 ID = 42
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum skillspecialpercent_Fields {
		skillspecialpercent_signname = 0,	//标记名称
		skillspecialpercent_signlv = 1,	//标记等级
		skillspecialpercent_percent = 2,	//百分比
		skillspecialpercent_Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum skillspecialpercent_signname_Enum {
		skillspecialpercent_signname_0,	//
		skillspecialpercent_signname_1,	//
		skillspecialpercent_signnameMax = 2
	};
	// 联系数据结构
	struct skillspecialpercent_t {
		skillspecialpercent_signname_Enum	m_esignname;	//标记名称
		int	m_nsignlv;	//标记等级
		int	m_npercent;	//百分比
	};
	//////////////////////////////////////////////////////////////
	// 联系 地图寻路 ID = 43
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum mapway_Fields {
		mapway_Max = 0
	};
	// 联系数据结构
	struct mapway_t {
	};
	//////////////////////////////////////////////////////////////
	// 联系 精炼部位联系表 ID = 44
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum jinglianlianxibiao_Fields {
		jinglianlianxibiao_part = 0,	//部位
		jinglianlianxibiao_equippart = 1,	//对应装备部位
		jinglianlianxibiao_Max = 2
	};
	// 属性域 (null) 枚举值定义
	enum jinglianlianxibiao_part_Enum {
		jinglianlianxibiao_part_0,	//
		jinglianlianxibiao_part_1,	//
		jinglianlianxibiao_part_2,	//
		jinglianlianxibiao_part_3,	//
		jinglianlianxibiao_partMax = 4
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct jinglianlianxibiao_t {
		jinglianlianxibiao_part_Enum	m_epart;	//部位
		jinglianlianxibiao_equippart_Enum	m_eequippart;	//对应装备部位
	};
	//////////////////////////////////////////////////////////////
	// 联系 装备生成 ID = 45
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum equipmake_Fields {
		equipmake_ID = 0,	//职业
		equipmake_pinzhi = 1,	//品质
		equipmake_jiazhi = 2,	//加值
		equipmake_wumian = 3,	//物免
		equipmake_momian = 4,	//魔免
		equipmake_pingjunshangmian = 5,	//平均伤免
		equipmake_kanzhefangbian = 6,	//看着方便
		equipmake_Max = 7
	};
	// 属性域 (null) 枚举值定义
	enum equipmake_ID_Enum {
		equipmake_ID_0,	//
		equipmake_ID_1,	//
		equipmake_ID_2,	//
		equipmake_ID_3,	//
		equipmake_ID_4,	//
		equipmake_IDMax = 5
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct equipmake_t {
		equipmake_ID_Enum	m_eID;	//职业
		equipmake_pinzhi_Enum	m_epinzhi;	//品质
		int	m_njiazhi;	//加值
		float	m_fwumian;	//物免
		float	m_fmomian;	//魔免
		float	m_fpingjunshangmian;	//平均伤免
		char	m_szkanzhefangbian[16];	//看着方便
	};
	//////////////////////////////////////////////////////////////
	// 联系 部位系数表 ID = 47
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum buweixishu_Fields {
		buweixishu_buwei = 0,	//部位
		buweixishu_buweixishu = 1,	//部位系数
		buweixishu_pricexishu = 2,	//价格系数
		buweixishu_Max = 3
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct buweixishu_t {
		buweixishu_buwei_Enum	m_ebuwei;	//部位
		int	m_nbuweixishu;	//部位系数
		float	m_fpricexishu;	//价格系数
	};
	//////////////////////////////////////////////////////////////
	// 联系 随机属性对照表 ID = 48
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum sjsxxishubiao_Fields {
		sjsxxishubiao_id = 0,	//ID
		sjsxxishubiao_class = 1,	//职业
		sjsxxishubiao_equiplv = 2,	//物品等级下限
		sjsxxishubiao_addvalue = 3,	//加值
		sjsxxishubiao_strmin = 4,	//力量下限
		sjsxxishubiao_strmax = 5,	//力量上限
		sjsxxishubiao_dexmin = 6,	//敏捷下限
		sjsxxishubiao_dexmax = 7,	//敏捷上限
		sjsxxishubiao_conmin = 8,	//体质下限
		sjsxxishubiao_conmax = 9,	//体质上限
		sjsxxishubiao_powermin = 10,	//灵力下限
		sjsxxishubiao_powermax = 11,	//灵力上限
		sjsxxishubiao_intmin = 12,	//智慧下限
		sjsxxishubiao_intmax = 13,	//智慧上限
		sjsxxishubiao_minattmin = 14,	//最小物攻下限
		sjsxxishubiao_minattmax = 15,	//最小物攻上限
		sjsxxishubiao_maxattmin = 16,	//最大物攻下限
		sjsxxishubiao_maxattmax = 17,	//最大物攻上限
		sjsxxishubiao_defmin = 18,	//物防下限
		sjsxxishubiao_defmax = 19,	//物防上限
		sjsxxishubiao_mdefmin = 20,	//法防下限
		sjsxxishubiao_mdefmax = 21,	//法防上限
		sjsxxishubiao_mattmin = 22,	//法伤下限
		sjsxxishubiao_mattmax = 23,	//法伤上限
		sjsxxishubiao_hpmin = 24,	//最大生命值下限
		sjsxxishubiao_homax = 25,	//最大生命值上限
		sjsxxishubiao_mpmin = 26,	//最大法力值下限
		sjsxxishubiao_mpmax = 27,	//最大法力值上限
		sjsxxishubiao_hitmin = 28,	//命中下限
		sjsxxishubiao_hitmax = 29,	//命中上限
		sjsxxishubiao_dodgemin = 30,	//闪躲下限
		sjsxxishubiao_dodgemax = 31,	//闪躲上限
		sjsxxishubiao_phycrmin = 32,	//物爆下限
		sjsxxishubiao_phycrmax = 33,	//物爆上限
		sjsxxishubiao_skicrmin = 34,	//技爆下限
		sjsxxishubiao_skicrmax = 35,	//技爆上限
		sjsxxishubiao_hpremin = 36,	//生命恢复速度下限
		sjsxxishubiao_hpremax = 37,	//生命恢复速度上限
		sjsxxishubiao_mpremin = 38,	//法力恢复速度下限
		sjsxxishubiao_moremax = 39,	//法力恢复速度上限
		sjsxxishubiao_rpmin = 40,	//掉宝下限
		sjsxxishubiao_rpmax = 41,	//掉宝上限
		sjsxxishubiao_Max = 42
	};
	// 属性域 (null) 枚举值定义
	enum sjsxxishubiao_class_Enum {
		sjsxxishubiao_class_0,	//
		sjsxxishubiao_class_1,	//
		sjsxxishubiao_class_2,	//
		sjsxxishubiao_class_3,	//
		sjsxxishubiao_classMax = 4
	};
	// 联系数据结构
	struct sjsxxishubiao_t {
		int	m_nid;	//ID
		sjsxxishubiao_class_Enum	m_eclass;	//职业
		int	m_nequiplv;	//物品等级下限
		int	m_naddvalue;	//加值
		int	m_nstrmin;	//力量下限
		int	m_nstrmax;	//力量上限
		int	m_ndexmin;	//敏捷下限
		int	m_ndexmax;	//敏捷上限
		int	m_nconmin;	//体质下限
		int	m_nconmax;	//体质上限
		int	m_npowermin;	//灵力下限
		int	m_npowermax;	//灵力上限
		int	m_nintmin;	//智慧下限
		int	m_nintmax;	//智慧上限
		int	m_nminattmin;	//最小物攻下限
		int	m_nminattmax;	//最小物攻上限
		int	m_nmaxattmin;	//最大物攻下限
		int	m_nmaxattmax;	//最大物攻上限
		int	m_ndefmin;	//物防下限
		int	m_ndefmax;	//物防上限
		int	m_nmdefmin;	//法防下限
		int	m_nmdefmax;	//法防上限
		int	m_nmattmin;	//法伤下限
		int	m_nmattmax;	//法伤上限
		int	m_nhpmin;	//最大生命值下限
		int	m_nhomax;	//最大生命值上限
		int	m_nmpmin;	//最大法力值下限
		int	m_nmpmax;	//最大法力值上限
		int	m_nhitmin;	//命中下限
		int	m_nhitmax;	//命中上限
		int	m_ndodgemin;	//闪躲下限
		int	m_ndodgemax;	//闪躲上限
		int	m_nphycrmin;	//物爆下限
		int	m_nphycrmax;	//物爆上限
		int	m_nskicrmin;	//技爆下限
		int	m_nskicrmax;	//技爆上限
		int	m_nhpremin;	//生命恢复速度下限
		int	m_nhpremax;	//生命恢复速度上限
		int	m_nmpremin;	//法力恢复速度下限
		int	m_nmoremax;	//法力恢复速度上限
		int	m_nrpmin;	//掉宝下限
		int	m_nrpmax;	//掉宝上限
	};
	//////////////////////////////////////////////////////////////
	// 联系 固定属性对照表 ID = 49
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum gdsxduizhaobiao_Fields {
		gdsxduizhaobiao_part = 0,	//部位
		gdsxduizhaobiao_job = 1,	//职业
		gdsxduizhaobiao_shuxingID = 2,	//属性ID
		gdsxduizhaobiao_Max = 3
	};
	// 属性域 (null) 枚举值定义
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
	// 属性域 (null) 枚举值定义
	enum gdsxduizhaobiao_job_Enum {
		gdsxduizhaobiao_job_0,	//
		gdsxduizhaobiao_job_1,	//
		gdsxduizhaobiao_job_2,	//
		gdsxduizhaobiao_job_3,	//
		gdsxduizhaobiao_job_4,	//
		gdsxduizhaobiao_jobMax = 5
	};
	// 联系数据结构
	struct gdsxduizhaobiao_t {
		gdsxduizhaobiao_part_Enum	m_epart;	//部位
		gdsxduizhaobiao_job_Enum	m_ejob;	//职业
		int	m_nshuxingID;	//属性ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 可预览列表 ID = 50
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum canpreviewlist_Fields {
		canpreviewlist_cellname = 0,	//单元名称
		canpreviewlist_cellid = 1,	//单元ID
		canpreviewlist_kindid = 2,	//子类ID
		canpreviewlist_Max = 3
	};
	// 联系数据结构
	struct canpreviewlist_t {
		char	m_szcellname[16];	//单元名称
		int	m_ncellid;	//单元ID
		int	m_nkindid;	//子类ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 礼包联系表 ID = 51
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum bagcontactlist_Fields {
		bagcontactlist_bagid = 0,	//礼包ID
		bagcontactlist_goodscell = 1,	//货物单元
		bagcontactlist_goodsid = 2,	//货物ID
		bagcontactlist_goodsnumber = 3,	//货物数量
		bagcontactlist_appearprobability = 4,	//出现概率
		bagcontactlist_Max = 5
	};
	// 联系数据结构
	struct bagcontactlist_t {
		int	m_nbagid;	//礼包ID
		int	m_ngoodscell;	//货物单元
		int	m_ngoodsid;	//货物ID
		int	m_ngoodsnumber;	//货物数量
		int	m_nappearprobability;	//出现概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 稀有随机属性对照表 ID = 52
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum xysjsxlist_Fields {
		xysjsxlist_id = 0,	//ID
		xysjsxlist_buwei = 1,	//部位
		xysjsxlist_strmin = 2,	//力量下限
		xysjsxlist_strmax = 3,	//力量上限
		xysjsxlist_dexmin = 4,	//敏捷下限
		xysjsxlist_dexmax = 5,	//敏捷上限
		xysjsxlist_poewrmin = 6,	//灵力下限
		xysjsxlist_powermax = 7,	//灵力上限
		xysjsxlist_wismin = 8,	//智慧下限
		xysjsxlist_wismax = 9,	//智慧上限
		xysjsxlist_hpmin = 10,	//最大生命值下限
		xysjsxlist_hpmax = 11,	//最大生命值上限
		xysjsxlist_mpmin = 12,	//最大法力值下限
		xysjsxlist_mpmax = 13,	//最大法力值上限
		xysjsxlist_shjmmin = 14,	//伤害减免下限
		xysjsxlist_shjmmax = 15,	//伤害减免上限
		xysjsxlist_conmin = 16,	//体质下限
		xysjsxlist_conmax = 17,	//体质上限
		xysjsxlist_defmin = 18,	//物理防御力下限
		xysjsxlist_defmax = 19,	//物理防御力上限
		xysjsxlist_mdefmin = 20,	//法术防御力下限
		xysjsxlist_mdefmax = 21,	//法术防御力上限
		xysjsxlist_shftmin = 22,	//伤害反弹下限
		xysjsxlist_shftmax = 23,	//伤害反弹上限
		xysjsxlist_atkmin = 24,	//最大攻击力下限
		xysjsxlist_atkmax = 25,	//最大攻击力上限
		xysjsxlist_matkmin = 26,	//法伤下限
		xysjsxlist_matkmax = 27,	//法伤上限
		xysjsxlist_Max = 28
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct xysjsxlist_t {
		int	m_nid;	//ID
		xysjsxlist_buwei_Enum	m_ebuwei;	//部位
		int	m_nstrmin;	//力量下限
		int	m_nstrmax;	//力量上限
		int	m_ndexmin;	//敏捷下限
		int	m_ndexmax;	//敏捷上限
		int	m_npoewrmin;	//灵力下限
		int	m_npowermax;	//灵力上限
		int	m_nwismin;	//智慧下限
		int	m_nwismax;	//智慧上限
		int	m_nhpmin;	//最大生命值下限
		int	m_nhpmax;	//最大生命值上限
		int	m_nmpmin;	//最大法力值下限
		int	m_nmpmax;	//最大法力值上限
		int	m_nshjmmin;	//伤害减免下限
		int	m_nshjmmax;	//伤害减免上限
		int	m_nconmin;	//体质下限
		int	m_nconmax;	//体质上限
		int	m_ndefmin;	//物理防御力下限
		int	m_ndefmax;	//物理防御力上限
		int	m_nmdefmin;	//法术防御力下限
		int	m_nmdefmax;	//法术防御力上限
		int	m_nshftmin;	//伤害反弹下限
		int	m_nshftmax;	//伤害反弹上限
		int	m_natkmin;	//最大攻击力下限
		int	m_natkmax;	//最大攻击力上限
		int	m_nmatkmin;	//法伤下限
		int	m_nmatkmax;	//法伤上限
	};
	//////////////////////////////////////////////////////////////
	// 联系 器灵 ID = 53
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum smeltstone_Fields {
		smeltstone_character = 0,	//品质
		smeltstone_equiplv = 1,	//物品等级
		smeltstone_goodsid = 2,	//物品编号
		smeltstone_fusionprice = 3,	//熔炼价格
		smeltstone_composenumber = 4,	//合成数
		smeltstone_composeid = 5,	//合成编号
		smeltstone_fuseprice = 6,	//融合价格
		smeltstone_Max = 7
	};
	// 属性域 (null) 枚举值定义
	enum smeltstone_character_Enum {
		smeltstone_character_0,	//
		smeltstone_character_1,	//
		smeltstone_character_2,	//
		smeltstone_character_3,	//
		smeltstone_character_4,	//
		smeltstone_character_5,	//
		smeltstone_characterMax = 6
	};
	// 联系数据结构
	struct smeltstone_t {
		smeltstone_character_Enum	m_echaracter;	//品质
		int	m_nequiplv;	//物品等级
		int	m_ngoodsid;	//物品编号
		int	m_nfusionprice;	//熔炼价格
		int	m_ncomposenumber;	//合成数
		int	m_ncomposeid;	//合成编号
		int	m_nfuseprice;	//融合价格
	};
	//////////////////////////////////////////////////////////////
	// 联系 器灵升级对照表 ID = 54
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum stonelvuplist_Fields {
		stonelvuplist_lvuotype = 0,	//提升类型
		stonelvuplist_id = 1,	//ID
		stonelvuplist_num = 2,	//需要数量
		stonelvuplist_scid = 3,	//生成ID
		stonelvuplist_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum stonelvuplist_lvuotype_Enum {
		stonelvuplist_lvuotype_0,	//
		stonelvuplist_lvuotype_1,	//
		stonelvuplist_lvuotypeMax = 2
	};
	// 联系数据结构
	struct stonelvuplist_t {
		stonelvuplist_lvuotype_Enum	m_elvuotype;	//提升类型
		int	m_nid;	//ID
		int	m_nnum;	//需要数量
		int	m_nscid;	//生成ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 地图内传送列表 ID = 55
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum dtncslist_Fields {
		dtncslist_mapid = 0,	//地图ID
		dtncslist_id = 1,	//传送点ID
		dtncslist_name = 2,	//传送点名称
		dtncslist_x = 3,	//坐标X
		dtncslist_y = 4,	//坐标Y
		dtncslist_Max = 5
	};
	// 联系数据结构
	struct dtncslist_t {
		int	m_nmapid;	//地图ID
		int	m_nid;	//传送点ID
		char	m_szname[16];	//传送点名称
		int	m_nx;	//坐标X
		int	m_ny;	//坐标Y
	};
	//////////////////////////////////////////////////////////////
	// 联系 出矿分布表 ID = 56
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum resourcelist_Fields {
		resourcelist_day = 0,	//星期
		resourcelist_hour = 1,	//钟点
		resourcelist_frequency = 2,	//出产频率
		resourcelist_magnification = 3,	//出产倍率
		resourcelist_Max = 4
	};
	// 联系数据结构
	struct resourcelist_t {
		int	m_nday;	//星期
		int	m_nhour;	//钟点
		int	m_nfrequency;	//出产频率
		int	m_nmagnification;	//出产倍率
	};
	//////////////////////////////////////////////////////////////
	// 联系 日常任务等级区间 ID = 57
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum richangrenwulvarea_Fields {
		richangrenwulvarea_Max = 0
	};
	// 联系数据结构
	struct richangrenwulvarea_t {
	};
	//////////////////////////////////////////////////////////////
	// 联系 随机属性到角色属性映射表 ID = 58
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum sjsxdjssxysb_Fields {
		sjsxdjssxysb_kinds = 0,	//种类
		sjsxdjssxysb_id = 1,	//ID
		sjsxdjssxysb_target = 2,	//目标
		sjsxdjssxysb_Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum sjsxdjssxysb_kinds_Enum {
		sjsxdjssxysb_kinds_0,	//
		sjsxdjssxysb_kinds_1,	//
		sjsxdjssxysb_kindsMax = 2
	};
	// 联系数据结构
	struct sjsxdjssxysb_t {
		sjsxdjssxysb_kinds_Enum	m_ekinds;	//种类
		int	m_nid;	//ID
		int	m_ntarget;	//目标
	};
	//////////////////////////////////////////////////////////////
	// 联系 传送NPC列表 ID = 59
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum csnpclb_Fields {
		csnpclb_npcid = 0,	//npcid
		csnpclb_csdname = 1,	//传送点名称
		csnpclb_mapid = 2,	//所属地图id
		csnpclb_meishuX = 3,	//美术X
		csnpclb_meishuY = 4,	//美术Y
		csnpclb_tips = 5,	//tips
		csnpclb_buttonid = 6,	//按钮id
		csnpclb_Max = 7
	};
	// 联系数据结构
	struct csnpclb_t {
		int	m_nnpcid;	//npcid
		char	m_szcsdname[16];	//传送点名称
		int	m_nmapid;	//所属地图id
		int	m_nmeishuX;	//美术X
		int	m_nmeishuY;	//美术Y
		char	m_sztips[128];	//tips
		int	m_nbuttonid;	//按钮id
	};
	//////////////////////////////////////////////////////////////
	// 联系 任务类型表 ID = 60
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum roletypelist_Fields {
		roletypelist_roletypeid = 0,	//任务类型ID
		roletypelist_type = 1,	//类型
		roletypelist_zqqs = 2,	//周期起始点
		roletypelist_zqsc = 3,	//周期时长
		roletypelist_kjsdqs = 4,	//可接时段起始
		roletypelist_kjsdzj = 5,	//可接时点终结
		roletypelist_ls = 6,	//链数
		roletypelist_hs = 7,	//环数
		roletypelist_jfzdz = 8,	//积分最大值
		roletypelist_Max = 9
	};
	// 属性域 (null) 枚举值定义
	enum roletypelist_type_Enum {
		roletypelist_type_0,	//
		roletypelist_type_1,	//
		roletypelist_type_2,	//
		roletypelist_typeMax = 3
	};
	// 联系数据结构
	struct roletypelist_t {
		int	m_nroletypeid;	//任务类型ID
		roletypelist_type_Enum	m_etype;	//类型
		int	m_nzqqs;	//周期起始点
		int	m_nzqsc;	//周期时长
		int	m_nkjsdqs;	//可接时段起始
		int	m_nkjsdzj;	//可接时点终结
		int	m_nls;	//链数
		int	m_nhs;	//环数
		int	m_njfzdz;	//积分最大值
	};
	//////////////////////////////////////////////////////////////
	// 联系 任务等级区间和对应库 ID = 61
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum rolelvareaanddepotid_Fields {
		rolelvareaanddepotid_roletypeid = 0,	//任务类型ID
		rolelvareaanddepotid_lvmin = 1,	//等级下限
		rolelvareaanddepotid_lvmax = 2,	//等级上限
		rolelvareaanddepotid_roledepotid = 3,	//任务库ID
		rolelvareaanddepotid_Max = 4
	};
	// 联系数据结构
	struct rolelvareaanddepotid_t {
		int	m_nroletypeid;	//任务类型ID
		int	m_nlvmin;	//等级下限
		int	m_nlvmax;	//等级上限
		int	m_nroledepotid;	//任务库ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 库和任务对应表 ID = 62
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum depotidandroleidlist_Fields {
		depotidandroleidlist_roletypeid = 0,	//任务库ID
		depotidandroleidlist_roleid = 1,	//任务ID
		depotidandroleidlist_Max = 2
	};
	// 联系数据结构
	struct depotidandroleidlist_t {
		int	m_nroletypeid;	//任务库ID
		int	m_nroleid;	//任务ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 可接任务列表 ID = 63
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum replacerolelist_Fields {
		replacerolelist_roletypeid = 0,	//任务类型ID
		replacerolelist_option = 1,	//项数
		replacerolelist_oddswhite = 2,	//几率白
		replacerolelist_oddsgreen = 3,	//几率绿
		replacerolelist_oddsbule = 4,	//几率蓝
		replacerolelist_oddspurple = 5,	//几率紫
		replacerolelist_oddsorange = 6,	//几率橙
		replacerolelist_Max = 7
	};
	// 联系数据结构
	struct replacerolelist_t {
		int	m_nroletypeid;	//任务类型ID
		int	m_noption;	//项数
		int	m_noddswhite;	//几率白
		int	m_noddsgreen;	//几率绿
		int	m_noddsbule;	//几率蓝
		int	m_noddspurple;	//几率紫
		int	m_noddsorange;	//几率橙
	};
	//////////////////////////////////////////////////////////////
	// 联系 积分奖励表 ID = 64
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum integralaward_Fields {
		integralaward_rolekind = 0,	//任务种类
		integralaward_lvmin = 1,	//等级下限
		integralaward_lvmax = 2,	//等级上限
		integralaward_integralmax = 3,	//积分最大值
		integralaward_integralvalueone = 4,	//积分奖励值一
		integralaward_integralawardonekind = 5,	//积分奖品一种类
		integralaward_integralawardoneid = 6,	//积分奖品一ID
		integralaward_integralawardonenum = 7,	//积分奖品一数量
		integralaward_integralvaluetwo = 8,	//积分奖励值二
		integralaward_integralawardtwokind = 9,	//积分奖品二种类
		integralaward_integralawardtwoid = 10,	//积分奖品二ID
		integralaward_integralawardtwonum = 11,	//积分奖品二数量
		integralaward_integralvaluethree = 12,	//积分奖励值三
		integralaward_integralawardthreekind = 13,	//积分奖品三种类
		integralaward_integralawardthreeid = 14,	//积分奖品三ID
		integralaward_integralawardthreenum = 15,	//积分奖品三数量
		integralaward_integralvaluefour = 16,	//积分奖励值四
		integralaward_integralawardfourkind = 17,	//积分奖品四种类
		integralaward_integralawardfourid = 18,	//积分奖品四ID
		integralaward_integralawardfournum = 19,	//积分奖品四数量
		integralaward_integralvaluefive = 20,	//积分奖励值五
		integralaward_integralawardfivekind = 21,	//积分奖品五种类
		integralaward_integralawardfiveid = 22,	//积分奖品五ID
		integralaward_integralawardfivenum = 23,	//积分奖品五数量
		integralaward_Max = 24
	};
	// 联系数据结构
	struct integralaward_t {
		int	m_nrolekind;	//任务种类
		int	m_nlvmin;	//等级下限
		int	m_nlvmax;	//等级上限
		int	m_nintegralmax;	//积分最大值
		int	m_nintegralvalueone;	//积分奖励值一
		int	m_nintegralawardonekind;	//积分奖品一种类
		int	m_nintegralawardoneid;	//积分奖品一ID
		int	m_nintegralawardonenum;	//积分奖品一数量
		int	m_nintegralvaluetwo;	//积分奖励值二
		int	m_nintegralawardtwokind;	//积分奖品二种类
		int	m_nintegralawardtwoid;	//积分奖品二ID
		int	m_nintegralawardtwonum;	//积分奖品二数量
		int	m_nintegralvaluethree;	//积分奖励值三
		int	m_nintegralawardthreekind;	//积分奖品三种类
		int	m_nintegralawardthreeid;	//积分奖品三ID
		int	m_nintegralawardthreenum;	//积分奖品三数量
		int	m_nintegralvaluefour;	//积分奖励值四
		int	m_nintegralawardfourkind;	//积分奖品四种类
		int	m_nintegralawardfourid;	//积分奖品四ID
		int	m_nintegralawardfournum;	//积分奖品四数量
		int	m_nintegralvaluefive;	//积分奖励值五
		int	m_nintegralawardfivekind;	//积分奖品五种类
		int	m_nintegralawardfiveid;	//积分奖品五ID
		int	m_nintegralawardfivenum;	//积分奖品五数量
	};
	//////////////////////////////////////////////////////////////
	// 联系 师徒奖励表 ID = 65
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum shitujianglilist_Fields {
		shitujianglilist_lv = 0,	//徒弟等级
		shitujianglilist_gdjfjl = 1,	//功德积分奖励
		shitujianglilist_Max = 2
	};
	// 联系数据结构
	struct shitujianglilist_t {
		int	m_nlv;	//徒弟等级
		int	m_ngdjfjl;	//功德积分奖励
	};
	//////////////////////////////////////////////////////////////
	// 联系 结拜亲密度 ID = 66
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum jiebaiqinmidu_Fields {
		jiebaiqinmidu_id = 0,	//ID
		jiebaiqinmidu_name = 1,	//名称
		jiebaiqinmidu_lvupqinmidu = 2,	//升级所需亲密度
		jiebaiqinmidu_qingyidu = 3,	//情谊度
		jiebaiqinmidu_Max = 4
	};
	// 联系数据结构
	struct jiebaiqinmidu_t {
		int	m_nid;	//ID
		char	m_szname[16];	//名称
		int	m_nlvupqinmidu;	//升级所需亲密度
		int	m_nqingyidu;	//情谊度
	};
	//////////////////////////////////////////////////////////////
	// 联系 辉光档次 ID = 67
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum huiguangdangci_Fields {
		huiguangdangci_id = 0,	//ID
		huiguangdangci_houdu = 1,	//厚度
		huiguangdangci_qiangdu = 2,	//强度
		huiguangdangci_Max = 3
	};
	// 联系数据结构
	struct huiguangdangci_t {
		int	m_nid;	//ID
		float	m_fhoudu;	//厚度
		float	m_fqiangdu;	//强度
	};
	//////////////////////////////////////////////////////////////
	// 联系 辉光颜色 ID = 68
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum huiguangyanse_Fields {
		huiguangyanse_id = 0,	//id
		huiguangyanse_rgb = 1,	//RGB
		huiguangyanse_Max = 2
	};
	// 联系数据结构
	struct huiguangyanse_t {
		int	m_nid;	//id
		char	m_szrgb[16];	//RGB
	};
	//////////////////////////////////////////////////////////////
	// 联系 新手地图转跳 ID = 69
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum xinshouzhuantiao_Fields {
		xinshouzhuantiao_maptiaozhuan = 0,	//地图转跳
		xinshouzhuantiao_mapid = 1,	//地图ID
		xinshouzhuantiao_zhiye = 2,	//职业
		xinshouzhuantiao_x = 3,	//X坐标
		xinshouzhuantiao_y = 4,	//Y坐标
		xinshouzhuantiao_Max = 5
	};
	// 属性域 (null) 枚举值定义
	enum xinshouzhuantiao_maptiaozhuan_Enum {
		xinshouzhuantiao_maptiaozhuan_0,	//
		xinshouzhuantiao_maptiaozhuan_1,	//
		xinshouzhuantiao_maptiaozhuanMax = 2
	};
	// 属性域 (null) 枚举值定义
	enum xinshouzhuantiao_zhiye_Enum {
		xinshouzhuantiao_zhiye_0,	//
		xinshouzhuantiao_zhiye_1,	//
		xinshouzhuantiao_zhiye_2,	//
		xinshouzhuantiao_zhiye_3,	//
		xinshouzhuantiao_zhiyeMax = 4
	};
	// 联系数据结构
	struct xinshouzhuantiao_t {
		xinshouzhuantiao_maptiaozhuan_Enum	m_emaptiaozhuan;	//地图转跳
		int	m_nmapid;	//地图ID
		xinshouzhuantiao_zhiye_Enum	m_ezhiye;	//职业
		int	m_nx;	//X坐标
		int	m_ny;	//Y坐标
	};
	//////////////////////////////////////////////////////////////
	// 联系 副本BOSS标记 ID = 70
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum fubenboss_Fields {
		fubenboss_bossid = 0,	//bossid
		fubenboss_mapid = 1,	//所属地图id
		fubenboss_meishuidX = 2,	//美术X
		fubenboss_meishuidY = 3,	//美术Y
		fubenboss_tips = 4,	//tips
		fubenboss_buttonid = 5,	//按钮id
		fubenboss_Max = 6
	};
	// 联系数据结构
	struct fubenboss_t {
		int	m_nbossid;	//bossid
		int	m_nmapid;	//所属地图id
		int	m_nmeishuidX;	//美术X
		int	m_nmeishuidY;	//美术Y
		char	m_sztips[128];	//tips
		int	m_nbuttonid;	//按钮id
	};
	//////////////////////////////////////////////////////////////
	// 联系 NPC自动寻路分类 ID = 71
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum npczidongxunlufenlei_Fields {
		npczidongxunlufenlei_npcleixing = 0,	//NPC类型
		npczidongxunlufenlei_npczhiye = 1,	//NPC职业
		npczidongxunlufenlei_Max = 2
	};
	// 属性域 (null) 枚举值定义
	enum npczidongxunlufenlei_npcleixing_Enum {
		npczidongxunlufenlei_npcleixing_0,	//
		npczidongxunlufenlei_npcleixing_1,	//
		npczidongxunlufenlei_npcleixing_2,	//
		npczidongxunlufenlei_npcleixing_3,	//
		npczidongxunlufenlei_npcleixing_4,	//
		npczidongxunlufenlei_npcleixing_5,	//
		npczidongxunlufenlei_npcleixingMax = 6
	};
	// 属性域 (null) 枚举值定义
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
	// 联系数据结构
	struct npczidongxunlufenlei_t {
		npczidongxunlufenlei_npcleixing_Enum	m_enpcleixing;	//NPC类型
		npczidongxunlufenlei_npczhiye_Enum	m_enpczhiye;	//NPC职业
	};
	//////////////////////////////////////////////////////////////
	// 联系 宝石合成 ID = 72
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum baoshihecheng_Fields {
		baoshihecheng_id = 0,	//ID
		baoshihecheng_scid = 1,	//生成ID
		baoshihecheng_xiaohaojinbi = 2,	//消耗金币
		baoshihecheng_xiaohaohun = 3,	//消耗魂
		baoshihecheng_Max = 4
	};
	// 联系数据结构
	struct baoshihecheng_t {
		int	m_nid;	//ID
		int	m_nscid;	//生成ID
		int	m_nxiaohaojinbi;	//消耗金币
		int	m_nxiaohaohun;	//消耗魂
	};
	//////////////////////////////////////////////////////////////
	// 联系 宠物品质 ID = 73
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petclass_Fields {
		petclass_id = 0,	//ID
		petclass_class = 1,	//品质
		petclass_gpoint = 2,	//成长值
		petclass_huoyueduxishu = 3,	//活跃度品质系数
		petclass_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum petclass_class_Enum {
		petclass_class_0,	//
		petclass_class_1,	//
		petclass_class_2,	//
		petclass_class_3,	//
		petclass_classMax = 4
	};
	// 联系数据结构
	struct petclass_t {
		int	m_nid;	//ID
		petclass_class_Enum	m_eclass;	//品质
		float	m_fgpoint;	//成长值
		float	m_fhuoyueduxishu;	//活跃度品质系数
	};
	//////////////////////////////////////////////////////////////
	// 联系 习性果实 ID = 74
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petfruit_Fields {
		petfruit_id = 0,	//ID
		petfruit_fruit = 1,	//习性果实
		petfruit_habit = 2,	//习性
		petfruit_pid = 3,	//道具ID
		petfruit_Max = 4
	};
	// 联系数据结构
	struct petfruit_t {
		int	m_nid;	//ID
		char	m_szfruit[16];	//习性果实
		char	m_szhabit[16];	//习性
		int	m_npid;	//道具ID
	};
	//////////////////////////////////////////////////////////////
	// 联系 宠物掉落 ID = 75
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petdrop_Fields {
		petdrop_room = 0,	//从属区间
		petdrop_itemnum = 1,	//物品编号
		petdrop_dropclass = 2,	//掉落类别
		petdrop_dropprobablity = 3,	//掉落概率
		petdrop_Max = 4
	};
	// 属性域 (null) 枚举值定义
	enum petdrop_dropclass_Enum {
		petdrop_dropclass_0,	//
		petdrop_dropclassMax = 1
	};
	// 联系数据结构
	struct petdrop_t {
		int	m_nroom;	//从属区间
		int	m_nitemnum;	//物品编号
		petdrop_dropclass_Enum	m_edropclass;	//掉落类别
		int	m_ndropprobablity;	//掉落概率
	};
	//////////////////////////////////////////////////////////////
	// 联系 宠物果实礼包联系表 ID = 76
	//////////////////////////////////////////////////////////////
	// 属性域枚举
	enum petfruitbagcontactlist_Fields {
		petfruitbagcontactlist_petbagid = 0,	//宠物礼包ID
		petfruitbagcontactlist_fruitclass = 1,	//果实品质
		petfruitbagcontactlist_fruitclassprobability = 2,	//果实品质概率
		petfruitbagcontactlist_Max = 3
	};
	// 属性域 (null) 枚举值定义
	enum petfruitbagcontactlist_fruitclass_Enum {
		petfruitbagcontactlist_fruitclass_0,	//
		petfruitbagcontactlist_fruitclass_1,	//
		petfruitbagcontactlist_fruitclass_2,	//
		petfruitbagcontactlist_fruitclassMax = 3
	};
	// 联系数据结构
	struct petfruitbagcontactlist_t {
		int	m_npetbagid;	//宠物礼包ID
		petfruitbagcontactlist_fruitclass_Enum	m_efruitclass;	//果实品质
		int	m_nfruitclassprobability;	//果实品质概率
	};
}
#pragma pack()
// 代码结束

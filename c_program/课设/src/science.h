#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef SCIENCE_H_INCLUDED
#define SCIENCE_H_INCLUDED

#define SCR_ROW 25             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/

#define TURE 1
#define FALSE 0

/**
 *院系基本信息链接点结构
 */
typedef struct college_node {
    char name[20];                  /*院系名称*/
    char dutyman[12];               /*负责人*/
    char tel[15];                   /*联系电话*/
    struct college_node *next;      /*指向下一结构点的指针*/
    struct team_node *tnext;        /*指向科研团队链的指针*/
} COLLEGE_NODE;

/**
 *科研团队基本信息链接点结构
 */
typedef struct team_node {
    char name[30];                  /*团队名称*/
    char dutyman[12];               /*负责人*/
    char college[20];               /*所属院系*/ 
    int num_tc;                     /*教师人数*/
    int num_stu;                    /*研究生人数*/
    struct team_node *next;         /*指向下一结构点的指针*/
    struct project_node *pnext;     /*指向科研项目链的指针*/
} TEAM_NODE;

/**
 *科研项目基本信息链接点结构
 */
typedef struct project_node {
    char num[15];                   /*项目编号*/
    char type;                      /*项目类型*/
    char start[8];                  /*起始时间*/
    float money;                    /*项目经费*/
    char dutyman[12];               /*负责人*/
    char team[30];                  /*所属团队*/
    struct project_node *next;      /*指向下一结构点的指针*/
} PROJECT_NODE;


/*
  研究生总数，教师总数及人数比链结点结构 
*/
typedef struct peoples_node{
	char *col_name; /*院系名称*/ 
	int student;   /*研究生总数*/ 
	int teacher;   /*教师总数*/ 
	float scale;     /*人数比*/ 
	struct peoples_node *next;
} PEOPLES_NODE; 



/*
  各类科研项目数链结点结构 
*/
typedef struct project_num_node{
	char *col_name; /*院系名称*/ 
	int total;     /*科研项目总数*/ 
	int p973;      /*973项目总数*/ 
	int p863;      /*863项目总数*/
	float money;    /*项目总资金*/ 
	struct project_num_node *next;
} PROJECT_NUM_NODE;

/*
  科研团队自然科学基金项目总数链结点结构 
*/
typedef struct team_nasi_node{
	char *team_name;  /*科研团队名称*/
	int num_nasi;   /*自然科学基金项目总数*/
	float money; /*项目总资金*/
	struct team_nasi_node *next; 
} TEAM_NASI_NODE; 

/*
  科研团队科研项目与教师人数比 
*/
typedef struct team_protea_node{
	char *team_name;   /*科研团队名称*/
	int num_tc;   /*教师人数*/
	int num_pro;   /*科研项目人数*/
	float scale;   /*人数比*/
	struct team_protea_node *next;
} TEAM_PROTEA_NODE;

/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;            /**< 弹出窗口层数*/
    SMALL_RECT rcArea;       /**< 弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /**< 弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;           /**< 弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next; /**< 指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< 标签字符串数组首地址*/
    COORD *pLoc;           /**< 标签定位数组首地址*/
    int num;               /**< 标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< 热区定位数组首地址*/
    char *pSort;           /**< 热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /**< 热区序号数组首地址*/
    int num;               /**< 热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/
COLLEGE_NODE *gp_head = NULL;                     /*主链头指针*/

char *gp_sys_name = "科研项目信息管理系统";    /*系统名称*/
char *gp_college_info_filename = "college.dat";        /*院系基本信息数据文件*/
char *gp_team_info_filename = "team.dat";       /*团队基本信息数据文件*/
char *gp_project_info_filename = "project.dat";      /*科研项目基本信息数据文件*/
char *gp_type_code_filename = "type.dat";        /*项目类别数据文件*/

char *ga_main_menu[] = {                       /*系统主菜单名*/
	 				    "文件(F)",
    	                "数据维护(M)",
                        "数据查询(Q)",
                        "数据统计(S)",
                        "帮助(H)"
                       };

char *ga_sub_menu[] = {
	 				   "[S] 数据保存",
					   "[B] 数据备份",
					   "[R] 数据恢复",
					   "[X] 退出 ALT+X" ,
					   "[S] 项目类别代码",
					   "", 
	 				   "[D] 院系基本信息维护",          /*系统子菜单名*/
                       "[P] 科研团队基本信息维护",
                       "[C] 科研项目基本信息维护",
                       "[D] 院系信息",
                       "[P] 科研团队基本信息",
                       "[C] 科研项目基本信息",
                       "[D] 教师与研究生的人数及人数比",
                       "[P] 科研项目总体信息",
                       "[C] 优秀科研团队",
                       "[I] 教师人均科研项目情况",
                       "[O]   ",
                       "[A] 关于..."
                      };

int ga_sub_menu_count[] = { 4, 5, 3, 5, 1};  /*各主菜单项下子菜单的个数*/
int gi_sel_menu = 1;                        /*被选中的主菜单项号,初始为1*/
int gi_sel_sub_menu = 0;                    /*被选中的子菜单项号,初始为0,表示未选中*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char *gp_scr_att = NULL;    /*存放屏幕上字符单元属性值的缓冲区*/
char *gp_type_code = NULL;    /*存放项目类别代码表的数据缓冲区*/
char gc_sys_state = '\0';   /*用来保存系统状态的字符*/

unsigned long gul_type_code_len = 0;   /*项目类型代码表长度*/

HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

int LoadCode(char *filename, char **pbuffer);  /*代码表加载*/
int CreatList(COLLEGE_NODE **phead);              /*数据链表初始化*/
void ClearScreen(void);                         /*清屏*/
void ShowMenu(void);                            /*显示菜单栏*/
void InitInterface(void);							/*初始化界面*/ 
void PopMenu(int num);                          /*显示下拉菜单*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea);  /*弹出窗口屏幕信息维护*/
void PopOff(void);                              /*关闭顶层弹出窗口*/
void DrawBox(SMALL_RECT *pRc);                /*绘制边框*/
void LocSubMenu(int num, SMALL_RECT *rc);    /*主菜单下拉菜单定位*/
void ShowState(void);                           /*显示状态栏*/
void TagMainMenu(int num);                      /*标记被选中的主菜单项*/
void TagSubMenu(int num);                       /*标记被选中的子菜单项*/

int DealInput(HOT_AREA *pHotArea, int *piHot);    /*控制台输入处理*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);    /*设置热区*/
void RunSys(COLLEGE_NODE **phead);                  /*系统功能模块的选择和运行*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*功能模块的调用*/
void CloseSys(COLLEGE_NODE *hd);                  /*退出系统*/
BOOL ShowModule(char **pString, int n);

BOOL LoadDate(void);           /*数据加载*/
BOOL SaveData(void);           /*保存数据*/
BOOL BackupData(void);          /*备份数据*/
BOOL RestoreData(void);        /*恢复数据*/
BOOL ExitSys(void);            /*退出系统*/ 
BOOL HelpTopic(void);          /*帮助主体*/
BOOL AboutDorm(void);          /*关于系统*/

BOOL TypeCode(void);           /*项目类别代码*/
BOOL MaintainCollegeInfo(void);   /*维护院系基本信息*/
BOOL MaintainTeamInfo(void);    /*维护科研团队基本信息*/
BOOL MaintainProjectInfo(void); /*维护科研项目信息*/

BOOL QueryCollegeInfo(void);      /*查询院系基本信息*/
BOOL QueryTeamInfo(void);       /*查询科研团队基本信息*/
BOOL QueryProjectInfo(void);    /*查询科研项目基本信息*/

BOOL StatManRate(void);       /*统计人数及人数比*/
BOOL StatProjectType(void);        /*科研项目总体统计*/
BOOL StatTeam(void);         /*优秀科研团队统计*/
BOOL StatProjectPer(void);       /*人均科研项目统计*/
BOOL Stat(void);              /*  abv*/

BOOL InsertCollegeNode(COLLEGE_NODE *hd, COLLEGE_NODE *pcol_node); /*插入院系基本信息节点*/
BOOL DelCollegeNode(COLLEGE_NODE *hd, char *col_name);    /*删除院系基本信息节点*/ 
BOOL ModifCollegeNode(COLLEGE_NODE *hd, char *col_name, COLLEGE_NODE *pcol_node); /*修改院系基本信息节点*/ 
 
BOOL InsertTeamNode(COLLEGE_NODE *hd,TEAM_NODE *pteamnode);   /*插入团队信息节点*/
BOOL DelTeamNode(COLLEGE_NODE *hd, char *col_name, char *team_name);  /*删除团队信息节点*/
BOOL ModifTeamNode(COLLEGE_NODE *hd, char *team_name, TEAM_NODE *pteamnode);  /*修改科研团队基本信息节点*/ 
 
BOOL InsertProjectNode(COLLEGE_NODE *hd,PROJECT_NODE *ppro_node);     /*插入科研项目信息节点*/ 
BOOL DelProjectNode(COLLEGE_NODE *hd, char *team_name, char *pro_num);   /*删除科研项目信息结点*/
BOOL ModifProjectNode(COLLEGE_NODE *hd, char *team_name, char *num, PROJECT_NODE *ppro_node);/*修改科研项目信息结点*/

BOOL MarchString(char *string_item, char *cond); /*匹配字符串*/


COLLEGE_NODE *SeekCollegeNode(COLLEGE_NODE *hd, char *col_name);/*查找院系基本信息节点*/ 
TEAM_NODE *SeekTeamNode(COLLEGE_NODE *hd, char* team_name);  /*查找团队基本信息结点*/
PROJECT_NODE *SeekProNode(COLLEGE_NODE *hd, char *team_name, char *num);/*查找缴费信息结点*/

COLLEGE_NODE *SeekColNodeMan(COLLEGE_NODE *hd, char* dutyman); /*按负责人查找院系信息节点*/ 
COLLEGE_NODE *SeekCollegeNodeNameM(COLLEGE_NODE *hd, char *namem); /*按院系名称关键字查找院系信息节点*/ 

TEAM_NODE *SeekTeamNodeNameM(COLLEGE_NODE *hd, char *namem); /*按团队名称关键字查找团队信息节点*/ 
TEAM_NODE *SeekTeamNodeTeacher(COLLEGE_NODE *hd, char mark, int num_tc);/*按教师人数查找团队信息节点*/ 

PROJECT_NODE *SeekProjectNodeNum(COLLEGE_NODE *hd, char *p_num); /*按项目编号查找科研项目信息节点*/ 
PROJECT_NODE *SeekProjectNodeT(COLLEGE_NODE *hd,char *tname);  /*按所属团队查找科研项目信息节点*/ 

PEOPLES_NODE *StatPeopleRate(COLLEGE_NODE *hd); /*统计研究生，教师总数及人数比*/ 
PEOPLES_NODE *SortPeopleInfo(PEOPLES_NODE *ppeo_node_hd); /*按研究生教师人数比对链表排序*/
PROJECT_NUM_NODE *StatCollegeProjectNum(COLLEGE_NODE *hd); /*统计各院系各类项目总数*/
PROJECT_NUM_NODE *SortColProInfo(PROJECT_NUM_NODE *ppro_num_node_hd); /*按科研项目降序对链表排序*/ 
TEAM_NASI_NODE *StatTeamNasiTen(COLLEGE_NODE *hd);  /*统计自然科学基金项目排名前十的科研团队*/
TEAM_NASI_NODE *SortTeamNasiInfo(TEAM_NASI_NODE *pteam_nasi_node_hd);
TEAM_PROTEA_NODE *StatProTeaScale(COLLEGE_NODE *hd);  /*统计科研项目教师人数比前五的团队*/
TEAM_PROTEA_NODE *SortProTeaInfo(TEAM_PROTEA_NODE *protea_node_hd);



BOOL SaveSysData(COLLEGE_NODE *hd);                       /*保存系统数据*/
BOOL BackupSysData(COLLEGE_NODE *hd, char *filename);     /*备份系统数据*/
BOOL RestoreSysData(COLLEGE_NODE **phead, char *filename);  /*恢复系统数据*/

#endif /**< TYPE_H_INCLUDED*/

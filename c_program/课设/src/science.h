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

#define SCR_ROW 25             /*��Ļ����*/
#define SCR_COL 80             /*��Ļ����*/

#define TURE 1
#define FALSE 0

/**
 *Ժϵ������Ϣ���ӵ�ṹ
 */
typedef struct college_node {
    char name[20];                  /*Ժϵ����*/
    char dutyman[12];               /*������*/
    char tel[15];                   /*��ϵ�绰*/
    struct college_node *next;      /*ָ����һ�ṹ���ָ��*/
    struct team_node *tnext;        /*ָ������Ŷ�����ָ��*/
} COLLEGE_NODE;

/**
 *�����Ŷӻ�����Ϣ���ӵ�ṹ
 */
typedef struct team_node {
    char name[30];                  /*�Ŷ�����*/
    char dutyman[12];               /*������*/
    char college[20];               /*����Ժϵ*/ 
    int num_tc;                     /*��ʦ����*/
    int num_stu;                    /*�о�������*/
    struct team_node *next;         /*ָ����һ�ṹ���ָ��*/
    struct project_node *pnext;     /*ָ�������Ŀ����ָ��*/
} TEAM_NODE;

/**
 *������Ŀ������Ϣ���ӵ�ṹ
 */
typedef struct project_node {
    char num[15];                   /*��Ŀ���*/
    char type;                      /*��Ŀ����*/
    char start[8];                  /*��ʼʱ��*/
    float money;                    /*��Ŀ����*/
    char dutyman[12];               /*������*/
    char team[30];                  /*�����Ŷ�*/
    struct project_node *next;      /*ָ����һ�ṹ���ָ��*/
} PROJECT_NODE;


/*
  �о�����������ʦ�����������������ṹ 
*/
typedef struct peoples_node{
	char *col_name; /*Ժϵ����*/ 
	int student;   /*�о�������*/ 
	int teacher;   /*��ʦ����*/ 
	float scale;     /*������*/ 
	struct peoples_node *next;
} PEOPLES_NODE; 



/*
  ���������Ŀ�������ṹ 
*/
typedef struct project_num_node{
	char *col_name; /*Ժϵ����*/ 
	int total;     /*������Ŀ����*/ 
	int p973;      /*973��Ŀ����*/ 
	int p863;      /*863��Ŀ����*/
	float money;    /*��Ŀ���ʽ�*/ 
	struct project_num_node *next;
} PROJECT_NUM_NODE;

/*
  �����Ŷ���Ȼ��ѧ������Ŀ���������ṹ 
*/
typedef struct team_nasi_node{
	char *team_name;  /*�����Ŷ�����*/
	int num_nasi;   /*��Ȼ��ѧ������Ŀ����*/
	float money; /*��Ŀ���ʽ�*/
	struct team_nasi_node *next; 
} TEAM_NASI_NODE; 

/*
  �����Ŷӿ�����Ŀ���ʦ������ 
*/
typedef struct team_protea_node{
	char *team_name;   /*�����Ŷ�����*/
	int num_tc;   /*��ʦ����*/
	int num_pro;   /*������Ŀ����*/
	float scale;   /*������*/
	struct team_protea_node *next;
} TEAM_PROTEA_NODE;

/**
 *��Ļ������Ϣ�������ṹ
 */
typedef struct layer_node {
    char LayerNo;            /**< �������ڲ���*/
    SMALL_RECT rcArea;       /**< ����������������*/
    CHAR_INFO *pContent;     /**< �������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;           /**< �������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next; /**< ָ����һ����ָ��*/
} LAYER_NODE;

/**
 *��ǩ���ṹ
 */
typedef struct labe1_bundle {
    char **ppLabel;        /**< ��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;           /**< ��ǩ��λ�����׵�ַ*/
    int num;               /**< ��ǩ����*/
} LABEL_BUNDLE;

/**
 *�����ṹ
 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /**< ������λ�����׵�ַ*/
    char *pSort;           /**< �������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;            /**< ������������׵�ַ*/
    int num;               /**< ��������*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*����������Ϣ����ͷ*/
COLLEGE_NODE *gp_head = NULL;                     /*����ͷָ��*/

char *gp_sys_name = "������Ŀ��Ϣ����ϵͳ";    /*ϵͳ����*/
char *gp_college_info_filename = "college.dat";        /*Ժϵ������Ϣ�����ļ�*/
char *gp_team_info_filename = "team.dat";       /*�Ŷӻ�����Ϣ�����ļ�*/
char *gp_project_info_filename = "project.dat";      /*������Ŀ������Ϣ�����ļ�*/
char *gp_type_code_filename = "type.dat";        /*��Ŀ��������ļ�*/

char *ga_main_menu[] = {                       /*ϵͳ���˵���*/
	 				    "�ļ�(F)",
    	                "����ά��(M)",
                        "���ݲ�ѯ(Q)",
                        "����ͳ��(S)",
                        "����(H)"
                       };

char *ga_sub_menu[] = {
	 				   "[S] ���ݱ���",
					   "[B] ���ݱ���",
					   "[R] ���ݻָ�",
					   "[X] �˳� ALT+X" ,
					   "[S] ��Ŀ������",
					   "", 
	 				   "[D] Ժϵ������Ϣά��",          /*ϵͳ�Ӳ˵���*/
                       "[P] �����Ŷӻ�����Ϣά��",
                       "[C] ������Ŀ������Ϣά��",
                       "[D] Ժϵ��Ϣ",
                       "[P] �����Ŷӻ�����Ϣ",
                       "[C] ������Ŀ������Ϣ",
                       "[D] ��ʦ���о�����������������",
                       "[P] ������Ŀ������Ϣ",
                       "[C] ��������Ŷ�",
                       "[I] ��ʦ�˾�������Ŀ���",
                       "[O]   ",
                       "[A] ����..."
                      };

int ga_sub_menu_count[] = { 4, 5, 3, 5, 1};  /*�����˵������Ӳ˵��ĸ���*/
int gi_sel_menu = 1;                        /*��ѡ�е����˵����,��ʼΪ1*/
int gi_sel_sub_menu = 0;                    /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/

CHAR_INFO *gp_buff_menubar_info = NULL;     /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO *gp_buff_stateBar_info = NULL;    /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char *gp_scr_att = NULL;    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
char *gp_type_code = NULL;    /*�����Ŀ�����������ݻ�����*/
char gc_sys_state = '\0';   /*��������ϵͳ״̬���ַ�*/

unsigned long gul_type_code_len = 0;   /*��Ŀ���ʹ������*/

HANDLE gh_std_out;          /*��׼����豸���*/
HANDLE gh_std_in;           /*��׼�����豸���*/

int LoadCode(char *filename, char **pbuffer);  /*��������*/
int CreatList(COLLEGE_NODE **phead);              /*���������ʼ��*/
void ClearScreen(void);                         /*����*/
void ShowMenu(void);                            /*��ʾ�˵���*/
void InitInterface(void);							/*��ʼ������*/ 
void PopMenu(int num);                          /*��ʾ�����˵�*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea);  /*����������Ļ��Ϣά��*/
void PopOff(void);                              /*�رն��㵯������*/
void DrawBox(SMALL_RECT *pRc);                /*���Ʊ߿�*/
void LocSubMenu(int num, SMALL_RECT *rc);    /*���˵������˵���λ*/
void ShowState(void);                           /*��ʾ״̬��*/
void TagMainMenu(int num);                      /*��Ǳ�ѡ�е����˵���*/
void TagSubMenu(int num);                       /*��Ǳ�ѡ�е��Ӳ˵���*/

int DealInput(HOT_AREA *pHotArea, int *piHot);    /*����̨���봦��*/
void SetHotPoint(HOT_AREA *phot_area, int hot_num);    /*��������*/
void RunSys(COLLEGE_NODE **phead);                  /*ϵͳ����ģ���ѡ�������*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);  /*����ģ��ĵ���*/
void CloseSys(COLLEGE_NODE *hd);                  /*�˳�ϵͳ*/
BOOL ShowModule(char **pString, int n);

BOOL LoadDate(void);           /*���ݼ���*/
BOOL SaveData(void);           /*��������*/
BOOL BackupData(void);          /*��������*/
BOOL RestoreData(void);        /*�ָ�����*/
BOOL ExitSys(void);            /*�˳�ϵͳ*/ 
BOOL HelpTopic(void);          /*��������*/
BOOL AboutDorm(void);          /*����ϵͳ*/

BOOL TypeCode(void);           /*��Ŀ������*/
BOOL MaintainCollegeInfo(void);   /*ά��Ժϵ������Ϣ*/
BOOL MaintainTeamInfo(void);    /*ά�������Ŷӻ�����Ϣ*/
BOOL MaintainProjectInfo(void); /*ά��������Ŀ��Ϣ*/

BOOL QueryCollegeInfo(void);      /*��ѯԺϵ������Ϣ*/
BOOL QueryTeamInfo(void);       /*��ѯ�����Ŷӻ�����Ϣ*/
BOOL QueryProjectInfo(void);    /*��ѯ������Ŀ������Ϣ*/

BOOL StatManRate(void);       /*ͳ��������������*/
BOOL StatProjectType(void);        /*������Ŀ����ͳ��*/
BOOL StatTeam(void);         /*��������Ŷ�ͳ��*/
BOOL StatProjectPer(void);       /*�˾�������Ŀͳ��*/
BOOL Stat(void);              /*  abv*/

BOOL InsertCollegeNode(COLLEGE_NODE *hd, COLLEGE_NODE *pcol_node); /*����Ժϵ������Ϣ�ڵ�*/
BOOL DelCollegeNode(COLLEGE_NODE *hd, char *col_name);    /*ɾ��Ժϵ������Ϣ�ڵ�*/ 
BOOL ModifCollegeNode(COLLEGE_NODE *hd, char *col_name, COLLEGE_NODE *pcol_node); /*�޸�Ժϵ������Ϣ�ڵ�*/ 
 
BOOL InsertTeamNode(COLLEGE_NODE *hd,TEAM_NODE *pteamnode);   /*�����Ŷ���Ϣ�ڵ�*/
BOOL DelTeamNode(COLLEGE_NODE *hd, char *col_name, char *team_name);  /*ɾ���Ŷ���Ϣ�ڵ�*/
BOOL ModifTeamNode(COLLEGE_NODE *hd, char *team_name, TEAM_NODE *pteamnode);  /*�޸Ŀ����Ŷӻ�����Ϣ�ڵ�*/ 
 
BOOL InsertProjectNode(COLLEGE_NODE *hd,PROJECT_NODE *ppro_node);     /*���������Ŀ��Ϣ�ڵ�*/ 
BOOL DelProjectNode(COLLEGE_NODE *hd, char *team_name, char *pro_num);   /*ɾ��������Ŀ��Ϣ���*/
BOOL ModifProjectNode(COLLEGE_NODE *hd, char *team_name, char *num, PROJECT_NODE *ppro_node);/*�޸Ŀ�����Ŀ��Ϣ���*/

BOOL MarchString(char *string_item, char *cond); /*ƥ���ַ���*/


COLLEGE_NODE *SeekCollegeNode(COLLEGE_NODE *hd, char *col_name);/*����Ժϵ������Ϣ�ڵ�*/ 
TEAM_NODE *SeekTeamNode(COLLEGE_NODE *hd, char* team_name);  /*�����Ŷӻ�����Ϣ���*/
PROJECT_NODE *SeekProNode(COLLEGE_NODE *hd, char *team_name, char *num);/*���ҽɷ���Ϣ���*/

COLLEGE_NODE *SeekColNodeMan(COLLEGE_NODE *hd, char* dutyman); /*�������˲���Ժϵ��Ϣ�ڵ�*/ 
COLLEGE_NODE *SeekCollegeNodeNameM(COLLEGE_NODE *hd, char *namem); /*��Ժϵ���ƹؼ��ֲ���Ժϵ��Ϣ�ڵ�*/ 

TEAM_NODE *SeekTeamNodeNameM(COLLEGE_NODE *hd, char *namem); /*���Ŷ����ƹؼ��ֲ����Ŷ���Ϣ�ڵ�*/ 
TEAM_NODE *SeekTeamNodeTeacher(COLLEGE_NODE *hd, char mark, int num_tc);/*����ʦ���������Ŷ���Ϣ�ڵ�*/ 

PROJECT_NODE *SeekProjectNodeNum(COLLEGE_NODE *hd, char *p_num); /*����Ŀ��Ų��ҿ�����Ŀ��Ϣ�ڵ�*/ 
PROJECT_NODE *SeekProjectNodeT(COLLEGE_NODE *hd,char *tname);  /*�������ŶӲ��ҿ�����Ŀ��Ϣ�ڵ�*/ 

PEOPLES_NODE *StatPeopleRate(COLLEGE_NODE *hd); /*ͳ���о�������ʦ������������*/ 
PEOPLES_NODE *SortPeopleInfo(PEOPLES_NODE *ppeo_node_hd); /*���о�����ʦ�����ȶ���������*/
PROJECT_NUM_NODE *StatCollegeProjectNum(COLLEGE_NODE *hd); /*ͳ�Ƹ�Ժϵ������Ŀ����*/
PROJECT_NUM_NODE *SortColProInfo(PROJECT_NUM_NODE *ppro_num_node_hd); /*��������Ŀ�������������*/ 
TEAM_NASI_NODE *StatTeamNasiTen(COLLEGE_NODE *hd);  /*ͳ����Ȼ��ѧ������Ŀ����ǰʮ�Ŀ����Ŷ�*/
TEAM_NASI_NODE *SortTeamNasiInfo(TEAM_NASI_NODE *pteam_nasi_node_hd);
TEAM_PROTEA_NODE *StatProTeaScale(COLLEGE_NODE *hd);  /*ͳ�ƿ�����Ŀ��ʦ������ǰ����Ŷ�*/
TEAM_PROTEA_NODE *SortProTeaInfo(TEAM_PROTEA_NODE *protea_node_hd);



BOOL SaveSysData(COLLEGE_NODE *hd);                       /*����ϵͳ����*/
BOOL BackupSysData(COLLEGE_NODE *hd, char *filename);     /*����ϵͳ����*/
BOOL RestoreSysData(COLLEGE_NODE **phead, char *filename);  /*�ָ�ϵͳ����*/

#endif /**< TYPE_H_INCLUDED*/

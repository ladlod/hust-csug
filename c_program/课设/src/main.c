#include "science.h"

unsigned long ul;
int main()
{
 	COORD size = {SCR_COL, SCR_ROW};              /*���ڻ�������С*/

    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* ��ȡ��׼����豸���*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* ��ȡ��׼�����豸���*/

    SetConsoleTitle(gp_sys_name);                 /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*���ô��ڻ�������С80*25*/
	
	LoadDate(); 
    InitInterface();          /*�����ʼ��*/

    RunSys(&gp_head);
	CloseSys(gp_head);

    return 0;
}


/*
  	��������: InitInterface
	��������: ��ʼ������.
	�������: ��
	�������: ��
	�� �� ֵ: ��
*/
void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*��ɫǰ������ɫ����*/

    SetConsoleTextAttribute(gh_std_out, att);  /*���ÿ���̨��Ļ�������ַ�����*/

    ClearScreen();  /* ����*/

    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;

    ShowMenu();     /*��ʾ�˵���*/
    ShowState();    /*��ʾ״̬��*/

    return;
}

/*
    �������ƣ�ShowMenu
    �������ܣ���ʾ���˵��������������������˵���һ������ѡ���
    �����������
    �����������
    ����ֵ����
*/
void ShowMenu()
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	CONSOLE_CURSOR_INFO lpCur;
	COORD size;
	COORD pos1={0,0};
	
	int i,j;
	int PosA=2,PosB;
	char ch;
	
	GetConsoleScreenBufferInfo(gh_std_out,&binfo);
	
	size.X=binfo.dwSize.X;
	size.Y=1;
	
	SetConsoleCursorPosition(gh_std_out,pos1);
	
	for(i=0;i<5;i++)
	{
	printf("  %s  ",ga_main_menu[i]); 
    }
    GetConsoleCursorInfo(gh_std_out,&lpCur);
    //lpCur.bVisible=FALSE;
    SetConsoleCursorInfo(gh_std_out,&lpCur);
    
    gp_buff_menubar_info=(CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT reMenu={0,0,size.X-1,0}; 
    
    ReadConsoleOutput(gh_std_out,gp_buff_menubar_info,size,pos1,&reMenu);
    for(i=0;i<size.X;i++)
	{
       (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
	}
	WriteConsoleOutput(gh_std_out,gp_buff_menubar_info,size,pos1,&reMenu);
	COORD endPos = {0, 1};
    SetConsoleCursorPosition(gh_std_out, endPos);  /*�����λ�������ڵ�2�е�1��*/

    /*���˵�����Ϊ�������������Ϊ�˵���ţ���������Ϊ0(��ť��)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*��λ��i+1�Ų˵������ֹλ��*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*���ò˵��������ַ���Ԫ������ֵ*/
        }
        PosA = PosB + 4;
        i++;
    } while (i<5);

    TagMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/

	return ;
}

/*
    �������ƣ�TagMainMenu
    �������ܣ������˵���ѡ�б�־
    ���������num ѡ�е����˵����
    �����������
    ����ֵ����
*/
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    char ch;
    int i;

    if (num == 0) /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
    {
        for (i=1; i<num; i++)
        {
            PosA += strlen(ga_main_menu[i-1]) + 4;
        }
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;

    /*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*��ѡ�в˵���������ǣ��ڵװ���*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }

    /*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                               | BACKGROUND_RED;
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED;
        }
    }

    /*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    return;
}

/*
    �������ƣ�ClearScreen
    �������ܣ������Ļ��Ϣ
    �����������
    �����������
    ����ֵ����
*/
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 0};
    unsigned long size;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*ȡ��Ļ��������Ϣ*/
    size = bInfo.dwSize.X * bInfo.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/

    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home,&ul);

    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(gh_std_out,' ', size, home,&ul);

    return;
}

/*
  	�������ƣ�PopMenu
	�������ܣ��������˵����Ӧ���Ӳ˵� 
	���������ָ�������˵��� 
	����������� 
	����ֵ���� 
*/
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh;
    int i, j, loc = 0;

    if (num != gi_sel_menu)       /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (gp_top_layer->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
    {
        return;
    }

    gi_sel_menu = num;    /*��ѡ�����˵�����Ϊָ�������˵���*/
    TagMainMenu(gi_sel_menu); /*��ѡ�е����˵����������*/
    LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/

    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD aLoc[labels.num];/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
    for (i=0; i<labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*����������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT aArea[areas.num];                    /*������������������λ��*/
    char aSort[areas.num];                      /*��������������������Ӧ���*/
    char aTag[areas.num];                         /*����������ÿ�������ı��*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*������λ*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1;           /*������˳����*/
    }
    areas.pArea = aArea;/*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;/*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;   /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*���������ڻ��߿�*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*����Ϊ0������Ϊ�մ�*/
        {   /*���Ȼ�����*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*ȡ���������ַ���Ԫ����������*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }

    }
    /*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*�����մ�*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/*
  	�������ƣ�PopUp
	�������ܣ���ָ���������������Ϣ������������������������Ϣ��ջ 
	�����������������λ�����ݴ�ŵ�ַ���������������ַ����ԣ����������б�ǩ����Ϣ��ŵ�ַ������������������Ϣ��ŵ�ַ 
	����������� 
	����ֵ���� 
*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
	LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
        {
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)
                                    | (pHotArea->pTag[i] << 2);
        }
    }
    return;
}

 
/*
  	�������ƣ�PopOff
	�������ܣ��رն��㵯�����ڣ��ָ���������ԭ��ۺ��ַ���Ԫ���� 
	����������� 
	����������� 
	����ֵ���� 
*/
void PopOff(void)
{
	LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;

    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*�ָ�������������ԭ���*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
} 

/*
  	�������ƣ�DrawBox
	�������ܣ���ָ�����򻭱߿� 
	����������������λ����Ϣ�ĵ�ַ 
	����������� 
	����ֵ�� �� 
*/
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*�����õ��ַ�*/
    COORD pos = {pRc->Left, pRc->Top};  /*��λ����������Ͻ�*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*��ѭ�����ϱ߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*��ѭ�����߿�����ߺ��ұ���*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*���±߿����*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    return;
}

/*
  	�������ƣ�TagSubMenu
	�������ܣ���ָ���Ӳ˵���ѡ�б�� 
	���������ѡ�е��Ӳ˵��� 
	����������� 
	����ֵ���� 
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    LocSubMenu(gi_sel_menu, &rcPop);  /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
        return;
    }

    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*���ƶ��Ӳ˵�������ѡ�б��*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/*
  	�������ƣ�LocSubMenu
	�������ܣ����㵯���Ӳ˵��������ϽǺ����½ǵ�λ�� 
	���������ѡ�е����˵���� 
	����������������λ����Ϣ�ĵ�ַ 
	����ֵ���� 
*/
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }
    rc->Right += rc->Left + 3;  /*����������ұ߽�*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*���������±ߵ��к�*/
    if (rc->Right >= SCR_COL)  /*�ұ߽�Խ��Ĵ���*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
} 

/*
  	��������: RunSys
	��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
	�������: ��
	�������: ����ͷָ��ĵ�ַ
	�� �� ֵ: ��
*/
void RunSys(COLLEGE_NODE **phead)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    while (bRet)
    {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);

        if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
            if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
            {
                /* cAtt > 0 ������λ�ô�������(���˵����ַ���Ԫ)
                 * cAtt != gi_sel_menu ������λ�õ����˵���δ��ѡ��
                 * gp_top_layer->LayerNo > 0 ������ǰ���Ӳ˵�����
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
                }
            }
            else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
            {
                TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*������������ߵ�һ��*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(���˵����ַ���Ԫ)*/
                    {
                        PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
                    }
                    /*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    }
                }
                else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
                    {
                        PopOff(); /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/

                        /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/

            /*ϵͳ��ݼ��Ĵ���*/
            if (vkc == 112) /*�������F1��*/
            {
                if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,4);
                        break;
                    case 70:  /*Alt+F*/
                        PopMenu(1);
                        break;
                    case 77: /*Alt+M*/
                        PopMenu(2);
                        break;
                    case 81: /*Alt+Q*/
                        PopMenu(3);
                        break;
                    case 83: /*Alt+S*/
                        PopMenu(4);
                        break;
                    case 72: /*Alt+H*/
                        PopMenu(5);
                        break;
                }
            }
            else if (asc == 0) /*�������Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40:
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)
                            {
                                num = ga_sub_menu_count[gi_sel_menu-1];
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num--;
                            }
                            TagSubMenu(num);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])
                            {
                                num = 1;
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num++;
                            }
                            TagSubMenu(num);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/
                            PopMenu(1);
                            break;
                        case 77: /*m��M*/
                            PopMenu(2);
                            break;
                        case 81: /*q��Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            PopMenu(4);
                            break;
                        case 72: /*h��H*/
                            PopMenu(5);
                            break;
                        case 13: /*�س�*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }
    return ; 
}

/**
 * ��������: ShowState
 * ��������: ��ʾ״̬��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��: ״̬���ַ�����Ϊ�׵׺���, ��ʼ״̬��״̬��Ϣ.
 */
void ShowState()
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int i;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SMALL_RECT rcMenu ={0, bInfo.dwSize.Y-1, size.X-1, bInfo.dwSize.Y-1};

    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }

    for (i=0; i<size.X; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                | BACKGROUND_RED;

    }

    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);

    return;
}


BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 6;
    pos.Y = n + 5;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];

    for (i=0; i<n; i++) {
        aLoc[i].X = rcPop.Left + 3;
        aLoc[i].Y = rcPop.Top + 2 + i;
    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = rcPop.Left + 3 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;

    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,
                           aLoc[n-1].X + 3, aLoc[n-1].Y}};

    char aSort[] = {0};
    char aTag[] = {1};

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 2 + n;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    DealInput(&areas, &iHot);
    PopOff();

    return bRet;

}

/*
  	�������ƣ�CloseSys 
	�������ܣ��ر�ϵͳ 
	�������������ͷָ�� 
	����������� 
	����ֵ�� �� 
*/
void CloseSys(COLLEGE_NODE *hd)
{
    COLLEGE_NODE *pcol_node1=hd, *pcol_node2;
    TEAM_NODE *pteamnode1, *pteamnode2;
    PROJECT_NODE *ppro_node1, *ppro_node2;

    while (pcol_node1 != NULL) /*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
    {
        pcol_node2 = pcol_node1->next;
        pteamnode1 = pcol_node1->tnext;
        while (pteamnode1 != NULL) /*�ͷ�ѧ��������Ϣ֧���Ķ�̬�洢��*/
        {
            pteamnode2 = pteamnode1->next;
            ppro_node1 = pteamnode1->pnext;
            while (ppro_node1 != NULL) /*�ͷŽɷ���Ϣ֧���Ķ�̬�洢��*/
            {
                ppro_node2 = ppro_node1->next;
                free(ppro_node1);
                ppro_node1 = ppro_node2;
            }
            free(pteamnode1);
            pteamnode1 = pteamnode2;
        }
        free(pcol_node1);  /*�ͷ��������Ķ�̬�洢��*/
        pcol_node1 = pcol_node2;
    }

    ClearScreen();        /*����*/

    /*�ͷŴ�Ų˵�����״̬������Ŀ���������Ϣ��̬�洢��*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    free(gp_type_code);

    /*�رձ�׼���������豸���*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);

    /*�����ڱ�������Ϊ���н���*/
    SetConsoleTitle("���н���");

    return;
} 


/*
 * ��������: ExeFunction
 * ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
 * �������: m ���˵���ţ�s �Ӳ˵����
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE �� FALSE
 * ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;

    /*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
    pFunction[0] = SaveData;
    pFunction[1] = BackupData;
    pFunction[2] = RestoreData;
    pFunction[3] = ExitSys;
    pFunction[4] = TypeCode;
    pFunction[5] = NULL;
    pFunction[6] = MaintainCollegeInfo;
    pFunction[7] = MaintainTeamInfo;
    pFunction[8] = MaintainProjectInfo;
    pFunction[9] = QueryCollegeInfo;
    pFunction[10] = QueryTeamInfo;
    pFunction[11] = QueryProjectInfo;
    pFunction[12] = StatManRate;
    pFunction[13] = StatProjectType;
    pFunction[14] = StatTeam;
    pFunction[15] = StatProjectPer;
    pFunction[16] = Stat;
    pFunction[17] = AboutDorm;
    
    for (i=1,loc=0; i<m; i++)  /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    {
        loc += ga_sub_menu_count[i-1];
    }
    loc += s - 1;

    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*�ú���ָ�������ָ��Ĺ��ܺ���*/
    }

    return bRet;
}


/*
  	�������ƣ�CreatList 
	�������ܣ����ļ���ȡ���ݣ�����ŵ������� 
	����������� 
	���������phead����ͷָ��ĵ�ַ���������ش�����ʮ������ 
	����ֵ�� int�ͣ���ʾ���������
	         0������
			 4���Ѽ���Ժϵ������Ϣ
			 12���Ѽ���Ժϵ���Ŷӻ�����Ϣ
			 28���Ѽ���Ժϵ���Ŷӣ�������Ŀ������Ϣ 
*/
int CreatList(COLLEGE_NODE **phead) 
{
	COLLEGE_NODE *hd = NULL, *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	FILE *pFile;
	int find;
	int re = 0;
	
	if((pFile=fopen(gp_college_info_filename, "rb"))==NULL)
	{
		printf("Ժϵ������Ϣ�ļ���ʧ�ܣ�\n");
		return re;
	}
	printf("Ժϵ������Ϣ�ļ��򿪳ɹ���\n"); 
	/*���ļ���ȡԺϵ������Ϣ*/

	while(!feof(pFile))
	{
		pcol_node=(COLLEGE_NODE*)malloc(sizeof(COLLEGE_NODE));
		fread(pcol_node,sizeof(COLLEGE_NODE),1,pFile); 
		pcol_node->tnext=NULL;
		pcol_node->next = hd;
		hd = pcol_node;
	}
	if(hd==NULL)
	{
	 	printf("Ժϵ������Ϣ�ļ�����ʧ�ܣ�\n");
		return re;	
	}
	printf("Ժϵ������Ϣ�ļ����سɹ���\n");
	
	*phead=hd;
	re += 4;
	if((pFile=fopen(gp_team_info_filename, "rb"))==NULL)
	{
		printf("�Ŷӻ�����Ϣ�ļ���ʧ�ܣ�\n");
		return re;
	}
	printf("�Ŷӻ�����Ϣ�򿪳ɹ���\n");
	re += 8;
	
	/*���Ŷӻ�����Ϣ����Ժϵ������Ϣ������Ӧ�ڵ��֧����*/
	while(!feof(pFile)) 
	{
		pteamnode=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
		fread(pteamnode, sizeof(TEAM_NODE),1,pFile);
		pteamnode->pnext=NULL;
		
		pcol_node=hd;
		while(pcol_node!=NULL&&strcmp(pcol_node->name,pteamnode->college)!=0)
		{
			pcol_node=pcol_node->next;
		}
		if(pcol_node!=NULL)
		{
			pteamnode->next=pcol_node->tnext;
			pcol_node->tnext=pteamnode;
		}
		else
		{
			free(pteamnode);
		}
	}
	fclose(pFile);
	
	
	if((pFile=fopen(gp_project_info_filename, "rb"))==NULL)
	{
		printf("������Ŀ������Ϣ�ļ���ʧ�ܣ�\n");
		return re;
	}
	printf("������Ŀ������Ϣ�򿪳ɹ���\n");
	re+=16;
	
	/*��������Ŀ������Ϣ��������Ŷӻ�����Ϣ֧����Ӧ�Ľڵ����Ŀ֧����*/
	while(!feof(pFile))
	{
	 	ppro_node=(PROJECT_NODE*)malloc(sizeof(PROJECT_NODE));
	 	fread(ppro_node,sizeof(PROJECT_NODE), 1, pFile);
	 	
	 	pcol_node = hd;
	 	find = 0;
	 	while(pcol_node != NULL && find==0)
	 	{
	 		pteamnode=pcol_node->tnext;
	 		while(pteamnode== NULL && find==0)
	 		{
	 		 	if(strcmp(pteamnode->name,ppro_node->team)==0)
					{
						find=1;
						break;
					}
				pteamnode=pteamnode->next;
			}
			pcol_node=pcol_node->next;
		}
 		if(find)
		{
			ppro_node->next=pteamnode->pnext;
			pteamnode->pnext=ppro_node;
		}
		else
		{
			free(ppro_node);
		}
	} 
	fclose(pFile);
	
	return re;
}

/*
    �������ƣ�SeekCollegeNode
	�������ܣ���Ժϵ���Ʋ���ָ��Ժϵ������Ϣ�ڵ�
	�������������ͷָ�룬�����ҵ�Ժϵ����
	�����������
	����ֵ�����з��ؽڵ��ַ��û���з���NULL
*/
COLLEGE_NODE *SeekCollegeNode(COLLEGE_NODE *hd, char *col_name)
{
	COLLEGE_NODE *pcol_node;
	int find=0;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		if(strcmp(pcol_node->name,col_name)==0)
		{
			find=1;
			break;
		}
	}
	if(find) return pcol_node;
	else return NULL;
}

/*
  	�������ƣ�SeekTeamNode
	�������ܣ����Ŷ����Ʋ���ָ�������Ŷӻ�����Ϣ�ڵ� 
	�������������ͷָ�룬�����ҵ��Ŷ����� 
	����������� 
	����ֵ�����з��ؽڵ��ַ��û���з���NULL
*/
TEAM_NODE *SeekTeamNode(COLLEGE_NODE *hd, char *team_name)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE  *pteamnode;
	int find = 0;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		pteamnode=pcol_node->tnext;
		while(pteamnode != NULL)
		{
			if(strcmp(pteamnode->name,team_name)==0)
			{
				find=1;
				break;
			}
			pteamnode=pteamnode->next;
		}
		if(find)   break;
	}
	if(find) return pteamnode;
	else return NULL;
} 

/*
  	�������ƣ�SeekProNode
	�������ܣ������Ŷ����ƣ���Ŀ��Ų��ҿ�����Ŀ��Ϣ�ڵ� 
	�������������ͷָ�룬�Ŷ����ƣ���Ŀ��� 
	����������� 
	����ֵ�����з��ؽڵ��ַ��û���з���NULL
*/
PROJECT_NODE *SeekProNode(COLLEGE_NODE *hd, char *team_name, char *num)
{
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node; 
	int find = 0;
	pteamnode = SeekTeamNode(hd, team_name); /*�����Ŷ���Ϣ�ڵ�*/
	
	if(pteamnode!=NULL)
	{
		ppro_node=pteamnode->pnext;
		while(ppro_node != NULL)
		{
			if(strcmp(ppro_node->num, num)==0)
			{
				find=1;
				break;
			}
			ppro_node=ppro_node->next; 
		}
	}
	if(find)
	{
		return ppro_node;
	}
	else return NULL;
}

/*
  	�������ƣ�InsertCollegeNode
	�������ܣ��������в���һ��Ժϵ������Ϣ�ڵ� 
	�������������ͷָ�룬��Ҫ����Ľڵ�ָ�� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ����ɹ���FALSE��ʾ����ʧ�� 
*/
BOOL InsertCollegeNode(COLLEGE_NODE *hd, COLLEGE_NODE *pcol_node)
{
	if(hd!=NULL)
	{
		pcol_node->next=hd->next;
		hd->next=pcol_node;
		return TURE;
	}
	else return FALSE;
}

/*
  	�������ƣ�InsertTeamNode
	�������ܣ��������в���һ���Ŷӻ�����Ϣ�ڵ� 
	�������������ͷָ�룬��Ҫ����Ľڵ�ָ�� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ����ɹ���FALSE��ʾ����ʧ�� 
*/
BOOL InsertTeamNode(COLLEGE_NODE *hd,TEAM_NODE *pteamnode)
{
	COLLEGE_NODE *pcol_node;
	
	/*���������ҵ���Ӧ��Ժϵ*/
	pcol_node = SeekCollegeNode(hd, pteamnode->college);
	if(pcol_node!=NULL)
	{
		pteamnode->next = pcol_node->tnext;
		pcol_node->tnext = pteamnode;
		return TURE;
	}
	else
		return FALSE;
}
 
/*
  	�������ƣ�InsertProjrctNode
	�������ܣ��������в���һ��������Ŀ��Ϣ�ڵ� 
	�������������ͷָ�룬��Ҫ����Ľڵ�ָ�� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ����ɹ���FALSE��ʾ����ʧ�� 
*/
BOOL InsertProjectNode(COLLEGE_NODE *hd,PROJECT_NODE *ppro_node)
{
	TEAM_NODE *pteamnode;
	
	/*���������ҵ���Ӧ�Ŀ����Ŷ�*/
	pteamnode = SeekTeamNode(hd, ppro_node->team);
	if(pteamnode!=NULL)
	{
		ppro_node->next = pteamnode->pnext;
		pteamnode->pnext = ppro_node;
		return TURE;
	}
	else
		return FALSE;
}

/*
  	�������ƣ�DelCollegeNode 
	�������ܣ�ɾ��Ժϵ������Ϣ�ڵ� 
	�������������ͷָ�룬Ժϵ���� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾɾ���ɹ���FALSE��ʾɾ��ʧ��
*/
BOOL DelCollegeNode(COLLEGE_NODE *hd, char *col_name)
{
	COLLEGE_NODE *pcol_node_prior;
	COLLEGE_NODE *pcol_node_current;
	BOOL bRet=FALSE;
	
	pcol_node_prior=NULL;
	pcol_node_current=hd->next;
	while(pcol_node_current!=NULL&&strcmp(pcol_node_current->name,col_name)!=0)
		{
			pcol_node_prior=pcol_node_current;
			pcol_node_current=pcol_node_current->next;
		}
		if(pcol_node_current!=NULL)
		{
			bRet=TURE;
			if(pcol_node_prior==NULL)
			{
				hd->next=pcol_node_current->next;
			}
			else
			{
				pcol_node_prior->next=pcol_node_current->next;
			}
			free(pcol_node_current);
		}
	return bRet;
}


/*
  	�������ƣ�DelTeamNode 
	�������ܣ�ɾ�������Ŷӻ�����Ϣ�ڵ� 
	�������������ͷָ��,Ժϵ����,�Ŷ����� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾɾ���ɹ���FALSE��ʾɾ��ʧ�� 
*/
BOOL DelTeamNode(COLLEGE_NODE *hd, char *col_name, char *team_name)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode_current;
	TEAM_NODE *pteamnode_prior;
	BOOL bRet=FALSE;
	pcol_node=SeekCollegeNode(hd,col_name);
	
	if(pcol_node!=NULL)
	{
		pteamnode_prior=NULL;
		pteamnode_current=pcol_node->tnext; 
		while(pteamnode_current!=NULL&&strcmp(pteamnode_current->name,team_name)!=0)
		{
			pteamnode_prior=pteamnode_current;
			pteamnode_current=pteamnode_current->next;
		}
		if(pteamnode_current!=NULL)
		{
			bRet=TURE;
			if(pteamnode_prior==NULL)
			{
				pcol_node->tnext=pteamnode_current->next;
			}
			else
			{
				pteamnode_prior->next=pteamnode_current->next;
			}
			free(pteamnode_current);
		}
	}
	return bRet;
} 

/*
  	�������ƣ�DelProjectNode
	�������ܣ�ɾ��������Ŀ��Ϣ�ڵ� 
	�������������ͷָ�룬�Ŷ����ƣ���Ŀ��� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾɾ���ɹ���FALSE��ʾɾ��ʧ�� 
*/
BOOL DelProjectNode(COLLEGE_NODE *hd, char *team_name, char *pro_num)
{
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node_prior;
	PROJECT_NODE *ppro_node_current;
	BOOL bRet=FALSE;
	pteamnode = SeekTeamNode(hd, team_name);
	if(pteamnode!=NULL)
	{
		ppro_node_prior=NULL;
		ppro_node_current=pteamnode->pnext;
		while(ppro_node_current!=NULL&&strcmp(ppro_node_current->num,pro_num)!=0)
		{
			ppro_node_prior=ppro_node_current;
			ppro_node_current=ppro_node_current->next;
		}
		
		if(ppro_node_current != NULL)
		{
			bRet=TURE;
			if(ppro_node_prior==NULL)
			{
				pteamnode->pnext=ppro_node_current->next;
			}
			else
			{
				ppro_node_prior->next=ppro_node_current->next; 
			}
			free(ppro_node_current);
		}
	}
	return bRet;
}

/*
  	�������ƣ�ModifCollegeNode
	�������ܣ��޸�ָ����Ժϵ������Ϣ 
	��������� ����ͷָ�룬Ժϵ���ƣ�����޸����ݽڵ��ָ�� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ�޸ĳɹ���FALSE��ʾ�޸�ʧ�� 
*/
BOOL ModifCollegeNode(COLLEGE_NODE *hd, char *col_name, COLLEGE_NODE *pcol_node)
{
	COLLEGE_NODE *pcol_node_temp;
	COLLEGE_NODE *pcol_node_next;
	
	pcol_node_temp=SeekCollegeNode(hd,col_name);
	if(pcol_node!=NULL)
	{
		pcol_node_next=pcol_node_temp->next;
		*pcol_node_temp=*pcol_node;
		pcol_node_temp->next=pcol_node_next;
		return TURE;
	}
	else return FALSE;
}


/*
  	�������ƣ�ModifTeamNode
	�������ܣ��޸�ָ���Ŀ����Ŷ���Ϣ 
	�������������ͷָ�룬�����Ŷ����ƣ�����޸����ݽڵ��ָ�� 
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ�޸ĳɹ���FALSE��ʾ�޸�ʧ��
*/
BOOL ModifTeamNode(COLLEGE_NODE *hd, char *team_name, TEAM_NODE *pteamnode)
{
	TEAM_NODE *pteamnode_temp;
	TEAM_NODE *pteamnode_next;
	
	pteamnode_temp=SeekTeamNode(hd, team_name);
	if(pteamnode_temp!=NULL)
	{
		pteamnode_next=pteamnode_temp->next;
		*pteamnode_temp=*pteamnode;
		pteamnode_temp->next=pteamnode_next;
		return TURE;
	}
	else return FALSE;
}

/*
  	�������ƣ�ModifProjectNode
	�������ܣ��޸�ָ���Ŀ�����Ŀ��Ϣ 
	�������������ͷָ�룬������Ŀ�������Ŷ����ƣ�������Ŀ��ţ�ָ�����޸����ݽڵ��ָ��  
	����������� 
	����ֵ��BOOL���ͣ�TURE��ʾ�޸ĳɹ���FALSE��ʾ�޸�ʧ�� 
*/
BOOL ModifProjectNode(COLLEGE_NODE *hd, char *team_name, char *num, PROJECT_NODE *ppro_node)
{
	PROJECT_NODE *ppro_node_temp;
	PROJECT_NODE *ppro_node_next;
	ppro_node_temp=SeekProNode(hd,team_name,num);
	if(ppro_node_temp!=NULL)
	{
		ppro_node_next=ppro_node_temp->next;
		*ppro_node_temp=*ppro_node;
		ppro_node_temp->next=ppro_node_next;
		return TURE;
	}
	else  return FALSE;
}

/*
  	�������ƣ�LoadDate
	�������ܣ�������������������ݴ������ļ����뵽�ڴ����������� 
	����������� 
	����������� 
	����ֵ��BOOL�ͣ����ܺ�������ExitSys�ķ���ֵ����ΪFALSE�⣬��������ΪTURE 
*/
BOOL LoadDate()
{
	int Re = 0;
	if(gp_type_code!=NULL)
	{
		free(gp_type_code);
	}
	gul_type_code_len=LoadCode(gp_type_code_filename, &gp_type_code);
	if(gul_type_code_len<3)
	{
		printf("��Ŀ����������ʧ�ܣ�\n");
		gc_sys_state &= 0xfe;
	}
	else
	{
		printf("��Ŀ���������سɹ���\n");
		gc_sys_state |= 1;
	}
	
	Re=CreatList(&gp_head);
	gc_sys_state |= Re;
	gc_sys_state &=~ (4+8+16-Re);
	if(gc_sys_state<(1|4|8|16))
	{
		printf("\nϵͳ�������ݲ�������\n");
		printf("\n�����������...\n");
		getch();
	}
	
	return TURE;
} 

/*
  	�������ƣ�LoadCode
	�������ܣ��������������ļ����뵽�ڴ滺�����������������ȥ���ո� 
	���������FileName��Ŵ����������ļ��� 
	���������ָ���ڴ滺������ָ���ַ 
	����ֵ����Ŵ������ڴ滺�����Ĵ�С 
*/
int LoadCode(char *filename, char **pbuffer)
{
	char *pTemp, *pStr1, *pStr2;
    int handle;
    int BufferLen, len, loc1, loc2, i;
    long filelen;

    if ((handle = open(filename, O_RDONLY | O_TEXT)) == -1) /*�����ֻ����ʽ��ʧ�� */
    {
        handle = open(filename, O_CREAT | O_TEXT, S_IREAD); /*�Դ�����ʽ��*/
    }
    filelen = filelength(handle);      /*�����ļ��ĳ���*/
    pTemp = (char *)calloc(filelen + 1, sizeof(char)); /*����ͬ����С�Ķ�̬�洢��*/
    BufferLen = read(handle, pTemp, filelen); /*�������ļ�������ȫ�����뵽�ڴ�*/
    close(handle);

    *(pTemp + BufferLen) = '\0'; /*�ڶ�̬�洢��β��һ�����ַ�����Ϊ�ַ���������־*/
    BufferLen++;

    for (i=0; i<BufferLen; i++) /*����̬�洢���е����л��з��滻�ɿ��ַ�*/
    {
        if (*(pTemp + i) == '\n')
        {
            *(pTemp + i) = '\0';
        }
    }

    /*������һ��ͬ����С�Ķ�̬�洢�������ڴ�������Ĵ��봮*/
    *pbuffer = (char *)calloc(BufferLen, sizeof(char));
    loc2 = 0;
    pStr1 = pTemp;
    len = strlen(pStr1);

    while (BufferLen > len + 1) /*ѡ������*/
    {
        loc1 = len + 1;
        while (BufferLen > loc1) /*ÿ���ҵ���������С���봮���׵�ַ����pStr1*/
        {
            pStr2 = pTemp + loc1;
            if (strcmp(pStr1, pStr2) > 0)
            {
                pStr1 = pStr2;
            }
            loc1 += strlen(pStr2) + 1;
        }
        len = strlen(pStr1);  /*��һ�����ҵ�����С���봮����*/

        /*������ǿմ�������и��ƣ�loc2����һ����С���봮��ŵ�ַ��ƫ����*/
        if (len > 0)
        {
            strcpy(*pbuffer + loc2, pStr1);
            loc2 += len + 1;  /*�Ѹ��ƵĴ��봮��ռ�洢�ռ��С*/
        }

        /*����С���봮��������ɾ����*/
        for(i=0; i<BufferLen-(pStr1-pTemp)-(len+1); i++)
        {
            *(pStr1 + i) = *(pStr1 + i + len + 1);
        }

        BufferLen -= len + 1; /*��һ���������������еĳ���*/
        pStr1 = pTemp;  /*�ٶ����еĵ�һ�����봮Ϊ��С���봮*/
        len = strlen(pStr1);
    } /*������ֻʣ��һ�����봮ʱ���������*/

    /*�������������봮*/
    len = strlen(pStr1);
    strcpy(*pbuffer + loc2, pStr1);

    /*�޸Ķ�̬�洢����С��ʹ�����÷����������봮*/
    loc2 += len + 1;
    *pbuffer = (char *)realloc(*pbuffer, loc2);
    free(pTemp);  /*�ͷ���������Ķ�̬�洢��*/

    return loc2;  /*���ش�Ŵ��봮���ڴ滺����ʵ�ʴ�С*/
}

/*
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �������: piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �� �� ֵ:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*ѭ��*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&
            (inRec.Event.MouseEvent.dwButtonState
             == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;

            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {
                    iRet = 13;
                    break;
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num-1].Top
                            == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                            && (pHotArea->pArea[num-1].Top
                            != pHotArea->pArea[*piHot-1].Top)))
                        {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {  /*ESC��*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {  /*�س�����ո��ʾ���µ�ǰ��ť*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

/*
  	�������ƣ�SetHotPoint
	�������ܣ��������� 
	�������������������ŵ�ַ������������� 
	����������� 
	����ֵ���� 
*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    for (i=0; i<pHotArea->num; i++)
    {  /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*�������������ı�����*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}

/*
  	�������ƣ�SaveSysData
	�������ܣ�����ϵͳ����������������� 
	�������������ͷָ�� 
	����������� 
	����ֵ��BOOL���ͣ�����ΪTURE 
*/ 
BOOL SaveSysData(COLLEGE_NODE *hd)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	FILE *pfout;
	int handle;
	
	if((handle=open(gp_type_code_filename,O_WRONLY|O_TEXT))==-1)
	{
		handle=open(gp_type_code_filename,O_CREAT|O_TEXT,S_IWRITE);
	}
	write(handle, gp_type_code, gul_type_code_len);
	close(handle);
	
	pfout=fopen(gp_college_info_filename, "wb");
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		fwrite(pcol_node,sizeof(COLLEGE_NODE),1,pfout);
	}
	fclose(pfout);
	
	pfout=fopen(gp_team_info_filename, "wb");
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		pteamnode=pcol_node->tnext;
		while(pteamnode!=NULL)
		{
			fwrite(pteamnode,sizeof(TEAM_NODE),1,pfout);
			pteamnode=pteamnode->next; 
		}
	}
	fclose(pfout);
	
	pfout=fopen(gp_project_info_filename,"wb");
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		pteamnode=pcol_node->tnext;
		while(pteamnode!=NULL)
		{
			ppro_node=pteamnode->pnext;
			while(ppro_node!=NULL)
			{
				fwrite(ppro_node,sizeof(PROJECT_NODE), 1, pfout);
				ppro_node=ppro_node->next; 
			}
			pteamnode=pteamnode->next;
		}
	}
	fclose(pfout);
	
	return TURE;
} 

/*
  	�������ƣ�BackupSysData
	�������ܣ���ϵͳ���������������ݱ��ݵ�һ�������ļ� 
	�������������ͷָ�룬�����ļ��� 
	����������� 
	����ֵ��BOOL���ͣ�����ΪTURE 
*/ 
BOOL BackupSysData(COLLEGE_NODE *hd, char *filename)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	unsigned long col_node_num=0;
	unsigned long teamnode_num=0;
	unsigned long pro_node_num=0;
	int handle;
	
	for(pcol_node=hd; pcol_node!=NULL; pcol_node=pcol_node->next)
	{
		col_node_num++;
		pteamnode=pcol_node->tnext;
		while(pteamnode!=NULL)
		{
			teamnode_num++;
			ppro_node=pteamnode->pnext;
			while(ppro_node!=NULL)
			{
				pro_node_num++;
				ppro_node=ppro_node->next;
			}
			pteamnode=pteamnode->next; 
		}
	}
	
	if((handle=open(filename,O_WRONLY | O_BINARY))==-1)
	{
		handle=open(filename,O_CREAT|O_BINARY, S_IWRITE);
		
		write(handle,(char*)&gul_type_code_len,sizeof(gul_type_code_len));
		write(handle,(char*)&col_node_num,sizeof(col_node_num));
		write(handle,(char*)&teamnode_num,sizeof(teamnode_num));
		write(handle,(char*)&pro_node_num,sizeof(pro_node_num));
		
		write(handle,gp_type_code,gul_type_code_len);
		
		for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
		{
			write(handle, (char*)pcol_node,sizeof(pcol_node));
		}
		for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
		{
			pteamnode=pcol_node->tnext;
			while(pteamnode!=NULL)
			{
				write(handle, (char*)pteamnode, sizeof(pteamnode));
				pteamnode=pteamnode->next;
			}
		}
		for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
		{
			pteamnode=pcol_node->tnext;
			while(pteamnode!=NULL)
			{
				ppro_node=pteamnode->pnext;
				while(ppro_node!=NULL)
				{
					write(handle,(char*)ppro_node,sizeof(ppro_node));
					ppro_node=ppro_node->next; 
				}
				pteamnode=pteamnode->next;
			} 
		}
	}
	close(handle);
	
	return TURE;
} 

/*
  	�������ƣ�RestoreSysData
	�������ܣ���ָ�������ļ��лָ�ϵͳ����������������� 
	�������������ͷָ���ַ����ű��������ļ����ļ��� 
	����������� 
	����ֵ��BOOL���ͣ�����ΪTURE 
*/ 
BOOL RestoreSysData(COLLEGE_NODE **phead, char *filename)
{
	COLLEGE_NODE *hd=NULL;
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	unsigned long col_node_num;
	unsigned long teamnode_num;
	unsigned long pro_node_num;
	unsigned long ulloop;
	int handle;
	int find;
	
	if((handle=open(filename, O_RDONLY|O_BINARY))==-1)
	{
		handle=open(filename, O_CREAT|O_BINARY, S_IREAD);
	}
	
	read(handle, (char*)&gul_type_code_len, sizeof(gul_type_code_len));
	read(handle, (char*)&col_node_num,sizeof(col_node_num));
	read(handle, (char*)&teamnode_num,sizeof(teamnode_num));
	read(handle, (char*)&pro_node_num,sizeof(pro_node_num));
	read(handle, gp_type_code, gul_type_code_len);
	
	for(ulloop=1;ulloop<col_node_num;ulloop++)
	{
		pcol_node=(COLLEGE_NODE *)malloc(sizeof(COLLEGE_NODE));
		read(handle, (char*)pcol_node,sizeof(COLLEGE_NODE));
		pcol_node->tnext=NULL;
		pcol_node->next=hd;
		hd=pcol_node; 
	}
	*phead=hd;
	
	for(ulloop=1;ulloop<=teamnode_num;ulloop++)
	{
		pteamnode=(TEAM_NODE*)malloc(sizeof(TEAM_NODE));
		read(handle, (char*)pteamnode,sizeof(TEAM_NODE));
		pcol_node=hd;
		while(pcol_node!=NULL&&strcmp(pcol_node->name,pteamnode->college)!=0)
		{
			pcol_node=pcol_node->next;
		}
		if(pcol_node!=NULL)
		{
			pteamnode->next=pcol_node->tnext;
			pcol_node->tnext=pteamnode;
		}
		else free(pteamnode);
	}
	
	for(ulloop=1;ulloop<=pro_node_num;ulloop++)
	{
		pcol_node=(COLLEGE_NODE *)malloc(sizeof(COLLEGE_NODE));
		read(handle, (char*)ppro_node,sizeof(PROJECT_NODE));
		pcol_node=hd;
		find=0;
		
		while(pcol_node!=NULL && find==0)
		{
			pteamnode=pcol_node->tnext;
			while(pteamnode!=NULL&&find==0)
			{
				if(strcmp(pteamnode->name, ppro_node->team)==0)
				{
					find=1;
					break;
				}
				pteamnode=pteamnode->next;
			}
			pcol_node=pcol_node->next;
		}
		if(find)
		{
		    ppro_node->next=pteamnode->pnext;
		    pteamnode->pnext=ppro_node;
		}
		else
		{
	        free(ppro_node);
	    }
	}
	close(handle);
	SaveSysData(hd);
	
	return TURE;
}

/*
  	�������ƣ�MarchString 
	�������ܣ��жϸ������ַ����Ƿ�ƥ�� 
	��������������ַ����������ַ��� 
	����������� 
	����ֵ��BOOL���ͣ�ƥ��ɹ�����TURE��ƥ��ʧ�ܷ���FALSE 
*/
BOOL MarchString(char *string_item, char *cond)
{
	char *sub_string_pos;
	BOOL bRet=FALSE;
	sub_string_pos=strstr(string_item,cond);
	if(sub_string_pos!=NULL) bRet=TURE;
	return bRet;
} 
 
/*
  	�������ƣ�SeekColNodeMan
	�������ܣ���Ժϵ�����˲�������������Ժϵ 
	�������������ͷָ�룬Ժϵ������ 
	����������� 
	����ֵ�����ҵ����ؽ�������ͷָ�룬δ�ҵ�����NULL 
*/
COLLEGE_NODE *SeekColNodeMan(COLLEGE_NODE *hd, char* dutyman)
{
	COLLEGE_NODE *pcol_node;
	COLLEGE_NODE *pcol_node_ret=NULL;
	COLLEGE_NODE *pcol_node_temp;
	int find=0;

	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		if(strcmp(pcol_node->dutyman,dutyman)==0)
		{
			pcol_node_temp=(COLLEGE_NODE *)malloc(sizeof(COLLEGE_NODE));
			*pcol_node_temp=*pcol_node;
			pcol_node_temp->next=pcol_node_ret;
			pcol_node_ret=pcol_node_temp;
			find=1;
		} 
	}
	
	if(find) return pcol_node_ret;
	else return NULL;
} 

/*
  	�������ƣ�SeekCollegeNodeNameM 
	�������ܣ�����Ժϵ���Ƶ�ȫ���򲿷ֲ��ҷ���������Ժϵ��Ϣ 
	�������������ͷָ�룬�ؼ��ַ��� 
	����������� 
	����ֵ�����ҵ����ؽ������ͷָ�룬δ�ҵ�����NULL
*/
COLLEGE_NODE *SeekCollegeNodeNameM(COLLEGE_NODE *hd, char *namem)
{
	COLLEGE_NODE *pcol_node;
	COLLEGE_NODE *pcol_node_ret=NULL;
	COLLEGE_NODE *pcol_node_temp;
	int find=0;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		if(MarchString(pcol_node->name,namem)==TURE)
		{
			pcol_node_temp=(COLLEGE_NODE *)malloc(sizeof(COLLEGE_NODE));
			*pcol_node_temp=*pcol_node;
			pcol_node_temp->next=pcol_node_ret;
			pcol_node_ret=pcol_node_temp;
			find=1;
		} 
	}
	if(find) return pcol_node_ret;
	else return NULL; 
} 

/*
  	�������ƣ�SeekTeamNodeNameM
	�������ܣ������Ŷ����Ƶ�ȫ���򲿷ֲ��ҷ����������Ŷ���Ϣ 
	�������������ͷָ�룬�ؼ��ַ��� 
	����������� 
	����ֵ�����ҵ����ؽ������ͷָ�룬δ�ҵ�����NULL 
*/
TEAM_NODE *SeekTeamNodeNameM(COLLEGE_NODE *hd, char *namem)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	TEAM_NODE *pteamnode_ret=NULL;
	TEAM_NODE *pteamnode_temp;
	int find=0; 
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			if(MarchString(pteamnode->name,namem)==TURE)
			{
				pteamnode_temp=(TEAM_NODE *)malloc(sizeof(TEAM_NODE));
				*pteamnode_temp=*pteamnode;
				pteamnode_temp->next=pteamnode_ret;
				pteamnode_ret=pteamnode_temp; 
				find=1;
			}
		}
	}
	if(find) return pteamnode_ret;
	else return NULL;
}

/*
  	�������ƣ�SeekTeamNodeTeacher
	�������ܣ�����ʦ�������ҷ��������Ŀ����Ŷ� 
	�������������ͷָ�룬��ʦ���� 
	����������� 
	����ֵ�� ���ҵ����ؽ������ͷָ�룬δ�ҵ�����NULL
*/
TEAM_NODE *SeekTeamNodeTeacher(COLLEGE_NODE *hd, char mark, int num_tc)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	TEAM_NODE *pteamnode_ret=NULL;
	TEAM_NODE *pteamnode_temp;
	int find=0;
	switch(mark)
	{
		case'=':
		{
		    for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
			{
  		        pteamnode=pcol_node->tnext;
		    	while(pteamnode!=NULL)
				{
   			        if(pteamnode->num_tc==num_tc)
					{
				        pteamnode_temp=(TEAM_NODE *)malloc(sizeof(TEAM_NODE));
					    *pteamnode_temp=*pteamnode;
						pteamnode_temp->next=pteamnode_ret;
						pteamnode_ret=pteamnode_temp; 
						find=1;
					}
					pteamnode=pteamnode->next;
				}
			}
			if(find) return pteamnode_ret;
			else return NULL;
		}
		case'<':
		{
			for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
			{
  		        pteamnode=pcol_node->tnext;
		    	while(pteamnode!=NULL)
				{
   			        if(pteamnode->num_tc<num_tc)
					{
				        pteamnode_temp=(TEAM_NODE *)malloc(sizeof(TEAM_NODE));
					    *pteamnode_temp=*pteamnode;
						pteamnode_temp->next=pteamnode_ret;
						pteamnode_ret=pteamnode_temp; 
						find=1;
					}
					pteamnode=pteamnode->next;
				}
			}
			if(find) return pteamnode_ret;
			else return NULL;	
		}
		case'>':
		{
			for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
			{
  		        pteamnode=pcol_node->tnext;
		    	while(pteamnode!=NULL)
				{
   			        if(pteamnode->num_tc>num_tc)
					{
				        pteamnode_temp=(TEAM_NODE *)malloc(sizeof(TEAM_NODE));
					    *pteamnode_temp=*pteamnode;
						pteamnode_temp->next=pteamnode_ret;
						pteamnode_ret=pteamnode_temp; 
						find=1;
					}
					pteamnode=pteamnode->next;
				}
			}
			if(find) return pteamnode_ret;
			else return NULL;
		}
		default:
			return NULL;
	}
}

/*
  	�������ƣ�SeekProjectNodeNum
	�������ܣ�������Ŀ��Ų��ҷ��������Ŀ�����Ŀ��Ϣ�ڵ� 
	�������������ͷָ�룬��Ŀ��� 
	����������� 
	����ֵ�����ҵ����ؽ������ͷָ�룬δ�ҵ�����NULL 
*/
PROJECT_NODE *SeekProjectNodeNum(COLLEGE_NODE *hd, char *p_num)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	PROJECT_NODE *ppro_node_ret=NULL;
	PROJECT_NODE *ppro_node_temp;
	int find=0;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			ppro_node=pteamnode->pnext;
			while(ppro_node!=NULL)
			{
				if(strcmp(ppro_node->num, p_num)==0)
				{
					ppro_node_temp=(PROJECT_NODE*)malloc(sizeof(PROJECT_NODE));
					*ppro_node_temp=*ppro_node;
					ppro_node_temp->next=ppro_node_ret;
					ppro_node_ret=ppro_node_temp;
					find=1;
				}
				ppro_node=ppro_node->next;
			}
		}
	} 
	if(find) return ppro_node_ret;
	else return NULL;
} 

/*
  	�������ƣ�SeekProjectNodeT 
	�������ܣ��������ŶӲ��ҷ��������Ŀ�����Ŀ 
	�������������ͷָ�룬�Ŷ����� 
	����������� 
	����ֵ�����ҵ����ؽ������ͷָ�룬δ�ҵ�����NULL 
*/
PROJECT_NODE *SeekProjectNodeT(COLLEGE_NODE *hd,char *tname)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	int find=0;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			if(strcmp(pteamnode->name, tname)==0)
			{
				ppro_node=pteamnode->pnext;
				find=1;
				break;
			}
		}
	}
	if(find) return ppro_node;
	else return NULL;
}

/*
  	�������ƣ�StatPeopleRate
	�������ܣ�ͳ��Ժϵ�о�������ʦ�����������Ȳ����� 
	�������������ͷָ�� 
	����������� 
	����ֵ��ָ��ͳ��������������ָ�� 
*/
PEOPLES_NODE *StatPeopleRate(COLLEGE_NODE *hd)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PEOPLES_NODE *ppeo_node;
	PEOPLES_NODE *ppeo_node_hd;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		ppeo_node=(PEOPLES_NODE *)malloc(sizeof(PEOPLES_NODE));
		ppeo_node->col_name=pcol_node->name;
		ppeo_node->student=0;
		ppeo_node->teacher=0;
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			ppeo_node->student+=pteamnode->num_stu;
			ppeo_node->teacher+=pteamnode->num_tc;
		}
		ppeo_node->scale=ppeo_node->student/ppeo_node->teacher;
		
		ppeo_node->next=ppeo_node_hd;
		ppeo_node_hd=ppeo_node;
	}
	SortPeopleInfo(ppeo_node_hd);
	
	return ppeo_node_hd; 
}

 
/*
  	�������ƣ�SortPeopleInfo
	�������ܣ�����������Ϣ���� 
	�����������������Ϣ����ͷ 
	��������������������ͷ 
	����ֵ���� 
*/
PEOPLES_NODE *SortPeopleInfo(PEOPLES_NODE *ppeo_node_hd)
{
	PEOPLES_NODE *ppeo_node_prior;
	PEOPLES_NODE *ppeo_node_after;
	PEOPLES_NODE *ppeo_node_cur;
	PEOPLES_NODE *ppeo_node_temp;
	ppeo_node_prior=ppeo_node_hd;
	if(ppeo_node_prior==NULL)
	{
		return ;
	}
	
	ppeo_node_temp=(PEOPLES_NODE *)malloc(sizeof(PEOPLES_NODE));
	
	while(ppeo_node_prior->next!=NULL)
	{
		ppeo_node_cur=ppeo_node_prior;
		ppeo_node_after=ppeo_node_prior->next;
		while(ppeo_node_after!=NULL)
		{
			if(ppeo_node_cur->scale<ppeo_node_cur->scale)
			{
				ppeo_node_cur=ppeo_node_after;
			}
			ppeo_node_after=ppeo_node_after->next;
		}
		if(ppeo_node_cur!=ppeo_node_prior)
		{
		    *ppeo_node_temp=*ppeo_node_prior;
			*ppeo_node_prior=*ppeo_node_cur;
			ppeo_node_prior->next=ppeo_node_temp->next;
			ppeo_node_temp->next=ppeo_node_cur->next;
			*ppeo_node_cur=*ppeo_node_temp; 
		}
		ppeo_node_prior=ppeo_node_prior->next;
	}
	free(ppeo_node_temp);
	
	return ;
} 

/*
  	�������ƣ�StatCollegeProjectNum
	�������ܣ�ͳ�Ƹ��������Ŀ���������� 
	�������������ͷָ�� 
	����������� 
	����ֵ��ָ���������ͷָ�� 
*/
PROJECT_NUM_NODE *StatCollegeProjectNum(COLLEGE_NODE *hd)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	PROJECT_NUM_NODE *ppro_num_node;
	PROJECT_NUM_NODE *ppro_num_node_hd;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		ppro_num_node=(PROJECT_NUM_NODE *)malloc(sizeof(PROJECT_NUM_NODE));
		ppro_num_node->col_name=pcol_node->name;
		ppro_num_node->total=0;
		ppro_num_node->p973=0;
		ppro_num_node->p863=0;
		ppro_num_node->money=0; 
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			for(ppro_node=pteamnode->pnext;ppro_node!=NULL;ppro_node=ppro_node->next)
			{
				ppro_num_node->total++;
				ppro_num_node->money+=ppro_node->money;
				if(ppro_node->type=='1') ppro_num_node->p973++;
				else if(ppro_node->type=='3') ppro_num_node->p863++;
			}
		}
		ppro_num_node->next=ppro_num_node_hd;
		ppro_num_node_hd=ppro_num_node;
	}
	
	SortColProInfo(ppro_num_node_hd);
	
	return ppro_num_node_hd;
} 

/*
  	�������ƣ�SortColProInfo
	�������ܣ��Կ�����Ŀ�������� 
	���������������Ŀ��Ŀ��Ϣ����ͷ 
	��������������������ͷ 
	����ֵ���� 
*/
PROJECT_NUM_NODE *SortColProInfo(PROJECT_NUM_NODE *ppro_num_node_hd)
{
	PROJECT_NUM_NODE *ppro_num_node_prior;
	PROJECT_NUM_NODE *ppro_num_node_after;
	PROJECT_NUM_NODE *ppro_num_node_cur;
	PROJECT_NUM_NODE *ppro_num_node_temp;
	ppro_num_node_prior=ppro_num_node_hd;
	if(ppro_num_node_prior==NULL)
	{
		return ;
	}
	
	ppro_num_node_temp=(PROJECT_NUM_NODE *)malloc(sizeof(PROJECT_NUM_NODE));
	while(ppro_num_node_prior->next!=NULL)
	{
		ppro_num_node_cur=ppro_num_node_prior;
		ppro_num_node_after=ppro_num_node_prior->next;
		while(ppro_num_node_after!=NULL)
		{
			if(ppro_num_node_cur->total<ppro_num_node_after->total)
			{
				ppro_num_node_cur=ppro_num_node_after;
			}
			ppro_num_node_after=ppro_num_node_after->next;
		}
		
		if(ppro_num_node_cur!=ppro_num_node_prior)
		{
			*ppro_num_node_temp=*ppro_num_node_prior;
			*ppro_num_node_prior=*ppro_num_node_cur;
			ppro_num_node_prior->next=ppro_num_node_temp->next;
			ppro_num_node_temp->next=ppro_num_node_cur->next;
			*ppro_num_node_cur=*ppro_num_node_temp;
		}
		ppro_num_node_prior=ppro_num_node_prior->next;
	}
	free(ppro_num_node_temp);
	
	return ;
}

/*
  	�������ƣ�StatTeamNasiTen
	�������ܣ�ͳ���Ŷ���Ȼ��ѧ������Ŀ��������
	�������������ͷָ�� 
	����������� 
	����ֵ��ָ���������ͷָ�� 
*/
TEAM_NASI_NODE *StatTeamNasiTen(COLLEGE_NODE *hd)
{
	COLLEGE_NODE *pcol_node;
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node;
	TEAM_NASI_NODE *pteam_nasi_node;
	TEAM_NASI_NODE *pteam_nasi_node_hd;
	
	for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
	{
		for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
		{
			pteam_nasi_node=(TEAM_NASI_NODE *)malloc(sizeof(TEAM_NASI_NODE));
			pteam_nasi_node->team_name=pteamnode->name;
			pteam_nasi_node->money=0;
			pteam_nasi_node->num_nasi=0;
			for(ppro_node=pteamnode->pnext;ppro_node!=NULL;ppro_node=ppro_node->next)
			{
				pteam_nasi_node->money+=ppro_node->money;
				if(ppro_node->type=='2') pteam_nasi_node->num_nasi++;
			}
			pteam_nasi_node=pteam_nasi_node_hd;
			pteam_nasi_node_hd=pteam_nasi_node->next;
		}
	}
	SortTeamNasiInfo(pteam_nasi_node_hd);
	
	return pteam_nasi_node_hd;
}

/*
  	�������ƣ�SortTeamNasiInfo
	�������ܣ�����Ȼ��ѧ������Ŀ��Ϣ������ 
	�����������Ȼ��ѧ������Ŀ��Ϣ����ͷ
	������������������ͷ 
	����ֵ�� �� 
*/
TEAM_NASI_NODE *SortTeamNasiInfo(TEAM_NASI_NODE *pteam_nasi_node_hd)
{
	TEAM_NASI_NODE *pteam_nasi_node_prior;
	TEAM_NASI_NODE *pteam_nasi_node_after;
	TEAM_NASI_NODE *pteam_nasi_node_cur;
	TEAM_NASI_NODE *pteam_nasi_node_temp;
	pteam_nasi_node_prior=pteam_nasi_node_hd;
	if(pteam_nasi_node_prior==NULL)
	{
		return ;
	}
	
	pteam_nasi_node_temp=(TEAM_NASI_NODE *)malloc(sizeof(TEAM_NASI_NODE));
	while(pteam_nasi_node_prior->next!=NULL)
	{
		pteam_nasi_node_cur=pteam_nasi_node_prior;
		pteam_nasi_node_after=pteam_nasi_node_prior->next;
		while(pteam_nasi_node_after!=NULL)
		{
			if(pteam_nasi_node_cur->num_nasi<pteam_nasi_node_after->num_nasi)
			{
				pteam_nasi_node_cur=pteam_nasi_node_after;
			}
			pteam_nasi_node_after=pteam_nasi_node_after->next;
		}
		
		if(pteam_nasi_node_cur!=pteam_nasi_node_prior)
		{
			*pteam_nasi_node_temp=*pteam_nasi_node_prior;
			*pteam_nasi_node_prior=*pteam_nasi_node_cur;
			pteam_nasi_node_prior->next=pteam_nasi_node_temp->next;
			pteam_nasi_node_temp->next=pteam_nasi_node_cur->next;
			*pteam_nasi_node_cur=*pteam_nasi_node_temp;
		}
		pteam_nasi_node_prior=pteam_nasi_node_prior->next;
	}
	free(pteam_nasi_node_temp);
	
	return ; 
} 

/*
  	�������ƣ�StatProTeaScale
	�������ܣ�ͳ���Ŷ��о���Ŀ�ͽ�ʦ�����Ȳ����� 
	�������������ͷָ�� 
	����������� 
	����ֵ��ָ��������ͷָ�� 
*/
TEAM_PROTEA_NODE *StatProTeaScale(COLLEGE_NODE *hd)
{
    COLLEGE_NODE *pcol_node;
    TEAM_NODE *pteamnode;
    PROJECT_NODE *ppro_node;
    TEAM_PROTEA_NODE *protea_node;
    TEAM_PROTEA_NODE *protea_node_hd;
    
    for(pcol_node=hd;pcol_node!=NULL;pcol_node=pcol_node->next)
    {
    	for(pteamnode=pcol_node->tnext;pteamnode!=NULL;pteamnode=pteamnode->next)
    	{
    		protea_node=(TEAM_PROTEA_NODE *)malloc(sizeof(TEAM_PROTEA_NODE));
    		protea_node->num_pro=0;
    		protea_node->num_tc=pteamnode->num_tc;
    		for(ppro_node=pteamnode->pnext;ppro_node!=NULL;pteamnode=pteamnode->next)
    		{
    			protea_node->num_pro++;
			}
			protea_node->scale=protea_node->num_pro/protea_node->num_tc;
			protea_node=protea_node_hd;
			protea_node_hd=protea_node->next;
		}
	}
	SortProTeaInfo(protea_node_hd);
	
	return protea_node_hd;
} 

/*
  �������ƣ�SortProTeaInfo
  �������ܣ����ݱ������Ŷӿ�����Ŀ���ʦ��������Ϣ���� 
  ����������Ŷӿ�����Ŀ��ʦ��������ͷָ�� 
  �������������������ͷָ�� 
  ����ֵ���� 
*/
TEAM_PROTEA_NODE *SortProTeaInfo(TEAM_PROTEA_NODE *protea_node_hd)
{
	TEAM_PROTEA_NODE *protea_node_prior;
	TEAM_PROTEA_NODE *protea_node_after; 
	TEAM_PROTEA_NODE *protea_node_cur;
	TEAM_PROTEA_NODE *protea_node_temp;
	protea_node_prior=protea_node_hd;
	if(protea_node_prior==NULL)
	{
		return ;
	}
	
	protea_node_temp=(TEAM_PROTEA_NODE *)malloc(sizeof(TEAM_PROTEA_NODE));
	while(protea_node_prior->next!=NULL)
	{
		protea_node_cur=protea_node_prior;
		protea_node_after=protea_node_prior->next;
		while(protea_node_after!=NULL)
		{
			if(protea_node_cur->scale>protea_node_after->scale)
			{
				protea_node_cur=protea_node_after;
			}
			protea_node_after=protea_node_after->next;
		}
		if(protea_node_cur!=protea_node_prior)
		{
			*protea_node_temp=*protea_node_prior;
			*protea_node_prior=*protea_node_cur;
			protea_node_prior->next=protea_node_temp->next;
			protea_node_temp->next=protea_node_cur->next;
		}
		protea_node_prior=protea_node_prior->next;
	}
	free(protea_node_temp);
	
	return ;
} 

BOOL SaveData(void)
{
	BOOL bRet=TURE;
	char *plabel_name[]={
	"���˵���ļ�", 
	"�Ӳ˵�����ݱ���",
	"ȷ��" 
	};
	
	ShowModule(plabel_name,3);
	
	return bRet; 
}

BOOL BackupData(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݱ���",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵���ļ�",
                           "�Ӳ˵�����ݻָ�",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL ExitSys(void)
{
	LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"ȷ���˳�ϵͳ��", "ȷ��    ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
                    {rcPop.Left+5, rcPop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{rcPop.Left + 5, rcPop.Top + 5,
                           rcPop.Left + 8, rcPop.Top + 5},
                          {rcPop.Left + 13, rcPop.Top + 5,
                           rcPop.Left + 16, rcPop.Top + 5}};
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();

    return bRet;
}
BOOL TypeCode(void)
{
	BOOL bRet = TRUE;
	printf("��Ŀ�����룺\n");
	printf("1    973�ƻ���Ŀ\n"); 
	printf("2    ������Ȼ��ѧ������Ŀ\n"); 
	printf("3    863�ƻ���Ŀ\n"); 
	printf("4    ���ʺ�����Ŀ\n"); 
	printf("5    ������Ŀ\n"); 

    return bRet;
}
BOOL MaintainCollegeInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵��Ժϵ������Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL MaintainTeamInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵���Ŷӻ�����Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL MaintainProjectInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������ά��",
                           "�Ӳ˵��������Ŀ������Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL QueryCollegeInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵��Ժϵ������Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL QueryTeamInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵���Ŷӻ�����Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet; 
}
BOOL QueryProjectInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵�����ݲ�ѯ",
                           "�Ӳ˵����Ŀ������Ϣ",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL StatManRate(void)
{
	return ;
}
BOOL StatProjectType(void)
{
	return ;
}
BOOL StatTeam(void)
{
	return ;
}
BOOL StatProjectPer(void)
{
	return ;
}
BOOL Stat(void)
{
	return ;
}
BOOL HelpTopic(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������",
                           "�Ӳ˵����������",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL AboutDorm(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"���˵������",
                           "�Ӳ˵������...",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

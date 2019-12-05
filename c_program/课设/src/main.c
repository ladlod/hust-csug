#include "science.h"

unsigned long ul;
int main()
{
 	COORD size = {SCR_COL, SCR_ROW};              /*窗口缓冲区大小*/

    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE); /* 获取标准输出设备句柄*/
    gh_std_in = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/

    SetConsoleTitle(gp_sys_name);                 /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/
	
	LoadDate(); 
    InitInterface();          /*界面初始化*/

    RunSys(&gp_head);
	CloseSys(gp_head);

    return 0;
}


/*
  	函数名称: InitInterface
	函数功能: 初始化界面.
	输入参数: 无
	输出参数: 无
	返 回 值: 无
*/
void InitInterface()
{
    WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
               | BACKGROUND_BLUE;  /*黄色前景和蓝色背景*/

    SetConsoleTextAttribute(gh_std_out, att);  /*设置控制台屏幕缓冲区字符属性*/

    ClearScreen();  /* 清屏*/

    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;

    ShowMenu();     /*显示菜单栏*/
    ShowState();    /*显示状态栏*/

    return;
}

/*
    函数名称：ShowMenu
    函数功能：显示主菜单，并设置热区，在主菜单第一项上置选标记
    输入参数：无
    输出参数：无
    返回值：无
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
    SetConsoleCursorPosition(gh_std_out, endPos);  /*将光标位置设置在第2行第1列*/

    /*将菜单项置为热区，热区编号为菜单项号，热区类型为0(按钮型)*/
    i = 0;
    do
    {
        PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
        for (j=PosA; j<PosB; j++)
        {
            gp_scr_att[j] |= (i+1) << 2; /*设置菜单项所在字符单元的属性值*/
        }
        PosA = PosB + 4;
        i++;
    } while (i<5);

    TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/

	return ;
}

/*
    函数名称：TagMainMenu
    函数功能：在主菜单中选中标志
    输入参数：num 选中的主菜单编号
    输出参数：无
    返回值：无
*/
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    char ch;
    int i;

    if (num == 0) /*num为0时，将会去除主菜单项选中标记*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
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

    /*去除选中菜单项前面的菜单项选中标记*/
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

    /*在选中菜单项上做标记，黑底白字*/
    for (i=PosA; i<PosB; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
                                               | FOREGROUND_RED;
    }

    /*去除选中菜单项后面的菜单项选中标记*/
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

    /*将做好标记的菜单条信息写到窗口第一行*/
    SMALL_RECT rcMenu ={0, 0, size.X-1, 0};
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

    return;
}

/*
    函数名称：ClearScreen
    函数功能：清除屏幕信息
    输入参数：无
    输出参数：无
    返回值：无
*/
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 0};
    unsigned long size;

    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*取屏幕缓冲区信息*/
    size = bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/

    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home,&ul);

    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(gh_std_out,' ', size, home,&ul);

    return;
}

/*
  	函数名称：PopMenu
	函数功能：弹出主菜单项对应的子菜单 
	输入参数：指定的主菜单号 
	输出参数：无 
	返回值：无 
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

    if (num != gi_sel_menu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();
            gi_sel_sub_menu = 0;
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
    {
        return;
    }

    gi_sel_menu = num;    /*将选中主菜单项置为指定的主菜单项*/
    TagMainMenu(gi_sel_menu); /*在选中的主菜单项上做标记*/
    LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/

    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
    {
        loc += ga_sub_menu_count[i-1];
    }
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    COORD aLoc[labels.num];/*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT aArea[areas.num];                    /*定义数组存放所有热区位置*/
    char aSort[areas.num];                      /*定义数组存放所有热区对应类别*/
    char aTag[areas.num];                         /*定义数组存放每个热区的编号*/
    for (i=0; i<areas.num; i++)
    {
        aArea[i].Left = rcPop.Left + 2;  /*热区定位*/
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;       /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1;           /*热区按顺序编号*/
    }
    areas.pArea = aArea;/*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;/*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;   /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    PopUp(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);  /*给弹出窗口画边框*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
        pCh = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pCh)==0) /*串长为0，表明为空串*/
        {   /*首先画横线*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3, pos, &ul);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*取消该区域字符单元的热区属性*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }

    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*跳过空串*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    return;
}

/*
  	函数名称：PopUp
	函数功能：在指定区域输出窗口信息并设置热区，将弹出窗口信息入栈 
	输入参数：弹出窗口位置数据存放地址，弹出窗口区域字符属性，弹出窗口中标签束信息存放地址，弹出窗口中热区信息存放地址 
	输出参数：无 
	返回值：无 
*/
void PopUp(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
	LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j, row;

    /*弹出窗口所在位置字符单元信息入栈*/
    size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y*sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X*size.Y*sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
        {
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),
                                        pLabel->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
        }
    }

    for (i=0; i<pHotArea->num; i++)
    {   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
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
  	函数名称：PopOff
	函数功能：关闭顶层弹出窗口，恢复覆盖区域原外观和字符单元属性 
	输入参数：无 
	输出参数：无 
	返回值：无 
*/
void PopOff(void)
{
	LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;

    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
    {   /*栈底存放的主界面屏幕信息，不用关闭*/
        return;
    }
    nextLayer = gp_top_layer->next;
    /*恢复弹出窗口区域原外观*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*恢复字符单元原属性*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    free(gp_top_layer->pContent);    /*释放动态存储区*/
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
} 

/*
  	函数名称：DrawBox
	函数功能：在指定区域画边框 
	输入参数：存放区域位置信息的地址 
	输出参数：无 
	返回值： 无 
*/
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*画框用的字符*/
    COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/

    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*此循环画上边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*此循环画边框左边线和右边线*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*画下边框横线*/
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
    return;
}

/*
  	函数名称：TagSubMenu
	函数功能：在指定子菜单做选中标记 
	输入参数：选中的子菜单号 
	输出参数：无 
	返回值：无 
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    int width;

    LocSubMenu(gi_sel_menu, &rcPop);  /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom-rcPop.Top-1))
    {   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
        return;
    }

    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 3;
    if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
        pos.X += 1;
        att |=  FOREGROUND_RED;/*白底红字*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
    }
    /*在制定子菜单项上做选中标记*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
    return;
}

/*
  	函数名称：LocSubMenu
	函数功能：计算弹出子菜单区域左上角和右下角的位置 
	输入参数：选中的主菜单项号 
	输出参数：存放区域位置信息的地址 
	返回值：无 
*/
void LocSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*区域的上边定在第2行，行号为1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }
    rc->Right += rc->Left + 3;  /*计算区域的右边界*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*计算区域下边的行号*/
    if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
} 

/*
  	函数名称: RunSys
	函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
	输入参数: 无
	输出参数: 主链头指针的地址
	返 回 值: 无
*/
void RunSys(COLLEGE_NODE **phead)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    while (bRet)
    {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);

        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
            {
                /* cAtt > 0 表明该位置处于热区(主菜单项字符单元)
                 * cAtt != gi_sel_menu 表明该位置的主菜单项未被选中
                 * gp_top_layer->LayerNo > 0 表明当前有子菜单弹出
                 */
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
            {
                TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
                    {
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    }
                    /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
                    {
                        PopOff(); /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/

                        /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                 && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

            /*系统快捷键的处理*/
            if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
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
            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
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
                else  /*已弹出子菜单时*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
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
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 70: /*f或F*/
                            PopMenu(1);
                            break;
                        case 77: /*m或M*/
                            PopMenu(2);
                            break;
                        case 81: /*q或Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s或S*/
                            PopMenu(4);
                            break;
                        case 72: /*h或H*/
                            PopMenu(5);
                            break;
                        case 13: /*回车*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*如果匹配成功*/
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
 * 函数名称: ShowState
 * 函数功能: 显示状态条.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明: 状态条字符属性为白底黑字, 初始状态无状态信息.
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

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
  	函数名称：CloseSys 
	函数功能：关闭系统 
	输入参数：主链头指针 
	输出参数：无 
	返回值： 无 
*/
void CloseSys(COLLEGE_NODE *hd)
{
    COLLEGE_NODE *pcol_node1=hd, *pcol_node2;
    TEAM_NODE *pteamnode1, *pteamnode2;
    PROJECT_NODE *ppro_node1, *ppro_node2;

    while (pcol_node1 != NULL) /*释放十字交叉链表的动态存储区*/
    {
        pcol_node2 = pcol_node1->next;
        pteamnode1 = pcol_node1->tnext;
        while (pteamnode1 != NULL) /*释放学生基本信息支链的动态存储区*/
        {
            pteamnode2 = pteamnode1->next;
            ppro_node1 = pteamnode1->pnext;
            while (ppro_node1 != NULL) /*释放缴费信息支链的动态存储区*/
            {
                ppro_node2 = ppro_node1->next;
                free(ppro_node1);
                ppro_node1 = ppro_node2;
            }
            free(pteamnode1);
            pteamnode1 = pteamnode2;
        }
        free(pcol_node1);  /*释放主链结点的动态存储区*/
        pcol_node1 = pcol_node2;
    }

    ClearScreen();        /*清屏*/

    /*释放存放菜单条、状态条和项目类别代码等信息动态存储区*/
    free(gp_buff_menubar_info);
    free(gp_buff_stateBar_info);
    free(gp_type_code);

    /*关闭标准输入和输出设备句柄*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);

    /*将窗口标题栏置为运行结束*/
    SetConsoleTitle("运行结束");

    return;
} 


/*
 * 函数名称: ExeFunction
 * 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
 * 输入参数: m 主菜单项号，s 子菜单项号
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE 或 FALSE
 * 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ga_sub_menu_count[2]+ga_sub_menu_count[3]+ga_sub_menu_count[4]])(void);
    int i, loc;

    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
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
    
    for (i=1,loc=0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
    {
        loc += ga_sub_menu_count[i-1];
    }
    loc += s - 1;

    if (pFunction[loc] != NULL)
    {
        bRet = (*pFunction[loc])();  /*用函数指针调用所指向的功能函数*/
    }

    return bRet;
}


/*
  	函数名称：CreatList 
	函数功能：从文件读取数据，并存放到链表中 
	输入参数：无 
	输出参数：phead主链头指针的地址，用来返回创建的十字链表 
	返回值： int型，表示链表创建情况
	         0，空链
			 4，已加载院系基本信息
			 12，已加载院系，团队基本信息
			 28，已加载院系，团队，科研项目基本信息 
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
		printf("院系基本信息文件打开失败！\n");
		return re;
	}
	printf("院系基本信息文件打开成功！\n"); 
	/*从文件读取院系基本信息*/

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
	 	printf("院系基本信息文件加载失败！\n");
		return re;	
	}
	printf("院系基本信息文件加载成功！\n");
	
	*phead=hd;
	re += 4;
	if((pFile=fopen(gp_team_info_filename, "rb"))==NULL)
	{
		printf("团队基本信息文件打开失败！\n");
		return re;
	}
	printf("团队基本信息打开成功！\n");
	re += 8;
	
	/*将团队基本信息存入院系基本信息主链对应节点的支链中*/
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
		printf("科研项目基本信息文件打开失败！\n");
		return re;
	}
	printf("科研项目基本信息打开成功！\n");
	re+=16;
	
	/*将科研项目基本信息存入科研团队基本信息支链对应的节点的项目支链中*/
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
    函数名称：SeekCollegeNode
	函数功能：按院系名称查找指定院系基本信息节点
	输入参数：主链头指针，所查找的院系名称
	输出参数：无
	返回值：查中返回节点地址，没查中返回NULL
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
  	函数名称：SeekTeamNode
	函数功能：按团队名称查找指定科研团队基本信息节点 
	输入参数：主链头指针，所查找的团队名称 
	输出参数：无 
	返回值：查中返回节点地址，没查中返回NULL
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
  	函数名称：SeekProNode
	函数功能：按照团队名称，项目编号查找科研项目信息节点 
	输入参数：主链头指针，团队名称，项目编号 
	输出参数：无 
	返回值：查中返回节点地址，没查中返回NULL
*/
PROJECT_NODE *SeekProNode(COLLEGE_NODE *hd, char *team_name, char *num)
{
	TEAM_NODE *pteamnode;
	PROJECT_NODE *ppro_node; 
	int find = 0;
	pteamnode = SeekTeamNode(hd, team_name); /*查找团队信息节点*/
	
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
  	函数名称：InsertCollegeNode
	函数功能：在链表中插入一个院系基本信息节点 
	输入参数：主连头指针，所要插入的节点指针 
	输出参数：无 
	返回值：BOOL类型，TURE表示插入成功，FALSE表示插入失败 
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
  	函数名称：InsertTeamNode
	函数功能：在链表中插入一个团队基本信息节点 
	输入参数：主连头指针，所要插入的节点指针 
	输出参数：无 
	返回值：BOOL类型，TURE表示插入成功，FALSE表示插入失败 
*/
BOOL InsertTeamNode(COLLEGE_NODE *hd,TEAM_NODE *pteamnode)
{
	COLLEGE_NODE *pcol_node;
	
	/*在链表中找到对应的院系*/
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
  	函数名称：InsertProjrctNode
	函数功能：在链表中插入一个科研项目信息节点 
	输入参数：主连头指针，所要插入的节点指针 
	输出参数：无 
	返回值：BOOL类型，TURE表示插入成功，FALSE表示插入失败 
*/
BOOL InsertProjectNode(COLLEGE_NODE *hd,PROJECT_NODE *ppro_node)
{
	TEAM_NODE *pteamnode;
	
	/*在链表中找到对应的科研团队*/
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
  	函数名称：DelCollegeNode 
	函数功能：删除院系基本信息节点 
	输入参数：主链头指针，院系名称 
	输出参数：无 
	返回值：BOOL类型，TURE表示删除成功，FALSE表示删除失败
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
  	函数名称：DelTeamNode 
	函数功能：删除科研团队基本信息节点 
	输入参数：主链头指针,院系名称,团队名称 
	输出参数：无 
	返回值：BOOL类型，TURE表示删除成功，FALSE表示删除失败 
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
  	函数名称：DelProjectNode
	函数功能：删除科研项目信息节点 
	输入参数：主链头指针，团队名称，项目编号 
	输出参数：无 
	返回值：BOOL类型，TURE表示删除成功，FALSE表示删除失败 
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
  	函数名称：ModifCollegeNode
	函数功能：修改指定的院系基本信息 
	输入参数： 主链头指针，院系名称，存放修改内容节点的指针 
	输出参数：无 
	返回值：BOOL类型，TURE表示修改成功，FALSE表示修改失败 
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
  	函数名称：ModifTeamNode
	函数功能：修改指定的科研团队信息 
	输入参数：主链头指针，科研团队名称，存放修改内容节点的指针 
	输出参数：无 
	返回值：BOOL类型，TURE表示修改成功，FALSE表示修改失败
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
  	函数名称：ModifProjectNode
	函数功能：修改指定的科研项目信息 
	输入参数：主链头指针，科研项目编所属团队名称，科研项目编号，指向存放修改内容节点的指针  
	输出参数：无 
	返回值：BOOL类型，TURE表示修改成功，FALSE表示修改失败 
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
  	函数名称：LoadDate
	函数功能：将代码表和三类基础数据从数据文件载入到内存区和链表中 
	输入参数：无 
	输出参数：无 
	返回值：BOOL型，功能函数除了ExitSys的返回值可以为FALSE外，其他必须为TURE 
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
		printf("项目类别代码表加载失败！\n");
		gc_sys_state &= 0xfe;
	}
	else
	{
		printf("项目类别代码表加载成功！\n");
		gc_sys_state |= 1;
	}
	
	Re=CreatList(&gp_head);
	gc_sys_state |= Re;
	gc_sys_state &=~ (4+8+16-Re);
	if(gc_sys_state<(1|4|8|16))
	{
		printf("\n系统基础数据不完整！\n");
		printf("\n按任意键继续...\n");
		getch();
	}
	
	return TURE;
} 

/*
  	函数名称：LoadCode
	函数功能：将代码表从数据文件载入到内存缓冲区，并进行排序和去除空格 
	输入参数：FileName存放代码表的数据文件名 
	输出参数：指向内存缓冲区的指针地址 
	返回值：存放代码表的内存缓冲区的大小 
*/
int LoadCode(char *filename, char **pbuffer)
{
	char *pTemp, *pStr1, *pStr2;
    int handle;
    int BufferLen, len, loc1, loc2, i;
    long filelen;

    if ((handle = open(filename, O_RDONLY | O_TEXT)) == -1) /*如果以只读方式打开失败 */
    {
        handle = open(filename, O_CREAT | O_TEXT, S_IREAD); /*以创建方式打开*/
    }
    filelen = filelength(handle);      /*数据文件的长度*/
    pTemp = (char *)calloc(filelen + 1, sizeof(char)); /*申请同样大小的动态存储区*/
    BufferLen = read(handle, pTemp, filelen); /*将数据文件的内容全部读入到内存*/
    close(handle);

    *(pTemp + BufferLen) = '\0'; /*在动态存储区尾存一个空字符，作为字符串结束标志*/
    BufferLen++;

    for (i=0; i<BufferLen; i++) /*将动态存储区中的所有换行符替换成空字符*/
    {
        if (*(pTemp + i) == '\n')
        {
            *(pTemp + i) = '\0';
        }
    }

    /*再申请一块同样大小的动态存储区，用于存放排序后的代码串*/
    *pbuffer = (char *)calloc(BufferLen, sizeof(char));
    loc2 = 0;
    pStr1 = pTemp;
    len = strlen(pStr1);

    while (BufferLen > len + 1) /*选择法排序*/
    {
        loc1 = len + 1;
        while (BufferLen > loc1) /*每趟找到序列中最小代码串，首地址存入pStr1*/
        {
            pStr2 = pTemp + loc1;
            if (strcmp(pStr1, pStr2) > 0)
            {
                pStr1 = pStr2;
            }
            loc1 += strlen(pStr2) + 1;
        }
        len = strlen(pStr1);  /*这一趟所找到的最小代码串长度*/

        /*如果不是空串，则进行复制，loc2是下一个最小代码串存放地址的偏移量*/
        if (len > 0)
        {
            strcpy(*pbuffer + loc2, pStr1);
            loc2 += len + 1;  /*已复制的代码串所占存储空间大小*/
        }

        /*将最小代码串从序列中删除掉*/
        for(i=0; i<BufferLen-(pStr1-pTemp)-(len+1); i++)
        {
            *(pStr1 + i) = *(pStr1 + i + len + 1);
        }

        BufferLen -= len + 1; /*下一趟排序所处理序列的长度*/
        pStr1 = pTemp;  /*假定序列的第一个代码串为最小代码串*/
        len = strlen(pStr1);
    } /*序列中只剩下一个代码串时，排序结束*/

    /*复制最后这个代码串*/
    len = strlen(pStr1);
    strcpy(*pbuffer + loc2, pStr1);

    /*修改动态存储区大小，使其正好放下排序后代码串*/
    loc2 += len + 1;
    *pbuffer = (char *)realloc(*pbuffer, loc2);
    free(pTemp);  /*释放最先申请的动态存储区*/

    return loc2;  /*返回存放代码串的内存缓冲区实际大小*/
}

/*
 * 函数名称: DealInput
 * 函数功能: 在弹出窗口区域设置热区, 等待并相应用户输入.
 * 输入参数: 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 * 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号
 * 返 回 值:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {    /*循环*/
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
                {  /*方向键(左、上、右、下)的处理*/
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
            {  /*ESC键*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {  /*回车键或空格表示按下当前按钮*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

/*
  	函数名称：SetHotPoint
	函数功能：设置热区 
	输入参数：焦点热区存放地址，焦点热区编号 
	输出参数：无 
	返回值：无 
*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i=0; i<pHotArea->num; i++)
    {  /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*热区是按钮类*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*被激活热区是文本框类*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}

/*
  	函数名称：SaveSysData
	函数功能：保存系统代码表和三类基础数据 
	输入参数：主链头指针 
	输出参数：无 
	返回值：BOOL类型，总是为TURE 
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
  	函数名称：BackupSysData
	函数功能：将系统代码和三类基础数据备份到一个数据文件 
	输入参数：主链头指针，数据文件名 
	输出参数：无 
	返回值：BOOL类型，总是为TURE 
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
  	函数名称：RestoreSysData
	函数功能：从指定数据文件中恢复系统代码表和三类基础数据 
	输入参数：主链头指针地址，存放备份数据文件的文件名 
	输出参数：无 
	返回值：BOOL类型，总是为TURE 
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
  	函数名称：MarchString 
	函数功能：判断给定的字符串是否匹配 
	输入参数：给定字符串，条件字符串 
	输出参数：无 
	返回值：BOOL类型，匹配成功返回TURE，匹配失败返回FALSE 
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
  	函数名称：SeekColNodeMan
	函数功能：按院系负责人查找满足条件的院系 
	输入参数：主链头指针，院系负责人 
	输出参数：无 
	返回值：若找到返回结果链表的头指针，未找到返回NULL 
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
  	函数名称：SeekCollegeNodeNameM 
	函数功能：按照院系名称的全部或部分查找符合条件的院系信息 
	输入参数：主链头指针，关键字符串 
	输出参数：无 
	返回值：若找到返回结果链表头指针，未找到返回NULL
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
  	函数名称：SeekTeamNodeNameM
	函数功能：按照团队名称的全部或部分查找符合条件的团队信息 
	输入参数：主链头指针，关键字符串 
	输出参数：无 
	返回值：若找到返回结果链表头指针，未找到返回NULL 
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
  	函数名称：SeekTeamNodeTeacher
	函数功能：按教师人数查找符合条件的科研团队 
	输入参数：主链头指针，教师人数 
	输出参数：无 
	返回值： 若找到返回结果链表头指针，未找到返回NULL
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
  	函数名称：SeekProjectNodeNum
	函数功能：按照项目编号查找符合条件的科研项目信息节点 
	输入参数：主链头指针，项目编号 
	输出参数：无 
	返回值：若找到返回结果链表头指针，未找到返回NULL 
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
  	函数名称：SeekProjectNodeT 
	函数功能：按所属团队查找符合条件的科研项目 
	输入参数：主链头指针，团队名称 
	输出参数：无 
	返回值：若找到返回结果链表头指针，未找到返回NULL 
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
  	函数名称：StatPeopleRate
	函数功能：统计院系研究生，教师总数及人数比并排序 
	输入参数：主链头指针 
	输出参数：无 
	返回值：指向统计人数结果链表的指针 
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
  	函数名称：SortPeopleInfo
	函数功能：对人数比信息排序 
	输入参数：人数比信息链链头 
	输出参数：排序后链表链头 
	返回值：无 
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
  	函数名称：StatCollegeProjectNum
	函数功能：统计各类科研项目总数并排序 
	输入参数：主链头指针 
	输出参数：无 
	返回值：指向结果链表的头指针 
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
  	函数名称：SortColProInfo
	函数功能：对科研项目总数排序 
	输入参数：科研项目数目信息链链头 
	输出参数：排序后链表链头 
	返回值：无 
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
  	函数名称：StatTeamNasiTen
	函数功能：统计团队自然科学基金项目总数排名
	输入参数：主链头指针 
	输出参数：无 
	返回值：指向结果链表的头指针 
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
  	函数名称：SortTeamNasiInfo
	函数功能：对自然科学基金项目信息链排序 
	输入参数：自然科学基金项目信息链链头
	输出参数：排序后链表头 
	返回值： 无 
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
  	函数名称：StatProTeaScale
	函数功能：统计团队研究项目和教师人数比并排序 
	输入参数：主链头指针 
	输出参数：无 
	返回值：指向结果链表头指针 
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
  函数名称：SortProTeaInfo
  函数功能：根据比例对团队科研项目与教师人数比信息排序 
  输入参数：团队科研项目教师人数比链头指针 
  输出参数：排序后的链表头指针 
  返回值：无 
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
	"主菜单项：文件", 
	"子菜单项：数据保存",
	"确认" 
	};
	
	ShowModule(plabel_name,3);
	
	return bRet; 
}

BOOL BackupData(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：文件",
                           "子菜单项：数据备份",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：文件",
                           "子菜单项：数据恢复",
                           "确认"
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
    char *pCh[] = {"确认退出系统吗？", "确定    取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
	printf("项目类别代码：\n");
	printf("1    973计划项目\n"); 
	printf("2    国家自然科学基金项目\n"); 
	printf("3    863计划项目\n"); 
	printf("4    国际合作项目\n"); 
	printf("5    横向项目\n"); 

    return bRet;
}
BOOL MaintainCollegeInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据维护",
                           "子菜单项：院系基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL MaintainTeamInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据维护",
                           "子菜单项：团队基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL MaintainProjectInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据维护",
                           "子菜单项：科研项目基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL QueryCollegeInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：院系基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL QueryTeamInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：团队基本信息",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet; 
}
BOOL QueryProjectInfo(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：数据查询",
                           "子菜单项：项目基本信息",
                           "确认"
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
    char *plabel_name[] = {"主菜单项：帮助",
                           "子菜单项：帮助主题",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}
BOOL AboutDorm(void)
{
	BOOL bRet = TRUE;
    char *plabel_name[] = {"主菜单项：帮助",
                           "子菜单项：关于...",
                           "确认"
                          };

    ShowModule(plabel_name, 3);

    return bRet;
}

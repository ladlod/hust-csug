//monitoring系统
//系统信息:显示CPU型号/频率，主机名，系统版本/运行时间/开启时间,可以选择关闭计算机
//进程信息:显示进程名，进程号，父进程号，优先级，内存占用，可以选择关闭进程或刷新进程信息
//资源信息:显示cpu使用率曲线图，内存使用率曲线图
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <gdk/gdkkeysyms.h>

void cpu_info(gchar string[]); //获取CPU信息
void os_info(gchar string[]);  //获取系统信息
void name_info(gchar string[]); //获取主机名
gboolean uptime_info(gpointer data); //获取系统运行时间
gboolean start_time_info(gpointer data); //获取系统启动时间
gboolean nowtime_info(gpointer data); //获取当前时间
void course_info(GtkListStore *list_store); //获取进程信息
void prefresh(gpointer data); //刷新进程信息
void pdelete(gpointer data); //结束进程
gboolean cpu_use_info(gpointer data); //获取CPU利用率
gboolean mem_use_info(gpointer data); //获取内存使用情况
gboolean draw_cpu_rate_graph(); //绘制cpu利用率曲线图
gboolean cpu_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean cpu_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean draw_mem_rate_graph(); //绘制内存利用率曲线图
gboolean mem_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean mem_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
void shutdown0(); //关机
void test2();

//测试函数
gboolean test(gpointer data)
{
    printf("test\n");
    return TRUE;
}

GtkWidget *proc; //主窗口
GtkWidget *notebook; //notebook控件
GtkWidget *mainbox;
GtkWidget *sys_label;
GtkWidget *course_label;
GtkWidget *resource_label;
GtkWidget *sys_box;
GtkWidget *course_box;
GtkWidget *course_box2;
GtkWidget *resource_box;
GtkWidget *sys_CPU_frame;
GtkWidget *sys_os_frame;
GtkWidget *sys_os_box;
GtkWidget *sys_button_box;
GtkWidget *shutdown_button; //关机按扭
GtkWidget *exit_button; //关闭按钮
GtkWidget *course_frame;
GtkWidget *resource_CPU_frame;
GtkWidget *resource_memory_frame;
GtkWidget *CPU_draw_area;
GtkWidget *memory_draw_area;
gchar cpu[128];
GtkWidget *cpu_label;
gchar os[128];
GtkWidget *os_label;
gchar name[128];
GtkWidget *name_label;
gchar uptime[128];
GtkWidget *uptime_label;
gchar starttime[128];
GtkWidget *starttime_label;
GtkWidget *statusbar; //状态栏
gchar nowtime[128];
GtkWidget *statusbar_label;
GtkWidget *scrolled; //下拉条
GtkWidget *tree;
GtkWidget *pdelete_button;
GtkWidget *prefresh_button;
GtkListStore *list_store;
GtkTreeViewColumn *column;
GtkCellRenderer *renderer;
gchar rate[128];
GtkWidget *cpu_rate_label;
gchar mem_info[128];
GtkWidget *mem_info_label;

GdkPixmap *cpu_graph;
GdkPixmap *mem_graph;
gint cpu_point[120];
gint mem_point[120];

gdouble ftotal = 0;
gdouble ffree = 0;
gdouble cpu_rate = 0;
gdouble memtotal;
gdouble memfree;


enum{
    NAME_COLUMN,
    PID_COLUMN,
    PARENT_COLUMN,
    STATUS_COLUMN,
    PRO_COLUMN,
    MEMORY_COLUMN,
    NP_COLUMNS,
};

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

    //主窗口
    proc = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(proc),"Monitoring");
    gtk_window_set_default_size(GTK_WINDOW(proc),600,500);
    gtk_window_set_position(GTK_WINDOW(proc),GTK_WIN_POS_CENTER_ALWAYS);

    //主box
    mainbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(proc), mainbox);

    //notebook
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    gtk_container_add(GTK_CONTAINER(mainbox),notebook);

    //notebook中的第一个界面
    sys_label = gtk_label_new("系统信息");
    sys_box = gtk_vbox_new(FALSE, 5); 

    sys_CPU_frame = gtk_frame_new("CPU信息:");
    gtk_box_pack_start(GTK_BOX(sys_box), sys_CPU_frame, TRUE, TRUE, 0);

    cpu_info(cpu);
    cpu_label = gtk_label_new(cpu);
    gtk_container_add(GTK_CONTAINER(sys_CPU_frame), cpu_label);

    sys_os_frame = gtk_frame_new("系统信息:");
    gtk_box_pack_start(GTK_BOX(sys_box), sys_os_frame, TRUE, TRUE, 0);

    sys_os_box = gtk_vbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(sys_os_frame), sys_os_box);


    name_info(name);
    name_label = gtk_label_new(name);
    gtk_container_add(GTK_CONTAINER(sys_os_box), name_label);

    os_info(os);
    os_label = gtk_label_new(os);
    gtk_container_add(GTK_CONTAINER(sys_os_box), os_label);

    g_timeout_add(1000, uptime_info, NULL);
    uptime_info(NULL);
    uptime_label = gtk_label_new(uptime);
    gtk_container_add(GTK_CONTAINER(sys_os_box), uptime_label);

    g_timeout_add(1000, start_time_info, NULL);
    start_time_info(NULL);
    starttime_label = gtk_label_new(starttime);
    gtk_container_add(GTK_CONTAINER(sys_os_box), starttime_label);

    //关机
    sys_button_box = gtk_hbox_new(FALSE, 50);
    gtk_container_add(GTK_CONTAINER(sys_os_box), sys_button_box);

    exit_button = gtk_button_new_with_label("关闭任务管理器");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_set_size_request(exit_button, 120, 30);
    gtk_box_pack_start(GTK_BOX(sys_button_box), exit_button, TRUE, FALSE, 0);

    shutdown_button = gtk_button_new_with_label("关闭计算机");
    g_signal_connect(shutdown_button, "clicked", G_CALLBACK(shutdown0), NULL);
    gtk_widget_set_size_request(shutdown_button, 120, 30);
    gtk_box_pack_start(GTK_BOX(sys_button_box), shutdown_button, TRUE, FALSE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), sys_box, sys_label);

    //notebook中的第二个界面
    course_label = gtk_label_new("进程信息");
    course_box = gtk_vbox_new(FALSE, 5);

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled, 100, 100);
    gtk_box_pack_start(GTK_BOX(course_box), scrolled, TRUE, TRUE, 0);
    //gtk_container_add(GTK_CONTAINER(course_box), scrolled);

    gchar *col_name[6] = {
        "进程名",
        "进程号",
        "父进程号",
        "进程状态",
        "动态优先级",
        "内存占用",
    };
    list_store = gtk_list_store_new(NP_COLUMNS,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING,
                                    G_TYPE_STRING);
    tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(G_OBJECT(list_store));
    //gtk_box_pack_start(GTK_BOX(course_box), scrolled, TRUE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(scrolled), tree);

    for(int i = 0; i < 6; i++)
    {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(col_name[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
    }

    course_info(list_store);

    course_box2 = gtk_hbox_new(FALSE, 50);
    gtk_box_pack_start(GTK_BOX(course_box), course_box2, FALSE, FALSE, 0);

    prefresh_button = gtk_button_new_with_label("刷新");
    g_signal_connect(G_OBJECT(prefresh_button), "clicked", G_CALLBACK(prefresh), NULL);
    
    gtk_widget_set_size_request(prefresh_button, 70, 30);
    gtk_box_pack_start(GTK_BOX(course_box2), prefresh_button, TRUE, FALSE, 0);

    pdelete_button = gtk_button_new_with_label("结束");
    g_signal_connect(G_OBJECT(pdelete_button), "clicked", G_CALLBACK(pdelete), NULL);

    gtk_widget_set_size_request(pdelete_button, 70, 30);
    gtk_box_pack_start(GTK_BOX(course_box2), pdelete_button, TRUE, FALSE, 0);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), course_box, course_label);

    //notebook中的第三个界面
    resource_label = gtk_label_new("资源信息");
    resource_box = gtk_vbox_new(FALSE, 5);

    resource_CPU_frame = gtk_frame_new("CPU使用率:");
    gtk_box_pack_start(GTK_BOX(resource_box), resource_CPU_frame, FALSE, FALSE, 0);
    CPU_draw_area = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(CPU_draw_area),600,200);
    gtk_container_add(GTK_CONTAINER(resource_CPU_frame),CPU_draw_area);
    g_timeout_add(1000, draw_cpu_rate_graph, NULL);
    draw_cpu_rate_graph();
    g_signal_connect(CPU_draw_area, "expose_event", G_CALLBACK(cpu_expose_event), NULL);
    g_signal_connect(CPU_draw_area, "configure_event", G_CALLBACK(cpu_configure_event), NULL);

    resource_memory_frame = gtk_frame_new("内存使用率:");
    gtk_box_pack_start(GTK_BOX(resource_box), resource_memory_frame, FALSE, FALSE, 0);
    memory_draw_area = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(memory_draw_area),600,200);
    gtk_container_add(GTK_CONTAINER(resource_memory_frame),memory_draw_area);
    g_timeout_add(1000, draw_mem_rate_graph, NULL);
    draw_mem_rate_graph();
    g_signal_connect(memory_draw_area, "expose_event", G_CALLBACK(mem_expose_event), NULL);
    g_signal_connect(memory_draw_area, "configure_event", G_CALLBACK(mem_configure_event), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), resource_box, resource_label);

    //状态栏
    statusbar = gtk_statusbar_new();
    gtk_container_add(GTK_CONTAINER(mainbox), statusbar);
    gtk_widget_set_size_request(statusbar, 600, 35);
    gtk_box_pack_start(GTK_BOX(mainbox), statusbar, TRUE, FALSE, 0);

    g_timeout_add(1000, cpu_use_info, NULL);
    cpu_use_info(NULL);
    cpu_rate_label = gtk_label_new(rate);
    gtk_container_add(GTK_CONTAINER(statusbar), cpu_rate_label);

    g_timeout_add(1000, mem_use_info, NULL);
    mem_use_info(NULL);
    mem_info_label = gtk_label_new(mem_info);
    gtk_container_add(GTK_CONTAINER(statusbar), mem_info_label);

    g_timeout_add(1000, nowtime_info, NULL);
    nowtime_info(NULL);
    statusbar_label = gtk_label_new(nowtime);
    gtk_container_add(GTK_CONTAINER(statusbar), statusbar_label);

    g_signal_connect(proc,"destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(proc);
    gtk_main();
}

//获取CPU型号和主频
void cpu_info(gchar string[])
{
    int i = 0, j = 0;
    FILE *file = NULL;
    gchar buf[128];
    gchar *s = NULL;

    i = 0;
    file = fopen("/proc/cpuinfo", "r");
    while(!feof(file))
    {
        fgets(buf, 128, file);
        if(s = strstr(buf, "model name"))
        {
            s += 13;
            sprintf(string, "%sCPU%d: \t%s", string, i++, s);
        }
    }
    
    fclose(file);
    return ;
}

//获取操作系统版本
void os_info(gchar string[])
{
    FILE *file = NULL;
    gchar s[64];

    file = fopen("/proc/version", "r");
    fgets(s, 33, file);
    sprintf(string, "操作系统: \t%s", s);
    fclose(file);
    
    return ;
}

//获取主机名
void name_info(gchar string[])
{
    FILE *file = NULL;
    gchar s[64];

    file = fopen("/proc/sys/kernel/hostname", "r");
    fgets(s, 19, file);
    sprintf(string, "主机名: \t%s", s);
    fclose(file);
    
    return ;
}

//获取进程信息
void course_info(GtkListStore *list_store)
{
    DIR *dir;
    gchar dir_buf[128];
    struct dirent *entry;
    int fd;
    GtkTreeIter iter;
    gchar s[128];
    gchar *c_info[26];
    gdouble mem;
    gchar mem_user[16];
    gchar state[7][16] = {
        "running",
        "idle",
        "sleeping",
        "waiting",
        "stoped",
        "zombie",
        "dead",
    };

    dir = opendir("/proc");
    while((entry = readdir(dir) )!= NULL)
    {
        if(entry->d_name[0] > '0'&& entry->d_name[0] <= '9')
        {
            sprintf(dir_buf, "/proc/%s/stat", entry->d_name);
            fd = open(dir_buf, O_RDONLY);
            read(fd, s, 128);
            close(fd);

            c_info[0] = strtok(s, " "); //进程号
            c_info[1] = strtok(NULL, ")"); //进程名
            c_info[1] += 1;
            if(c_info[1][0] == '(') c_info[1] += 1;
            c_info[2] = strtok(NULL, " ");
            if(c_info[2][0] == ')') c_info[2] = strtok(NULL, " ");
            for(int i = 3; i < 26; i++)
            {
                c_info[i] = strtok(NULL, " ");
            }

            switch(c_info[2][0])
            {
                case 'R': c_info[2] = state[0];break;
                case 'I': c_info[2] = state[1];break;
                case 'S': c_info[2] = state[2];break;
                case 'D': c_info[2] = state[3];break;
                case 'T': c_info[2] = state[4];break;
                case 'Z': c_info[2] = state[5];break;
                case 'X': c_info[2] = state[6];break;
            }

            mem = atoi(c_info[22]);           //内存占用
            mem = mem / (1024*1024);
            sprintf(mem_user, "%-2.f MB", mem);

            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter, 
                                NAME_COLUMN, c_info[1],
                                PID_COLUMN, c_info[0],
                                PARENT_COLUMN, c_info[3],
                                STATUS_COLUMN, c_info[2],
                                PRO_COLUMN, c_info[17],
                                MEMORY_COLUMN, mem_user,
                                -1);
        }
    }
    closedir(dir);
}

//刷新进程信息
void prefresh(gpointer data)
{
    gtk_list_store_clear(list_store);
    course_info(list_store);
}

//结束进程
void pdelete(gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *pid;
    pid_t pid_int;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
    if(gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, PID_COLUMN, &pid ,-1);
        pid_int = atoi(pid);
        kill(pid_int, SIGTERM);
        gtk_list_store_clear(list_store);
        course_info(list_store);
    }
}

//获取CPU利用率
gboolean cpu_use_info(gpointer data)
{
    int fd;
    gchar s[1024];
    gchar *cpu_time[11];
    gdouble cfree;
    gdouble ctotal;
    gdouble tfree;
    gdouble total;

    fd = open("/proc/stat", O_RDONLY);
    read(fd , s, 1024);
    close(fd);

    cpu_time[0] = strtok(s, " ");
    for(int i = 1; i < 12; i++) cpu_time[i] = strtok(NULL, " ");

    cfree = atoi(cpu_time[4]);
    ctotal = atoi(cpu_time[1]) + atoi(cpu_time[2]) + atoi(cpu_time[3]) + atoi(cpu_time[4]);
    total = ctotal - ftotal;
    tfree = cfree - ffree;
    cpu_rate = 1 - tfree / total;

    ffree = cfree;
    ftotal = ctotal;

    sprintf(rate, "CPU利用率：%.2f %%", cpu_rate*100);
    //printf("%.2f %%\n", cpu_rate*100);
    gtk_label_set_text(GTK_LABEL(cpu_rate_label), rate);
    
    return TRUE;
}

//获取内存使用情况
gboolean mem_use_info(gpointer data)
{
    FILE *file = NULL;
    gchar buf[128];
    gchar *s = NULL;

    file = fopen("/proc/meminfo", "r");
    while(!feof(file))
    {
        fgets(buf, 128, file);
        if(s = strstr(buf, "MemTotal:"))
        {
            s += 10;
            memtotal = atoi(s);
            memtotal = memtotal / (1024);
        }
        if(s = strstr(buf, "MemFree:"))
        {
            s += 9;
            memfree = atoi(s);
            memfree = memfree / (1024);
            break;
        }
    }

    sprintf(mem_info, "内存剩余：%-2.fMB/%-2.fMB", memfree, memtotal);
    gtk_label_set_text(GTK_LABEL(mem_info_label), mem_info);
    
    fclose(file);
    return TRUE;
}

//绘制cpu利用率曲线图
gboolean draw_cpu_rate_graph()
{
    if(cpu_graph == NULL) return TRUE;
    gint current, dis;
    gint x, y;
    gdk_draw_rectangle(GDK_DRAWABLE(cpu_graph), proc->style->white_gc, TRUE, 0, 0, 
                        CPU_draw_area->allocation.width, CPU_draw_area->allocation.height);
    x = CPU_draw_area->allocation.width;
    y = CPU_draw_area->allocation.height;

    current = (int)(cpu_rate*(double)y);
    for(int i = 0; i < 119; i++)
    {
        cpu_point[i] = cpu_point[i+1];
    }
    cpu_point[119] = y - current - 5;

    dis = x / 120;
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(cpu_graph));

    GdkColormap *colormap;
    colormap = gdk_colormap_get_system();
    GdkColor color;
    gdk_color_parse("red", &color);
    gdk_colormap_alloc_color(colormap, &color ,FALSE, FALSE);
    gdk_gc_set_foreground(gc, &color);

    for(int i = 119; i > 0; i--)
    {
        gdk_draw_line(GDK_DRAWABLE(cpu_graph), gc, i*dis, cpu_point[i], (i-1)*dis, cpu_point[i-1]);
    }


    gtk_widget_queue_draw(CPU_draw_area);

    return TRUE;
}

gboolean cpu_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if(cpu_graph){
        g_object_unref(cpu_graph);
    }
    cpu_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
    gdk_draw_rectangle(cpu_graph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);

    return TRUE;
}

gboolean cpu_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                    cpu_graph, event->area.x, event->area.y, event->area.x, event->area.y,
                    event->area.width, event->area.height);

    return TRUE;
}

//绘制内存利用率曲线图
gboolean draw_mem_rate_graph()
{
    if(mem_graph == NULL) return TRUE;
    gint current, dis;
    gint x, y;
    gdk_draw_rectangle(GDK_DRAWABLE(mem_graph), proc->style->white_gc, TRUE, 0, 0, 
                        memory_draw_area->allocation.width, memory_draw_area->allocation.height);
    x = memory_draw_area->allocation.width;
    y = memory_draw_area->allocation.height;

    current = (int)(memfree/memtotal*(double)y);
    for(int i = 0; i < 119; i++)
    {
        mem_point[i] = mem_point[i+1];
    }
    mem_point[119] = current;

    dis = x / 120;
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(mem_graph));

    GdkColormap *colormap;
    colormap = gdk_colormap_get_system();
    GdkColor color;
    gdk_color_parse("blue", &color);
    gdk_colormap_alloc_color(colormap, &color ,FALSE, FALSE);
    gdk_gc_set_foreground(gc, &color);

    for(int i = 119; i > 0; i--)
    {
        gdk_draw_line(GDK_DRAWABLE(mem_graph), gc, i*dis, mem_point[i], (i-1)*dis, mem_point[i-1]);
    }


    gtk_widget_queue_draw(memory_draw_area);

    return TRUE;
}

gboolean mem_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if(mem_graph){
        g_object_unref(mem_graph);
    }
    mem_graph = gdk_pixmap_new(widget->window, widget->allocation.width, widget->allocation.height, -1);
    gdk_draw_rectangle(mem_graph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);

    return TRUE;
}

gboolean mem_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                    mem_graph, event->area.x, event->area.y, event->area.x, event->area.y,
                    event->area.width, event->area.height);

    return TRUE;
}

//获取系统运行时间
gboolean uptime_info(gpointer data)
{
    FILE *file = NULL;
    file = fopen("/proc/uptime", "r");

    char t[20], u[20]; 
    fscanf(file, "%s %s", t, u);

    long int time = 0;
    int i; 
    for(i = 0; i < 20; i++) 
    { 
        if(t[i] == '.') break; 
    } 
    int j = 1; 
    for(i--; i>=0; i--)
    { 
        time += (t[i]-'0') * j;
        j *= 10; 
    }

    long int day, hour, minute, second;
    day = time / (60*60*24);
    time = time % (60*60*24);
    hour = time / (60*60);
    time = time % (60*60);
    minute = time / 60;
    second = time % 60;

    sprintf(uptime, "系统已运行时间：%ld天%ld小时%ld分钟%ld秒", day, hour, minute, second);
    //printf("%s\n", uptime);
    gtk_label_set_text(GTK_LABEL(uptime_label), uptime);

    fclose(file);
    return TRUE;
}

//获取系统启动时间
gboolean start_time_info(gpointer data)
{
    FILE *file = NULL;
    file = fopen("/proc/uptime", "r");

    char t[20], u[20]; 
    fscanf(file, "%s %s", t, u);

    long int uptime = 0;
    int i; 
    for(i = 0; i < 20; i++) 
    { 
        if(t[i] == '.') break; 
    } 
    int j = 1; 
    for(i--; i>=0; i--)
    { 
        uptime += (t[i]-'0') * j;
        j *= 10; 
    }

    long int upday, uphour, upminute, upsecond;
    upday = uptime / (60*60*24);
    uptime = uptime % (60*60*24);
    uphour = uptime / (60*60);
    uptime = uptime % (60*60);
    upminute = uptime / 60;
    upsecond = uptime % 60;

    time_t nSeconds;
    struct tm * pTM;
    
    time(&nSeconds);
    pTM = localtime(&nSeconds);
            
    int startyear = 0,
        startmonth = 0, 
        startday = 0, 
        starthour = 0, 
        startminute = 0, 
        startsecond = 0;

    startsecond = pTM->tm_sec - (int)upsecond;
    if(startsecond < 0) { startsecond += 60; startminute -= 1;}
    startminute = pTM->tm_min - (int)upminute;
    if(startminute < 0) { startminute += 60; starthour -= 1;}
    starthour = pTM->tm_hour - (int)uphour;
    if(starthour < 0) { starthour += 24; startday -= 1;}
    startday = pTM->tm_mday - (int)upday;
    startmonth = pTM->tm_mon + 1;
    startyear = pTM->tm_year + 1900;

    sprintf(starttime, "系统启动时间：%04d-%02d-%02d %02d:%02d:%02d",
            startyear, startmonth, startday,
            starthour, startminute, startsecond);
    gtk_label_set_text(GTK_LABEL(starttime_label), starttime);
    fclose(file);

    return 0;
}

//获取当前时间
gboolean nowtime_info(gpointer data)
{
    time_t nSeconds;
    struct tm * pTM;
    
    time(&nSeconds);
    pTM = localtime(&nSeconds);

    sprintf(nowtime, "时间：%04d-%02d-%02d %02d:%02d:%02d",
            pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
            pTM->tm_hour, pTM->tm_min, pTM->tm_sec);

    gtk_label_set_text(GTK_LABEL(statusbar_label), nowtime);

    return TRUE;
}

//关机
void shutdown0()
{
    //printf("shutdown\n");
    system("shutdown now");
}

void test2()
{
    printf("test\n");
}
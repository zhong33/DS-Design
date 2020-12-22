#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#include "Windows.h"

#define M 8
#define Pwd_Max 10
#define Input_Max 3
#define INF 9999

typedef struct
{
    int num;         // 景点编号
    char name[20];   // 景点名称
    char intro[400]; // 景点简介
} vertex;

typedef struct // 景点图结构体
{
    vertex vexs[M];     // 顶点信息数组
    int edge[M][M];     // 邻接矩阵
    int vexNum, edgNum; // 顶点数和边数
} mgraph;

// @Author Rx762
int menu();                      // 主菜单
int Login();                     // 后台登录校验
int Admin_Menu();                // 管理员主菜单
int Judge_Num(int num);          // 景点编号判断
void Change_Pwd();               // 管理员密码修改
void Admin_System();             // 后台管理系统
void Exit(int *quit);            // 主程序退出
void Admin_Exit(int *quit);      // 管理员系统退出
void Create_Map(mgraph *g);      // 读取文件内容建立图
void All_Scenic(mgraph *g);      // 输出所有景点信息
void Change_Info(mgraph *g);     // 景点信息修改
void Search_Scenic(mgraph *g);   // 景点信息查询
void Search_Distance(mgraph *g); // 查询与所在位置最近景点
void encrypt(char *s);           // 加密
void decrypt(char *s);           // 解密

// @Author QWQ
void Ai_Plan(mgraph *g);                                            // dijkstra算法求最短路径
void showPath(int path[], int v0, int v1);                          // 输出单条路劲
void showResult(int v0, int v1, int dist[], int path[]);            // 输出最短路径

// @Author WHY
void showFloydPath(mgraph *g , int path[][M], int v0, int v);       //输出起点到终点的中间结点
void showFloydResult(mgraph *g, int dist[][M], int path[][M]);      //输出floyd算法的结果
void floyd(mgraph *g);                                              //floyd算法

// 主菜单
int menu()
{
    int s;
    printf("        ┌────────────────────────────┐       \n");
    printf("┌───────├ 欢迎使用井冈山导游咨询系统 ┤──────┐\n");
    printf("│       └────────────────────────────┘      │\n");
    printf("│                                           │\n");
    printf("│         1.景点信息查询                    │\n");
    printf("│                                           │\n");
    printf("│         2.景点路线智能规划                │\n");
    printf("│                                           │\n");
    printf("│         3.查询离我最近的景点              │\n");
    printf("│                                           │\n");
    printf("│         4.景区路线参考                    │\n");
    printf("│                                           │\n");
    printf("│         5.管理员登录                      │\n");
    printf("│                                           │\n");
    printf("│         6.退出导游咨询系统                │\n");
    printf("│                                           │\n");
    printf("└───────────────────────────────────────────┘\n\n");
    printf("[-]请按照您的需求选择操作编号：");
    scanf("%d", &s);
    printf("\n\n");
    return s;
}

// 管理员菜单
int Admin_Menu()
{
    int s;
    printf("        ┌───────────────────────┐       \n");
    printf("┌───────├  欢迎使用后台管理系统 ┤──────┐\n");
    printf("│       └───────────────────────┘      │\n");
    printf("│                                      │\n");
    printf("│         1.景点信息更改               │\n");
    printf("│                                      │\n");
    printf("│         2.管理员账号密码修改         │\n");
    printf("│                                      │\n");
    printf("│         3.退出管理系统               │\n");
    printf("│                                      │\n");
    printf("└──────────────────────────────────────┘\n\n");
    printf("[-]请按照您的需求选择操作编号：");
    scanf("%d", &s);
    printf("\n\n");
    return s;
}

// 程序退出
void Exit(int *quit)
{
    int i, j;
    *quit = 0;
    printf("\n");
    printf("\t感谢您的使用，Bye!\n");
    printf("\n");
    printf("*     *     *****      *              *\n");
    printf("  * *      *     *      *     *      * \n");
    printf("   *       *   * *       *   *  *   *  \n");
    printf("   *       *     *        * *    * *   \n");
    printf("   *        *****  *       *      *    \n");
    Sleep(1500);
}

// 管理员系统退出函数
void Admin_Exit(int *quit)
{
    *quit = 0;
    printf("\n[-]正在退出......");
    Sleep(500);
}

// 判断景点编号是否正确
int Judge_Num(int num)
{
    if (num < 1 || num > 8)
    {
        printf("[-]输入的景点编号有误，请重新输入！\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

// 读取文件数据建立图
void Create_Map(mgraph *g)
{
    int i, j, k, e;
    FILE *rf;
    rf = fopen("data.txt", "r");
    if (rf)
    {
        fscanf(rf, "%d%d", &g->vexNum, &g->edgNum); // 读取景点个数和边数
        for (i = 0; i < g->vexNum; i++)             // 读取顶点（景点）信息
            fscanf(rf, "%d%s%s", &g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);
        for (i = 0; i < g->vexNum; i++) // 创建邻接矩阵
            for (j = 0; j < g->vexNum; j++)
            {
                if (i == j)
                    g->edge[i][j] = 0;
                else
                    g->edge[i][j] = INF; // 表示不通
            }
        for (k = 0; k < g->edgNum; k++) // 附上权重
        {
            fscanf(rf, "%d%d%d", &i, &j, &e);
            g->edge[i][j] = g->edge[j][i] = e;
        }
        fclose(rf);
    }
    else
        g->edgNum = 0;
}

// 所有景点信息输出
void All_Scenic(mgraph *g)
{
    int i, j;
    printf("\n[-]景点信息列表\n\n");
    for (i = 0; i < g->vexNum; i++)
    {
        printf(" %d:%s\n", g->vexs[i].num, g->vexs[i].name); // 输出景点编号、名称
        printf("\n");
    }
}

// 景点信息查询
void Search_Scenic(mgraph *g)
{
    int s;
    do
    {
        printf("[-]请输入您要查找的景点编号：");
        scanf("%d", &s);
    } while (Judge_Num(s));
    printf("\n[-]景点名称：%s\n\n", g->vexs[s - 1].name);
    printf("[-]景点简介： %s\n\n", g->vexs[s - 1].intro);
    system("pause");
}

// 查询离我最近的景点
void Search_Distance(mgraph *g)
{
    int i, s, dis = 9998, res[M];
    do
    {
        printf("\n[-]请输入您当前位置的编号：");
        scanf("%d", &s);
    } while (Judge_Num(s));
    s--;
    for (i = 0; i < M; i++) // 查询最短距离
    {
        if ((g->edge[s][i] <= dis) && (s != i))
        {
            dis = g->edge[s][i];
        }
    }
    for (i = 0; i < M; i++) // 输出结果
    {
        if (g->edge[s][i] == dis)
        {
            printf("\n[-]%s离您最近，距离是%d米\n", g->vexs[i].name, g->edge[s][i]);
        }
    }
    printf("\n");
    system("pause");
}

// 登陆校验函数
int Login()
{
    int i = 0, j = 0;                       // 输入密码长度、输入次数
    char correct_pwd[11], input_pwd[11], s; // 正确密码、输入密码、临时存储字符
    FILE *rf;
    rf = fopen("pwd.txt", "r");
    fscanf(rf, "%s", correct_pwd); // 读取密码
    decrypt(correct_pwd);
    printf("\n[-]请输入管理员密码：\n");
    while (j < Input_Max)
    {
        while (s = getch())
        {
            if (s == '\r') // 回车退出输入
            {
                input_pwd[i] = '\0';
                break;
            }
            if (s == '\b')
            {
                if (i != 0)
                {
                    i--;
                    printf("\b \b"); // 回退、遮挡*、回退
                }
            }
            else if (i < Pwd_Max)
            {
                putchar('*');
                input_pwd[i++] = s;
            }
        }
        if (strcmp(input_pwd, correct_pwd) == 0 ? 1 : 0)
        {
            printf("\n[-]登陆成功！即将进入管理页面......\n");
            Sleep(1000);
            system("cls"); // 登录信息清屏
            return strcmp(input_pwd, correct_pwd) == 0 ? 1 : 0;
        }
        else
        {
            i = 0; // 密码初始化
            if (++j == Input_Max)
            {
                printf("\n[-]输入错误次数超出限制范围，即将跳转至主菜单......\n");
                Sleep(1000);
            }
            else
            {
                printf("\n[-]密码输入错误，请重新输入，还剩下%d次机会！\n", Input_Max - j);
            }
        }
    }
    return 0;
}

// 景点信息修改函数
void Change_Info(mgraph *g)
{
    int i, j, s;
    FILE *rf;
    rf = fopen("data.txt", "w");
    do
    {
        printf("\n[-]请输入您要更改信息景点的编号：");
        scanf("%d", &s);
    } while (Judge_Num(s));
    s--;
    printf("\n[-]请输入新的景点名称：");
    scanf("%s", g->vexs[s].name);
    printf("\n[-]请输入新的的景点简介：");
    scanf("%s", g->vexs[s].intro);
    fprintf(rf, "%d %d\n", g->vexNum, g->edgNum); // 写入景点个数、边数
    for (i = 0; i < g->vexNum; i++)               // 写入景点信息
        fprintf(rf, "%d %s\n%s\n", g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);
    for (i = 0; i < g->vexNum; i++) // 写入景点边权重
        for (j = i; j < g->vexNum; j++)
            if (g->edge[i][j] != 0 && g->edge[i][j] != INF)
                fprintf(rf, "%d %d %d\n", i, j, g->edge[i][j]);
    fclose(rf);
    printf("\n[-]景点信息更改成功，重启程序生效！");
    Sleep(1000);
}

// 管理员密码更改函数
void Change_Pwd()
{
    int i = 0;           // 输入密码长度
    char New_Pwd[11], s; // 新密码、临时存储字符
    FILE *rf;
    rf = fopen("pwd.txt", "w");
    printf("\n[-]请输入新密码：");
    while (s = getch())
    {
        if (s == '\r') // 回车退出输入
        {
            New_Pwd[i] = '\0';
            break;
        }
        if (s == '\b')
        {
            if (i != 0)
            {
                i--;
                printf("\b \b"); // 回退、遮挡*、回退
            }
        }
        else if (i < Pwd_Max)
        {
            putchar('*');
            New_Pwd[i++] = s;
        }
    }
    encrypt(New_Pwd);
    fprintf(rf, "%s", New_Pwd);
    printf("\n\n[-]密码更改成功！");
    Sleep(1000);
    fclose(rf);
}

// 管理系统
void Admin_System()
{
    if (Login())
    {
        int quit = 1;
        mgraph g;
        Create_Map(&g);
        while (quit)
        {
            switch (Admin_Menu())
            {
            case 1: // 景点信息更改
                system("cls");
                Change_Info(&g);
                break;
            case 2: // 管理员密码更改
                system("cls");
                Change_Pwd();
                break;
            case 3: // 退出管理系统
                system("cls");
                Admin_Exit(&quit);
                break;
            default:
                printf("[-]编号输入错误，请重新输入！\n\n");
            }
            system("cls");
        }
    }
}

// 加密函数
void encrypt(char *s)
{
    int i;
    for (i = 0; i < strlen(s); i++)
        if (s[i] % 2 == 0)
            s[i] += 2;
        else
            s[i] -= 2;
}

// 解密函数
void decrypt(char *s)
{
    int i;
    for (i = 0; i < strlen(s); i++)
        if (s[i] % 2 == 0)
            s[i] -= 2;
        else
            s[i] += 2;
}

void showPath(int path[], int v0, int v1)
{ //输出v0到v的路径
    if (v1 == v0)
    {
        switch (v0)
        {
        case 0:
            printf("八角楼");
            break;
        case 1:
            printf("红军医院");
            break;
        case 2:
            printf("黄洋界哨口");
            break;
        case 3:
            printf("百竹园景区");
            break;
        case 4:
            printf("领袖峰");
            break;
        case 5:
            printf("南山公园");
            break;
        case 6:
            printf("荆竹山景区");
            break;
        case 7:
            printf("笔架山");
            break;
        default:
            printf("[-]编号输入错误！请重新输入！\n\n");
        }
        return;
    }
    showPath(path, v0, path[v1]); //递归，直到第一个景点
    switch (v1)
    {
    case 0:
        printf("->八角楼");
        break;
    case 1:
        printf("->红军医院");
        break;
    case 2:
        printf("->黄洋界哨口");
        break;
    case 3:
        printf("->百竹园景区");
        break;
    case 4:
        printf("->领袖峰");
        break;
    case 5:
        printf("->南山公园");
        break;
    case 6:
        printf("->荆竹山景区");
        break;
    case 7:
        printf("->笔架山");
        break;
    default:
        printf("[-]编号输入错误！请重新输入！\n\n");
    }
}

void showResult(int v0, int v1, int dist[], int path[])
{ //输出结果
    showPath(path, v0, v1);
    printf("，您需要走的距离为%d米\n\n", dist[v1]);
}

void Ai_Plan(mgraph *g)
{
    int dist[M];                       //距离向量，dist[i]存储当前节点v到i的最小长度
    int path[M];                       //路径向量，path[i]=k;表示从k到i
    int final[M];                      //表示当前元素是否求出最短路径
    int i, j, c, v, min, minV, v0, v1; //min记录最小边的值,minV记录对应的点

    //1.获取起点和终点
    do
    {
        printf("\n[-]您从哪个景点出发(编号)：");
        scanf("%d", &v0);
    } while (Judge_Num(v0));
    v0--;
    do
    {
        printf("\n[-]您想去哪个景点(编号)：");
        scanf("%d", &v1);
    } while (Judge_Num(v1));
    v1--;

    //2.初始化集合S与距离向量dist
    for (i = 0; i < g->vexNum; i++)
    {
        dist[i] = INF;
        path[i] = -1;
        final[i] = 0;
    }
    v = v0;
    dist[v] = 0;

    //2.依次找出n-1个节点加入S中
    for (c = 1; c < g->vexNum; c++)
    { //到其余n-1个点的最短距离(进行n-1次循环)，每次求出一个点的最短距离(新增一个点)，c起计数作用
        min = INF;
        final[v] = 1; //让v加入集合s
        for (i = 0; i < g->vexNum; i++)
        { //扫描G中v的所有邻接点i，并选取最小值，v->minV，值为min
            //修改S与v-s中各节点的距离
            if (i != v && final[i] == 0 && dist[v] + g->edge[v][i] < dist[i])
            {                                      //如果发现了更小的
                dist[i] = dist[v] + g->edge[v][i]; //修改路径的最小距离
                path[i] = v;                       //修改路径
            }
            if (final[i] == 0 && min > dist[i]) //final[i]==0，排除集合S已存在的结点
            {
                min = dist[i]; //修改当前最小值
                minV = i;      //修改最小值的节点
            }
        }
        //printf("\n选出边：%d ->%d( %d )", v,minV,min);
        v = minV; //更新当前节点，从新节点开始找最短距离
    }
    printf("\n[-]最短路径为：\n\n");
    printf("[-]");
    showResult(v0, v1, dist, path);
    system("pause");
}

void showFloydPath(mgraph *g, int path[][M], int v0, int v)
{ //输出v0到v的所有中间结点
    int k = path[v0][v];
    if (k == -1)
        return;
    showFloydPath(g, path, v0, k);
    printf("%s->", g->vexs[k].name);
    showFloydPath(g, path, k, v);
}

void showFloydResult(mgraph *g, int dist[][M], int path[][M])
{ //输出floyd的结果
    int u, v;
    for (u = 0; u < g->vexNum; u++)
        for (v = 0; v < g->vexNum; v++)
            if (dist[u][v] == INF)
                printf("[%d->%d]:不可达.\n", u, v);
            else if(g->vexs[u].num != g->vexs[v].num)
            {
                printf("[%d->%d]:%s->", g->vexs[u].num, g->vexs[v].num, g->vexs[u].name);
                showFloydPath(g, path, u, v);
                printf("%s", g->vexs[v].name);
                printf("，总路程%d米\n", dist[u][v]);
            }
}

void floyd(mgraph *g)
{
    int dist[M][M];
    int path[M][M];
    int u, v, k;
    for (u = 0; u < g->vexNum; u++) //初始化
        for (v = 0; v < g->vexNum; v++)
        {
            dist[u][v] = g->edge[u][v];
            path[u][v] = -1;
        }
    for (k = 0; k < g->vexNum; k++)
        for (u = 0; u < g->vexNum; u++)
            for (v = 0; v < g->vexNum; v++)
                if (dist[u][k] != INF && dist[k][v] != INF && dist[u][k] + dist[k][v] < dist[u][v])
                {
                    dist[u][v] = dist[u][k] + dist[k][v];
                    path[u][v] = k;
                }
    printf("\n所有景点的距离和路径如下：\n");
    showFloydResult(g, dist, path);
    system("pause");
}

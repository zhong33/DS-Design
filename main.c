#include "fuc.h"

int main()
{
    int quit = 1;
    mgraph g;
    Create_Map(&g);
    while (quit)
    {
        switch (menu())
        {
            case 1:                 // 景点信息查询
                system("cls");
                All_Scenic(&g);
                Search_Scenic(&g);
                break;
            case 2:                 // 线路智能推荐（最短路径）
                system("cls");
                All_Scenic(&g);
                Ai_Plan(&g);
                break;
            case 3:                 // 查找离我最近的景点
                system("cls");
                All_Scenic(&g);
                Search_Distance(&g);
                break;
            case 4:                 // 输出景点所有线路
                system("cls");
                floyd(&g);
                break;
            case 5:                 // 管理菜单
                system("cls");
                Admin_System();
                break;
            case 6:                 // 退出导航系统
                system("cls");
                Exit(&quit);
                break;
            default:
                printf("\t\t编号输入错误，请重新输入！\n\n");
        }
        system("cls");
    }
    return 0;
}
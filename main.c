#include "show.h"
#include "do.h"
#include "public.h"


int main(int argc, char const* argv[])
{
    mkfifo("fifo_mp3", 0777);
    GetVideoFile();//获取媒体文件
    while (1)
    {
        ShowMenu();               // 软件菜单栏显示
        int choose = GetChoose(); // 接收输入选项
        SwitchChoose(choose);//功能
        system("clear");
    }

    return 0;
}
#include "show.h"
#include "do.h"

char* filename[1024] = { 0 };
int filenum = 0;
extern int FileStat;

void SwitchChoose(int tmp)
{
    switch (tmp)
    {
        case 1:
            LookList(); // 查看播放列表
            DayBook("input:1 >>> 查看播放列表,进入播放列表");
            break;
        case 2:
            Start_Pause(); // 开始/暂停
            DayBook("input:2 >>> 开始/暂停");
            break;
        case 3:
            StopOff(); // 停止
            DayBook("input:3 >>> 播放关闭");
            break;
        case 4:
            LastFile(); // 上一个
            DayBook("input:4 >>> 播放上一个");
            break;
        case 5:
            NextFile(); // 下一个
            DayBook("input:5 >>> 播放下一个");
            break;
        case 6:
            SpeedOn(); // 倍速播放
            DayBook("input:6 >>> 设置倍速,进入倍速选项列表");
            break;
        case 7:
            Location(); // 定位
            DayBook("input:7 >>> 定位播放进度");
            break;
        case 8:
            VideoMode(); // 播放方式
            DayBook("input:8 >>> 设置播放方式,进入播放方式列表");
            break;

        case 9:
            QuitApp(); // 退出
            break;

        default:
            printf("\33[31m\33[1m输入越界，请重新输入！！！\33[0m\n");
            DayBook("error!!!输入越界，请重新输入！！！");
            sleep(2);
            break;
    }
}
void ShowMenu(void)
{
    system("clear");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[43m\33[1m|         ***\33[31m视频播放器\33[0m\33[43m\33[1m***        |\33[0m\n");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[42m\33[1m*         1. 查看播放列表         *\33[0m\n");
    printf("\33[42m\33[1m*         2. 开始/暂停            *\33[0m\n");
    printf("\33[42m\33[1m*         3. 停止                 *\33[0m\n");
    printf("\33[42m\33[1m*         4. 上一个               *\33[0m\n");
    printf("\33[42m\33[1m*         5. 下一个               *\33[0m\n");
    printf("\33[42m\33[1m*         6. 倍速播放             *\33[0m\n");
    printf("\33[42m\33[1m*         7. 定位                 *\33[0m\n");
    printf("\33[42m\33[1m*         8. 播放方式             *\33[0m\n");
    printf("\33[42m\33[1m*         9. 退出                 *\33[0m\n");
    printf("\33[42m\33[1m*=================================*\33[0m\n");
    printf("\33[1m\33[35m~~请输入您的选择：\33[0m");
}
void LookList(void) // 查看播放列表
{
    system("clear");
    char tmp[32];
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[43m\33[1m|         ***\33[31m视频播放器\33[0m\33[43m\33[1m***        |\33[0m\n");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    for (int i = 0; i < filenum; i++)
    {
        printf("\33[31m\33[1m%d. %s\33[0m\n", i + 1, filename[i]);
    }
    printf("\33[1m\33[35m~~输入“last”返回上一步\33[0m\n\n");
    printf("\33[1m\33[35m~~请输入：\33[0m");
    fgets(tmp, 32, stdin);
    if (!strcmp("last\n", tmp))
    {
        DayBook("input: last >>> 返回主菜单");
        return;
    }
    int num = atoi(tmp);
    if (num >= 1 && num <= filenum)
    {
        FileStat = num - 1;
        StopOff();
        Start_Pause();
        DayBook(filename[FileStat]);

        return;
    }
    else
    {
        printf("\33[31m\33[1m输入不正确，请重新输入！！！\33[0m\n");
        DayBook("error!!!输入不正确，请重新输入！！！");
        sleep(2);
        LookList();
    }



}

void ShowSpeed(void)
{
    system("clear");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[43m\33[1m|         ***\33[31m视频播放器\33[0m\33[43m\33[1m***        |\33[0m\n");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[31m\33[1m1. 1倍速播放\33[0m\n\n");
    printf("\33[31m\33[1m2. 2倍速播放\33[0m\n\n");
    printf("\33[31m\33[1m3. 4倍速播放\33[0m\n\n");
    printf("\33[1m\33[35m~~输入“last”返回上一步\33[0m\n\n");
    printf("\33[1m\33[35m~~请输入您的选择：\33[0m");
}
void ShowLocation(void)
{
    system("clear");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[43m\33[1m|         ***\33[31m视频播放器\33[0m\33[43m\33[1m***        |\33[0m\n");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[1m\33[35m~~输入“last”返回上一步\33[0m\n\n");
    printf("\33[1m\33[35m~~或者请输入您要定位到num%%\33[0m\n");
    printf("\33[1m\33[35m~~(请输入0~100)\33[0m\n");
    printf("\33[1mInput :\33[0m");

}
void ShowMode(void)
{
    system("clear");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[43m\33[1m|         ***\33[31m视频播放器\33[0m\33[43m\33[1m***        |\33[0m\n");
    printf("\33[43m\33[1m+=================================+\33[0m\n");
    printf("\33[31m\33[1m1. 顺序播放\33[0m\n\n");
    printf("\33[31m\33[1m2. 单曲循环\33[0m\n\n");
    printf("\33[31m\33[1m3. 随机播放\33[0m\n\n");
    printf("\33[1m\33[35m~~输入“last”返回上一步\33[0m\n\n");
    printf("\33[1m\33[35m~~或者请输入您要设定的播放模式（请输入）：\33[0m");
}
#include "do.h"
#include "show.h"
#include <sys/stat.h>  // mkfifo 必需
#include <errno.h>     // errno 必需
#include <stdlib.h>    // exit 已包含，无需重复加

void DayBook(char* CmdSentence)
{
    time_t t = 0;
    struct tm* pt = NULL;
    time(&t);
    pt = localtime(&t);

    FILE* fp = NULL;
    fp = fopen(LOG_FILE, "a+");
    fprintf(fp, "   \33[36m\33[1m[%04d-%02d-%02d %02d:%02d:%02d]    执行：    \33[0m", pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday,
        pt->tm_hour, pt->tm_min, pt->tm_sec);

    fprintf(fp, "\33[32m%s\33[0m", CmdSentence);
    fputc('\n', fp);
    fclose(fp);
}
char Mplayerbuf[4096] = { 0 };
int FileStat = 0; // 切换媒体文件字符串数组
STATING stat_get = STAT_FREE;
VIDEO_MOD MODING = CYCLE_ORDER;
extern char* filename[1024];
extern int filenum;

void GetVideoFile(void)
{
    DIR* fdir;
    fdir = opendir(MEDIA_DIR);
    if (NULL == fdir)
    {
        perror("fail to open");
        exit(0);
    }
    struct dirent* pdir;
    while (1)
    {
        pdir = readdir(fdir);
        if (NULL == pdir)
            break;
        char* tmpbuf = pdir->d_name;
        while ('\0' != *tmpbuf) // 找末尾
        {
            tmpbuf++;
        }
        while ('.' != *tmpbuf) // 找 .
        {
            tmpbuf--;
        }
        if (!strcmp(tmpbuf, ".mp3") || !strcmp(tmpbuf, ".mp4") || !strcmp(tmpbuf, ".avi") || !strcmp(tmpbuf, ".wma") || !strcmp(tmpbuf, ".rmvb") || !strcmp(tmpbuf, ".flv") || !strcmp(tmpbuf, ".rm"))
        {
            filename[filenum++] = pdir->d_name;
        }
    }
}
int GetChoose(void) // 接收选项
{
    char tmp[4];
    fgets(tmp, 4, stdin);

    return atoi(tmp);
}

void Start_Pause(void) // 开始、暂停
{
    // 新增：自动创建管道，已存在则不报错（需引入2个头文件，见下文）
    if (mkfifo(FIFO_FILE, 0666) < 0) {
        if (errno != EEXIST) {
            perror("fail to mkfifo");
            return;
        }
    }
    if (stat_get == STAT_FREE) //==========开始=========
    {
        char tmpbuf[1024] = { 0 };
        //sprintf(tmpbuf, "/home/linux/Music/%s", filename[FileStat]);
        sprintf(tmpbuf, "%s/%s", MEDIA_DIR, filename[FileStat]);
        stat_get = STAT_ON;
        pid_t pid = fork();
        if (pid > 0)
        {
            // 父进程
        }
        else if (pid == 0)
        {

            close(1);
            close(2);
            //execlp("mplayer", "mplayer", "-slave", "-input", "file=./fifo_mp3", tmpbuf, NULL);
            // 关键修改：把 "file=./fifo_mp3" 改为 "file="FIFO_FILE
            //execlp("mplayer", "mplayer", "-slave", "-input", "file="FIFO_FILE, tmpbuf, NULL);
            // 纯命令行模式：-nogui(无窗口) -ao pulse(WSL2音频输出) -quiet(减少日志)
            //execlp("mplayer", "mplayer", "-slave", "-input", "file="FIFO_FILE, "-nogui", "-ao", "pulse", "-quiet", tmpbuf, NULL);
            // 核心修改：-ao alsa（ALSA音频输出），去掉-ao pulse，保留-nogui(无窗口)-quiet(静默)
            //execlp("mplayer", "mplayer", "-slave", "-input", "file="FIFO_FILE, "-nogui", "-ao", "alsa", "-quiet", tmpbuf, NULL);
            // WSL2纯命令行出声音核心：-ao pulse指定音频驱动 + 显式指向WSLg的Pulse服务器
            execlp("mplayer", "mplayer", "-slave", "-input", "file="FIFO_FILE, "-nogui", "-ao", "pulse:server=/mnt/wslg/PulseServer", "-quiet", tmpbuf, NULL);
            perror("fail to mplayer");
            exit(0);
        }
        else
        {
            perror("fail to fork");
            return;
        }
    }
    else //=============暂停pause============
    {
        int fd = open(FIFO_FILE, O_WRONLY);
        if (-1 == fd)
        {
            perror("fail to open");
            return;
        }
        write(fd, "pause\n", 6);
        if (stat_get == STAT_ON)
        {
            stat_get = STAT_PAUSE;
        }
        else if (stat_get == STAT_PAUSE)
        {
            stat_get = STAT_ON;
        }

        close(fd);
    }
}
void StopOff(void) // 停止
{
    int fd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK);
    if (-1 == fd)
    {
        perror("fail to open");
        return;
    }

    write(fd, "stop\n", 5);
    close(fd);
    stat_get = STAT_FREE;
}
void LastFile(void) // 上一个
{
    if (MODING == CYCLE_ORDER) // 顺序循环
    {

        if (0 == FileStat)
        {
            printf("已是第一个文件无法切换上一个\n");
            sleep(2);
            return;
        }
        StopOff(); // 关闭当前文件
        FileStat--;
        Start_Pause(); // 播放
    }
    else if (MODING == CYCLE_SINGLE) // 单曲循环
    {
        StopOff();     // 关闭当前文件
        Start_Pause(); // 播放
    }
    else if (MODING == CYCLE_RANDOM) // 随机循环
    {
        StopOff(); // 关闭当前文件
        FileStat = rand() % filenum;
        Start_Pause(); // 播放
    }
}
void NextFile(void) // 下一个
{
    if (MODING == CYCLE_ORDER)
    {
        if (filenum == (FileStat + 1))
        {

            printf("最后一个文件无法切换下一个\n");
            sleep(2);
            return;
        }
        StopOff(); // 关闭当前文件
        FileStat++;
        Start_Pause(); // 播放
    }
    else if (MODING == CYCLE_SINGLE) // 单曲循环
    {
        StopOff();     // 关闭当前文件
        Start_Pause(); // 播放
    }
    else if (MODING == CYCLE_RANDOM) // 随机循环
    {
        StopOff(); // 关闭当前文件
        FileStat = rand() % filenum;
        Start_Pause(); // 播放
    }
}
void SpeedOn(void) // 倍速播放
{
    ShowSpeed();
    char tmp[12];
    fgets(tmp, sizeof(tmp), stdin);
    if (!strcmp("last\n", tmp))
    {
        DayBook("input: last >>> 返回主菜单");
        return;
    }
    int fd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK); // 管道会阻塞
    if (-1 == fd)
    {
        perror("fail to open");
        sleep(2);
        return;
    }
    int data = atoi(tmp);
    switch (data)
    {
        case 1:
            write(fd, "speed_set 1\n", 12);
            printf("\33[1m当前为 1 倍速\33[0m\n");
            DayBook("input:1 >>> 设置1倍速");
            sleep(2);
            break;
        case 2:
            write(fd, "speed_set 2\n", 12);
            printf("\33[1m当前为 2 倍速\33[0m\n");
            DayBook("input:2 >>> 设置2倍速");
            sleep(2);
            break;
        case 3:
            write(fd, "speed_set 4\n", 12);
            printf("\33[1m当前为 4 倍速\33[0m\n");
            DayBook("input:3 >>> 设置3倍速");
            sleep(2);
            break;

        default:
            printf("\33[31m\33[1m输入不正确，请重新输入！！！\33[0m\n");
            DayBook("error!!!输入不正确，请重新输入！！！");
            sleep(2);
            SpeedOn();
            break;
    }
    close(fd);
}
void Location(void) // 定位
{
    ShowLocation();
    char tmp[12];
    char Tmpbuf[1024] = { 0 };
    fgets(tmp, sizeof(tmp), stdin);
    if (!strcmp("last\n", tmp))
    {
        DayBook("input: last >>> 返回主菜单");
        return;
    }

    int fd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK); // 管道会阻塞
    if (-1 == fd)
    {
        perror("fail to open");
        sleep(2);
        return;
    }
    int data = atoi(tmp);
    char tmpbuf[1024] = { 0 };
    if (data >= 0 && data <= 100)
    {
        sprintf(tmpbuf, "seek %d 1\n", data);
        write(fd, tmpbuf, strlen(tmpbuf));
        sprintf(Tmpbuf, "定位到%d%%", data);
        DayBook(Tmpbuf);
        close(fd);

        return;
    }
    printf("\33[31m\33[1m输入不正确，请重新输入！！！\33[0m\n");
    DayBook("error!!!输入不正确，请重新输入！！！");
    sleep(2);
    Location();
}
void VideoMode(void) // 播放方式
{
    ShowMode();
    char tmp[12];
    fgets(tmp, sizeof(tmp), stdin);
    if (!strcmp("last\n", tmp))
    {
        DayBook("input: last >>> 返回主菜单");
        return;
    }
    int fd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK); // 管道会阻塞
    if (-1 == fd)
    {
        perror("fail to open");
        sleep(2);
        return;
    }
    int data = atoi(tmp);
    switch (data)
    {
        case 1:
            MODING = CYCLE_ORDER;
            printf("\33[1m当前为循环播放\33[0m\n");
            DayBook("input:1 >>> 设置循环播放");
            sleep(2);
            break;
        case 2:
            MODING = CYCLE_SINGLE;
            printf("\33[1m当前为单曲循环\33[0m\n");
            DayBook("input:2 >>> 设置单曲循环");
            sleep(2);
            break;
        case 3:
            MODING = CYCLE_RANDOM;
            printf("\33[1m当前为随机播放\33[0m\n");
            DayBook("input:3 >>> 设置随机播放");
            sleep(2);
            break;

        default:
            printf("\33[31m\33[1m输入不正确，请重新输入！！！\33[0m\n");
            DayBook("error!!!输入不正确，请重新输入！！！");
            sleep(2);
            VideoMode();
            break;
    }
    close(fd);
}
void QuitApp(void) // 退出
{
    StopOff();
    system("clear");
    printf("\33[42m\33[1m\33[31m         期待下次再见!            \33[0m\n");
    printf("\33[1m        Mplayer已退出!!!\33[0m\n");
    DayBook("Mplayer已退出!!!");
    exit(0);
}
#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <dirent.h>

typedef enum
{
    STAT_FREE,//空闲
    STAT_ON,//播放
    STAT_PAUSE,//暂停
}STATING;

typedef enum
{
    CYCLE_ORDER,//顺序播放
    CYCLE_SINGLE,//单曲循环
    CYCLE_RANDOM,//随机播放
}VIDEO_MOD;


#endif
#ifndef __DO_H__
#define __DO_H__

#include "public.h"

#define MEDIA_DIR     "/home/lnx/Music"  // 媒体文件目录（替换为自己的路径）
#define FIFO_FILE     "./fifo_mp3"             // 管道文件（可改绝对路径，如"/tmp/fifo_mp3"）
#define LOG_FILE      "./log_video.log"        // 日志文件（可改绝对路径，如"/var/log/mp3_player.log"）


extern int GetChoose(void);//接收选项
extern void Start_Pause(void);//开始、暂停
extern void StopOff(void); // 停止
extern void LastFile(void); // 上一个
extern void NextFile(void); // 下一个
extern void SpeedOn(void); // 倍速播放
extern void Location(void); // 定位
extern void QuitApp(void); // 退出
extern void VideoMode(void); // 播放方式
extern void GetVideoFile(void);
extern void DayBook(char* CmdSentence);

#endif
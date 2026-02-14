#ifndef __DO_H__
#define __DO_H__

#include "public.h"

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
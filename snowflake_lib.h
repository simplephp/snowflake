/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | Snowflake 算法基础                                                   |
  | 占位 0 + 毫秒级时间41位 + 机器ID 10位 + 毫秒内序列12位。             |
  | 0 - 1 - 42 - 52 - 64  = 0 + time + macheine id + increment           |
  | 1bits 占位不可用                                                     |  
  | 41bits是以微秒为单位的timestamp。                                    |
  | 10bits是事先配置好的机器ID,最多只能有1023台机器同时产生ID            |
  | 12bits是累加计数器,也标明1台机器1ms中最多产生4095个ID。              |
  | 因为使用到位移运算，需要64位操作系统，不然生成的ID会有可能不正确.    |
  +----------------------------------------------------------------------+
  | Author: Kevin <841694874@qq.com>                                     |
  +----------------------------------------------------------------------+
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/types.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#define WORKID_WIDTH 1024

typedef struct globle
{
	int global_int:12;
	int64_t last_stamp;
	int workid;
	int seqid;
} globle;

void set_workid(int workid);
pid_t gettid( void );
uint64_t get_curr_ms();
uint64_t wait_next_ms(int64_t lastStamp);
int atomic_incr(int id);
int64_t get_unique_id();

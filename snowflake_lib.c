/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Kevin <841694874@qq.com>                                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "snowflake_lib.h"

struct globle g_info;
#define  sequenceMask (-1L ^ (-1L << 12L))
void set_workid(int workid)
{
 g_info.workid = workid;
}
pid_t gettid( void )
{
	return syscall( __NR_gettid );
}
uint64_t get_curr_ms()
{
	struct timeval time_now;
	gettimeofday(&time_now,NULL);
	uint64_t ms_time = time_now.tv_sec*1000 + time_now.tv_usec/1000;
	return ms_time;
}

uint64_t wait_next_ms(int64_t lastStamp)
{
	uint64_t cur = 0;
	do {
		cur = get_curr_ms();
	} while (cur <= lastStamp);
	return cur;
}
int atomic_incr(int id)
{
	__sync_add_and_fetch( &id, 1 );
	return id;
}
int64_t get_unique_id()
{
	int64_t  uniqueId = 0;
	uint64_t nowtime = get_curr_ms();
	uniqueId = nowtime<<22;
	uniqueId |=(g_info.workid&0x3ff)<<12;

	if (nowtime <g_info.last_stamp)
	{
		perror("error");
		exit(-1);
	}
	if (nowtime == g_info.last_stamp)
	{
		g_info.seqid = atomic_incr(g_info.seqid)& sequenceMask;
		if (g_info.seqid ==0)
		{
			nowtime = wait_next_ms(g_info.last_stamp);
		}
	}
	else
	{
		g_info.seqid  = 0;
	}
	g_info.last_stamp = nowtime;
	uniqueId |=g_info.seqid;
	return uniqueId;
}

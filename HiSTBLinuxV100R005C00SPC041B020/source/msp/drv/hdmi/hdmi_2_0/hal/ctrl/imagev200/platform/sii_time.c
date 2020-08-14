// This is for delay or time stamp use library



#include "sii_time.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "delay.h"
#endif

static unsigned long processInitTime;

unsigned long Sii_Get_Process_Time(void)
{
	unsigned long globalTime = SiI_get_global_time();
	if(globalTime > processInitTime)
		return (globalTime - processInitTime);
	else
		processInitTime = globalTime;
	return globalTime;
}

// This function will return time with ms
//
unsigned long SiI_get_global_time( void )
{
#ifndef HDMI_BUILD_IN_BOOT
    struct timeval tv;

    do_gettimeofday(&tv);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;

#else
    return 0;
#endif
}
unsigned long SiI_difftime( unsigned long t )
{
    return SiI_get_global_time() - t;
}

void SiI_DelayMS( unsigned long delay )
{
#ifndef HDMI_BUILD_IN_BOOT
    if (delay > 10)
    {
        msleep(delay);
    }
    else
    {
        mdelay(delay);
    }
#else
    udelay(delay * 1000);
#endif
}

/*****************************************************************************/
/**
 *  @brief  Waits for the specified number of milliseconds
 *
 *  @param[in]  baseTime: Set the baseTime that start to delay
 *
 *  @param[in]  delay: number of milliseconds
 *
 *  @return     If delay expired, return true
 *
 *****************************************************************************/
bool_t SiI_TimerDelay(unsigned long baseTime, unsigned long delay)
{
    uint32_t diff_time = SiI_difftime(baseTime);

    if( diff_time < delay)
        return false;
    else
        return true;
}
#if 0
void sii_mcutime( SII_LOCAL_TIME *tm )
{

	time_t tTime;
    struct tm tmStart;
	struct __timeb32 timeptr;
    _ftime32_s( &timeptr );
   time( &tTime );
	localtime_s( &tmStart, &tTime );

	tm->msec = timeptr.millitm;
	tm->sec = tmStart.tm_sec;
	tm->min = tmStart.tm_min;
	tm->hour = tmStart.tm_hour;
	tm->day = tmStart.tm_mday;

}
#endif
void Sii_Init_ProcessTime(void)
{
	processInitTime = SiI_get_global_time();
}

#if 0
unsigned long sii_mktime( SII_LOCAL_TIME *tm )
{
    unsigned long t;
    t = tm->day;
    t = t*24 + tm->hour;
    t = t*60 + tm->min;
    t = t*60 + tm->sec;
    t = t*1000 + tm->msec;
    return t;
}
#endif



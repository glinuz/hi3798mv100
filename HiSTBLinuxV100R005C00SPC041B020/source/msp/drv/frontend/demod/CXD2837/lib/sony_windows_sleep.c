/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-06-22 19:06:21 #$
  File Revision : $Revision:: 5632 $
------------------------------------------------------------------------------*/
#include "sony_windows_sleep.h"
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/
int sony_windows_Sleep (unsigned long sleepTimeMs)
{
    BOOL qpcSupported = TRUE;
    LARGE_INTEGER qpcStartTime;
    LARGE_INTEGER qpcFrequency;
    MMRESULT mmResult = 0;
    UINT resolution = 0;

    if (sleepTimeMs == 0) {
        return 1;
    }

    if (!QueryPerformanceFrequency(&qpcFrequency)) {
        qpcSupported = FALSE;
    }

    /* Start time measurement using QueryPerformanceCounter. */
    if (!QueryPerformanceCounter(&qpcStartTime)) {
        qpcSupported = FALSE;
    }

    /* Get multimedia timer resolution information. */
    {
        TIMECAPS timeCaps;

        mmResult = timeGetDevCaps(&timeCaps, sizeof(timeCaps));
        if (mmResult != TIMERR_NOERROR) {
            return 0;
        }

        resolution = timeCaps.wPeriodMin;
    }

    /* Change resolution. */
    mmResult = timeBeginPeriod(resolution);
    if (mmResult != TIMERR_NOERROR) {
        return 0;
    }

    if (qpcSupported) {
        Sleep(sleepTimeMs);
    } else {
        /* To ensure that sleeping time is not shorter than specified time
           without using QueryPerformanceCounter. */
        Sleep(sleepTimeMs + resolution);
    }

    /* Clear resolution setting. */
    mmResult = timeEndPeriod(resolution);
    if (mmResult != TIMERR_NOERROR) {
        return 0;
    }

    /* Double-check sleeping time using QueryPerformanceCounter. */
    if (qpcSupported) {
        LARGE_INTEGER qpcCurrentTime;
        unsigned long currentDelayMs = 0;
        unsigned long targetDelayMs = sleepTimeMs;

        do {
            if (!QueryPerformanceCounter(&qpcCurrentTime)) {
                return 0;
            }

            currentDelayMs = (unsigned long)(1000 * (qpcCurrentTime.QuadPart - qpcStartTime.QuadPart)/qpcFrequency.QuadPart);

            if (currentDelayMs >= targetDelayMs) {
                break;
            } else if (targetDelayMs > (currentDelayMs + resolution)) {
                /* 
                   We believe that sleeping time by Sleep function is accurate like as follows:
                   (specified time - resolution) <= sleeping time <= (specified time + resolution)
                   So, if above condition is true, we should suspect the QueryPerformanceCounter.
                   As written in web, if QueryPerformanceCounter internally uses RDTSC (depends on HAL),
                   the estimated time may not be accurate because CPU clock can be changed dynamically.
                   Following line limits busy loop waiting time even if the estimated time
                   by QueryPerformanceCounter is not accurate.
                */
                targetDelayMs = currentDelayMs + resolution;
            }
        } while(1);
    }

    return 1;
}

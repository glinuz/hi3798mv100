#ifndef __SECURE_RTC_H__
#define __SECURE_RTC_H__

#define OK    0
#define ERROR 1

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

int secure_rtc_read_time(struct rtc_time *tm);
int secure_rtc_set_time(struct rtc_time *tm);

#endif /* ! __SECURE_RTC_H__ */

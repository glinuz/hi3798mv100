#ifndef __SECURE_RTC_OFFSET__H
#define __SECURE_RTC_OFFSET__H

#define RTC_DATA_REG        (RTC_BASE_ADDR+0x000)
#define RTC_MATCH_REG       (RTC_BASE_ADDR+0x004)
#define RTC_LOAD_REG        (RTC_BASE_ADDR+0x008)
#define RTC_CONTROL_REG     (RTC_BASE_ADDR+0x00C)
#define RTC_IMSC            (RTC_BASE_ADDR+0x010)
#define RTC_RIS             (RTC_BASE_ADDR+0x014)
#define RTC_MIS             (RTC_BASE_ADDR+0x018)
#define RTC_ICR             (RTC_BASE_ADDR+0x01C)
#define RTC_PeriphID0       (RTC_BASE_ADDR+0xFE0)
#define RTC_PeriphID1       (RTC_BASE_ADDR+0xFE4)
#define RTC_PeriphID2       (RTC_BASE_ADDR+0xFE8)
#define RTC_PeriphID3       (RTC_BASE_ADDR+0xFEC)
#define RTC_PCellID0        (RTC_BASE_ADDR+0xFF0)
#define RTC_PCellID1        (RTC_BASE_ADDR+0xFF4)
#define RTC_PCellID2        (RTC_BASE_ADDR+0xFF8)
#define RTC_PCellID3        (RTC_BASE_ADDR+0xFFC)

#endif /* __SECURE_RTC_OFFSET__H */

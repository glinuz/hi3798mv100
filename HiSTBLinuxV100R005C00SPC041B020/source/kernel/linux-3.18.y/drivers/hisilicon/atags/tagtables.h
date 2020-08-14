#ifndef _UAPI__TAGTABLES_H
#define _UAPI__TAGTABLES_H

extern struct tagtable tag_sdkver;
extern struct tagtable tag_param;
extern struct tagtable tag_macaddr;
extern struct tagtable tag_phyaddr;
extern struct tagtable tag_phyintf;
extern struct tagtable tag_phygpio;
extern struct tagtable tag_chiptrim;
#ifdef CONFIG_MMC
extern struct tagtable tag_emmc;
#endif
#endif

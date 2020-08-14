
#ifndef MTD_PART_H
#define MTD_PART_H

unsigned long long memparse(const char *ptr, char **retptr);
int find_part_info(char *s, char *ptnname, uint64_t *start, uint64_t *length);
int find_part(char *s, char **retptr, char *ptnname, uint64_t *start, uint64_t *length);

#endif /* MTD_PART_H */
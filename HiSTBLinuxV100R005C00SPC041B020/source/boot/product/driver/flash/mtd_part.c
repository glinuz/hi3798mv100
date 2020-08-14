// #include <linux/types.h>
// #include <nand.h>
// #include <config.h>
#include <uboot.h>
#include "mtd_part.h"
#include <nand.h>


/*****************************************************************************/
#ifdef CONFIG_GENERIC_NAND
#ifdef HI_ADVCA_TYPE_NAGRA
int find_part_info(char *s, char *ptnname, uint64_t *start, uint64_t *length)
{
    int rel;
    nand_info_t  *nand;
    nand_info_t *nand_info = get_local_nand_info();
    
    for( ; s != NULL; )
    {
        char *p;
        (*start) = 0;

        /* fetch <mtd-id> */
        if (!(p = strchr(s, ':')))
        {
            printf("no mtd-id\n");
            return 0;
        }else
        {
            *p = '\0';
            rel = strncmp(s, "hinand", sizeof("hinand"));
            *p = ':';
            if (!rel)
            {            	
                /* the following commands operate on the current device */
                if (get_nand_curr_device() < 0 
                    || get_nand_curr_device() >= CONFIG_SYS_MAX_NAND_DEVICE 
                    || !nand_info[get_nand_curr_device()].name) 
                {
                    printf("no devices available\n");
                    return 0;
                }
                nand = &nand_info[get_nand_curr_device()];
       	    }else
       	    {
       	    	if (!(p = strchr(s, ';')))
       	    	{
       	    	    printf("bad character after partition (%c)\n", *s);
                    return 0;
       	    	}
       	    	
       	    	s = p + 1;
       	    	continue;
	    }
        }

        /*
        * parse one mtd. have it reserve memory for the
        * struct cmdline_mtd_partition and the mtd-id string.
        */
        rel =find_part(p + 1, &s, ptnname, start, length);

        if (rel == 1)
        {
            if(*length == (uint64_t)(-1))
            	*length = nand->size - *start;            
            return 1;
        }
        else if (rel == -1)
        {
            return 0;
        }
	
        /* EOS - we're done */
        if (*s == 0)
            break;

        /* does another spec follow? */
        if (*s != ';')
        {
            printf("bad character after partition (%c)\n", *s);
            return 0;
        }
        s++;
    }
    return 0;
}

#else

int find_part_info(char *s, char *ptnname, uint64_t *start, uint64_t *length)
{
    int rel;
    nand_info_t  *nand;
    
    for( ; s != NULL; )
    {
        char *p;
        (*start) = 0;

        /* fetch <mtd-id> */
        if (!(p = (char *)strchr(s, ':')))
        {
            printf("no mtd-id\n");
            return 0;
        }else
        {
            *p = '\0';
            rel = strncmp(s, "hinand", sizeof("hinand"));
            *p = ':';
            if (!rel)
            {            	
                /* the following commands operate on the current device */
                if (nand_curr_device < 0 
                    || nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE 
                    || !nand_info[nand_curr_device].name) 
                {
                    printf("no devices available\n");
                    return 0;
                }
                nand = &nand_info[nand_curr_device];
       	    }else
       	    {
       	    	if (!(p = (char *)strchr(s, ';')))
       	    	{
       	    	    printf("bad character after partition (%c)\n", *s);
                    return 0;
       	    	}
       	    	
       	    	s = p + 1;
       	    	continue;
	    }
        }

        /*
        * parse one mtd. have it reserve memory for the
        * struct cmdline_mtd_partition and the mtd-id string.
        */
        rel =find_part(p + 1, &s, ptnname, start, length);

        if (rel == 1)
        {
            if(*length == (uint64_t)(-1))
            	*length = nand->size - *start;            
            return 1;
        }
        else if (rel == -1)
        {
            return 0;
        }
	
        /* EOS - we're done */
        if (*s == 0)
            break;

        /* does another spec follow? */
        if (*s != ';')
        {
            printf("bad character after partition (%c)\n", *s);
            return 0;
        }
        s++;
    }
    return 0;
}
#endif

/*****************************************************************************/
int find_part(char *s, char **retptr, char *ptnname, uint64_t *start,uint64_t *length)
{
    int isfind = 0;
    char delim = 0;

    /* fetch the partition size */
    if (*s == '-')
    {	/* assign all remaining space to this partition */
        (*length) = (uint64_t)(-1);
        s++;
    }
    else
    {
        (*length) = (uint64_t)memparse(s, &s);
        if ((*length) < 8192)
        {
            printf("partition size too small (%llx)\n", (*length));
            return -1;
        }
    }

    /* check for offset */
    if (*s == '@')
    {
        s++;
        memparse(s, &s);
    }

    /* now look for name */
    if (*s == '(')
        delim = ')';

    if (delim)
    {
        char *p;
        char *name;

        name = ++s;
        p = (char *)strchr(name, delim);
        if (!p)
        {
            printf("no closing %c found in partition name\n", delim);
            return -1;
        }

        *p = '\0';
        if (!strncmp(name, ptnname, strlen(ptnname) + 1))
            isfind = 1;
        *p = ')';
        s = p + 1;
    }

    /* test for options */
    if (strncmp(s, "ro", 2) == 0)
    {
        s += 2;
    }

    /* if lk is found do NOT unlock the MTD partition*/
    if (strncmp(s, "lk", 2) == 0)
    {
        s += 2;
    }

    /* test if more partitions are following */
    if (*s == ',')
    {
        int ret;
        if ((*length) == (uint64_t)(-1))
        {
            printf("no partitions allowed after a fill-up partition\n");
            return -1;
        }

        if (isfind)
            return 1;
        (*start) += (*length);
        ret = find_part(s + 1, &s, ptnname, start, length);
        *retptr = s;
        return ret;
    }
    *retptr = s;
    return isfind;
}

/*****************************************************************************/
unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */

	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G':
	case 'g':
		ret <<= 10;
	case 'M':
	case 'm':
		ret <<= 10;
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}
#endif /* CONFIG_GENERIC_NAND  */

#include <stdio.h>
#include <stdlib.h>

#define EI_NIDENT 16
#define SHN_UNDEF 0
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_NOBITS 8
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct {
	u32	    sh_name;		/* Section name (string tbl index) */
	u32	    sh_type;		/* Section type */
	u32	    sh_flags;		/* Section flags */
	u32	    sh_addr;		/* Section virtual addr at execution */
	u32	    sh_offset;		/* Section file offset */
	u32    sh_size;        /* Section size in bytes */
	u32    sh_link;        /* Link to another section */
	u32    sh_info;        /* Additional section information */
	u32    sh_addralign;       /* Section alignment */
	u32    sh_entsize;     /* Entry size if section holds table */
} Elf32_Shdr;

typedef struct {
	u8	eh_ident[EI_NIDENT];	/* Magic number and other info */
	u16	eh_type;			/* Object file type */
	u16	eh_machine;		/* Architecture */
	u32	eh_version;		/* Object file version */
	u32	eh_entry;		/* Entry point virtual address */
	u32	eh_phoff;		/* Program header table file offset */
	u32	eh_shoff;		/* Section header table file offset */
	u32    eh_flags;        /* Processor-specific flags */
	u16    eh_ehsize;       /* ELF header size in bytes */
	u16    eh_phentsize;        /* Program header table entry size */
	u16    eh_phnum;        /* Program header table entry count */
	u16    eh_shentsize;        /* Section header table entry size */
	u16    eh_shnum;        /* Section header table entry count */
	u16    eh_shstrndx;     /* Section header string table index */
} Elf32_Ehdr;

void usage()
{
	printf("Usage: \"elf_extract type infile outfile\"!!!\n");
}

Elf32_Ehdr *elf_head;
Elf32_Shdr *sec_head;

int main(int argc, char *argv[])
{
	if (argc < 4) {
		usage();
		return -1;
	}

	int ret;
	char *type = argv[1];
	if (strcmp(type, "globaltask") && strcmp(type, "rtosck")) {
		printf("Error:wrong type input.\n");
		return -1;
	}

	FILE *infile = fopen(argv[2], "rb");
	if (infile == NULL) {
		printf("Error:can't open file.\n");
		return -2;
	}
	FILE *outfile = fopen(argv[3], "ab");
	if (outfile == NULL) {
		fclose(infile);
		printf("Error:can't open file.\n");
		return -2;
	}

	elf_head = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
	if (elf_head == NULL) {
		fclose(infile);
		fclose(outfile);
		printf("Error:elf head read error!.\n");
		return -3;
	}

	ret = fread(elf_head, sizeof(Elf32_Ehdr), 1, infile);
	if (!ret) {
		fclose(infile);
		fclose(outfile);
		free(elf_head);
		printf("Error:elf head read error!.\n");
		return -3;
	}

	if (elf_head->eh_shnum == 0) {
		fclose(infile);
		fclose(outfile);
		free(elf_head);
		printf("Error:elf head shnum is zero!.\n");
		return -3;

	}

	if (fseek(infile, elf_head->eh_shoff, SEEK_SET) != 0) {
		fclose(infile);
		fclose(outfile);
		free(elf_head);
		printf("Error:elf head read error!.\n");
		return -3;
	}

	sec_head = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr) * elf_head->eh_shnum);
	if (sec_head == NULL) {
		fclose(infile);
		fclose(outfile);
		free(elf_head);
		printf("Error:elf head read error!.\n");
		return -3;
	}


	ret = fread(sec_head, sizeof(Elf32_Shdr), elf_head->eh_shnum, infile);
	if (!ret) {
		fclose(infile);
		fclose(outfile);
		free(elf_head);
		free(sec_head);
		printf("Error:section head read error!.\n");
		return -4;
	}

	int i = 0;
	int j = 0;
	unsigned int symtab_offset = 0;
	unsigned int strtab_offset = 0;

	Elf32_Shdr *sec_entry = sec_head;
	for (i; i < elf_head->eh_shnum; i++) {

		if (sec_entry->sh_size == 0) {
			sec_entry++;
			continue;
		}

		if (sec_entry->sh_type == SHT_SYMTAB) {
			symtab_offset = sec_entry->sh_offset;
			(void)fseek(infile, symtab_offset, SEEK_SET);
			char *sym_tab = (char *)malloc(sec_entry->sh_size);
			if (NULL == sym_tab) {
				printf("ERROR:malloc failed for sym_tab\n");
				free(elf_head);
				free(sec_head);
				fclose(infile);
				fclose(outfile);
				return -1;
			}

			(void)fread(sym_tab, sec_entry->sh_size, 1, infile);
			(void)fseek(outfile, 0, SEEK_END);
			(void)fwrite(sym_tab, sec_entry->sh_size, 1, outfile);
			free(sym_tab);
		} else if (sec_entry->sh_type == SHT_STRTAB) {
			if (j == 0 && !strcmp("rtosck", type)) {
				strtab_offset = sec_entry->sh_offset;
				(void)fseek(infile, strtab_offset, SEEK_SET);
				char *str_tab = (char *)malloc(sec_entry->sh_size);
				if (NULL == str_tab) {
					printf("ERROR:malloc failed for str_tab\n");
					free(elf_head);
					free(sec_head);
					fclose(infile);
					fclose(outfile);
					return -1;
				}

				(void)fread(str_tab, sec_entry->sh_size, 1, infile);
				(void)fseek(outfile, 0, SEEK_END);
				(void)fwrite(str_tab, sec_entry->sh_size, 1, outfile);
				free(str_tab);
				break;
			} else if (j == 1 && !strcmp("globaltask", type)) {
				strtab_offset = sec_entry->sh_offset;
				(void)fseek(infile, strtab_offset, SEEK_SET);
				char *str_tab = (char *)malloc(sec_entry->sh_size);
				if (NULL == str_tab) {
					printf("ERROR:malloc failed for str_tab\n");
					free(elf_head);
					free(sec_head);
					fclose(infile);
					fclose(outfile);
					return -1;
				}
				(void)fread(str_tab, sec_entry->sh_size, 1, infile);
				(void)fseek(outfile, 0, SEEK_END);
				(void)fwrite(str_tab, sec_entry->sh_size, 1, outfile);
				free(str_tab);
				break;
			}
			j++;
		}
		sec_entry++;
	}

	free(elf_head);
	free(sec_head);
	fclose(infile);
	fclose(outfile);
	return 0;
}

//dynamic load elf added
struct tee_symbol {
	unsigned long addr;
	const char *symbol;
};

/* For every exported symbol, place a struct in the __tee_symtab section */
#define __EXPORT_SYMBOL(sym, sec) \
	const char __tee_strtab_##sym[]	\
	__attribute__((section("__tee_symstr"), aligned(1))) \
		=  #sym; \
	const struct tee_symbol __tee_symtab_##sym \
	__attribute__((section("__tee_symtab" sec), unused)) \
		= { (unsigned long)sym, __tee_strtab_##sym }

#define EXPORT_SYMBOL(sym) \
	__EXPORT_SYMBOL(sym, "")


#define __EXPORT_DATA_SYMBOL(sym, sec) \
	const char __tee_strtab_##sym[] \
	__attribute__((section("__tee_symstr"), aligned(1))) \
		=  #sym; \
	const struct tee_symbol __tee_symtab_##sym \
	__attribute__((section("__tee_symtab" sec), unused)) \
		= { (unsigned long)&sym, __tee_strtab_##sym }

#define EXPORT_DATA_SYMBOL(sym) \
	__EXPORT_DATA_SYMBOL(sym, "")

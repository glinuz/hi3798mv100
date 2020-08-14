#+++++++++++++++++++++++++++++++++++++++++++++++++++
menu "SCI Config"
	depends on HI_SCI_SUPPORT

choice
	prompt "Clock Mode"
	depends on HI_SCI_SUPPORT
	help
	    HI_UNF_SCI_MODE_CMOS means CMOS output, HI_UNF_SCI_MODE_OD means OD output.
	default HI_SCI_CLK_MODE_CMOS

config  HI_SCI_CLK_MODE_CMOS
	bool "CMOS"
config  HI_SCI_CLK_MODE_OD
	bool "OD"
endchoice

choice
	prompt "VccEn Mode"
	depends on HI_SCI_SUPPORT
	help
	    HI_UNF_SCI_MODE_CMOS means CMOS output, HI_UNF_SCI_MODE_OD means OD output.
	default HI_SCI_VCCEN_MODE_CMOS

config  HI_SCI_VCCEN_MODE_CMOS
	bool "CMOS"
config  HI_SCI_VCCEN_MODE_OD
	bool "OD"
endchoice

choice
	prompt "Reset Mode"
	depends on HI_SCI_SUPPORT
	help
	    HI_UNF_SCI_MODE_CMOS means CMOS output, HI_UNF_SCI_MODE_OD means OD output.
	default HI_SCI_RESET_MODE_CMOS

config  HI_SCI_RESET_MODE_CMOS
	bool "CMOS"
config  HI_SCI_RESET_MODE_OD
	bool "OD"
endchoice

choice
	prompt "VccEn Pin Active Level"
	depends on HI_SCI_SUPPORT
	help
	    Low level active or high level active.
	default HI_SCI_VCCEN_LOW

config  HI_SCI_VCCEN_LOW
	bool "Low Level"
config  HI_SCI_VCCEN_HIGH
	bool "High Level"
endchoice

choice
	prompt "Detect Pin Active Level"
	depends on HI_SCI_SUPPORT
	help
	    Low level active or high level active.
	default HI_SCI_DETECT_LOW

config  HI_SCI_DETECT_LOW
	bool "Low Level"
config  HI_SCI_DETECT_HIGH
	bool "High Level"
endchoice
endmenu
#+++++++++++++++++++++++++++++++++++++++++++++++++++

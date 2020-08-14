#+++++++++++++++++++++++++++++++++++++
menu "SCI Config"
#+++++++++++++++++++++++++++++++++++++

config HI_SCI_SUPPORT
    bool "SCI Support"
    depends on !HI3798MV100
    default y

#+++++++++++++++++++++++++++++++++++++
endmenu
#+++++++++++++++++++++++++++++++++++++

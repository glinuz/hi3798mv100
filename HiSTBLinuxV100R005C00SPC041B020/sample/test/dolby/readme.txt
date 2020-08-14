"dolby" application
   executable program: dolby
   source code: dolby_main.c dolby_ir.c dolby_win.c dolby_dvbplay.c
   introduction: this demo shows the dolby approval function
   Command: ./sample_dolby
Usage:        
   1: After you run the './dolby ' command, you can see a program search window on the  screen.  There are three fields as follow:
       1) Freq: 0~999. The default value is 610. 
       2) Symborate:0~9999. The default value is 6875. 
       3) QAM:The value can be 16, 32, 64, 128, 256, or 512. The default value is 64 quadrature amplitude modulation (QAM). Using "LEFT" and "RIGHT" menu to change the value of qam.        	
       4) Tuner Type: There are eight tuner types for user to select.
      Notes:
       1) Please press "OK" menu of IR to set 'freq' or 'Symborate' fields onEdit,then you can set the value of current fields. 
       2) When you finish  to input the value , Press "OK" to set current fields offEdit. 
       3) use "UP" and "DOWN" menu to change focus field.
       4) use LEFT" and "RIGHT" menu to change  QAM or Tuner Type's Value.
       5) Select 'start search' dields ande press "ok" menu to start program search.
   2: When the DVB playing scenario is runing, you can see a channel bar on the botton of the screen. 
      Then channel bar contains Audio Stream Type, Dual Mono Type, Track Mode, Volume and Vodio Stream Type.
      Notes:
       1) "UP" and "DOWN" menu is design to control the DVB changing channels .
       2) "LEFT" and "RIGHT" menu is design to control Volume.
       3) "MUTE" menu is design to set Sond mute.
       4) "AUIDO" menu is design to set Track Mode of Sond.
       5) "MENU" menu is design to call PassThrough_Window Draw process.     
       6) "BACK" menu is design to display the ID of current program.
       7) "SEARCH" menu is design to go back to the  programe search window
       8) "DEL" menu is desing  to display and hide the channel bar. 
       9) '0~9' menu is design to play the program.
   3: When you enter to the PassThrough_Window, you can set the passthrough property of HDMI and SPDIF port. LPCM represents no passthrough.

Purpose: to demonstrate how to use the interface related to PQ.
Command: sample_pq [operating] [option] [value] -ch [chan]
Parameter:
	  operating: -s or --set: Set the PQ options;  -g or --get: Get the PQ options
          option:
                  -im, --imagemode: Change the image mode(value 0:natural/1:person/2:film/3:UD);
                  -df, --default: Init image mode (value 0:natural/1:person/2:film/3:UD);                 
                  -bn, --brightness: Change the brightness (value 0~100);
                  -co, --contrast: Change the contrast (value 0~100);
                  -hu, --hue: Change the hue (value 0~100);;
                  -sa, --saturation: Change the saturation (value 0~100);
                  -ct, --colortempe: Change the colortempe(value 0:cold/1:middle/2:warm);
                  -dc, --dyncontrast: Change the dyncontrast(value 0:low/1:middle/2:high/3:disable);
                  -ic, --intelligcol: Change the intelligentcolor(value 0:blue/1:green/2:BG/3:skin/4:vivid/5:disable);
                  -ga, --gamma: Change the gamma(value 0:intension_1.8/1:intension_2.5/2:light/3:dark/4:disable);
                  -sh, --sharpness: Change the sharpness (value 0~100);
                  -dn, --denoise: Change the denoise (value 0:disable/1:auto);
                  -fm, --filmmode: Change the filmmode (value 0:disable/1:auto);
                  -sf, --saveflash: Save all PQ options to flash;
                  -dp, --defaultparam: Set the default PQ configuration for video parameter test;
                  -a,  --all: Show all PQ options in current image mode
          value: Operating must be -s or --set
          chan: Set the chan,0:SD,1:HD,default:1
				 
Notice: pqparam must be burn 


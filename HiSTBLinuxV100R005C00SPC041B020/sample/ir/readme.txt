Purpose: to demonstrate how to get the key of infrared remotes
Command: sample_ir [-m 0|1] [-u 0|1] [-U num] [-r 0|1] [-R num] [-T readtimeout] [-E 0|1] [-g protocolname] [-l] [-s protocolname -e 0|1] [-h]
Parameter:
				 -m:sets whether key values or raw levels are obtained. -m 0 indicates that key values are obtained, 
				    and -m 1 indicates that raw levels are obtained. Key values are obtained by default.
				 每u:sets whether up events are reported. 每u 0 indicates that up events are not reported,
						and 每u 1 indicates that up events are reported. Up events are reported by default.
				 每U:sets the delay for reporting up events. An up event is reported if no repeated key
						value is received within a specific period after a key is pressed. The unit is ms. The value
						cannot be a negative one. The default value is 300 ms. This parameter currently cannot be set by users.
				 每r:sets whether repeated keys are reported. 每r 0 indicates that repeated keys are not
						reported, and 每r 1 indicates that repeated keys are reported. Repeated keys are reported
						by default. If a key on the remote control is quickly and repeatedly pressed, the driver
						reports repeated keys. If reporting of repeated keys is disabled, the driver does not report repeated keys but generates an alarm.
				 每R:sets the interval for reporting repeated keys. The unit is ms. The default value is 200ms. The value cannot be a negative one.
  			 每T:sets the read timeout interval. The driver does not time out by default and waits until
						an infrared key is pressed. The unit is ms. The value cannot be a negative one.
    		 每E:sets whether the infrared module is enabled. 每E 0 indicates that the infrared module
						is disabled and the chip does not receive any infrared signals. 每E 1 indicates that the
						infrared module is enabled, the chip can receive infrared signals, the driver parses and reports key values. The infrared module is enabled by default.
				 每g:used to check whether a protocol is enabled. The 每g parameter is followed by a
						protocol name. The protocol name can be obtained by using the 每l parameter. If the
						protocol name contains a space, use quotation marks to enclose the protocol name. For
						example, 每g "rc6 32bit data". If the protocol name does not contain a space, do not use quotation marks.
				 每l:used to obtain the names of all protocols supported by the infrared driver.
				 每s:The 每s parameter is valid only when it is used with the 每e parameter. The 每s
						parameter is used to set whether a protocol supported by the infrared driver is enabled. If
						a protocol is disabled, the driver discards and does not parse infrared signals complying
						with the protocol when receiving them. The 每s parameter is followed by a protocol name. If the protocol name contains a space,
						use quotation marks to enclose the protocol name. Example: 每s "rc6 32bit data"
				 每e:The 每e parameter is valid only when it is used with the 每s parameter. The 每e
						parameter is followed by the value 0 or 1. 0 indicates that a protocol is disabled, and 1
						indicates that a protocol is enabled. For details, see the 每s parameter.
				 每h:used to display help information and exit.
				 
Notice: The parameters are case-sensitive.
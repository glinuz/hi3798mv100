Purpose: to demonstrate how to detect key in front-panel and show data in LEDs.
Command: sample_keyled [keyled_type]
Parameter: keyled_type£º
					 0: 74HC164;
					 1: PT6961;
					 2: CT1642;
					 3: PT6964;
					 4: FD650.
Notice: Before selecting one kind of keyled, make sure the driver of it has been compiled into the code, 
        or the sample would fail to run. At the same time, check the pin multiplexing of that keyled.
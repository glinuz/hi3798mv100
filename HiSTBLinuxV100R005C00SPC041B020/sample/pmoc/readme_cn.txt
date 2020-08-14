功能描述：展示待机基本功能
命令格式：sample_pmoc TimerCount [Display] [Running Count] [Scene] [IR Type] [Keyled Type]
参数说明：TimerCount，定时时间: 0: 不使能定时唤醒功能; other: 唤醒时间（单位s），取值范围：1-0xffffffff
          Display，显示模式: 0: 前面板不显示; 1: 显示数字（0000-9999）; 2: 显示时间（hh:mm）.可选，默认值2    
          Running Count，运行次数: 取值范围：1-0xffffffff.可选，默认值1
          Scene，待机场景: 0: 标准场景; 1: 待机网口转发场景，可选，默认值0
          IR Type，唤醒遥控器类型: 0:nec simple; 1:tc9012; 2:nec full; 3:sony_12bit; 4:raw. 可选，默认值0
          Keyled Type，唤醒前面板类型: 0: 74hc164; 1: pt6961; 2: ct1642; 3: pt6964; 4: fd650.可选，默认值2
          Eth & USB Type: 0: 不使能; 1: USB鼠键唤醒; 2: 单播包唤醒; 3: 魔法包唤醒; 4: 唤醒帧唤醒.默认值0
          Reset: 0: 唤醒到DDR; 1: 唤醒到重启系统.
		      GPIO No.: 0: 不使能GPIO唤醒 ; 其它: gpio唤醒端口号. 
         
注意事项：1、测试待机网口转发场景时需要待机前先将两个网口都设置为Up状态；
          2、样例中的网络唤醒包的数据可以参考eth_package.txt文件(测试单板的MAC地址需要设置成00:71:83:05:08:13)。
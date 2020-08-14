功能描述：展示实体VI及VI-VO回环功能
命令格式：sample_vi [vo_format] [Width] [Height] [X] [Y] [VideoFormat]
参数说明：vo_format 视频输出制式 取值范围 1080P_60 | 1080P_50 | 1080i_60 | 1080i_50 | 720P_60 可选 默认值720P_50
					VideoFormat: 视频像素格式 取值范围 0:YUV422 | 1:YUV420 可选 默认值1:YUV420
例    子：sample_vi 720P_50 600 400 90 80 0
注意事项：
HI3798系列IC
1.sample运行前需要加载hi_hdmirx.ko。
HI3716CV200系列IC：
1.手动加载SIL9293.KO，“insmod pub/kmod/hi_sil9293.ko” 。
2.确认HDMI输入信号与SIL9293芯片已正确连接。
3.如果选用“hi3716cdmo2f_hi3716cv200_ddr3_2gbyte_8bitx4_4layers_XXX.reg”如下管脚复用为默认配置。
如果选择其他配置文件需要手动进行管脚复用配置。

a.确认SIL9293使用到的GPIO管脚已正确配置，包括I2C_SCL,I2C_SDA,int,reset
如果使用HI3716CV200ES芯片，管脚复用配置命令如下：
himm 0xf8a2104c 0x0   (GPIO2_3   I2C_SCL)
himm 0xf8a21050 0x0   (GPIO2_4   I2C_SDA)
himm 0xf8a21040 0x0   (GPIO2_0   RESET)
himm 0xf8a21044 0x0   (GPIO2_1   INT)
如果使用HI3716CV200芯片，管脚复用配置命令如下：
 A.选择硬件I2c (默认配置) 
himm 0xf8a2104c 0x5   (GPIO2_3   I2C_SDA)
himm 0xf8a21050 0x5   (GPIO2_4   I2C_SCL)
 B.选择软件I2c 
himm 0xf8a2104c 0x0   (GPIO2_3   I2C_SDA)
himm 0xf8a21050 0x0   (GPIO2_4   I2C_SCL)

b.gpio管脚复用配置
himm 0xf8a21128 0x0   (GPIO10_2  RESET)
himm 0xf8a21124 0x0   (GPIO10_1  INT)

c.确认VI管脚复用已正确配置，包括clk,de,filed,hsync,vsync,dat0~dat15
如果使用HI3716CV200ES芯片，管脚复用配置命令如下：
himm 0xf8a210d4 0x5
himm 0xf8a2106c 0x5
himm 0xf8a21070 0x5
himm 0xf8a21014 0x5
himm 0xf8a21018 0x5
himm 0xf8a210c0 0x5
himm 0xf8a210d0 0x5
himm 0xf8a210ec 0x5
himm 0xf8a210e8 0x5
himm 0xf8a2105c 0x5
himm 0xf8a210b8 0x5
himm 0xf8a210b4 0x5
himm 0xf8a210b0 0x5
himm 0xf8a210bc 0x5
himm 0xf8a210cc 0x5
himm 0xf8a210c8 0x5
himm 0xf8a210c4 0x5
himm 0xf8a210e0 0x5
himm 0xf8a210d8 0x5
himm 0xf8a210dc 0x5
himm 0xf8a210e4 0x5
如果使用HI3716CV200芯片，管脚复用配置命令如下：
himm 0xf8a21074 0x5
himm 0xf8a21078 0x5
himm 0xf8a2107c 0x5
himm 0xf8a21080 0x5
himm 0xf8a21084 0x5
himm 0xf8a21088 0x5
himm 0xf8a2108c 0x5
himm 0xf8a21090 0x5
himm 0xf8a21094 0x5
himm 0xf8a21098 0x5
himm 0xf8a2109c 0x5
himm 0xf8a210d8 0x5
himm 0xf8a210dc 0x5
himm 0xf8a210e0 0x5
himm 0xf8a210e4 0x5
himm 0xf8a21040 0x105
himm 0xf8a21044 0x5
himm 0xf8a21048 0x5
himm 0xf8a2106c 0x5
himm 0xf8a21070 0x5
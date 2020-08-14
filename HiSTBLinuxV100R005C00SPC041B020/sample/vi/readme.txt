Purpose:   This sample demonstrates the REAL VI and the VI->VO function.
Commmand:  sample_vi [vo_format] [Width] [Height] [X] [Y] [VideoFormat]
Parameter: vo_format: video display format, range: 1080P_60|1080P_50|1080i_60|1080i_50|720P_60, optional, default as 720P_50
           VideoFormat:  video pixel format range: 0:YUV422 | 1:YUV420  optional, default as 1:YUV420
Example:   sample_vi 720P_50 600 400 90 80 0
Attention: 
HI3798 Series IC
1.sample needs to be loaded before running hi_hdmirx.ko.
HI3716CV200 Series IC:
1. ¡±insmod /kmod/hi_sil9293.ko¡°.
2. Make sure the HIMI-IN and SIL9293 are connected correctly.
3. If you choose "hi3716cdmo2f_hi3716cv200_ddr3_2gbyte_8bitx4_4layers_XXX.reg" as follows pins reused to default configuration.
If you select a different configuration pin multiplexing configuration files need to be manually as follows.

a.Make sure multiple pins of GPIO used by SIL9293 have been set correctly, including I2C_SCL,I2C_SDA,int,reset
If you are using HI3716CV200ES chip, please using following commands:
himm 0xf8a2104c 0x0   (GPIO2_3   I2C_SCL)
himm 0xf8a21050 0x0   (GPIO2_4   I2C_SDA)
himm 0xf8a21040 0x0   (GPIO2_0   RESET)
himm 0xf8a21044 0x0   (GPIO2_1   INT)
If you are using HI3716CV200 chip, please using following commands:
	A.Select the Hardware I2C(default)
himm 0xf8a2104c 0x5   (GPIO2_3   I2C_SCL)
himm 0xf8a21050 0x5   (GPIO2_4   I2C_SDA)
	B.Select the Software I2C
himm 0xf8a2104c 0x0   (GPIO2_3   I2C_SCL)
himm 0xf8a21050 0x0   (GPIO2_4   I2C_SDA)

b.Gpio pin multiplexing configuration
himm 0xf8a21128 0x0   (GPIO10_2  RESET)
himm 0xf8a21124 0x0   (GPIO10_1  INT)

c. Make sure multiple pins of VI have been set correctly, including clk,de,field,hsync,vsync,dat0~dat15
If you are using HI3716CV200ES chip, please using following commands:
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
If you are using HI3716CV200 chip, please using following commands:
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
功能描述: 
        1,演示如何调用PDM模块的函数更新baseparam,logo,fastplay分区的参数和数据.
        2,演示如何从开机logo或者fastplay过渡到应用中的图形或视频播放.
命令格式: 
        1,update parameter and data:
	Usage: sample_mce_update image_type file
        image_type: 1:baseparam, 2:logo, 3:fastplay(dvb直播模式)
        example: sample_mce_update 1 null
        example: sample_mce_update 2 /mnt/test.jpg
        example: sample_mce_update 3 null

	2,transition :
	Usage: sample_mce_trans transtype [freq] [srate] [qamtype or polarization]
        transtype: 1:logo->graphics, 2:logo->video play, 3:fastplay->graphics, 4:fastplay->video play 
        example: sample_mce_trans 1 
        example: sample_mce_trans 2 3840 27500 0
        example: sample_mce_trans 3 
        example: sample_mce_trans 4 3840 27500 0
参数说明: see Usage.

注意事项: 在flash中必须存在baseparam分区，以及logo分区或fastplay分区中一个分区或两个分区。
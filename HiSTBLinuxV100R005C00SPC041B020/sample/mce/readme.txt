Purpose: 
        1,to demonstrate how to update parameter and data on baseparam,logo and fastplay partition with calling function of PDM module .
        2,to demonstarte how to transition from logo or fastplay to graphics or video play.
Command: 
        1,update parameter and data:
	Usage: sample_mce_update image_type file
        image_type: 1:baseparam, 2:logo, 3:fastplay(dvb mode)
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
Parameter: see Usage.

Notice: There must be 'baseparam' partition on flash; also 'logo',or 'fastplay' partition on flash.
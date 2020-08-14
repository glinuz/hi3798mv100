			localplay	specification

1¡¢play command
   The name of executable program is sample_localplay,you can execute follow command when
   you want to play a media file,xxx.mp3,for example.
   		./sample_localplay /filepath/xxx.mp3

   You can specify external subtitle files optionally:
  	  ./sample_localplay /filepath/xxx.mp3 -s /filepath/xxx1.lrc,/filepath/xxx2.lrc  	

   play a bluray dir:
      ./sample_localplay bluray:/bluray_dir_path/?playlist=-1
      
   play a bluray(ISO format),have 2 steps to do:
   ->mount the bluray iso file like this
      mkdir /tmp/avatar/;mount -o loop -t udf /mnt/usb/3DBLURAYISO.COM_AVATAR_3D.iso  /tmp/avatar/
   ->play the mounted bluray dir
      ./sample_localplay bluray:/tmp/avatar/?playlist=-1
        

2¡¢Frequently used control commands while playing in progress
	 h:help info
	 play£ºplay
	 pause:pause
	 seek: seek time_offset(in milliseconds,offset of start point of the playing file)
	 stop: stop
	 resume: resume
	 bw:fast backward play.bw 2,fast backward play in x2 speed.
	 ff:fast forward play.ff 2,fast forward play in x2 speed.
	 info:display the player information.
	 set sync 3000 0 0:extra add 3000ms to the pts(present timestamp) of current video stream.
     set sync 0 3000 0:extra add 3000ms to the pts(present timestamp) of current audio stream.
	 set sync 0 0 3000:extra add 3000ms to the pts(present timestamp) of current subtitle stream.
	 set mute:set audio mute.
	 set vol  20£ºset volume level to 20(0~100)
	 set vmode 0: set hdmi sink video mode(0-2D mode£¬1-3D FramePacking,2-3D SideBySide, 3-3D TopAndBottom, for the values of vmode, please refer to source files)
	 set id 0 0 1 0: switch stream id,the four parameter are programe id,video stream id, audio stream id,subtitle stream id	


3¡¢ HiPlayer(not ADVCA version) provide playready support£¬you need to know:
	1> Before using playready component, user must achieve the commercial device certificate from Microsoft Corp.
       User can't use PlayReady component without device certificate. 
	   
	2> The system will read the certificate automatically when the certificate is put at /system/bin/prpd/,or you need
	   to specify the path where it is stored like this:
	   ./sample_localplay ./xxxx.pyv --drm trans=1 mimetype=application/vnd.ms-playready DrmPath=./prpd/,LocalStoreFile=data.localstore
	   
	   Attention:After the "./prpd",there must be a '/'.
	             xxxx.pyv is the encrypted video file.
	   
	3> HiPlayer will register the playready engine automatically when the lib "libHiplayreadyEngine.so" is put at /system/lib/drm/,or
	   you need to specify the path where it is stored like this:
	   ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
	   
	4> All the audio or video files or .cms files for testing can be downloaded from the microsoft's Official Website:
	   http://playready.directtaps.net/pr/public/rightstests.aspx?detail=thO/caiU10mTrzhLjSR06g==
	   
	5> The tests.
	   Normal test:
		./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
		
	   Domain test(join domain):You need to dowload a stream file which have the domain certificate.
		./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=JoinDomain.cms
	   Domain test(leave domain):
		./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=LeaveDomain.cms

	   Evelope test£º
		./sample_localplay ./envelope_test.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=102 mimetype=application/vnd.ms-playready EnvelopFile=envelope_test.pye,\
		  EnvelopOutputFile=envelope_test.asf 
		Note:envelope_test.asf is created by decrypting envelope_test.pye
		
	   Metering test:You need to dowload a stream file which have the metering certificate.
	     Metering is used to limited the play times.
		 ./sample_localplay xxxx.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=Metering.cms
		 Note:After decrypting the stream file,you need to run the command once more to send the metering report to the server.
		 

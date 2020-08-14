1.get chip id 
   executable program£ºsample_ca_getchipid
   source code: sample_ca_getchipid.c
   introduction: print chipid on the debug screen.It will be shown like this "chip id:xxxxxxx"
   usage: sample_ca_getchipid

2.set jtag mode
   executable program£ºsample_ca_setjtagmode
   source code: sample_ca_setjtag.c
   introduction: set the work mode of jtag port.
   usage: sample_ca_setjtagmode mode[0|1|2]
          mode:0-Opened 1-Password Protected 2-Closed
          
3.enable security boot
   executable program£ºsample_ca_opensecboot
   source code: sample_ca_opensecboot.c
   introduction: enable security boot
   usage: sample_ca_opensecboot
          
4.tsplay demo
   executable program£ºsample_ca_tsplay
   source code:sample_ca_tsplay.c 
   introduction: this demo show the following functions
                 1> loading of clear CW or loading of encrypted CWPK and encrypted CWs onto the chipset
                 2> processing of a scrambled transport stream and rendering the descrambled and decoded video on a display
                 The video data can be output through CVBS YPBPR or HDMI interface.
                 The source code should be changed and recompiled if the CWs and CWPK need to change.
   usage: sample_ca_tsplay tsfile cwtype[0|1]
          cwtype: 0-ClearCw 1-EncrytedCw
          if there is more than one program in the tsfile,you can select program by number key.

   executable sample_ca_csa3_sp_tsplay
   source code:sample_ca_csa3_sp_tsplay.c
   introduction: this demo show the following functions           
                1> loading of clear CW or loading of encrypted CWPK and encrypted CWs onto the chipset
                2> processing of a scrambled transport stream and rendering the descrambled and decoded video on a display
                The video data can be output through CVBS YPBPR or HDMI interface.
                The source code should be changed and recompiled if the CWs and CWPK need to change.
   usage: sample_ca_csa3_sp_tsplay tsfile CwType KeyLadderType
           CwType: 0-ClearCw  1-TDES
           KeyLadderType:   sp csa3

5.dvbplay demo
   executable program£ºsample_ca_dvbplay
   source code:sample_ca_dvbplay.c 
   introduction:this demo show the following functions
                1>get chip id 
                2>lock the frequency
                3>load clear CWs or encrypted CWPK and encrypted CWs
                4>play program by appointed video pid and audio pid
                5>play program by appointed num
   usage: sample_ca_dvbplay
          You will see a command list when the program run.You can control the program according to the command list.

   executable program: sample_ca_dvbplay_sp                    
   source code:sample_ca_dvbplay_sp.c
   introduction:this demo show the following functions
                1>getchipid
                2>tunerlock freq symbrate tunerid
                3>loadcsa2cw type
                4>loadcsa3cw type
                5>loadaesidsacw type
                6>loadtdescbccw type
                7>playbynum prognum
                8>playbypid vpid apid
                9>stopplay
  usage: sample_ca_dvbplay
          You will see a command list when the program run.You can control the program according to the command list.

   executable program: sample_ca_dvbplay_misc
   source code:sample_ca_dvbplay_misc.c
   introduction:this demo show the following functions
                1>getchipid
                2>tunerlock freq symbrate tunerid
                3>loadcsa2cw type
                4>loadcsa3cw type
                5>loadaesidsacw type
                6>loadtdescbccw type
                7>playbynum prognum
                8>playbypid vpid apid
                9>stopplay   
  usage: sample_ca_dvbplay
          You will see a command list when the program run.You can control the program according to the command list.
     
6.encrypt and decrypt demo
   executable program£ºsample_ca_crypto
   source code:sample_ca_crypto.c 
   introduction:this demo show the following functions
                1> encrypting and decrypting data.
   usage: sample_ca_crypto
   
7.PVR record
executable program: sample_ca_pvrrec
source code:sample_ca_pvr_rec.c 
introduction:this demo show the PVR recording
usage: sample_ca_pvrrec
pvrrec file_path freq [SymbolRate] [qamtype] [vo_format]
      file_path: path of the recorded file. You do not need to enter the file name, because the file name is automatically generated based on the audio/video packet IDs (PIDs).
      The fields SymbolRate, qamtype, and vo_format are optional.
      The default value of SymbolRate is 6875.
      The value of qamtype is 16, 32, 64, 128, 256, or 512. The default value is 64QAM.
      The value vo_format is 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50. The default value is 1080i_60.
      
      Notes£º When running this sample ,user can enter 
      1. 'r' to stop the recording and start new  recording again.
      2. 'f' to create a test fingerprint which will be saved in the CA private file.
      2. 'm' to create a test maturity rating which will be saved in CA private file.
      
8.PVR play
executable program: sample_ca_pvrplay
source code:sample_ca_pvr_play.c 
introduction:this demo show the PVR playing
usage: sample_ca_pvrplay
sample_ca_pvrplay file_name [vo_format]
      file_name: name of the file to be played, including the storage path
      The value vo_format is 1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50. The default value is 1080i_60.

9.PVR timeshift
executable program: sample_ca_pvrtimeshift
source code:sample_ca_pvr_timeshift.c 
introduction:this demo show the PVR record and play with timeshift
Usage: sample_ca_pvrtimeshift file_path freq [SymbolRate] [qamtype] [vo_format]
      file_path: Path of the replayed file. The file name is not required because it is generated automatically based on the audio and video pid.
      SymbolRate, qamtype, and vo_format are optional.
      The default value of SymbolRate is 6875.
      The value of qamtype should be in the range of 16|32|[64]|128|256|512. The default value is 64 QAM.
      The value of vo_format should be in the range of 1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50. The default value is 1080i_50.

      After time shift is started, the command line interface is displayed, on which the following operations are allowed.
         l: live play
         n: normal play
         p: pause/play
         f: fast forward (4x)
         s: slow forward (4x)
         r: fast rewind (4x)
         h: help
         a: start a new recording again
         q: quit


10.set STB SN
executable program: sample_ca_setSTBSN
source code:sample_ca_setSTBSN.c 
introduction: set the STB SN.
usage: sample_ca_stbSTBSN 

11.set MarketID
executable program: sample_ca_setMarketID
source code:sample_ca_setMarketID.c 
introduction: set the MarketID.
usage: sample_ca_setMarketID 

12.SWPK Keyladder
executable program: sample_ca_swpk_keyladder
source code:sample_ca_swpk_keyladder.c 
introduction: show the usage of SWPK keyladder.
usage: sample_ca_swpk_keyladder

13.SWPK
executable program: sample_ca_swpk
source code:sample_ca_swpk.c 
introduction: show the usage of how to encrypt SWPK, this sample is used for MV200 chipset.
usage: sample_ca_swpk

14.aes_cbc_mac
executable program: sample_ca_aes_cbc_mac
source code:sample_ca_aes_cbc_mac.c
introduction: show the usage of AES CBC-MAC algorithm.
usage: sample_ca_aes_cbc_mac

15.RSAkey
executable program: sample_ca_writeRSAkey
source code:sample_ca_writeRSAkey.c
introduction: show how to set RSAkey
usage: sample_ca_writeRSAkey 

16.version id
executable program: sample_ca_setversionid
source code:sample_ca_setversionid.c
introduction: show how to set version id
usage: sample_ca_setversionid

17.Set OTP fuse
executable program: sample_ca_set_otp_fuse
source code:sample_ca_set_otp_fuse.c
introduction: set OTP fuse
usage: sample_ca_set_otp_fuse num

18.Lock hardcw 
executable program: sample_ca_lockhardcw
source code:sample_ca_lockhardcw.c
introduction: show how to set lockhardcw otp fuse
usage: sample_ca_lockhardcw

19.judge marketid set or not
executable program: sample_ca_isMarketIdSet
source code:sample_ca_isMarketIdSet.c
introduction: show the marketId set or not
usage: sample_ca_isMarketIdSet

20.get trustzones status
executable program: sample_ca_gettrustzonestatus
source code:sample_ca_gettrustzonestatus.c
introduction: show the trustzones enable or not
usage: sample_ca_gettrustzonestatus

21.get OTP fuse
executable program: sample_ca_get_otp_fuse
source code:sample_ca_get_otp_fuse.c
introduction: show how to get otp fuse status
usage: sample_ca_get_otp_fuse

22.enable trustzone
executable program: sample_ca_enabletrustzone
source code:sample_ca_enabletrustzone.c
introduction: show how to enable trustzone
usage.sample_ca_enabletrustzone

23.encrypt boot demo
executable program: sample_ca_blpk
source code:sample_ca_blpk.c
introduction: show how to encrypt boot and burn it to flash
usage: sample_ca_blpk


说明
1.本工具是CASignTool.exe签名工具的linux版本
2.本工具使用cfg文本导入配置信息(文件名可更改)
3.目前工具不支持各种路径中存在空格
4.使用方式如下：

  1>./CASignTool 0 ../cfg/Signboot_config.cfg
  仅签名Key区：如果三个Key文件路径，仅填写了Root_private_key_file、External_public_key_file和BootFile的二者之一，工具会仅签名Key区，BootFile的输入可以是fastboot.bin或者keyarea.bin
  不设置ROOT_RSA_Private_Key，设置external priv key，external pub key，生成其他区域的镜像及签名，以及key区的镜像
  
  签名所有区
  从Signboot_config.cfg读入chiptype,key file,cfg.bin,msid,version,inputdir,outputdir 等信息，签名镜像生成在OutputFileDir参数设置的目录。
  可用-k -b -r -o等选项配置对应的key文件、boot表格、源文件、输出文件的路径

  2>./CASignTool 1 ../cfg/merge_config.cfg
  合并各区文件为最终的boot
  从merge_config.cfg读入chiptype, 已签名 bin 文件,inputdir,outputdir 等信息，合成final boot生成在OutputFileDir参数设置的目录。
  可用 -r -o等选项配置对应的源文件、输出文件的路径

  3>./CASignTool 2 ../cfg/common_config.cfg
  非boot镜像common方式签名。
  从common_config.cfg读入rsa签名私钥，待签名文件，签名后的文件名，是否分段签名,inputdir,outputdir 等信息,签名镜像在OutputFileDir参数设置的目录。
    说明： SignCommon_config.cfg中参数SectionSize，如果为0，则不分段签名；如果分段签名，则设置此值为分段大小，
           例如按1MB大小分段签名，请设置SectionSize=1.
  可用-k -r -o等选项配置对应的key文件、源文件、输出文件的路径

  4>./CASignTool 3 ../cfg/special_config.cfg  
  非boot镜像special方式签名。
  从special_config.cfg读入rsa签名私钥，待签名文件，签名后的文件名，CustomerData数据,InputFileDir,OutputFileDir 等信息，签名后镜像放在OutputFileDir目录
    说明： special_config.cfg中虽有参数SectionSize，HeaderSize、RSALength、HashMode等，但暂时只支持默认的.  
           其中ImageNumber参数为需要签名的镜像数，增加一个签名镜像，增加ImageXXX，PartitionSizeXXX和SignImageXXX。XXX为索引数，依次增加。   
  可用-k -r -o等选项配置对应的key文件、源文件、输出文件的路径
      
  5>./CASignTool 4 ../cfg/crypto_config.cfg
  非boot镜像加密和解密。
  从crypto_config.cfg读入rsa签名私钥文件、AES Key和IV向量，待签名文件，签名后的文件名，InputFileDir,OutputFileDir 等信息，加密后镜像放在OutputFileDir参数设置的目录。
    说明： 需要根据需要配置crypto_config.cfg中的参数，比如AES加密，一定需要配置加密算法Algorithm，加密模式Mode，加密私钥Key，和IV向量.  
           支持批量加密或者解密。ImageNumber参数为需要加解密的镜像数，需要与实际的镜像数一致，否则会报错。
           增加一个签名镜像，增加InputXXX和OutputXXX参数，分别代表输入和输出，XXX为索引数，依次增加。

  6>./CASignTool 5 ../cfg/VMXSignCrypto_config.cfg -K keyFilePath -R srcFilePath -O destFilePath

  VMX签名加密镜像
  从VMXSignCrypto_config.cfg读入签名算法、模式、AES Key和IV向量等信息，并用命令指定key文件、源文件和目标文件的路径进行签名加密。
    说明： 需要根据需要配置VMXSignCrypto_config.cfg中的参数，比如AES加密，一定需要配置加密算法Algorithm，加密模式Mode，加密私钥Key，和IV向量.  

  7>./CASignTool 6 ../cfg/special_crypto_config.cfg
  非boot镜像special方式签名加密
  从special_crypto_config.cfg读入rsa签名私钥文件、签名算法、AES Key和IV向量，待签名文件，签名后的文件名，InputFileDir,OutputFileDir等信息，加密后镜像放在OutputFileDir参数设置的目录。
    说明： 需要根据需要配置special_crypto_config.cfg中的参数，比如AES加密，一定需要配置加密算法Algorithm，加密模式Mode，加密私钥Key，和IV向量.  
           支持批量处理。ImageNumber参数为需要加解密的镜像数，需要与实际的镜像数一致，否则会报错。
           增加一个签名镜像，增加InputXXX和OutputXXX参数，分别代表输入和输出，XXX为索引数，依次增加。 
  可用-k -r -o等选项配置对应的key文件、源文件、输出文件的路径

  8>./CASignTool 7 ../cfg/verifyboot.cfg
  验证Finalboot。
  从verifyboot.cfg读入Finalboot镜像、root pub key、msid、version、msidmask、versionmask、是否为加密镜像、加密密钥等信息，验证FinalBoot是否为正确生成。

  9>./CASignTool 8 eValue keyDestPath
  生成RSA密钥。
  输入素数eValue，以及密钥生成目录keyDestPath。创建的密钥生成到密钥生成目录

  10>./CASignTool 9 RSAPubKeyPath RSAPriKeyPath AESKeyPath DdrInitPath UBootPath OutputFilePath
  生成匹配Hi3519V101的Boot。
  输入rsa公钥、rsa私钥、aes密钥、ddrinit二进制文件路径、uboot二进制文件路径以及最终输出的文件路径，即可生成最终的boot文件。
aes密钥仅支持txt格式，需要是合法的十六进制数，16字节（32字符）长，并不以0x开头。若密钥超过16字节会截取前16字节进行处理。
   
   
           
4.输入文件，输出文件名可在cfg文件里面配置
5.boot镜像签名时，支持的芯片包括Hi3798CV200,Hi3716MV300,Hi3716CV200,Hi3719MV100,Hi3719CV100,Hi3796CV100,Hi3798CV100,Hi3798MV100,Hi3716MV420,Hi3716MV410,Hi3110EV500,Hi3796MV100,可在cfg文件中配置，非boot镜像其他与芯片无关。
6.cfg文件自动去除以#开头的注释行
7.其他问题请另外咨询

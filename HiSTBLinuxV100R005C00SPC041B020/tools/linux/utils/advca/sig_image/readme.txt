
一、使用说明
1.本工具是CASignTool.exe签名工具的linux版本，当前功能在完善。
2.本工具使用cfg文本导入配置信息(文件名可更改)
3.目前支持五种使用方式
  1>Linux_CA_Boot_Sign.exe 0 Signboot_config.cfg 由Signboot_config.cfg读入
    chiptype,key file,cfg.bin,msid,version id,inputdir,outputdir 等信息,签
    名镜像生成在outputdir
  2>Linux_CA_Boot_Sign.exe 1 merge_config.cfg  由merge_config.cfg读入
    chiptype, 已签名 bin 文件,inputdir,outputdir 等信息,合成final boot生成在outputdir
  3>Linux_CA_Boot_Sign.exe 2 common_config.cfg  common_config.cfg读入
    rsa签名私钥，待签名文件，签名后的文件名，是否分段签名,inputdir,outputdir 等信息,签名镜像在outputdir目录
    说明： SignCommon_config.cfg中参数SectionSize，如果为0，则不分段签名；如果分段签名，则设置此值为分段大小，
           例如按1MB大小分段签名，请设置SectionSize=0x100000.
  4>Linux_CA_Boot_Sign.exe 3 special_config.cfg  非boot镜像special方式签名。
    由special_config.cfg读入rsa签名私钥，待签名文件，签名后的文件名，CustomerData数据,InputFileDir,OutputFileDir 等信息,
    签名后镜像放在OutputFileDir目录
    说明： special_config.cfg中虽有参数SectionSize，HeaderSize、RSALength、HashMode等，但暂时只支持默认的.  
           其中ImageNumber参数为需要签名的镜像数，增加一个签名镜像，增加ImageXXX，PartitionSizeXXX和SignImageXXX。XXX为索引数，依次增加。         
  5>Linux_CA_Boot_Sign.exe 4 crypto_config.cfg  非boot镜像加密和解密。
    crypto_config.cfg读入rsa签名私钥文件、AES Key和IV向量，待签名文件，签名后的文件名，InputFileDir,OutputFileDir 等信息,
    加密后镜像放在OutputFileDir参数设置的目录。
    说明： 需要根据需要配置crypto_config.cfg中的参数，比如AES加密，一定需要配置加密算法Algorithm，加密模式Mode，加密私钥Key，和IV向量.  
           支持批量加密或者加密。ImageNumber参数为需要加解密的镜像数，需要与实际的镜像数一致，否则会报错。
           增加一个签名镜像，增加Input1XXX和OutputXXX参数，分别代表输入和输出，XXX为索引数，依次增加。         
   				 
           
4.输入文件，输出文件名可在cfg文件里面配置
5.boot镜像签名时，支持的芯片包括Hi3716MV300,Hi3716CV200,Hi3719MV100,Hi3719CV100,Hi3796CV100,Hi3798CV100,Hi3798MV100,Hi3716MV420,Hi3716MV410,Hi3110EV500,Hi3796MV100,可在cfg文件中配置，非boot镜像其他与芯片无关。
6.cfg文件自动去除以#开头得注释行
7.其他问题请另外咨询

二、开发说明
    1. 本工具使用开源openssl v1.0.1g版本，并在其版本基础上做了些修改适配，
       源码包为openssl-1.0.1g_hisi.tar.gz和已经编译好libcrypto.a库均已放在openssl目录中。
    2. openssl源码包解压： tar -zxvf openssl-1.0.1g_hisi.tar.gz
       编译openssl源码请参考解压后目录中的readme.txt文件。
    3. 生成可执行程序Linux_CA_Boot_Sign的方法：
	   3.1 32位Linux服务器平台：
	        步骤1：在32位服务器平台上，参考openssl-1.0.1g_hisi.tar.gz中的readme.txt，编译生成
			       libcrypto.a，并拷贝此libcrypto.a到编译目录Linux_CA_Boot_Sign_VX.X.X.X/openssl/libcrypto.a
			步骤2：在根目录执行：make PLATFORM=32 (或 make PLATFORM=32 clean)
	   3.2 64位服务器，请直接在根目录执行：
	   		请直接在根目录执行：make (或 make clean)
	   		
三、注意事项
		1. 部分CA厂商要求加密功能在板端或者由CA公司完成，因此SDK中合入的签名工具不制作加密镜像。需要结合sample/advca/下的sample完成镜像的加密烧写。
		2. 本工具可作为单独的组件使用，操作步骤：
			a. 编译SDK生成高安镜像，将pub/image拷贝到SignInput。
			b. 生成apploader、stbid镜像，并拷贝到SignInput目录。
			c. 执行sig.sh ca_type chiptype cfg_file可生成签名镜像。
		3. 如果需要替换root rsa key或者external rsa	key，需要修改配置文件。
			替换方法：本工具SignInput目录下mv410_conax_integration_rsa_*.txt是测试所选用的external rsa key。替换external rsa key时在工具目录搜索mv410_conax_integration_rsa_*.txt
			并将搜索到的相应的地方替换成你所需要的external rsakey。
			本工具SignInput目录下rsa_root_*.txt是测试所选用的root rsa key。替换方法同external rsa key。

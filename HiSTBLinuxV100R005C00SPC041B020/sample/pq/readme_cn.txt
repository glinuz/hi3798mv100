功能描述：展示图像质量的各类操作
命令格式：sample_pq [operating] [option] [value] -ch [chan]
参数说明：operating，操作类型: -s或--set: 设置;  -g或--get: 获取、显示
          option，选项:
                  -im, --imagemode: 图像模式(value值0:自然模式/1:人物模式/2:电影模式/3:自定义模式); 
                  -df, --default: 初始化图像模式(value值0:自然模式/1:人物模式/2:电影模式/3:自定义模式);                 
                  -bn, --brightness：亮度（value值0~100）；
                  -co, --contrast：对比度（value值0~100）；
                  -hu, --hue ：色调（value值0~100）；
                  -sa, --saturation：饱和度（value值0~100）；
                  -ct, --colortempe：色温(value值0:冷色温/1:中色温/2:暖色温)；
                  -dc, --dyncontrast：动态对比度(value值0:低/1:中/2:高/3:关闭)；
                  -ic, --intelligcol：智能颜色(value值0:蓝色/1:绿色/2:蓝绿增强/3:肤色/4:生动/5:关闭)；
                  -ga, --gamma：:伽马模式选择(value值0:1.8强度/1:2.5强度/2:亮/3:暗/4:关闭)；
                  -sh, --sharpness：锐化强度（value值0~100）；
                  -dn, --denoise：降噪使能(value值0:关闭/1:打开)；
                  -fm, --filmmode：电影模式(value值0:关闭/1:打开)；
                  -sf, --saveflash：保存参数到Flash；
                  -dp, --defaultparam：为入网指标测试设置PQ的默认值；
                  -a,  --all：显示所有图像质量信息。
          value，参数值: 各选项参数值意义不同，见上面所述，参数值只在Operating为设置时才有效
          chan，高清/标清通道: 0: 标清模式; 1: 高清模式，默认值1 
         
注意事项：运行sample_pq前务必保证Flash烧写了pqparam分区
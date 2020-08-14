    when you run CA version you should do as follows first
    (1) mkdir -p /home/stb/bin/gpu
    (2) cp (your SDK)/Develop/sample/gpu/exe /home/stb/bin/gpu -rf
    (3) filecap /home/stb/bin/gpu/exe/es11/coverflow sys_rawio                           
	filecap /home/stb/bin/gpu/exe/es11/coverflow_vertical sys_rawio                  
	filecap /home/stb/bin/gpu/exe/es11/cylinder sys_rawio                            
	filecap /home/stb/bin/gpu/exe/es11/mesh sys_rawio                                
	filecap /home/stb/bin/gpu/exe/es11/videocube sys_rawio                           
	filecap /home/stb/bin/gpu/exe/es20/sphere sys_rawio
    
    
    
    /**=====================================================================================================**/
    now you can run the bin file

    (1) run exe/es11/coverflow
        (a) you can run this bin with ./coverflow
        (b) when you run it, you can use "left" and "right" key control the coverflow motion,
            and exit the program by "back" key.

    (2) run exe/es11/coverflow_vertical
        (a) you can run this bin with ./coverflow
        (b) when you run it, you can use "left" and "right" key control the coverflow motion,
            and exit the program by "back" key.
            
    (3) run exe/es11/cylinder
        (a) you can run this bin with ./coverflow
        (b) when you run it, you can use "left" and "right" and "up" and "down" key control the coverflow motion,
            and exit the program by "back" key.
            
    (4) run exe/es11/mesh
        (a) you can run it by 
            ./mesh
        
    (5) run exe/es20/sphere
        (a) you can run it by 
            ./sphere

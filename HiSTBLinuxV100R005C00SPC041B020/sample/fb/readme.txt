demo description:
    framebuffer usage example

Default Command: ./sample_fb, this would using /dev/fb0 for the framebuffer device, pixel format was ARGB8888
Extend Command : ./sample_fb /dev/fb0(/dev/fb1|/dev/fb2|/dev/fb4) TAB(FMT/SBS) 32(16) 
/dev/fb0 --> framebuffer device, 
TAB --> stereomode top and bottom, FP --> framepacking, SBS side by side
32 --> ARGB8888, 16 --> ARGB1555
1 Run the ./sample_fb command. Color blocks are displayed from top to bottom, and the top color block is in blue.

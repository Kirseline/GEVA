# GEVA
 Good Enough Video Abstraction
 
 ![geva_logo](https://github.com/Kirseline/GEVA/assets/90918734/ad7a79de-167d-4351-8dcd-b5615041f18a)


# USE
* include GEVA.h into main
* include driver for the MCU in usage
* create buffer (video_buffer vid_buf;)
* initialize hardware i2c (for now)
* initialize dispay comunication (init_display(0x78, &hi2c1);)
* initialize buffer (init_buf(&vid_buf, 128, 64);)
* whenever you whant to display use update_display(&vid_buf);



NOTE: not really test with smaller lcd than 128x64

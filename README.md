# YUV-encryption-decryption-player
This is a player for YUV videos with encryption &amp; decryption capabilities (based on YUV_Viewer, see ReadMe.txt in folder "YUVviewer_EncDec"). It can encrypt/decrypt videos with different strength, as wll as in different channels (Y/U/V/all 3 channels).  
The YUV videos can be downloaded from http://trace.eas.asu.edu/yuv/.  
Encrypted file is named "XXX.e.yuv".  
Decrypting will not generate a video file. The player will only decrypt and play the decrypted video.  

EXE file: YUVviewer_EncDec.exe

Key input instructions:  
Low strength encryption/decryption: 24bit key, 3 numbers no more than 255, separated by a comma (example: 20,50,10).  
High strength encryption/decryption: 80bit key, 10 numbers no more than 255, separated by a comma (example: 20,50,10,59,166,1,23,56,55,99).

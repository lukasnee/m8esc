
sudo /mnt/c/Program\ Files/SEGGER/JLink_V794b/JLinkRTTLogger.exe -Device STM32H750VB -If SWD -Speed 4000 -RTTChannel 2 speedscope.log 

arm-none-eabi-nm -lnC .build/platform/STM32H750/STM32H750 > .build/platform/STM32H750/STM32H750.symbols
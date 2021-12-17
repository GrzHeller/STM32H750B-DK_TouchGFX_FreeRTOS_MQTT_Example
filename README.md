Author: Grzegorz Heller  
Created on: 30.11.2021  
Updated on: 17.12.2021  
Tested and found working on:  

CubeIDE version:  
CubeMX version:  
TouchGFX version:  

Video demonstration: https://youtu.be/hxHlEgOlPiU

# STM32H750B-DK_TouchGFX_FreeRTOS_MQTT_Example
## Introduction
<p align = "justify"> This is a guide on how to create a working MQTT application with TouchGFX on STM32H750B-DK development board. This guide covers all issues I have encountered during the setup of my MQTT project so that you will hopefully be able to painlessly create a working sample project and go from there. I created this guide due to the lack of any comprehensive tutorials on MQTT for STM32H750B-DK I have found. Follow these instructions carefully and exactly as specified, otherwise you will encounter issues, for example naming conflicts or hardfaults. This guide assumes you know your way around the used tools. I won't be explaining much here. If you wish to know why I did what I did, please consult the useful links section. If you find a step unclear and do not know how to follow, find any other issues or have suggestions, feel free to contact me here: grzegorz4heller@gmail.com. </p>

## Useful links
### STM32H750B-DK
Official ST "tutorials" for H7:  
https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-online-training/stm32h7-online-training.html  

Official ST example projects for STM32H750B-DK:  
https://github.com/STMicroelectronics/STM32CubeH7/tree/master/Projects/STM32H750B-DK  

### TouchGFX
TouchGFX docs:  
https://support.touchgfx.com/docs/introduction/welcome  

Introduction to TouchGFX by embryonic.dk:  
https://www.youtube.com/watch?v=9QDWti5RzBU  

Tutorial on creating a more advanced GUI application with TouchGFX by EE by Karl:  
https://www.youtube.com/watch?v=uE6eiTjOP-g  

How to enable "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" without getting a white screen:  
https://community.st.com/s/question/0D53W00001DkHUrSAN/white-screen-after-checking-generate-peripheral-initialization-as-a-pair-of-ch-files-per-peripheral-in-cubemx  

### Ethernet and LWIP
Tutorial on Ethernet configuration by Controllers Tech:  
https://www.youtube.com/watch?v=8r8w6mgSn1A  

Another useful information about Ethernet configuration:  
https://community.st.com/s/article/FAQ-Ethernet-not-working-on-STM32H7x3  

Tutorial on how to make LWIP stack working:  
https://community.st.com/s/article/How-to-create-project-for-STM32H7-with-Ethernet-and-LwIP-stack-working  

### FreeRTOS, newlib and reentrancy
Newlib and reentrancy with FreeRTOS:  
https://nadler.com/embedded/newlibAndFreeRTOS.html  

A useful thread about reentrancy and newlib issues:  
https://forums.freertos.org/t/why-does-rand-or-random-return-0-in-threads-but-is-ok-in-main/10065  

### MQTT
Official LWIP MQTT docs:  
https://www.nongnu.org/lwip/2_0_x/group__mqtt.html  

Tutorial on paho MQTT by eziya:  
https://github.com/eziya/STM32F4_HAL_ETH_MQTT_CLIENT  

Tutorial on LWIP MQTT by M.f.abouHASHEM:  
https://www.youtube.com/watch?v=8P3_R6Xmhb0&t=2s  

# 1. TouchGFX
Start by creating a project for STM32H750B-DK by selecting the template.  
<p align = "center"> <img src = "images/tgfx_project1.PNG" align = "middle" /> <img src = "images/tgfx_project2.PNG" align = "middle"  /> </p>
Create a similar interface.  
<p align = "center"> <img src = "images/tgfx_interface1.PNG" align = "middle" /> </p>
It is important to have the same resource names as here to avoid naming issues later.  
<p align = "center"> <img src = "images/tgfx_resource_names1.PNG" align = "middle" /> </p>
In order to get the zero and one images you have to put the attached zero.png and one.png files into your project images.  
<p align = "center"> <img src = "images/tgfx_images1.PNG" align = "middle" /> </p>
These two images should be hidden by default, but it is not crucial.  
<p align = "center"> <img src = "images/tgfx_visible1.PNG" align = "middle" /> </p>
Create a wildcard for the textArea.  
<p align = "center"> <img src = "images/tgfx_wildcard1.png" align = "middle" /> </p>
To prevent issues it may be necessary to put a couple characters into the wildcard typography.  
<p align = "center"> <img src = "images/tgfx_typography1.PNG" align = "middle" /> </p>
Create two interactions exactly like this.  
<p align = "center"> <img src = "images/tgfx_interactions1.png" align = "middle" /> </p>
Generate the code. If the white circle doesn't disappear after the first generation, generate again just to be safe.  

# 2. CubeMX
<p align = "justify"> Import the Cube project by going to STM32CubeIDE folder inside the root folder of your TouchGFX project and launching the .cproject or .project file. Open the .ioc file from withing the workspace. The order of the following steps should not matter much. </p>

<p align = "justify"> Enabling the ETHERNET module in Connectivity tab is prevented by pin conflict. This can be "fixed" by unassigning the PA2 pin labeled LCD_RESET. I do not know whether the role of LCD_RESET could be assigned to another pin and was not interested in finding out yet. </p>
<p align = "center"> <img src = "images/mx_gpio.PNG" align = "middle" /> </p>
<p align = "justify"> Now we can enable ETHERNET module. Also enable Ethernet global interrupt with Preemption Priority 5. </p>
<p align = "center"> <img src = "images/mx_ethernet.PNG" align = "middle" /> </p>
<p align = "center"> <img src = "images/mx_nvic.PNG" align = "middle" /> </p>
<p align = "justify"> You should be careful with this module's pinout as some users reported the default one could be wrong. If you are unsure, check the datasheet/schematic of your board. Assuming you are using the same board as me, you should be fine with my settings. </p>
<p align = "center"> <img src = "images/mx_ethernet_gpio.PNG" align = "middle" /> </p>
<p align = "justify"> Now enter CORTEX_M7 in System Core and add these two sections. </p>
<p align = "center"> <img src = "images/mx_cortex.PNG" align = "middle" /> </p>
<p align = "justify"> Enable LWIP. Thus far I have been using static IP address. Set the IP address of the device. </p>
<p align = "center"> <img src = "images/mx_lwip_general.PNG" align = "middle" /> </p>
<p align = "justify"> Set the MEM_SIZE and LWIP_RAM_HEAP_POINTER as shown below. </p>
<p align = "center"> <img src = "images/mx_lwip_key.PNG" align = "middle" /> </p>
<p align = "justify"> Set the platform. </p>
<p align = "center"> <img src = "images/mx_lwip_platform.PNG" align = "middle" /> </p>
<p align = "justify"> Increase the MINIMAL_STACK_SIZE of FreeRTOS just in case. </p>
<p align = "center"> <img src = "images/mx_freertos_config.PNG" align = "middle" /> </p>
<p align = "justify"> I prefer enabling NEWLIB to prevent the annoying warning. </p>
<p align = "center"> <img src = "images/mx_freertos_newlib.PNG" align = "middle" /> </p>
<p align = "justify"> Create three queues used to communicate between MQTT and TGFX tasks. </p>
<p align = "center"> <img src = "images/mx_freertos_queues.PNG" align = "middle" /> </p>
<p align = "justify"> If you want to use "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" option, consult useful links section. Generate the code. </p>

# 3. CubeIDE
<p align = "justify"> This is the part which caused me the most trouble. There are three very important steps we should do first here. Firstly, let's make the appropriate changes to the FLASH.ld file. Add the following section to the file. </p>
<p align = "center"> <img src = "images/ide_flashld.PNG" align = "middle" /> </p>
<p align = "justify"> These addresses correspond to the sections we have set in CORTEX_M7 in MX. </p>

<p align = "justify"> The PINGREQ functionality is disabled by default. This might be an overlook from ST's part. To enable it, find lwipopts.h file and add the following line into the user code section. </p>
<p align = "center"> <img src = "images/ide_lwipopts.PNG" align = "middle" /> </p>
<p align = "justify"> This way we overwrite the macro that is set by default in opt.h file with a wrong value (for our purpose) and also prevent CubeMX from deleting this change on regeneration if we set the value directly in opt.h file. The increase of this value by 1 is necessary to enable sys timeouts accounting for MQTT, which in turn is necessary to enable automatic PINGREQ functionality required by the MQTT standard. </p>

<p align = "justify"> Last important thing to do is to remove the sysmem.c file from the project. This file's code causes issues with code reentrancy, which invariably causes hardfault errors in this project. <p>

<p align = "justify"> After all this you can overwrite your project files with the files provided here. I have found it necessary to always regenerate the project inside TouchGFX before building and uploading the code onto my device, especially after regenerating the code in CubeMX. This however might not actually be necessary. Build the project and run the code on your board. </p>

# 4. Windows 10
<p align = "justify"> Testing the application with static IP requires you to make some changes in your Ethernet configuration. </p>
<p align = "center"> <img src = "images/windows_ethernet.PNG" align = "middle" /> </p>
<p align = "justify"> Upon connecting your device to the PC over LAN, a network should be created, allowing to start up a broker under the gateway address we have set up in CubeMX. This step may be troublesome, because the network doesn't always change to the gateway address we have set. Just try until you succeed. If everything went as planned, you now should be able to see something like this after pressing the Subscribe button. Your device should also be sending PINGREQ message about every 10 seconds (this is it's keep alive time). </p>
<p align = "center"> <img src = "images/windows_mosquitto.PNG" align = "middle" /> </p>
<p align = "justify"> Just to be safe I have also provided my Mosquitto configuration file. To run Mosquitto with a configuration file, use this command with the path to your configuration file: <br>
mosquitto -c c:\mosquitto\configuration.conf -v </p>

# 5. Conclusion
<p align = "justify"> Hopefully you now have a very basic, but working device utilising MQTT communication and a project that did not cost you hours of your time figuring out why something doesn't work. You can analyse the code provided by me or look into the links provided by me to find out more and go from there. </p>

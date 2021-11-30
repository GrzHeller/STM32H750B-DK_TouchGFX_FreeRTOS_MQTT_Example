Author: Grzegorz Heller  
Created on: 30.11.2021  

# STM32H750B-DK_TouchGFX_FreeRTOS_MQTT_Example
## Introduction
<p align = "justify"> This is a guide on how to create a working MQTT application with TouchGFX on STM32H750B-DK development board.  
This guide covers all issues I have encountered during the setup of my MQTT project.  
I created this guide due to the lack of any comprehensive tutorials for STM32H750B-DK.  </p>

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

Tutorial on Paho MQTT by eziya:  
https://github.com/eziya/STM32F4_HAL_ETH_MQTT_CLIENT  

Tutorial on LWIP MQTT by M.f.abouHASHEM:  
https://www.youtube.com/watch?v=8P3_R6Xmhb0&t=2s  

# 1. TouchGFX
Start by creating a project for STM32H750B-DK by selecting the template.  
<p align = "center"> <img src = "images/tgfx_project1.PNG" align = "middle" /> <img src = "images/tgfx_project2.PNG" align = "middle"  /> </p>
Create a similar or the same interface.  
<p align = "center"> <img src = "images/tgfx_interface1.PNG" align = "middle" /> </p>
It is important to have the same resource names as here to avoid naming issues later.  
<p align = "center"> <img src = "images/tgfx_resource_names1.PNG" align = "middle" /> </p>
In order to get the zero and one images you have to put the attached zero.png and one.png files into your project images.  
<p align = "center"> <img src = "images/tgfx_images1.PNG" align = "middle" /> </p>
These two images should be hidden at the beginning, but it is not crucial.  
<p align = "center"> <img src = "images/tgfx_visible1.PNG" align = "middle" /> </p>
Create a wildcard for the textArea.  
<p align = "center"> <img src = "images/tgfx_wildcard1.png" align = "middle" /> </p>
To prevent issues it may be necessary to put a couple characters into wildcard typography.  
<p align = "center"> <img src = "images/tgfx_typography1.PNG" align = "middle" /> </p>
Create two interactions exactly like this.  
<p align = "center"> <img src = "images/tgfx_interactions1.png" align = "middle" /> </p>
Generate the code. If the white circle doesn't disappear after the first generation, generate again just to be safe.  

# 2. CubeMX

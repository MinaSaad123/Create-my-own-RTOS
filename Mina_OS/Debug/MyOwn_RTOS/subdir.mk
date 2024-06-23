################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyOwn_RTOS/CortexMx_OS_Porting.c \
../MyOwn_RTOS/MyRTOS_FIFO.c \
../MyOwn_RTOS/Scheduler.c 

OBJS += \
./MyOwn_RTOS/CortexMx_OS_Porting.o \
./MyOwn_RTOS/MyRTOS_FIFO.o \
./MyOwn_RTOS/Scheduler.o 

C_DEPS += \
./MyOwn_RTOS/CortexMx_OS_Porting.d \
./MyOwn_RTOS/MyRTOS_FIFO.d \
./MyOwn_RTOS/Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
MyOwn_RTOS/CortexMx_OS_Porting.o: ../MyOwn_RTOS/CortexMx_OS_Porting.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MyOwn_RTOS/CortexMx_OS_Porting.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MyOwn_RTOS/MyRTOS_FIFO.o: ../MyOwn_RTOS/MyRTOS_FIFO.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MyOwn_RTOS/MyRTOS_FIFO.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MyOwn_RTOS/Scheduler.o: ../MyOwn_RTOS/Scheduler.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MyOwn_RTOS/Scheduler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


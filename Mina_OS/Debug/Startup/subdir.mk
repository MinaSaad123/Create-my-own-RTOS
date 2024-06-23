################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f103c6tx.s 

OBJS += \
./Startup/startup_stm32f103c6tx.o 

S_DEPS += \
./Startup/startup_stm32f103c6tx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_stm32f103c6tx.o: ../Startup/startup_stm32f103c6tx.s
	arm-none-eabi-gcc -gdwarf-2 -mcpu=cortex-m3 -g3 -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -x assembler-with-cpp -MMD -MP -MF"Startup/startup_stm32f103c6tx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"


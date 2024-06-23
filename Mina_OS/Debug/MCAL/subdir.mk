################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/Stm32f103c6_EXTI_Driver.c \
../MCAL/Stm32f103c6_GPIO_Driver.c \
../MCAL/Stm32f103c6_I2C_Driver.c \
../MCAL/Stm32f103c6_RCC_Driver.c \
../MCAL/Stm32f103c6_SPI_Driver.c \
../MCAL/Stm32f103c6_Uart_Driver.c 

OBJS += \
./MCAL/Stm32f103c6_EXTI_Driver.o \
./MCAL/Stm32f103c6_GPIO_Driver.o \
./MCAL/Stm32f103c6_I2C_Driver.o \
./MCAL/Stm32f103c6_RCC_Driver.o \
./MCAL/Stm32f103c6_SPI_Driver.o \
./MCAL/Stm32f103c6_Uart_Driver.o 

C_DEPS += \
./MCAL/Stm32f103c6_EXTI_Driver.d \
./MCAL/Stm32f103c6_GPIO_Driver.d \
./MCAL/Stm32f103c6_I2C_Driver.d \
./MCAL/Stm32f103c6_RCC_Driver.d \
./MCAL/Stm32f103c6_SPI_Driver.d \
./MCAL/Stm32f103c6_Uart_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/Stm32f103c6_EXTI_Driver.o: ../MCAL/Stm32f103c6_EXTI_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_EXTI_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MCAL/Stm32f103c6_GPIO_Driver.o: ../MCAL/Stm32f103c6_GPIO_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_GPIO_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MCAL/Stm32f103c6_I2C_Driver.o: ../MCAL/Stm32f103c6_I2C_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_I2C_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MCAL/Stm32f103c6_RCC_Driver.o: ../MCAL/Stm32f103c6_RCC_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_RCC_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MCAL/Stm32f103c6_SPI_Driver.o: ../MCAL/Stm32f103c6_SPI_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_SPI_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MCAL/Stm32f103c6_Uart_Driver.o: ../MCAL/Stm32f103c6_Uart_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MCAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/HAL/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/CMSIS/INC" -I"C:/Users/HP/STM32CubeIDE/workspace_1.4.0/Mina_OS/MyOwn_RTOS/INC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MCAL/Stm32f103c6_Uart_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


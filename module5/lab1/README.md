# Задание 1
[Текст задания](#текст-задания)  
[Ход работы](#ход-работы)  
[Компиляция](#компиляция)  
[Использование](#использование)  
[Результаты](#результаты)  

## Текст задания
[Назад](#задание-1)  
Написать модуль ядра Hello World для своей версии ядра. Поменять описание модуля, добавить себя как автора и придумать свою лицензию. Результаты выложить на github или др. общедоступный git.  

## Ход работы
[Назад](#задание-1)  
Пример модуля ядра был взят здесь: https://www.thegeekstuff.com/2013/07/write-linux-kernel-module/  
Изменения: в hello.c были изменены лицензия и описание, а также добавлен "Ivan Yashkov" как автор к уже существовавшему автору:  
```bash
MODULE_LICENSE("eltex-v42-2026");
MODULE_AUTHOR("Ivan Yashkov");
MODULE_AUTHOR("Lakshmanan");
MODULE_DESCRIPTION("My first module!");
```

## Использование
[Назад](#задание-1)  
### Запуск
```bash
> make
make -C /lib/modules/7.0.0-28-generic/build M=/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab1 modules
make[1]: Entering directory '/usr/src/linux-headers-7.0.0-28-generic'
make[2]: Entering directory '/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab1'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
  You are using:           gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
  CC [M]  hello.o
  MODPOST Module.symvers
  CC [M]  hello.mod.o
  CC [M]  .module-common.o
  LD [M]  hello.ko
  BTF [M] hello.ko
Skipping BTF generation for hello.ko due to unavailability of vmlinux
make[2]: Leaving directory '/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab1'
make[1]: Leaving directory '/usr/src/linux-headers-7.0.0-28-generic'
```
```bash
> sudo insmod hello.ko
```
```bash
# Проверка успешности подключения
> sudo dmesg | tail -1
[ 1052.878109] Hello world!
> lsmod | grep hello
hello                  12288  0
```
### Отключение
```bash
> sudo rmmod hello
```
```bash
# Проверка правильности отключения
> sudo dmesg | tail -1
[ 1800.862108] Cleaning up module.
> lsmod | grep hello
```

## Результаты
[Назад](#задание-1)  
```bash
> modinfo hello.ko
filename:       /home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab1/hello.ko
description:    My first module!
author:         Lakshmanan
author:         Ivan Yashkov
license:        eltex-v42-2026
srcversion:     7F376075CBD821392D1D144
depends:        
name:           hello
retpoline:      Y
vermagic:       7.0.0-28-generic SMP preempt mod_unload modversions
```

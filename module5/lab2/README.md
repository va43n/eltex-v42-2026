# Задание 2
[Текст задания](#текст-задания)  
[Ход работы](#ход-работы)  
[Использование](#использование)  
[Результаты](#результаты)  

## Текст задания
[Назад](#задание-2)  
Написать модуль ядра для своей версии ядра, который будет обмениваться информацией с userspace через proc. Адаптировать для своей версии ядра (Структура обработчиков). Избавиться от харкода (маг чисел) и изолировать переменные модуля (static).

## Ход работы
[Назад](#задание-2)  
Пример модуля ядра был взят здесь: https://pastebin.com/HhYmGSAM  
Этот пример подходит для версии ядра 4.15. Моя версия ядра - 7.0.0-28-generic, потому, например, struct file_operations уже не используется. Вместо этого использовалась struct proc_ops.  
Также, в соответствии с заданием, каждая функция и переменная были сделаны static. Название proc файла было выделено в PROC_NAME, размер буфера был выделен в MSG_SIZE.  

## Использование
[Назад](#задание-2)  
### Запуск
```bash
> make
make -C /lib/modules/7.0.0-28-generic/build M=/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab2 modules
make[1]: Entering directory '/usr/src/linux-headers-7.0.0-28-generic'
make[2]: Entering directory '/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab2'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
  You are using:           gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
  CC [M]  proc.o
  MODPOST Module.symvers
  CC [M]  proc.mod.o
  CC [M]  .module-common.o
  LD [M]  proc.ko
  BTF [M] proc.ko
Skipping BTF generation for proc.ko due to unavailability of vmlinux
make[2]: Leaving directory '/home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab2'
make[1]: Leaving directory '/usr/src/linux-headers-7.0.0-28-generic'
```
```bash
> sudo insmod proc.ko
```
```bash
# Проверка успешности подключения
> sudo dmesg | tail -1
[ 2001.988918] proc.ko: proc_init - started
> lsmod | grep proc
proc                   16384  0
> cat /proc/hello # Был создан файл proc
```
### Отключение
```bash
> sudo rmmod proc
```
```bash
# Проверка правильности отключения
> sudo dmesg | tail -1
[ 2335.443551] proc.ko: proc_cleanup - cleaning
> lsmod | grep proc # ничего не найдено
> cat /proc/hello
cat: /proc/hello: No such file or directory
```

## Результаты
[Назад](#задание-2)  
```bash
> modinfo proc.ko
filename:       /home/dodiq/Desktop/school3/eltex-v42-2026/module5/lab2/proc.ko
description:    Info exchange through proc
author:         Ivan Yashkov
license:        eltex-v42-2026
srcversion:     0EE2E744321C6C2A8310560
depends:        
name:           proc
retpoline:      Y
vermagic:       7.0.0-28-generic SMP preempt mod_unload modversions
```

Сразу после запуска файл в proc пуст
```bash
> cat /proc/hello # ничего не вывелось
```

Заполнение файла и повторная проверка
```bash
> echo "some text" > /proc/hello
> cat /proc/hello
some text
```

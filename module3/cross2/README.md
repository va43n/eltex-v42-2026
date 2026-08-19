# Кросс-задание №2
[Текст задания](#текст-задания)  
[Компиляция](#компиляция)  
[Запуск](#запуск)  
[Пример запуска](#пример-запуска)  

## Текст задания
[Назад](#кросс-задание-2)  
Управление такси. Есть опорный пункт – исходное приложение. Запускается один раз и привязано к терминалу, имеет CLI.  
В CLI должны быть команды
- create_driver  
- send_task <pid> <task_timer>  
- get_status <pid>  
- get_drivers  

**1. create_driver** создает новый процесс (driver), который ожидает команд от CLI. PID процесса будет использоваться для обращения к этому driver.  
**2. send_task <pid> <task_timer>** создаст задачу для driver номер pid и займет его на task_timer секунд. Если попытаться обратиться к этому driver во время выполнения задания с помощью send_task, то driver должен послать ошибку Busy <task_timer>. По истечению task_timer секунд driver меняет свое состояние на Available.  
**3. get_status <pid>** показывает статус driver с номером pid. Может быть Busy <task_timer> либо Available.  
**4. get_drivers** показывает статусы и pid всех drivers запущенных из этого инстанса CLI.  
Механизм IPC - очереди сообщений POSIX.  
Способ мультиплексирования - epoll.  

## Компиляция
[Назад](#кросс-задание-2)  
```bash
make
```
Создается исполняемый файл taxi.out.

## Запуск
[Назад](#кросс-задание-2)  
```bash
sudo ./taxi.out
```
После запуска программы пользователь может сразу начать вводить команды в соответствии с тем как они описаны в тексте задания. Неправильный ввод не прерывает программу. При нажатии Ctrl+C программа остановится, остановив все процессы и удалив все созданные очереди.  

## Пример запуска
[Назад](#кросс-задание-2)  
### Создание нескольких процессов, назначение некоторым заданий, выход
```bash
> create_driver
create_driver: Creating the driver...
The driver is created with pid (197698)

> create_driver
create_driver: Creating the driver...
The driver is created with pid (197714)

> create_driver
create_driver: Creating the driver...
The driver is created with pid (197726)

> create_driver
create_driver: Creating the driver...
The driver is created with pid (197732)

> create_driver
create_driver: Creating the driver...
The driver is created with pid (197751)

> get_drivers
get_drivers: Getting the drivers...
1. Driver (197698) AVAILABLE
2. Driver (197714) AVAILABLE
3. Driver (197726) AVAILABLE
4. Driver (197732) AVAILABLE
5. Driver (197751) AVAILABLE

> send_task 197726 15        
send_task: Sending task that takes (15) seconds to driver (197726)...
Ok

> send_task 197732 40
send_task: Sending task that takes (40) seconds to driver (197732)...
Ok

> get_drivers
get_drivers: Getting the drivers...
1. Driver (197698) AVAILABLE
2. Driver (197714) AVAILABLE
3. Driver (197726) BUSY 6.593375
4. Driver (197732) BUSY 37.964997
5. Driver (197751) AVAILABLE

(197726) is available again # После прошествия 15 секунд процесс сам сообщил о своем освобождении

> get_status 197732
get_status: Getting status from driver (197732)...
BUSY 22.704303

> get_drivers
get_drivers: Getting the drivers...
1. Driver (197698) AVAILABLE
2. Driver (197714) AVAILABLE
3. Driver (197726) AVAILABLE
4. Driver (197732) BUSY 17.575104
5. Driver (197751) AVAILABLE

> ^Cget_input - interrupted by signal. # Пользователь нажал Ctrl+C
```

### Проверка статусов, попытка назначения нескольких заданий одному процессу
```bash
> create_driver # далее этот процесс не используется, создан просто так
create_driver: Creating the driver...
The driver is created with pid (201049)

> create_driver
create_driver: Creating the driver...
The driver is created with pid (201065)

> send_task 201065 35
send_task: Sending task that takes (35) seconds to driver (201065)...
Ok

> send_task 201065 1000 # попытка выдать одному процессу еще одно задание
send_task: Sending task that takes (1000) seconds to driver (201065)...
BUSY 25.005286

> get_status 201065
get_status: Getting status from driver (201065)...
BUSY 17.804929

> get_drivers
get_drivers: Getting the drivers...
1. Driver (201049) AVAILABLE
2. Driver (201065) BUSY 13.759992

> (201065) is available again # процесс освободился, можно выдать задание снова

send_task 201065 1
send_task: Sending task that takes (1) seconds to driver (201065)...
Ok

> send_task 201065 1(201065) is available again


send_task: Sending task that takes (1) seconds to driver (201065)...
Ok

> (201065) is available again

send_task 201065 1
send_task: Sending task that takes (1) seconds to driver (201065)...
Ok

> (201065) is available again

send_task 201065 1
send_task: Sending task that takes (1) seconds to driver (201065)...
Ok

> (201065) is available again

get_drivers
get_drivers: Getting the drivers...
1. Driver (201049) AVAILABLE
2. Driver (201065) AVAILABLE

> ^Cget_input - interrupted by signal.
```

### Ввод неправильных команд, неправильных значений для команд
```bash
> create_driver # правильное создание процесса
create_driver: Creating the driver...
The driver is created with pid (204205)

> sdfsdfsdfsdg
ERROR: define_command - command not found.
> create_driver 1
ERROR: create_driver - this command doesnt imply any parameters.
> send_task    
ERROR: send_task - there should be 2 parameters in the input:
        drivers pid (uint32_t)
        task timer (uint32_t)
> send_task 123 3
send_task: Sending task that takes (3) seconds to driver (123)...
Driver (123) is not found...
> send_task abc 3
ERROR: parse_str_to_uint - abc in not a uint.
> send_task 204205 qwe
ERROR: parse_str_to_uint - qwe in not a uint.
> get_status 123
get_status: Getting status from driver (123)...
Driver (123) is not found...
> get_status
ERROR: get_status - there should be 1 parameter in the input:
        drivers pid (uint32_t)
> get_status 204205 1 2 3 4 5
ERROR: get_status - there should be 1 parameter in the input:
        drivers pid (uint32_t)
> get_status 204205 # правильный ввод команды get_status
get_status: Getting status from driver (204205)...
AVAILABLE

> get_drivers 123
ERROR: get_drivers - this command doesnt imply any parameters.
> get_drivers # правильный ввод команды get_drivers
get_drivers: Getting the drivers...
1. Driver (204205) AVAILABLE

> ^Cget_input - interrupted by signal.
```


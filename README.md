
# BashLabServer&Client2024

## Данная лабораторная работа включает в себя:
- два Bash-скрипта;
- клиент и сервер, написанные на языке Lua;

## Возможности и описание
- Данный сервер принпмает исключительно три команды: ping (отвечает на неё pong), exit и connect.
- Когда сервер не запущен. а клиент - да, то после команды connect, клиент каждые пять секунд пытается "достучаться" до сервера.
- При вводе неизвестной команды Вы будете оповещены об ошибке.
- Если сервер незапущен, то клиент уведомляет об этом пользователя.
## Требования для запуска
Стандрат языка "Lua" - 5.3
## Инструкция по использованию

- Предварительно

Запустить ```setup.sh``` крипт для установки lua и библиотреки socket
```bash setup.sh```

- Первый вариант
1) Скачиваем проект с ветки master в Zip-файле, распаковываем.
2) Далее запускаем консоль, переходим в директорию, в которой сохранены файлы - ``` cd ~"Путь до папки или директории"```.
3) Устанавливаем модули с помощью ввода команды ```bash setup.sh```.
4) Прописывааем ```bash core_launch.sh``` для запуска клиента и сервера.


- Второй вариант

1) Используем ```git clone https://github.com/qFioofa/BashLabServer2024.git```.
2) Устанавливаем модули с помощью ввода команды ```bash setup.sh```.
3) Прописывааем ```bash core_launch.sh``` для запуска клиента и сервера.

## Авторы
- Дмитрий Воскобойник
- Леонид Высоцкий
- Самир Сибатуллин 
- Антон Ищенко 
- Александр Денисов

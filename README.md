# Simple_DNS-proxy_server
# Задание

Написать DNS прокси-сервер с поддержкой "черного" списка доменных имен.

1.	Для параметров используется конфигурационный файл, считывающийся при запуске сервера;
2.	"Черный" список доменных имен находится в конфигурационном файле;
3.	Адрес вышестоящего сервера также находится в конфигурационном файле;
4.	Сервер принимает запросы DNS-клиентов на стандартном порту;
5.	Если запрос содержит доменное имя, включенное в "черный" список, сервер возвращает клиенту ответ, заданный конфигурационным файлом (варианты: not resolved, адрес в локальной сети, ...).
6.	Если запрос содержит доменное имя, не входящее в "черный" список, сервер перенаправляет запрос вышестоящему серверу, дожидается ответа и возвращает его клиенту.
7. Язык разработки: С
Использование готовых библиотек: без ограничений.
Использованный чужой код должен быть помечен соответсвующими копирайтами, нарушать авторские права запрещено.
Остальные условия/допущения, не затронутые в тестовом задании – по собственному усмотрению.

# Планы по совершенствованию
1. Внедрить и организовать чтение конфиг файла в JSON формате.
2. Блокировка домена должна блокировать и субдомены.
3. Внедрение кэширования адресов, учитывая ttl (для хранения предварительно использовать map implementation).

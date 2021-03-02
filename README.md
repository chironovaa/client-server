# Описание
Клиент посылает введенное пользователем на консоль сообщение, сервер при получении данного сообщения обрабатывает его: находит в данном сообщении все числа, вычисляет их сумму и отправляет в ответ результат. Если в сообщении от клиента нет ни одного числа, то сервер должен просто отправить в ответ клиенту полученное сообщение. Можно считать, что для хранения каждого из чисел в сообщении хватает размерности встроенных типов.



Допустим, клиент отправил сообщение

**20 apples, 30 bananas, 15 peaches and 1 watermelon**

в ответ он должен получить сообщение

**1 15 20 30**

**66**

# Сборка и запуск
## Сервер
* Сборка: <br/>
$ g++ server.cpp -o server <br/>
* Запуск: <br/>
$ ./server <br/>

## Клиент
* Сборка: <br/>
$ g++ client.cpp -o client <br/>
* Запуск: <br/>
$ ./client <br/> <br/>
Для выбора порта (TCP или UDP) необходимо ввести<br/>
1 - если предполагается работа пр использовании TCP,<br/>
2 - для UDP. <br/>
* Завершение работы  <br/>
$ exit
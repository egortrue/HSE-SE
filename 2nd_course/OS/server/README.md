# Веб-сервер с доступом к файловой системе

## Задача
Необходимо установить, настроить и запустить файловый сервер Samba с доступ через веб-интерфейс Apache.

## Подготовка
Настройка и проверка веб-сервера проводилась в локальной сети:
- Машина на Linux дистрибутиве Debian (хост: 192.168.1.100)
- Машина на Windows 10 (пользователь: 192.168.1.101)

Обновление системы:<br>
`sudo apt update -y && sudo apt full-upgrade -y`

Установка веб-сервера Apache и сетевой службы Samba:<br>
`sudo apt install apache2 samba -y`

## Проверка установки
Запустим службу, отвечающую за работу веб-сервера:<br>
`sudo service apache2 start`

В поисковой строке веб-браузера введём:<br>
`http://localhost`

___Если всё сделано правильно, мы увидим стандартную страницу:___<br>
<img src="img/default_page.png">

## Настройка хоста
### Apache
Создадим рабочую директорию нашего сервера:<br>
`sudo mkdir /server`

Создадим новый файл конфигурации для подключения:<br>
`sudo touch /etc/apache2/sites-available/server.conf`

Заполним его этими данными:<br>
```html
<VirtualHost *:80>

	ServerAdmin webmaster@localhost
	DocumentRoot /server

	<Directory /server>
		Require all granted
		Options Indexes
		AllowOverride None
	</Directory>

</VirtualHost>
```

Отключим стандартный сайт и подключим новый:<br>
`sudo a2dissite 000-default.conf`<br>
`sudo a2ensite server.conf`

Запустим службу Apache:<br>
`sudo service apache2 start`

___Проверим работу:___<br>
В поисковой строке веб-браузера введём: `http://localhost` <br>
Мы увидим страницу, отображающую пустую директорию:<br>
<img src="img/empty_dir_page.png">

### Samba
Создадим гостевую директорию, доступную всем:<br>
`sudo mkdir /server/guest`<br>
`sudo chown -R nobody:nogroup /server/guest/`<br>
`sudo chmod -R 777 /server/guest/`

Откроем конфигурационный файл Samba:<br>
`vi /etc/samba/smb.conf`

В самый конец добавим описание гостевой директории:<br>
```
[guest]
	path = /server/guest
	force user = nobody
	browsable = yes
	writeable = yes
	guest ok = yes
```

Запустим службу Samba:<br>
`sudo service smbd start`

___На веб-странице появиться пустая папка:___<br>
<img src="img/new_folder_page.png">

## Использование

___Для подключения к гостевой папке используем проводник Windows.___<br>
В адресную строку введём следующую строку: `\\192.168.1.100\guest`<br>
- 192.168.1.100 - адрес сервера(хоста)
- guest - имя директории, к которой мы хотим подключиться

___Перед нами появиться пустая директория:___<br>
<img src="img/win_connect.png">

___Создадим пустой файл и новую поддиректорию:___<br>
<img src="img/hello_folder.png">

___На нашей веб-странице и в файловой системе Linux также отображаются новые файлы:___<br>
<img src="img/hello_page.png">
<img src="img/hello_linux.png">
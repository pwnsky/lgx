# lgx web server



## Introduction

The lgx server is a high-performance web server, written by c++ language.



## Compile lgx server

You should make sure you have install compile environment as follows:
### Debian, Ubuntu ....

```
sudo apt install g++
sudo apt install make
```
### Get open source
Git clone src to compile

```sh
git clone https://github.com/I0gan/lgx.git
cd lgx
```
Now you can compile with make
### compile
```
make
```

### run

```
chmod +x start && ./start
```

 Input ip address or domain you can access web to see as follows:

```
Welcome to lgx server!
```



## Configure File

The default configure file at bin/etc/config.json, 

If you wanna change default page, you need to change the configure file, default configure file content as follows:

```
{
    "port" : 80,
    "number_of_thread": 8,
    "log_path" : "data/log",
    "root_path" : "www",
    "web_page" : "index.html",
    "web_404_page" : "404.html",
    "log_path" : "./lgx.log",
    "firewall" : {
    }
}
```

port:  server listen port

number_of_thread: your server will run with how many threads, default with 4

log_path: the path of print server log

root_path: the root path of access web

web_page: default access web page file

web_404_page: not found web page file

log_path:  the path of log file

firewall: forbid specific client to access server, for example:

```
"firewall" : {
	"1", "10.1.1.1",
	"2", "101.2.4.5"
 }
```

It means to forbid client with ip address as  "10.1.1.1" or "101.2.4.5" to access server 



If you configured the file,you can restart server directly, not to recomplie the server again!



## dev log

2020-04-...   : Create a server multi-threads and epoll event framework

2020-08-26  : Create a github repositories

2020-09-21 : Add firewall to forbid specific  ip address to access server

2020-09-25: Add logger module

2020-09-28: Make lgx server security quit when press ctrl + c.

2020-09-30: Fix memory leak when ctrl + c and add a cypto module to decode url code



## About author

Name: `I0gan` 

QQ: `418894113`

Email: `l418894113@gmail.com`



## feedback

If you have found any vulnerability in lgx source code, welcome to feedback by sending email to  `l418894113@gmail.com` 

### Example

[i0gan.cn](http://i0gan.cn)
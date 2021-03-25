 # HOW TO INSTALL LGX SERVER



You should make sure you have install compile environment as follows
### Debian, Ubuntu ....

```
sudo apt install g++
sudo apt install gcc
sudo apt install make
```
### Get open source
Git clone src to compile

```sh
git clone https://github.com/I0gan/lgx.git
cd lgx
```
Now you can compile with make

```
make -j 8
make install
```

### Run

```
sudo lgx -r
```

 Input ip address or domain you can access web to see as follows:

```
Welcome to use lgx server!
```

if you wnnae unstall lgx

```
make uninstall
```

or

```
sudo rm /etc/lgx
sudo rm /usr/bin/lgx
```

Then you can uninstall lgx server



## Configure File

The default configure file at bin/etc/config.json, 

If you wanna change default page, you need to change the configure file, default configure file content as follows:

```
{
    "port" : 80,
    "number_of_thread": 4,
    "root_path" : "/var/www",
    "web_page" : "index.html",
    "web_404_page" : "404.html",
    "log_path" : "/var/log/lgx.log",
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
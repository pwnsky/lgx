# lgx web server

## Complie lgx server

You should make sure you have install complie env as follows:
### Debian, Ubuntu, Kali ....

```
sudo apt install g++
sudo apt install make

```
### Get open source
First, you should git clone src to complie

```sh
git clone https://github.com/I0gan/lgx.git
cd lgx
```
Now you can comple with make
### comple
```
make
```

### run

```
chmod +x start && ./start
```

 Input ip or domain you can see

```
Welcome to lgx server!
```

If you wanna change default page, you need to change the configure file(bin/etc/config.json), default configure file data as follows:

```
{
    "port" : 80,
    "number_of_thread": 8,
    "log_path" : "data/log",
    "root_path" : "www",
    "web_page" : "index.html",
    "web_404_page" : "404.html"
}
```

You can restart server directly, not only recomplie the server again!



Author: I0gan (徐绿国)

QQ: 418894113

[blog_1](https://www.cnblogs.com/lyxf/)

[blog_2](https://i0gan.github.io)
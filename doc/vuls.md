# LGX CVE LIST



### VULN-2020-1003 

#### Intro

Frequent disconnection, multi thread competition, log module write to memory at the same time, resulting in memory error, server crash

```
Open file [www//cssfont_1429596_nzgqgvnmkjb.css] failed!
recv 0 DISCONNECTING
Open file [www//js/gitalk.min.css] failed!
terminate called after throwing an instance of 'std::bad_alloc'
  what():  std::bad_alloc
[1]    37779 abort      sudo ./lgx -r


...
Open file [www/js/photoswipe-ui-default.min.js] failed!
terminate called after throwing an instance of 'std::length_error'
  what():  basic_string::_M_create
  
 
...
Open file [www/cssfont_1429596_nzgqgvnmkjb.css] failed!
[1]    44529 segmentation fault  sudo ./lgx -r
```

#### fixed

Synchronization log module



## VULN-2020-1017

### Intro

Malicious forgery of HTTP protocol leads to the Server HTTP protocol parsing error, causing the server to crash. The cause of the crash is being found

Victim website: http://i0gan.cn 

Attacker address: TaiWan

```log
connected: ip: 47.101.34.11448168
2020-10-17 02:39:47
request: get
2020-10-17 03:21:52
connected: ip: 122.117.46.24142784
2020-10-17 03:21:52
ERROR_PARSING_URL:
2020-10-17 03:24:09
connected: ip: 59.126.74.10433939
2020-10-17 03:24:09
ERROR_PARSING_URL:
2020-10-17 03:24:09
ERROR_PARSING_URL:
2020-10-17 03:24:09
ERROR_PARSING_URL:
2020-10-17 03:24:09
ERROR_PARSING_URL:
......
......
request:
2020-10-17 03:25:08
ERROR_PARSING_URL:
2020-10-17 03:25:08
ERROR_PARSING_URL:
2020-10-17 03:25:08
ERROR_PARSING_URL:
2020-10-17 03:25:08
ERROR_PARSING_URL:
2020-10-17 03:25:08
ERROR_PARSING_URL:
```

This attack just take a one minute then make the lgx server break down.



## VULN-2020-1125

### Intro

In the AXB 2020 CTF competition, the framework of the server is used to make a pwn challenge. However, the contestants give an unexpected solution. Directory traversal vulnerability can cause any sensitive file to be leaked. This vulnerability is also the vulnerability of lgx server at present.

poc

```
┌[logan☮arch]-(~)
└> curl -i "http://i0gan.cn/..%2f..%2f..%2f../etc/passwd"
HTTP/1.1 200 OK
Content-Length: 1902
Access-Control-Allow-Origin: *
Connection: keep-alive
Content-Type: obj
Keep-Alive: timeout=4
Proxy-Connection: keep-alive
Server: lgx-linux1.6

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin

```

### patch

Add a check_file_name function to check traversal vulnerability.

```c++
bool lgx::net::http::check_file_path(const std::string &file_name) {
    char last_ch = 0;
    for (auto ch : file_name) {
        if(ch == '.')
            if(last_ch == '.')
                return false;
    }
    return true;
}
```


## VULN-2021-04-04

### Intro

If Content-Length value is not integer in http header, that server will crashed!

poc:

```
POST /?request=login? HTTP/1.1
Connection: Keep-Alive
Content-Length: .
```



The vulnerable code in http.cc: 185

```
if(map_header_info_.find("content-length") != map_header_info_.end()) {
	content_length_ = std::stoi(map_header_info_["content-length"]);
 }
```



Patch

```c++

if(map_header_info_.find("content-length") != map_header_info_.end()) {
  	try {
       content_length_ = std::stoi(map_header_info_["content-length"]);
   	} catch (std::invalid_argument) {
  		logger() << log_dbg("content-length is invalid");
   		recv_error_ = true;
    	response_error((int)HttpResponseCode::BAD_REQUEST, "Bad Request");
    }
}
```

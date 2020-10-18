# LGX CVE LIST



### LGX-CVE-2020-1003 

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



## LGX-CVE-2020-1017

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






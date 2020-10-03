# LGX CVE LIST



### LGX-CVE-20201003-0001 

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






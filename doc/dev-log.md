# Dev Log



2020-04-...   : Create a server multi-threads and epoll event framework

2020-08-26  : Create a github repositories

2020-09-21 : Add firewall to forbid specific  ip address to access server

2020-09-25: Add logger module

2020-09-28: Make lgx server security quit when press ctrl + c.

2020-09-30: Fix memory leak when ctrl + c and add a cypto module to decode url code

2020-10-03: Fix server breakdown when access web frequently (BUG: The log module is not synchronized)

2020-10-16: Set logger module  thread blocking avoids a lot of CPU usage

2020-10-16: Add a sqlite database module

2020-10-18: Reinforced firewall function (website has been attacked, detailed in CVE-2020-1018 )

2020-11-10: Replace type of http recv buffer  to lgx::util::vessel, so it's faster than std::string when recv data

2020-11-27: Fix directory traversal vulnerability

2021-01-01: Remove not important moudle 
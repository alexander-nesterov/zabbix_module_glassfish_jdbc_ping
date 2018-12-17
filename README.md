## Loadable module of Zabbix for ping connection pool

Note: Only *nix

Official Documentation: 

https://www.zabbix.com/documentation/3.4/manual/config/items/loadablemodules

# zabbix_module_glassfish_jdbc_ping
zabbix_module_glassfish_jdbc_ping

1) Download source code of Zabbix and then run the ```./configure``` command (without arguments) in the root of Zabbix source tree

2) Build this module
```bash
alexander@dev:~$ make
```

3) Set load module in zabbix_agentd.conf
```bash
alexander@dev:~$ cat /etc/zabbix/zabbix_agentd.conf | grep ^LoadModule
LoadModulePath=${libdir}/modules
LoadModule=glassfish-jdbc.so
```

4) Restart zabbix-agent

5) You need to add teamplate

to check log:
```bash
alexander@dev:~$ grep glassfish-jdbc.so zabbix_agentd.log
 28132:20181212:113155.127 Module: glassfish-jdbc.so - build with: 3.4.12; libcurl: 7.61.0; pcre: 8.42 (src/module.c:63)
 28132:20181212:113155.127 loaded modules: glassfish-jdbc.so
 ```

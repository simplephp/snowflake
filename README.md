->**Twitter的分布式自增ID算法Snowflake PHP 扩展**


----------

> 服务器要求

 1. linux 64 位，因为涉及到位移问题
 2. php  版本 5.5 以上
 
----------


> 安装
> 

 1. phpize
 2. ./configure
 3. make
 4. make test
 5. make install
 6. php.ini 加入配置 extension= snowflake.so
 7. php.ini 加入配置 work_id, snowflake.work_id = 1

> 写到最后
	> 

 snowflake 算法很简单，高效的生成不重复的 id 还是扩展好，不在个位看官面前班门弄斧，学海无涯，so 写代码去吧

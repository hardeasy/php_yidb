# php_yidb
yidb存储器的PHP扩展客户端

# 安装和配置
-----
~~~
phpize
./configure [php-config路径]
make && make install
~~~

在php.ini中加入
~~~
extension=yidb.so
~~~

# 一个简单的列子
~~~
$yidb = new Yidb();
if($yidb->connect("127.0.0.1",2048) == false){
	echo 'connect error';exit;
}
$yidb->set("key1","ddsad",time()+100);
$value = $yidb->get("key1");
$exptime = $yidb->exptime("key1");
$yidb->delete("key1");
~~~
 

 
~~~
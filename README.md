# php_yidb
yidb�洢����PHP��չ�ͻ���

# ��װ������
-----
~~~
phpize
./configure [php-config·��]
make && make install
~~~

��php.ini�м���
~~~
extension=yidb.so
~~~

# һ���򵥵�����
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
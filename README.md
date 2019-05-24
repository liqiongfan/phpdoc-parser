# Xannotation PHP的数据库模型扩展

An extension for PHP DB model operation. written in C code.

Author：Josin <774542602@qq.com> | <a href="https://www.supjos.cn/archives/46.html#directory0065624643873947846">博客</a>

**Xannotation是一个PHP的数据库操作扩展，基于PDO，完全采用C语言开发，性能相比纯原生PHP数据库组件性能更高**
## 数据库适配器 ##

__Xan__中适配器类：__Xan\Db\Adapter__，提供构造函数完成数据库的连接操作，函数参数等于 PDO库的参数，如：

```php
$adapter = new \Xan\Db\Adapter(
            "mysql:host=localhost;dbname=test", "root", "333", 
            [ 
                \PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES UTF8MB4'
            ]);
```

## 模型 ##

__Xan__ 中模型必须继承于 __Xan\Db\Model__ 抽象类，并且实现方法： __tableName()__：

```php
namesapce app\models;

class User extends \Xan\Db\Model 
{
    public function tableName() 
    {
        return 'tb_user';
    }
	
    // 如果模型中存在 init 方法，引擎会自动调用此方法完成自定义的初始化工作
    public function init() 
    {
        // 初始化工作
    }
}
```
使用模型之前必须使用  __Xan\Di__ 来完成适配器的注入 才可以使用模型，需要切换模型的数据库操作链接，可以使用模型的方法： __setAdapter($diKey)__ 如：

```php
\Xan\Di::set('master', function(){
    return new Adapter("mysql:xxxxx", "xxx", "xxx");
});

\Xan\Di::set('slave', function(){
    return new Adapter("mysql:xxxxx", "xxx", "xxx");
});
$user = new User();
$user->setAdapter('slave');
```

## SESSION ##

__Xan__ 的 __SESSION__ 组件包装了系统的 `$_SESSION` 。 如：

```php
$session = new Xan\Session();

// 下面两种都是设置值
$session->set('hello.xan', 'world');
$session['hello.xan'] = 'world';

// 设置的结果就是：
$session = [
    'hello' => [
        'xan' => 'world'
    ]
];

// 下面都是获取值
$session->get('hello.xan');
$session['hello.xan'];
$session['hello']['xan'];
```

__Xan__ 中SESSION组件可以采用多维数组的方式访问元素内容，但不可以使用多维数组的方式进行修改，因此为了保持一致，推荐采用内置的“.”语言进行设置与访问。

多维的SESSION采用“ __.__ ”进行获取对应的数据。

## $_GET|$_POST|$_SERVER ##

__Xan\Request__ 封装了系统的 $_GET|$_POST|$_SERVER 超全局变量


__`$_GET`__：

```php
$request = new Xan\Request();

$data = $reqeust->get('data');

// 也可以使用过滤器
$data = $request->get('data', 'strtoupper');
$data = $request->get('data', function($data){
    return strtoupper($data);
});

// 也可以指定默认值
$data = $request->get('data', 'strtoupper', 'Xan');

```

__`$_POST`__：

```php
$request = new Xan\Request();

$data = $reqeust->getPost('data');

// 也可以使用过滤器
$data = $request->getPost('data', 'strtoupper');

// 也可以指定默认值
$data = $request->getPost('data', 'strtoupper', 'Xan');
```

__`$_SERVER`__：

```php
$request = new Xan\Request();

$data = $reqeust->getServer('data');

// 也可以使用过滤器
$data = $request->getServer('data', 'strtoupper');

// 也可以指定默认值
$data = $request->getServer('data', 'strtoupper', 'Xan');
```

## 支持环境 ##

```>= PHP7.0```
```Linux/Unix(Mac OS X)```

```Windows``` version need to be changed the ```config.w32``` file and compile, wish some one do the job instead of me.

## 安装 ##

**1: CMake 安装**

版本 3.10+， 手工修改自带的 CMakeLists.txt 文件的 php 路径即可

``` cmake .```

**2: php-configure**

```shell
/usr/path_to_php/phpize

./configure --with-php-config=/usr/path_to_php/php-config

make -j && sudo make install -j

echo "xannotation.so" >> /usr/path_to_php/php.ini
```








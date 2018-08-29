# Xan 高性能的PHP扩展框架

An extension for PHP document parsing. written in C code.

Author：Josin <774542602@qq.com> | <a href="https://www.supjos.cn/archives/46.html#directory0065624643873947846">博客</a>

**Xan是一款高性能的易于使用上手的PHP开发框架，颠覆了传统的配置模式，利用注解进行配置，便于优化代码、清晰逻辑；并且基于C语言开发，性能极高。**

#### 框架特点：

​    **1、PSR自动加载**

​    **2、注解特性**

​    **3、AOP编程**

​    **4、IOC反转**

​    **5、高效视图引擎**

​    **6、高效API接口XML与JSON**

## 何为注解?  ##

在 __Java__ 中到处可见注解的身影，如 __`@Null`__ **or** __`@NotNull`__ 等，其实注解就是一种修饰 变量、类、方法的对象，如下，注解也可以携带参数：

```java
package cn.supjos;

class A
{
    @NotNull(age)
    @Default(age=22, type = "Car")
    @Set("www.supjos.cn", email="774542602@qq.com")
    void test()
    {
    }
}
```

在 __PHP__ 中由于没有语法糖的支持，只能采用注释文档的形式组成注解，如下：

```php
<?php
    
class A
{
    /**
     * @NotNull
     * @Default(age=22, sex="male")
     * @Set("www.supjos.cn", email="774542602@qq.com")
     */
    function test()
    {
    }
}
```

#### 特性说明

**1、** 在 **Xan** 中，控制器中存在 **init()** 方法的情况下，引擎会自动从顶级父类开始执行 **init()** 到本类，不需要用户手动使用 **parent::init()** 进行调用。

**2、** 全局变量：**Xan\Apps::$app** 表示的是全局的 **Xan\Apps** 类的对象；IOC与容器基于此变量。

**3、** **XML** 数据，如果键名为数字也就是索引数组的情况下，系统会自动创建 **key** 作为元素标签名。系统会自动创建一个 **root** 键名作为根元素的键名。

## AOP切面思想 ##

__切面思想__：将那些与业务无关，却为业务模块所共同调用的逻辑或责任封装起来，便于减少系统的重复代码，降低 模块间的耦合度，并有利于未来的可操作性和可维护性；AOP是一个横向的关系组合，也就是将应用中的公共服务进行分离。

__重要概念：__

1、 __连接点__ ，程序的一个执行点，__Xan__ 中仅支持方法级别的连接点

2、 __切入点__ ，捕获连接点的结构，一般可以通过正则匹配或者表达式，目前  __Xan__ 并不支持

3、 __通知__ ，切入点的业务逻辑代码，目前 __Xan__ 支持(@before, @after, @success, @failure)四种通知

4、 __切面__ ，定义一个切面类，使用注解： __`@Aspect`__ 来定义一个切面

5、 __引入__ ，通过引入附加的属性等，达到修改对象或者类结构的目的，目前  __Xan__ 可以通过注解 __AttrAnnotation__ 与  __ConstAnnotation__  来完成此功能

**简单示例：**

```php
/**
 * 定义一个切面类
 *
 * @Aspect
 */
class Basic
{
    function before()
    {
        echo 'before';
    }
    
    function after($data)
    {
    	echo 'after =>' . $data;    
    }
    
    function success()
    {
        echo 'success';
    }
 
    /**
     * 定义通知
     * 通知支持传递参数到指定的切入点，如需指定参数，那么必须使用
     * (value="xxx.xxx", parameters="xxx||xxx||xxx")的格式来指定通知
     * 通知的 value 表示切入点，分别是类名 + "." + 方法名
     * 通知的 parameters 表示参数，多个参数使用 "||"分割
     * 可以完全省略 value 与 parameters 两个参数，直接填入 “类与方法”
     * 如下：
     * 
     * @before("Base.before")
     * @after(value="Base.after", parameters="Xan Extension")
     * @success("Base.success")
     */
    function test()
    {
        echo 'I\'m the test method for Aspect programming.';
        // 返回true，那么AOP执行 success通知，否则执行 failure 通知
        return true;
    }    
}

// 实例化代理，代理返回的对象可以当做常规对象(new生成的对象一样)使用
$base = Xan\Aop\Proxy::instance(Base::class);

// 调用方法
$base->test();
```

**注意：在Xan中通知是可以无止境的嵌套运行的，也就是一个通知可以嵌套运行另一个通知，每一个通知都有一个通知链，不可以进行闭合通知的，如果闭合通知，则会提示一个 “ __Fatal Error：Recursive calling: xx::xx__ ” 的致命错误**

## WEB示例 ##

```php
    
    $loader = new Xan\Loader();
    $loader->setMap('@app', __DIR__);

    $app = new Xan\App();
    $app->bootstrap()->run();
```

前两行 __自动加载功能__ , 后两行 __框架运行__。其中 App构造函数支持全局参数设置。

__控制器__：

```php
namespace app\modules\index\controllers;

class Index
{

    public function init() {

    }

    /**
     * 默认方法
     */
    public function indexAction() {

    }
}
```

在 __Xan__ 中，方法以 __Action__ 结尾，控制器首字母大写，并且不携带后缀: __Controller__。
如果进行 __JSON__ 与 __XML__ 接口开发，请使用 __AOP__ 注解。

## AOP 注解 ##

```php
namespace app\modules\index\controllers;

/**
 * @Aspect
 */
class Index
{

    /**
     * @api(type=JSON,charset=GBK)
     */
    public function indexAction()
    {
        return [
            'Xan' => 'Fast & Easy & Stack.',
            'Version' => 'v0.1.9'
        ];
    }
}
```

在 __Xan__ 中，只有框架调用的方法、路由才会解析注解完成 __AOP__ 特性的使用。  __Xan__ 注解分为 __类注解__ 与 __方法注解__ ，注解类都必须实现 __Xan\Type\Annotation\Annotation__ 接口，接口仅包含有一个方法 __input__ 方法，如下：

```php
namespace app\annotations;

class TagAnnotation implements \Xan\Type\Annotation\Annotation
{
    public function input($objectOrName, $annotations) {
        
    }
}
```

当 __Xan__ 解析到一个注解的时候会调用相应的注解类的 __input__ 方法完成注解使用，其中 第一个参数 __$objectOrName__ 在类注解的情况下为 当前的类名称，在方法注解下为当前的类对象。如：

```php

namespace app\moduels\index\controllers;

/**
 * @Aspect
 * @app\annotations\TagAnnotation(version="v1.2.1", aop="Xan")
 */
class Index
{
    /**
     * @LabelAnnotation(v = "V", t = "T")
     */
    public function indexAction() {
        
    }
}


namespace app\annotations;

class TagAnnotation implements \Xan\Type\Annotation\Annotation
{
    public function input($objectOrName, $annotations) {
        
        // $objectOrName
        echo $objectOrName;    // output: app\modules\index\controllers\Index;

        // $annotations:
        print_r($annotations); // output: [ 'version' => 'v1.2.1', 'aop' => 'Xan' ]
    }
}

namespace app\annotations;

class LabelAnnotation implements \Xan\Type\Annotation\Annotation
{
    public function input($objectOrName, $annotations) {
        
        // $objectOrName
        var_dump($objectOrName); // output: new app\modules\index\controllers\Index();

        // $annotations:
        print_r($annotations);   // output: [ 'version' => 'v1.2.1', 'aop' => 'Xan' ]
    }
}
```

## SESSION ##

__Xan__ 的 __SESSION__ 组件包装了系统的 `$_SESSION` 。 如：

```php
$session = new Xan\Session();

// 设置值
$session->set('hello.xan', 'world');
 <==>
$session['hello.xan'] = 'world';

// 获取值
$session->get('hello.xan');
 <==>
$session['hello.xan'];

```

## $_GET|$_POST|$_SERVER ##

__Xan\Request__ 封装了系统的 $_GET|$_POST|$_SERVER 超全局变量


__$_GET：__

```php
$request = new Xan\Request();

$data = $reqeust->get('data');

// 也可以使用过滤器
$data = $request->get('data', 'strtoupper');
 <==>
$data = $request->get('data', function($data){
    return strtoupper($data);
});

// 也可以指定默认值
$data = $request->get('data', 'strtoupper', 'Xan');

```

__$_POST：__

```php
$request = new Xan\Request();

$data = $reqeust->getPost('data');

// 也可以使用过滤器
$data = $request->getPost('data', 'strtoupper');

// 也可以指定默认值
$data = $request->getPost('data', 'strtoupper', 'Xan');
```

__$_SERVER：__

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

## 部分APIs ##

#### 注解Xan

**Xan 类构造函数**

```php
final class Xan
{
    function __construct(){};
}
```

**类注解： Xan::getClassDocComment($classNameOrObject)**

```php
/**
 * 返回类的注解字符串
 * 
 * @param $classNameOrObject 类名称或者对象
 */
function getClassDocComment($classNameOrObject)
{
    return $this;
}
```


**方法注解：Xan::getMethodDocComment($classNameOrObject, $methodName)**

```php
/**
 * 获取方法的注解信息，返回注解字符串
 *
 * @param $classNameOrObject 类名称或者对象
 * @param $methodName        方法名
 */
function getMethodDocComment($classNameOrObject, $methodName)
{
}
```

**解析注解：Xan::parseDocComment($docComments)**

```php
/**
 * 返回一个包含注解信息的对象，[body] 包含的是说明信息，其余的注解每一个注解是一个数组
 * 
 * @param $docComments 需要解析的注解信息
 */
function parseDocComment($docComments)
{
}
```

**获取数组形式的注解：Xan::getParseResult($docComments)**

```php
/**
 * 同 parseDocComent() 结果,区别在于返回的数组还是对象
 */
function getParseResult($docComments)
{
}
```

**获取类的所有方法的注解信息：parseAllMethodsDocComment($objectOrName)**

```php
/**
 * Function: `parseAllMethodsDocComment` used to parsing all methods which in class{$objectOrName}
 * return the object contains the result, the result format like this:
 *
 * The result contains the `num` key means the methods number
 * the `annotations` key contains the result array, each key-value pair contains the key(The function name) & the value(array)
 * each value contains three key-value pairs:
 * `annotations`: The annotations the function contains
 * `num`        : The number of the annotations
 * `body`       : The body of the annotations
 *
 * @param $objectOrName  The class name or object which you want to obtain the annotations
 * @return this          The object contains the value
 */
function parseAllMethodsDocComment($objectOrName) : this
{
}
```

#### 自动加载 Xan\Loader类

**构造函数Xan\Loader::__construct()**

```php
namespace Xan;
class Loader
{
    function __construct();
}
```

**设置别名：Xan\Loader::setMap($aliasName, $path)**

```php
namespace Xan;
class Loader
{
    function setMap($aliasName, $path);
}
```

**启动自动加载：Xan\Loader::start($prepend = false)**

```php
namespace Xan;
class Loader
{
    function autoLoad($prepend = false);
}
```

#### 类配置工具： Xan\Type\ConfigClass

**构造函数 Xan\Type\ConfigClass::__construct()**

```php
namespace Xan\Type;
class ConfigClass
{
    function __construct();
}
```

**设置类常量 Xan\Type\ConfigClass::setConstant($className, $key, $value)**

```php
namespace Xan\Type;
class ConfigClass
{
    function setConstant($className, $key, $value);
}
```

**设置属性 Xan\Type\ConfigClass::setAttribute($object, $key, $value, $flags = Xan\Type\ConfigClass::PUBLIC)**

```php
namespace Xan\Type;
class ConfigClass
{
    function setAttribute($object, $key, $value, $flags = Xan\Type\ConfigClass::PUBLIC);
}
```

**设置属性可见性 Xan\Type\ConfigClass::setAccessFlags($className, $key, $flags = Xan\Type\ConfigClass::PUBLIC)**

```php
namespace Xan\Type;
class ConfigClass
{
    function setAccessFlags($className, $key, $flags = Xan\Type\ConfigClass::PUBLIC)
}
```







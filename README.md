# Xan 高性能的PHP注解扩展

An extension for PHP document parsing. written in C code.

Author：Josin <774542602@qq.com> | <a href="https://www.supjos.cn/archives/46.html#directory0065624643873947846">博客</a>

## 何为注解?  ##

在 __Java__ 中到处可见注解的身影，如 __`@Null`__ **or** __`@NotNull`__ 等，其实注解就是一种修饰 变量、类、方法的对象，如下，注解也可以携带参数：

```php
class A
{
    /**
     * @NotNull(age)
     * @Default(age=22, type = "Car")
     * @Set("www.supjos.cn", email="774542602@qq.com")
     */
    function test()
    {
    }
}
```

##  注解初始化类属性

当前的版本中，可以使用注解来进行类常量与属性的添加，注解类属性依次来完成功能，目前注解类已经实现 __`ConstAnnotation`__ 与 __`AttrAnnotation`__ ，用来初始化类的常量与普通的属性，本功能需要使用内置的自动加载引擎才可以，如下：

__`index.php`__ 文件代码如下：

```php
// 使用 Xan\Loader 加载引擎完成类的自动加载功能
$loader = new \Xan\Loader();

// 当前的 app 命名空间是当前文件夹
// 本方法可以多次调用来生成多个命名空间，优先级的顺序越来越低
$loader->setMap('@app', __DIR__);

$loader->autoLoad();


// 开始使用注解功能
$base = new app\Base();

echo app\Base::TOOL . '<=>' . app\Base::URL;
echo $base->name . '<=>' . $base->version . PHP_EOL;
```

__`app\Base`__ 类的代码如下(文件命名为：__`Base.php`__)：

```php
namespace app;

/**
 * Class Base
 *
 * @\Xan\Type\Annotation\AttrAnnotation(name="Xannotation", version="v1.0.2")
 * @\Xan\Type\Annotation\ConstAnnotation(TOOL="C/C++", URL="https://www.supjos.cn")
 */
class Base
{
    const GAP = 1;
}
```

工程的文件路径示意：

```php
-dir
  +++index.php
  +++Base.php
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

## 使用示例 ##

**测试类 Hello**

```php
/**
 * Class Hello * hello * world
 *
 * This is the first time to do the job for the annotation
 * License: LGPL-v3
 *
 ****This is the toast which need to be test the annotation.
 *
 * @type(name = "Hello", age = "world",  good=fruit, info=www.supjos.cn)
 * @version(value="v2.1.23",  version="2323.23", name = "game")
 */
class Hello
{
    /**
     * 商品列表视图
     *
     * @Route("hello/world")
     * @Method("vs", "value se"="hello", attr="private" )
     */
    public function world()
    {

    }

    /**
     * 获取方法的注解信息，返回一个对象
     *
     * @NotEmpty("classNameOrObject", "methodName")
     */
    function getMethodDocComment($classNameOrObject, $methodName)
    {
    }

    /**
     * 返回类的注解信息
     *
     * @NotEmpty("classNameOrObject")
     */
    function getClassDocComment($classNameOrObject)
    {
    }
}
```

**获取注解信息示例：**

```php
$xan = new Xan();
```
**获取类的注解信息:**

```php
$docComments = $xan->getClassDocComment(Hello::class);
print_r($xan->getParseResult($docComments));
```

**输出如下**

```php
Array
(
    [annotations] => Array
        (
            [type] => Array
                (
                    [name] => Hello
                    [age] => world
                    [good] => fruit
                    [info] => www.supjos.cn
                )

            [version] => Array
                (
                    [value] => v2.1.23
                    [version] => 2323.23
                    [name] => game
                )

        )

    [num] => 2
    [body] => Class Hello * hello * world

 This is the first time to do the job for the annotation
 License: LGPL-v3

This is the toast which need to be test the annotation.
)
```


**获取类方法的注解信息：**

```php
$docComments = $xan->getMethodDocComment(Hello::class, "world");
print_r($xan->getParseResult($docComments));
```

**输出如下**

```php
Array
(
    [annotations] => Array
        (
            [Route] => Array
                (
                    [1] => hello/world
                )

            [method] => Array
                (
                    [3] => vs
                    [value se] => hello
                    [yes] => fsdfls
                )

        )

    [num] => 2
    [body] => 商品列表视图
)

```

**获取类的所有的方法的注解信息：**

```php
print_r($xan->parseAllMethodsDocComment(Hello::class));
```

**输出如下**

```php
Xan Object
(
    [num] => 3
    [annotations] => Array
        (
            [world] => Array
                (
                    [annotations] => Array
                        (
                            [Route] => Array
                                (
                                    [1] => hello/world
                                )

                            [method] => Array
                                (
                                    [3] => vs
                                    [value se] => hello
                                    [yes] => fsdfls
                                )

                        )

                    [num] => 2
                    [body] => 商品列表视图
                )

            [getMethodDocComment] => Array
                (
                    [annotations] => Array
                        (
                            [NotEmpty] => Array
                                (
                                    [2] => classNameOrObject
                                    [3] => methodName
                                )

                        )

                    [num] => 1
                    [body] => 获取方法的注解信息，返回一个对象
                )

            [getClassDocComment] => Array
                (
                    [annotations] => Array
                        (
                            [NotEmpty] => Array
                                (
                                    [1] => classNameOrObject
                                )

                        )

                    [num] => 1
                    [body] => 返回类的注解信息
                )

        )

)
```

## 简洁的APIs ##


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





# phpdoc-parser

高性能的 **PHP** 注解分析器，基于 **PHP7.x** 开发的一个PHP扩展

An extension for PHP document parsing. written in C code.

Author：Josin <774542602@qq.com>

## 使用环境 ##

PHP7.x

## 简洁的APIs ##


**Xan 类构造函数**

```php

final class Xan
{
	function __construct();
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
 * 返回一个包含注解信息的对象，[body]包含的是说明信息，其余的注解每一个注解是一个数组
 * 如：
 * /**
 *  * 类对象A
 *  *
 *  * @Route("/hello/world")
 *  * @NotEmpty(name="age", version="v2.1.1")
 *  */
 * class A
 * {
 * }
 *
 * 那么返回结果如下：
 * Xan Object
 * (
 *     [Route] => Array
 *     (
 *       [1] => "/hello/world"
 *     ),
 *     [NotEmpty] => Array
 *     (
 *        [name] => "age",
 *        [version] => "v2.1.1"
 *     ),
 *     [body] => ''
 *)
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





<?php

/**
 * Class Hello * hello * world
 *
 * This is the first time to do the job for the annotation
 * License: LGPL-v3
 *
 * ***@type(name="Hello", age="world")
 * @version(value="v2.1.23")
 */
class Hello
{
    /**
     * @Route("hello/world")
     * @method("vs")
     */
    public function world()
    {

    }
}


$xan = new Xan();

print_r($xan->getClassDocComment(Hello::class));

echo PHP_EOL;

print_r($xan->getMethodDocComment(Hello::class, "world"));

echo PHP_EOL;

$xan->parseDocComment($xan->getClassDocComment(Hello::class));
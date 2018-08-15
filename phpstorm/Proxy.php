<?php


namespace Xan\Aop;

/**
 * Class Proxy
 *
 * Xan\Aop\Proxy was the AOP component for the Xan extension
 * provide for the features with the annotations:
 * '@before', '@after', '@success', '@failure'
 *
 * if you use the Xan\Loader, the loader will also provide the AOP feature default.
 *
 * @package Xan\Aop
 */
class Proxy
{
    /**
     *
     * @param string $className The class which you want to use the AopProxy
     * @return \Xan\Aop\Proxy
     */
    static function instance($className){ return self; }
}
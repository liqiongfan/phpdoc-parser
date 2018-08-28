<?php


namespace Xan;

/**
 * Class App
 *
 * Xan\App provide features for the MVC programming in PHP code.
 * if you want to use the Xan to develop the MVC programm, you can use the Xan\App
 * to do the job.
 *
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
class App
{
    /**
     * @var \Xan\App
     */
    public static $app;

    /**
     * App constructor.
     *
     * @param array $options The $options contains the following elements
     *  'urlPattern'        => 'AUTO',
     *  'defaultModule'     => 'index',
     *  'urlGetStr'         => '_xurl',
     *  'defaultController' => 'index',
     *  'defaultAction'     => 'index',
     *  'viewSuffix'        => 'html',
     *  'urlSuffix'         => 'html',
     *  'allowModules'      => 'index,home',
     *  'applicationDir'    => './../apps',
     *  'defaultNamespace'  => 'app'
     */
    final function __construct($options = []) { }

    /**
     * App::run();
     *
     * To start the router parsing
     */
    final function run(){}
    
    /**
     * Set the $key to $value
     *
     * @param string $name
     * @param mixed  $value
     */
    final function __set($name, $value) {
    
    }
    
    /**
     * The alias of the function `__set`
     *
     * @param string $name
     * @param mixed  $value
     */
    final function set($name, $value) {
    
    }
    
    /**
     * Magic method to get the value named `$name`
     *
     * @param string $name
     */
    final function __get($name) {
    
    }
    
    /**
     * Alias of the function `__get`
     *
     * @param string $name
     */
    final function get($name) {
    
    }
    
    /**
     * To start a bootstrap step for the Xan framework.
     * When calling the bootstrap method, Xan will to find the Bootstrap class
     * and running the method with the prefix `__init`, so :
     *
     * class Bootstrap
     * {
     *     function __initView() {}
     *
     *     function __initDi() {}
     *
     *     function __initDb() {}
     *
     *     function __initSession() {}
     * }
     *
     * The Xan framework will run the method `__initView` `__initDi` `__initDb` `__initSession`
     * @return App
     */
    final function bootstrap() {
        return $this;
    }
    
    /**
     * To get the current module name
     *
     * [NOTE]
     *
     * In bootstrap step or index.php the getModule() will always return `index` the default module name
     *
     * @return string
     */
    final function getModule() {
    
    }
    
    /**
     * To get the current controller name
     *
     * [NOTE]
     *
     * In bootstrap step or index.php the getController() will always return `index` the default controller name
     *
     * @return string
     */
    final function getController() {
    
    }
    
    /**
     * To get the current Action name
     *
     * [NOTE]
     *
     * In bootstrap step or index.php the getAction() will always return `index` the default action name
     *
     * @return string
     */
    final function getAction() {
    
    }
    
    /**
     * To get the current url suffix, the default suffix is 'html`, which in default mode
     * the suffix `html` can pass or not, the kernel will parsing the url correctly. but
     * when in strict mode: pass the urlSuffix to Xan\App's constructor, the suffix of the url must
     * be passed explicitly
     *
     * @return string
     */
    final function getUrlSuffix() {
    
    }
}
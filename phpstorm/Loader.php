<?php

namespace Xan;

/**
 * Class Loader
 * The class Xan\Loader is the loader to helper user to autoload the
 * PHP file automatically
 *
 * for example:
 *
 * when you want to autoload the `app\Hello` class, which the file name is
 * `app/Hello.php` and the `app` alias point to the absolute path: __DIR__
 * so the Xan\Loader will to require the file __DIR__/Hello.php
 *
 * Example:
 * <pre>
 *   $loader = new Xan\Loader()
 *   $loader->setMap('@app', __DIR__);
 *   $loader->autoLoad();
 * </pre>
 *
 * @package Xan
 */
class Loader
{
    /**
     * Loader constructor.
     *
     * The constructor of the Xan\Loader class
     */
    function __construct()
    {
    }

    /**
     * Setting the mapping relationship between the $aliasName and $aliasPath
     *
     * for example:
     * <pre>
     *   setMap('@app', __DIR__);
     * </pre>
     *
     * @param $aliasName    string The $aliasName which start with '@' char.
     * @param $aliasPath    string The absolute path corresponding to the $aliasName
     * @return bool  true/false
     */
    function setMap($aliasName, $aliasPath) : bool
    {
    }

    /**
     * Start the auto load step
     */
    function autoLoad()
    {
    }
}
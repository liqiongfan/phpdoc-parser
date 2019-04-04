<?php

namespace Xan;

/**
 * Class Di
 *
 * The Xan\Di is the global Di container for user to Store plus Fetch object from the container
 * whole object stored in runtime global but so you can use it anywhere what you want to use the static method
 * calling, such as:
 * <pre>
 *  Xan\Di::set('db', function(){ return new Adapter('mysql:xxx', 'root', 'root'); });
 * </pre>
 *
 * Also you can get the Object from the global container such as the following do:
 * <pre>
 *  Xan\Di::get('db');
 * </pre>
 *
 * @author Josin<xeapplee@gmail.com>
 * @package Xan
 */
class Di
{
    /**
     * Store the object with the key to the global Di container.
     * @param string $key Container key to fetch the object from the Di
     * @param object $value Value to store to the container.
     * @return boolean
     */
    public static function set($key, $value) {

    }

    /**
     * Fetch the object from the container.
     * @param string $key The key to fetch the object which you set when storing the object.
     * @return object
     */
    public static function get($key) {

    }
    
}
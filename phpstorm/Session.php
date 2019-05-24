<?php

namespace Xan;

/**
 * Class Session
 *
 * Xan\Session was the component wrapper $_SESSION
 * users can use the Xan\Session to easy manage the $_SESSION
 *
 * for example:
 *
 * $session = new Xan\Session();
 *
 * set the key and value:
 *
 *      $session->set('xan.version', 'v1.1.9');
 * or
 *      $session['xan.version'] = 'v1.1.9';
 *
 * get the value from the $_SESSION:
 *
 *      echo $session->get('xan.version');
 * or
 *      echo $session['xan.version'];
 *
 * Also support iterator:
 *
 * foreach($session as $key => $value) {
 *      echo $key;
 *      print_r($value);
 * }
 * @package Xan
 * @author Josin<774542602@qq.com>
 */
class Session
{
    /**
     * Session constructor.
     *
     * Start the PHP SESSION if not active
     */
    public function __construct() { }
    
    /**
     * Return the $_SESSION
     * @return mixed
     */
    public static function getRaw() {
        return $_SESSION;
    }
    
    /**
     * Magic method to implement the __set method
     * @param string $name
     * @param mixed $value
     */
    public function __set($name, $value) { }
    
    /**
     * To get the given value with the key $name
     * @param string $name
     */
    public function __get($name) { }
    
    /**
     * To set the value into to the $_SESSION with the key $name and value $value
     *
     * @param string $name
     * @param mixed $value
     */
    public function set($name, $value) { }
    
    /**
     * Same use of the __get($name) method
     *
     * @param string $name
     * @return mixed
     */
    public function get($name) { }
    
    /**
     * Return the current data
     *
     * @return mixed
     */
    public function current() { }
    
    /**
     * Return the current key
     * @return string
     */
    public function key() { }
    
    /**
     * Return the next value
     * @return mixed
     */
    public function next() { }
    
    /**
     * Set the position to 0, move to first
     */
    public function rewind() { }
    
    /**
     * The current pos. was invalid or not.
     * @return bool
     */
    public function valid() {
        return true;
    }
    
    /**
     * Return the count of the array|$_SESSION
     * @return int
     */
    public function count() {
        return 1;
    }
    
    /**
     * Same with the __get($name)
     * @param string $name
     * @return mixed
     */
    public function offsetGet($name) { }
    
    /**
     * Same with the magic method __set($name, $value)
     * @param string $name
     * @param mixed $value
     */
    public function offsetSet($name, $value) { }
    
    /**
     * unset the current value of the $_SESSION
     *
     * @param string $name
     */
    public function offsetUnset($name) { }
    
    /**
     * Exists value with the given $name key
     *
     * @param string $name
     */
    public function offsetExists($name) { }
}
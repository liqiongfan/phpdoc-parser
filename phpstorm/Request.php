<?php

namespace Xan;

/**
 * Class Request
 *
 * The Xan\Request was the network component to easy use the $_GET|$_POST|$_SERVER
 * in the traditional programming, such as the following:
 * <pre>
 *      $valuea = !empty($_GET['keya']) ? $_GET['keya'] : 0
 *      $valueb = !empty($_GET['keyb']) ? $_GET['keyb'] : 0
 *      $valuec = !empty($_GET['keyc']) ? $_GET['keyc'] : 0
 *      $valued = !empty($_GET['keyd']) ? $_GET['keyd'] : 0
 *      $valuee = !empty($_GET['keye']) ? $_GET['keye'] : 0
 * </pre>
 *
 * In the new Xan\Request, you only use the following code instead:
 *
 * $valuea = $request->get('keya', 0)
 * $valueb = $request->get('keyb', 0)
 * and so on.
 * to make the code clean the easy understand.
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
class Request
{
    /**
     * Request constructor.
     */
    function __construct() { }
    
    /**
     * @param null                 $key              The key which you want to get the variable from the global variables: $_GET
     * @param null|string|callable $filters          The filters which you want to filter the value, if you set the value to string
     *                                                   it will let the kernel to call the given string-function to filter the value, `string` can
     *                                                   be gapped by '|' char, or you can set the $filters to a callable function
     * @param null|mixed           $defaultValue     The default value if the given key not exists in $_GET
     * @return mixed
     */
    function get($key = NULL, $filters = NULL, $defaultValue = NULL ) { }
    
    /**
     * @param null                 $key              The key which you want to get the variable from the global variables: $_POST
     * @param null|string|callable $filters          The filters which you want to filter the value, if you set the value to string
     *                                                   it will let the kernel to call the given string-function to filter the value, `string` can
     *                                                   be gapped by '|' char, or you can set the $filters to a callable function
     * @param null|mixed           $defaultValue     The default value if the given key not exists in $_POST
     * @return mixed
     */
    function getPost($key = NULL, $filters = NULL, $defaultValue = NULL ) { }
    
    /**
     * @param null                 $key              The key which you want to get the variable from the global variables: $_SERVER
     * @param null|string|callable $filters          The filters which you want to filter the value, if you set the value to string
     *                                                   it will let the kernel to call the given string-function to filter the value, `string` can
     *                                                   be gapped by '|' char, or you can set the $filters to a callable function
     * @param null|mixed           $defaultValue     The default value if the given key not exists in $_SERVER
     * @return mixed
     */
    function getServer($key = NULL, $filters = NULL, $defaultValue = NULL ) { }
    
}
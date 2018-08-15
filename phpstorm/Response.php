<?php


namespace Xan;

/**
 * Class Response
 *
 * Xan\Response component for the Xan extension was used to dispatching the url
 * to instead of the function `header` by php.
 *
 * @package Xan
 */
class Response
{
    /**
     * Response constructor.
     */
    function __construct() { }

    /**
     * To forward the given url without changing the browser's url info
     * and keep the raw input data, such as $_GET | $_POST | $_SERVER and so on
     *
     * @param string      $url            The url info which you want to dispatch to or forward to
     * @param array       $getParams      The $_GET data which you want to send to the url
     */
    function forward($url, $getParams = []) { }

    /**
     * To redirect the url, note that this will change the url info and the global data such as
     * $_GET | $_POST | $_SERVER will be lost after redirecting.
     *
     * @param string      $url            The url info which you want to dispatch to or forward to
     * @param array       $getParams      The $_GET data which you want to send to the url
     */
    function redirect($url, $getParams = []) { }

}
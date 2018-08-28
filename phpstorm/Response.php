<?php

namespace Xan;

/**
 * Class Response
 *
 * Response : a component for response contained in Xan framework.
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
class Response
{
    /**
     * Support not redirecting the url with the $_GET value
     *
     * @param $url
     * @param array $getData
     */
    function forward($url, $getData = []) {
    
    }
    
    /**
     * Redirect the url & $_GET data
     *
     * @param $url
     * @param array $getDate
     */
    function redirect($url, $getDate = []) {
    
    }
}
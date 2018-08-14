<?php

namespace Xan;

class App
{
    /**
     * App constructor.
     *
     * @param array $options The options for the Xan\App to parse the url
     *                       $options = [
     *                          'urlPattern'        => 'AUTO'  // AUTO | GET | PATH
     *                          'defaultModule'     => 'index',
     *                          'defaultController' => 'index',
     *                          'defaultAction'     => 'index',
     *                          'viewSuffix'        => 'html',
     *                          'urlSuffix'         => 'html',
     *                          'allowModules'      => 'index,home'
     *                      ]
     */
    function __construct($options = []) { }

    /**
     * Start the route parsing
     *
     * Start the process of the route
     */
    function run(){ }
}
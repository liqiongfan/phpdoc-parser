<?php


namespace Xan;

class Apps
{
    /**
     * @var \Xan\Apps
     */
    public static $app;

    /**
     * Apps constructor.
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
    function __construct($options = []) { }

    /**
     * Apps::run();
     *
     * To start the router parsing
     */
    function run(){}
}
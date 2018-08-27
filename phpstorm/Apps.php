<?php


namespace Xan;

/**
 * Class Apps
 *
 * Xan\Apps provide features for the MVC programming in PHP code.
 * if you want to use the Xan to develop the MVC programm, you can use the Xan\Apps
 * to do the job.
 *
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
abstract class Apps
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
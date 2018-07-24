<?php

/**
 * Class Xan
 *
 * Xan is a extension which written by C code.
 * Xan can parsing the PHP doc-comments easy & fast.
 *
 * License: GNU LGPL-3
 *
 * Author: Josin <774542602@qq.com> or Website: https://www.supjos.cn
 */
final class Xan
{
    /**
     * Xan constructor.
     *
     * To construct the Xan object
     */
    function __construct()
    {
    }

    /**
     * Function : `getClassDocComment` used to obtain the Class doc-comments with the given object-name or class name
     * such as:
     *
     * getClassDocComment(new Xan()) or
     * getClassDocComment(Xan::class)
     *
     * @param $objectOrName object|string   The class object or name which you want to obtain the doc-comments
     * @return string                       The doc-comments which you want to obtain
     */
    function getClassDocComment($objectOrName) : string
    {
    }

    /**
     * Function `getMethodDocComment` was used to obtain the object's method doc-comments
     * with the given object & method name
     *
     * @param $objectOrName object|string     The class object or name which you want to obtain the doc-comment
     * @param $methodName   string            The method name which exists in the $objectOrName class
     * @return string                         The result contains the doc-comments
     */
    function getMethodDocComment($objectOrName, $methodName) : string
    {
    }

    /**
     * Function `parseDocComment` was designed to parsing the doc-comments
     *
     * @param $docComments string      The doc-comments which you want to parsing
     * @return this                    The object which contains the result.
     */
    function parseDocComment($docComments) : this
    {
    }

    /**
     * Function: `getParseResult` parsing the result which return an array that contains the parsing result.
     *
     * @param $docComments  string   The doc-comments which you want to parsing.
     * @return array                 An array contains the parsing result.
     */
    function getParseResult($docComments) : array
    {
    }

    /**
     * Function: `parseAllMethodsDocComment` used to parsing all methods which in class{$objectOrName}
     * return the object contains the result, the result format like this:
     * <pre>
     * Xan Object
     * (
     *     [num] => 3
     *     [annotations] => Array
     *         (
     *             [world] => Array
     *                 (
     *                     [annotations] => Array
     *                         (
     *                             [Route] => Array
     *                                 (
     *                                     [1] => hello/world
     *                                 )
     *
     *                             [method] => Array
     *                                 (
     *                                     [3] => vs
     *                                     [value se] => hello
     *                                     [yes] => fsdfls
     *                                 )
     *
     *                         )
     *
     *                     [num] => 2
     *                     [body] =>
     *                 )
     *
     *             [getMethodDocComment] => Array
     *                 (
     *                     [annotations] =>
     *                     [num] => 0
     *                     [body] => 获取方法的注解信息，返回一个对象
     *                 )
     *
     *             [getClassDocComment] => Array
     *                 (
     *                     [annotations] =>
     *                     [num] => 0
     *                     [body] => 返回类的注解信息
     *                 )
     *         )
     * )
     * <pre>
     *
     * The result contains the `num` key means the methods number
     * the `annotations` key contains the result array, each key-value pair contains the key(The function name) & the value(array)
     * each value contains three key-value pairs:
     * `annotations`: The annotations the function contains
     * `num`        : The number of the annotations
     * `body`       : The body of the annotations
     *
     * @param $objectOrName object|string  The class name or object which you want to obtain the annotations
     * @return this                        The object contains the value
     */
    function parseAllMethodsDocComment($objectOrName) : this
    {
    }

    /**
     * Get the attrbute doc-comment
     * @param $className   The class which you want to obtain from
     * @param $attrName    The class' attribute
     * @return string      The result
     */
    function getAttrDocComment($className, $attrName): string {}

    /**
     * To obtain the const attr. doc-comment
     *
     * @param $className   The class which you want to obtain from
     * @param $constName   The const name which you get from the class
     * @return string      The result string
     */
    function getConstDocComment($className, $constName): string {}
}
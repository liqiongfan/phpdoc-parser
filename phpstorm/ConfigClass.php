<?php

namespace Xan\Type;

/**
 * Class ConfigClass
 *
 * The class config tool library for configure the class's key in runtime,
 * such as setting the constant value or the property with the compile time-mode.
 *
 * @package Xan\Type
 */
class ConfigClass
{
    const FINAL = 0x04;
    const PUBLIC = 0x100;
    const STATIC = 0x01;
    const PRIVATE = 0x400;
    const PROTECTED = 0x200;

    /**
     * Add a constant key-value to the given class with the given class name named `$className`
     *
     * @param string $className Class name, since PHP5.5 user can use the `::class` to get the class name
     * @param string $key       The key name
     * @param mixed $value      The key value.
     */
    static function setConstant($className, $key, $value){ }

    /**
     * Add an attribute to the given object.
     *
     * [NOTE]: This method will first to found the property_info in class_entry. if exists, modify the value and the access
     * flags with the given flgas, otherwise add the attribute-value to the object's property_tables.
     *
     * @param object $object The object which you want to set the attributes.
     * @param string $key    The string key also the attribute name
     * @param mixed $value   The value associated with the key
     * @param int $flags FINAL|PUBLIC and so on, or value with the `|` such as:
     *                            FINAL | PUBLIC or
     *                            STATIC | PUBLIC | FINAL
     */
    static function setAttribute($object, $key, $value, $flags = self::PUBLIC) { }

    /**
     * Set the access flags to the given class's key with the params:
     * `$key` & `$flags`
     *
     * @param string $className  The name which you want to set
     * @param string $key        The key name you want to set
     * @param int $flags FINAL|PUBLIC and so on, or value with the `|` such as:
     *                            FINAL | PUBLIC or
     *                            STATIC | PUBLIC | FINAL
     */
    static function setAccessFlags($className, $key, $flags) { }
}
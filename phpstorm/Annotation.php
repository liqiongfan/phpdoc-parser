<?php

namespace Xan\Type\Annotation;

/**
 * Interface Annotation
 *
 * All annotations will implements this interface to do the job.
 *
 * Every annotation need to implements the `Xan\Type\Annotation` to do the Annotation job.
 *
 * [Note]:
 *
 * When in Class-Level, the first parameter in function `input` will be the class name, but
 * when use the Annotation in function level, the parameter named `$objectOrName` will be the object of the current
 * class. you can do something to the object, modify, and so on.
 *
 * for example:
 *
 * @TagAnnotation(TT="Xan", VV="Extension")
 * class A
 * {
 *      @CartAnnotation(num=5, owner="Josin", money="55.5")
 *      function addToCart() {
 *
 *      }
 * }
 *
 * then in the annotation class it will be the following :
 *
 * class TagAnnotation implements Annotation
 * {
 *      public function input($className, $annotations) {
 *          // $className will be class name: 'A'
 *          // $annotations will be array: [ 'TT' => 'Xan', 'VV' => 'Extension ]
 *      }
 * }
 *
 *
 * class CartAnnotation implements Annotation
 * {
 *      public function input($object, $annotations) {
 *          // $object was object created by class A
 *          // $annotations was an array: [ 'num' => 5, 'owner' => 'Josin', 'money' => 55.5 ]
 *      }
 * }
 *
 * @package Xan\Type\Annotation
 */
interface Annotation
{
    /**
     * @param object|string $objectOrName  `class` level annotation will be class name, `function` level annotation was `Object`
     * @param array $annotations            The annotations which provided by annotation
     * @return mixed
     */
    public function input($objectOrName, $annotations);
}
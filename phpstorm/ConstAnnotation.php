<?php

namespace Xan\Type\Annotation;

/**
 * Class ConstAnnotation
 *
 * ConstAnnotation was used to init the class's const variables
 * This is the inner annotation class which to add some const variables to the calling object
 * for example:
 *
 * @Xan\Type\Annotation\ConstAnnotation(VERSION="v1.2", LEVEL=2)
 * class Index
 * {
 *
 * }
 *
 * when doing the step, the class Index will have two const variables, named `VERSION` & `LEVEL`
 * so :
 *
 * Index::VERSION => v1.2
 * Index::LEVEL   => 2
 *
 * This feature only be used in Xan loader
 *
 * @package Xan\Type\Annotation
 */
class ConstAnnotation implements Annotation
{
    public function input($classEntry, $annotations) {
    
    }
}
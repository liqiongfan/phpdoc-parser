<?php

namespace Xan\Type\Annotation;

/**
 * Class AttrAnnotation
 *
 * The AttrAnnotation class was used to annotation the class's attributes
 * used to add const variables to the given class not the object.
 * such as:
 *
 * <pre>
 *
 * @AttrAnnotation(name="hello")
 * class Abs
 * {
 * }
 *
 * @package Xan\Type\Annotation
 */
class AttrAnnotation implements Annotation
{
    public function input($classEntry, $annotations)
    {
    }
}
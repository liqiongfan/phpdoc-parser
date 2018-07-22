<?php

namespace Xan\Type\Annotation;

/**
 * Interface Annotation
 *
 * All annotations will implements this interface to do the job.
 *
 * @package Xan\Type\Annotation
 */
interface Annotation
{
    public function input($classEntry, $annotations);
}
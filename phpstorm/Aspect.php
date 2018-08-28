<?php


/**
 * Class Aspect
 *
 *
 * This was used to use the @Aspect feature
 *
 * `@Aspect` was the AOP featuring flag, to express the meaning that to use the AOP programming feature
 * every class modified by @Aspect will be parsing the doc-comment with the engine `Xan`, then users will use the
 * `@before` | `@after` | `@success` | `@failure` and other annotation class written by user. *
 *
 * Author: Josin
 */
class Aspect implements Xan\Type\Annotation\Annotation
{
    public function input($classEntry, $annotations) {
    
    }
}
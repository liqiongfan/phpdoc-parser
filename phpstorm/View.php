<?php

namespace Xan;

class View
{
    /**
     * View constructor.
     */
    function __construct() { }

    /**
     * To render the template file with the given variables
     *
     * @param string $templateFile The template file which you want to render
     * @param array  $variables    The variables which you will use in template file.
     */
    function render($templateFile, $variables=[]){ }

    /**
     * Add variables to the template file, this function can be called serval times
     *
     * @param string $variableName   The variables name
     * @param mixed $variableValue   The variables value
     */
    function assign($variableName, $variableValue){ }

    /**
     * Enable auto render or not; true means to enable auto render otherwise disable
     * @param bool $status  The status which the engine to render the file; true: means to autorender
     *                      otherwise disable autorendering.
     */
    function setAutoRender($status = true) { }
}
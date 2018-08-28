<?php

namespace Xan;

/**
 * Class View
 *
 * Xan\View is the View component for the render engine.
 * it has the more faster performance then the raw php code rendering engine.
 * simple use:
 * <pre>
 *     $view->render('index')
 * </pre>
 *
 * or if you want set some variables into the temple view file:
 * <pre>
 *      $view->render('index', [ 'version' => 'v2.1.1' ]);
 * </pre>
 * then in the view `index.html` file you can use the $version variable to get the value: 'v2.1.1'
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
class View
{
    /**
     * View constructor.
     */
    function __construct() { }

    /**
     * To render the template file with the given variables
     * in index module & index controller & index action:
     *
     * render('index');
     *
     * which will render the template file;
     *
     * `/index/modules/views/index/index.html`
     *
     * @param string $templateFile The template file which you want to render
     * @param array  $variables    The variables which you will use in template file.
     */
    function render($templateFile, $variables = []){ }
    
    /**
     * To render the module public view with different from the render call
     * path not contain the controller & action name only with the module: for example:
     *
     * partial('index');
     *
     * which will in index module, to render the template file:
     * `/index/views/index.html'
     *
     * @param $templateFile
     * @param array $variables
     */
    function partial($templateFile, $variables = []) { }

    /**
     * Add variables to the template file, this function can be called serial times
     *
     * @param string $variableName   The variables name
     * @param mixed $variableValue   The variables value
     */
    function assign($variableName, $variableValue){ }

    /**
     * Enable auto render or not; true means to enable auto render otherwise disable
     * @param bool $status  The status which the engine to render the file; true: means to auto-render
     *                      otherwise disable auto-rendering.
     */
    function setAutoRender($status = true) { }
}
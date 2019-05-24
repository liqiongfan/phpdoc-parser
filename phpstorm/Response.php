<?php

namespace Xan;

/**
 * Class Response
 *
 * Response : a component for response contained in Xan framework.
 *
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
class Response
{
	/**
	 * Response constructor.
	 */
	public function __construct() { }
	
	/**
	 * Encoding the given data into JSON format
	 * @param array $arrayData
	 * @param int $option The encoding option such as : JSON_UNESCAPED_UNICODE
	 * @return $this
	 */
	public function setJsonContent($arrayData, $option = JSON_UNESCAPED_UNICODE){ }
	
	/**
	 * Set the raw format html data to client or browser
	 * @param string $htmlData
	 * @return $this
	 */
	public function setRawContent($htmlData) { }
	
	/**
	 * Set the HTTP header with key & value
	 * @param string $headerName
	 * @param string $headerValue
	 * @return $this
	 */
	public function setHeader($headerName, $headerValue) { }
	
	/**
	 * Unset the HTTP header name
	 * @param string $headerName
	 * @return $this
	 */
	public function unsetHeader($headerName) {}
	
	/**
	 * Send the HTTP header and response data to the browser or client
	 * @return null
	 */
	public function send(){  }
	
}
<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/** 
 * Exception thrown when you cannot make an call to that action 
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class ForbiddenException extends Exception {}

/**
* request object, datawrapper for an request call
* 
* @author Yannick <yannick.l.88@gmail.com>
*/
class Request
{
    private static $requests;
    
    public $component;
    public $table;
    public $method;
    public $action;
    public $data;
    public $fullUrl;
    public $url;
    private $isProxyRequest;
    
    /**
     * Constructor, all relevent data is gathered here
     */
    public function __construct()
    {
        $config = Config::getInstance();
        
        $this->method = $_SERVER['REQUEST_METHOD'];
        $this->isProxyRequest = ($config->get("enabled", false, 'proxy') == "1" && strpos('http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'], $config->get("proxy", 'www.google.com', 'proxy')) === 0);
        
        switch($this->method)
        {
            case "POST" :
                $this->data = $_POST;
            break;
            case "PUT" :
                $this->data = array();
                parse_str(file_get_contents("php://input"), $data);
            break;
            default :
                $this->data = $_GET;
            break;
        }
        $this->data['method'] = strtolower($this->method);
        
        $this->fullUrl = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
        $this->url = $_SERVER['REQUEST_URI'];
        if($this->isProxyRequest)
        {
            $this->table = substr($this->fullUrl, strlen($config->get("proxy", 'www.google.com', 'proxy'))+1);
            if(strrpos($this->table, "?") > 0)
            {
                $this->table = substr($this->table, 0, strrpos($this->table, "?"));
            }
            
            $tableArr = explode(".", $this->table);
            
            $this->component = $tableArr[0];
        }
        else
        {
            if(isset($this->data['component']))
            {
                $this->component = $this->data['component'];
                unset($this->data['component']);
                
                if(isset($this->data['action']))
                {
                    $this->action = "action_".$this->data['action'];
                    unset($this->data['action']);
                }
            }
            else
            {
                //set the default actions
                $urls = Config::getInstance()->getURLMapping();
                list($this->component, $this->action) = $urls->getMapping($this->url, $this->data);
            }
        }
    }
    /**
     * Check if the request is made through the proxy url
     * 
     * @return Boolean
     */
    public function isMadeByProxy()
    {
        return $this->isProxyRequest;
    }
    /**
     * translate the HTTP method to SQL method
     * 
     * @return String           SELECT|INSERT|UPDATE|DELETE
     */
    public function translateAction()
    {
        switch($this->method)
        {
            case "POST" :
                return "INSERT";
            break;
            case "PUT" :
                return "UPDATE";
            break;
            case "DELETE" :
                return "DELETE";
            break;
            default :
                return "SELECT";
            break;
        }
    }
}
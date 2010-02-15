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
 */ 
class ForbiddenException extends Exception {}

/**
* request object, datawrapper for an request call
*/
class Request
{
    private static $requests;
    
    public $component;
    public $table;
    public $method;
    public $action;
    
    /**
     * translate the HTTP method to SQL method
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

/**
 * superclass for all component request classes
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 * @deprecated Please use the Table instaid of this
 */
class RequestComponent
{
    /**
    * handel the request for data through HTTP, all data comes from the 
    * URL. Eg: http://<host>/<component>, the action is based on the HTTP method
    * 
    * @return string
    */
    public static function handelRequest()
    {
        $config = Config::getInstance();
        
        //create the request
        $request = new Request();
        $request->method = $_SERVER['REQUEST_METHOD'];
        
        $url = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
        $request->table = substr($url, strlen($config->get("proxy", 'www.google.com', 'proxy'))+1);
        if(strrpos($request->table, "?") > 0)
        {
            $request->table = substr($request->table, 0, strrpos($request->table, "?"));
        }
        
        $tableArr = explode(".", $request->table);
        
        $request->component = $tableArr[0];
        $request->action = $request->translateAction();
        
        switch($request->method)
        {
            case "POST" :
                $data = $_POST;
            break;
            case "PUT" :
                $data = array();
                parse_str(file_get_contents("php://input"), $data);
            break;
            default :
                $data = $_GET;
            break;
        }
        
        $components = $config->getComponenets();
        $needsAuth = true;
        
        switch($request->method)
        {
            case "GET":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_select', 0, 'core'));
                break;
            case "POST":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_insert', 0, 'core'));
                break;
            case "PUT":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_update', 0, 'core'));
                break;
            case "DELETE":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_delete', 0, 'core'));
                break;
        }
        
        //auth
        if(
            $needsAuth && 
            (
                !isset($data['username']) || 
                !is_string($data['username']) || 
                $data['username'] != $config->get('username', null, 'proxy') || 
                !isset($data['password']) || 
                !is_string($data['password']) || 
                $data['password'] != $config->get('password', null, 'proxy')
            )
        )
        {
            header("HTTP/1.0 403 Forbidden");
            exit;
        }
        
        //handle request
        $proxyObject = Table::init($request->table);
        
        if($proxyObject)
        {
            
            unset($data['username']);
            unset($data['password']);
            unset($data['url']);
            
            try
            {
                $proxyObject->setExternal(true);
                $proxyObject->setRequest((object) $data);
                $proxyObject->setRecord((object) $data);
                
                $result = call_user_func(array($proxyObject, 'do'.ucfirst(strtolower($request->action))));
                $result = $result->getRows();
            }
            catch(ProxyForbiddenException $e)
            {
                header("HTTP/1.0 403 Forbidden");
                exit;
            }
            catch(Exception $e)
            {
                header("HTTP/1.0 500 Internal Server Error");
                
                echo nl2br($e->getMessage()."\n\n".$e->getTraceAsString());
                exit;
            }
            
            if($result)
            {
                header('Content-type: application/json');
                echo json_encode($result);
            }
        }
        else
        {
            header("HTTP/1.0 404 Not Found");
            exit;
        }
        
        exit;
    }
    /**
     * Do an redirect
     *
     * @param string $url           Url to redirect to
     * @param array $data           Array with data to be send
     */
    protected function redirect($url)
    {
        header("Location: {$url}");
        exit;
    }
    /**
     * Do an select call
     *
     * @param array $data          Array with data
     * @return Mixed
     */
    protected function do_SELECT($data = array())
    {
        throw new ForbiddenException();
    }
    /**
     * Do an delete call
     *
     * @param array $data          Array with data
     * @return Mixed
     */
    protected function do_DELETE($data = array())
    {
        throw new ForbiddenException();
    }
    /**
     * Do an update call
     *
     * @param array $data          Array with data
     * @return Mixed
     */
    protected function do_UPDATE($data = array())
    {
        throw new ForbiddenException();
    }
    /**
     * Do an insert call
     *
     * @param array $data          Array with data
     * @return Mixed
     */
    protected function do_INSERT($data = array())
    {
        throw new ForbiddenException();
    }
    /**
     * Preform an Select
     * 
     * @param array $data          Array with data
     * @return Mixed
     */
    public function doSelect($data = array())
    {
        return $this->do_SELECT($data);
    }
    /**
     * Preform an Update
     * 
     * @param array $data          Array with data
     * @return Mixed
     */
    public function doUpdate($data = array())
    {
        return $this->do_UPDATE($data);
    }
    /**
     * Preform an Insert
     * 
     * @param array $data          Array with data
     * @return Mixed
     */
    public function doInsert($data = array())
    {
        return $this->do_INSERT($data);
    }
    /**
     * Preform an Delete
     * 
     * @param array $data          Array with data
     * @return Mixed
     */
    public function doDelete($data = array())
    {
        return $this->do_DELETE($data);
    }
}
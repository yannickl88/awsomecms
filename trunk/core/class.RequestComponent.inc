<?php
/** @desc Exception thrown when you cannot make an call to that action */ 
class ForbiddenException extends Exception {}

/**
* @desc request object, datawrapper for an request call
*/
class Request
{
    private static $requests;
    
    public $component;
    public $methode;
    public $action;
    
    /**
    * @desc translate the HTTP methode to SQL methode
    */
    public function translateAction()
    {
        switch($this->methode)
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
* @desc superclass for all component request classes
*/
class RequestComponent
{
    /**
    * @desc handel the request for data through HTTP, all data comes from the 
    * URL. Eg: http://<host>/<component>, the action is based on the HTTP methode
    * 
    * @return               Echo of the data json encoded
    */
    public static function handelRequest()
    {
        $config = Config::getInstance();
        
        //create the request
        $request = new Request();
        $request->methode = $_SERVER['REQUEST_METHOD'];
        
        $url = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
        $request->component = substr($url, strlen($config->get("proxy", 'www.google.com', 'proxy'))+1);
        $request->component = substr($request->component, 0, strrpos($request->component, "?"));
        $request->action = $request->translateAction();
        
        switch($request->methode)
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
        
        $components = $config->getRequests();
        $needsAuth = true;
        
        switch($request->methode)
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
        $proxyObject = Request::init($request->component);
        
        if($proxyObject)
        {
            unset($data['username']);
            unset($data['password']);
            unset($data['url']);
            
            try
            {
                $result = call_user_func(array($proxyObject, 'do'.ucfirst(strtolower($request->action))), $data);
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
    * @desc Do an redirect
    * 
    * @param $url           Url to redirect to
    * @param $data          Array with data to be send
    */
    protected function redirect($url)
    {
        header("Location: {$url}");
        exit;
    }
    
    /**
    * @desc Do an select call
    * 
    * @param $request       Request
    * @param $data          Array with data
    * @return               Mixed
    */
    protected function do_SELECT($data = array())
    {
        throw new ForbiddenException();
    }
    
    /**
    * @desc Do an delete call
    * 
    * @param $request       Request
    * @param $data          Array with data
    * @return               Mixed
    */
    protected function do_DELETE($data = array())
    {
        throw new ForbiddenException();
    }
    
    /**
    * @desc Do an update call
    * 
    * @param $request       Request
    * @param $data          Array with data
    * @return               Mixed
    */
    protected function do_UPDATE($data = array())
    {
        throw new ForbiddenException();
    }
    
    /**
    * @desc Do an insert call
    * 
    * @param $request       Request
    * @param $data          Array with data
    * @return               Mixed
    */
    protected function do_INSERT($data = array())
    {
        throw new ForbiddenException();
    }
    
    public function doSelect($data = array())
    {
        return $this->do_SELECT($data);
    }
    public function doUpdate($data = array())
    {
        return $this->do_UPDATE($data);
    }
    public function doInsert($data = array())
    {
        return $this->do_INSERT($data);
    }
    public function doDelete($data = array())
    {
        return $this->do_DELETE($data);
    }
}
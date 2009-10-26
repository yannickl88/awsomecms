<?php
import('/core/shared/class.Component.inc');
import('/core/shared/class.RequestComponent.inc');

/** @desc Exception thrown if the given component is not installed or found */ 
class ComponentNotFoundException extends Exception {}
/** @desc Exception thrown if the given action for an component could not be found */ 
class ActionNotFoundException extends Exception {}
/** @desc Exception thrown if controller is waiting for the setData to be called */ 
class NotInstanciatedException extends Exception {}

/**
* @desc Controller class that handels all the incomming data requests
*/
class Controller
{
    private static $instance;
    
    private $smarty;
    private $smartyLoader;
    private $instanciated = false;
    
    private $components;
    
    public $lastRecord;
    
    /**
    * @desc singelton
    * 
    * @return               Instance of Controller
    */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    
    /**
    * @desc constructor
    */
    private function __construct()
    {
        $this->components = array();
        $this->lastRecord = array();
    }
    
    /**
    * @desc set the data for this controller, this should be called before anything else
    * 
    * @param $smarty        Smarty object
    * @param $smartyLoader  SmartyLoader object
    */
    public function setData($smarty, $smartyLoader)
    {
        $this->smarty = $smarty;
        $this->smartyLoader = $smartyLoader;
        
        $this->instanciated = true;
    }
    
    /**
    * @desc serve a page to the user
    * 
    * @param $action        Action
    * @param $component     Component name
    * @return               HTML
    */
    public function dispatch($action = "index", $component = "core")
    {
        if(!$this->instanciated)
        {
            throw new NotInstanciatedException();
        }
        
        switch(strtoupper($_SERVER['REQUEST_METHOD']))
        {
            case 'POST' :
                $request = $_POST;
                $request['methode'] = 'post';
                break;
            default :
                $request = $_GET;
                $request['methode'] = 'get';
                break;
        }
        
        if(isset($_SESSION['FORMERROR']))
        {
            import('/core/shared/class.InvalidFormException.inc');
            
            $e = unserialize($_SESSION['FORMERROR']);
            $data = array("record" => $e->getRecord(), "fields" => $e->getInvalidFields());
            
            if("/".$_GET['url'] == $e->getRedirect())
            {
                $this->lastRecord = $e->getRecord();
                $this->smarty->assign("FORMERROR", $data);
            }
            else
            {
                unset($_SESSION['FORMERROR']);
            }
        }
        
        //add extra session related things to smarty
        try
        {
            $this->callComponentAction(Config::get("component", "session", "session"), "action_".Config::get("action", "session", "session"), array($this->smarty, $this->smartyLoader, array()));
        }
        catch(Exception $e) { }
        
        //do the requests
        if(isset($request['component']))
        {
            $component = $request['component'];
            unset($request['component']);
            
            if(isset($request['action']))
            {
                $action = $request['action'];
                unset($request['action']);
            }
        }
        
        $this->handleRequest($action, $component, $request);
    }
    
    /**
    * @desc handel a request for a component
    * 
    * @param $action        Action
    * @param $component     Component name
    * @param $request       Array of parameters
    * @return               HTML
    */
    private function handleRequest($action, $component, $request)
    {
        $actionHandler = "action_".$action;
        
        try
        {
            return $this->callComponentAction($component, $actionHandler, array($this->smarty, $this->smartyLoader, $request));
        }
        catch(InvalidFormException $e)
        {
            $_SESSION['FORMERROR'] = serialize($e);
            
            $url = $e->getRedirect();
            $data = $e->getRedirectData();
            
            foreach($data as $key => $item)
            {
                $url .= '&'.$key.'='.$item;
            }
            
            header("Location: {$url}");
            exit;
        }
        catch(ComponentNotFoundException $e)
        {
            header("HTTP/1.0 404 Not Found");
            exit;
        }
        catch(ActionNotFoundException $e)
        {
            header("HTTP/1.0 404 Not Found");
            exit;
        }
        catch(Exception $e)
        {
            header("HTTP/1.0 500 Internal Server Error");
            
            echo nl2br($e->getMessage()."\n\n".$e->getTraceAsString());
            exit;
        }
    }
    
    /**
    * @desc make a reqest for data to an component
    * 
    * @param $action        Action
    * @param $component     Component name
    * @param $data          Array of data
    * @return               Mixed
    */
    public static function makeRequest($component, $methode = "GET", $data = array())
    {
        $config = Config::getInstance();
        
        try
        {
            $request = new Request();
        
            $request->methode = $methode;
            $request->component = $component;
            $request->action = $request->translateAction();
            
            $componentObject = Request::init($component);
            
            if($componentObject)
            {
                $output = call_user_func(array($componentObject, 'do'.ucfirst(strtolower($request->action))), $data);
            }
            else
            {
                $output = false;
            }
        }
        catch(ForbiddenException $e)
        {
            $output = false;
        }
        
        return $output;
    }
    
    /**
    * @desc Get the user auth data for the session
    * 
    * @param $user_id       Id from the user
    * @return               Array with data
    */
    public function auth($user_id)
    {
        if(!$this->instanciated)
        {
            throw new NotInstanciatedException();
        }
        
        $component = Config::get('component', false, 'auth');
        $actionHandler = "auth";
        
        try
        {
            return $this->callComponentAction($component, $actionHandler, array($user_id));
        }
        catch(Exception $e)
        {
            return false;
        }
    }
    
    /**
    * @desc Check if the user is a valid one
    * 
    * @param $user_name     User name
    * @param $user_pass     User password
    * @return               Boolean
    */
    public function auth_login($user_name, $user_pass)
    {
        if(!$this->instanciated)
        {
            throw new NotInstanciatedException();
        }
        
        $component = Config::get('component', false, 'auth');
        $actionHandler = "auth_login";
        
        try
        {
            return $this->callComponentAction($component, $actionHandler, array($user_name, $user_pass));
        }
        catch(Exception $e)
        {
            return false;
        }
    }
    
    /**
    * @desc call and action on an component
    * 
    * @param $component     Component name (string)
    * @param $action        Action (Methode) name
    * @param $parameters    Array with parameters
    * @return               Mixed
    */
    private function callComponentAction($component, $action, $parameters)
    {
        if($this->components[$component])
        {
            $componentObject = $this->components[$component];
            
            return call_user_func_array(array($componentObject, $action), $parameters);
        }
        
        $config = Config::getInstance();
        
        $components = $config->getComponenets();
        
        $actionHandler = $action;
        $componentObject = Component::init($component);
        
        if($componentObject)
        {
            if(!method_exists($componentObject, $actionHandler))
            {
                throw new ActionNotFoundException();
            }
            
            $this->components[$component] = $componentObject;
            
            return call_user_func_array(array($componentObject, $actionHandler), $parameters);
        }
        else
        {
            throw new ComponentNotFoundException();
        }
    }
}

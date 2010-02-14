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
import('/core/class.Component.inc');
import('/core/class.RequestComponent.inc');

/** 
 * Exception thrown if the given component is not installed or found
 *  
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class ComponentNotFoundException extends Exception {}
/** 
 * Exception thrown if the given action for an component could not be found 
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class ActionNotFoundException extends Exception {}
/** 
 * Exception thrown if controller is waiting for the setData to be called 
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class NotInstanciatedException extends Exception {}

/**
 * Controller class that handels all the incomming data requests
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Controller
{
    private static $instance;
    
    private $smarty;
    private $smartyLoader;
    private $instanciated = false;
    private $redirect = false;
    
    private $components;
    
    public $lastRecord;
    
    /**
     * singelton
     *
     * @return Controller
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    /**
     * constructor
     */
    private function __construct()
    {
        $this->components = array();
        $this->lastRecord = array();
    }
    /**
     * Set the data for this controller, this should be called before anything else
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     */
    public function setData($smarty, $smartyLoader)
    {
        $this->smarty = $smarty;
        $this->smartyLoader = $smartyLoader;
        
        $this->instanciated = true;
    }
    /**
     * Serve a page to the user
     *
     * @param string $action        Action
     * @param string $component     Component name
     * @return string
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
                $request['method'] = 'post';
                break;
            default :
                $request = $_GET;
                $request['method'] = 'get';
                break;
        }
        
        if(isset($_SESSION['FORMERROR']))
        {
            import('/core/class.InvalidFormException.inc');
            
            $e = unserialize($_SESSION['FORMERROR']);
            
            if("/".$_GET['url'] == $e->getRedirect())
            {
                $record = $e->getRecord();
                
                $this->smarty->assign("FORMERROR", array("record" => $record[0], "fields" => $e->getInvalidFields()));
                
                Table::init($e->getTable())
                    ->setErrorRecord($record[0], $e->getInvalidFields());
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
        
        $prehook = Config::getInstance()->getHook($component, $action, "pre");
        $posthook = Config::getInstance()->getHook($component, $action, "post");
        
        //pre action
        if($prehook)
        {
            try
            {
                Debugger::getInstance()->log("Calling PRE hook {$prehook[1]} on {$prehook[0]}");
                
                $prehookComponent = Component::init($prehook[0]);
                $prehookAction = "hook_".$prehook[1];
                
                if(method_exists($prehookComponent, $prehookAction))
                {
                    call_user_func(array($prehookComponent, $prehookAction), $smarty, $request);
                }
            }
            catch(Exception $e) 
            {
                Debugger::getInstance()->log("There was an error calling PRE hook {$prehook[1]} on {$prehook[0]}, '{$e->getMessage()}'");
            }
        }
        
        $this->handleRequest($action, $component, $request);
        
        //post action
        if($posthook)
        {
            try
            {
                Debugger::getInstance()->log("Calling POST hook {$posthook[1]} on {$posthook[0]}");
                
                $posthookComponent = Component::init($posthook[0]);
                $posthookAction = "hook_".$posthook[1];
                
                if(method_exists($posthookComponent, $posthookAction))
                {
                    call_user_func(array($posthookComponent, $posthookAction), $smarty, $request);
                }
            }
            catch(Exception $e)
            {
                Debugger::getInstance()->log("There was an error calling POST hook {$posthook[1]} on {$posthook[0]}, '{$e->getMessage()}'");
            }
        }
        
        //redirect if set
        $this->redirect();
    }
    /**
     * Handel a request for a component
     *
     * @param string $action
     * @param string $component
     * @param array $request
     * @param string $functionPrefix    String of the prefix for the function to be called
     * @return string
     */
    private function handleRequest($action, $component, &$request, $functionPrefix = "action")
    {
        $actionHandler = $functionPrefix."_".$action;
        
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
            
            if(Config::get('debug', 0, 'debug') > 1)
            {
                echo "Non debug would have redirected to \" <a href='{$url}'>{$url}</a> \"";
                echo Debugger::getInstance();
            }
            else
            {
                header("Location: {$url}");
            }
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
     * Make a reqest for data to an component
     *
     * @param string $action
     * @param string $component
     * @param array $data
     * @return string
     */
    public static function makeRequest($component, $method = "GET", $data = array())
    {
        $config = Config::getInstance();
        
        try
        {
            $request = new Request();
        
            $request->method = $method;
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
     * Get the user auth data for the session
     *
     * @param int $user_id
     * @return array
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
     * Check if the user is a valid one
     *
     * @param string $user_name
     * @param string $user_pass
     * @return boolean
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
     * Call and action on an component
     *
     * @param string $component
     * @param string $action
     * @param array $parameters
     * @return mixed
     */
    private function callComponentAction($component, $actionHandler, $parameters)
    {
        $config = Config::getInstance();
        
        $components = $config->getComponenets();
        
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
    /**
     * Do an redirect, based on what is set.
     *
     * @param string $url
     * @param array $data
     */
    public function redirect()
    {
        if($this->redirect)
        {
            if(Config::get('debug', 0, 'debug') > 1)
            {
                echo "Non debug would have redirected to \" <a href='{$this->redirect}'>{$this->redirect}</a> \"";
                echo Debugger::getInstance();
            }
            else
            {
                header("Location: {$this->redirect}");
            }
            exit;
        }
    }
    /**
     * Set the redirect, this can be used to prospone the redirect till everything is done executing
     * 
     * @param string $url
     */
    public function setRedirect($url)
    {
        $this->redirect = $url;
    }
    /**
     * Get the smarty object
     * 
     * @return Smarty
     */
    public function getSmarty()
    {
        return $this->smarty;
    }
}

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
 * Exception to be triggered when the current user does not have the proper authorization
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UnauthException extends Exception {}
/**
 * Manager for handeling notifications
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Notifications
{
    private $notifications;
    private $map;
    
    /**
     * Singelton
     * 
     * @return Notifications
     */
    public static function getInstance()
    {
        if(isset($_SESSION['notifications']))
        {
            return unserialize($_SESSION['notifications']);
        }
        else
        {
            return new Notifications();
        }
    }
    /**
     * Constructor
     */
    private function __construct()
    {
        $this->notifications = array();
        $this->map["ids"] = array();
        $this->map["url"] = array();
        $this->save();
    }
    /**
     * Add a notifications for a given URL
     * 
     * @param string $mess
     * @param string $url
     */
    public function addNotification($mess, $url, $id = false)
    {
        if(!$id)
        {
            $id = md5($mess);
        }
        
        if(!isset($this->notifications[$id]))
        {
            $this->notifications[$id] = $mess;
            $this->map["ids"][$id] = $url;
            if(!isset($this->map["url"][$url]))
            {
                $this->map["url"][$url] = array();
            }
            $this->map["url"][$url][] = $id;
            $this->save();
        }
    }
    /**
     * Check if a given notification already exists
     * 
     * @param string $key
     * @return boolean
     */
    public function hasNotification($key)
    {
        return isset($this->notifications[$key]);
    }
    /**
     * Get the notifications for a give URL
     * 
     * @param string $url
     * @return array
     */
    public function getNotifications($url = false)
    {
        $notes = array();
        
        if(!$url && isset($_GET['URL']))
        {
            $url = $_GET['URL'];
        }
        
        if(isset($this->map["url"][$url]) && is_array($this->map["url"][$url]) && count($this->map["url"][$url]) > 0)
        {
            foreach($this->map["url"][$url] as $key)
            {
                $notes[$key] = $this->notifications[$key];
            }
            removeNotification($url);
        }
        if(isset($this->map["url"]["*"]) && is_array($this->map["url"]["*"]) && count($this->map["url"]["*"]) > 0)
        {
            foreach($this->map["url"]["*"] as $key)
            {
                $notes[$key] = $this->notifications[$key];
            }
        }
        
        return $notes;
    }
    /**
     * Save the notications to the session. 
     * 
     * Note: This is also called when changes happen inside this object
     */
    public function save()
    {
        $_SESSION['notifications'] = serialize($this);
    } 
    /**
     * remove the notifications for an URL
     * 
     * @param string $url
     */
    public function removeNotification($url, $id = false)
    {
        if($id === false)
        {
            foreach($this->map["url"][$url] as $key)
            {
                unset($this->map["ids"][$key]);
                unset($this->notifications["ids"][$key]);
            }
            unset($this->map["url"][$url]);
        }
        else
        {
            unset($this->map["ids"][$id]);
            unset($this->notifications["ids"][$id]);
            unset($this->map["url"][$url][array_search($id, $this->map["url"][$url])]);
        }
        $this->save();
    }
}
/**
 * Controller class that handels all the incomming data requests
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Controller
{
    private static $instance;
    
    /**
     * @var Smarty
     */
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
    public function dispatch($action = false, $component = false)
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
            $url = "/";
            if(isset($_GET['url']))
            {
                $url = "/".$_GET['url'];
            }
            if(is_a($e, 'InvalidFormException') && $url == $e->getRedirect())
            {
                $this->smarty->assign("FORMERROR", $e);
                
                $table = $e->getTable();
                
                if(!empty($table))
                {
                    Table::init($e->getTable())
                        ->setError($e);
                }
                
                unset($_SESSION['FORMERROR']);
            }
            else
            {
                unset($_SESSION['FORMERROR']);
            }
        }
        
        $url = "";
        if(isset($_GET['url']))
        {
            $url = $_GET['url'];
        }
        $this->smarty->assign("NOTIFICATIONS", Notifications::getInstance()->getNotifications($url));
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
        else
        {
            //set the default actions
            $urls = Config::get("urlmap", array());
            
            foreach($urls as $check)
            {
                $matches = array();
                if(preg_match("/^".$check['pattern']."$/", "/".$url, $matches) === 1 && Config::getInstance()->isComponentInstalled($check['component']))
                {
                    $component = $check['component'];
                    $action = $check['action'];
                    $request['urlmapmatch'] = $matches[1];
                    break;
                }
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
                    call_user_func(array($prehookComponent, $prehookAction), $this->smarty, $request);
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
                    call_user_func(array($posthookComponent, $posthookAction), $this->smarty, $request);
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
        $exceptions = $this->getActionAuthExceptions();
        
        if(!in_array($component.".".$actionHandler, $exceptions))
        {
            //check the authentication
            $auth = SQLQuery::doSelect()
                ->table("auth")
                ->where("auth_component", $component)
                ->where("auth_action ", $actionHandler)
                ->exec()
                ->getRows();
            
            if(count($auth) > 0)
            {
                $valid = false;
                
                foreach($auth as $authRow)
                {
                    if($authRow->auth_privilage == "*guest*")
                    {
                        $valid = true;
                        break;
                    }
                    elseif($_SESSION['usr']['auth'][$authRow->auth_privilage])
                    {
                        $valid = true;
                        break;
                    }
                }
                
                if(!$valid)
                {
                    throw new ForbiddenException();
                }
            }
        }
        
        try
        {
            return $this->callComponentAction($component, $actionHandler, array($this->smarty, $this->smartyLoader, $request));
        }
        catch(InvalidFormException $e)
        {
            $_SESSION['FORMERROR'] = serialize($e);
            
            $url = $e->getRedirect();
            $query = "";
            $data = $e->getRedirectData();
            
            foreach($data as $key => $item)
            {
                if($query != "")
                {
                    $query .= "&";
                }
                else
                {
                    $query .= "?";
                }
                
                $query .= $key.'='.$item;
            }
            
            $url .= $query;
            
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
    /**
     * List of action that do not need auth checking
     * 
     * @return array
     */
    public function getActionAuthExceptions()
    {
        $exceptions = array(
            "core.action_index"
        );
        
        $exceptions[] = Config::get("component", "", "session").".action_".Config::get("action", "", "session");
        
        return $exceptions;
    }
}

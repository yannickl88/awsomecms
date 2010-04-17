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
import('/core/class.Request.inc');

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
    
    public static function getInstance()
    {
        if(!self::$instance)
        {
            self::$instance = new Controller();
        }
        
        return self::$instance;
    }
    
    private $redirect;
    private $request = null;
    private $smarty;
    private $smartyLoader;
    
    /**
     * Handle a request
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     */
    public function dispatch($smarty, $smartyLoader)
    {
        $request = $this->getRequest();
        $this->smarty = $smarty;
        $this->smartyLoader = $smartyLoader;
        
        if($request->isMadeByProxy())
        {
            echo $this->callTable($request->component, $request->translateAction(), $request);
        }
        else
        {
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
            $this->smarty->assign("NOTIFICATIONS", Notifications::getInstance()->getNotifications($request->url));
            
            //add extra session related things to smarty
            try
            {
                $this->callAction(Config::get("component", "session", "session"), "action_".Config::get("action", "session", "session"), $this->getRequest());
            }
            catch(Exception $e) { }
            
            $this->callAction($request->component, $request->action, $this->getRequest());
            $this->redirect();
        }
    }
    /**
     * call an action on a component with it's hooks
     * 
     * @param Component $component
     * @param String $action            action with the action prefix
     * @param Request $request
     * @return mixed
     */
    public function callAction($component, $action, $request)
    {
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
                    call_user_func(array($prehookComponent, $prehookAction), $this->smarty, $this->smartyLoader, $request->data);
                }
            }
            catch(Exception $e) 
            {
                Debugger::getInstance()->log("There was an error calling PRE hook {$prehook[1]} on {$prehook[0]}, '{$e->getMessage()}'");
            }
        }
        
        $result = call_user_func(array(Component::init($component), $action), $this->smarty, $this->smartyLoader, $request->data);
        
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
                    call_user_func(array($posthookComponent, $posthookAction), $this->smarty, $this->smartyLoader, $request->data);
                }
            }
            catch(Exception $e)
            {
                Debugger::getInstance()->log("There was an error calling POST hook {$posthook[1]} on {$posthook[0]}, '{$e->getMessage()}'");
            }
        }
        
        return $result;
    }
    /**
     * Handel the request made by the proxy
     * 
     * @param Component $component
     * @param String $action            SELECT|INSERT|UPDATE|DELETE
     * @param Request $request
     */
    public function callTable($component, $action, $request)
    {
        $config = Config::getInstance();
        $components = $config->getComponenets();
        $needsAuth = true;
        
        switch(strtoupper($request->translateAction()))
        {
            case "SELECT":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_select', 0, 'core'));
                break;
            case "INSERT":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_insert', 0, 'core'));
                break;
            case "UPDATE":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_update', 0, 'core'));
                break;
            case "DELETE":
                $needsAuth = Config::hasFlag($components[$request->component]->component_auth, (int) $config->get('auth_delete', 0, 'core'));
                break;
        }
        
        //auth
        if(!$needsAuth || ($needsAuth && !empty($request->data['username']) && !empty($request->data['password']) && $request->data['username'] == Config::get("username", false, "proxy") && $request->data['password'] == Config::get("password", false, "proxy")))
        {
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
                    
                    $result = call_user_func(array($proxyObject, 'do'.ucfirst(strtolower($request->translateAction()))));
                    $result = $result->getRows();
                }
                catch(ProxyForbiddenException $e)
                {
                    header("HTTP/1.0 403 Forbidden");
                }
                
                if($result)
                {
                    header('Content-type: application/json');
                    return json_encode($result);
                }
            }
            else
            {
                header("HTTP/1.0 404 Not Found");
            }
        }
        else
        {
            header("HTTP/1.0 403 Forbidden");
        }
    }
    /**
     * @return Request
     */
    private function getRequest()
    {
        if(!$this->request)
        {
            $this->request = new Request();
        }
        
        return $this->request;
    }
    /**
     * Get the smarty object that is used for the page rendering
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
    /**
     * Do an redirect, based on what is set.
     *
     * @param string $url
     * @param array $data
     */
    public function redirect()
    {
        if(!empty($this->redirect))
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
}

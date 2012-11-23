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
import("core/class.Cache.inc");
import("core/class.Component.inc");
import("core/class.Extention.inc");
/**
 * Manager for registering components, hooks, mappings and pages dynamicly
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class RegisterManager
{
    private static $instance;

    private $mappings;
    private $hooks;
    private $pages;
    private $components;
    private $auth;
    private $max;

    /**
     * Singelton
     * @return RegisterManager
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    /**
     * Construcor
     */
    private function __construct()
    {
        $this->load();
    }
    /**
     * Check if the cache is avalible and usable
     */
    public function needsRevalidation()
    {
        if(Cache::has("components") && Cache::has("mappings") && Cache::has("hooks") && Cache::has("pages") && Cache::has("auth"))
        {
            if(Config::get("debug", 0, "debug") >= 1)
            {
                $component = Cache::get("components");

                foreach($component as $component)
                {
                    $componentFile = $component->component_path."class.".ucfirst($component->component_name)."Component.inc";
                    if(file_exists($componentFile))
                    {
                        if($component->component_lastModified < filemtime($componentFile))
                        {
                            return true;
                        }
                    }
                }
                
                //check if the config has changed
                if(Cache::has("configCheck") && Cache::get("configCheck") != Config::get("checksum", ""))
                {
                    Cache::set("configCheck", Config::get("checksum", ""));
                    return true;
                }
                else if(!Cache::has("configCheck"))
                {
                    Cache::set("configCheck", Config::get("checksum", ""));
                }

                return false;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    /**
     * Load the content for the RegisterManager, either from cache or from the components
     */
    private function load()
    {
        if($this->needsRevalidation())
        {
            Debugger::getInstance()->log("Rebuilding Cache for RegisterManager");
            $this->max = -1;

            $this->componenets = array();
            $this->hooks = array();
            $this->pages = array();
            $this->mappings = array();
            $this->auth = array();

            //Load the components
            try {
                $result = SQLQuery::doSelect()
                    ->table('components')
                    ->orderBy("component_name")
                    ->exec();
            }
            catch(SQLExeption $e)
            {
                return; //seems we are not properly installed (yet)
            }

            while($row = $result->getRow())
            {
                //set the path if empty
                if(empty($row->component_path))
                {
                    $row->component_path = realpath(getFrameworkRoot() . "/components/" . $row->component_name).DIRECTORY_SEPARATOR;
                }
                if(Config::get("debug", 0, "debug") >= 1)
                {
                    $componentFile = $row->component_path."class.".ucfirst($row->component_name)."Component.inc";
                    if(file_exists($componentFile))
                    {
                        $row->component_lastModified = filemtime($componentFile);
                    }
                }

                $this->componenets[$row->component_name] = $row;

                //init the component
                try
                {
                    $component = Component::createComponent($row->component_name, $row->component_path);
                    $component->register($this);
                }
                catch(Exception $e) { }
            }
            
            //Load the extentions
            $extentions = scandir(getFrameworkRoot()."/extentions/");
            
            foreach($extentions as $extention)
            {
                $matches = array();
                if($extention != "." && $extention != ".." && preg_match('/^(.*)\.ext\.inc$/', $extention, $matches) === 1)
                {
                    try
                    {
                        $ext = Extention::init($matches[1]);
                        $ext->register($this);
                    }
                    catch(Exception $e) { throw $e; }
                }
            }

            $this->save();
        }
        else
        {
            $this->componenets = Cache::get("components");
            $this->mappings = Cache::get("mappings");
            $this->hooks = Cache::get("hooks");
            $this->pages = Cache::get("pages");
            $this->auth = Cache::get("auth");
        }
    }
    /**
     * Cache the content of the RegisterManager
     */
    private function save()
    {
        Cache::set("components", $this->componenets);
        Cache::set("mappings", $this->mappings);
        Cache::set("hooks", $this->hooks);
        Cache::set("pages", $this->pages);
        Cache::set("auth", $this->auth);
    }
    /**
     * Reload the content of the RegisterManager
     * 
     * @param Bool $noload      Prevent Registermanager from already loading new data
     */
    public function reload()
    {
        Cache::clear("components");
        Cache::clear("mappings");
        Cache::clear("hooks");
        Cache::clear("pages");
        Cache::clear("auth");
        
        $this->load();
    }
    /**
     * Register a Hook
     *
     *
     */
    public function registerHook($component, $target, $prepost, $action, $function)
    {
        $key = "{$component}-{$action}-{$prepost}";
        $this->hooks[$key] = (object) array(
        "component" => $component,
        "target" => $target,
        "prepost" => $prepost,
        "action" => $action,
        "function" => $function
        );
    }
    /**
     * Register a page
     *
     * @param * $url
     * @param mixed $content    Either a string or a callback
     */
    public function registerPage($url, $content)
    {
        $this->pages[makeLink($url)] = array($content, time());
    }
    /**
     * Register an auth group to which this action belongs
     * 
     * @param String $action
     * @param String $group
     */
    public function registerAuth($action, $group)
    {
        if(!isset($this->auth[$group]))
            $this->auth[$group] = array();
            
        $this->auth[$group][] = $action;
    }
    /**
     * Register an urlmapping
     *
     * @param string $pattern
     * @param string $action
     * @param int $sort
     */
    public function registerURLMap($pattern, $action, $sort = -1)
    {
        if($sort > $this->max)
        {
            $this->max = $sort;
        }
        else if($sort == -1)
        {
            $sort = $this->max;
        }

        $this->mappings[] = array($sort, $pattern, explode(".", $action));

        usort($this->mappings, create_function('$a,$b', 'if ($a[0] == $b[0]) { return 0; } else { return ($a[0] < $b[0]) ? 1 : -1; }'));
    }
    /**
     * Get a Hook
     *
     * @return
     */
    public function getHook($component, $action, $prepost)
    {
        $key = "{$component}-{$action}-{$prepost}";
        
        if(isset($this->hooks[$key]))
        {
            return $this->hooks[$key];
        }
        else
        {
            return false;
        }
    }
    /**
     * Get a page
     * 
     * @param * $url
     * @return String
     */
    public function getPage($url)
    {
        $page = $this->pages[makeLink($url)];

        if(is_array($page[0]) && is_callable($page[0]))
        {
            return array(call_user_func($page[0]), $page[1]);
        }
        else
        {
            return $page;
        }
    }
    /**
     * Get the mapping for an URL
     *
     * @param string $url
     * @param Array $request
     */
    public function getURLMap($url, &$request)
    {
        //remove the query string if needed
        if(strpos($url, "?") !== false) $url = substr($url, 0, strpos($url, "?"));
        
        foreach($this->mappings as $check)
        {
            $matches = array();

            if(preg_match("/^".$check[1]."$/", $url, $matches) === 1)
            {
                $component = $check[2][0];
                $action = $check[2][1];
                $request['urlmapmatch'] = $matches;

                return array($component, "action_".$action);
            }
        }

        return array(false, false);
    }
    /**
     * Get all components
     * 
     * @return array
     */
    public function getComponents()
    {
        return $this->componenets;
    }
    /**
     * Get a component
     * 
     * @param String $name
     * @return array
     */
    public function getComponent($name)
    {
        return $this->componenets[$name];
    }
    /**
     * Get all registered Auth groups
     * 
     * @return array
     */
    public function getAuths()
    {
        return $this->auth;
    }
    /**
     * Check if a component is installed
     * 
     * @param String $component
     * @return boolean
     */
    public function hasComponent($component)
    {
        return array_key_exists($component, $this->componenets);
    }
    /**
     * Check if a page is rgistered
     * 
     * @param String $pageName
     * @param String $pageLocation
     * @return boolean
     */
    public function hasPage($url)
    {
        return isset($this->pages[makeLink($url)]);
    }
    /**
     * Check if a hook is rgistered
     * 
     * @param String $component
     * @param String $action
     * @param String $prepost   pre|post
     * @return boolean
     */
    public function hasHook($component, $action, $prepost)
    {
        $key = "{$component}-{$target}-{$prepost}";

        return isset($this->hooks[$key]);
    }
    /**
     * Check if an user can execute the given action
     * NOTE: if there is no information on the action, valid is assumed
     * 
     * @param String $action
     * @param Array $user
     */
    public function hasAuth($action, $user = null)
    {
        if($user == null && isset($_SESSION['usr']))
            $user = $_SESSION['usr'];
        
        $group = false;
        foreach($this->auth as $authGroup => $actions)
        {
            if(in_array($action, $actions))
            {
                $group = $authGroup;
            }
        }
        //$_SESSION['usr']['privs']
        return ($user && $group && in_array($group, $user['privs']) || $group == null);
    }
}
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

            $result = SQLQuery::doSelect()
            ->table('components')
            ->orderBy("component_name")
            ->exec();

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
     * @param String $location
     * @param mixed $content    Either a string or a callback
     */
    public function registerPage($pageName, $pageLocation, $content)
    {
        $this->pages[$pageLocation."/".$pageName] = array($content, time());
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
     */
    public function getPage($pageName, $pageLocation)
    {
        $page = $this->pages[$pageLocation."/".$pageName];

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
    public function getComponenets()
    {
        return $this->componenets;
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
    public function hasPage($pageName, $pageLocation)
    {
        return isset($this->pages[$pageLocation."/".$pageName]);
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
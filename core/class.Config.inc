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
 * Configuration class, this is used to retrive all the data from seperate config files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Config
{
    private static $instance;
    
    private $data;
    private $componenets;
    private $hooks;
    private $requests;
    
    /**
     * singelton
     *
     * @return Config
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    /**
     * Get something from the config
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     * @return mixed
     */
    public static function get($key, $default, $section = false)
    {
        return self::getInstance()->getItem($key, $default, $section);
    }
    /**
     * constructor
     */
    private function __construct()
    {
        $this->fillData();
    }
    /**
     * Populate the config from the ini files
     */
    private function fillData()
    {
        global $websiteroot;
        $frameworkroot = getFrameworkRoot();
        
        $this->data = array();
        
        if(file_exists($websiteroot.'/config.ini'))
        {
            $this->data = parse_ini_file($websiteroot.'/config.ini', true);
        }
        if(file_exists($frameworkroot.'/core/core.ini'))
        {
            $this->data = array_merge_recursive($this->data, parse_ini_file($frameworkroot.'/core/core.ini', true));
        }
        
        //parse extra ini's
        if(file_exists($websiteroot.'/configs/'))
        {
            $files = scandir($websiteroot.'/configs/');
            
            foreach($files as $file)
            {
                if($file != '.' && $file != ".." && substr($file, strrpos($file, '.')+1) == 'ini')
                {
                    $this->data = array_merge_recursive($this->data, parse_ini_file($websiteroot.'/configs/'.$file, true));
                }
            }
        }
        
        //parse the layout map
        if(file_exists($frameworkroot.'/core/layoutmap.conf'))
        {
            $data = file($frameworkroot.'/core/layoutmap.conf');
            $this->data['layoutmap'] = array();
            
            foreach($data as $line)
            {
                $this->data['layoutmap'][] = explode(" ", trim($line));
            }
        }
        //parse the layout map
        if(file_exists($frameworkroot.'/core/robots.conf'))
        {
            $data = file($frameworkroot.'/core/robots.conf');
            $this->data['robots'] = array();
            
            foreach($data as $line)
            {
                $this->data['robots'][] = preg_quote(trim($line), "/");
            }
        }
        
        $this->data['websiteroot'] = $websiteroot;
    }
    /**
     * Get something from the config
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     * @return mixed
     */
    public function getItem($key, $default, $section = false)
    {
        if($section === false)
        {
            if(array_key_exists($key, $this->data))
            {
                return $this->data[$key];
            }
            else
            {
                return $default;
            }
        }
        else
        {
            if(array_key_exists($section, $this->data) && array_key_exists($key, $this->data[$section]))
            {
                return $this->data[$section][$key];
            }
            else
            {
                return $default;
            }
        }
    }
    /**
     * reload the config values, this should be called when the config is changed during the execution of a page
     */
    public function reload()
    {
        $this->fillData();
    }
    /**
     * Set something for the current request.
     * Note that the value is not saved in a config.
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     */
    public function set($key, $value, $section = false)
    {
        if($section === false)
        {
            $this->data[$key] = $value;
        }
        else
        {
            $this->data[$section][$key] = $value;
        }
    }
    /**
     * Reload the configs that were found in the database. 
     * This should be called when something changes in the components table.
     */
    public function reloadComponenets()
    {
        //clear the cache, seeing as we changed the components
        $cachefile = getFrameworkRoot()."/cache/components.cache";
        if(file_exists($cachefile))
        {
            unlink($cachefile);
        }
        
        $cachefile = getFrameworkRoot()."/cache/hooks.cache";
        if(file_exists($cachefile))
        {
            unlink($cachefile);
        }
        
        $this->componenets = false;
    }
    /**
     * Get an list of all installed components
     *
     * @return Array
     */
    public function getComponenets()
    {
        if(!$this->componenets)
        {
            //check if there is cache file.
            $cachefile = getFrameworkRoot()."/cache/components.cache";
            if(file_exists($cachefile))
            {
                $cache = unserialize(file_get_contents($cachefile));
                
                $this->componenets = $cache[0];
                $this->requests = $cache[1];
            }
            else
            {
                $this->componenets = array();
                $this->requests = array();
                
                $result = SQLQuery::doSelect()
                    ->table('components')
                    ->exec();
                
                while($row = $result->getRow())
                {
                    $this->componenets[$row->component_name] = $row;
                    $requests = explode(';', $row->component_requests);
                    
                    foreach($requests as $request)
                    {
                        if($request != '')
                        {
                            if($row->component_name != $request || count($requests) > 1)
                            {
                                $this->requests[$row->component_name.".".$request] = $row;
                            }
                            else
                            {
                                $this->requests[$request] = $row;
                            }
                        }
                    }
                }
                
                file_put_contents($cachefile, serialize(array($this->componenets, $this->requests)));
            }
        }
        
        return $this->componenets;
    }
    /**
     * Check whenever a component is installed
     *
     * @param string $component
     * @return boolean
     */
    public function isComponentInstalled($component)
    {
        $components = $this->getComponenets();
        
        return isset($component[$component]);
    }
    /**
     * Get the hook for the action, if there is one
     *
     * @param string $component
     * @param string $action        (without prefix)
     * @param string $prepost
     * @return callback to the function or False when there is none
     */
    public function getHook($component, $action, $prepost)
    {
        if(!$this->hooks)
        {
            //check if there is cache file.
            $cachefile = getFrameworkRoot()."/cache/hooks.cache";
            if(file_exists($cachefile))
            {
                $this->hooks = unserialize(file_get_contents($cachefile));
            }
            else
            {
                $this->hooks = array();
                
                $result = SQLQuery::doSelect()
                    ->table('hooks')
                    ->exec();
                
                while($row = $result->getRow())
                {
                    if(!isset($this->hooks[$row->component_target]))
                    {
                        $this->hooks[$row->component_target] = array(
                            "pre" => array(),
                            "post" => array()
                        );
                    }
                    
                    if($row->hook_prepost == "pre")
                    {
                        $this->hooks[$row->hook_target]['pre'][$row->hook_action] = array($row->hook_component, $row->hook_function);
                    }
                    elseif($row->hook_prepost == "post")
                    {
                        $this->hooks[$row->hook_target]['post'][$row->hook_action] = array($row->hook_component, $row->hook_function);
                    }
                }
                file_put_contents($cachefile, serialize($this->hooks));
            }
        }
        
        if(isset($this->hooks[$component][$prepost][$action]))
        {
            return $this->hooks[$component][$prepost][$action];
        }
        else
        {
            return false;
        }
    }
    /**
     * Get an list of all installed requests
     * @deprecated Do not use request, use tables :)
     *
     * @return Array
     */
    public function getRequests()
    {
        if(!$this->componenets)
        {
            $this->getComponenets();
        }
        
        return $this->requests;
    }
    /**
     * Check if a value contains another value. This is done using bitwise comparison
     * 
     * @param int $int
     * @param int $flag
     * @return boolean
     */
    public static function hasFlag($int, $flag)
    {
        return (($int & $flag) == $flag);
    }
}
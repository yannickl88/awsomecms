<?php
/**
* @desc wrapper around the condig file
*/
class Config
{
    private static $instance;
    
    private $data;
    private $componenets;
    private $hooks;
    private $requests;
    
    /**
    * @desc singelton
    */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    
    /**
    * @desc get something from the config
    * 
    * @param $key           String with the config key
    * @param $default       Mixed, default value
    * @param $section       String with the section name (optional)
    */
    public static function get($key, $default, $section = false)
    {
        return self::getInstance()->getItem($key, $default, $section);
    }
    
    /**
    * @desc constructor
    */
    private function __construct()
    {
        $this->fillData();
    }
    
    /**
    * @desc populate the ini array
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
        
        $this->data['websiteroot'] = $websiteroot;
    }
    
    /**
    * @desc get something from the config
    * 
    * @param $key           String with the config key
    * @param $default       Mixed, default value
    * @param $section       String with the section name (optional)
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
    * @desc reload the config values, this should be called when the config is changed during the execution of a page
    * 
    */
    public function reload()
    {
        $this->fillData();
    }
    
    /**
    * @desc set something for the current request
    * 
    * @param $key           String with the config key
    * @param $default       Mixed, default value
    * @param $section       String with the section name (optional)
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
    * @desc get an list of all installed components
    * 
    * @return               Array with components 
    *                        Eg: array(componentname => component_name, component_requests, component_path, component_auth, component_patchlevel)
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
    * @param $component     String with the component name
    * @return               Boolean
    */
    public function isComponentInstalled($component)
    {
        $components = $this->getComponenets();
        
        return isset($component[$component]);
    }
    
    /**
    * @desc get the hook for the action, if there is one
    * 
    * @param $component     String with the component name
    * @param $action        String with the action (without prefix)
    * @param $prepost       String with type pre|post
    * @return               callback to the function or False when there is none
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
    * @desc get an list of all installed requests
    * 
    * @return               Array with requests 
    *                        Eg: array(request => component_name, component_requests, component_path, component_auth, component_patchlevel)
    */
    public function getRequests()
    {
        if(!$this->componenets)
        {
            $this->getComponenets();
        }
        
        return $this->requests;
    }
    
    public static function hasFlag($int, $flag)
    {
        return (($int & $flag) == $flag);
    }
}
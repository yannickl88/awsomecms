<?php
/**
* @desc wrapper around the condig file
*/
class Config
{
    private static $instance;
    
    private $data;
    private $componenets;
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
        
        $this->data = array();
        
        if(file_exists($websiteroot.'/../config.ini'))
        {
            $this->data = parse_ini_file($websiteroot.'/../config.ini', true);
        }
        if(file_exists($websiteroot.'/../core/core.ini'))
        {
            $this->data = array_merge_recursive($this->data, parse_ini_file($websiteroot.'/../core/core.ini', true));
        }
        
        //parse extra ini's
        $files = scandir($websiteroot.'/configs/');
        
        foreach($files as $file)
        {
            if($file != '.' && $file != ".." && substr($file, strrpos($file, '.')+1) == 'ini')
            {
                $this->data = array_merge_recursive($this->data, parse_ini_file($websiteroot.'/configs/'.$file, true));
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
                        $this->requests[$request] = $row;
                    }
                }
            }
        }
        
        return $this->componenets;
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
                        $this->requests[$request] = $row;
                    }
                }
            }
        }
        
        return $this->requests;
    }
    
    public static function hasFlag($int, $flag)
    {
        return (($int & $flag) == $flag);
    }
}
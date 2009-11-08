<?php
import('/core/shared/class.SQL.inc');

/** @desc Exception thrown when a page could not be found */ 
class PageNotFoundExeption extends Exception {}
class NotInstalledException extends Exception {}

/**
* @desc Handel all the database loading and file checking. Also do some util 
* actions with smarty, like loading all the plugins
*/
class SmartyPageLoader
{
    private $timestamps;
    
    /**
    * @desc constructor
    */
    public function __construct()
    {
        $this->timestamps = array();
    }
    
    /**
    * @desc Get the HTML of an page
    * 
    * @param $url           Url of the page, Eg: "/about/me"
    * @param $smarty_obj    Smarty object
    * @return               HTML
    */
    public function getPage($url, &$smarty_obj)
    {
        $forceDefaultLayout = false;
        
        if(preg_match('/(.*)\/$/i', $url) === 1 || $url == '')
        {
            $url .= 'index';
        }
        
        
        try
        {
            if(Config::get('needslogin', false, 'admin') && $smarty_obj->get_template_vars('isADMIN') === true && !($_SESSION['usr']['auth']['admin'] === true))
            {
                $url = '/'.Config::get('login', '', 'admin');
            }
            $page = $smarty_obj->fetch("scf:{$url}");
        }
        catch(PageNotFoundExeption $e)
        {
            //set header
            header("HTTP/1.0 404 Not Found");
            
            try
            {
                $page = $smarty_obj->fetch("404.tpl");
            }
            catch(PageNotFoundExeption $e)
            {
                $page = "404 - PAGE NOT FOUND";
            }
        }
        
        if($smarty_obj->get_template_vars('LAYOUT')) //check if layout was set from withing a page
        {
            $template = $smarty_obj->get_template_vars('LAYOUT');
            
            if($template == "json")
            {
                header('Content-type: application/json');
                return $page;
            }
            else
            {
                return $smarty_obj->fetch("{$template}.tpl");
            }
        }
        elseif(file_exists("{$smarty_obj->template_dir}/layout.tpl")) //check for layout file
        {
            $smarty_obj->assign('content', $page);
            
            if(file_exists("{$smarty_obj->template_dir}/{$url}.tpl"))
            {
                $smarty_obj->assign('content', $smarty_obj->fetch("{$url}.tpl"));
            }
            
            $layoutmap = Config::get('layoutmap', array());
            
            foreach($layoutmap as $map)
            {
                if(preg_match("/^".$map[0]."$/", "/".$url) > 0)
                {
                    return $smarty_obj->fetch($map[1]);
                }
            }
            
            //no layout fond so we fallback to the default
            return $smarty_obj->fetch("layout.tpl");
        }
        else
        {
            return $page;
        }
    }
    
    /**
    * @desc load the template from the database
    * 
    * @param $tpl_name      Template name 
    * @param $tpl_source    Template content, should be set
    * @param $smarty_obj    Smarty object
    * @return               Boolean
    */
    public function getTemplate($tpl_name, &$tpl_source, &$smarty_obj)
    {
        $pageArray = explode('/', $tpl_name);
        $pageName = array_pop($pageArray);
        $pageLocation = implode('/', $pageArray);
        
        if($pageLocation != '')
        {
            $pageLocation .= '/';
        }
        
        $result = SQLQuery::doSelect()
                    ->table('pages')
                    ->where('page_name', $pageName)
                    ->where('page_location', '/'.$pageLocation)
                    ->exec();
        
        if($result->count() > 0) 
        {
            // cache template timestamp
            $this->timestamps[$tpl_name] = strtotime($result->record->page_timestamp);
            
            $smarty_obj->assign('PAGE', $result->record);
            
            $tpl_source = $result->record->page_template;
            return true;
        } 
        else 
        {
            throw new PageNotFoundExeption("Page not found '{$tpl_name}'");
            return false;
        }
    }
    
    /**
    * @desc check the timestamp for an template
    * 
    * @param $tpl_name      Template name 
    * @param $tpl_timestamp Template timestamp, should be set
    * @param $smarty_obj    Smarty object
    * @return               Boolean
    */
    public function getTimestamp($tpl_name, &$tpl_timestamp, &$smarty_obj)
    {
        //check cache
        if(array_key_exists($tpl_name, $this->timestamps))
        {
            $tpl_timestamp = $this->timestamps[$tpl_name];
            return true;
        }
        else
        {
            $pageArray = explode('/', $tpl_name);
            $pageName = array_pop($pageArray);
            $pageLocation = implode('/', $pageArray);
            
            if($pageLocation != '')
            {
                $pageLocation .= '/';
            }
            
            $result = SQLQuery::doSelect()
                    ->table('pages')
                    ->where('page_name', $pageName)
                    ->where('page_location', '/'.$pageLocation)
                    ->exec();
            
            if($result->count() > 0)
            {
                // cache template timestamp
                $this->timestamps[$tpl_name] = strtotime($result->record->page_timestamp);
                
                $tpl_timestamp = $this->timestamps[$tpl_name];
                return true;
            } 
            else 
            {
                throw new PageNotFoundExeption("Page not found '{$tpl_name}'");
                return false;
            } 
        }
        
    }

    /**
    * @desc check if the template is secure
    * 
    * @param $tpl_name      Template name 
    * @param $smarty_obj    Smarty object
    * @return               Boolean
    */
    public function getSecure($tpl_name, &$smarty_obj)
    {
        return true;
    }

    /**
    * @desc check if the template is trusted
    * 
    * @param $tpl_name      Template name 
    * @param $smarty_obj    Smarty object
    * @return               Boolean
    */
    public function getTrusted($tpl_name, &$smarty_obj)
    {
        
    }
    
    /**
    * @desc register all the smarty extentions from the components, 
    * see http://smarty.net/manual/en/plugins.php
    * 
    * @param $smarty_obj    Smarty object
    */
    public function registerModulePlugins(&$smarty_obj)
    {
        try
        {
            $modules = Config::getInstance()->getComponenets();
        }
        catch(SQLExeption $e)
        {
            // something went wrong getting the components, must not be installed
            throw new NotInstalledException();
        }
        
        if(count($modules) == 0)
        {
            throw new NotInstalledException();
        }
        
        foreach($modules as $module => $component)
        {
            $pluginDir = $component->component_path.'/plugins/';
            
            if(file_exists($pluginDir))
            {
                $plugins = scandir($pluginDir);
                
                foreach($plugins as $plugin)
                {
                    $matches = array();
                    
                    if(preg_match('/^function\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_function_".$matches[1]);
                    }
                    else if(preg_match('/^modifier\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_modifier($matches[1], "smarty_modifier_".$matches[1]);
                    }
                    else if(preg_match('/^block\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_block_".$matches[1]);
                    }
                    else if(preg_match('/^compiler\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_compiler_".$matches[1]);
                    }
                    else if(preg_match('/^prefilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_prefilter_".$matches[1]);
                    }
                    else if(preg_match('/^postfilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_postfilter_".$matches[1]);
                    }
                    else if(preg_match('/^outputfilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_outputfilter_".$matches[1]);
                    }
                    else if(preg_match('/^resource\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_resource_".$matches[1]);
                    }
                    else if(preg_match('/^insert\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register_function($matches[1], "smarty_insert_".$matches[1]);
                    }
                }
            }
        }
    }
}
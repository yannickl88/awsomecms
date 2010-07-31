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
import("libs/class.BBCodeParser.inc");

/** 
 * Exception thrown when a page could not be found 
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class PageNotFoundExeption extends Exception {}
/**
 * Exception to be triggered when the framework is not properly installed (No components)
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class NotInstalledException extends Exception {}

/**
 * Handel all the database loading and file checking. Also do some util
 * actions with smarty, like loading all the plugins
 */
class SmartyPageLoader
{
    private $timestamps;
    
    /**
     * constructor
     */
    public function __construct()
    {
        $this->timestamps = array();
    }
    /**
     * Get the HTML of an page
     *
     * @param string $url           Url of the page, Eg: "/about/me"
     * @param Smarty $smarty_obj    Smarty object
     * @return string
     */
    public function getPage($url, $smarty_obj)
    {
        $forceDefaultLayout = false;
        
        //check if there is an install DIR
        $note = Notifications::getInstance();
        if(file_exists(Config::get("websiteroot", "/")."/install") && Config::get("doNotCheckInstall", true))
        {
            if(!$note->hasNotification("installnotremoved"))
            {
                $text = "There still seems to be an install directory, keeping it there is a <b>very high security risk</b>. <a href='#' onclick='return deleteInstall();'>Click here to remove the Install directories.</a>";
                $text .= "<script type=\"text/javascript\">";
                $text .= "function deleteInstall() { return getComponent(\"core\", \"deleteinstall\", null, function(data) { $('#notification_installnotremoved').remove(); }); }";
                $text .= "</script>";
                $note->addNotification($text, "*", "installnotremoved");
            }
        }
        else
        {
            $note->removeNotification("*","installnotremoved");
        }
        
        if(preg_match('/(.*)\/$/i', $url) === 1 || $url == '')
        {
            $url .= 'index';
        }
        
        try
        {
            if(Config::get('needslogin', false, 'admin') && $smarty_obj->getTemplateVars('isADMIN') === true && !(isset($_SESSION['usr']) && $_SESSION['usr']['auth']['admin'] === true))
            {
                $url = Config::get('login', '', 'admin');
                $smarty_obj->assign("URL", $url); //reassign the URL var
            }
            
            $page = $smarty_obj->fetch("scf:{$url}");
        }
        catch(PageNotFoundExeption $e)
        {
            $page = null;
        }
        
        return $this->getRenderedPage($page, $url, $smarty_obj);
    }
    /**
     * Get the page with the rendered page with the layout wraped around it
     * 
     * @param string $content
     * @param string $url
     * @param Smarty $smarty_obj
     * @return string
     */
    public function getRenderedPage($content, $url, $smarty_obj)
    {
        if($content == null)
        {
            //set header
            header("HTTP/1.0 404 Not Found");
            
            try
            {
                $content = $smarty_obj->fetch("404.tpl");
            }
            catch(PageNotFoundExeption $e)
            {
                $content = "404 - PAGE NOT FOUND";
            }
        }
        
        if($smarty_obj->getTemplateVars('LAYOUT') != null) //check if layout was set from withing a page
        {
            $template = $smarty_obj->getTemplateVars('LAYOUT');
            
            if($template == "json")
            {
                header('Content-type: application/json');
                return $content;
            }
            else
            {
                return $smarty_obj->fetch("{$template}.tpl");
            }
        }
        elseif(file_exists("{$smarty_obj->template_dir}/layout.tpl")) //check for layout file
        {
            $smarty_obj->assign('content', $content);
            
            if(file_exists("{$smarty_obj->template_dir}/{$url}.tpl"))
            {
                $smarty_obj->assign('content', $smarty_obj->fetch("{$url}.tpl"));
            }
            
            $robotmap = Config::get('robots', array());
            
            foreach($robotmap as $map)
            {
                if(preg_match("/".$map."/i", $_SERVER['HTTP_USER_AGENT']) > 0)
                {
                    Debugger::getInstance()->setRobot($map);
                    return $smarty_obj->fetch("robot_layout.tpl");
                }
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
            return $content;
        }
    }
    /**
     * load the template from the database
     *
     * @param string $tpl_name      Template name
     * @param string $tpl_source    Template content, should be set
     * @param Smarty $smarty_obj    Smarty object
     * @return boolean
     */
    public function getTemplate($tpl_name, $tpl_source, $smarty_obj)
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
            
            if(!$result->record->page_html)
                $tpl_source = BBCodeParser::parse($result->record->page_template);
            else
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
     * check the timestamp for an template
     *
     * @param string $tpl_name      Template name
     * @param string $tpl_timestamp Template timestamp, should be set
     * @param Smarty $smarty_obj    Smarty object
     * @return Boolean
     */
    public function getTimestamp($tpl_name, $tpl_timestamp, $smarty_obj)
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
     * check if the template is secure
     *
     * @param $tpl_name      Template name
     * @param $smarty_obj    Smarty object
     * @return               Boolean
     */
    public function getSecure($tpl_name, $smarty_obj)
    {
        return true;
    }
    /**
     * check if the template is trusted
     *
     * @param string $tpl_name      Template name
     * @param Smarty $smarty_obj    Smarty object
     * @return Boolean
     */
    public function getTrusted($tpl_name, $smarty_obj)
    {
        return true;
    }
    /**
     * register all the smarty extentions from the components,
     * see http://smarty.net/manual/en/plugins.php
     *
     * @param Smarty $smarty_obj    Smarty object
     */
    public function registerModulePlugins($smarty_obj)
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
                        $smarty_obj->register->templateFunction($matches[1], "smarty_function_".$matches[1]);
                    }
                    else if(preg_match('/^modifier\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->modifier($matches[1], "smarty_modifier_".$matches[1]);
                    }
                    else if(preg_match('/^block\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->block($matches[1], "smarty_block_".$matches[1]);
                    }
                    else if(preg_match('/^compiler\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->compilerFunction($matches[1], "smarty_compiler_".$matches[1]);
                    }
                    else if(preg_match('/^prefilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->preFilter($matches[1], "smarty_prefilter_".$matches[1]);
                    }
                    else if(preg_match('/^postfilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->postFilter($matches[1], "smarty_postfilter_".$matches[1]);
                    }
                    else if(preg_match('/^outputfilter\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->outputFilter($matches[1], "smarty_outputfilter_".$matches[1]);
                    }
                    else if(preg_match('/^resource\.(.*)\.php$/', $plugin, $matches))
                    {
                        require_once $pluginDir.'/'.$plugin;
                        $smarty_obj->register->resource($matches[1], "smarty_resource_".$matches[1]);
                    }
                }
            }
        }
    }
}
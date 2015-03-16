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
class SmartyPageLoader extends Smarty_Resource_Custom
{
    private $timestamps;
    private $smarty;

    /**
     * constructor
     */
    public function __construct($smarty)
    {
        $this->timestamps = array();
        $this->smarty = $smarty;
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
        elseif($smarty_obj->templateExists("layout.tpl")) //check for layout file
        {
            $smarty_obj->assign('content', $content);

            if(file_exists("{$smarty_obj->template_dir[0]}/{$url}.tpl"))
            {
                $smarty_obj->assign('content', $smarty_obj->fetch("{$url}.tpl"));
            }

            $robotmap = Config::get('robots', array());

            foreach($robotmap as $map)
            {
                if(preg_match("/".$map."/i", $_SERVER['HTTP_USER_AGENT']) > 0)
                {
                    if(file_exists("{$smarty_obj->template_dir}/robot_layout.tpl")) //check for layout file
                    {
                        Debugger::getInstance()->setRobot($map);
                        return $smarty_obj->fetch("robot_layout.tpl");
                    }
                    else
                    {
                        Debugger::getInstance()->error("Cannot find the required 'robot_layout.tpl' file.");
                    }
                }
            }

            $layoutmap = Config::get('layoutmap', array());

            foreach($layoutmap as $map)
            {
                if(preg_match("/^".$map[0]."$/", $url) > 0)
                {
                    return $smarty_obj->fetch($map[1]);
                }
            }

            //no layout fond so we fallback to the default
            if(Config::get("privLayout", true, "debug") && isset($_GET["debug_public"]) && $_GET["debug_public"] != 1 && in_array($_SERVER['REMOTE_ADDR'], Config::get("privLayout_ip", array(), "debug"))) {
                return $smarty_obj->fetch("priv_layout.tpl");
            }
            else
            {
                return $smarty_obj->fetch("layout.tpl");
            }
        }
        else
        {
            return $content;
        }
    }
    /**
     * load the template from the database
     *
     * @param string  $name    template name
     * @param string  &$source template source
     * @param integer &$mtime  template modification timestamp (epoch)
     * @return boolean
     */
    public function fetch($name, &$source, &$mtime)
    {
        //check registered pages first
        if(RegisterManager::getInstance()->hasPage($name))
        {
            $page = RegisterManager::getInstance()->getPage($name);

            $source = $page[0];
        }
        else
        {
            $pageArray = explode('/', $name);
            $pageName = array_pop($pageArray);
            $pageLocation = implode('/', $pageArray);

            if($pageLocation != "") $pageLocation .= "/";

            $result = SQLQuery::doSelect()
            ->table('pages')
            ->where('page_name', $pageName)
            ->where('page_location', '/'.$pageLocation)
            ->exec();

            if($result->count() > 0)
            {
                // cache template timestamp
                $this->timestamps[$name] = strtotime($result->record->page_timestamp);

                $result->record->page_template = unserialize($result->record->page_template);

                $this->smarty->assign('PAGE', $result->record);

                $html = $result->record->page_template[getLang()];

                $mtime = $this->timestamps[$tpl_name];

                if(!$result->record->page_html)
                    $source = BBCodeParser::parse($html);
                else
                    $source = $html;
            }
            else
            {
                throw new PageNotFoundExeption("Page not found '{$name}'");
            }
        }
    }
    /**
     * check the timestamp for an template
     *
     * @param string $name template name
     * @return integer|boolean timestamp (epoch) the template was modified, or false if not found
     */
    public function fetchTimestamp($name)
    {
        //check cache
        if(array_key_exists($name, $this->timestamps))
        {
            return $this->timestamps[$name];
        }
        else
        {
            //check registered pages first
            if(RegisterManager::getInstance()->hasPage($name))
            {
                $page = RegisterManager::getInstance()->getPage($name);

                return $page[1];
            }
            else
            {
                $pageArray = explode('/', $name);
                $pageName = array_pop($pageArray);
                $pageLocation = implode('/', $pageArray);

                if($pageLocation != "") $pageLocation .= "/";

                $result = SQLQuery::doSelect()
                ->table('pages')
                ->where('page_name', $pageName)
                ->where('page_location', '/'.$pageLocation)
                ->exec();

                if($result->count() > 0)
                {
                    $this->smarty->assign('PAGE', $result->record);
                    // cache template timestamp
                    $this->timestamps[$name] = strtotime($result->record->page_timestamp);

                    return $this->timestamps[$name];
                }
                else
                {
                    throw new PageNotFoundExeption("Page not found '{$name}'");
                    return false;
                }
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
     * Set the background image
     *
     * @param $row
     * @param Smarty $smarty_obj
     */
    public function assignBackgroundImage($row, $smarty_obj)
    {
        if($smarty_obj->getTemplateVars('BGIMG') != null)
            return;

        if(!empty($row->page_slideshow) && empty($row->page_bg))
        {
            $fileComp = Component::init("file");
            $loc = explode("/", $row->page_slideshow);
            array_shift($loc);
            array_shift($loc);
            $type = array_shift($loc);

            $file_loc = '/'.implode("/", $loc);
            $images = array();

            $rows = Table::init("file.files")
                ->setRequest((object) array("file_location" => $file_loc))
                ->doSelect();

            while($row = $rows->getRow())
            {
                if($fileComp->getType($row->file_data->type) == $type)
                {
                    $images[] = $row->file_data->url;
                }
            }

            $smarty_obj->assign('BGIMG', $images[rand(0, count($images) - 1)]);
        }
        else
        {
            $smarty_obj->assign('BGIMG', unserialize($row->page_bg)->url);
        }
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
            $modules = RegisterManager::getInstance()->getComponents();
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

        //Load components
        foreach($modules as $module => $component)
        {
            $smarty_obj->addPluginsDir($component->component_path.'/plugins/');
        }

        //Load the extentions
        if(file_exists(getFrameworkRoot()."/extentions/"))
        {
            $extentions = scandir(getFrameworkRoot()."/extentions/");

            foreach($extentions as $extention)
            {
                $matches = array();
                if($extention != "." && $extention != ".." && preg_match('/^(.*)\.ext\.inc$/', $extention, $matches) === 1)
                {
                    try
                    {
                        $ext = Extention::init($matches[1]);
                        $ext->extend($smarty_obj);
                    }
                    catch(Exception $e) { throw $e; }
                }
            }
        }
    }
}
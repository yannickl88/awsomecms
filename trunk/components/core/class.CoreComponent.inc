<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');
import('/components/core/util/class.InstallHelper.inc');

/**
 * Core component class, this includes all the logic for the core actions
 */
class CoreComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('core');
    }
    /**
     * Index action, this is used when a page is shown by the cms
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_index($smarty, $smartyLoader, $request)
    {
        //check for logout
        if(isset($_GET['logout']) && $_GET['logout'] === 'true')
        {
            $authComponent = Component::init(Config::get("component", "core", "auth"));
            $authComponent->auth_logout();
            $this->redirect("/");
            Controller::getInstance()->redirect();
        }
        
        $smarty->register_resource("scf", array(
            array($smartyLoader, "getTemplate"),
            array($smartyLoader, "getTimestamp"),
            array($smartyLoader, "getSecure"),
            array($smartyLoader, "getTrusted"),
        ));

        //Handle URL
        echo $smartyLoader->getPage($_GET['url'], $smarty);
    }
    /**
     * Session action, this is used to set some data for the templates, like if a user is logged in or if the page is part of the admin section.
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_session($smarty, $smartyLoader, $request)
    {
        if(!isset($_GET['url']))
        {
            $_GET['url'] = "";
        }
        $smarty->assign("URL", $_GET['url']);
        $smarty->assign("isADMIN", (preg_match('/^'.preg_quote(Config::get('location', 'admin/', 'admin'), '/').'(.*?)/si', $_GET['url'])) === 1);
        if(!isset($_SESSION['usr']))
        {
            $smarty->assign("USR", null);
        }
        else
        {
            $smarty->assign("USR", $_SESSION['usr']);
        }
        $smarty->assign("HASDOC", file_exists(Config::get("websiteroot", "/")."/doc.php"));
    }
    /**
     * Layout action, this is used for the layout mapping page.
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader    * @param array $request
     */
    public function action_layout($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $data = "";
            
            foreach($request['map'] as $map)
            {
                if($data != "")
                {
                    $data .= "\n";
                }
                
                $data .= trim(urldecode($map));
            }
            
            file_put_contents(getFrameworkRoot().'/core/layoutmap.conf', $data);
            $this->redirect("/".Config::get('corelayout', 'corelayout', 'admin'));
        }
        else
        {
            $smarty->assign("maping", Config::get("layoutmap", array()));
        }
    }
    /**
     * Install action, this is used for the update manager.
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_install($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $installer = new InstallHelper();
            
            if(isset($request['installComponent']))
            {
                $result = $installer->installComponent($request['installComponent'], ($request['installAdmin'] == 'true'));
                
                echo json_decode($result);
                
                Config::getInstance()->reloadComponenets();
                
                return;
            }
        }
        echo "false";
    }
    /**
     * Update action, this is used for the update manager.
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_update($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $updateURL = Config::get("updateurl", "", "update");
            $versions = json_decode(file_get_contents("{$updateURL}/version.json"));
            $installer = new InstallHelper();
            
            if($request['framework'] == 1)
            {
                //framework?
                if(!(file_exists(getFrameworkRoot()."/version.info") && trim(file_get_contents(Config::get("websiteroot")."/version.info")) == trim($versions->framework)))
                {
                    $installer->downloadAndInstall("{$updateURL}/framework.zip");
                    file_put_contents(getFrameworkRoot()."/version.info", trim($versions->framework));
                }
                
                echo "true";
                return;
            }
            
            if(isset($request['updateComponent']))
            {
                //components
                $components = Config::getInstance()->getComponenets();
                $component = $components[$request['updateComponent']];
                $component_name = $component->component_name;
                
                if(!empty($component->component_updateurl))
                {
                    $extVersions = json_decode(file_get_contents("{$component->component_updateurl}/version.json"));
                    
                    $version = trim($extVersions->$component_name);
                }
                else
                {
                    $version = trim($versions->components->$component_name);
                }
                
                if($version > $component->component_version)
                {
                    //is there an update URL set?
                    if(!empty($component->component_updateurl))
                    {
                        if(!$installer->downloadAndInstall("{$component->component_updateurl}/{$component_name}.zip", $component->component_path))
                        {
                            die("false");
                        }
                    }
                    else
                    {
                        if(!$installer->downloadAndInstall("{$updateURL}/components/{$component_name}.zip", $component->component_path))
                        {
                            die("false");
                        }
                    }
                    
                    Table::init('core.components')->setRecord(array(
                        'component_version'=> trim($version),
                        'component_name' => $component->component_name
                    ))
                    ->doUpdate();
                }
                
                //we want to run this no matter what, this will allow us to manually update the files and run this anyways
                //now to run the patches
                $installer->runPatches($component_name);
                
                //copy any new stuff from the install folder
                $installer->rcopy($component->component_path."/install", Config::get('websiteroot'), false);
                
                echo "true";
                
                Config::getInstance()->reloadComponenets();
                
                return;
            }
        }
        echo "false";
    }
    /**
     * Components action, this is used for the compontent settings page
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_components($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $components = Config::getComponenets();
            
            foreach($components as $component)
            {
                $key = "auth_".$component->component_name;
                
                if(!isset($request[$key]))
                {
                    $total = 15;
                }
                else
                {
                    $total = 15 - array_sum($request[$key]);
                }
                
                if($total != $component->component_auth)
                {
                    Table::init('core.components')
                        ->setRecord((object) array(
                            "component_auth" => $total, 
                            "component_name" => $component->component_name
                        ))
                        ->doUpdate();
                }
            }
            
            Config::getInstance()->reloadComponenets();
            $this->redirect("/".Config::get('corecomponents', 'corecomponents', 'admin'));
        }
        else
        {
            $updateURL = Config::get("updateurl", "", "update");
            $versions = new stdClass();
            
            if(file_exists("{$updateURL}/version.json"))
            {
                //fetch the current version info
                $versions = json_decode(file_get_contents("{$updateURL}/version.json"));
            }
            //check if the framework is up to date
            $smarty->assign("frameworkU2D", (file_exists(getFrameworkRoot()."/version.info") && isset($versions->framework) && trim(file_get_contents(getFrameworkRoot()."/version.info")) == trim($versions->framework)));
            
            $components = Config::getInstance()->getComponenets();
            
            foreach($components as $component)
            {
                $component_name = $component->component_name;
                
                //is there an update URL set?
                if(!empty($component->component_updateurl))
                {
                    $extVersions = json_decode(file_get_contents("{$component->component_updateurl}/version.json"));
                    
                    if($extVersions === false || $extVersions === null)
                    {
                        $components[$component->component_name]->U2D = "???";
                    }
                    else
                    {
                        $components[$component->component_name]->U2D = (isset($versions->components->$component_name) && trim($extVersions->$component_name) == $component->component_version);
                    }
                }
                else
                {
                    $components[$component->component_name]->U2D = (isset($versions->components->$component_name) && trim($versions->components->$component_name) == $component->component_version);
                }
                
                $components[$component->component_name]->info = parseInfoFile($component->component_path."/{$component_name}.info", $component_name);
            }
            
            //search for non installed components
            $componentsDir = getFrameworkRoot()."/components";
            $uninstalledComponents = array();
            
            if(file_exists($componentsDir))
            {
                $folders = scandir($componentsDir);
                
                foreach($folders as $folder)
                {
                    $componentDir = $componentsDir."/".$folder;
                    
                    if($folder != "." && $folder != '..' && is_dir($componentDir) && $folder != ".svn")
                    {
                        //check for a info file, if so then it must be component
                        if(!isset($components[$folder]) && file_exists($componentDir."/info.ini"))
                        {
                            $component = new stdClass();
                            $component->component_name = $folder;
                            $component->info = parseInfoFile($componentDir."/{$folder}.info", $folder);
                            
                            $uninstalledComponents[$component->component_name] = $component;
                        }
                    }
                }
            }
            
            $smarty->assign("components", $components);
            $smarty->assign("uninstalledComponents", $uninstalledComponents);
            $smarty->assign("auth", array(
                "auth_select" => Config::get("auth_select", 1, "core"),
                "auth_delete" => Config::get("auth_delete", 2, "core"),
                "auth_insert" => Config::get("auth_insert", 4, "core"),
                "auth_update" => Config::get("auth_update", 8, "core"),
            ));
        }
    }
    public function action_auth($smarty, $smartyLoader, $request)
    {
        $components = Config::getInstance()->getComponenets();
        
        if($request['method'] == 'post')
        {
            Table::init("core.auth")
                ->doDelete();
            
            foreach($components as $component)
            {
                $actions = getActions($component->component_name);
                
                foreach($actions as $key => $action)
                {
                    $key = str_replace(".", "_", $key);
                    
                    if($request[$key])
                    {
                        foreach($request[$key] as $priv)
                        {
                            Table::init("core.auth")
                                ->setRecord((object) array("auth_component" => $component->component_name, "auth_action" => $action, "auth_privilage" => $priv))
                                ->doInsert();
                        }
                    }
                }
            }
            $this->redirect("/".Config::get('coreauth', 'coreauth', 'admin'));
        }
        else
        {
            $exceptions = Controller::getInstance()->getActionAuthExceptions();
            
            $values = array();
            $auths = Table::init("core.auth")
                ->doSelect()
                ->getRows();
            
            foreach($auths as $auth)
            {
                if(!isset($values[$auth->auth_component.".".$auth->auth_action]))
                {
                    $values[$auth->auth_component.".".$auth->auth_action] = array();
                }
                
                $values[$auth->auth_component.".".$auth->auth_action][] = $auth->auth_privilage;
            }
            foreach($components as $component)
            {
                $component->actions = getActions($component->component_name);
                $component->actions_length = count($component->actions);
            }
            
            //TODO: fix depedancie
            $smarty->assign("privileges", Table::init('users.privileges')->doSelect()->getRows());
            $smarty->assign("exceptions", $exceptions);
            $smarty->assign("components", $components);
            $smarty->assign("record", $values);
        }
    }
    public function action_deleteinstall($smarty, $smartyLoader, $request)
    {
        $websiteRoot = Config::get("websiteroot");
        $installHelper = new InstallHelper();
        
        $installHelper->remove($websiteRoot."/install/");
        $installHelper->remove($websiteRoot."/install.php");
        
        //remove the notification
        Notifications::getInstance()->removeNotification("*","installnotremoved");
        
        $this->ajax(true);
    }
    public function action_getform($smarty, $smartyLoader, $request)
    {
        $html = smarty_function_form($request["data"], $smarty);
        $this->ajax(array("html" => rawurlencode($html)));
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("system", ""));
        $menu->addChild(new MenuItem("layoutmaps", Config::get('corelayout', 'corelayout', 'admin'), "system"));
        $menu->addChild(new MenuItem("components", Config::get('corecomponents', 'corecomponents', 'admin'), "system"));
        $menu->addChild(new MenuItem("auth", Config::get('coreauth', 'coreauth', 'admin'), "system"));
    }
}
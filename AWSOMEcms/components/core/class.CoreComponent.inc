<?php
import('/core/shared/class.Component.inc');
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
        if($_GET['logout'] === 'true')
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
        $smarty->assign("URL", $_GET['url']);
        $smarty->assign("isADMIN", (preg_match('/^'.preg_quote(Config::get('location', 'admin/', 'admin'), '/').'(.*?)/si', $_GET['url'])) === 1);
        $smarty->assign("USR", $_SESSION['usr']);
    }
    
    /**
    * Layout action, this is used for the layout mapping page.
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader    * @param array $request
    */
    public function action_layout($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
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
    * Update action, this is used for the update manager.
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
    public function action_update($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
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
                    
                    Request::init('core')->doUpdate(array(
                        'component_version'=> trim($version),
                        'component_name' => $component->component_name
                    ));
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
            
            echo "false";
        }
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
        if($request['methode'] == 'post')
        {
            $components = Config::getComponenets();
            
            foreach($components as $component)
            {
                $key = "auth_".$component->component_name;
                
                if(isset($request[$key]) || (!isset($request[$key]) && $component->component_auth < 15))
                {
                    $total = 15 - array_sum($request[$key]);
                    Request::init('core')->doUpdate(array("component_auth" => $total, "component_name" => $component->component_name));
                }
            }
            
            Config::getInstance()->reloadComponenets();
            $this->redirect("/".Config::get('corecomponents', 'corecomponents', 'admin'));
        }
        else
        {
            $updateURL = Config::get("updateurl", "", "update");
            
            //fetch the current version info
            $versions = json_decode(file_get_contents("{$updateURL}/version.json"));
            
            //check if the framework is up to date
            $smarty->assign("frameworkU2D", (file_exists(getFrameworkRoot()."/version.info") && trim(file_get_contents(getFrameworkRoot()."/version.info")) == trim($versions->framework)));
            
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
                        $components[$component->component_name]->U2D = (trim($extVersions->$component_name) == $component->component_version);
                    }
                }
                else
                {
                    $components[$component->component_name]->U2D = (trim($versions->components->$component_name) == $component->component_version);
                }
                
                $components[$component->component_name]->info = parseInfoFile($component->component_path."/{$component_name}.info", $component_name);
            }
            
            $smarty->assign("components", $components);
            
            $smarty->assign("auth", array(
                "auth_select" => Config::get("auth_select", 1, "core"),
                "auth_delete" => Config::get("auth_delete", 2, "core"),
                "auth_insert" => Config::get("auth_insert", 4, "core"),
                "auth_update" => Config::get("auth_update", 8, "core"),
            ));
        }
    }
    
    /**
    * registering the core menu items
    * 
    * @param Menu $menu
    */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("system", "", ""));
        $menu->addChild(new MenuItem("layoutmaps", "Layout Mapping", Config::get('corelayout', 'corelayout', 'admin'), "system"));
        $menu->addChild(new MenuItem("components", "Components", Config::get('corecomponents', 'corecomponents', 'admin'), "system"));
    }
}
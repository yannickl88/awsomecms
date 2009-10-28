<?php
import('/core/shared/class.Component.inc');

class CoreComponent extends Component
{
    public function __construct()
    {
        parent::__construct('core');
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        $smarty->register_resource("scf", array(
            array($smartyLoader, "getTemplate"),
            array($smartyLoader, "getTimestamp"),
            array($smartyLoader, "getSecure"),
            array($smartyLoader, "getTrusted"),
        ));

        //Handle URL
        echo $smartyLoader->getPage($_GET['url'], $smarty);
    }
    
    public function action_session($smarty, $smartyLoader, $request)
    {
        $smarty->assign("URL", $_GET['url']);
        $smarty->assign("isADMIN", (preg_match('/^'.preg_quote(Config::get('location', 'admin/', 'admin'), '/').'(.*?)/si', $_GET['url'])) === 1);
        $smarty->assign("USR", $_SESSION['usr']);
    }
    
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
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("system", "", ""));
        $menu->addChild(new MenuItem("layoutmaps", "Layout Mapping", Config::get('corelayout', 'corelayout', 'admin'), "system"));
    }
}
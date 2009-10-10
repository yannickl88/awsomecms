<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.RequestComponent.inc';

class CoreComponent extends RequestComponent
{
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
}
<?php
session_start();

global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__);

try
{
    require_once '../core/init.inc';

    $config = Config::getInstance();

    //check if incomming request if directed at the proxy
    if($config->get("enabled", false, 'proxy') == "1" && 'http://' . $_SERVER['SERVER_NAME'] == $config->get("proxy", 'www.google.com', 'proxy'))
    {
        import('/core/shared/class.RequestComponent.inc');
        
        RequestComponent::handelRequest();
    }
    
    $smarty = new Smarty();
    $smarty->assign("SCRIPT_START", $start);

    $smarty->compile_check = true;
    $smarty->debugging = false;

    $smarty->template_dir = $websiteroot.'/templates';
    $smarty->compile_dir = $websiteroot.'/templates_c';
    $smarty->cache_dir = $websiteroot.'/../cache';


    $smartyLoader = new SmartyPageLoader();
    $smartyLoader->registerModulePlugins($smarty);

    $controller = Controller::getInstance();

    $controller->setData($smarty, $smartyLoader);
    $controller->dispatch();
}
catch(NotInstalledException $e)
{
    if(file_exists('install.php') && file_exists('install'))
    {
        header("Location: /install.php");
        exit;
    }
    else
    {
        header("HTTP/1.0 500 Internal Server Error");
        echo "Something went wrong, please contact the server administrator.";
        
        if(Config::get('debug', 0, 'debug') >= 1)
        {
            echo "<br /><br /><b>".get_class($e)."</b>: ".$e->getMessage()."<pre>".$e->getTraceAsString()."</pre>";
        }
    }
}
catch(Exception $e)
{
    header("HTTP/1.0 500 Internal Server Error");
    echo "Something went wrong, please contact the server administrator.";
    
    if(Config::get('debug', 0, 'debug') >= 1)
    {
        echo "<br /><br /><b>".get_class($e)."</b>: ".$e->getMessage()."<pre>".$e->getTraceAsString()."</pre>";
    }
}

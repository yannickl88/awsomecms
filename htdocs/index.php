<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 * 
 * @version $Revision$
 */
session_start();

global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__);

try
{
    require_once '../core/init.inc';

    $config = Config::getInstance();
    
    $smarty = new Smarty();
    $smarty->assign("SCRIPT_START", $start);

    $smarty->compile_check = true;
    $smarty->debugging = false;

    $smarty->template_dir = $websiteroot.'/templates';
    $smarty->compile_dir = $websiteroot.'/templates_c';
    $smarty->compile_id = getLang();
    $smarty->cache_dir = $websiteroot.'/../cache';
    $smarty->assign("LANG", $smarty->compile_id);

    $smartyLoader = new SmartyPageLoader();
    $smartyLoader->registerModulePlugins($smarty);

    Controller::getInstance()
        ->dispatch($smarty, $smartyLoader);
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
catch(ForbiddenException $e)
{
    header("HTTP/1.0 403 Forbidden");
    echo "You do not have access to preform this action";
    
    if(Config::get('debug', 0, 'debug') >= 1)
    {
        echo "<br /><br /><b>".get_class($e)."</b>: ".$e->getMessage()."<pre>".$e->getTraceAsString()."</pre>";
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

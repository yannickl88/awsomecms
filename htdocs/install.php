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
require_once __DIR__ . '/../vendor/autoload.php';
session_start();

if(!file_exists('./install/')) //can we even do an install?
{
    header("Location: /");
    exit;
}

global $websiteroot;
$websiteroot = dirname(__FILE__);

include_once '../core/init.inc';

/**
 * Install the database
 *
 * @return array()      [boolean, errormessage]
 */
function installDB()
{
    //prepare the database
    $dbhost = $_POST['dbhost'];
    $database = $_POST['dbdatabase'];
    $username = $_POST['dbusername'];
    $password = $_POST['dbpassword'];

    //check connection
    if($link = @mysql_connect($dbhost, $username, $password))
    {
        //check database
        if(!$db = @mysql_selectdb($database))
        {
            //try to create the DB
            $sql = 'CREATE DATABASE '.$database;
            if (mysql_query($sql, $link))
            {
                SQL::getInstance()->reconnect();
                return array(true, "");
            }
            else
            {
                return array(false, "could not create DB");
            }
        }
        else
        {
            SQL::getInstance()->reconnect();
            return array(true, "");
        }
    }
    else
    {
        return array(false, "could not connect to DB");
    }
}
/**
 * Install the components
 *
 * @return array()      [boolean, errormessage]
 */
function installComponents()
{
    global $websiteroot;

    $h = new InstallHelper();
    $db = SQL::getInstance();

    if(array_keys($_POST['components'], 'core'))
    {
        unset($_POST['components'][array_search('core', $_POST['components'])]);
    }

    array_unshift($_POST['components'], 'core');

    //fetch version number
    $versions = json_decode(file_get_contents($websiteroot."/install/version.json"));

    file_put_contents(getFrameworkRoot()."/version.info", $versions->framework);

    foreach($_POST['components'] as $component)
    {
        try
        {
            //run install sql
            $dir = realpath($websiteroot.'/../components/'.$component);
            $file = $dir.'/db/install.xml';

            if(file_exists($file) && is_readable($file))
            {
                $h->loadTable(file_get_contents($file));
            }

            //insert component into the database
            $ini = parse_ini_file($dir.'/info.ini', true);
            $info = parseInfoFile($dir.'/'.$component.".info");

            $dir = addslashes($dir);

            $version = $info["atlines"]["version"];

            //copy the install files
            if(file_exists($dir.'/install'))
            {
                $h->rcopy($dir.'/install', $websiteroot);
            }

            $db->query(
                "INSERT INTO
                    `components`
                (
                    `component_name`,
                    `component_path`,
                    `component_auth`,
                    `component_version`
                )
                VALUES
                (
                    '{$component}',
                    '',
                    15,
                    {$version}
                );"
            );
        }
        catch(Exception $e)
        {
            return array(false, $e->getMessage());
        }
    }

    return array(true);
}
/**
 * Write the config file
 *
 * @return boolean
 */
function saveConfig()
{
    global $websiteroot;
    $h = new InstallHelper();

    $header = <<<HEADER
; This file is part of the A.W.S.O.M.E.cms distribution.
; Detailed copyright and licensing information can be found
; in the doc/COPYRIGHT and doc/LICENSE files which should be
; included in the distribution.
;
; @package configs
;
; @copyright (c) 2009-2010 Yannick de Lange
; @license http://www.gnu.org/licenses/gpl.txt
;
; @version $Revision$
;
; NOTE: This file is generated from config-default.ini unpon instalation
;

HEADER;

    copy($websiteroot.'/config-default.ini', $websiteroot.'/config.ini');

    $ini = parse_ini_file($websiteroot.'/config.ini', true);

    //update the data
    $ini['db']['dbhost'] = $_POST['dbhost'];
    $ini['db']['database'] = $_POST['dbdatabase'];
    $ini['db']['username'] = $_POST['dbusername'];
    $ini['db']['password'] = $_POST['dbpassword'];
    $ini['auth']['component'] = $_POST['authcomponent'];
    $ini['proxy']['enabled'] = ($_POST['proxy'] === 'true');
    $ini['proxy']['proxy'] = $_POST['proxy_url'];
    $ini['proxy']['username'] = $_POST['proxy_user'];
    $ini['proxy']['password'] = $_POST['proxy_pass'];
    $ini['admin']['needslogin'] = ($_POST['admin_needslogin'] === 'true');
    $ini['admin']['location'] = $_POST['admin_location'];
    $ini['admin']['login'] = $_POST['admin_login'];
    $ini['debug']['debug'] = $_POST['debug'];

    $result = (file_put_contents($websiteroot.'/config.ini', $header.$h->arrayToIni($ini)) !== false);

    if($result)
    {
        Config::getInstance()->reload();
    }

    return $result;
}

$config = Config::getInstance();

//check for actions
if(!empty($_REQUEST['action']))
{
    $data = true;

    switch($_REQUEST['action'])
    {
        case 'checkphp' :
            $data = (floatval(phpversion()) >= 5);
            break;
        case 'checkSQL' :
            $version = preg_replace('/[a-zA-Z]/', '', SQL_get_client_info());
            $data = (floatval($version) >= 5);
            break;
        case 'checkgdmod' :
            $data = function_exists('gd_info');
            break;
        case 'checkapache' :
            $data = (preg_match('/^Apache\/2\.[0-9]{1}/', $_SERVER['SERVER_SOFTWARE']) === 1);
            break;
        case 'checkdbconnection' :
            $connection = @mysql_connect($_REQUEST['host'], $_REQUEST['user'], $_REQUEST['pass']);
            $error = "";

            if($connection == false)
            {
                $error = "Could not connect to Database Host";
            }
            else
            {
                //check privilages
                $result = @mysql_query("SELECT Select_priv, Insert_priv, Update_priv, Delete_priv, Create_priv, Drop_priv, Alter_priv FROM mysql.user WHERE User = '{$_REQUEST['user']}' AND Host = '{$_REQUEST['host']}';", $connection);
                $privs = @mysql_fetch_assoc($result);

                if(implode("", $privs) != "YYYYYYY")
                {
                    $error = "Not enough privilages";
                }
                else
                {
                    //is the tabel empty if we have it? (if it empty we get an error)
                    $result = @mysql_query("SHOW TABLES FROM {$_REQUEST['db']}", $connection);

                    if($result !== false && mysql_num_rows($result) > 0)
                    {
                        $error = "Database is not empty";
                    }
                }
            }

            $data = array("success" => ($error == ""), "error" => $error);
            @mysql_close($connection);
            break;
        case 'save' :
            $check1 = false;
            $check2 = false;
            $check3 = false;
            $error = "";

            if($check1 = saveConfig())
            {
                $check2 = installDB();

                if($check2[0])
                {
                    $check3 = installComponents();

                    if(!$check3[0])
                    {
                        $error = "could not install componentes, '" . $check3[1] . "'";
                    }
                }
                else
                {
                    $error = "could not install DB, '" . $check2[1] . "'";
                }
            }
            else
            {
                $error = "Could not save the config file";
            }

            $data = array("result" => ($check1 && $check2[0] && $check3[0]), "error" => $error );
            break;
    }
    echo json_encode($data);
    die();
}

$smarty = new Smarty();

$smarty->compile_check = true;
$smarty->debugging = false;

$smarty->template_dir = $websiteroot.'/install/templates';
$smarty->compile_dir = $websiteroot.'/install/templates_c';
$smarty->cache_dir = $websiteroot.'/install/cache';

//prepare the components list
$components = array();
$componentsDirs = scandir($websiteroot.'/../components/');

foreach($componentsDirs as $component)
{
    if($component != '.' && $component != '..' && '.svn' && is_dir($websiteroot.'/../components/'.$component))
    {
        if(file_exists($websiteroot.'/../components/'.$component.'/info.ini'))
        {
            $infodata = parseInfoFile($websiteroot.'/../components/'.$component.'/'.$component.'.info', $component);
            $inidata = parse_ini_file($websiteroot.'/../components/'.$component.'/info.ini', true);

            $components[$component] = array("name" => ucfirst($component), "info" => $infodata);

            if($inidata['dependson'])
            {
                $components[$component]['dependson'] = $inidata['dependson'];
                $components[$component]['dependson_string'] = implode(', ', $components[$component]['dependson']);
                $components[$component]['hasdependencies'] = true;
            }
            else
            {
                $components[$component]['dependson'] = array();
                $components[$component]['hasdependencies'] = false;
            }

            $components[$component]['canauth'] = ($inidata['canauth'] === "1");
            $components[$component]['development'] = $inidata['development'];

            if($inidata['required'])
            {
                $components[$component]['required'] = $inidata['required'];
            }
            else
            {
                $components[$component]['required'] = false;
            }
        }
    }
}

$ini = parse_ini_file($websiteroot.'/config-default.ini', true);

$smarty->assign('setting_adminlocation', $ini['admin']['location']);
$smarty->assign('setting_adminlogin', $ini['admin']['login']);
$smarty->assign('components', $components);
$smarty->display('index.tpl');
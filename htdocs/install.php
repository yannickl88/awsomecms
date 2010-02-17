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

if(!file_exists('./install/')) //can we even do an install?
{
    header("Location: /");
    exit;
}
/**
 * Check if all keys in an array are numbers
 * 
 * @param array $array
 * @return boolean
 */
function is_numeric_keys($array)
{
    foreach($array as $key => $value)
    {
        if(!is_numeric($key))
        {
            return false;
        }
    }
    
    return true;
}
/**
 * Convert an array to ini format
 * 
 * @param array $array
 * @return string
 */
function arrayToIni($array)
{
    $body = '';
    
    foreach($array as $section => $items)
    {
        if($body != '')
        {
            $body .= "\n";
        }
        
        if(!is_array($items))
        {
            if(is_bool($items))
            {
                $items = ($items)? 'true' : 'false';
            }
            elseif(!is_numeric($items))
            {
                $value = "\"{$value}\"";
            }
            
            $body .= "{$section} = {$items}\n";
        }
        elseif(is_numeric_keys($items))
        {
            foreach($items as $key => $value)
            {
                if(is_bool($value))
                {
                    $value = ($value)? 'true' : 'false';
                }
                elseif(!is_numeric($value))
                {
                    $value = "\"{$value}\"";
                }
                
                $body .= "{$section}[] = {$value}\n";
            }
        }
        else
        {
            $body .= "[{$section}]\n";
            
            foreach($items as $key => $value)
            {
                if(is_bool($value))
                {
                    $value = ($value)? 'true' : 'false';
                }
                elseif(!is_numeric($value))
                {
                    $value = "\"{$value}\"";
                }
                
                $body .= "{$key} = {$value}\n";
            }
        }
        
    }
    return $body;
}
/**
 * Recursivly copy one directory to another
 * 
 * @param string $source
 * @param string $dest
 */
function rcopy($source, $dest)
{
    //is the source a file?
    if(is_file($source))
    {
        copy($source, $dest);
    }
    elseif(is_dir($source))
    {
        if(!file_exists($dest))
        {
            mkdir($dest, 0777, true);
        }
        
        //loop through it and copy the content
        $content = scandir($source);

        foreach($content as $file)
        {
            if($file != '.' && $file != '..' && $file != '.svn')
            {
                rcopy($source.'/'.$file, $dest.'/'.$file);
            }
        }
    }
}
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
 * Get the highest patch level
 * 
 * @param string $installDir
 * @return int
 */
function getHighestPatchLevel($installDir)
{
    $files = scandir($installDir);
    $patchlevel = 0;
    
    foreach($files as $file)
    {
        $matches = array();
        
        if(preg_match('/^patch-([0-9]+)\.sql$/', $file, $matches) == 1)
        {
            if($patchlevel < (int) $matches[1])
            {
                $patchlevel = (int) $matches[1];
            }
        }
    }
    
    return $patchlevel;
}
/**
 * Install the components
 * 
 * @return array()      [boolean, errormessage]
 */
function installComponents()
{
    global $websiteroot;
    
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
            $file = $dir.'/db/install.sql';
            
            if(file_exists($file) && is_readable($file))
            {
                $sql = file_get_contents($file);
                if($sql != '')
                {
                    $db->multi_query($sql);
                }
            }
            
            //insert component into the database
            $ini = parse_ini_file($dir.'/info.ini', true);
            $info = parseInfoFile($dir.'/'.$component.".info");
            $patchlevel = getHighestPatchLevel($dir.'/db');
            
            $dir = addslashes($dir);
            
            $version = $info["atlines"]["version"];
            
            $db->query(
                "INSERT INTO 
                    `components` 
                (
                    `component_name`, 
                    `component_path`, 
                    `component_auth`,
                    `component_patchlevel`,
                    `component_version`
                )
                VALUES
                (
                    '{$component}',
                    '{$dir}',
                    15,
                    {$patchlevel},
                    {$version}
                );"
            );
            
            //install the hooks
            if($ini['hooks'])
            {
                foreach($ini['hooks'] as $hook)
                {
                    $hook_function = explode("->", $hook);
                    $hook_target = explode(":", $hook_function[0]);
                    
                    //insert
                    $db->query(
                        "INSERT INTO 
                            `hooks` 
                        (
                            `hook_component`, 
                            `hook_target`, 
                            `hook_prepost`, 
                            `hook_action`,
                            `hook_function`
                        )
                        VALUES
                        (
                            '{$component}',
                            '{$hook_target[0]}',
                            '{$hook_target[1]}',
                            '{$hook_target[2]}',
                            '{$hook_function[1]}'
                        );"
                    );
                }
            }
        }
        catch(Exception $e)
        {
            return array(false, $e->getMessage());
        }
    }
    
    // second run so we can install the default content
    foreach($_POST['components'] as $component)
    {
        try
        {
            $dir = realpath($websiteroot.'/../components/'.$component);
            
            //copy the install files
            if(file_exists($dir.'/install'))
            {
                rcopy($dir.'/install', $websiteroot);
            }
            
            //run default sql
            $file = $dir.'/db/default.sql';
            if(file_exists($file) && is_readable($file))
            {
            
                $sql = file_get_contents($file);
                if($sql != '')
                {
                    $db->multi_query($sql);
                }
            }
            
            if($_POST['admin_default'] === 'true')
            {
                //run admin sql
                $file = $dir.'/db/admin.sql';
                if(file_exists($file) && is_readable($file))
                {
                
                    $sql = file_get_contents($file);
                    if($sql != '')
                    {
                        $db->multi_query($sql);
                    }
                }
            }
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
    
    $result = (file_put_contents($websiteroot.'/config.ini', arrayToIni($ini)) !== false);
    
    if($result)
    {
        Config::getInstance()->reload();
    }
    
    return $result;
}

global $websiteroot;
$websiteroot = dirname(__FILE__);

include_once '../core/init.inc';

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
            $data = (strpos($_SERVER['SERVER_SOFTWARE'], 'Apache/2.0') === 0);
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
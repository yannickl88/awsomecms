<?php
session_start();

if(!file_exists('install'))
{
    header("Location: /");
    exit;
}

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
            if($file != '.' && $file != '..')
            {
                rcopy($source.'/'.$file, $dest.'/'.$file);
            }
        }
    }
}

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
function installComponents()
{
    global $websiteroot;
    
    $db = SQL::getInstance();
    
    if(array_keys($_POST['components'], 'core'))
    {
        unset($_POST['components'][array_search('core', $_POST['components'])]);
    }
    
    array_unshift($_POST['components'], 'core');
    
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
            
            //copy the install files
            if(file_exists($dir.'/install'))
            {
                rcopy($dir.'/install', $websiteroot);
            }
            
            //insert component into the database
            $dir = addslashes($dir);
            
            $db->query("INSERT INTO `components` (`component_name`, `component_path`) VALUES('{$component}', '{$dir}');");
        }
        catch(Exception $e)
        {
            return false;
        }
        
    }
    
    if($_POST['admin_default'] === 'true')
    {
        //run default sql
        $file = $websiteroot.'/../components/core/db/default.sql';
        try
        {
            if(file_exists($file) && is_readable($file))
            {
            
                $sql = file_get_contents($file);
                if($sql != '')
                {
                    $db->multi_query($sql);
                }
            }
        }
        catch(Exception $e)
        {
            return false;
        }
    }
    return true;
}

function saveConfig()
{
    global $websiteroot;
    
    copy($websiteroot.'/../config-default.ini', $websiteroot.'/../config.ini');
    
    $ini = parse_ini_file($websiteroot.'/../config.ini', true);
    
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
    
    $result = (file_put_contents($websiteroot.'/../config.ini', arrayToIni($ini)) !== false);
    
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
        case 'checkcurl' :
            $data = function_exists('curl_version');
            break;
        case 'checkgdmod' :
            $data = function_exists('gd_info');
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
                    if(!($check3 = installComponents()))
                    {
                        $error = "could not install componentes";
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
            
            $data = array("result" => ($check1 && $check2 && $check3), "error" => $error );
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
    if($component != '.' && $component != '..' && is_dir($websiteroot.'/../components/'.$component))
    {
        $components[$component] = array("name" => ucfirst($component));
        
        if(file_exists($websiteroot.'/../components/'.$component.'/info.ini'))
        {
            $inidata = parse_ini_file($websiteroot.'/../components/'.$component.'/info.ini', true);
            
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

$smarty->assign('components', $components);
$smarty->display('index.tpl');
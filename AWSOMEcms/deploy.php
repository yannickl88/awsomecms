<?php
//cli functions
function prompt($question, $pattern = false)
{
    fwrite(STDOUT, trim($question)." ");
    $result = trim(fgets(STDIN));
    
    //check if there was a valid awnser
    if($pattern !== false && preg_match($pattern, $result) == 0)
    {
        return prompt($question, $pattern);
    }
    
    return $result;
}
function confirm($question)
{
    $awnser = strtolower(prompt(trim($question)." [Y/N]"));
    
    //check if there was a valid awnser
    if($awnser != 'y' && $awnser != 'n')
    {
        return confirm($question);
    }
    
    return ($awnser == "y");
}
function output($message, $sameline = false)
{
    $lb = '';
    
    if(!$sameline)
    {
        $lb = "\n";
    }
    
    fwrite(STDOUT, $message.$lb);
}

//some util function
function rcopy($source, $dest)
{
    if(is_dir($source))
    {
        if(!file_exists($dest))
        {
            mkdir($dest);
        }
        
        $files = scandir($source);
        
        foreach($files as $file)
        {
            if($file != '.' && $file != '..' && $file != '.svn')
            {
                rcopy($source.DIRECTORY_SEPARATOR.$file, $dest.DIRECTORY_SEPARATOR.$file);
            }
        }
    }
    else
    {
        if(!@copy($source, $dest))
        {
            output("FAILED to copy '{$source}' => '{$dest}'");
        }
    }
}
function raddFileToZip($source, $dest, $zip)
{
    if(is_dir($source))
    {
        $files = scandir($source);
        
        if(count($files) > 2)
        {
            foreach($files as $file)
            {
                if($file != '.' && $file != '..' && $file != '.svn')
                {
                    raddFileToZip($source.DIRECTORY_SEPARATOR.$file, $dest.DIRECTORY_SEPARATOR.$file, $zip);
                }
            }
        }
        else
        {
            $zip->addEmptyDir($dest);
        }
    }
    else
    {
        $zip->addFile($source, $dest);
    }
}
function clearDir($source, $unless = array())
{
    $files = scandir($source);
    
    foreach($files as $file)
    {
        if($file != '.' && $file != '..')
        {
            if(is_dir($source.DIRECTORY_SEPARATOR.$file))
            {
                if(!in_array(realpath($source.DIRECTORY_SEPARATOR.$file), $unless))
                {
                    clearDir($source.DIRECTORY_SEPARATOR.$file);
                    rmdir($source.DIRECTORY_SEPARATOR.$file);
                }
            }
            else
            {
                if(!in_array(realpath($source.DIRECTORY_SEPARATOR.$file), $unless))
                {
                    unlink($source.DIRECTORY_SEPARATOR.$file);
                }
            }
        }
    }
}
function dir2zip($source, $dest)
{
    $ds = DIRECTORY_SEPARATOR;
    $zip = new ZipArchive();
    
    if(file_exists($dest))
    {
        unlink($dest);
    }

    if($zip->open($dest, ZIPARCHIVE::CREATE))
    {
        $files = scandir($source);
        
        foreach($files as $file)
        {
            if($file != '.' && $file != '..')
            {
                raddFileToZip($source.$ds.$file, $file, $zip);
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
    $zip->close();
}
function getHightestRevNumber($source)
{
    if(is_array($source))
    {
        $hightest = -1;
        
        foreach($source as $file)
        {
            $rev = getHightestRevNumber($file);
            
            if($rev > $hightest)
            {
                $hightest = $rev;
            }
        }
        
        return $hightest;
    }
    else
    {
        $source = realpath($source);
        $result = array();
        
        exec("svn info -r HEAD {$source}", $result);
        $matches = array();
        preg_match('/Last Changed Rev: ([0-9]*)/m', implode("\n", $result), $matches);
        
        return (int) trim($matches[1]);
    }
}

output("==============================");
output("A.W.S.O.M.E. cms deploy script");
output("© 2009 yannickl88.nl");
output("==============================");
output("");

if(!class_exists('ZipArchive'))
{
    output("ZipArchive not found, please check your PHP distribution");
    die();
}

$ds = DIRECTORY_SEPARATOR;

switch($argv[1])
{
    case 'release': 
        $env = 'release';
        break;
    case 'stable': 
        $env = 'stable';
        break;
    case 'beta': 
        $env = 'beta';
        break;
    case 'update': 
        $env = 'update';
        break;
    default: 
        $env = 'alpha';
        break;
}

$location = dirname(__FILE__).$ds;

output("'{$location}'");
if(!confirm("Is this location correct?"))
{
    $location = prompt("Path to framework root:");
}

//check for tmp dir
if(!file_exists($location."RELEASES{$ds}tmp"))
{
    mkdir($location."RELEASES{$ds}tmp", 0777, true);
}

if($env != 'update')
{
    $version = prompt("Version:", '/^[0-9]+(\.[0-9]+)*$/');
    
    output("Creating archive");
    
    //copy all the required stuff
    mkdir($location."RELEASES{$ds}tmp{$ds}cache", 0777, true);
    mkdir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}install", 0777, true);
    output(".", true);
    rcopy($location.'components', $location."RELEASES{$ds}tmp{$ds}components");
    output(".", true);
    rcopy($location.'core', $location."RELEASES{$ds}tmp{$ds}core");
    output(".", true);
    rcopy($location.'docs', $location."RELEASES{$ds}tmp{$ds}docs");
    output(".", true);
    rcopy($location."htdocs{$ds}install", $location."RELEASES{$ds}tmp{$ds}htdocs{$ds}install");
    rcopy($location."htdocs{$ds}install.php", $location."RELEASES{$ds}tmp{$ds}htdocs{$ds}install.php");
    output(".", true);
    rcopy($location.'libs', $location."RELEASES{$ds}tmp{$ds}libs");
    output(".", true);
    rcopy($location.'.htaccess', $location."RELEASES{$ds}tmp{$ds}.htaccess");
    output(".", true);
    rcopy($location.'index.php', $location."RELEASES{$ds}tmp{$ds}index.php");
    output(".", true);

    if($env != 'release' && $env != 'stable')
    {
        rcopy($location.'deploy.php', $location."RELEASES{$ds}tmp{$ds}deploy.php");
        output(".", true);
    }
    
    //archive
    if($env != 'release')
    {
        $suffix = '-'.$env;
    }

    dir2zip($location."RELEASES{$ds}tmp", $location."RELEASES{$ds}AWSOMEcms_v{$version}{$suffix}.zip");

    output(".");
    //done
    output("Archive '".$location."RELEASES{$ds}AWSOMEcms_v{$version}{$suffix}.zip' created");
}
else
{
    $versions = array();
    output("Creating core archive");
    
    $versions["framework"] = getHightestRevNumber(array($location."core", $location."docs", $location."libs"));
    
    //copy the core files
    rcopy($location."core", $location."RELEASES{$ds}tmp{$ds}core");
    output(".", true);
    rcopy($location.'docs', $location."RELEASES{$ds}tmp{$ds}docs");
    output(".", true);
    rcopy($location."libs", $location."RELEASES{$ds}tmp{$ds}libs");
    output(".", true);
    
    dir2zip($location."RELEASES{$ds}tmp", $location."RELEASES{$ds}framework.zip");
    $versions["components"] = array();
    
    //cleanup
    clearDir($location."RELEASES{$ds}tmp");
    
    //create zips for all components
    $components = scandir($location."components");
    
    foreach($components as $component)
    {
        if($component != '.' && $component != '..' && $component != '.svn')
        {
            if(!file_exists($location."RELEASES{$ds}components"))
            {
                mkdir($location."RELEASES{$ds}components", 0777, true);
            }
            
            $versions["components"][$component] = getHightestRevNumber($location."components{$ds}{$component}");
            
            rcopy($location."components{$ds}{$component}", $location."RELEASES{$ds}tmp{$ds}{$component}");
            dir2zip($location."RELEASES{$ds}tmp{$ds}{$component}", $location."RELEASES{$ds}components{$ds}{$component}.zip");
            output(".", true);
            
            //cleanup
            clearDir($location."RELEASES{$ds}tmp{$ds}{$component}");
            rmdir($location."RELEASES{$ds}tmp{$ds}{$component}");
            output(".", true);
        }
    }
    
    //create version file
    file_put_contents($location."RELEASES{$ds}version.json", json_encode($versions));
    
    output(".");
    //done
    output("Archives created");
}

//cleanup
clearDir($location."RELEASES{$ds}tmp");
rmdir($location."RELEASES{$ds}tmp");
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

$version = prompt("Version:", '/^[0-9]+(\.[0-9]+)*$/');
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
output("Creating archive", true);
//check for releases dir
if(!file_exists($location."RELEASES"))
{
    mkdir($location."RELEASES");
}
//check for tmp dir
if(!file_exists($location."RELEASES{$ds}tmp"))
{
    mkdir($location."RELEASES{$ds}tmp");
}

output(".", true);

if($env != 'update')
{
    //copy all the required stuff
    mkdir($location."RELEASES{$ds}tmp{$ds}cache", 0777, true);
    output(".", true);
    rcopy($location.'components', $location."RELEASES{$ds}tmp{$ds}components");
    output(".", true);
    rcopy($location.'core', $location."RELEASES{$ds}tmp{$ds}core");
    output(".", true);
    rcopy($location.'docs', $location."RELEASES{$ds}tmp{$ds}docs");
    output(".", true);
    rcopy($location.'htdocs', $location."RELEASES{$ds}tmp{$ds}htdocs");
    output(".", true);
    rcopy($location.'libs', $location."RELEASES{$ds}tmp{$ds}libs");
    output(".", true);
    rcopy($location.'.htaccess', $location."RELEASES{$ds}tmp{$ds}.htaccess");
    output(".", true);
    rcopy($location.'config-default.ini', $location."RELEASES{$ds}tmp{$ds}config-default.ini");
    output(".", true);
    rcopy($location.'index.php', $location."RELEASES{$ds}tmp{$ds}index.php");
    output(".", true);

    if($env != 'release' && $env != 'stable')
    {
        rcopy($location.'config.ini', $location."RELEASES{$ds}tmp{$ds}config.ini");
        output(".", true);
        rcopy($location.'deploy.php', $location."RELEASES{$ds}tmp{$ds}deploy.php");
        output(".", true);
    }

    output(".", true);

    //clear directories
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}configs");
    output(".", true);
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}img");
    output(".", true);
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}js");
    output(".", true);
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}css");
    output(".", true);
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}templates_c");
    output(".", true);
    clearDir($location."RELEASES{$ds}tmp{$ds}htdocs{$ds}install{$ds}templates_c");
    output(".", true);
}
else
{
    rcopy($location."components", $location."RELEASES{$ds}tmp{$ds}components");
    output(".", true);
    rcopy($location."core", $location."RELEASES{$ds}tmp{$ds}core");
    output(".", true);
    rcopy($location.'docs', $location."RELEASES{$ds}tmp{$ds}docs");
    output(".", true);
    rcopy($location."libs", $location."RELEASES{$ds}tmp{$ds}libs");
    output(".", true);
}

//archive
$zip = new ZipArchive();

if($env != 'release')
{
    $suffix = '-'.$env;
}

$zipfile = $location."RELEASES{$ds}AWSOMEcms_v{$version}{$suffix}.zip";

if(file_exists($zipfile))
{
    unlink($zipfile);
}

if($zip->open($zipfile, ZIPARCHIVE::CREATE))
{
    $files = scandir($location."RELEASES{$ds}tmp{$ds}");
    
    foreach($files as $file)
    {
        if($file != '.' && $file != '..')
        {
            raddFileToZip($location."RELEASES{$ds}tmp{$ds}{$file}", $file, $zip);
            output(".", true);
        }
    }
}
else
{
    output("Could not create archive");
    die();
}
$zip->close();

output(".", true);

//cleanup
clearDir($location."RELEASES{$ds}tmp");
rmdir($location."RELEASES{$ds}tmp");
output(".");

//done
output("Archive '{$zipfile}' created");
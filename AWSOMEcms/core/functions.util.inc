<?php
function import($file)
{
    $frameworkRoot = dirname(dirname(__FILE__));
    
    require_once realpath($frameworkRoot.'/'.$file);
}

function getFrameworkRoot()
{
    return dirname(dirname(__FILE__));
}

function handleError($errno, $errstr, $errfile, $errline)
{
    $debugger = Debugger::getInstance();
    
    if($errno == E_ERROR || $errno == E_USER_ERROR || $errno == E_CORE_ERROR || $errno == E_PARSE)
    {
        $debugger->error("There was an error in '{$errfile}' at line {$errline}");
        $debugger->error($errstr);
    }
    if(Config::get("debug", 0, "debug") > 1 && ($errno == E_WARNING || $errno == E_CORE_WARNING || $errno == E_USER_WARNING))
    {
        $debugger->warning("{$errstr} '{$errfile}:{$errline}'");
    }
    if(Config::get("debug", 0, "debug") > 1 && ($errno == E_CORE_NOTICE || $errno == E_USER_NOTICE))
    {
        $debugger->log("{$errstr} '{$errfile}:{$errline}'");
    }
    
    if($errno == E_ERROR || $errno == E_USER_ERROR || $errno == E_CORE_ERROR)
    {
        if(Config::get("debug", 0, "debug") > 0)
        {
            die($debugger);
        }
        else
        {
            die("Something went wrong, please contact the server administrator.");
        }
    }
    
    return true;
}
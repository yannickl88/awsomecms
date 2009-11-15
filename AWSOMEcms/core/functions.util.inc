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

function parseInfoFile($file, $component)
{
    if(file_exists($file))
    {
        $data = file($file);
        $result = array(
            "desc" => "",
            "tags" => array(),
            "modifiers" => array(),
        );
        
        $currentTextType = 0;
        $currentLine = 0;
        
        while($currentLine < count($data))
        {
            $line = $data[$currentLine];
            $currentLine++;
            
            if(trim($line) == strtoupper($component->component_name.":"))
            {
                $currentTextType = 1;
                continue;
            }
            if(trim($line) == strtoupper("TAGS:"))
            {
                $currentTextType = 2;
                
                continue;
            }
            if(trim($line) == strtoupper("MODIFIERS:"))
            {
                $currentTextType = 3;
                continue;
            }
            
            if($currentTextType == 1)
            {
                $result['desc'] .= $line;
            }
            if($currentTextType == 2)
            {
                $tagName = explode(",", $line);
                
                while(count($tagName) > 2)
                {
                    
                    $tagName[1] .= ",".implode("", array_slice($tagName, 2, 1));
                    unset($tagName[2]);
                }
                
                $tag = array(
                    "name" => trim($tagName[0]),
                    "desc" => trim($tagName[1]),
                    "params" => array(),
                );
                
                while($data[$currentLine][0] == " ")
                {
                    $data[$currentLine] = trim($data[$currentLine]);
                    
                    if(strpos($data[$currentLine], " ") !== false)
                    {
                        $line = substr($data[$currentLine], 0, strpos($data[$currentLine], " "));
                        $desc = substr($data[$currentLine], strpos($data[$currentLine], " ")+1);
                    }
                    else
                    {
                        $line = $data[$currentLine];
                        $desc = "";
                    }
                    $currentLine++;
                    
                    $line2 = explode(",",trim($data[$currentLine]));
                    $currentLine++;
                    
                    $tag['params'][] = array(array(substr(trim($line), 1, -1), $desc), array(ucfirst($line2[0]), trim($line2[1])));
                }
                $result['tags'][] = $tag;
            }
            if($currentTextType == 3)
            {
                $tagName = explode(",", $line);
                
                $tag = array(
                    "name" => trim($tagName[0]),
                    "desc" => trim($tagName[1]),
                    "params" => array(),
                );
                
                while($data[$currentLine][0] == " ")
                {
                    $data[$currentLine] = trim($data[$currentLine]);
                    
                    if(strpos($data[$currentLine], " ") !== false)
                    {
                        $line = substr($data[$currentLine], 0, strpos($data[$currentLine], " "));
                        $desc = substr($data[$currentLine], strpos($data[$currentLine], " ")+1);
                    }
                    else
                    {
                        $line = $data[$currentLine];
                        $desc = "";
                    }
                    $currentLine++;
                    
                    $line2 = explode(",",trim($data[$currentLine]));
                    $currentLine++;
                    
                    $tag['params'][] = array(array(substr(trim($line), 1, -1), $desc), array(ucfirst($line2[0]), trim($line2[1])));
                }
                $result['modifiers'][] = $tag;
            }
        }
        
        $result['desc'] = trim($result['desc']);
        
        return $result;
    }
    else
    {
        return array();
    }
}
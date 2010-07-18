<?php
session_start();

global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__).DIRECTORY_SEPARATOR."htdocs";

require_once 'core/init.inc';

function generateTranslateable($file)
{
    $content = (array) json_decode(file_get_contents($file));
    $file = array();
    
    $text = "";
    
    $count = 1;
    foreach($content as $key => $value)
    {
        $text .= "    \"".md5($key)."\": \"".$value."\"";
        if($count < count($content))
        {
            $text .= ",";
        }
        $text .= "\n";
        $count++;
    }
    
    return $text;
}
function generateNewLangFile($from, $to)
{
    $oldcontent = (array) json_decode(file_get_contents($from));
    
    if(file_exists($to))
    {
        $newcontent = (array) json_decode(file_get_contents($to));
    }
    $data = array();
    
    $text = "{\n";
    $count = 1;
    foreach($oldcontent as $key => $value)
    {
        if(!isset($newcontent[$key]))
        {
            $url = "http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q=".rawurlencode($value)."&langpair=en|nl";
            $result = json_decode(file_get_contents($url));
            
            echo "getting data '{$key}' <br />";
            
            $newValue = $result->responseData->translatedText;
        }
        else
        {
            $newValue = $newcontent[$key];
        }
        $indent = strlen($key) + 9;
        
        $text .= "    \"".$key."\":";
        while($indent < 30)
        {
            $text .= " ";
            $indent++;
        }
        $text .= "\"".$newValue."\"";
        if($count < count($oldcontent))
        {
            $text .= ",";
        }
        $text .= "\n";
        $count++;
    }
    $text .= "}";
    
    file_put_contents($to, $text);
}

generateNewLangFile(getFrameworkRoot()."/core/lang/en.lang", getFrameworkRoot()."/core/lang/nl.lang");

$components = Config::getInstance()->getComponenets();

foreach($components as $component)
{
    generateNewLangFile($component->component_path."/lang/en.lang", $component->component_path."/lang/nl.lang");
}
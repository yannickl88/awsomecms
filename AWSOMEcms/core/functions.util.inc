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

<?php
function import($file)
{
    $frameworkRoot = dirname(dirname(__FILE__));
    
    require_once realpath($frameworkRoot.'/'.$file);
}

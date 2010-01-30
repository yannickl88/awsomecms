<?php
global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__)."/htdocs";

require_once $websiteroot.'/../core/init.inc';
require_once $websiteroot.'/../libs/class.CronParser.inc';

$components = Config::getInstance()->getComponenets();

$now = $lastrun = time();
$cacheFile = getFrameworkRoot()."/cache/cron.cache";
if(file_exists($cacheFile))
{
    $lastrun = (int) file_get_contents($cacheFile);
}
$cronParser = new CronParser();

foreach($components as $component)
{
    $cronTime = Config::get($component->component_name, false, "cron");
    
    $count = $lastrun;
    if($cronTime)
    {
        while($count <= $now)
        {
            if($cronParser->mustRunNow($cronTime, $count))
            {
                Component::init($component->component_name)->execCron();
            }
            
            $count += 60;
        }
    }
}

file_put_contents($cacheFile, $now);

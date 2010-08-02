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
global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__)."/htdocs";

require_once $websiteroot.'/../core/init.inc';
require_once $websiteroot.'/../core/class.Cache.inc';
require_once $websiteroot.'/../libs/class.CronParser.inc';

Debugger::getInstance()->setType(Debugger::CLI);
$components = RegisterManager::getInstance()->getComponenets();

$now = $lastrun = time();
if(Cache::has("cron"))
{
    $lastrun = Cache::get("cron");
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

Cache::set("cron", $now);

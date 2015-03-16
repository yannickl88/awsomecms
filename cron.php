<?php
require_once __DIR__ . '/vendor/autoload.php';

global $websiteroot;
$websiteroot = dirname(__FILE__).DIRECTORY_SEPARATOR."htdocs";

require_once 'core/init.inc';

class Usage extends CLIAction
{
    /**
     * (non-PHPdoc)
     * @see libs/CLIAction::exec()
     *
     * @param CLI $cli
     * @param String $action
     */
    public function exec($cli, $action)
    {
        $cli->output("php -f cron.php [action] [-t=time]");
        $cli->output("");
        $cli->output("Options:");
        $cli->output("action       help, run");
        $cli->output("  help       This text");
        $cli->output("  run        Run the cron jobs");
        $cli->output("time         Specify a time to which is now, used for debugging.");
        $cli->output("             See strtotime for time formats.");
        $cli->output("");
        $cli->output("Usage:");
        $cli->output("php -f cron.php run");
        $cli->output("             Run the con jobs");
        $cli->output("php -f cron.php run -t=12:00");
        $cli->output("             Run the con jobs using 12:00 today as now");
        $cli->output("");
    }
}
class Cron extends CLIAction
{
    /**
     * (non-PHPdoc)
     * @see libs/CLIAction::exec()
     *
     * @param CLI $cli
     * @param String $action
     */
    public function exec($cli, $action)
    {
        $now = $lastrun = time();
        $cache = true;
        if(Cache::has("cron"))
        {
            $lastrun = Cache::get("cron");
        }
        if($cli->getArg("-t") !== false) //check if we got an time value
        {
            $lastrun = $now = strtotime($cli->getArg("-t"));
            $cache = false;
        }
        $cronParser = new CronParser();
        $components = RegisterManager::getInstance()->getComponents();

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
        if($cache) Cache::set("cron", $now);
    }
}


$cli = new CLI($argv);
//$cli->setVerbose();
$cli->header();

//register the actions
$cli->registerAction("help",        "Usage");
$cli->registerAction("run",         "Cron");

if($cli->getAction() == "") $cli->setAction("run");
$cli->doAction();
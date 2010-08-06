<?php
include "../class.CLI.inc"; //Command line framework
include "./class.PHPDoc.inc";

//action
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
        $cli->output("php -f phpdoc.php [action]");
        $cli->output("");
        $cli->output("Options:");
        $cli->output("action       help, doc");
        $cli->output("  help       This text");
        $cli->output("  doc        Create xml doc files");
        $cli->output("");
    }
}
class Doc extends CLIAction
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
        $doc = new PHPDoc();
        $doc->process();
    }
}
class Render extends CLIAction
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
        $doc = new PHPDoc();
        $doc->render();
    }
}
class Header extends CLIAction
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
        $cli->output("===============================================");
        $cli->output("A.W.S.O.M.E. PHPdoc");
        $cli->output("© 2009-2010 http://code.google.com/p/awsomecms/");
        $cli->output("===============================================");
        $cli->output("");
    }
}

$cli = new CLI($argv);

//register the actions
$cli->registerAction("help",        "Usage");
$cli->registerAction("doc",         "Doc");
$cli->registerAction("render",      "Render");
$cli->registerAction("header",      "Header");

$cli->header();
$cli->doAction();
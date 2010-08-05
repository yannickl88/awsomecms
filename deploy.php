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
global $websiteroot;
$websiteroot = dirname(__FILE__).DIRECTORY_SEPARATOR."htdocs";

require_once 'core/functions.util.inc';

import('core/class.Config.inc');
import('libs/class.FTP.inc');
import('libs/class.InstallHelper.inc');
import('libs/class.CLI.inc');

//actions
abstract class DeployAction extends CLIAction
{
    protected $helper;
    protected $location;
    /**
     * Constructor
     */
    public function __construct()
    {
        $this->helper = new InstallHelper();
        $this->location = dirname(__FILE__);
        var_dump($this->location);
    }
    /**
     * (non-PHPdoc)
     * @see libs/CLIAction::setup()
     *
     * @param CLI $cli
     */
    public function setup($cli)
    {
        if(!$cli->isVerbose())
        {
            $cli->output($this->location);
            if(!$cli->confirm("Is this location correct?"))
            {
                $this->location = $cli->prompt("Path to framework root:");
            }
        }

        if(!file_exists($cli->os->join($this->location,"RELEASES","tmp")))
        {
            mkdir($cli->os->join($this->location,"RELEASES","tmp"), 0777, true);
        }
    }
    /**
     * (non-PHPdoc)
     * @see libs/CLIAction::cleanup()
     *
     * @param CLI $cli
     */
    public function cleanup($cli)
    {
        $this->helper->clearDir($cli->os->join($this->location,"RELEASES","tmp"));
        rmdir($cli->os->join($this->location,"RELEASES","tmp"));
    }
}

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
        $cli->output("php -f deploy.php [action] [u] [v]");
        $cli->output("");
        $cli->output("Options:");
        $cli->output("action       help, update, pack, alpha, beta, stable, release");
        $cli->output("  help       This text");
        $cli->output("  update     Update");
        $cli->output("  pack       Pack the site into an archve which can be send");
        $cli->output("  alpha      Create an alpha release package");
        $cli->output("  beta       Create a beta release package");
        $cli->output("  stable     Create a stable release package");
        $cli->output("  release    Create a release release package");
        $cli->output("u            Upload data, only in Update made");
        $cli->output("v            Verbose mode");
        $cli->output("");
        $cli->output("Usage:");
        $cli->output("php -f deploy.php beta");
        $cli->output("             To create a beta package of the current version");
        $cli->output("php -f deploy.php update");
        $cli->output("             To create the update packages");
        $cli->output("php -f deploy.php update u");
        $cli->output("             To create the update packages and directly upload them");
        $cli->output("php -f deploy.php pack");
        $cli->output("             To create a package of the current site for transport");
        $cli->output("");
    }
}
class Pack extends DeployAction
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
        $h = $this->helper;
        $location = $this->location;

        $dirname = strtolower(basename(rtrim(dirname(__FILE__), '/')));

        $cli->output("Creating archive");
        mkdir($cli->os->join($location,"RELEASES","tmp","cache"), 0777, true);
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'components'), $cli->os->join($location,"RELEASES","tmp","components"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'core'), $cli->os->join($location,"RELEASES","tmp","core"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'docs'), $cli->os->join($location,"RELEASES","tmp","docs"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'htdocs'), $cli->os->join($location,"RELEASES","tmp","htdocs"));
        $h->clearDir($cli->os->join($location,"RELEASES","tmp","htdocs","templates_c"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'libs'), $cli->os->join($location,"RELEASES","tmp","libs"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'.htaccess'), $cli->os->join($location,"RELEASES","tmp",".htaccess"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'index.php'), $cli->os->join($location,"RELEASES","tmp","index.php"));
        $h->rcopy($cli->os->join($location,'version.info'), $cli->os->join($location,"RELEASES","tmp","version.info"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'cron.php'), $cli->os->join($location,"RELEASES","tmp","cron.php"));
        $cli->output(".", true);

        $h->dir2zip($cli->os->join($location,"RELEASES","tmp"), $cli->os->join($location,"{$dirname}.zip"));
        $cli->output(".");

        //done
        $cli->output("Archive '".$cli->os->join($location,"{$dirname}.zip")."' created");
    }
}
class Update extends DeployAction
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
        $h = $this->helper;
        $location = $this->location;
        $versions = array();

        $cli->output("Creating core archive");

        $versions["framework"] = $h->getHightestRevNumber(array(
            $cli->os->join($location,"core"),
            $cli->os->join($location,"docs"),
            $cli->os->join($location,"libs"))
        );

        //copy the core files
        $h->rcopy($cli->os->join($location,"core"), $cli->os->join($location,"RELEASES","tmp","core"));
        $h->rcopy($cli->os->join($location,'docs'), $cli->os->join($location,"RELEASES","tmp","docs"));
        $h->rcopy($cli->os->join($location,"libs"), $cli->os->join($location,"RELEASES","tmp","libs"));
        $h->rcopy($cli->os->join($location,"index.php"), $cli->os->join($location,"RELEASES","tmp","index.php"));
        $h->rcopy($cli->os->join($location,"cron.php"), $cli->os->join($location,"RELEASES","tmp","cron.php"));

        $h->dir2zip($cli->os->join($location,"RELEASES","tmp"), $cli->os->join($location,"RELEASES","framework.zip"));
        $versions["components"] = array();

        //cleanup
        $h->clearDir($cli->os->join($location,"RELEASES","tmp"));

        //create zips for all components
        $components = scandir($cli->os->join($location,"components"));
        $componentsList = array();

        foreach($components as $component)
        {
            if($component != '.' && $component != '..' && $component != '.svn')
            {
                if(!file_exists($cli->os->join($location,"RELEASES","components")))
                {
                    mkdir($cli->os->join($location,"RELEASES","components"), 0777, true);
                }

                $versions["components"][$component] = $h->getHightestRevNumber($cli->os->join($location,"components",$component));

                $h->rcopy($cli->os->join($location,"components",$component), $cli->os->join($location,"RELEASES","tmp",$component));

                $infoContent = file_get_contents($cli->os->join($location,"RELEASES","tmp",$component,$component.".info"));
                $infoContent = preg_replace("/@version: ?([1-9]*)/", "@version:".$versions["components"][$component], $infoContent);
                file_put_contents($cli->os->join($location,"RELEASES","tmp",$component,$component.".info"), $infoContent);

                $h->dir2zip($cli->os->join($location,"RELEASES","tmp",$component), $cli->os->join($location,"RELEASES","components",$component.".zip"));

                //cleanup
                $h->clearDir($cli->os->join($location,"RELEASES","tmp",$component));
                rmdir($cli->os->join($location,"RELEASES","tmp",$component));
                $cli->output(".", true, $verbose);

                $componentsList[$component] = array("name" => $component, "version" => $versions["components"][$component]);

                if($upload)
                {
                    $ftp = FTP::getInstance();
                    $ftp->upload($cli->os->join($location,"RELEASES","components",$component.".zip"), "/public_html_update/components/{$component}.zip");
                }
            }
        }

        //create version file
        file_put_contents($cli->os->join($location,"RELEASES","version.json"), json_encode($versions));
        file_put_contents($cli->os->join($location,"RELEASES","components","components.json"), json_encode($componentsList));

        if($upload)
        {
            $ftp = FTP::getInstance();
            $ftp->upload($cli->os->join($location,"RELEASES","framework.zip"), "/public_html_update/framework.zip");
            $ftp->upload($cli->os->join($location,"RELEASES","version.json"), "/public_html_update/version.json");
            $ftp->upload($cli->os->join($location,"RELEASES","components","components.json"), "/public_html_update/components/components.json");
        }

        $cli->output(".");
        //done
        $cli->output("Archives created");
    }
}
class Release extends DeployAction
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
        $h = $this->helper;
        $location = $this->location;

        //ask for version
        if(!$cli->isVerbose())
        {
            $version = $cli->prompt("Version:", '/^[0-9]+(\.[0-9]+)*$/');
        }
        else
        {
            $version = 0;
        }

        $cli->output("Creating archive");

        $cli->output(".", true);
        //copy all the required stuff
        mkdir($cli->os->join($location,"RELEASES","tmp","cache"), 0777, true);
        mkdir($cli->os->join($location,"RELEASES","tmp","htdocs","install"), 0777, true);
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'components'), $cli->os->join($location,"RELEASES","tmp","components"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'core'), $cli->os->join($location,"RELEASES","tmp","core"));
        $h->rcopy($cli->os->join($location,'docs'), $cli->os->join($location,"RELEASES","tmp","docs"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,"htdocs","install"), $cli->os->join($location,"RELEASES","tmp","htdocs","install"));
        $h->rcopy($cli->os->join($location,"htdocs","install.php"), $cli->os->join($location,"RELEASES","tmp","htdocs","install.php"));
        $h->rcopy($cli->os->join($location,"htdocs","index.php"), $cli->os->join($location,"RELEASES","tmp","htdocs","index.php"));
        $h->rcopy($cli->os->join($location,"htdocs","config-default.ini"), $cli->os->join($location,"RELEASES","tmp","htdocs","config-default.ini"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'libs'), $cli->os->join($location,"RELEASES","tmp","libs"));
        $cli->output(".", true);
        $h->rcopy($cli->os->join($location,'.htaccess'), $cli->os->join($location,"RELEASES","tmp",".htaccess"));
        $h->rcopy($cli->os->join($location,'index.php'), $cli->os->join($location,"RELEASES","tmp","index.php"));
        $h->rcopy($cli->os->join($location,'cron.php'), $cli->os->join($location,"RELEASES","tmp","cron.php"));
        $cli->output(".", true);

        if($action != 'release' && $action != 'stable')
        {
            $h->rcopy($cli->os->join($location,'deploy.php'), $cli->os->join($location,"RELEASES","tmp","deploy.php"));
        }

        //fetch the version numbers
        file_put_contents(
        $cli->os->join($location,"RELEASES","tmp","version.info"),
            $h->getHightestRevNumber(array(
                $cli->os->join($location,"core"),
                $cli->os->join($location,"docs"),
                $cli->os->join($location,"libs")
            ))
        );
        $cli->output(".", true);

        $components = scandir($cli->os->join($location,"components"));

        foreach($components as $component)
        {
            if($component != '.' && $component != '..' && $component != '.svn')
            {
                $compversion = $h->getHightestRevNumber($cli->os->join($location,"components",$component));

                $infoContent = file_get_contents($cli->os->join($location,"RELEASES","tmp","components",$component, $component.".info"));
                $infoContent = preg_replace("/@version: ?([1-9]*)/", "@version:".$compversion, $infoContent);

                file_put_contents($cli->os->join($location,"RELEASES","tmp","components",$component,$component.".info"), $infoContent);
            }
        }
        $cli->output(".", true);


        //archive
        if($action != 'release')
        {
            $suffix = '-'.$action;
        }

        $h->dir2zip($cli->os->join($location,"RELEASES","tmp"), $cli->os->join($location,"RELEASES","AWSOMEcms_v{$version}{$suffix}.zip"));
        $cli->output(".");
        //done
        $cli->output("Archive '".$cli->os->join($location,"RELEASES","AWSOMEcms_v{$version}{$suffix}.zip")."' created");
    }
}

$cli = new CLI($argv);
$cli->header();

//check if we can even zip
if(!class_exists('ZipArchive'))
{
    $cli->output("ZipArchive not found, please check your PHP distribution");
    die(1);
}
//register the actions
$cli->registerAction("help",        "Usage");
$cli->registerAction("release",     "Release");
$cli->registerAction("stable",      "Release");
$cli->registerAction("beta",        "Release");
$cli->registerAction("alpha",       "Release");
$cli->registerAction("update",      "Update");
$cli->registerAction("pack",        "Pack");

$cli->doAction();
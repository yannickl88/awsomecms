<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package libs
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * CliAction interface
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
abstract class CLIAction
{
    /**
     * Setup function, you can override this to add setup stuff. This is 
     * called before the exec
     * 
     * @param CLI $cli
     */
    function setup($cli) {}
    /**
     * Cleanup function, you can override this to add cleanup stuff. This is 
     * called after the exec
     * 
     * @param CLI $cli
     */
    function cleanup($cli) {}
    /**
     * Execute the action
     * 
     * @param CLI $cli
     * @param String $action    action that was passed to the commandline
     */
    abstract function exec($cli, $action);
}
/**
 * OS abstraction, this provides a way to easiely join a path
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class OS
{
    /**
     * Join a series of path elements together in an OS safe way
     * 
     * @param String ...
     * @param String ...
     * @return String
     */
    public function join()
    {
        $items = func_get_args();
        foreach($items as &$item)
        {
            $item = rtrim($item, DIRECTORY_SEPARATOR);
        }
        return implode(DIRECTORY_SEPARATOR, $items);
    }
}
/**
 * Cli main class
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class CLI
{
    public $os;
    private $verbose;
    private $argv;
    private $file;
    private $actions;
    private $action;
    /**
     * Constructor
     * 
     * @param array $argv
     */
    public function __construct($argv)
    {
        $this->actions = array();
        $this->argv = $this->parseArgv($argv);
        $this->verbose = in_array('v', $argv);
        $this->os = new OS();
    }
    /**
     * Parse the argument variables
     * 
     * @param Array $argv
     * @return Array
     */
    private function parseArgv($argv)
    {
        $data = array();
        $this->file = array_shift($argv);
        $this->action = array_shift($argv);
        
        foreach($argv as $arg)
        {
            if(strpos($arg, "=") !== false)
            {
                $data[substr($arg, 0, strpos($arg, "="))] = substr($arg, strpos($arg, "=") + 1);
            }
            else
            {
                $data[$arg] = true;
            }
        }
        return $data;
    }
    /**
     * Set the CLI interface to verbose, all output will not be shown.
     * 
     * @param boolean $verbose
     */
    public function setVerbose($verbose = true)
    {
        $this->verbose = $verbose;
    }
    /**
     * Check if the CLI interface is verbose
     * 
     * @return boolean
     */
    public function isVerbose()
    {
        return $this->verbose;
    }
    /**
     * Get a value from the arguments
     * 
     * @param String $key
     * @return String
     */
    public function getArg($key)
    {
        if(isset($this->argv[$key]))
        {
            return $this->argv[$key];
        }
        return false;
    }
    /**
     * Show a question, if the pattern is not valid, the question will be 
     * asked again.
     * 
     * @param String $question
     * @param String $pattern       regex pattern to match
     * @return String
     */
    public function prompt($question, $pattern = false)
    {
        fwrite(STDOUT, trim($question)." ");
        $result = trim(fgets(STDIN));
        
        //check if there was a valid awnser
        if($pattern !== false && preg_match($pattern, $result) == 0)
        {
            return prompt($question, $pattern);
        }
        
        return $result;
    }
    /**
     * Show a Confirm (yes/no).
     * 
     * @param String $question
     * @return boolean
     */
    public function confirm($question)
    {
        $awnser = strtolower($this->prompt(trim($question)." [Y/N]"));
        
        //check if there was a valid awnser
        if($awnser != 'y' && $awnser != 'n')
        {
            return $this->confirm($question);
        }
        
        return ($awnser == "y");
    }
    /**
     * Output something in the CLI interface
     * 
     * @param String $message
     * @param boolean $sameline     keep the input on the previous line
     */
    public function output($message, $sameline = false)
    {
        $lb = '';
        
        if(!$sameline)
        {
            $lb = "\n";
        }
        
        if(!$this->verbose)
            fwrite(STDOUT, $message.$lb);
    }
    /**
     * Do an action
     */
    public function doAction()
    {
        if(isset($this->actions[$this->action]))
        {
            $this->callAction($this->action);
        }
        else
        {
            $this->output("Cannot execute action {$this->action}");
            if(isset($this->actions["help"]))
            {
                $this->output("Type 'help' for more information");
            }
        }
    }
    /**
     * Call an action
     * 
     * @param String $action
     */
    private function callAction($action)
    {
        $className = $this->actions[$action];
        $actionObj = new $className();
        $actionObj->setup($this);
        $actionObj->exec($this, $action);
        $actionObj->cleanup($this);
    }
    /**
     * Register an action
     * 
     * @param String $action
     * @param String $class
     */
    public function registerAction($action, $class)
    {
        $this->actions[$action] = $class;
    }
    /**
     * Show the header info
     */
    public function header()
    {
        if(isset($this->actions["header"]))
        {
            $this->callAction("header");
        }
        else
        {
            $this->_header();
        }
    }
    /**
     * Set the action manualy
     * 
     * @param String $action
     */
    public function setAction($action)
    {
        $this->action = $action;
    }
    /**
     * Set the action manualy
     * 
     * @param String $action
     */
    public function getAction()
    {
        return $this->action;
    }
    /**
     * Fallback when there was no header
     */
    private function _header()
    {
        $this->output("===============================================");
        $this->output("A.W.S.O.M.E. cms commandline script");
        $this->output("© 2009-2010 http://code.google.com/p/awsomecms/");
        $this->output("===============================================");
        $this->output("");
    }
}
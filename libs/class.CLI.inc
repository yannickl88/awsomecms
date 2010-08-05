<?php

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
        return implode(DIRECTORY_SEPARATOR, func_get_args());
    }
}

class CLI
{
    public $os;
    private $verbose;
    private $argv;
    private $file;
    private $actions;
    private $action;
    
    public function __construct($argv)
    {
        $this->actions = array();
        $this->argv = $this->parseArgv($argv);
        $this->verbose = in_array('v', $argv);
        $this->os = new OS();
    }
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
    public function setVerbose($verbose = true)
    {
        $this->verbose = $verbose;
    }
    public function isVerbose()
    {
        return $this->verbose;
    }
    public function getArg($key)
    {
        if(isset($this->argv[$key]))
        {
            return $this->argv[$key];
        }
        return false;
    }
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
    public function doAction()
    {
        if(isset($this->actions[$this->action]))
        {
            $className = $this->actions[$this->action];
            $actionObj = new $className();
            $actionObj->setup($this);
            try {
                $actionObj->exec($this, $this->action);
            } catch(Exception $e) { }
            $actionObj->cleanup($this);
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
    public function registerAction($action, $class)
    {
        $this->actions[$action] = $class;
    }
    public function header()
    {
        $this->output("===============================================");
        $this->output("A.W.S.O.M.E. cms commandline script");
        $this->output("© 2009-2010 http://code.google.com/p/awsomecms/");
        $this->output("===============================================");
        $this->output("");
    }
}
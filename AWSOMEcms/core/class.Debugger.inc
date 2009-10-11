<?php
/**
* @desc Debugger class that handels all debug messages generated from the CMS
*/
class Debugger
{
    const NOTICE    = 1;
    const WARNING   = 2;
    const ERROR     = 3;
    
    private static $instance;
    
    private $logs;
    
    /**
    * @desc singelton
    * 
    * @return               Instance of Debugger
    */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    
    /**
    * @desc constructor
    */
    private function __construct()
    {
        $this->logs = array();
        $this->log("Debugger initialized");
    }
    
    public function log($message, $level = Debugger::NOTICE)
    {
        global $start;
        
        $timestamp = round(microtime(true) - $start, 2);
        
        $this->logs[] = array(Debugger::NOTICE, "[{$timestamp}sec] - {$message}");
    }
    
    public function warning($message)
    {
        $this->log($message, Debugger::WARNING);
    }
    
    public function error($message)
    {
        $this->log($message, Debugger::ERROR);
    }
    
    public function __toString() {
        if(Config::get('debug', 0, 'debug') == 1)
        {
            return $this->renderDebug();
        }
        else
        {
            return '';
        }
    }
    
    public function renderDebug()
    {
        global $start;
        $debug = '';
        
        $baseLevel = Debugger::NOTICE;
        
        foreach($this->logs as $log)
        {
            if($debug != "")
            {
                $debug .= "<br />";
            }
            
            switch($log[0])
            {
                case Debugger::WARNING:
                    $debug .= "<span style='color: #ffcc00;' class='debugRow warning'>";
                    
                    if($baseLevel < Debugger::WARNING)
                    {
                        $baseLevel = Debugger::WARNING;
                    }
                    
                    break;
                case Debugger::ERROR:
                    $debug .= "<span style='color: #ff0000;' class='debugRow error'>";
                    
                    if($baseLevel < Debugger::ERROR)
                    {
                        $baseLevel = Debugger::ERROR;
                    }
                    
                    break;
                default:
                    $debug .= "<span class='debugRow notice'>";
                    break;
            }
            
            $debug .= $log[1]."</span>";
        }
        
        $timestamp = round(microtime(true) - $start, 2);
        
        $html = "";
        
        //HTML
        $html .= "<div class='debug' style='margin-top: 5px;'>";
        switch($baseLevel)
        {
            case Debugger::WARNING:
                $html .= "<div style='display: block; cursor: pointer; color: #ffcc00;' onclick='debug_toggleDebug();'>";
                break;
            case Debugger::ERROR:
                $html .= "<div style='display: block; cursor: pointer; color: #ff0000;' onclick='debug_toggleDebug();'>";
                break;
            default:
                $html .= "<div style='display: block; cursor: pointer;' onclick='debug_toggleDebug();'>";
                break;
        }
        $html .= "Site was rendered in {$timestamp}sec </div>";
        if(isset($_COOKIE['debugHide']))
        {
            $html .= "<div class='debugBody' style='font-size: 0.9em; padding-left: 10px; display: none;'>";
        }
        else
        {
            $html .= "<div class='debugBody' style='font-size: 0.9em; padding-left: 10px;'>";
        }
        $html .= $debug;
        $html .= "</div></div>";
        
        //JAVASCRIPT
        $html .= <<<STR
<script>
    function debug_hadCookie() {
        var nameEQ = "debugHide=";
        var ca = document.cookie.split(';');
        for(var i=0;i < ca.length;i++) 
        {
            var c = ca[i];
            
            while (c.charAt(0)==' ') 
                c = c.substring(1,c.length);
            
            if (c.indexOf(nameEQ) == 0) 
                return true;
        }
        return false;
    }
    
    function debug_toggleDebug()
    {
        if(debug_hadCookie())
        {
            $('.debugBody').css('display', 'block');
            
            var date = new Date();
            date.setTime(date.getTime()+(-1*24*60*60*1000));
            document.cookie = "debugHide=true; expires="+date.toGMTString()+"; path=/";
        }
        else
        {
            $('.debugBody').css('display', 'none');
            document.cookie = "debugHide=true; path=/";
        }
    }
</script>
STR;
        
        return $html;
    }
}

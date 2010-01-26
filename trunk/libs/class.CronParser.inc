<?php
class CronParser
{
    public function mustRunNow($cronTime, $now = null)
    {
        if($now === null)
        {
            $now = time();
        }
        
        $cronValues = $this->parseTimeString($cronTime);
        
        //check if it must run now
        $curMin     = date('i', $now);
        $curHour    = date('G', $now);
        $curDay     = date('j', $now);
        $curMonth   = date('n', $now);
        $curYear    = date('Y', $now);
        
        return (
            $this->check($cronValues[0], $curMin) && 
            $this->check($cronValues[1], $curHour) && 
            $this->check($cronValues[2], $curDay) && 
            $this->check($cronValues[3], $curMonth) && 
            $this->check($cronValues[4], $curYear)
        );
    }
    
    private function check($cronValue, $timeValue)
    {
        if(is_array($cronValue))
        {
            return in_array($timeValue, $cronValue);
        }
        else if($cronValue == "all")
        {
            return true;
        }
        else if($cronValue[0] == "m")
        {
            return ($timeValue%((int) substr($cronValue, 1)) == 0);
        }
        else
        {
            return false;
        }
    }
    
    private function parseTimeString($cronTime)
    {
        $cronParts = explode(" ", $cronTime);
        
        $min    = $this->parseNumberString($cronParts[0]);
        $hour   = $this->parseNumberString($cronParts[1]);
        $day    = $this->parseNumberString($cronParts[2]);
        $month  = $this->parseNumberString($cronParts[3]);
        $year   = $this->parseNumberString($cronParts[4]);
        
        return array($min, $hour, $day, $month, $year);
    }
    
    private function parseNumberString($timeString)
    {
        $times = array();
        $parts = array();
        
        if($timeString == "*")
        {
            return "all";
        }
        elseif(preg_match('/^\*\/([0-9]*)$/', $timeString))
        {
            return "m".substr($timeString, 2);
        }
        elseif(strpos($timeString, ",") >= 0)
        {
            $parts = explode(",", $timeString);
        }
        else
        {
            $parts[] = $timeString;
        }
        
        foreach($parts as $part)
        {
            if(strpos($part, "-") !== false)
            {
                $fromto = explode("-", trim($part));
                
                for($i = $fromto[0]; $i <= $fromto[1]; $i++)
                {
                    array_push($times, (int) $i);
                }
            }
            else
            {
                array_push($times, (int) $part);
            }
        }
        
        return $times;
    }
}
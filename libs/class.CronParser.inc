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
 * Parser for cron time string
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CronParser
{
    /**
     * Check if a string must run now
     * 
     * @param String $cronTime
     * @param int $now          UNIX timestamp
     * @return Array
     */
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
    /**
     * Check if the time value is in the list of items
     * 
     * @param String $cronValue
     * @param String $timeValue
     * @return Boolean
     */
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
    /**
     * Parse a cron time string
     * 
     * @param String $cronTime
     * @return Array
     */
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
    /**
     * Parse a set of numbers
     * 
     * @param String $timeString
     * @return Array
     */
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
<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Abstract class for extentions, these are small files which can extend the 
 * framework with smarty plugins or url Hooks
 */
abstract class Extention
{
    private static $extentions;

    /**
     * Create an extention
     *
     * @param String $name
     * @return Extention
     * @throws Exception
     */
    public static function init($name)
    {
        if(isset(self::$extentions[$name]))
        {
            return self::$extentions[$name];
        }
        else
        {
            $className = ucfirst($name)."Extention";
            $loc = getFrameworkRoot().'/extentions/'.$name.'.ext.inc';

            Debugger::getInstance()->log("Creating Extention '{$className}'");

            if(file_exists($loc) )
            {
                include_once $loc;

                $obj = new $className();
                if(!self::$extentions)
                {
                    self::$extentions = array();
                }

                self::$extentions[$name] = $obj;

                return $obj;
            }
            else
            {
                throw new Exception("Cannot find Extention file for '{$className}'");
            }
        }
    }
    /**
     * Register items to the registerManager
     *
     * @param RegisterManager $registerManager
     */
    abstract function register($registerManager);
    /**
     * Register items to Smarty
     *
     * @param Smarty $smarty
     */
    abstract function extend($smarty);
}
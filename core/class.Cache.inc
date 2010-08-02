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
 * Cache class which handels quick caching of data
 * NOTE: data is serialized using the build-in PHP serialize and unserialize 
 *       functions, make sure your data can be serialized. 
 *       More info: http://php.net/manual/en/language.oop5.serialization.php
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */ 
class Cache
{
    /**
     * Get the cache directory
     * 
     * @return String
     */
    public static function getCacheDir()
    {
        return getFrameworkRoot()."/cache";
    }
    /**
     * Get cached data
     * 
     * @param String $key
     * @return mixed
     */
    public static function get($key)
    {
        return unserialize(file_get_contents(self::getCacheDir()."/{$key}.cache"));
    }
    /**
     * Check if there is cached data
     * 
     * @param String $key
     * @return boolean
     */
    public static function has($key)
    {
        return file_exists(self::getCacheDir()."/{$key}.cache");
    }
    /**
     * Set cached data
     * 
     * @param String $key
     * @param String $value
     */
    public static function set($key, $value)
    {
        return file_put_contents(self::getCacheDir()."/{$key}.cache", serialize($value));
    }
    /**
     * Clear cached data
     * 
     * @param String $key
     */
    public static function clear($key = false)
    {
        if($key === false)
        {
            $files = scandir(self::getCacheDir());
            
            foreach($files as $file)
            {
                if(file_exists(self::getCacheDir()."/{$files}"))
                {
                    unlink(self::getCacheDir()."/{$files}");
                }
            }
        }
        else
        {
            if(file_exists(self::getCacheDir()."/{$key}.cache"))
            {
                unlink(self::getCacheDir()."/{$key}.cache");
            }
        }
    }
}
<?php
class Cache
{
    public static function getCacheDir()
    {
        return getFrameworkRoot()."/cache";
    }
    
    public static function get($key)
    {
        return unserialize(file_get_contents(self::getCacheDir()."/{$key}.cache"));
    }
    
    public static function has($key)
    {
        return file_exists(self::getCacheDir()."/{$key}.cache");
    }
    
    public static function set($key, $value)
    {
        return file_put_contents(self::getCacheDir()."/{$key}.cache", serialize($value));
    }
    
    public static function clear($key = false)
    {
        if($key === false)
        {
            $files = scandir(self::getCacheDir());
            
            foreach($files as $file)
            {
                unlink(self::getCacheDir()."/{$files}");
            }
        }
        else
        {
            unlink(self::getCacheDir()."/{$key}.cache");
        }
    }
}
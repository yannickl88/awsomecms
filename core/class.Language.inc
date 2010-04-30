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

class Language
{
    private static $instance;
    
    private $lang;
    
    /**
     * Singelton
     *
     * @return Language
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    
    public static function get($key, $lang = false)
    {
        return self::getInstance()->getText($key, $lang);
    }
    
    public function getText($key, $lang = false)
    {
        if(!$lang)
        {
            $lang = Config::get("lang", "en", "lang");
        }
        $map = $this->getLangMap($lang);
        
        if(isset($map[$key]))
        {
            return $map[$key];
        }
        else
        {
            return "{{$key}}";
        }
    }
    
    public function getLangMap($lang)
    {
        if(!isset($this->lang[$lang]))
        {
            $this->loadLang($lang);
        }
        
        return $this->lang[$lang];
    }
    
    private function loadLang($lang)
    {
        $this->lang[$lang] = (array) json_decode(file_get_contents(getFrameworkRoot()."/core/lang/{$lang}.lang"));
        
        $components = Config::getInstance()->getComponenets();
        
        foreach($components as $component)
        {
            if(file_exists($component->component_path."/lang/{$lang}.lang"))
            {
                $langArr = json_decode(file_get_contents($component->component_path."/lang/{$lang}.lang"));
                
                if($langArr != null)
                {
                    $this->lang[$lang] = array_merge($this->lang[$lang], (array) $langArr);
                }
                else
                {
                    Debugger::getInstance()->warning("could not parse lang file '{$component->component_path}/lang/{$lang}.lang'");
                }
            }
            else
            {
                $this->lang[$lang] = array_merge($this->lang[$lang], array());
            }
        }
    }
}
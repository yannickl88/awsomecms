<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.core.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
* Modifier to translate a text string
* 
* @param Array $array
* @return Boolean
*/
function smarty_modifier_text($string, $lang = null)
{
    if(empty($string)) return "";
    
    if($lang == null)
        $lang = getLang();
    
    if(is_array($string))
    {
        return $string[$lang];
    }
    else 
    {
        if(Controller::getInstance()->getSmarty()->getTemplateVars('isADMIN') == 1)
            return Language::get($string);
        return Language::get($string, $lang);
    }
}

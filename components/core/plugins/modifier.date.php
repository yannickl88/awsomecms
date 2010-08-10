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
 * Make the first letter of a string uppercase
 *
 * @param string $string
 * @return string
 */
function smarty_modifier_date($string, $format)
{
    if(strpos($format, "F"))
    {
        $months = array(
            "january",
            "february",
            "march",
            "april",
            "may",
            "june",
            "july",
            "august",
            "september",
            "october",
            "november",
            "december"
        );
        $translations = array(
            ucfirst(Language::get("january")),
            ucfirst(Language::get("february")),
            ucfirst(Language::get("march")),
            ucfirst(Language::get("april")),
            ucfirst(Language::get("may")),
            ucfirst(Language::get("june")),
            ucfirst(Language::get("july")),
            ucfirst(Language::get("august")),
            ucfirst(Language::get("september")),
            ucfirst(Language::get("october")),
            ucfirst(Language::get("november")),
            ucfirst(Language::get("december"))
        );
        
        return str_ireplace($months, $translations, date($format, strtotime($string)));
    }
    return date($format, strtotime($string));
}

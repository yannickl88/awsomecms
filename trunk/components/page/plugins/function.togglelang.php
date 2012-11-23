<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Toggel the lang parameter in the current URL
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_togglelang($params, &$smarty)
{
    $queryStr = explode("?", $_SERVER['REQUEST_URI']);
    $page = array_shift($queryStr);
    $queryArr = array();
    if(count($queryStr) > 0)
    {
        foreach (explode("&", $queryStr[0]) as $p)
        {
            $param = explode("=", $p);
            if($param[0] != "url")
                $queryArr[array_shift($param)] = implode("=", $param);
        }
    }
    
    $queryArr["lang"] = $params["lang"];
    $url = "";
    
    foreach($queryArr as $key => $value)
    {
        if($url == "")
            $url .= "?";
        else
            $url .= "&";
        
        $url .= $key . "=" . $value;
    }
    
    return $page . $url;
}
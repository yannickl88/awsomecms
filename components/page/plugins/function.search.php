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
 * Show a list of results on this page
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_search($params, &$smarty)
{
    import('libs/class.TextSpider.inc');

    $key = "q";
    
    if(isset($params['key']))
    {
        $key = $params['key'];
    }
    
    $indexer = new TextSpider();
    $results = $indexer->search(explode(" ", $_GET[$key]));
    
    $smarty->assign("searchResults", $results);
    return $smarty->fetch("search/search.tpl");
}
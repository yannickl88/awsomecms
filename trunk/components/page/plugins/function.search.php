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
    $itemspp = 20;
    $currentpage = 0;
    
    if(isset($params['key']))
    {
        $key = $params['key'];
    }
    
    $indexer = new TextSpider();
    $results = $indexer->search(explode(" ", htmlentities($_GET[$key])));
    
    foreach($results as &$result)
    {
        $text = unserialize($result->item_text);
        $text['en'] = _searchTruncate($text['en']);
        $text['nl'] = _searchTruncate($text['nl']);
        
        $result->item_text = serialize($text);
    }
    
    $smarty->assign("searchTerm", htmlentities($_GET[$key]));
    $smarty->assign("searchResults", $results);
    return $smarty->fetch("search/search.tpl");
}

function _searchTruncate($str)
{
    $str = trim($str);
    $lastChar1 = strpos($str, ".") + 1;
    $lastChar2 = strpos($str, "\n");
    
    if($lastChar1 === false) $lastChar1 = 100;
    if($lastChar2 === false) $lastChar2 = 100;
    
    $lastChar = min($lastChar1, $lastChar2);
    
    if($lastChar < 100)
    {
        return substr($str, 0, $lastChar);
    }
    else
        return $str;
}
<?php
/**
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_search($params, &$smarty)
{
    $key = "q";
    $itemspp = 20;
    $currentpage = 0;

    if(isset($params['key']))
    {
        $key = $params['key'];
    }

    $indexer = new TextSpider();
    $results_all = $indexer->search(explode(" ", htmlentities($_GET[$key])));
    $results_highlights = array();
    $results = array();

    foreach($results_all as $result)
    {
        $text = unserialize($result->item_text);
        $text['en'] = _searchTruncate($text['en']);
        $text['nl'] = _searchTruncate($text['nl']);

        $result->item_text = serialize($text);

        if($result->item_priority > 0)
        {
            $results_highlights[] = $result;
        }
        else
        {
            $results[] = $result;
        }
    }

    $smarty->assign("searchTerm", htmlentities($_GET[$key]));
    $smarty->assign("searchHighlightsResults", $results_highlights);
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
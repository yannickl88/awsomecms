<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.news.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Show a news item
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_newsitem($params, &$smarty)
{
    $table = Table::init('news.news');
    $template = "news/news_item.tpl";
    
    if(isset($params['template']))
    {
        $template = $params['template'];
    }

    $news = $table
        ->setRequest((object) array_merge($params, array('news_id' => $_GET['id'])))
        ->doSelect()
        ->getRow();
    $smarty->assign("news", $news);
    
    //fetch the next and previous items
    $news2 = $table
        ->doSelect()
        ->getRows();
    
    $previtem = -1;
    $nextitem = -1;
    
    foreach($news2 as $key => $newsitem)
    {
        if($newsitem->news_id == $news->news_id)
        {
            $previtem = $news2[$key + 1];
            $nextitem = $news2[$key - 1];
            break;
        }
    }
    
    $smarty->assign("nextnews", $nextitem);
    $smarty->assign("prevnews", $previtem);

    return $smarty->fetch($template);
}
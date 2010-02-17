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
 * Show a list of tags that are used by all the news items
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_newstags($params, &$smarty)
{
    $template = "news/news_tags.tpl";
    
    //select
    $tags = SQLQuery::doSelect()
        ->table('news')
        ->orderBy('news_tag')
        ->orderBy('news_date', 'DESC')
        ->exec()
        ->getRows();

    $lasttag = null;
    
    foreach($tags as $key => $tag)
    {
        if($lasttag == $tag->news_tag)
        {
            unset($tags[$key]);
        }
        else
        {
            $lasttag = $tag->news_tag;
        }
    }
    $smarty->assign("tags", $tags);

    return $smarty->fetch($template);
}
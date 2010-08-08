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
 * Show a list of news items
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_news($params, &$smarty)
{
    $newsComponent = Table::init('news.news');
    $template = "news/news.tpl";
    
    if(isset($params['template']))
    {
        $template = $params['template'];
    }
    if(isset($params['tag']))
    {
        $params['news_tag'] = $params['tag'];
    }
    
    $items = array();
    $rows = $newsComponent->setRequest((object) $params)->doSelect();
    
    while($row = $rows->getRow())
    {
        
        $row->news_text = BBCodeParser::parse($row->news_text);
        $items[] = $row;
    }
    
    $smarty->assign("news", $items);

    return $smarty->fetch($template);
}
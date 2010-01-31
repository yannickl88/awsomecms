<?php
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

    $smarty->assign("news", $newsComponent->doSelect($params)->getRows());

    return $smarty->fetch($template);
}
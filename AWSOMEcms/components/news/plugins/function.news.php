<?php
function smarty_function_news($params, &$smarty)
{
    $newsComponent = Component::init('news');
    $template = "news/news.tpl";
    
    if(isset($params['template']))
    {
        $template = $params['template'];
    }
    if(isset($params['tag']))
    {
        $params['news_tag'] = $params['tag'];
    }

    $smarty->assign("news", $newsComponent->select($params));

    return $smarty->fetch($template);
}
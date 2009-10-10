<?php
function smarty_function_newsitem($params, &$smarty)
{
    $newsComponent = Component::init('news');
    $template = "news/newsitem.tpl";
    
    if(isset($params['template']))
    {
        $template = $params['template'];
    }

    $news = $newsComponent->select(array_merge($params, array('news_id' => $_GET['id'])));
    $smarty->assign("news", $news[0]);

    return $smarty->fetch($template);
}
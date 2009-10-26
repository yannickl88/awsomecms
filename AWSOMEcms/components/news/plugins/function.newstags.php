<?php
function smarty_function_newstags($params, &$smarty)
{
    $newsComponent = Request::init('news');
    $template = "news/newstags.tpl";
    
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
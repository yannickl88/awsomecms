<?php
function smarty_function_link($params, &$smarty)
{
    if(Cache::has("link_".$params['code']))
    {
        $data = Cache::get("link_".$params['code']);
    }
    else
    {
        $link = Table::init('linker.links')
            ->setRequest((object) array("link_code" => $params['code']))
            ->doSelect()
            ->getRow();

        $data = $link;
        Cache::set("link_".$params['code'], $data);
    }
    $url = "";
    $target = "";
    $goto = isset($params['action'])? $params['action'] : "default";

    if(!empty($params['text']))
    {
        $text = stripslashes($params['text']);
    }
    else
    {
        $text = $data->link_title;
    }

    if((!empty($data->link_page) && $goto != "url") || $goto == "page")
    {
        $url = $data->link_page;

        if(empty($url)) $url = "/";
    }
    else
    {
        $url = $data->link_url;
        $target = " rel='external'";
    }

    return "<a href=\"{$url}\"{$target}>{$text}</a>";
}
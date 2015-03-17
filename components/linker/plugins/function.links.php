<?php
function linker_lcatSort($a, $b)
{
    return strcmp($a->lcat_name, $b->lcat_name);
}

function linker_linkSort($a, $b)
{
    return strcmp($a->link_title, $b->link_title);
}

function smarty_function_links($params, &$smarty)
{
    $requestData = Controller::getInstance()->getRequest()->data;
    if(isset($requestData["urlmapmatch"]) && isset($requestData["urlmapmatch"][1]) && is_numeric($requestData["urlmapmatch"][1]))
    {
        $links = Table::init('linker.links')
            ->setRequest((object) array("link_cat" => (int) $requestData["urlmapmatch"][1] ))
            ->doSelect()
            ->getRows();

        foreach($links as &$link)
        {
            $link->link_description = $link->link_description[getLang()];

            $link->lcat_name = unserialize($link->lcat_name);
            $link->lcat_name = $link->lcat_name[getLang()];
            $link->lcat_description = unserialize($link->lcat_description);
            $link->lcat_description = $link->lcat_description[getLang()];
        }
        uasort($links, "linker_linkSort");

        $smarty->assign("links", $links);
        return $smarty->fetch("_linkcat.tpl");
    }
    else
    {
        $links = Table::init('linker.lcats')
            ->doSelect()
            ->getRows();

        foreach($links as &$link)
        {
            $link->lcat_name = $link->lcat_name[getLang()];
            $link->lcat_description = $link->lcat_description[getLang()];
        }
        uasort($links, "linker_lcatSort");

        $smarty->assign("links", $links);
        return $smarty->fetch("_links.tpl");
    }
}
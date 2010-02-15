<?php
/**
* Comments tag, this adds the comments functionality to a page
* 
* params: hook, redirect
* 
* @param array $params
* @param Smarty $smarty
*/
function smarty_function_comments($params, &$smarty)
{
    $hook = $_GET['url'];

    if(isset($params['hook']))
    {
        $hook = $params['hook'];
    }
    if(isset($params['redirect']))
    {
        $smarty->assign("comment_redirect", $params['redirect']);
    }

    include_once Config::get('websiteroot', '.').'/../components/comments/class.CommentsComponent.inc';

    $smarty->assign("comments", Table::init("comments.comments")->setRequest((object) array('comment_hook' => $hook))->doSelect()->getRows());
    $smarty->assign("comment_hook", $hook);

    return $smarty->fetch("comments/comments.tpl");
}
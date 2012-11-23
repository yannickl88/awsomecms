<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.comments.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

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
    $smarty->assign("comment_candelete", (true || isset($_SESSION['usr']) && $_SESSION['usr']['auth']['admin'] === true));

    return $smarty->fetch("comments/comments.tpl");
}
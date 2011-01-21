<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.twitter.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Add the latest twitter message to your site
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_twitter($params, &$smarty)
{
    $username = rawurlencode(Config::get("username", "", "twitter"));
    $url = "http://api.twitter.com/1/statuses/user_timeline.json?screen_name={$username}&count=1";

    if(!isset($_SESSION['twitter']['status']))
    {
        $_SESSION['twitter']['status'] = json_decode(file_get_contents($url));
    }
    $statuses = $_SESSION['twitter']['status'];
    
    if(isset($statuses->error))
    {
        $text = "There was an error at twitter.";
        $smarty->assign("time", date("H:i n/j/Y"));
    }
    else
    {
        $smarty->assign("time", date("H:i n/j/Y", parseTwitterDateString($statuses[0]->created_at)));
        $text = $statuses[0]->text;
    }

    //parse for the extra stuff
    $text = preg_replace('/http:\/\/(www\.)?[a-zA-Z-\.0-9]*\.[a-z\.]{2,5}(\/[.\/\S]*)?/m', '<a href="$0" rel="external">$0</a>', $text); //link
    $text = preg_replace('/@([a-zA-Z0-9_-]*)/', '<a href="http://twitter.com/$1" rel="external">$0</a>', $text); //user
    $text = preg_replace('/#([a-zA-Z0-9_-]*)/', '<a href="http://twitter.com/#search?q=%23$1" rel="external">$0</a>', $text); //hashtags

    $smarty->assign("message", $text);
    $smarty->assign("url", "http://twitter.com/".Config::get("username", "", "twitter"));

    return $smarty->fetch("twitter/twitter.tpl");
}

function parseTwitterDateString($string)
{
    $m = array();
    preg_match('/([A-Z]{1}[a-z]{2}) ([A-Z]{1}[a-z]*) ([0-9]*) ([0-9]*):([0-9]*):([0-9]*) \+([0-9]*) ([0-9]*)/', $string, $m);
    
    return strtotime("{$m[1]}, {$m[3]} {$m[2]} {$m[8]} {$m[4]}:{$m[5]}:{$m[6]} +{$m[7]}");
}

<?php
function smarty_function_twitter($params, &$smarty)
{
    include_once Config::get('websiteroot', '.').'/../components/twitter/util/class.Twitter.inc';

    $twitter = new Twitter(Config::get("username", "", "twitter"), Config::get("password", "", "twitter"));

    if(!$_SESSION['twitter']['statuses'])
    {
        try
        {
            $_SESSION['twitter']['statuses'] = $twitter->getUserTimeline();
        }
        catch(Exception $e)
        {
            $statuses = array(array(
                "text" => $e->getMessage(),
                "created_at" => time()
            ));
        }
    }

    $statuses = $_SESSION['twitter']['statuses'];

    $text = $statuses[0]['text'];

    //parse for the extra stuff
    $text = preg_replace('/http:\/\/(www\.)?[a-zA-Z-\.0-9]*\.[a-z\.]{2,5}(\/[.\/\S]*)?/m', '<a href="$0" rel="external">$0</a>', $text); //link
    $text = preg_replace('/@([.\S]*)/', '<a href="http://twitter.com/$1" rel="external">$0</a>', $text); //user

    $smarty->assign("message", $text);
    $smarty->assign("url", "http://twitter.com/".Config::get("username", "", "twitter"));
    $smarty->assign("time", date("H:m n/j/Y", $statuses[0]['created_at']));

    return $smarty->fetch("twitter/twitter.tpl");
}
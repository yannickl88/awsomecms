<?php
if(ini_get("register_argc_argv"))
{
    //CLI mode, which means we are called from the cron
    if(file_exists("../update.conf"))
    {
        
        
        passthru("php -f ../deploy.php update v");
        unlink("../update.conf");
    }
}
else
{
    $key    = 'dR1sZEXHwG01Sf-N'; //your secret key
    $data   = file_get_contents('php://input'); //raw POST data
    $digest = hash_hmac('md5', $data, $key); //hash created with data + key
    if(isset($_SERVER['HTTP_GOOGLE_CODE_PROJECT_HOSTING_HOOK_HMAC']))
    {
        $hook   = @$_SERVER['HTTP_GOOGLE_CODE_PROJECT_HOSTING_HOOK_HMAC']; //the special header google sends
    }
    else
    {
        $hook = false;
    }
    
    if ($hook && ($hook == $digest))
    {
        file_put_contents("../update.conf", "true");
    }
}
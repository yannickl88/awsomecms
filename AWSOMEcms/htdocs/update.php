<?php
session_start();

if(!file_exists('install'))
{
    header("Location: /");
    exit;
}

global $websiteroot;
$websiteroot = dirname(__FILE__);

include_once '../core/init.inc';

$config = Config::getInstance();

$smarty = new Smarty();

$smarty->compile_check = true;
$smarty->debugging = false;

$smarty->template_dir = $websiteroot.'/install/templates';
$smarty->compile_dir = $websiteroot.'/install/templates_c';
$smarty->cache_dir = $websiteroot.'/install/cache';
$smarty->display('update.tpl');
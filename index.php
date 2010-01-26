<?php
$url = "http://www.".$_SERVER['SERVER_NAME'].'/'.$_SERVER['REQUEST_URI'];

header("HTTP/1.1 301 Moved Permanently");
header("Location: {$url}");
exit();

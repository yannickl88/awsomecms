<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
        <title>A.W.S.O.M.E. cms site - {if $URL != ''}{$URL|ucfirst}{else}Index{/if}</title>
        <meta http-equiv="Content-type" content="text/html;charset=UTF-8" />
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            {literal}
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            google.load("jqueryui", "1.5.3");
            // ]]> 
            {/literal}
        </script>
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
        <script type="text/javascript" src="/js/jquery.flash.js"></script>
        <link rel="stylesheet" type="text/css" href="/css/css.css" media="screen" />
        <link rel="stylesheet" type="text/css" href="/css/lightbox.css" media="screen" />
    </head>
    <body>
        {$content}
        {renderdebug}
    </body>
</html>

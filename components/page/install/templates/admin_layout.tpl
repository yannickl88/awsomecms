<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
        <title>Admin</title>
        <meta http-equiv="Content-type" content="text/html;charset=UTF-8" />
        <meta name="description" content="SlyFox Design - We provide professional web solutions to all clients, that are cost effective in all aspects. With full control, our clients can maintain their own websites with ease and without having to know complicated computer launguages!"/>
        <meta name="keywords" content="web, design, website, uk, cheap, professional, effective" />
        <meta name="country" content="United Kingdom" />
        <meta name="geo.country" content="GB" />
        <link type="text/css" rel="stylesheet" href="/css/admin.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/lightbox.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/jHtmlArea.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/datePicker.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/style.css" media="screen" />
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            {literal}
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.4.1", {uncompressed:false});
            google.load("jqueryui", "1.7.2");
            // ]]> 
            {/literal}
        </script>
        <script type="text/javascript" src="/js/site.js"></script>
        <script type="text/javascript" src="/js/admin.js"></script>
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
        <script type="text/javascript" src="/js/ajaxupload.3.5.js"></script>
        <script type="text/javascript" src="/js/tinyeditor.js"></script>
        <script type="text/javascript">
            {literal}
            $(document).ready(function(){
                // load lightbox
                $(".lightbox").lightbox({
                    fitToScreen: true,
                    imageClickClose: false,
                    disableNavbarLinks: true
                });
                $('#doc').click(function(e) {
                    window.open("/doc.php", "_blank", "width=500,height=600");
                    return false;
                });
            });
            {/literal}
        </script>
    </head>
    {if $URL == "admin/login"}
    <body class="login">
        <div id="loginWrapper">
            <span class="header">{"adminlogin"|text}</span>
            {$content}
        </div>
        {renderdebug}
    {else}
    <body>
        <div id="formWrapperBackground">
            <div id="formWrapperBorder">
                <div id="formInnerWrapper">
                    <span id="formHeaderWrapper">
                        <a href="#" onclick="return closeFormPopup()">{"cancel"|text} <img src="/img/icons/cross.png" /></a>
                    </span>
                    <div id="formWrapper">
                    </div>
                </div>
            </div>
        </div>
        <div id="sidebarWrapper">
            <div id="sidebarHeader">
                <a href="/admin/" id="headerLink">
                    <img src="/img/admin/logo.png" />
                </a>
                <div id="headerLinks">
                    {if $HASDOC}
                    <a href="/doc.php" id="doc" target="_blank">{"doc"|text}</a> |
                    {/if} 
                    <a href="/">{"viewsite"|text}</a> | 
                    <a href="/?logout=true">{"logout"|text}</a>
                </div>
            </div>
            <div id="adminMenuWrapper">
                {adminmenu}
            </div>
            <div id="adminTreeWrapper">
                <ul class="title">
                    <li><a class="title">{"sitepages"|text}</a>
                        <div id="treeWrapper">
                            {admintree}
                        </div>
                    </li>
                </ul>
            </div>
        </div>
        <div id="mainWrapper">
            <div id="mainHeader">
                <h1>{"welcome"|text} {$USR.user_name|ucfirst}</h1>
            </div>
            <div id="contentWrapper">
                {foreach from=$NOTIFICATIONS item=note key=id}
                    <div class="notification" id="notification_{$id}">
                        {$note}
                    </div>
                {/foreach}
                {$content}
            </div>
            <div id="copyright">
                <hr /><br />
                <a href="http://code.google.com/p/awsomecms/">A.W.S.O.M.E. cms</a> &copy; 2009 - 2010
            </div>
            {renderdebug}
        </div>
        {/if}
    </body>
</html>
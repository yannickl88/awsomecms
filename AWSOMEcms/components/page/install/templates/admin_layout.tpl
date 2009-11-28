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
        <script type="text/javascript" src="/js/site.js"></script>
        <script type="text/javascript" src="/js/tree.js"></script>
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
        <script type="text/javascript" src="/js/ajaxupload.3.5.js"></script>
        <script type="text/javascript" src="/js/jHtmlArea-0.6.0.min.js"></script>
        <script type="text/javascript" src="/js/adminmenu.js"></script>
        <script type="text/javascript">
            {literal}
            $(document).ready(function(){
                // load lightbox
                $(".lightbox").lightbox({
                    fitToScreen: true,
                    imageClickClose: false,
                    disableNavbarLinks: true
                });
                
                //load the editor
                $("form .htmleditor").htmlarea({
                    css: "/css/jHtmlArea.Editor.css"
                });
                $("form").submit(function(e) {
                    $(this).find(".htmleditor").each(function(key, value){
                        var name = $(value).attr('name');
                        var content = $(value).htmlarea('toHtmlString');
                        
                        $(value).replaceWith("<textarea style='display: none;' name='" + name + "'>" + content + "</textarea>");
                    });
                });
                $('#doc').click(function(e) {
                    window.open("/doc.php", "_blank", "width=500,height=600");
                    return false;
                });
            });
            {/literal}
        </script>
    </head>
    <body>
        <div id="siteWrapper">
            <div id="headerWrapper">
                <img src="/img/admin/header-right.png" style="float: right;"/>
                <a href="/" style="float: right;" class="backtosite"><img src="/img/admin/backtosite.png"/> Back to site</a>
                <a href="/doc.php" style="float: right;" class="backtosite" id="doc"><img src="/img/admin/help.png"/></a>
                <a href="/admin/"><img src="/img/admin/header-left.png" /></a>
            </div>
            <div id="sideWrapper">
                <div id="menuWrapper">
                    {adminmenu}
                </div>
                <div id="treeWrapper">
                    {admintree}
                </div>
                <img src="/img/admin/tree-bottom.png" />
                <div id="copyWrapper">
                    <a href="http://yannickl88.nl/cms" target="_blank">A.W.S.O.M.E. cms</a> &copy; 2009
                </div>
            </div>
            <div id="contentWrapper">
                {$content}
            </div>
        </div>
        {renderdebug}
    </body>
</html>
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
        <script type="text/javascript" src="/js/tree.js"></script>
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
        <script type="text/javascript" src="/js/ajaxupload.3.5.js"></script>
        <script type="text/javascript" src="http://js.nicedit.com/nicEdit-latest.js"></script>
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
                $("form .htmleditor").each(function(key, value) {
                    var data = {
                        buttonList: [
                            'bold',
                            'italic',
                            'underline',
                            'left',
                            'center',
                            'right',
                            'justify',
                            'ol',
                            'ul',
                            'subscript',
                            'superscript',
                            'strikethrough',
                            'removeformat',
                            'hr',
                            'image',
                            'link',
                            'unlink',
                            'upload',
                            'fontFormat',
                            'xhtml'
                        ],
                        xhtml: true
                    };
                    
                    new nicEditor(data).panelInstance(value);
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
            <div id="sidebarWrapper">
                <div id="sidebarHeader">
                    <a href="/admin/" id="headerLink">
                        <img src="/img/admin/logo.png" />
                    </a>
                    <div id="headerLinks">
                        {if $HASDOC}
                        <a href="/doc.php" id="doc" target="_blank">Doc</a> |
                        {/if} 
                        <a href="/">View site</a> | 
                        <a href="/?logout=true">Log out</a>
                    </div>
                </div>
                <div id="adminMenuWrapper">
                    {adminmenu}
                </div>
                <div id="adminTreeWrapper">
                    <ul class="title">
                        <li><a class="title">Site Pages</a>
                            <div id="treeWrapper">
                                {admintree}
                            </div>
                        </li>
                    </ul>
                </div>
            </div>
            <div id="mainWrapper">
                <div id="mainHeader">
                    <h1>Welcome {$USR.user_name|capitalize}</h1>
                </div>
                <div id="contentWrapper">
                    {$content}
                    <div id="copyright">
                        <hr /><br />
                        <a href="http://yannickl88.nl/cms">A.W.S.O.M.E. cms</a> &copy; 2009 - 2010
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="nl" lang="nl">
    <head>
        <title>SV the classicks</title>
        <link rel="stylesheet" type="text/css" href="/css/css.css" />
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
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
    </head>
    <body>
        <div id="siteWrapper">
            <div id="headerWrapper">
                <ul id="menuWrapper">
                    <li><a href="/" class="selected">Home</a></li>
                    <li><a href="/poll">Poll</a></li>
                    <li><a href="/ideeen">Ideeen</a></li>
                    <li><a href="/vacatures">Vactures</a></li>
                    <li><a href="/oversv">Over SV</a></li>
                    <li><a href="/faq">FAQ</a></li>
                </ul>
            </div>
            <div id="mainWrapper">
                <div id="sidebarWrapper">
                    <h1>Rubrieken</h1>
                    {newstags}
                </div>
                <div id="contentWrapper">
                    <div>
                        {$content}
                    </div>
                </div>
            </div>
            <div id="footerWrapper">
                <div id="copyWrapper">
                    theclassicks.nl &copy; 2009 - <img src="/img/poweredby.gif" /> Powered by <a href="http://yannickl88.nl/cms/" target="_blank">A.W.S.O.M.E cms</a>
                </div>
                {renderdebug}
            </div>
        </div>
    </body>
</html>
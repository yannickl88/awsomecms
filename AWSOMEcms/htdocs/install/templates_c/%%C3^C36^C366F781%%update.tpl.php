<?php /* Smarty version 2.6.22, created on 2009-09-12 17:20:44
         compiled from update.tpl */ ?>
<html>
    <head>
        <title>Install</title>
        <link type="text/css" rel="stylesheet" href="/install/css/css.css" media="screen" />
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            <?php echo '
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            google.load("jqueryui", "1.5.3");
            // ]]> 
            '; ?>

        </script>
        <script type="text/javascript" src="/install/js/install.js"></script>
    </head>
    <body>
        <div id="updateWrapper">
            <div id="headWrapper">
                <ul>
                    <li id="step1_menu"><span class="selected">1. Welcome</span></li>
                    <li id="step2_menu"><span>2. Update</span></li>
                </ul>
            </div>
            <div id="contentWrapper">
                <div id="step1">
                    Welcome to the CMS update wizard, this will guide you through the update steps needed in order to make your framework up to date again.<br /><br />A up-to-date framework will be faster and more secure, so it's important you keep it up to date.
                    <br /><br />
                    The wizard detected the following uninstalled update:
                    <ul>
                        <li>Gallery v3 <img src="/install/img/arrow.png" class="arrow"/> v4</li>
                    </ul>
                </div>
                <div id="step2" style="display: none;">
                    In order to use the SlyFox Component Framework you system needs to support to following items:<br />
                    <br />
                    <img src='/install/img/loader.gif' id="check1"/> PHP 5 or higher <br />
                    <img src='/install/img/loader.gif' id="check2"/> MySQL 5.0 or higher <br />
                    <img src='/install/img/loader.gif' id="check3"/> CURL extention <br />
                    <img src='/install/img/loader.gif' id="check4"/> GDMod <br />
                    <br />
                    <a href="javascript: void(0)" onclick="action_step2();">Re-check</a>
                </div>
            </div>
            <div id="buttonWrapper">
                <span id="copyright">&copy; 2009 <a href="http://www.slyfoxdesign.co.uk/" target="_blank">SlyFox Design</a></span>
                <button type="button" id="prev" disabled="disabled" onclick="prevStep();">Back</button>
                <button type="button" id="next" onclick="nextStep();">Next</button>
            </div>
        </div>
    </body>
</html>
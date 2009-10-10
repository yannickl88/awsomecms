<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
        <title>Skimboarding</title>
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
        <script type="text/javascript">
            {literal}
            var preLoadedImages = new Array();
            
            $(function() {
                $('.lightbox').lightbox();
            });
            
            function preload()
            {
                var imgArray = new Array('/img/bg.jpg', '/img/bg_1.jpg', '/img/bg_2.jpg', '/img/bg_3.jpg', '/img/bg_4.jpg', '/img/bg_5.jpg');
                
                $(imgArray).each(function(key, element){
                    var image = new Image();
                    image.src = element;
                    
                    preLoadedImages.push(image);
                });
            }
            
            function map(mapId)
            {
                if(mapId > 0 && mapId <= 5)
                {
                    changeMap('bg_'+mapId);
                }
                else
                {
                    changeMap('bg');
                }
            }
            
            function changeMap(backgroundImage)
            {
                $('#mainBody').css("background-image", "url(/img/"+backgroundImage+".jpg)");
            }
            
            function mapClick(mapId)
            {
                alert(mapId);
            }
            {/literal}
        </script>
        <style type="text/css">
            {literal}
            body
            {
                background-color: #222222;
                background-image: url(/img/bg_main.gif);
                background-repeat: no-repeat;
            }
            img
            {
                border-width: 0px;
            }
            body, table
            {
                color: #ffffff;
                font-family: Verdana;
                font-size: 13px;
            }
            li
            {
                padding-left: 5px;
                text-indent: -5px;
            }
            h1, h2
            {
                color: #ed5806;
            }
            h2
            {
                font-size: 17px;
                margin: 0px;
                margin-bottom: 4px;
            }
            hr
            {
                border: 0px solid #131313;
                border-top: 1px dotted #ffffff;
                width: 80%;
            }
            a:link, a:visited, a:active {
                color: #ed5806;
                text-decoration: none;
            }
            a:hover { 
                color: #ed5806;
                text-decoration: underline;
            }
            
            .head
            {
                background-image: url(/img/head.gif);
                background-position: 50% 100%;
                background-repeat: no-repeat;
            }
            .grayBG
            {
                background-color: #121212;
            }
            .main
            {
                padding: 4px;
            }
            .news
            {
                font-size: 12px;
                background-image: url(/img/footer2.gif);
                background-position: 100% 100%;
                background-repeat: no-repeat;
            }
            .footer
            {
                background-image: url(/img/footer1.gif);
                background-position: 100% 100%;
                background-repeat: no-repeat;
                font-size: 10px;
            }
            .menuItem
            {
                float: left;
                height: 153px;
                
                padding: 0px;
                margin: 0px;
                
            }
            .menuItemImage
            {
                height: 153px;
                display: block;
                background-position: 100% 100%;
            }
            .menuItemImage:hover
            {
                background-position: 0% 0%;
            }
            {/literal}
        </style>
        <link rel="stylesheet" type="text/css" href="/css/lightbox.css" media="screen" />
    </head>
    <body style="margin: 0px; padding: 0px;" onload="preload()">
        <table cellpadding="0" cellspacing="0" border="0" style="width: 900px; height: 900px; background-repeat: no-repeat; background-image: url(/img/bg.jpg);" id="mainBody">
            <tr>
                <td rowspan="3" style="height: 350px; width: 200px;" valign="top">
                    <img src="/img/spacer.gif" class="spacer" style="width: 100%; height: 30px;" onmouseover="map(5);" onmouseout="map(0)" onclick="mapClick(5);"/><br />
                    <img src="/img/spacer.gif" class="spacer" style="width: 145px; height: 55px;" onmouseover="map(5);" onmouseout="map(0)" onclick="mapClick(5);" align="left"/>
                    <img src="/img/spacer.gif" class="spacer" style="width: 50px; height: 55px;" onmouseover="map(4);" onmouseout="map(0)" onclick="mapClick(4);"/><br />
                    <img src="/img/spacer.gif" class="spacer" style="width: 100%; height: 80px;" onmouseover="map(3);" onmouseout="map(0)" onclick="mapClick(3);"/><br />
                    <img src="/img/spacer.gif" class="spacer" style="width: 100%; height: 60px;" onmouseover="map(2);" onmouseout="map(0)" onclick="mapClick(2);"/><br />
                    <img src="/img/spacer.gif" class="spacer" style="width: 100%; height: 110px;" onmouseover="map(1);" onmouseout="map(0)" onclick="mapClick(1);"/><br />
                </td>
                <td style="height: 200px;" valign="top" class="head" colspan="2">
                    <div style="background-image: url(/img/klikNL.gif); background-repeat: no-repeat; background-position: 0% 100%; height: 195px;">
                        <ul style="list-style-type: none; margin: 0px; padding: 0px; float: right;">
                            <li class="menuItem"><a href="/index" class="menuItemImage" style="background-image: url(/img/menu1.jpg); width: 103px;"></a></li>
                            <li class="menuItem"><a href="/skimspots" class="menuItemImage" style="background-image: url(/img/menu2.jpg); width: 147px;"></a></li>
                            <li class="menuItem"><a href="#" class="menuItemImage" style="background-image: url(/img/menu3.jpg); width: 149px;"></a></li>
                            <li class="menuItem"><a href="/forum" class="menuItemImage" style="background-image: url(/img/menu4.jpg); width: 108px;"></a></li>
                            <li class="menuItem"><a href="#" class="menuItemImage" style="background-image: url(/img/menu5.jpg); width: 109px;"></a></li>
                        </ul>
                        <img src="img/spacer.gif" class="spacer" style="width: 70px; height: 20px;" onmouseover="map(5);" onmouseout="map(0)" onclick="mapClick(5);"/><br />
                        <img src="img/spacer.gif" class="spacer" style="width: 70px; height: 140px;" onmouseover="map(4);" onmouseout="map(0)" onclick="mapClick(4);"/><br />
                    </div>
                </td>
            </tr>
            <tr>
                <td class="main grayBG" valign="top">
                    {$content}
                </td>
                <td class="news grayBG" valign="top" style="width: 200px;">
                    {news max=20 template="news/news_side.tpl"}
                    <hr />
                </td>
            </tr>
            <tr>
                <td class="footer grayBG" style="height: 30px;" colspan="2">
                    <center>
                        Copyright ©2009 Skimfo - Site made by Yannick
                    </center>
                </td>
            </tr>
        </table>
    </body>
</html>
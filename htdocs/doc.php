<?php
session_start();

global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__);

require_once '../core/init.inc';

$components = RegisterManager::getInstance()->getComponents();
?>
<html>
    <head>
        <title>Documentation</title>
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            google.load("jqueryui", "1.5.3");
            // ]]> 
        </script>
        <style>
            h1, h2, h3 {
                margin: 10px 0 5px 0;
            }
            p {
                margin: 0 0 10px 0;
            }
            ul, ol {
                margin: 0;
            }
            body {
                font-family: Verdana;
                font-size: 12px;
            }
            .body {
                padding-left: 15px;
            }
        </style>
    </head>
    <body style="margin: 0; padding: 0;">
        <div style="float: left; width: 150px; height: 100%; overflow: auto;">
            <div style="margin: 4px;">
                <h3>Content</h3>
                Components
                <ul>
<?php
    foreach($components as $component)
    {
        echo "                    <li><a href='#{$component->component_name}'>{$component->component_name}</a></li>\n";
    }
?>
                </ul>
            </div>
        </div>
        <div style="height: 100%; overflow: auto;">
            <h1>Documentation</h1>
            The following content is targeted mainly for the web designers that will be using the CMS. The content here describes each component and what functionalty they offer in the form of tags and modifiers.<br />
            <br />
            The framework uses the <a href="http://smarty.net/" target="_blank">Smarty template engine</a> as core, so if you are not fammiliar with that. It is recommended to read the docummentation of Smarty first. <a href="http://smarty.net/manual/en/" target="_blank">It can be found here</a>.
<?php
    foreach($components as $component)
    {
        echo "            <div id=\"comp_{$component->component_name}\" style=\"display: block;\">\n";
        
        //let's parse that info file
        $data = parseInfoFile("{$component->component_path}/{$component->component_name}.info", $component->component_name);
        
        echo "            <h1><a name='{$component->component_name}'>".ucfirst($component->component_name)."</a></h1>\n";
        echo "            <div class='body'>\n";
        echo "            <p>".nl2br($data['desc'])."</p>\n";
        if(count($data['tags']) > 0)
        {
            echo "            <h2>Tags:</h2>\n";
            
            foreach($data['tags'] as $tag)
            {
                echo "            <b>{$tag['name']}</b>";
                if(!empty($tag['desc']))
                {
                    echo ", <span style='font-size: 0.8em; color: #333333;'>{$tag['desc']}</span>";
                }
                echo "<br />\n";
                if(count($tag['params']) > 0)
                {
                    echo "            Parameters:<br />\n";
                    echo "            <ul>\n";
                    foreach($tag['params'] as $param)
                    {
                        echo "                <li>";
                        if($param[1][1] == "optional")
                        {
                            echo "[ ";
                        }
                        echo "<i style='font-size: 0.8em;'>{$param[1][0]}</i> {$param[0][0]}";
                        if($param[1][1] == "optional")
                        {
                            echo " ]";
                        }
                        echo "<br/>".$param[0][1];
                        echo "</li>\n";
                    }
                    echo "            </ul>\n";
                }
            }
        }
        if(count($data['modifiers']) > 0)
        {
            echo "            <h2>Modifiers:</h2>\n";
            
            foreach($data['modifiers'] as $tag)
            {
                echo "            <b>{$tag['name']}</b>";
                if(!empty($tag['desc']))
                {
                    echo "<span style='font-size: 0.8em; margin-left: 30px;'>{$tag['desc']}</span>";
                }
                echo "<br />\n";
                if(count($tag['params']) > 0)
                {
                    echo "            Parameters:<br />\n";
                    echo "            <ol>\n";
                    foreach($tag['params'] as $param)
                    {
                        echo "                <li>";
                        if($param[1][1] == "optional")
                        {
                            echo "[ ";
                        }
                        echo "<i style='font-size: 0.8em;'>{$param[1][0]}</i>";
                        if($param[1][1] == "optional")
                        {
                            echo " ]";
                        }
                        echo "<br/>{$param[0][0]}";
                        if(!empty($param[0][1]))
                        {
                            echo ", ".$param[0][1];
                        }
                        echo "</li>\n";
                    }
                    echo "            </ol>\n";
                }
            }
        }
        echo "            </div>\n";
        echo "            </div>\n";
    }
?>

            </div>
        </div>
    </body>
</html>
<?php
session_start();

global $websiteroot, $start;

$start = microtime(true);
$websiteroot = dirname(__FILE__);

require_once '../core/init.inc';

$debugID = $_GET['id'];
$debugKey = $_GET['key'];

$data = Cache::get("debug_{$debugID}");
$data = $data[$debugKey];

function getFileName($absPath)
{
    return array_pop(explode(DIRECTORY_SEPARATOR,$absPath));
}

function findLineNumber()
{
    $args = func_get_args();
    $file = array_shift($args);
    
    $lines = file($file);
    if(count($args) == 0) return -1;
    
    $cSearch = array_shift($args);
    
    foreach($lines as $lineno => $line)
    {
        if(strstr($line, $cSearch) !== false)
        {
            if(count($args) == 0) return $lineno + 1;
            $cSearch = array_shift($args);
        }
    }
    
    return -1;
}

if($_POST["action"] == "loadCode")
{
    $lines = file(rawurldecode($_POST['file']));
    
    $data = array();
    $code = array_slice($lines, $_POST['line']-5, 10);
    $code[4] = "<div class='highlight'>".$code[4]."</div>";
    $data["code"] = implode("", $code);
    
    echo(json_encode($data));
    exit;
}

?>
<html>
    <head>
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            // ]]> 
        </script>
        <script type="text/javascript">
            // <![CDATA[ 
            var debugID = "<?php echo $debugID; ?>";
            var debugKey = <?php echo $debugKey; ?>;
            
            function loadCode(file, line, rowKey)
            {
            	data = {
					action: "loadCode",
					id: debugID,
					key: debugKey,
					rowKey: rowKey,
					file: file,
					line: line
                    	}
				$.post("debug.php", data, function(data) {
					$("#codeView").html(data.code);
					}, "json");

				return false;
            }
            // ]]> 
        </script>
        <style type="text/css">
        	body, html {
        		font-family: Courier New;
        		font-size: 11px;
        	}
        	.highlight {
        		background: #EEE;
        	}
        </style>
    </head>
    <body>
    	<div id="sidebar">
	        <div id="stacktrace">
<?php
    foreach($data as $key => $item)
    {
        $absFile = rawurlencode($item['file']);
        $file = getFileName($item['file']);
        $line = $item['line'];
        
        if(empty($file))
        {
            $file = getFileName($data[$key - 1]['file']);
            $absFile = rawurlencode($data[$key - 1]['file']);
            $line = findLineNumber($data[$key - 1]['file'], "class {$item['class']}", "function {$item['function']}");
        }
        echo "<a href='#' class='trace' onclick='return loadCode(\"{$absFile}\", {$line}, {$key});'>{$file} : {$line}</a><br />\n";
    }
?>
        	</div>
    	</div>
    	<pre id="codeView">
    	
    	</pre>
    </body>
</html>
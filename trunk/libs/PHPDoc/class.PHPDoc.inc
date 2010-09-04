<?php
class PHPDocConfig
{
    private static $data;

    public static function get($key)
    {
        if(self::$data == null)
        {
            self::$data = parse_ini_file("./settings.ini");
        }

        if(isset(self::$data[$key]))
        {
            return self::$data[$key];
        }
        return false;
    }
}
class PHPDocComment
{
    private $file;
    private $text;
    //@ lines
    private $author;
    private $category;
    private $copyright;
    private $deprecated;
    private $license;
    private $package;
    private $param;
    private $return;
    private $see;
    private $throws;
    private $todo;
    private $version;

    /** unsuported @ lines
     private $abstract;
     private $access;
     private $example;
     private $filesource;
     private $final;
     private $global;
     private $ignore;
     private $internal;
     private $link;
     private $method;
     private $name;
     private $property;
     private $since;
     private $static;
     private $staticvar;
     private $subpackage;
     private $tutorial;
     private $uses;
     private $var;
     */

    public function __construct($file)
    {
        $this->empty = true;
        $this->text = "";
        $this->file = $file;
    }
    public function has($key)
    {
        return property_exists($this, $key);
    }
    public function __get($key)
    {
        return $this->$key;
    }
    public function __set($key, $value)
    {
        if(property_exists($this, $key))
        {
            $value = trim($value);
            if(PHPDocConfig::get("hasSVN") && strtolower($value) == '$revision$')
            {
                $value = $this->file->getRevision();
            }

            if(in_array($key, array("param")))
            {
                if($this->$key == null)
                {
                    $this->$key = array();
                }
                array_push($this->$key, $value);
            }
            else
            {
                $this->$key = $value;
            }
        }
    }
    public function setText($text)
    {
        $this->text = trim($text);
    }

    public function toXML()
    {
        $xml = "<header>";
        if($this->text)
        $xml .= "<text>".htmlentities($this->text)."</text>";
        if($this->author)
        $xml .= "<author>".htmlentities($this->author)."</author>";
        if($this->category)
        $xml .= "<category>".htmlentities($this->category)."</category>";
        if($this->copyright)
        $xml .= "<copyright>".htmlentities($this->copyright)."</copyright>";
        if($this->deprecated)
        $xml .= "<deprecated>".htmlentities($this->deprecated)."</deprecated>";
        if($this->license)
        $xml .= "<license>".htmlentities($this->license)."</license>";
        if($this->package)
        $xml .= "<package>".htmlentities($this->package)."</package>";
        if($this->param)
        {
            foreach($this->param as $param)
            {
                $matches = array();
                if(preg_match('/^([a-zA-Z0-9_]*) ?(\$[a-zA-Z0-9_]*) ?(.*?)$/', $param, $matches) === 1)
                {
                    $type = htmlentities($matches[1]);
                    $var = htmlentities($matches[2]);
                    $text = htmlentities($matches[3]);
                }
                else if(preg_match('/^(\$[a-zA-Z0-9_]*) ?(.*?)$/', $param, $matches) === 1)
                {
                    $type = "*";
                    $var = htmlentities($matches[2]);
                    $text = htmlentities($matches[3]);
                }
                else
                {
                    $type = "*";
                    $var = htmlentities($param);
                    $text = "";
                }
                list($type, $primitive) = $this->getType($type);

                if($primitive)
                {
                    $xml .= "<param type='{$type}' primitive='1'><field>{$var}</field><text>{$text}</text></param>";
                }
                else
                {
                    $xml .= "<param type='{$type}'><field>{$var}</field><text>{$text}</text></param>";
                }
            }
        }
        if($this->return)
        {
            $matches = array();
            if(preg_match('/^([a-zA-Z0-9_]*) ?(.*?)$/', $this->return, $matches) === 1)
            {
                $type = htmlentities($matches[1]);
                $text = htmlentities($matches[2]);
            }
            else
            {
                $type = "*";
                $var = htmlentities($this->return);
                $text = "";
            }
            list($type, $primitive) = $this->getType($type);

            if($primitive)
            {
                $xml .= "<return type='{$type}' primitive='1'><text>{$text}</text></return>";
            }
            else
            {
                $xml .= "<return type='{$type}'><text>{$text}</text></return>";
            }
        }
        if($this->see)
        $xml .= "<see>".htmlentities($this->see)."</see>";
        if($this->throws)
        $xml .= "<throws>".htmlentities($this->throws)."</throws>";
        if($this->todo)
        $xml .= "<todo>".htmlentities($this->todo)."</todo>";
        if($this->version)
        $xml .= "<version>".htmlentities($this->version)."</version>";
        $xml .= "</header>";

        return $xml;
    }
    private function getType($type)
    {
        $primitive = false;
        if($type == "mixed" || $type == "unknown_type") $type = "*";

        if(in_array(strtolower($type), array("*", "int", "boolean", "array", "string", "float", "object", "resource")))
        {
            $primitive = true;

            $type = strtolower($type);
            //fix mistate I made >.<
            if($type == "boolean") $type = "bool";
        }

        return array($type, $primitive);
    }
}
class PHPDocBlock
{
    const PHPFUNCTION   = 1;
    const PHPCLASS      = 2;
    const PHPMETHOD     = 3;
    const PHPFILE       = 4;

    public $header;
    protected $blocks;
    protected $type;
    public $parent;

    public function __construct($type)
    {
        $this->blocks = array();
        $this->type = $type;
    }

    public function append($block)
    {
        array_push($this->blocks, $block);
        $block->parent = $this;
    }
    public function getID()
    {
        return "block.".htmlentities($this->name);
    }
    public function toXML()
    {
        $xml = "<block id='{$this->getID()}'>";
        $xml .= $this->header->toXML();
        foreach($this->blocks as $block)
        {
            $xml .= $block->toXML();
        }
        $xml .= "</block>";

        return $xml;
    }
    public function getFile()
    {
        if($this->parent != null)
        {
            return $this->parent->getFile();
        }
    }
    public function getPackage()
    {
        if($this->parent != null)
        {
            return $this->parent->getPackage();
        }
    }
}
class PHPDocFile extends PHPDocBlock
{
    private $file;
    private $revision;

    public function __construct($file)
    {
        $this->file = $file;
        parent::__construct(PHPDocBlock::PHPFILE);
    }
    public function toXML()
    {
        $xml = "<file>";
        $xml .= "<name>".htmlentities($this->file)."</name>";
        if(isset($this->header))
        {
            $xml .= $this->header->toXML();
        }
        else
        {
            $xml .= "<header />";
        }
        foreach($this->blocks as $block)
        {
            $xml .= $block->toXML();
        }
        $xml .= "</file>";

        return $xml;
    }
    public function getRevision()
    {
        if(!$this->revision)
        {
            $source = realpath($this->file);
            $result = array();

            exec("svn info {$source}", $result);
            $matches = array();
            preg_match('/Last Changed Rev: ([0-9]*)/m', implode("\n", $result), $matches);

            $this->revision = (int) trim($matches[1]);
        }
        return $this->revision;
    }
    public function getFile()
    {
        return substr($this->file, strlen(PHPDocConfig::get("input")) + 1);
    }
    public function getPackage()
    {
        if($this->header && $this->header->package)
        {
            return $this->header->package;
        }
        return PHPDocConfig::get("defaultPackage");
    }
}
class PHPDocClass extends PHPDocBlock
{
    public $abstract;
    public $interface;
    public $name;
    public $extends;
    public $implements;

    public function __construct()
    {
        parent::__construct(PHPDocBlock::PHPCLASS);
    }
    public function getID()
    {
        return "class.".htmlentities($this->name);
    }
    public function toXML()
    {
        $xml = "<class id='{$this->getID()}'>";
        $xml .= "<name>".htmlentities($this->name)."</name>";
        $xml .= "<file>".htmlentities($this->getFile())."</file>";
        $xml .= "<abstract>{$this->abstract}</abstract>";
        $xml .= "<interface>{$this->interface}</interface>";
        $xml .= "<extends>{$this->extends}</extends>";
        $xml .= "<implements>";
        if($this->implements != null)
        {
            foreach($this->implements as $implement)
            {
                $xml .= "<interface>{$implement}</interface>";
            }
        }
        $xml .= "</implements>";
        $xml .= $this->header->toXML();
        foreach($this->blocks as $block)
        {
            $xml .= $block->toXML();
        }
        $xml .= "</class>";

        return $xml;
    }
}
class PHPDocMethod extends PHPDocBlock
{
    public $abstract;
    public $name;
    public $accessor;
    public $static;

    public function __construct()
    {
        parent::__construct(PHPDocBlock::PHPMETHOD);
    }
    public function getID()
    {
        return "method.".htmlentities($this->parent->name.".".$this->name);
    }
    public function toXML()
    {
        $xml = "<method id='{$this->getID()}'>";
        $xml .= "<name>".htmlentities($this->name)."</name>";
        $xml .= "<abstract>{$this->abstract}</abstract>";
        $xml .= "<accessor>{$this->accessor}</accessor>";
        $xml .= "<static>{$this->static}</static>";
        $xml .= $this->header->toXML();
        foreach($this->blocks as $block)
        {
            $xml .= $block->toXML();
        }
        $xml .= "</method>";

        return $xml;
    }
}
class PHPDocFunction extends PHPDocBlock
{
    public $name;

    public function __construct()
    {
        parent::__construct(PHPDocBlock::PHPFUNCTION);
    }
    public function getID()
    {
        return "function.".htmlentities($this->name);
    }
    public function toXML()
    {
        $xml = "<function id='{$this->getID()}'>";
        $xml .= "<name>".htmlentities($this->name)."</name>";
        $xml .= "<file>".htmlentities($this->getFile())."</file>";
        $xml .= $this->header->toXML();
        foreach($this->blocks as $block)
        {
            $xml .= $block->toXML();
        }
        $xml .= "</function>";

        return $xml;
    }
}
class PHPFileDoc
{
    const READING = 1;
    const RBCOMMENT = 2;
    const RLCOMMENT = 3;

    private $state;
    private $file;

    public function __construct($file)
    {
        $this->file = $file;
    }

    public function process()
    {
        $this->state = self::READING;

        $content = file($this->file);
        $commentBlock = "";
        $fileInfo = new PHPDocFile($this->file);
        $currentClass = null;
        $currentItem = new PHPDocComment($fileInfo);
        $level = 0;
        $refs = array("items" => array(), "packages" => array());

        foreach($content as $key => $line)
        {
            $line = trim($line);
            $text = "";
            $matches = array();
            if($this->state == self::READING)
            {
                //remove singel line comment blocks
                $line = trim(preg_replace('/(\/\/(.*)|#(.*)|\/\*\*?(.*)\*\/)$/', "", $line));

                if(empty($line) || $line == "<?php" || $line == "?>") //empty or singel line comment
                {
                    continue;
                }

                if(preg_match('/^\/\*\*$/', $line) === 1) //comment open
                {
                    $this->state = self::RBCOMMENT;

                    //are we still in the header?
                    if(!empty($commentBlock) && $currentClass == null && $fileInfo->header == null)
                    {
                        $fileInfo->header = $currentItem;
                        $currentItem = new PHPDocComment($fileInfo);
                    }
                    $commentBlock = "";
                }
                elseif(preg_match('/^(abstract)?[ ]*class (.*)$/', $line) === 1) //comment open
                {
                    if($currentClass != null)
                    {
                        $fileInfo->append($currentClass);
                    }
                    $e = new PHPDocClass();
                    $e->header = $currentItem;
                    $this->parseClassHeader($e, $line);
                    $refs["items"][$e->getID()] = $e;

                    $currentItem = new PHPDocComment($fileInfo);
                    $currentClass = $e;
                }
                elseif(preg_match('/^interface (.*)$/', $line) === 1) //comment open
                {
                    if($currentClass != null)
                    {
                        $fileInfo->append($currentClass);
                    }
                    $e = new PHPDocClass();
                    $e->header = $currentItem;
                    $this->parseClassHeader($e, $line);
                    $refs["items"][$e->getID()] = $e;

                    $currentItem = new PHPDocComment($fileInfo);
                    $currentClass = $e;
                }
                elseif(preg_match('/^(abstract)? ?(public|private|protected)?[ ]*(static)?[ ]*function/', $line) === 1) //comment open
                {
                    if($currentClass != null)
                    {
                        $e = new PHPDocMethod();
                        $e->header = $currentItem;
                        $this->parseMethodHeader($e, $line);
                        $currentClass->append($e);
                    }
                    else
                    {
                        $e = new PHPDocFunction();
                        $e->header = $currentItem;
                        $this->parseFunctionHeader($e, $line);
                        $fileInfo->append($e);
                    }
                    $refs["items"][$e->getID()] = $e;

                    $currentItem = new PHPDocComment($fileInfo);
                }
            }
            elseif($this->state == self::RBCOMMENT)
            {
                if(preg_match('/^\*\/$/', $line, $matches) === 1)
                {
                    $currentItem->setText($commentBlock);

                    $this->state = self::READING;
                }
                elseif(preg_match('/^\*(.*)$/', $line, $matches) === 1)
                {
                    $mline = trim($matches[1]);
                    //check if we have an @line
                    if(substr($mline, 0, 1) == "@")
                    {
                        $attr = substr($mline, 1, strpos($mline, " ") - 1);
                        $value = substr($mline, strpos($mline, " ")+1);

                        $currentItem->$attr = $value;
                    }
                    else
                    {
                        $commentBlock .= "{$mline}\n";
                    }
                }
            }
        }
        //check if we had a class
        if($currentClass != null)
        {
            $fileInfo->append($currentClass);
        }

        $dom = new DOMDocument("1.0", "utf-8");
        $dom->formatOutput = true;
        $dom->preserveWhiteSpace = false;
        $dom->loadXML($fileInfo->toXML());
        $fileNode = $dom->getElementsByTagName("file")->item(0);

        if($fileInfo->header && $fileInfo->header->package)
        {
            $package = str_replace(".", "_", $fileInfo->header->package);
        }
        else
        {
            $package = PHPDocConfig::get("defaultPackage");
        }
        $packageFile = PHPDocConfig::get("output")."/xml/".$package.".xml";
        $packageXML = new DOMDocument("1.0", "utf-8");
        $packageXML->preserveWhiteSpace = false;
        //do we already have a package file?
        if(file_exists($packageFile))
        {
            $packageXML->loadXML(trim(file_get_contents($packageFile)));
            $packageNode    = $packageXML->getElementsByTagName("package")->item(0);
            $classesNode    = $packageNode->getElementsByTagName("classes")->item(0);
            $functionNode   = $packageNode->getElementsByTagName("functions")->item(0);
        }
        else
        {
            $packageNode    = $packageXML->createElement("package");
            $classesNode    = $packageXML->createElement("classes");
            $functionNode   = $packageXML->createElement("functions");
            $packageNode->appendChild($classesNode);
            $packageNode->appendChild($functionNode);
            $packageXML->appendChild($packageNode);
        }
        $header = $packageNode->getElementsByTagName("header");
        if($header->length == 0)
        {
            $packageNode->appendChild($packageXML->importNode($fileNode->getElementsByTagName("header")->item(0), true));
        }
        else if($header->item(0)->nodeValue == "" && $fileNode->getElementsByTagName("header")->length > 0)
        {
            try {
                $packageNode->replaceChild($packageXML->importNode($fileNode->getElementsByTagName("header")->item(0), true), $header->item(0));
            }
            catch(DOMException $e) { }
        }
        //add blocks to it
        $blocks = $fileNode->getElementsByTagName("class");
        foreach($blocks as $block)
        {
            $classesNode->appendChild($packageXML->importNode($block, true));
        }
        $blocks = $fileNode->getElementsByTagName("function");
        foreach($blocks as $block)
        {
            $classesNode->appendChild($packageXML->importNode($block, true));
        }
        $packageXML->formatOutput = true;
        if(!file_exists(dirname($packageFile)))
        {
            mkdir(dirname($packageFile), 0777, true);
        }
        $packageXML->save($packageFile);

        //update refs
        $refFile = PHPDocConfig::get("output")."/xml/_refs.xml";
        $refDom = new DOMDocument("1.0", "utf-8");
        $refDom->formatOutput = true;
        $refDom->preserveWhiteSpace = false;

        if(file_exists($refFile))
        {
            $refDom->load($refFile);
            $refNode = $refDom->getElementsByTagName("refs")->item(0);
        }
        else
        {
            $refNode = $refDom->createElement("refs");
            $refDom->appendChild($refNode);
        }

        foreach($refs["items"] as $id => $ref)
        {
            $r = $refDom->createElement("ref");
            $r->setAttribute("id", $id);
            $r->appendChild($refDom->createElement("file", $ref->getFile()));
            $r->appendChild($refDom->createElement("name", $ref->name));
            $r->appendChild($refDom->createElement("package", $ref->getPackage()));
            $refNode->appendChild($r);
        }

        $refDom->save($refFile);
    }
    /**
     * parse the class header
     * 
     * @param PHPDocClass $classInfo
     * @param string $line
     */
    private function parseClassHeader($classInfo, $line)
    {
        $classInfo->abstract = (preg_match('/^abstract/', $line) === 1);
        $matches = array();
        if(preg_match('/^(abstract)? ?class ([a-zA-Z1-9_]*)/', $line, $matches) > 0)
        {
            $classInfo->name = trim($matches[2]);
            $classInfo->interface = false;
    
            $matches = array();
            preg_match('/extends ([a-zA-Z1-9_]*)/', $line, $matches);
            if(count($matches) > 0)
            {
                $classInfo->extends = trim($matches[1]);
            }
    
            $matches = array();
            preg_match('/implements ([a-zA-Z1-9_, ]*)/', $line, $matches);
            if(count($matches) > 0)
            {
                $classInfo->implements = explode(",", trim($matches[1]));
                array_walk($classInfo->implements, create_function('&$a', '$a = trim($a);'));
            }
        }
        else if(preg_match('/^interface ([a-zA-Z1-9_]*)/', $line, $matches) > 0)
        {
            $classInfo->name = trim($matches[1]);
            $classInfo->interface = true;
        }
    }
    /**
     * @param PHPDocMethod $methodInfo
     * @param $line
     */
    private function parseMethodHeader($methodInfo, $line)
    {
        $methodInfo->abstract = (preg_match('/^abstract/', $line) === 1);
        $matches = array();
        preg_match('/(public|private|protected)?/', $line, $matches);
        if(isset($matches[1]))
        {
            $methodInfo->accessor = $matches[1];
        }

        $methodInfo->static = (preg_match('/(abstract)? ?(public|private|protected)? static function/', $line) === 1);

        $matches = array();
        preg_match('/function (.*?) ?\(/', $line, $matches);
        $methodInfo->name = $matches[1];

    }
    private function parseFunctionHeader($functionInfo, $line)
    {
        $matches = array();
        preg_match('/function (.*?) ?\(/', $line, $matches);
        $functionInfo->name = $matches[1];
    }
}
class PHPDoc
{
    private $excludes;

    public function __construct() { }

    public function process()
    {
        $this->excludes = array();
        $excludes = PHPDocConfig::get("exclude");
        foreach($excludes as $exclude)
        {
            if(file_exists($exclude))
            {
                $this->excludes[] = realpath($exclude);
            }
        }
        //clear the target dir
        if(file_exists(PHPDocConfig::get("output")."/xml/"))
        {
            $files = scandir(PHPDocConfig::get("output")."/xml/");

            foreach($files as $file)
            {
                if($file != '.' && $file != "..")
                {
                    unlink(PHPDocConfig::get("output")."/xml/".$file);
                }
            }
        }
        $this->processDir(PHPDocConfig::get("input"));
    }

    public function render()
    {
        $className = "Output".ucfirst(strtolower(PHPDocConfig::get("outputType")));
        if(file_exists("./types/class.{$className}.inc"))
        {
            include "./types/class.{$className}.inc";
            $writer = new $className();
            $writer->process();
        }
    }

    private function processDir($dir)
    {
        $files = scandir($dir);

        foreach($files as $file)
        {
            if($file != "." && $file != ".." && !in_array($file, PHPDocConfig::get("ignore")))
            {
                if(is_dir($dir."/".$file))
                {
                    if(!in_array(realpath($dir."/".$file), $this->excludes))
                    {
                        $this->processDir($dir."/".$file);
                    }
                }
                else
                {
                    $matches = explode(";", PHPDocConfig::get("filesTypes"));

                    foreach($matches as $matche)
                    {
                        if(fnmatch(trim($matche), $file))
                        {
                            $file = new PHPFileDoc($dir."/".$file);
                            $file->process();
                            break;
                        }
                    }
                }
            }
        }
    }
}

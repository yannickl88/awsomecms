<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package lib
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
class BBCodeException extends Exception { }

interface BBCodeItem
{
    function getType();
    function text();
    function isInline();
    function valueOf($level);
}
interface BBCodeContainer
{
    function addChild($child);
}
class BBCodeContentBlock implements BBCodeItem
{
    private $text;
    private $inline;
    
    public function __construct($text, $inline = true)
    {
        $this->text = nl2br($text, true);
        $this->inline = $inline;
    }
    public static function add($element, $text, $inline)
    {
        $hadEnter = false;
        $block = "";
        $lines = explode("\n", $text);
        $count = 1;
        
        foreach($lines as $line)
        {
            $tLine = trim($line);
            
            if(empty($tLine))
            {
                $hadEnter = true;
            }
            else
            {
                $block .= $line;
                $hadEnter = false;
            }
            
            if(!empty($block) && $hadEnter)
            {
                $element->addChild(new BBCodeContentBlock($block, $inline));
                $block = "";
                $hadEnter = false;
            }
            $count++;
        }
        
        if(!empty($block))
        {
            $element->addChild(new BBCodeContentBlock($block, $inline));
        }
    }
    public function getType()
    {
        if($this->inline)
        {
            return false;
        }
        else
        {
            return "TEXT";
        }
    }
    public function isInline()
    {
        return true;
    }
    public function valueOf($level)
    {
        if($this->inline)
        {
            return $level."  {I} '{$this->text}'";
        }
        else
        {
            return $level."  {B} '{$this->text}'";
        }
    }
    public function text()
    {
        if($this->inline)
        {
            return $this->text;
        }
        else
        {
            return "<p>{$this->text}</p>\n";
        }
    }
}
class BBCodeDummy implements BBCodeItem, BBCodeContainer
{
    public $inline = true;
    
    public function getType()
    {
        return false;
    }
    public function text()
    {
        return "";
    }
    public function isInline()
    {
        return $this->inline;
    }
    public function valueOf($level)
    {
        return "";
    }
    public function addChild($child)
    {
        return;
    }
}
class BBCodeTag implements BBCodeItem, BBCodeContainer
{
    private $type;
    protected $children;
    private $params;
    private $end;
    
    public $id;
    
    public function __construct($tag)
    {
        $this->children = array();
        $this->params = array();
        $this->end = false;
        $this->parseTag($tag);
        $this->id = md5(rand(0, 9999)."blaat".microtime());
    }
    public function addChild($child)
    {
        array_push($this->children, $child);
    }
    public function getLastChild()
    {
        if(count($this->children) > 0)
        {
            return $this->children[count($this->children) - 1];
        }
        else
        {
            return new BBCodeDummy();
        }
    }
    public function getType()
    {
        return $this->type;
    }
    public function isEnd()
    {
        return $this->end;
    }
    public function hasParam($key)
    {
        return isset($this->params[$key]);
    }
    public function getParam($key)
    {
        return $this->params[$key];
    }
    private function parseTag($tag)
    {
        $tag = substr($tag, 1, -1);
        
        if($tag[0] == "/")
        {
            $this->end = true;
            $tag = substr($tag, 1);
        }
        $tagArr = explode(" ", $tag);
        if(strpos($tagArr[0], "=") === false)
            $this->type = array_shift($tagArr);
        else
            $this->type = substr($tagArr[0], 0, strpos($tagArr[0], "="));
        
        foreach($tagArr as $param)
        {
            if(strpos($param, "=") !== false)
            {
                $paramArr = explode("=", $param);
                $this->params[$paramArr[0]] = $paramArr[1];
            }
            else
            {
                $this->params[$param] = true;
            }
        }
    }
    public function text()
    {
        $html = "";
        
        foreach($this->children as $child)
        {
            switch($child->getType())
            {
                case "b":
                    $html .= "<b>".$child->text()."</b>";
                    break;
                case "i":
                    $html .= "<i>".$child->text()."</i>";
                    break;
                case "u":
                    $html .= "<u>".$child->text()."</u>";
                    break;
                case "h1":
                    $html .= "<h1>".$child->text()."</h1>\n";
                    break;
                case "h2":
                    $html .= "<h2>".$child->text()."</h2>\n";
                    break;
                case "img":
                    $html .= "<img src='{$child->text()}' />";
                    break;
                case "url":
                    if($child->hasParam("url"))
                    {
                        $html .= "<a href='{$child->getParam("url")}'>".$child->text()."</a>";
                    }
                    else
                    {
                        $html .= "<a href='{$child->text()}'>".$child->text()."</a>";
                    }
                    break;
                case "left":
                    $html .= "<div style='text-align: left;'>\n".$child->text()."</div>\n";
                    break;
                case "center":
                    $html .= "<div style='text-align: center;'>\n".$child->text()."</div>\n";
                    break;
                case "right":
                    $html .= "<div style='text-align: right;'>\n".$child->text()."</div>\n";
                    break;
                default:
                    $html .= $child->text();
                    break;
            }
        }
        
        return $html;
    }
    public function isInline()
    {
        switch($this->getType())
        {
            case "left":
            case "center":
            case "right":
                return false;
                break;
            case "b":
            case "i":
            case "u":
            case "h1":
            case "h2":
            case "url":
            case "img":
            default:
                return true;
                break;
        }
    }
    public function valueOf($level)
    {
        $return = $level."  {$this->getType()}";
        foreach($this->children as $child)
        {
            $return .= "\n".$child->valueOf($level."  ");
        }
        
        return $return;
    }
}
class ContentRoot extends BBCodeTag {
    public function isInline()
    {
        return false;
    }
    public function valueOf($level)
    {
        $return = "ROOT";
        foreach($this->children as $child)
        {
            $return .= "\n".$child->valueOf("  ");
        }
        
        return $return;
    }
}
/**
 * BBCode parser, which generates XHTML valid HTML code
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BBCodeParser
{
    private $text;
    private $blocks;
    private $paragraphs;
    /**
     * Constructor
     * 
     * @param String $string
     */
    private function __construct($string)
    {
        $this->text = $string;
        $this->blocks = explode("\n", trim($string));
        $this->paragraphs = array();
    }
    /**
     * Process the given text input and return the HTML string
     * 
     * @return String
     */
    private function process()
    {
        $queue = array(new ContentRoot());
        
        $offset = 0;
        $lastEnd = -1;
        
        while(($s = strpos($this->text, "[", $offset)) !== false)
        {
            $end = strpos($this->text, "]", $s);
            
            if($end === false)
                throw new BBCodeException("No ending ] found at {$s}");
            
            if($lastEnd >= 0)
            {
                $text = substr($this->text, $lastEnd + 1, $s - $lastEnd - 1);
            }
            $tag = new BBCodeTag(substr($this->text, $s, $end - $s + 1));
            
            if($tag->isEnd())
            {
                if($queue[0]->getType() != $tag->getType())
                    throw new BBCodeException("Cannot close {$tag->getType()}");
                
                $openTag = array_shift($queue);
                $queue[0]->addChild($openTag);
                //add the gathered text
                if(!empty($text))
                {
                    BBCodeContentBlock::add($openTag, $text, $queue[0]->getLastChild()->isInline());
                }
            }
            else
            {
                //add the gathered text
                if(!empty($text))
                {
                    BBCodeContentBlock::add($queue[0], $text);
                }
                array_unshift($queue, $tag);
            }
            $offset = $s + 1;
            $lastEnd = $end;
        }
        if($lastEnd >= 0)
        {
            $text = substr($this->text, $lastEnd + 1);
            BBCodeContentBlock::add($queue[0], $text, $queue[0]->getLastChild()->isInline());
        }
        
        return $queue[0]->text();
    }
    
    private function parseTag($tag)
    {
    }
    /**
     * Parse a string from BBCode to HTML
     * 
     * @param String $value
     * @return String
     */
    public static function parse($value)
    {
        if(Config::get("type", "wysiwig", "input") == "bbcode")
        {
            $parser = new TextParser($value);
            return $parser->process();
        }
        else
        {
            return $value;
        }
    }
}
class Element
{
    protected $type;
    protected $children;
    protected $attributes;
    
    public function __construct($type)
    {
        $this->children = array();
        $this->attributes = array();
        
        $this->type = $this->parseTag($type);
    }
    protected function parseTag($tag)
    {
        $tagArr = explode(" ", $tag);
        if(strpos($tagArr[0], "=") === false)
            $type = array_shift($tagArr);
        else
            $type = substr($tagArr[0], 0, strpos($tagArr[0], "="));
        
        foreach($tagArr as $param)
        {
            if(strpos($param, "=") !== false)
            {
                $paramArr = explode("=", $param);
                $this->attributes[$paramArr[0]] = $paramArr[1];
            }
            else
            {
                $this->attributes[$param] = true;
            }
        }
        
        return $type;
    }
    public function addChild($child)
    {
        array_push($this->children, $child);
    }
    public function getLastChild()
    {
        if(count($this->children) > 0)
            return $this->children[count($this->children) - 1];
        else
            return new DummyElement();
    }
    public function setAttr($key, $value)
    {
        $this->attributes[$key] = $value;
    }
    public function hasAttr($key)
    {
        return isset($this->attributes[$key]);
    }
    public function getAttr($key)
    {
        return $this->attributes[$key];
    }
    public function type()
    {
        return $this->type;
    }
    public function isBlock()
    {
        switch($this->type())
        {
            case "h1":
            case "h2":
            case "left":
            case "center":
            case "right":
            case "DOCROOT":
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    public function isPar()
    {
        switch($this->type())
        {
            case "left":
            case "center":
            case "right":
            case "DOCROOT":
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    public function text2par()
    {
        $curPar = null;
        
        foreach($this->children as $key => $child)
        {
            if($child instanceof Text && $this->isBlock())
            {
                if($curPar == null)
                {
                    $curPar = new Paragraph();
                    $curPar->addChild($child);
                    $this->children[$key] = $curPar;
                }
                else
                {
                    if($child->isEnter() && $curPar->hadEndingEnter())
                    {
                        $curPar = new Paragraph();
                        $curPar->addChild($child);
                        $this->children[$key] = $curPar;
                    }
                    else
                    {
                        $curPar->addChild($child);
                        unset($this->children[$key]);
                    }
                }
            }
            else if(!($child instanceof Paragraph))
            {
                $child->text2par();
                
                if($child->isBlock())
                {
                    $curPar = null;
                }
                else
                {
                    if($curPar == null)
                    {
                        $curPar = new Paragraph();
                        $curPar->addChild($child);
                        $this->children[$key] = $curPar;
                    }
                    else
                    {
                        $curPar->addChild($child);
                        unset($this->children[$key]);
                    }
                }
            }
        }
    }
    public function text()
    {
        $html = "";
        
        foreach($this->children as $child)
        {
            $html .= $child->text();
        }
        
        return $html;
    }
}
class Document extends Element
{
    public function __construct()
    {
        parent::__construct("DOCROOT");
    }
}
class DummyElement extends Element
{
    public function __construct()
    {
        parent::__construct("DUMMY");
    }
    
    public function text() { return ""; }
}
class Text extends Element
{
    private $value;
    
    public function __construct($text)
    {
        parent::__construct("TEXT");
        $this->value = $text;
    }
    public function text() { return nl2br($this->value, true); }
    public function value() { return $this->value; }
    public function isEnter() { return ($this->value == "\n" || $this->value == "\r"); }
}
class Paragraph extends Element
{
    private $hadEnter = false;
    public function __construct($text)
    {
        parent::__construct("PAR");
        $this->value = $text;
    }
    
    public function addChild($child)
    {
        if($child instanceof Text && $child->isEnter())
        {
            $this->hadEnter = true;
        }
        else
        {
            $this->hadEnter = false;
            parent::addChild($child);
        }
    }
    public function hadEndingEnter()
    {
        return $this->hadEnter;
    }
    public function isEmpty()
    {
        return (count($this->children) == 0);
    }
    public function text()
    {
        $html = "";
        
        if(!$this->isEmpty())
        {
            $html .= "<p>";
            foreach($this->children as $child)
            {
                $html .= $child->text();
            }
            $html .= "</p>";
        }
        return $html;
    }
}
class TextParser 
{
    const READING       = 0;
    const READINGPAR    = 3;
    const TAGREAD       = 1;
    const TAGENDREAD    = 2;
    
    private $text;
    private $blockTags;
    
    public function __construct($text)
    {
        $this->text = $text;
        $this->blockTags = array("center");
    }
    public function process()
    {
        $state = TextParser::READING;
        $tag = '';
        $text = '';
        $tagStack = array(new Document());
        $countFromStateChange = 0;
        
        for($i = 0; $i < strlen($this->text); $i++)
        {
            $char = $this->text[$i];
            if($state == TextParser::READING)
            {
                if($char == "[")
                {
                    if($text != "")
                    {
                        $tagStack[0]->addChild(new Text($text));
                        $text = "";
                    }
                    
                    $state = TextParser::TAGREAD;
                    $countFromStateChange = 0;
                }
                else if($char == "\n" || $char == "\r")
                {
                    if($text != "")
                    {
                        $tagStack[0]->addChild(new Text($text));
                        $text = "";
                    }
                    else if($text == "")
                    {
                        $tagStack[0]->addChild(new Text($char));
                    }
                }
                else
                {
                    $text .= $char;
                }
            }
            else if($state == TextParser::TAGREAD)
            {
                if($char == "/" && $countFromStateChange == 1)
                {
                    $state = TextParser::TAGENDREAD;
                    $countFromStateChange = 0;
                }
                else if($char == "]")
                {
                    array_unshift($tagStack, new Element($tag));
                    $tag = '';
                    $state = TextParser::READING;
                    $countFromStateChange = 0;
                }
                else if($char == "\n" || $char == "\r") { } //Nothin on enter
                else
                {
                    $tag .= $char;
                }
            }
            else if($state == TextParser::TAGENDREAD)
            {
                if($char == "]")
                {
                    if($tagStack[0]->type() == $tag)
                    {
                        $e = array_shift($tagStack);
                        $tagStack[0]->addChild($e);
                    }
                    else
                    {
                        throw new Exception("Cannot end '{$tag}' without opening it first");
                    }
                    $tag = '';
                    $state = TextParser::READING;
                    $countFromStateChange = 0;
                }
                else if($char == "\n" || $char == "\r") { } //Nothin on enter
                else
                {
                    $tag .= $char;
                }
            }
            
            $countFromStateChange++;
        }
        
        if($text != "")
        {
            $tagStack[0]->addChild(new Text($text));
            $text = "";
        }
        
        $tagStack[0]->text2par();
        
        return $tagStack[0]->text();
    }
    
}
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
        $this->text = nl2br(trim($text), true);
        $this->inline = $inline;
    }
    public static function add($element, $text)
    {
        $hadEnter = false;
        $block = "";
        $lines = explode("\n", $text);
        $count = 1;
        
        foreach($lines as $line)
        {
            $line = trim($line);
            if(!empty($line) && $hadEnter)
            {
                $element->addChild(new BBCodeContentBlock($block, $element->isInline()));
                $block = "";
                $hadEnter = false;
            }
            
            if(empty($line))
            {
                $hadEnter = true;
            }
            else
            {
                if($count < count($lines))
                {
                    $block .= $line."\n";
                }
                else
                {
                    $block .= $line;
                }
                $hadEnter = false;
            }
            $count++;
        }
        
        if(!empty($block))
        {
            $element->addChild(new BBCodeContentBlock($block, $element->isInline()));
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
class BBCodeTag implements BBCodeItem, BBCodeContainer
{
    private $type;
    protected $children;
    private $params;
    private $end;
    
    public function __construct($tag)
    {
        $this->children = array();
        $this->params = array();
        $this->end = false;
        $this->parseTag($tag);
    }
    public function addChild($child)
    {
        array_push($this->children, $child);
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
                    $html .= "<img src='{$child->text()}' />\n";
                    break;
                case "url":
                    if($child->hasParam("url"))
                    {
                        $html .= "<a href='{$child->getParam("url")}'>".$child->text()."</a>\n";
                    }
                    else
                    {
                        $html .= "<a href='{$child->text()}'>".$child->text()."</a>\n";
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
            case "b":
            case "TEXT":
            case "i":
            case "u":
            case "h1":
            case "h2":
            case "url":
            case "img":
                return true;
                break;
            case "left":
            case "center":
            case "right":
            default:
                return false;
                break;
        }
    }
}
class ContentRoot extends BBCodeTag { }
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
                    BBCodeContentBlock::add($openTag, $text);
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
            BBCodeContentBlock::add($queue[0], $text);
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
            $parser = new BBCodeParser($value);
            return $parser->process();
        }
        else
        {
            return $value;
        }
    }
}
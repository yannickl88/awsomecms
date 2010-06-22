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

/**
 * A paragraph within a text
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BBCodeParagraph
{
    private $blocks;
    private $end;
    /**
     * Constructor
     */
    public function __construct()
    {
        $this->blocks = array();
        $this->end = false;
    }
    /**
     * Add a text block to the paragraph
     * 
     * @param String $block
     */
    public function addBlock($block)
    {
        if(!empty($block))
        {
            $blockObj = new BBCodeText($block);
            $this->blocks[] = $blockObj;
            
            if(!$blockObj->inline())
            {
                $this->end = true;
            }
        }
        else
        {
            $this->end = true;
        }
    }
    /**
     * Is the text inputed at the end of the paragraph
     * 
     * @return Boolean
     */
    public function end()
    {
        return $this->end;
    }
    /**
     * Is the paragraph empty
     * 
     * @return Boolean
     */
    public function isEmpty()
    {
        return count($this->blocks) == 0;
    }
    /**
     * Get the text generated from the paragraph.
     * 
     * @return String
     */
    public function text()
    {
        $text = "";
        
        if(count($this->blocks) == 1 && !$this->blocks[0]->inline())
        {
            $text = $this->blocks[0]->text()."\n";
        }
        else
        {
            $text .= "<p>";
            
            foreach($this->blocks as $block)
            {
                $text .= $block->text();
            }
            
            $text .= "</p>\n";
        }
        
        return $text;
    }
}
/**
 * Text block within a paragraph
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BBCodeText
{
    const DUAL      = 1;
    const SINGLE    = 2;
    const WRAP      = 3;
    // Block tags
    public static $blockTags = array(
        "h1"            => array(BBCodeText::DUAL, 'h1'),
        "h2"            => array(BBCodeText::DUAL, 'h2'),
        "left"          => array(BBCodeText::DUAL, 'div style="text-align: left;"', 'div'),
        "center"        => array(BBCodeText::DUAL, 'div style="text-align: center;"', 'div'),
        "right"         => array(BBCodeText::DUAL, 'div style="text-align: right;"', 'div'),
    );
    // Inline tags
    public static $inlineTags = array(
        "img"           => array(BBCodeText::WRAP, 'img alt="" src='),
        "url"           => array(BBCodeText::DUAL, 'a href=', 'a' ),
        "b"             => array(BBCodeText::DUAL, 'b'),
        "i"             => array(BBCodeText::DUAL, 'i'),
        "u"             => array(BBCodeText::DUAL, 'u'),
    );
    
    private $inline;
    private $text;
    /**
     * Constructor
     * 
     * @param String $text
     */
    public function __construct($text)
    {
        $this->text = $text;
        $this->inline = true;
        
        $this->parse();
    }
    /**
     * Is the Textblock inline (is not starting and ending with the same blocktag)
     * 
     * @return Boolean
     */
    public function inline()
    {
        return $this->inline;
    }
    /**
     * Parse the text from BBCode to HTML
     */
    private function parse()
    {
        //check if it starts with a block tag
        $matches = array();
        if(preg_match('/^\[(.*?)\].*\[\/(.*?)\]$/', $this->text, $matches) > 0)
        {
            if($matches[1] == $matches[2] && isset(BBCodeText::$blockTags[$matches[1]]))
            {
                $this->inline = false;
            }
        }
        
        $this->text = preg_replace_callback('/(\[(.*?)\])/', array($this, "processTag"), $this->text);
    }
    /**
     * Helper function to proccess the found tags
     * 
     * @param Array $matches
     * @return String
     */
    private function processTag($matches)
    {
        $tags = array_merge(BBCodeText::$blockTags, BBCodeText::$inlineTags);
        $end = false;
        $hasParams = false;
        $param = '';
        
        //is ending tag?
        $tag = $matches[2];
        if($tag[0] == "/")
        {
            $end = true;
            $tag = substr($tag, 1);
        }
        
        if(strpos($tag, " ") !== false)
        {
            $tag = substr($tag, 0, strpos($tag, " "));
            $hasParams = true;
        }
        if(strpos($tag, "=") !== false)
        {
            $param = '"'.substr($tag, strpos($tag, "=")+1).'"';
            $tag = substr($tag, 0, strpos($tag, "="));
            $hasParams = true;
        }
        
        if(isset($tags[$tag]))
        {
            $HTMLTag = $tags[$tag];
            
            switch($HTMLTag[0])
            {
                case BBCodeText::WRAP:
                    if($end && count($HTMLTag) == 3)
                    {
                        return "</".$HTMLTag[2].">";
                    }
                    return ($end? "\" />": "<".$HTMLTag[1].$param."\"");
                    break;
                case BBCodeText::DUAL:
                    if($end && count($HTMLTag) == 3)
                    {
                        return "</".$HTMLTag[2].">";
                    }
                    return "<".($end? "/": "").$HTMLTag[1].$param.">";
                    break;
            }
        }
        
        return $matches[0];
    }
    /**
     * Get the text generated from this block
     * 
     * @return String
     */
    public function text()
    {
        return $this->text;
    }
}
/**
 * BBCode parser, which generates XHTML valid HTML code
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BBCodeParser
{
    private $blocks;
    private $paragraphs;
    /**
     * Constructor
     * 
     * @param String $string
     */
    private function __construct($string)
    {
        $this->blocks = explode("\n", trim($string));
        $this->paragraphs = array();
    }
    /**
     * Devide the input string into paragraphs of text
     */
    private function makeParagraphs()
    {
        $par = new BBCodeParagraph();
        
        foreach($this->blocks as $block)
        {
            $block = trim($block);
            
            $par->addBlock($block);
            
            if($par->end())
            {
                $this->paragraphs[] = $par;
                $par = new BBCodeParagraph();
            }
        }
        
        if(!$par->isEmpty())
        {
            $this->paragraphs[] = $par;
        }
    }
    /**
     * Process the given text input and return the HTML string
     * 
     * @return String
     */
    private function process()
    {
        $this->makeParagraphs();
        
        $text = "";
        
        foreach($this->paragraphs as $par)
        {
            $text .= $par->text();
        }
        
        return $text;
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
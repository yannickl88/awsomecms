<?php
/**
 * Exception which is triggered if something goes wrong
 */
class BBCodeException extends Exception { }
/**
 * And BBCode element which is linked to an HTML element
 */
class BBCodeElement
{
    protected $type;
    protected $parent;
    protected $next;
    protected $prev;
    protected $children;
    protected $attributes;
    /**
     * Constructor
     *
     * @param string $type
     */
    public function __construct($type)
    {
        $this->children = array();
        $this->attributes = array();

        $this->type = $type;
    }
    /**
     * Init an element dynamicly based on the tag
     * @param string $tag
     * @return BBCodeElement
     * @throws BBCodeException
     */
    public static function init($tag)
    {
        list($type, $attributes) = BBCodeElement::parseTag($tag);

        $className = ucfirst($type)."Element";
        if(class_exists($className))
        {
            $obj = new $className($type);
        }
        else
        {
            $obj = new BBCodeUnknownElement($type);
        }

        foreach($attributes as $key => $value)
        {
            $obj->setAttr($key, $value);
        }

        return $obj;
    }
    /**
     * Parse the tag name for type and arguments passed
     *
     * @param string $tag
     * @return array        array(String, Array<String, String>)
     *                      where the first element is the tag and the second a key value array with arguments
     */
    public static function parseTag($tag)
    {
        $tagArr = explode(" ", $tag);
        $attributes = array();
        $specialTypes = array("*" => "item");

        if(strpos($tagArr[0], "=") === false)
        {
            $type = strtolower(array_shift($tagArr));
        }
        else
        {
            $type = strtolower(substr($tagArr[0], 0, strpos($tagArr[0], "=")));
        }

        if(key_exists($type, $specialTypes))
        {
            $type = $specialTypes[$type];
        }

        foreach($tagArr as $param)
        {
            if(strpos($param, "=") !== false)
            {
                $paramArr = explode("=", $param);
                $attributes[$paramArr[0]] = $paramArr[1];
            }
            else
            {
                $attributes[$param] = true;
            }
        }

        return array($type, $attributes);
    }
    /**
     * Get the parent or null if not set
     *
     * NOTE: this is only set when added to another element using the BBCodeElement::addChild()
     *
     * @return BBCodeElement
     */
    public function getParent()
    {
        return $this->parent;
    }
    /**
     * Check if the element doesn't need a closing tag and can only be a singel line
     *
     * @return Boolean
     */
    public function singelLine()
    {
        return false;
    }
    /**
     * Check if the element can have line break after it
     *
     * @return Boolean
     */
    public function removeBreak()
    {
        return false;
    }
    /**
     * Check if the element doesn't need a closing tag
     *
     * @return Boolean
     */
    public function singelTag()
    {
        return false;
    }
    /**
     * Add a Child to the element
     *
     * @param BBCodeElement $child
     */
    public function addChild($child)
    {
        $child->parent = $this;
        array_push($this->children, $child);
    }
    /**
     * Get the last child in this element
     *
     * NOTE: if there is no child, this will return a BBCodeDummyElement rather then null or false
     *
     * @return BBCodeElement
     */
    public function getLastChild()
    {
        if(count($this->children) > 0)
        return $this->children[count($this->children) - 1];
        else
        return new BBCodeDummyElement();
    }
    /**
     * Set an attribute on this element
     *
     * @param String $key
     * @param String $value
     */
    public function setAttr($key, $value)
    {
        $this->attributes[$key] = $value;
    }
    /**
     * Check if the element has an attribute set
     * @param String $key
     * @return Boolean
     */
    public function hasAttr($key)
    {
        return isset($this->attributes[$key]);
    }
    public function hasChildren()
    {
        return (count($this->children) > 0);
    }
    /**
     * Get an attribute of this element
     *
     * @param String $key
     * @return String
     */
    public function getAttr($key)
    {
        return $this->attributes[$key];
    }
    /**
     * Get the type of this element, this is the tag used in the BBCode
     *
     * @return String
     */
    public function type()
    {
        return $this->type;
    }
    /**
     * Check if this is a block element
     *
     * @return Boolean
     */
    public function isBlock()
    {
        switch($this->type())
        {
            case "DOCROOT":
            case "PAR":
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    /**
     * Check if the element may contain paragraphs.
     * Overwrite this in your elements
     *
     * @return Boolean
     */
    protected function mayContainPars()
    {
        return true;
    }
    /**
     * Wrap the content of the element
     *
     * @param String $text
     */
    protected function wrap($text)
    {
        return $text;
    }
    /**
     * Wrap the content of the element (without formatting)
     *
     * @param String $text
     */
    protected function plain($text)
    {
        if($this->isBlock())
            return $text . "\n";
        return $text;
    }
    /**
     * Wrap paragraphs around groups of elements and create new ones on new line elements
     */
    public function text2par()
    {
        $curPar = null;
        $prevChild = new BBCodeDummyElement();
        $hadParSep = false;

        foreach($this->children as $key => $child)
        {
            if($this->isBlock() && $this->mayContainPars())
            {
                if($child->type == "PARSEP")
                {
                    if($hadParSep || $prevChild->isBlock())
                    {
                        $curPar = null;
                    }
                    else
                    {
                        $hadParSep = true;
                        $curPar->addChild($child);
                    }
                    unset($this->children[$key]);
                }
                else
                {
                    if($child->isBlock())
                    {
                        $hadParSep = false;
                        $curPar = null;

                        if($child->isBlock())
                        {
                            $child->text2par();
                        }
                    }
                    else if(!$child->isBlock())
                    {
                        $hadParSep = false;
                        if($curPar == null)
                        {
                            $curPar = new BBCodeParagraph();
                            $this->children[$key] = $curPar;
                        }
                        else
                        {
                            unset($this->children[$key]);
                        }

                        $curPar->addChild($child);
                    }
                    $prevChild = $child;
                }
            }
            if($this->isBlock() && !$this->mayContainPars())
            {
                if($child->isBlock())
                {
                    $child->text2par();
                }
            }
        }
        $this->children = array_merge(array(), $this->children); //reset the keys
    }
    /**
     * Get the HTML, including the wrapping, of the element
     *
     * @return String
     */
    public function text($formatting)
    {
        $html = "";
        $lastElement = null;

        foreach($this->children as $child)
        {
            if($lastElement != null && is_a($child, "BBCodeNewLine") && $lastElement->removeBreak())
                continue;
            $html .= $child->text($formatting);
            $lastElement = $child;
        }
        if($formatting)
            return $this->wrap($html);
        else
            return $this->plain($html);
    }
    /**
     * Debug for showing the element tree from this element
     *
     * @param String $level
     * @param int $i
     * @return String
     */
    public function valueOf($level, $i)
    {
        $return = $level."[{$i}] {$this->type()}";
        foreach($this->children as $key => $child)
        {
            $return .= "\n".$child->valueOf($level."    ", $key);
        }

        return $return;
    }
    /**
     * Remove trailing breaks so don't end a block with a return
     */
    public function removeTrailingBreak()
    {
        foreach($this->children as $child)
        {
            if($child->isBlock())
            {
                $child->removeTrailingBreak();
            }
        }

        while($this->getLastChild()->type() == "PARSEP")
        {
            array_pop($this->children);
        }
    }
}
/**
 * BBCode Document root
 */
class BBCodeDocument extends BBCodeElement
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("DOCROOT");
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::valueOf()
     */
    public function valueOf()
    {
        $return = "ROOT";
        foreach($this->children as $key => $child)
        {
            $return .= "\n".$child->valueOf("", $key);
        }

        return $return;
    }
}
/**
 * Dummy element, this is used for checking stuff
 */
class BBCodeDummyElement extends BBCodeElement
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("DUMMY");
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::text()
     */
    public function text() { return ""; }
}
/**
 * Element which only contains text
 */
class BBCodeText extends BBCodeElement
{
    private $value;
    /**
     * Constructor
     */
    public function __construct($text)
    {
        parent::__construct("TEXT");
        $this->value = $text;
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::text()
     */
    public function text() { return $this->value; }
}
/**
 * Element which indecates a new line
 */
class BBCodeNewLine extends BBCodeElement
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("PARSEP");
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::text()
     */
    public function text()
    {
        return "<br />";
    }
}
/**
 * Element which is a paragraph in the document
 */
class BBCodeParagraph extends BBCodeElement
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("PAR");
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::wrap()
     */
    public function wrap($text)
    {
        if(!empty($text))
            return "<p>{$text}</p>";
    }
}
/**
 * Element which is a handler when there is no element to be found
 */
class BBCodeUnknownElement extends BBCodeElement
{
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::wrap()
     */
    public function wrap($text)
    {
        //check if we have a smarty handler somewhere
        $smartyFunction = "smarty_function_".$this->type();

        if(function_exists($smartyFunction))
        {
            return "{gallery{$this->serializeAttr()}}";
        }
        else
        {
            return "";
            throw new BBCodeException("Unknown tag '{$this->type}'");
        }
    }
    /**
     * Create a string which we can use as smarty params
     *
     * @return String
     */
    public function serializeAttr()
    {
        $str = "";

        foreach($this->attributes as $key => $value)
        {
            $str .= " ";

            if(!is_numeric($value))
            {
                $value = "'{$value}'";
            }
            $str .= "{$key}={$value}";
        }

        return $str;
    }
    /**
     * (non-PHPdoc)
     * @see libs/BBCodeElement::singelTag()
     */
    public function singelTag()
    {
        return true;
    }
}
/**
 * The BBCodeParser which does the actual work
 */
class BBCodeParser
{
    const READING       = 0;
    const READINGPAR    = 3;
    const TAGREAD       = 1;
    const TAGENDREAD    = 2;

    private $text;
    private $blockTags;
    /**
     * Parse a string into a XHTML valid string
     *
     * @param String $text
     * @return String
     */
    public static function parse($text, $force = false, $formatting = true)
    {
        if(Config::get("type", "wysiwig", "input") == "bbcode" || $force)
        {
            $parser = new BBCodeParser(htmlentities($text, ENT_NOQUOTES, "UTF-8"));
            return $parser->process($formatting);
        }
        return $text;
    }
    /**
     * Constructor
     *
     * @param String $text
     */
    private function __construct($text)
    {
        $this->text = str_replace(array("\r\n", "\n", "\r"), "[br]", $text);
        $this->blockTags = array("center");
    }
    /**
     * Process the given string and return a XHTML string
     *
     * @return String
     */
    private function process($formatting)
    {
        $state = BBCodeParser::READING;
        $tag = '';
        $text = '';
        $tagStack = array(new BBCodeDocument());
        $countFromStateChange = 0;

        for($i = 0; $i < strlen($this->text); $i++)
        {
            $char = $this->text[$i];
            if($state == BBCodeParser::READING)
            {
                if($char == "[")
                {
                    if($text != "")
                    {
                        $tagStack[0]->addChild(new BBCodeText($text));
                        $text = "";
                    }

                    $state = BBCodeParser::TAGREAD;
                    $countFromStateChange = 0;
                }
                else
                {
                    $text .= $char;
                }
            }
            else if($state == BBCodeParser::TAGREAD)
            {
                if($char == "/" && $countFromStateChange == 1)
                {
                    $state = BBCodeParser::TAGENDREAD;
                    $countFromStateChange = 0;
                }
                else if($char == "]")
                {
                    if($tag == "br")
                    {
                        //check if we had a special char open
                        if($tagStack[0]->singelLine())
                        {
                            $e = array_shift($tagStack);
                            $tagStack[0]->addChild($e);
                        }
                        if($tagStack[0]->hasChildren())
                        {
                            $tagStack[0]->addChild(new BBCodeNewLine());
                        }
                        $tag = '';
                        $state = BBCodeParser::READING;
                        $countFromStateChange = 0;
                    }
                    else
                    {
                        $e = BBCodeElement::init($tag);
                        if($e->singelTag())
                        {
                            $tagStack[0]->addChild($e);
                        }
                        else
                        {
                            array_unshift($tagStack, $e);
                        }
                        $tag = '';
                        $state = BBCodeParser::READING;
                        $countFromStateChange = 0;
                    }
                }
                else if($char == "\n" || $char == "\r") { } //Nothin on enter
                else
                {
                    $tag .= $char;
                }
            }
            else if($state == BBCodeParser::TAGENDREAD)
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
                        $near = substr($this->text, $i - 10, 20);
                        throw new BBCodeException("Cannot end '{$tag}' without opening it first. Near '{$near}'");
                    }
                    $tag = '';
                    $state = BBCodeParser::READING;
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
            $tagStack[0]->addChild(new BBCodeText($text));
            $text = "";
        }

        if(!is_a($tagStack[0], 'BBCodeDocument'))
        {
            throw new BBCodeException("Cannot find end for '{$tagStack[0]->type()}'.");
        }

        $tagStack[0]->text2par();
        $tagStack[0]->removeTrailingBreak();
        return $tagStack[0]->text($formatting);
    }

}
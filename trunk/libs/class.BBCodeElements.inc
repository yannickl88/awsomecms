<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package libs
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import("libs/class.BBCodeParser.inc");

/**
 * This file contains All the supported BBCodeElements and all methods that 
 * are defined are overwrites from BBCodeElement 
 */

/** Text modifiers */
class BElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<b>{$text}</b>";
    }
}
class UElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<u>{$text}</u>";
    }
}
class IElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<i>{$text}</i>";
    }
}
/** Headers */
class H1Element extends BBCodeElement
{
    public function wrap($text)
    {
        return "<h1>{$text}</h1>";
    }
    public function isBlock()
    {
        return true;
    }
    public function mayContainPars()
    {
        return false;
    }
}
class H2Element extends BBCodeElement
{
    public function wrap($text)
    {
        return "<h2>{$text}</h2>";
    }
    public function isBlock()
    {
        return true;
    }
    public function mayContainPars()
    {
        return false;
    }
}
class H3Element extends BBCodeElement
{
    public function wrap($text)
    {
        return "<h3>{$text}</h3>";
    }
    public function isBlock()
    {
        return true;
    }
    public function mayContainPars()
    {
        return false;
    }
}
class H4Element extends BBCodeElement
{
    public function wrap($text)
    {
        return "<h4>{$text}</h4>";
    }
    public function isBlock()
    {
        return true;
    }
    public function mayContainPars()
    {
        return false;
    }
}
/** Media */
class ImgElement extends BBCodeElement
{
    public function wrap($text)
    {
        $style = "";
        
        if($this->hasAttr("width"))
        {
            if($style != "")
                $style .= "; ";
            $style .= "width: {$this->getAttr("width")}px";
        }
        
        if($this->hasAttr("height"))
        {
            if($style != "")
                $style .= "; ";
            $style .= "height: {$this->getAttr("height")}px";
        }
        
        if($this->hasAttr("float"))
        {
            if($style != "")
                $style .= "; ";
            $style .= "float: {$this->getAttr("float")}";
        }
        
        if($style != "")
            $style = "style='{$style}'";
        
        return "<img src='{$text}' {$style} class='bbcodeimg' alt='image'/>";
    }
    public function plain($text)
    {
        return "";
    }
    public function isBlock()
    {
        return $this->hasAttr("float");
    }
    public function mayContainPars()
    {
        return !$this->hasAttr("float");
    }
}
class YoutubeElement extends BBCodeElement
{
    public function wrap($text)
    {
        //http://www.youtube.com/watch?v={key} -> http://www.youtube.com/v/{key}&amp;fs=1
        $matches = array();
        if(preg_match('/^http:\/\/(www\.)?youtube\.com\/watch\?v=([a-zA-Z0-9\-_]*)$/', $text, $matches) === 1)
        {
            $width = 480;
            $height = 385;
            
            if($this->hasAttr("width")) $width = $this->getAttr("width");
            if($this->hasAttr("height")) $height = $this->getAttr("height");
            
            $url = "http://www.youtube.com/v/{$matches[2]}&amp;fs=1";
            return "<object width=\"{$width}\" height=\"{$height}\"><param name=\"movie\" value=\"{$url}\"></param><param name=\"allowFullScreen\" value=\"true\"></param><param name=\"allowscriptaccess\" value=\"always\"></param><embed src=\"{$url}\" type=\"application/x-shockwave-flash\" allowscriptaccess=\"always\" allowfullscreen=\"true\" width=\"{$width}\" height=\"{$height}\"></embed></object>";
        }
        else
        {
            return "NO YOUTUBE VIDEO FOUND<br /> Make you use the format [youtube]http://www.youtube.com/watch?v=&lt;key&gt;[/youtube]";
        }
    }
    public function plain($text)
    {
        return "";
    }
}
/** Linking */
class UrlElement extends BBCodeElement
{
    public function wrap($text)
    {
        if($this->hasAttr("url"))
        {
            return "<a href='{$this->getAttr("url")}'>{$text}</a>";
        }
        else
        {
            return "<a href='{$text}'>{$text}</a>";
        }
    }
}
class MailElement extends BBCodeElement
{
    public function wrap($text)
    {
        if($this->hasAttr("email"))
        {
            return "<a href='mailto:{$this->getAttr("email")}'>{$text}</a>";
        }
        else
        {
            return "<a href='mailto:{$text}'>{$text}</a>";
        }
    }
    public function plain($text)
    {
        return "";
    }
}
/** Alignment */
class LeftElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<div style='text-align: left;'>{$text}</div>";
    }
    public function isBlock()
    {
        return true;
    }
}
class RightElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<div style='text-align: right;'>{$text}</div>";
    }
    public function isBlock()
    {
        return true;
    }
}
class CenterElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<div style='text-align: center;'>{$text}</div>";
    }
    public function isBlock()
    {
        return true;
    }
}
class LinkElement extends BBCodeElement
{
    public function wrap($text)
    {
        $text = addslashes($text);
        $action = isset($this->attributes['goto']) ? $this->attributes['goto'] : "default";
        return "{link text=\"{$text}\" code='{$this->attributes['code']}' action='{$action}'}";
    }
}
class RefElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "{ref code='{$this->attributes['code']}'}";
    }
    public function singelTag()
    {
        return true;
    }
}
/** Lists */
class ListElement extends BBCodeElement
{
    public function wrap($text)
    {
        if($this->hasAttr("list"))
        {
            switch($this->getAttr("list"))
            {
                case "1":
                    return "<ol style='list-style-type: decimal;'>{$text}</ol>";
                    break;
                case "a":
                    return "<ol style='list-style-type: lower-alpha;'>{$text}</ol>";
                    break;
                default:
                    return "<ul>{$text}</ul>";
                    break;
            }
        }
        else 
        {
            return "<ul>{$text}</ul>";
        }
    }
    public function mayContainPars()
    {
        return false;
    }
}
class ItemElement extends BBCodeElement
{
    public function wrap($text)
    {
        if($this->parent->type() != "list")
        {
            throw new BBCodeException("list item must be inside a list, parent is '{$this->parent->type()}'");
        }
        
        return "<li>{$text}</li>";
    }
    public function singelLine()
    {
        return true;
    }
    public function removeBreak()
    {
        return true;
    }
    public function mayContainPars()
    {
        return false;
    }
}
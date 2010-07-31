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
/** Media */
class ImgElement extends BBCodeElement
{
    public function wrap($text)
    {
        return "<img src='{$text}' />";
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
class EmailElement extends BBCodeElement
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
}
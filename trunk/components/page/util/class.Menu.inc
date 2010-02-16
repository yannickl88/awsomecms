<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Menu item that is used in the admin menu
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class MenuItem
{
    public $name;
    public $text;
    public $page;
    public $parent;
    private $children;
    
    /**
     * Constructor
     * 
     * @param string $name
     * @param string $text
     * @param string $page
     * @param string $parent
     */
    public function __construct($name, $page, $parent = "")
    {
        $this->children = array();
        
        $this->name = $name;
        if(empty($text))
        {
            $text = ucfirst($name);
        }
        $this->text = Language::get("menu_".$name);
        $this->page = $page;
        $this->parent = $parent;
    }
    /**
     * Add a menu items to this item, returns true or false depending if succeded
     * 
     * @param MenuItem $child
     * @return boolean
     */
    public function addChild($child)
    {
        if($this->name == $child->parent)
        {
            $this->children[$child->name] = $child;
            usort($this->children, array($this, 'sort'));
            return true;
        }
        else
        {
            foreach($this->children as $childNode)
            {
                if($childNode->addChild($child))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    /**
     * sort the items in this MenuItem
     * 
     * @param MenuItem $child1
     * @param MenuItem $child2
     * @return int
     */
    private function sort($child1, $child2)
    {
        return strcasecmp($child1->text, $child2->text);
    }
    /**
     * Create a HTML represenation of this MenuItem
     * 
     * @return string
     */
    public function toHTML(&$selected)
    {
        //check if this is selected
        $classes = array("adminmenuitem");
        
        if($_GET['url'] == $this->page)
        {
            if(count($this->children) == 0)
            {
                $classes[] = "selected";
            }
            $selected = true;
        }
        
        $child_html = "";
        
        if(count($this->children) > 0)
        {
            $child_html .= "<ul>";
            foreach($this->children as $childNode)
            {
                $child_html .= $childNode->toHTML($selected);
            }
            $child_html .= "</ul>";
        }
        
        
        if($selected)
        {
            $classes[] = "active";
        }
        
        $html = "<li class='".implode(" ", $classes)."'>";
        if(empty($this->page))
        {
            $html .= "<a>{$this->text}";
        }
        else
        {
            $html .= "<a href=\"/{$this->page}\">{$this->text}";
        }
        
        $html .= "</a>";
        $html .= $child_html;
        $html .= "</li>";
        
        return $html;
    }
}
/**
 * Menu class, this is the root element of the menu
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Menu
{
    private static $instance;
    
    private $children;
    private $names;
    private $que;
    
    /**
     * Singelton
     * 
     * @return Menu
     */
    public static function getInstance()
    {
        if(!Menu::$instance)
        {
             Menu::$instance = new Menu();
        }
        
        return Menu::$instance;
    }
    /**
     * Constructor
     */
    private function __construct()
    {
        $this->children = array();
        $this->que = array();
    }
    /**
     * Add a menu items to this menu, returns true or false depending if succeded
     * 
     * @param MenuItem $child
     * @return boolean
     */
    public function addChild($child)
    {
        if($child->parent == "")
        {
            $this->children[$child->name] = $child;
            usort($this->children, array($this, 'sort'));
            return true;
        }
        else
        {
            $this->names[] = $child->name;
            
            foreach($this->children as $childNode)
            {
                if($childNode->addChild($child))
                {
                    return true;
                }
            }
            //hmm, menu item might be missing, let's add it to the que
            array_push($this->que, $child);
            
            //run the que, the missing parents might be there now
            foreach($this->que as $key => $queNode)
            {
                foreach($this->children as $childNode)
                {
                    if($childNode->addChild($queNode))
                    {
                        unset($this->que[$key]);
                    }
                }
            }
        }
    }
    /**
     * Create HTML representation of this menu
     * 
     * @return string
     */
    public function toHTML()
    {
        //run the que, the missing parents might be there now
        while(count($this->que) > 0)
        {
            $foundItem = false;
            $queNode = array_shift($this->que);
            
            foreach($this->children as $childNode)
            {
                if($childNode->addChild($queNode))
                {
                    $foundItem = true;
                    break;
                }
            }
            
            if(!$foundItem && in_array($queNode->parent, $this->names))
            {
                //hmm, menu item is STILL missing, let's add it to the que
                array_push($this->que, $queNode);
            }
        }
        
        
        $html = "<ul>";
        foreach($this->children as $childNode)
        {
            $selected = false;
            $html .= $childNode->toHTML($selected);
        }
        $html .= "</ul>";
        
        return $html;
    }
    /**
     * Sort the items in the menu
     * 
     * @param MenuItem $child1
     * @param MenuItem $child2
     * @return int
     */
    private function sort($child1, $child2)
    {
        return strcasecmp($child1->text, $child2->text);
    }
}
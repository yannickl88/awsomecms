<?php
class MenuItem
{
    public $name;
    public $text;
    public $page;
    public $parent;
    private $children;
    
    public function __construct($name, $text, $page, $parent = "")
    {
        $this->children = array();
        
        $this->name = $name;
        if(empty($text))
        {
            $text = ucfirst($name);
        }
        $this->text = $text;
        $this->page = $page;
        $this->parent = $parent;
    }
    
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
    
    private function sort($child1, $child2)
    {
        return strcasecmp($child1->text, $child2->text);
    }
    
    public function toHTML()
    {
        $html = "<li onmouseover=\"adminmenu_openMenu(this);\" onmouseout=\"adminmenu_closeMenu(this);\">";
        if(empty($this->page))
        {
            $html .= "<a>{$this->text}";
        }
        else
        {
            $html .= "<a href=\"/{$this->page}\">{$this->text}";
        }
        
        if(count($this->children) > 0)
        {
            $html .= "<span style='float: right'>&#187;</span>";
        }
        $html .= "</a>";
        
        if(count($this->children) > 0)
        {
            $html .= "<ul>";
            foreach($this->children as $childNode)
            {
                $html .= $childNode->toHTML();
            }
            $html .= "</ul>";
        }
        $html .= "</li>";
        
        return $html;
    }
}

class Menu
{
    private static $instance;
    
    private $children;
    private $names;
    private $que;
    
    public static function getInstance()
    {
        if(!Menu::$instance)
        {
             Menu::$instance = new Menu();
        }
        
        return Menu::$instance;
    }
    
    private function __construct()
    {
        $this->children = array();
        $this->que = array();
    }
    
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
            $html .= $childNode->toHTML();
        }
        $html .= "</ul>";
        
        return $html;
    }
    
    private function sort($child1, $child2)
    {
        return strcasecmp($child1->text, $child2->text);
    }
}
<?php
class Tree
{
    private $children;
    public $onlyShowFolders;
    public $javascript;
    public $hideAdmin;
    
    public function __construct()
    {
        $this->children = array();
        $this->onlyShowFolders = false;
        $this->hideAdmin = Config::get("hideadmintree", true, "admin") === "1";
        $this->javascript = "";
    }
    /**
     * Populate the tree from the components
     * @return Tree
     */
    public function populate()
    {
        $components = Config::getInstance()->getComponenets();
        
        foreach($components as $component)
        {
            $componentObj = Component::init($component->component_name);
            
            if($componentObj)
            {
                $componentObj->registerTreeItems($this);
            }
        }
        
        return $this;
    }
    /**
     * Add a child to the tree
     * 
     * @param TreeItem $child
     * @return Tree
     */
    public function addChild($child, $location)
    {
        $locationNodes = explode("/", $location);
        
        //remove the empty places
        foreach($locationNodes as $key => $value)
        {
            if(empty($value))
            {
                unset($locationNodes[$key]);
            }
        }
        if(count($locationNodes) == 0)
        {
            $child->setParent($this);
            $this->children[] = $child;
        }
        else
        {
            $nextNode = array_shift($locationNodes);
            $childAdded = false;
            
            foreach($this->children as $childNode)
            {
                if($childNode->getType() == 2 && $childNode->name == $nextNode)
                {
                    $childNode->addChild($child, $locationNodes);
                    $childAdded = true;
                    break;
                }
            }
            
            if(!$childAdded) //hmm we need to create a folder for this child
            {
                $folder = new TreeFolder($nextNode, "");
                $folder->addChild($child, $locationNodes);
                $folder->setParent($this);
                $this->children[] = $folder;
                $this->sort();
            }
        }
        
        return $this;
    }
    public function getLocation()
    {
        return "";
    }
    public function getRoot()
    {
        return $this;
    }
    public function sort()
    {
        usort($this->children, array($this, "sortCompare"));
    }
    protected function sortCompare($a, $b)
    {
        return $a->compare($b);
    }
    public function toHTML()
    {
        return $this->__toString();
    }
    public function __toString()
    {
        $html = "";
        $html .= "<div class=\"treeNode\">";
        foreach($this->children as $child)
        {
            $html .= $child->toHTML();
        }
        $html .= "</div>";
        return $html;
    }
}

class TreeItem
{
    public $name;
    public $editURL;
    public $deleteURL;
    public $viewURL;
    protected $parent;
    
    public function __construct($name, $editURL, $deleteURL, $viewURL)
    {
        $this->name = $name;
        $this->editURL = $editURL;
        $this->deleteURL = $deleteURL;
        $this->viewURL = $viewURL;
    }
    public function getType()
    {
        return 1;
    }
    public function setParent($parent)
    {
        $this->parent = $parent;
    }
    public function getRoot()
    {
        return $this->parent->getRoot();
    }
    public function getLocation()
    {
        return $this->parent->getLocation()."/".$this->name;
    }
    public function toHTML()
    {
        return $this->__toString();
    }
    public function compare($a)
    {
        if($this->getType() == $a->getType())
        {
            return strnatcasecmp($this->name, $a->name);
        }
        else
        {
            return ($this->getType() < $a->getType())? 1 : -1;
        }
    }
    public function __toString()
    {
        $html = "";
        if(!$this->getRoot()->onlyShowFolders)
        {
            $html .= "<div class=\"hideIcons\" onmouseout=\"$(this).addClass('hideIcons');\" onmouseover=\"$(this).removeClass('hideIcons');\">";
            $html .= "<a class=\"deleteIcon\" href=\"{$this->deleteURL}\"></a><a class=\"viewIcon\" href=\"{$this->viewURL}\"></a>";
            $html .= "<a href=\"{$this->editURL}\">";
            if($this->name == 'index')
            {
                $html .= '<img src="/img/icons/page_home.png" />';
            }
            else
            {
                $html .= '<img src="/img/icons/page_page.png" />';
            }
            $html .= "{$this->name}</a>";
            $html .= "</div>";
        }
        return $html;
    }
}

class TreeFolder extends TreeItem
{
    private $children;
    
    public function __construct($name)
    {
        parent::__construct($name, "", "", "");
        
        $this->children = array();
    }
    /**
     * Add a child to the tree
     * 
     * @param TreeItem $child
     * @return TreeItem
     */
    public function addChild($child, $locationNodes)
    {
        if(count($locationNodes) == 0)
        {
            $child->setParent($this);
            $this->children[] = $child;
            $this->sort();
        }
        else
        {
            $nextNode = array_shift($locationNodes);
            $childAdded = false;
            
            foreach($this->children as $childNode)
            {
                if($childNode->getType() == 2 && $childNode->name == $nextNode)
                {
                    $childNode->addChild($child, $locationNodes);
                    $childAdded = true;
                    break;
                }
            }
            
            if(!$childAdded) //hmm we need to create a folder for this child
            {
                $folder = new TreeFolder($nextNode, "");
                $folder->addChild($child, $locationNodes);
                $folder->setParent($this);
                $this->children[] = $folder;
                $this->sort();
            }
        }
        
        return $this;
    }
    public function getType()
    {
        return 2;
    }
    public function sort()
    {
        usort($this->children, array($this, "sortCompare"));
    }
    protected function sortCompare($a, $b)
    {
        return $a->compare($b);
    }
    public function __toString()
    {
        $id = md5($this->getLocation());
        
        $html = "";
        $html .= "<div class=\"\">";
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= "<a class=\"folder link{$id}\" href=\"#\" onClick=\"return admin_toggleTree('{$id}');\" onMouseOver=\"$(this).addClass('focus')\"  onMouseOut=\"$(this).removeClass('focus')\"></a>";
        } 
        else
        {
            $html .= "<a class=\"folder link{$id} open\" href=\"#\" onClick=\"return admin_toggleTree('{$id}');\" onMouseOver=\"$(this).addClass('focus')\"  onMouseOut=\"$(this).removeClass('focus')\"></a>";
        }
        $html .= '<img src="/img/icons/page_folder.png" />';
        if($this->getRoot()->javascript)
        {
            $html .= "<a href=\"#\" onClick=\"$('#{$this->getRoot()->javascript}').val('{$this->getLocation()}'); return false;\" style=\"padding: 0;\">";
        }
        else
        {
            $html .= '<a href="#" onClick="return admin_toggleTree(\''.$id.'\');" style="padding: 0;">';
        }
        $html .= $this->name;
        $html .= '</a>';
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= '<div class="treeNode hidden sub'.$id.'">';
        }
        else
        {
            $html .= '<div class="treeNode sub'.$id.'">';
        }
        
        foreach($this->children as $child)
        {
            $html .= $child->toHTML();
        }
        
        $html .= '</div>';
        $html .= "</div>";
        return $html;
    }
}
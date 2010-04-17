<?php
class Tree
{
    const PAGE          = 1;
    const CONTENT       = 2;
    
    protected $children;
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
    public function addChild($child, $location, $type = Tree::PAGE)
    {
        $location = substr($location, 0, -1);
        
        if(!isset($this->children[$type]))
        {
            $this->children[$type] = array();
        }
        
        if(empty($location))
        {
            $locationNodes = array("");
        }
        else
        {
            $locationNodes = explode("/", $location);
        }
        
        $nextNode = array_shift($locationNodes);
        $childAdded = false;
        $child->setContentType($type);
        
        foreach($this->children[$type] as $childNode)
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
            $folder = new TreeFolder($nextNode);
            $folder->setContentType($type);
            $folder->addChild($child, $locationNodes);
            $folder->setParent($this);
            $this->children[$type][] = $folder;
            $this->sort($type);
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
    public function sort($type)
    {
        usort($this->children[$type], array($this, "sortCompare"));
    }
    protected function sortCompare($a, $b)
    {
        return $a->compare($b);
    }
    public function toHTML($types = 0)
    {
        return $this->__toString($types);
    }
    public function __toString($types = 0)
    {
        $html = "";
        $html .= "<div class=\"treeNode\">";
        foreach($this->children as $type => $children)
        {
            if($types == 0 || ($types > 0 && Config::hasFlag($types, $type)))
            {
                foreach($children as $child)
                {
                    $html .= $child->toHTML();
                }
            }
        }
        $html .= "</div>";
        if($this->javascript)
        {
        	$html .= "<script type=\"text/javascript\"> admin_treeTarget = \"{$this->javascript}\";</script>";
        }
        return $html;
    }
}

class TreeItem
{
    public $name;
    public $editURL;
    public $deleteURL;
    public $viewURL;
    private $contentType;
    protected $parent;
    
    public function __construct($name, $editURL, $deleteURL, $viewURL)
    {
        $this->name = $name;
        $this->editURL = $editURL;
        $this->deleteURL = $deleteURL;
        $this->viewURL = $viewURL;
        $this->contentType = Tree::PAGE;
    }
    public function getType()
    {
        return 1;
    }
    public function getContentType()
    {
        return $this->contentType;
    }
    public function setContentType($type)
    {
        $this->contentType = $type;
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
        return $this->parent->getLocation().$this->name."/";
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
            $html .= "<a class=\"deleteIcon treeIcon\" href=\"{$this->deleteURL}\"><img src=\"/img/icons/page_delete.png\" /></a>";
            $html .= "<a class=\"viewIcon treeIcon\" href=\"{$this->viewURL}\"><img src=\"/img/icons/page_view.png\" /></a>";
            $html .= "<a href=\"{$this->editURL}\">";
            if($this->name == 'index')
            {
                $html .= '<img src="/img/icons/page_home.png" alt="page"/>';
            }
            else
            {
                $html .= '<img src="/img/icons/page_page.png" alt="page"/>';
            }
            $html .= "{$this->name}</a>";
            $html .= "</div>";
        }
        return $html;
    }
}

class TreeFolder extends TreeItem
{
    protected $children;
    
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
        $classes = array();
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= "<a class=\"folder link{$id} clickableTreeArrow\" href=\"#\"></a>";
        } 
        else
        {
            $html .= "<a class=\"folder link{$id} open clickableTreeArrow\" href=\"#\"></a>";
        }
        if($this->name == "" && $this->getContentType() == Tree::PAGE)
        {
            $html .= '<img src="/img/icons/folder_page.png" alt="folder"/>';
        }
        else
        {
            $html .= '<img src="/img/icons/folder.png" alt="folder"/>';
        }
        if($this->getRoot()->javascript)
        {
            $html .= "<a href=\"{$this->getLocation()}\" class=\"clickableTreeItem javascript\" style=\"padding: 0;\">";
        }
        else
        {
            $html .= "<a href=\"#\" class=\"clickableTreeItem\" style=\"padding: 0;\">";
        }
        if($this->name == "")
        {
            switch($this->getContentType())
            {
                case Tree::CONTENT:
                    $html .= "[contentroot]";
                    $classes[] = "contentroot";
                    break;
                case Tree::PAGE:
                    $html .= "[siteroot]";
                    $classes[] = "siteroot";
                    break;
                default:
                    $html .= "/";
                    break;
            }
        }
        else
        {
            $html .= $this->name;
        }
        $html = "<div class=\"".implode(" ",$classes)."\">".$html;
        $html .= '</a>';
        if(!isset($_COOKIE['menu'.$id]))
        {
            $html .= '<div class="treeNode hidden">';
        }
        else
        {
            $html .= '<div class="treeNode">';
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
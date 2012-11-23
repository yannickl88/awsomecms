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
 * Tree class for the page admin tree
 * 
 * @author Yannick
 */
class Tree
{
    private static $instance = null;
    
    public static function getInstance()
    {
        if(self::$instance == null)
        {
            self::$instance = new Tree();
        }
        
        return self::$instance;
    }
    
    const PAGE          = 1;
    const CONTENT       = 2;
    
    const NONE          = 1;
    const RADIO         = 2;
    const CHECK         = 3;
    
    private $children;
    private $name;
    public $hideAdmin;
    public $systemFileTree = false;
    public $fileTypes = array();
    private $icons;
    private $calledTypes;
    private $openFolders;
    
    /**
     * Constructor
     * 
     * @param String $name
     * @param String $element
     */
    private function __construct()
    {
        $this->children = array();
        $this->calledTypes = array();
        $this->icons = array(
            array("delete",         "/img/icons/delete.png"),
            array("view",           "/img/icons/page_view.png"),
            array("page",           "/img/icons/page_page.png"),
            array("index",          "/img/icons/page_home.png"),
            array("folder",         "/img/icons/folder.png"),
            array("add",            "/img/icons/add.png"),
            array("overview",       "/img/icons/overview.png"),
            array("reload",         "/img/icons/arrow_refresh_small.png"),
            array("loader",         "/img/admin/loader.gif"),
        );
        $this->hideAdmin = Config::get("hideadmintree", true, "admin");
        
        $contentroot = new RootFolder("[contentroot]", "/");
        $contentroot->setType(Tree::CONTENT);
        $sitetroot = new RootFolder("[siteroot]", "/");
        $sitetroot->setType(Tree::PAGE);
        
        $this->addChild($contentroot);
        $this->addChild($sitetroot);
        
        $components = RegisterManager::getInstance()->getComponents();
        
        foreach($components as $component)
        {
            $componentObj = Component::init($component->component_name);
            
            if($componentObj)
            {
                $componentObj->registerTreeItems($this);
            }
        }
        
        //parse cookie data
        $this->openFolders = array();
        
        $headers = apache_request_headers();
        
        if(isset($headers["Cookie"])) {
            $cookies = explode(";", $headers["Cookie"]);
            foreach ($cookies as $value)
            {
                $data = explode("=", $value);
                if($data[1] === "1")
                    $this->openFolders[] = trim($data[0]);
            }
        }
    }
    public function isOpen($loc) {
        return $loc == "" || in_array($loc, $this->openFolders);
    }
    /**
     * Add a child to the tree
     * 
     * @param TreeNode $child
     */
    public function addChild($child)
    {
        $child->setParent($this);
        $this->children[] = $child;
        
        if(!in_array(get_class($child), $this->calledTypes))
        {
            $child->registerIcons();
            $this->calledTypes[] = get_class($child);
        }
    }
    public function addIcon($key, $url)
    {
        $this->icons[] = array($key, $url);
    }
    /**
     * get the name of the javascript element
     * 
     * @return String
     */
    public function getName()
    {
        return $this->name;
    }
    public function getCheckboxType()
    {
        return $this->checkboxes;
    }
    private function addFolders(&$array, $folder)
    {
        $loc = explode("/", $folder);
        
        while(count($loc) > 0) {
            $location = implode("/", $loc);
            if(!in_array($location, $array))
                array_push($array, $location);
            else 
                break;
            array_pop($loc);
        }
    }
    
    public function getNodesDataUnder($path) {
        $nodes = array();
        
        $addedFolders = array();
        
        foreach($this->children as $child)
        {
            if(substr($child->getLocation(), 0, -1) == $path)
            {
                if(empty($types) || in_array($child->getType(), $types))
                {
                    $nodes[] = $child->toData();;
                    if(is_a($child, "Folder"))
                        $this->addFolders($addedFolders, $child->getLocation() . $child->getName());
                }
            }
        }
        
        foreach($this->children as $child)
        {
            if(strpos($child->getLocation(), $path."/") === 0 && $child->getLocation() != $path."/")
            {
                //there might be a folder which is added due to this guy which is supose to been shown
                $parentLoc = explode("/", substr($child->getLocation(), strlen($path."/"), -1));
                
                $parentName = array_shift($parentLoc);
                
                if(!in_array($path."/".$parentName, $addedFolders))
                {
                    $folder = new Folder($parentName, $path . "/", $child->getType());
                    $folder->setParent($this);
                    $nodes[] = $folder->toData();
                    
                    $this->addFolders($addedFolders, $path . "/" . $parentName);
                }
            }
        }
        
        return $nodes;
    }
    
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript($name, $element = null, $types = array(), $foldersOnly = false, $javascript = false, $checkBoxes = array(Tree::NONE, array()), $filetypes = array(), $noCookie = false)
    {
        $this->name = $name;
        $this->fileTypes = $filetypes;
        
        if(!empty($element))
        {
            $element = "#".$element."";
        }
        
        $html = "<script type=\"text/javascript\">\n";
        $html .= "var {$this->name} = new Tree(\"{$element}\", ".json_encode($checkBoxes).", ".json_encode($this->systemFileTree).");\n";
        if($foldersOnly)
        {
            $html .= "{$this->name}.foldersOnly = true;\n";
        }
        if($javascript)
        {
            $html .= "{$this->name}.javascript = \"{$javascript}\";\n";
        }
        if($noCookie)
        {
            $html .= "{$this->name}.noCookie = true;\n";
        }
        foreach($this->icons as $icon)
        {
            $html .= "{$this->name}.assignIcon(\"{$icon[0]}\",  \"{$icon[1]}\");\n";
        }
        $html .= "{$this->name}.updateCheckboxValues();\n";

        $addedFolders = array();
        
        foreach($this->children as $child)
        {
            if($this->isOpen(substr($child->getLocation(), 0, -1)))
            {
                if(empty($types) || in_array($child->getType(), $types))
                {
                    if($child->getLocation() == "/" || !$noCookie)
                    {
                        $html .= $child->toJavascript();
                        if($child->getLocation() != "/")
                            $this->addFolders($addedFolders, substr($child->getLocation(), 0, -1));
                        
                        if(is_a($child, "Folder"))
                            $this->addFolders($addedFolders, $child->getLocation() . $child->getName());
                    }
                }
            }
        }
        
        foreach($this->children as $child)
        {
            //there might be a folder which is added due to this guy which is supose to been shown
            $parentLoc = explode("/", substr($child->getLocation(), 0, -1));
            
            while(count($parentLoc) > 1)
            {
                $parentName = array_pop($parentLoc);
                $parent = implode("/", $parentLoc);
                
                if($this->isOpen($parent) && !in_array($parent."/".$parentName, $addedFolders))
                {
                    if((empty($types) || in_array($child->getType(), $types)) && !$noCookie)
                    {
                        $folder = new Folder($parentName, $parent . "/", $child->getType());
                        $folder->setParent($this);
                        $html .= $folder->toJavascript();
                        
                        $this->addFolders($addedFolders, $parent . "/" . $parentName);
                        break;
                    }
                }
            }
        }
        
        $html .= "</script>\n";
        
        return $html;
    }
}
/**
 * Abstract class for the nodes of the tree
 * 
 * @author Yannick
 */
abstract class TreeNode
{
    protected $parent;
    protected $location;
    protected $name;
    protected $type = 0;
    
    /**
     * Set the parent, which is the tree
     * 
     * @param Tree $parent
     */
    public function setParent($parent)
    {
        $this->parent = $parent;
    }
    /**
     * Set the type of this node
     * 
     * @param Int $type
     */
    public function setType($type)
    {
        $this->type = $type;
    }
    /**
     * Get the type of this node
     * 
     * @return Int
     */
    public function getType()
    {
        return $this->type;
    }
    /**
     * Create the javascript code for the node
     * 
     * @return String
     */
    abstract function toJavascript();
    /**
     * Create a JSON representation of this node
     * 
     * @return String
     */
    abstract function toData();
    /**
     * function that is called once on each type of node, you can add icons to the tree here
     * 
     * @return String
     */
    public function registerIcons()
    {
        
    }
    
    public function getLocation() {
        return $this->location;
    }
    
    public function getName() {
        return $this->name;
    }
}
/**
 * The node for the page items
 * 
 * @author Yannick
 */
class PageItem extends TreeNode
{
    protected $id;
    protected $viewURL;
    
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($record)
    {
        $this->name = $record->page_name;
        $this->id = $record->page_id;
        $this->location = "/[siteroot]".$record->page_location;
        $this->viewURL = $record->page_location.$record->page_name;
        $this->setType(Tree::PAGE);
    }
    /**
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJavascript()
     *
     * @return String
     */
    public function toJavascript()
    {
        $component = Component::init("page");
        
        $linkEdit = makeLink(array($component, "edit"))."?page_id={$this->id}";
        $linkDelete = makeLink(array($component, "delete"))."?page_id={$this->id}";
        $icon = ($this->name == "index")? "index" : "page";
        
        return "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()}), new TreeAction(\"delete\", \"{$linkDelete}\", {$this->parent->getName()})], {$this->getType()}));\n";
    }
    /**
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJSON()
     *
     * @return String
     */
    public function toData()
    {
        $component = Component::init("page");
        
        $linkEdit = makeLink(array($component, "edit"))."?page_id={$this->id}";
        $linkDelete = makeLink(array($component, "delete"))."?page_id={$this->id}";
        $icon = ($this->name == "index")? "index" : "page";
        
        $actions = array(
            array("icon" => "view", "link" => $this->viewURL),
            array("icon" => "delete", "link" => $linkDelete)
        );
        return array(
            "classname" => 'TreeNode',
            "name" => $this->name,
            "location" => $this->location,
            "url" => $linkEdit,
            "icon" => $icon,
            "actions" => $actions,
            "contentType" => $this->type,
            "id" => -1
        );
    }
}

class Folder extends TreeNode
{
    
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($name, $location, $type = Tree::PAGE)
    {
        $this->name = $name;
        $this->location = $location;
        $this->type = $type;
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $html = "{$this->parent->getName()}.addChild(new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [], {$this->getType()}));\n";
        
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJSON()
     *
     * @return String
     */
    public function toData()
    {
        return array(
            "classname" => 'TreeFolder',
            "name" => $this->name,
            "location" => $this->location,
            "icon" => "folder",
            "actions" => array(),
            "contentType" => $this->type,
            "isRootFolder" => false
        );
    }
}
class RootFolder extends Folder
{
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $html = "{$this->parent->getName()}.addChild(new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [], {$this->getType()}, true));\n";
        
        return $html;
    }
    
    public function toData()
    {
        return array(
            "classname" => 'TreeFolder',
            "name" => $this->name,
            "location" => $this->location,
            "icon" => "folder",
            "actions" => array(),
            "contentType" => $this->type,
            "isRootFolder" => true
        );
    }
}
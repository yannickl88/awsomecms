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
    
    private $children;
    private $name;
    public $hideAdmin;
    private $icons;
    private $calledTypes;
    
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
            array("loader",         "/img/admin/loader.gif"),
        );
        $this->hideAdmin = Config::get("hideadmintree", true, "admin");
        
        $contentroot = new RootFolder("[contentroot]", "/");
        $contentroot->setType(Tree::CONTENT);
        $sitetroot = new RootFolder("[siteroot]", "/");
        $sitetroot->setType(Tree::PAGE);
        
        $this->addChild($contentroot);
        $this->addChild($sitetroot);
        
        $components = RegisterManager::getInstance()->getComponenets();
        
        foreach($components as $component)
        {
            $componentObj = Component::init($component->component_name);
            
            if($componentObj)
            {
                $componentObj->registerTreeItems($this);
            }
        }
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
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript($name, $element = null, $types = array(), $foldersOnly = false, $javascript = false)
    {
        $this->name = $name;
        
        if(!empty($element))
        {
            $element = "#".$element."";
        }
        
        $html = "<script type=\"text/javascript\">\n";
        $html .= "var {$this->name} = new Tree(\"{$element}\", false, 1);\n";
        if($foldersOnly)
        {
            $html .= "{$this->name}.foldersOnly = true;\n";
        }
        if($javascript)
        {
            $html .= "{$this->name}.javascript = \"{$javascript}\";\n";
        }
        foreach($this->icons as $icon)
        {
            $html .= "{$this->name}.assignIcon(\"{$icon[0]}\",  \"{$icon[1]}\");\n";
        }
        
        foreach($this->children as $child)
        {
            if(empty($types) || in_array($child->getType(), $types))
            {
                $html .= $child->toJavascript();
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
     * function that is called once on each type of node, you can add icons to the tree here
     * 
     * @return String
     */
    public function registerIcons()
    {
        
    }
}
/**
 * The node for the page items
 * 
 * @author Yannick
 */
class PageItem extends TreeNode
{
    protected $name;
    protected $id;
    protected $location;
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
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkEdit = "/".Config::get("pageedit", "pageedit", "admin")."?page_id={$this->id}";
        $linkDelete = "/".Config::get("pagedelete", "pagedelete", "admin")."?page_id={$this->id}";
        $icon = ($this->name == "index")? "index" : "page";
        
        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()}), new TreeAction(\"delete\", \"{$linkDelete}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
        return $html;
    }
}

class Folder extends TreeNode
{
    protected $name;
    protected $location;
    
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
}
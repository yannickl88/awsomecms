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
    private $children;
    private $name;
    private $element;
    public $hideAdmin;
    private $icons;
    private $calledTypes;
    
    /**
     * Constructor
     * 
     * @param String $name
     * @param String $element
     */
    public function __construct($name, $element = null)
    {
        $this->children = array();
        $this->calledTypes = array();
        $this->icons = array(
            array("page_delete",    "/img/icons/page_delete.png"),
            array("view",           "/img/icons/page_view.png"),
            array("page",           "/img/icons/page_page.png"),
            array("index",          "/img/icons/page_home.png"),
            array("folder",         "/img/icons/folder.png"),
        );
        $this->name = $name;
        $this->element = $element;
        
        $components = Config::getInstance()->getComponenets();
        
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
    public function toJavascript()
    {
        $html = "<script type=\"text/javascript\">\n";
        $html .= "var {$this->name} = new Tree(\"#{$this->element}\");\n";
        foreach($this->icons as $icon)
        {
            $html .= "{$this->name}.assignIcon(\"{$icon[0]}\",  \"{$icon[1]}\");\n";
        }
        
        foreach($this->children as $child)
        {
            $html .= $child->toJavascript();
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
    
    /**
     * Set the parent, which is the tree
     * 
     * @param $parent
     */
    public function setParent($parent)
    {
        $this->parent = $parent;
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
class TreePage extends TreeNode
{
    private $pageName;
    private $pageID;
    private $pageLocation;
    
    /**
     * Constructor
     * 
     * @param Object $page
     */
    public function __construct($page)
    {
        $this->pageName = $page->page_name;
        $this->pageID = $page->page_id;
        $this->pageLocation = "/[contentroot]".$page->page_location;
        $this->pageURL = $page->page_location.$page->page_name;
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkEdit = "/".Config::get("pageedit", "pageedit", "admin")."?page_id={$this->pageID}";
        $linkDelete = "/".Config::get("pagedelete", "pagedelete", "admin")."?page_id={$this->pageID}";
        $icon = ($this->pageName == "index")? "index" : "page";
        
        $html = "tree.addChild(new TreeItem(\"{$this->pageName}\", \"{$this->pageLocation}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->pageURL}\", tree), new TreeAction(\"page_delete\", \"{$linkDelete}\", tree)]));\n";
        
        return $html;
    }
}
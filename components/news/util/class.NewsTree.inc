<?php
import('/components/page/util/class.Tree.inc');

/**
 * The node for the page items
 * 
 * @author Yannick
 */
class NewsItem extends TreeNode
{
    private $name;
    private $id;
    private $location;
    private $viewURL;
    
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($record)
    {
        $this->name = $record->news_title;
        $this->id = $record->news_id;
        $this->location = "/[siteroot]/".$record->news_location;
        $newsName = strtolower(Language::get("news"));
        $this->viewURL = "/".$newsName.date("/Y/m/", strtotime($record->news_date)).$record->news_titleurl;
        $this->setType(Tree::PAGE);
    }
    /**
     * See parent
     */
    public function registerIcons()
    {
        $this->parent->addIcon("news", "/img/icons/news.png");
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkEdit = "/".Config::get("newsedit", "newsedit", "admin")."?news_id={$this->id}";
        $linkDelete = "/".Config::get("newsdelete", "newsdelete", "admin")."?news_id={$this->id}";
        $icon = "news";
        
        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()}), new TreeAction(\"delete\", \"{$linkDelete}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
        return $html;
    }
}

class NewsFolder extends TreeNode
{
    private $name;
    private $location;
    private $realLocation;
    
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($name, $location)
    {
        $this->name = $name;
        $this->location = "/[siteroot]".$location;
        $this->realLocation = $location;
        $this->setType(Tree::PAGE);
    }
    /**
     * See parent
     */
    public function registerIcons()
    {
        $this->parent->addIcon("news_folder", "/img/icons/news_folder.png");
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkAdd = "/".Config::get("newsadd", "newsadd", "admin");
        $linkView = "/".Config::get("newsadmin", "newsadmin", "admin");
        
        $html = "{$this->parent->getName()}.addChild(new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"news_folder\", [new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
        return $html;
    }
}
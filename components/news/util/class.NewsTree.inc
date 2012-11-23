<?php
import('/components/page/util/class.Tree.inc');

/**
 * The node for the page items
 * 
 * @author Yannick
 */
class NewsItem extends TreeNode
{
    private $id;
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
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJavascript()
     *
     * @return String
     */
    public function toJavascript()
    {
        $component = Component::init("news");
        
        $linkEdit = makeLink(array($component, "edit"))."?news_id={$this->id}";
        $linkDelete = makeLink(array($component, "delete"))."?news_id={$this->id}";
        $icon = "news";
        
        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()}), new TreeAction(\"delete\", \"{$linkDelete}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
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
        $component = Component::init("news");
        
        $linkEdit = makeLink(array($component, "edit"))."?news_id={$this->id}";
        $linkDelete = makeLink(array($component, "delete"))."?news_id={$this->id}";
        $icon = "news";
        
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
/**
 * News root folder which contains all the news items
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class NewsFolder extends Folder
{
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
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJavascript()
     *
     * @return String
     */
    public function toJavascript()
    {
        $component = Component::init("news");
        
        $linkAdd = makeLink(array($component, "add"));
        $linkView = makeLink(array($component, "admin"));
        
        $html = "{$this->parent->getName()}.addChild(new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"news_folder\", [new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
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
        $component = Component::init("news");
        
        $linkAdd = makeLink(array($component, "add"));
        $linkView = makeLink(array($component, "admin"));
        
        $actions = array(
            array("icon" => "add", "link" => $linkAdd),
            array("icon" => "overview", "link" => $linkView)
        );
        
        return array(
            "classname" => 'TreeFolder',
            "name" => $this->name,
            "location" => $this->location,
            "icon" => "news_folder",
            "actions" => $actions,
            "contentType" => $this->type,
            "isRootFolder" => false
        );
    }
}
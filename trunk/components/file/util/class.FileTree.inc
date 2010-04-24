<?php
import('/components/page/util/class.Tree.inc');

class ImageItem extends TreeNode
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
        $this->name = $record->file_name;
        $this->id = $record->file_id;
        $this->location = "/[contentroot]/".$record->file_location;
        $this->viewURL = $record->file_data->url;
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
        $linkEdit = "/".Config::get("newsedit", "newsedit", "admin")."?page_id={$this->id}";
        $linkDelete = "/".Config::get("newsdelete", "newsdelete", "admin")."?page_id={$this->id}";
        $icon = "news";
        
        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()}), new TreeAction(\"delete\", \"{$linkDelete}\", {$this->parent->getName()})], {$this->getType()}));\n";
        
        return $html;
    }
}
class ImageFolder extends Folder
{
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($name, $location)
    {
        parent::__construct($name, $location);
        $this->location = "/[contentroot]".$location;
    }
    /**
     * See parent
     */
    public function registerIcons()
    {
        $this->parent->addIcon("image_folder", "/img/icons/images_folder.png");
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $html = "{$this->parent->getName()}.addChild(new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"image_folder\", [], {$this->getType()}));\n";
        
        return $html;
    }
}
class UploadFolder extends Folder
{
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($name, $location)
    {
        parent::__construct($name, $location);
        $this->location = "/[contentroot]".$location;
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkAdd = "/".Config::get("fileadd", "fileadd", "admin");
        $linkView = "/".Config::get("fileadmin", "fileadmin", "admin");
        
        $html = "var node = new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()});\n";
        $html .= "$(node.html).attr({\n";
        $html .= "    \"ondrop\":       \"file_upload(event);\",\n";
        $html .= "    \"ondragenter\":  \"file_dragenter(event);\",\n";
        $html .= "    \"ondragover\":   \"file_dragover(event);\",\n";
        $html .= "    \"ondragleave\":  \"file_dragout(event);\"\n";
        $html .= "});\n";
        $html .= "{$this->parent->getName()}.addChild(node);\n";
        
        return $html;
    }
}
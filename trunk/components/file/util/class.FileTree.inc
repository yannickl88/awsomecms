<?php
import('/components/page/util/class.Tree.inc');

class FileItem extends TreeNode
{
    private $name;
    private $id;
    private $location;
    private $viewURL;
    private $fileType;
    
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($record)
    {
        $this->name = $record->file_name;
        $this->id = $record->file_id;
        $this->location = $record->file_location;
        $this->viewURL = $record->file_data->url;
        $this->fileType = $record->file_data->type;
    }
    /**
     * Create the javascript code for the tree
     * 
     * @return String
     */
    public function toJavascript()
    {
        $linkEdit = "/".Config::get("fileedit", "fileedit", "admin")."?file_id={$this->id}";
        $linkDelete = "/".Config::get("filedelete", "filedelete", "admin")."?file_id={$this->id}";
        $icon = Component::init("file")->getType($this->fileType);
        
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
    private $nodeMap;
    /**
     * Constructor
     * 
     * @param Object $record
     */
    public function __construct($name, $location, $nodeMap)
    {
        parent::__construct($name, $location);
        $this->location = "/[contentroot]".$location;
        $this->nodeMap = $nodeMap;
    }
    /**
     * See parent
     */
    public function registerIcons()
    {
        $this->parent->addIcon("images", "/img/icons/image.png");
        $this->parent->addIcon("docs", "/img/icons/doc.png");
        $this->parent->addIcon("vid", "/img/icons/video.png");
        $this->parent->addIcon("other", "/img/icons/other.png");
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
        
        $html = "uploadFolder = new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()});\n";
        $html .= "file_bindListeners(uploadFolder.html.get(0));\n";
        $html .= "uploadFolder.nodes = ".stripslashes(json_encode($this->nodeMap))."\n";
        $html .= "{$this->parent->getName()}.addChild(uploadFolder);\n";
        
        return $html;
    }
}
<?php
/**
 * Tree item which contains a file
 *
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class FileItem extends TreeNode
{
    private $id;
    private $viewURL;
    private $fileType;

    public $canView = true;
    public $canEdit = true;
    public $canDelete = true;

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
        $this->type = Tree::CONTENT;

        $this->canView = $record->permissions['view'];
        $this->canEdit = $record->permissions['edit'];
        $this->canDelete = $record->permissions['delete'];
    }
    /**
     * (non-PHPdoc)
     * @see components/page/util/TreeNode::toJavascript()
     *
     * @return String
     */
    public function toJavascript()
    {
        if(!empty($this->parent->fileTypes) && !in_array(substr($this->fileType, strrpos($this->fileType, "/") + 1), $this->parent->fileTypes))
            return;

        $component = Component::init("file");
        $linkEdit = "";
        $actions = array();
        if($this->canEdit)
            $linkEdit = makeLink(array($component, "edit"))."?file_id={$this->id}";
        if($this->canView && !empty($this->viewURL))
            $actions[] = "new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()})";;
        if($this->canDelete)
            $actions[] = "new TreeAction(\"delete\", \"".makeLink(array($component, "delete"))."?file_id={$this->id}"."\", {$this->parent->getName()})";

        $icon = $component->getType($this->fileType);

        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [".implode(",", $actions)."], {$this->getType()}, {$this->id}));\n";

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
        $component = Component::init("file");
        $linkEdit = "";
        $actions = array();
        if($this->canEdit)
            $linkEdit = makeLink(array($component, "edit"))."?file_id={$this->id}";
        if($this->canView && !empty($this->viewURL))
            $actions[] = array("icon" =>"view", "link" => $this->viewURL);
        if($this->canDelete)
            $actions[] = array("icon" =>"delete", "link" => makeLink(array($component, "delete"))."?file_id={$this->id}");

        $icon = $component->getType($this->fileType);

        return array(
            "classname" => 'TreeNode',
            "name" => $this->name,
            "location" => $this->location,
            "url" => $linkEdit,
            "icon" => $icon,
            "actions" => $actions,
            "type" => $this->type,
            "id" => $this->id
        );
    }
}
/**
 * Tree items which contains the images
 *
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
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
        $this->type = Tree::CONTENT;
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

    public function toData()
    {
        return array(
            "classname" => 'TreeFolder',
            "name" => $this->name,
            "location" => $this->location,
            "icon" => "image_folder",
            "actions" => array(),
            "contentType" => $this->type,
            "isRootFolder" => false
        );
    }
}
/**
 * Tree items to which can be uploaded by drag and drop
 *
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
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
        $this->type = Tree::CONTENT;
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
        $component = Component::init("file");

        $linkAdd = makeLink(array($component, "add"));
        $linkView = makeLink(array($component, "admin"));

        $html = "var child = new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [new TreeAction(\"reload\", \"\", {$this->parent->getName()}), new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()});\n";
        $html .= "if({$this->parent->getName()}.systemFileTree) { \n";
        $html .= "    uploadFolder = child;\n";
        $html .= "    uploadFolder.nodes = ".stripslashes(json_encode($this->nodeMap))."\n";
        $html .= "    $().ready(function(e) {new FileUpload(uploadFolder.html.get(0)); });\n";
        $html .= "}\n";
        $html .= "{$this->parent->getName()}.addChild(child);\n";

        return $html;
    }

    public function toData()
    {
        return array(
            "classname" => 'TreeUploadFolder',
            "name" => $this->name,
            "location" => $this->location,
            "icon" => "image_folder",
            "actions" => array(),
            "contentType" => $this->type,
            "isRootFolder" => false,
            "nodes" => $this->nodeMap
        );
    }
}
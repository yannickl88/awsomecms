<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.file.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/components/page/util/class.Tree.inc');
/**
 * Tree item which contains a file
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class FileItem extends TreeNode
{
    private $name;
    private $id;
    private $location;
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
        $component = Component::init("file");
        $linkEdit = "";
        $actions = array();
        if($this->canEdit)
            $linkEdit = makeLink(array($component, "edit"))."?file_id={$this->id}";
        if($this->canView)
            $actions[] = "new TreeAction(\"view\", \"{$this->viewURL}\", {$this->parent->getName()})";;
        if($this->canDelete)
            $actions[] = "new TreeAction(\"delete\", \"".makeLink(array($component, "delete"))."?file_id={$this->id}"."\", {$this->parent->getName()})";
            
        $icon = $component->getType($this->fileType);
        
        $html = "{$this->parent->getName()}.addChild(new TreeItem(\"{$this->name}\", \"{$this->location}\", \"{$linkEdit}\", {$this->parent->getName()}, \"{$icon}\", [".implode(",", $actions)."], {$this->getType()}));\n";
        
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
        if($this->canView)
            $actions[] = array("icon" =>"view", "link" => $this->viewURL);
        if($this->canDelete)
            $actions[] = array("icon" =>"delete", "link" => makeLink(array($component, "delete"))."?file_id={$this->id}");
            
        $icon = $component->getType($this->fileType);
        
        return array(
            "name" => $this->name,
            "location" => $this->location,
            "editLink" => $linkEdit,
            "icon" => $icon,
            "actions" => $actions,
            "type" => $this->type
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
        
        $html = "uploadFolder = new TreeFolder(\"{$this->name}\", \"{$this->location}\", {$this->parent->getName()}, \"folder\", [new TreeAction(\"reload\", \"\", {$this->parent->getName()}), new TreeAction(\"overview\", \"{$linkView}\", {$this->parent->getName()}), new TreeAction(\"add\", \"{$linkAdd}\", {$this->parent->getName()})], {$this->getType()});\n";
        $html .= "uploadFolder.nodes = ".stripslashes(json_encode($this->nodeMap))."\n";
        $html .= "$().ready(function(e) {new FileUpload(uploadFolder.html.get(0)); });\n";
        $html .= "{$this->parent->getName()}.addChild(uploadFolder);\n";
        
        return $html;
    }
}
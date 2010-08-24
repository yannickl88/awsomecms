<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.file
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');
import('file.util.FileTree');

/**
 * A way to create a centeral file repository to upload files.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FileComponent extends Component
{
    /**
     * Constructor
     * 
     * @return unknown_type
     */
    public function __construct()
    {
        parent::__construct('file');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("file.action_add", "admin");
        $registerManager->registerAuth("file.action_edit", "admin");
        $registerManager->registerAuth("file.action_delete", "admin");
        $registerManager->registerAuth("file.action_upload", "admin");
        
        $registerManager->registerPage(array($this, 'add'), '{form table="file.files" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="file.files" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="file.files" form="delete"}');
        $registerManager->registerPage(array($this, 'admin'), '{form table="file.files" form="admin"}');
    }
    /**
     * Get a connector object based on the type
     * 
     * @param String $name
     * @return FileConnector
     */
    public function getConnector($name)
    {
        $className = ucfirst($name)."Connector";
        $conn = "class.".$className.".inc";
        $obj = null;
        
        if(file_exists($this->location."connectors/".$conn))
        {
            import($this->location."connectors/".$conn);
            $obj = new $className();
        }
        
        return $obj;
    }
    /**
     * Add action
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->redirect(array($this, 'admin'));
        }
    }
    /**
     * update the references made by the connectors
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param Array $request
     */
    public function action_update($smarty, $smartyLoader, $request)
    {
        $connectors = scandir($this->location."/connectors");
        $rows = array();
        
        foreach($connectors as $connectorFile)
        {
            $matches = array();
            if($connectorFile != "." && $connectorFile != ".." && preg_match('/class\.([a-zA-Z]*)Connector\.inc/', $connectorFile, $matches))
            {
                $connector = $this->getConnector(strtolower($matches[1]));
                if($connector && $connector->isActive())
                {
                    $rows = array_merge($rows, $connector->fetch());
                }
            }
        }
        $this->ajax($rows);
    }
    /**
     * Upload a file action
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_upload($smarty, $smartyLoader, $request)
    {
        $request["file_name"] = basename($_FILES['file_data']['name'], substr($_FILES['file_data']['name'], strrpos($_FILES['file_data']['name'], ".")));
        $request["file_date"] = "now";
        
        try
        {
            $return = Table::init("file.files")
                ->setRecord((object) $request)
                ->doInsert();
            
            $linkEdit = makeLink(array($this, "edit"))."?file_id={$return->insertID()}";
            $linkDelete = makeLink(array($this, "delete"))."?file_id={$return->insertID()}";
            $row = $return->getRow();
            
            $this->ajax(array("succes" => true, "id" => $return->insertID(), "editLink" => $linkEdit, "deleteLink" => $linkDelete, "viewLink" => $row->file_data->url, "type" => $row->file_data->type));
        }
        catch(InvalidFormException $e)
        {
            $fields = $e->getInvalidFields();
            $this->ajax(array("error" => true, "exception" => $fields["file_data"]));
        }
    }
    /**
     * Edit action
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRecord((object) $request)
                ->doUpdate();
            
            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['file_id'])
            {
                Table::init('file.files')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Delete action
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRequest((object) $request)
                ->doDelete();
            
            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['file_id'])
            {
                $record = Table::init("file.files")
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerTreeItems($tree)
     */
    public function registerTreeItems($tree)
    {
        $table = Table::init("file.files", "admintree");
        
        $result = $table
            ->doSelect()
            ->getRows();

        $filesName = strtolower(Language::get("files"));
        $imageName = strtolower(Language::get("images"));
        $docName = strtolower(Language::get("documents"));
        $vidName = strtolower(Language::get("videos"));
        $otherName = strtolower(Language::get("other"));
        
        $mapping = $this->getLocationMap();
        
        $tree->addChild(new UploadFolder($filesName, "/", $mapping));
        $tree->addChild(new ImageFolder($imageName, "/{$filesName}/"));
        $tree->addChild(new Folder($docName, "/[contentroot]/{$filesName}/", Tree::CONTENT));
        $tree->addChild(new Folder($vidName, "/[contentroot]/{$filesName}/", Tree::CONTENT));
        $tree->addChild(new Folder($otherName, "/[contentroot]/{$filesName}/", Tree::CONTENT));
        
        foreach($result as $row)
        {
            $row->file_location = $mapping[$this->getType($row->file_data->type)];
            
            $item = new FileItem($row);
            $tree->addChild($item);
        }
    }
    public function getLocationMap()
    {
        $filesName = strtolower(Language::get("files"));
        $imageName = strtolower(Language::get("images"));
        $docName = strtolower(Language::get("documents"));
        $vidName = strtolower(Language::get("videos"));
        $otherName = strtolower(Language::get("other"));
        
        return array(
            "images"    => "/[contentroot]/{$filesName}/{$imageName}/",
            "docs"      => "/[contentroot]/{$filesName}/{$docName}/",
            "vid"       => "/[contentroot]/{$filesName}/{$vidName}/",
            "other"     => "/[contentroot]/{$filesName}/{$otherName}/",
        );
    }
    /**
     * Get the type of a file based on the mimetype
     * 
     * @param String $fileType      mimetype like image/jpg
     * @return String               [images|vid|docs|other]
     */
    public function getType($fileType)
    {
        if(preg_match('/^image\/.*$/i', $fileType))
        {
            return "images";
        }
        elseif(preg_match('/^audio\/.*$/i', $fileType))
        {
            return "vid";
        }
        elseif(preg_match('/^video\/.*$/i', $fileType))
        {
            return "vid";
        }
        elseif(preg_match('/^application\/vnd\.openxmlformats\-officedocument\.spreadsheetml\.sheet$/i', $fileType))
        {
            return "docs";
        }
        elseif(preg_match('/^application\/vnd\.openxmlformats\-officedocument\.wordprocessingml\.document$/i', $fileType))
        {
            return "docs";
        }
        elseif(preg_match('/^application\/vnd\.ms-powerpoint$/i', $fileType))
        {
            return "docs";
        }
        elseif(preg_match('/^application\/msword$/i', $fileType))
        {
            return "docs";
        }
        elseif(preg_match('/^application\/vnd\.oasis\..*$/i', $fileType))
        {
            return "docs";
        }
        else 
        {
            return "other";
        }
    }
    /**
     * Set the permissions for a row using the Table
     * 
     * @param Object $row
     */
    public function permissions($row)
    {
        $conn = $this->getConnector($row->file_reference);
        if($conn)
        {
            $conn->walk($row);
            return true;
        }
        
        return false;
    }
}
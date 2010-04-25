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
import('/components/file/util/class.FileTree.inc');

/**
 * 
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
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->redirect("/".Config::get('fileadmin', 'fileadmin', 'admin'));
        }
    }
    public function action_upload($smarty, $smartyLoader, $request)
    {
        $request["file_name"] = basename($_FILES['file_data']['name']);
        $request["file_date"] = "now";
        
        try
        {
            $return = Table::init("file.files")
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->ajax(array("succes" => true, "id" => $return));
        }
        catch(Exception $e)
        {
            $this->ajax(array("error" => true, "exception" => $e->getMessage()));
        }
    }
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRecord((object) $request)
                ->doUpdate();
            
            $this->redirect("/".Config::get('fileadmin', 'fileadmin', 'admin'));
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
                $this->redirect("/".Config::get('fileadd', 'fileadd', 'admin'));
            }
        }
    }
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("file.files")
                ->setRequest((object) $request)
                ->doDelete();
            
            $this->redirect("/".Config::get('fileadmin', 'fileadmin', 'admin'));
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
                $this->redirect("/".Config::get('fileadmin', 'fileadmin', 'admin'));
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
        
        $mapping = array(
            "images"    => "/[contentroot]/{$filesName}/{$imageName}/",
            "docs"      => "/[contentroot]/{$filesName}/{$docName}/",
            "vid"       => "/[contentroot]/{$filesName}/{$vidName}/",
            "other"     => "/[contentroot]/{$filesName}/{$otherName}/",
        );
        
        $tree->addChild(new UploadFolder($filesName, "/", $mapping));
        $tree->addChild(new ImageFolder($imageName, "/{$filesName}/"));
        $tree->addChild(new Folder($docName, "/[contentroot]/{$filesName}/"));
        $tree->addChild(new Folder($vidName, "/[contentroot]/{$filesName}/"));
        $tree->addChild(new Folder($otherName, "/[contentroot]/{$filesName}/"));
        
        foreach($result as $row)
        {
            $row->file_location = $mapping[$this->getType($row->file_data->type)].(!empty($row->file_tag)? $row->file_tag."/" : "");
            $tree->addChild(new ImageItem($row));
        }
    }
    
    private function getType($fileType)
    {
        if(preg_match('/^image\/.*$/i', $fileType))
        {
            return "images";
        }
        else 
        {
            return "other";
        }
    }
}
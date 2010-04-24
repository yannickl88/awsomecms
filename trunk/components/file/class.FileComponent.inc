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

        $imageName = strtolower(Language::get("images"));
        $filesName = strtolower(Language::get("files"));
        $tree->addChild(new UploadFolder($filesName, "/"));
        $tree->addChild(new ImageFolder($imageName, "/{$filesName}/"));
        
        foreach($result as $row)
        {
            $row->file_location = "/{$filesName}/{$imageName}/".(!empty($row->file_tag)? $row->file_tag."/" : "");
            $tree->addChild(new ImageItem($row));
        }
    }
}
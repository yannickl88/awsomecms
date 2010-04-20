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
        $tree->addChild(new FileFolder($filesName, ""), "/", Tree::CONTENT);
        $tree->addChild(new ImageFolder($imageName, ""), "/{$filesName}/", Tree::CONTENT);
        
        foreach($result as $row)
        {
            $tree->addChild(new ImageItem(
                $row->file_name, 
                "/".Config::get("fileedit", "fileedit", "admin")."?file_id={$row->file_id}", 
                "/".Config::get("filedelete", "filedelete", "admin")."?file_id={$row->file_id}", 
                "/".$row->file_data->url
                ), 
                "/{$filesName}/{$imageName}/".((!empty($row->file_tag))? $row->file_tag."/" : ""), Tree::CONTENT
            );
        }
    }
}
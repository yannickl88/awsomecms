<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.gallery
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("TagField");
importField("DateField");
importField("TextField");
importField("file.UploadField");

/**
 * Core table, this handels all interaction with the core table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FilesTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("files");
        $this->assign("file");
        
        $this->addField(new IntField("file_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("file_name"), 1);
        $this->addField(new DateField("file_date"), 2);
        $this->addField(new UploadField("file_data", Config::get("uploaddir", Config::get("frameworkroot", "/"), "file")), 3);
        $this->addField(new TagField("file_tag", Field::OPTIONAL|Field::FORCEUPDATE), 4);
        $this->addField(new TextField("file_description", Field::HIDE_VIEW|Field::OPTIONAL), 5);
        $this->setTitle("[file_name]");
        
        $this->setURLs(
            Config::get('fileadmin', 'fileadmin', 'admin'),
            Config::get('fileadd', 'fileadd', 'admin'),
            Config::get('fileedit', 'fileedit', 'admin'),
            Config::get('filedelete', 'filedelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderBy("file_name", "desc");
        
        if($data)
        {
            if($data->max)
            {
                $query->limit($data->max);
            }
            $query->whereWhenSet("file_tag", $data->image_tag);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
    protected function delete($query, $data = null) {}

}
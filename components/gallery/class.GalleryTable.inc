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
importField("file.FileSelectField");

/**
 * Core table, this handels all interaction with the core table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GalleryTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("gallery");
        $this->assign("gallery");
        
        $this->addField(new IntField("gallery_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("gallery_name"), 1);
        $this->addField(new FileSelectField("gallery_images", array("gif", "jpg", "png", "bmp"), Field::OPTIONAL), 2);
        $this->addField(new Field("gallery_tags"), 3);
        $this->setTitle("[gallery_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderBy("gallery_name");
        
        if(isset($data->gallery_name))
        {
            $query->where("gallery_name", $data->gallery_name);
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
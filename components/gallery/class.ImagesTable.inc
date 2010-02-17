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

/**
 * Core table, this handels all interaction with the core table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class ImagesTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("images");
        $this->assign("gallery");
        
        $this->addField(new IntField("image_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("image_title"), 1);
        $this->addField(new Field("image_url", Field::OPTIONAL|Field::HIDE));
        $this->addField(new Field("image_urlthumb", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_width", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_height", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_thumbwidth", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_thumbheight", Field::OPTIONAL|Field::HIDE));
        $this->addField(new Field("image_tag", Field::OPTIONAL), 2);
        $this->setTitle("[image_title]");
        
        $this->setURLs(
            Config::get('galleryadmin', 'galleryadmin', 'admin'),
            Config::get('galleryadd', 'galleryadd', 'admin'),
            Config::get('galleryedit', 'galleryedit', 'admin'),
            Config::get('gallerydelete', 'gallerydelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderBy("image_id", "desc");
        
        if($data)
        {
            if($data->max)
            {
                $query->limit($data->max);
            }
            $query->whereWhenSet("image_tag", $data->image_tag);
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
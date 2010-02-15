<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.slideshow
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");

/**
 * Images table, this handels all interaction with the images table in the database
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
        parent::__construct("s_images");
        $this->assign("slideshow");
        
        $this->addField(new IntField("image_id", "", Field::AUTOKEY|Field::INDEX));
        $this->addField(new Field("image_title", "Title"));
        $this->addField(new Field("image_url", "", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_width", "", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_height", "", Field::OPTIONAL|Field::HIDE));
        $this->addField(new IntField("image_size", "", Field::OPTIONAL|Field::HIDE));
        $this->setTitle("[image_title]");
        
        $this->setURLs(
            Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'),
            Config::get('slideshow_imageadd', 'slideshow_imageadd', 'admin'),
            Config::get('slideshow_imageedit', 'slideshow_imageedit', 'admin'),
            Config::get('slideshow_imagedelete', 'slideshow_imagedelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null) {}
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
    /**
     * (non-PHPdoc)
     * @see core/Table#getAction($mode)
     */
    public function getAction($mode)
    {
        if(!is_numeric($mode))
        {
            $mode = $this->string2action($mode);
        }
        
        switch($mode)
        {
            case Field::EDIT:
                return "edit_image";
                break;
            case Field::DELETE:
                return "delete_image";
                break;
            case Field::ADD:
            default:
                return "add_image";
                break;
        }
    }

}
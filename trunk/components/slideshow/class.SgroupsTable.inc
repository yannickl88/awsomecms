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
 * Groups table, this handels all interaction with the groups table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class SgroupsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("s_groups");
        $this->assign("slideshow");
        
        $this->addField(new IntField("group_id", Field::AUTOKEY|Field::INDEX));
        $this->addField(new Field("group_title"));
        $this->addField(new IntField("group_header", Field::HIDE));
        $this->setTitle("[group_title]");
        
        $this->setURLs(
            Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'),
            Config::get('slideshow_groupadd', 'slideshow_groupadd', 'admin'),
            Config::get('slideshow_groupedit', 'slideshow_groupedit', 'admin'),
            Config::get('slideshow_groupdelete', 'slideshow_groupdelete', 'admin')
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
     * @see core/Table#doSelect()
     */
    public function doSelect()
    {
        $result = parent::doSelect();
        
        if($this->getExternal()) //if we ask this from the external api, we need some more info
        {
            foreach($result->records as $record)
            {
                $header = Table::init("slideshow.images")
                    ->setRequest((object) array("image_id" => $record->group_header))
                    ->doSelect()
                    ->getRow();
                        
                $record->image_id = $header->image_id;
                $record->image_title = $header->image_title;
                $record->image_url = $header->image_url;
                $record->image_width = $header->image_width;
                $record->image_height = $header->image_height;
                $record->image_size = $header->image_size;
                
                $record->images = Table::init("slideshow.images")
                    ->setRequest((object) array("group_id" => $record->group_id))
                    ->doSelect()
                    ->getRows();
            }
        }
        
        return $result;
    }
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
                return "edit_group";
                break;
            case Field::DELETE:
                return "delete_group";
                break;
            case Field::ADD:
            default:
                return "add_group";
                break;
        }
    }
}
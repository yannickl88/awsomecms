<?php
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
        $this->addField(new FileSelectField("gallery_images", array("gif", "jpg", "jpeg", "png", "bmp"), Field::OPTIONAL|Field::FORCEUPDATE), 2);
        $this->addField(new Field("gallery_tags", Field::OPTIONAL|Field::FORCEUPDATE), 3);
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
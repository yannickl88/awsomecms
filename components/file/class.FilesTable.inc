<?php
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
        $this->addField(new UploadField("file_data", Config::get("uploaddir", "/", "file")), 3);
        $this->addField(new Field("file_location", Field::HIDE_VIEW), 4);
        $this->addField(new TagField("file_tag", Field::OPTIONAL|Field::FORCEUPDATE), 5);
        $this->addField(new TextField("file_description", Field::HIDE_VIEW|Field::OPTIONAL), 6);
        $this->addField(new Field("file_reference", Field::HIDE|Field::OPTIONAL));
        $this->addField(new TablePermissionField(array(Component::init("file"), "permissions")));
        $this->setTitle("[file_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderBy("file_name", "ASC");

        if($data)
        {
            if($data->max)
            {
                $query->limit($data->max);
            }
            $query->whereWhenSet("file_tag", $data->file_tag);
            $query->whereWhenSet("file_reference", $data->file_reference);
            $query->whereWhenSet("file_location", $data->file_location);
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
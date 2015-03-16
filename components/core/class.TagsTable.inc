<?php
/**
 * Auth table, this handels all interaction with the auth table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TagsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("tags");

        $this->addField(new IntField("tag_id", Field::AUTOKEY));
        $this->addField(new Field("tag_name"));
        $this->addField(new Field("tag_field"));
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

}
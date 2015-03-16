<?php
/**
 * Core table, this handels all interaction with the core table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class ComponentsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("components");
        $this->assign("core");

        $this->addField(new Field("component_name", Field::STR_INDEX));
        $this->addField(new TextField("component_requests", Field::OPTIONAL));
        $this->addField(new Field("component_path", Field::OPTIONAL));
        $this->addField(new IntField("component_auth", Field::OPTIONAL));
        $this->addField(new IntField("component_patchlevel", Field::OPTIONAL));
        $this->addField(new Field("component_version", Field::OPTIONAL));
        $this->addField(new Field("component_updateurl", Field::OPTIONAL));
        $this->setTitle("[component_name]");
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
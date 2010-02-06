<?php
importField("Field");
importField("IntField");
importField("TextField");

class ComponentsTable extends Table
{
    public function __construct()
    {
        parent::__construct("components");
        $this->assign("core");
        
        $this->addField(new Field("component_name", "Name", Field::STR_INDEX));
        $this->addField(new TextField("component_requests", "", Field::OPTIONAL));
        $this->addField(new Field("component_path", "", Field::OPTIONAL));
        $this->addField(new IntField("component_auth", "", Field::OPTIONAL));
        $this->addField(new IntField("component_patchlevel", "", Field::OPTIONAL));
        $this->addField(new Field("component_version", "", Field::OPTIONAL));
        $this->addField(new Field("component_updateurl", "", Field::OPTIONAL));
        $this->setTitle("[component_name]");
    }
    
    
    protected function select($query, $data = null) {}
    protected function insert($query, $data = null) {}
    protected function update($query, $data = null) {}
    protected function delete($query, $data = null) {}

}
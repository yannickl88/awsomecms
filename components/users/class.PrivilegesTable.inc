<?php
class PrivilegesTable extends Table
{
    public function __construct()
    {
        parent::__construct("privileges");
        $this->assign("users");
        
        $this->addField(new Field("privilege_name", "", Field::STR_INDEX|Field::FORCEUPDATE));
        $this->addField(new IntField("privilege_int", "", Field::HIDE));
        $this->addField(new IntField("privilege_locked", "", Field::HIDE|Field::OPTIONAL));
    }
    
    protected function select($query, $data = array())
    {
        $query->orderby('privilege_name');
        
        $query->where('privilege_name', '*deleted*', "!=");
        $query->whereWhenSet('privilege_name', $data['privilege_name']);
        $query->whereWhenSet('privilege_int', $data['privilege_int']);
    }
    protected function insert($query, $data = array())
    {
        $rows = SQLQuery::doSelect()
            ->table('privileges')
            ->exec()
            ->getRows();
        
        $query->insert('privilege_int', pow(2, count($rows)));
    }
    protected function update($query, $data = array())
    {
        $query->where('privilege_name', $data['privilege_name_old']);
    }
    
    protected function delete($query, $data = array()) {}
}
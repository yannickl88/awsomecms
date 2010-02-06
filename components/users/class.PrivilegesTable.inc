<?php
importField("Field");
importField("IntField");

class PrivilegesTable extends Table
{
    public function __construct()
    {
        parent::__construct("privileges");
        $this->assign("users");
        
        $this->addField(new IntField("privilege_id", "", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("privilege_name", "Name", Field::FORCEUPDATE));
        $this->addField(new IntField("privilege_int", "", Field::HIDE|Field::OPTIONAL));
        $this->addField(new IntField("privilege_locked", "", Field::HIDE|Field::OPTIONAL));
        $this->setTitle("[privilege_name]");
        
        $this->setURLs(
            Config::get('privadmin', 'privadmin', 'admin'),
            Config::get('privadd', 'privadd', 'admin'),
            Config::get('privedit', 'privedit', 'admin'),
            Config::get('privdelete', 'privdelete', 'admin')
        );
    }
    
    protected function select($query, $data = null)
    {
        $query->orderby('privilege_name');
        
        $query->where('privilege_name', '*deleted*', "!=");
        if($data)
        {
            $query->whereWhenSet('privilege_name', $data->privilege_name);
            $query->whereWhenSet('privilege_int', $data->privilege_int);
        }
    }
    protected function insert($query, $data = null)
    {
        $rows = SQLQuery::doSelect()
            ->table('privileges')
            ->exec()
            ->getRows();
        
        $query->insert('privilege_int', pow(2, count($rows)));
    }
    
    protected function update($query, $data = null) {}
    protected function delete($query, $data = null) {}
    
    /**
     * @see core/Table#doDelete($data)
     * 
     * Because we need to know how many privileges there are we do not want to delete them. 
     * Rather we set them as deleted so we can still cout them
     */
    public function doDelete($data = array())
    {
        $this->validate(Field::DELETE);
        
        $query = SQLQuery::doUpdate();
        $query->table($this->name);
        $query->update('privilege_name', '*deleted*');
        
        foreach($this->fields as $field)
        {
            //still calling the delete, we need the where stuff they add. 
            //And since this is an Update delete items will be ignored.
            $field->delete($query, $this->record[0]);
        }
        
        return $query->exec();
    }
    
    public function getAction($mode)
    {
        if(!is_numeric($mode))
        {
            $mode = $this->string2action($mode);
        }
        
        switch($mode)
        {
            case Field::EDIT:
                return "privedit";
                break;
            case Field::DELETE:
                return "privdelete";
                break;
            case Field::ADD:
            default:
                return "privadd";
                break;
        }
    }
}
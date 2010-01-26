<?php
import('/core/shared/class.RequestComponent.inc');

class PrivilegesRequest extends RequestComponent
{
    //CORE ACTIONS
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('privileges');
        
        if(isset($data['privilege_name']))
        {
            $query->where('privilege_name', $data['privilege_name']);
        }
        if(isset($data['privilege_int']))
        {
            $query->where('privilege_int', $data['privilege_int']);
        }
        
        $query->where('privilege_name', '*deleted*', "!=");
        $query->orderBy('privilege_name');
        
        return $query->exec()->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $query = SQLQuery::doUpdate()
            ->update('privilege_name', '*deleted*')
            ->where('privilege_name', $data['privilege_name'])
            ->table('privileges')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('privilege_name', $data['privilege_name'])
            ->where('privilege_name', $data['privilege_name_old'])
            ->table('privileges')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //first let us request all the records
        $rows = SQLQuery::doSelect()
            ->table('privileges')
            ->exec()
            ->getRows();
        
        //insert
        $result = SQLQuery::doInsert('privilege_name', $data['privilege_name'])
            ->insert('privilege_int', pow(2, count($rows)))
            ->table('privileges')
            ->exec();
        
        return $result->insertID();
    }
}
?>
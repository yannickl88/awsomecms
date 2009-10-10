<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.RequestComponent.inc';

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
        
        if(isset($data['privilege_name']))
        {
            $rows = $query->exec();
            $total = 0;
            
            while($row = $rows->getRow())
            {
                $total += $row->privilege_int;
            }
            
            return $total;
        }
        else
        {
            return $query->exec()->getRows();
        }
    }
}
?>
<?php
import('/core/shared/class.RequestComponent.inc');

class ContactRequest extends RequestComponent
{
    //CORE ACTIONS
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('forms')
            ->join('form_fields', 'field_form', 'form_id') //relation with forms table
            ->orderBy('field_id'); //relation with forms table
        
        if(isset($data['form_id']) && (is_numeric($data['form_id']) || is_array($data['form_id'])))
        {
            $query->where('form_id', $data['form_id']);
        }
        if(isset($data['form_name']))
        {
            $query->where('form_name', $data['form_name']);
        }
        
        $result = $query->exec();
        
        $record = array();
        $currentForm = null;
        
        while($result->count() > 0 && $row = $result->getRow())
        {
            if($currentForm == null)
            {
                $currentForm = $row;
                $currentForm->fields = array();
            }
            
            if($currentForm->form_name != $row->form_name)
            {
                $record[] = $currentForm;
                $currentForm = $row;
                $currentForm->fields = array();
            }
            
            if($row->field_id != null)
            {
                array_push($currentForm->fields, $row);
            }
        }
        
        if($currentForm != null)
        {
            $record[] = $currentForm;
        }
        
        return $record;
    }
    protected function do_DELETE($data = array())
    {
        //delete
        SQLQuery::doDelete()
            ->table("form_fields")
            ->where("field_form", $data['form_id'])
            ->exec();
            
        SQLQuery::doDelete()
            ->where('form_id', $data['form_id'])
            ->table('forms')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        parse_str($data['fields'], $fields);
        
        //update forms table
        $query = SQLQuery::doUpdate()
            ->where('form_id', $data['form_id'])
            ->table('forms')
            ->updateWhenSet('form_name', $data['form_name'])
            ->updateWhenSet('form_to', $data['form_to'])
            ->update('form_bcc', $data['form_bcc'])
            ->updateWhenSet('form_subject', $data['form_subject'])
            ->updateWhenSet('form_thanks', $data['form_thanks'])
            ->exec();
            
        //update form_fields table
        SQLQuery::doDelete()
            ->table("form_fields")
            ->where("field_form", $data['form_id'])
            ->exec();
        
        if($fields["field_sort"] != null)
        {
            asort($fields["field_sort"]);
            
            foreach($fields["field_sort"] as $key => $value)
            {
                $result = SQLQuery::doInsert()
                    ->insert('field_form', $data['form_id'])
                    ->insert('field_name', $fields["field_name"][$key])
                    ->insert('field_code', $fields["field_code"][$key])
                    ->insert('field_type', $fields["field_type"][$key])
                    ->insert('field_required', $fields["field_required"][$key])
                    ->table('form_fields')
                    ->exec();
            }
        }
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        parse_str($data['fields'], $fields);
        
        $result = SQLQuery::doInsert('form_name', $data['form_name'])
            ->insert('form_to', $data['form_to'])
            ->insert('form_bcc', $data['form_bcc'])
            ->insert('form_subject', $data['form_subject'])
            ->insert('form_thanks', $data['form_thanks'])
            ->table('forms')
            ->exec();
        
        if($fields["field_sort"] != null)
        {
            asort($fields["field_sort"]);
        
            foreach($fields["field_sort"] as $key => $value)
            {
                SQLQuery::doInsert()
                    ->insert('field_form', $result->insertID())
                    ->insert('field_name', $fields["field_name"][$key])
                    ->insert('field_code', $fields["field_code"][$key])
                    ->insert('field_type', $fields["field_type"][$key])
                    ->insert('field_required', $fields["field_required"][$key])
                    ->table('form_fields')
                    ->exec();
            }
        }
        
        return $result->insertID();
    }
}
?>
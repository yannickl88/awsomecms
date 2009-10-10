<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.Component.inc';

class ContactComponent extends Component
{
    public function __construct()
    {
        parent::__construct('contact');
        
        $this->fields = array(
            "form_id" => "index",
            "form_name" => "text",
            "form_to" => "text",
            "form_subject" => "text",
            "form_thanks" => "text"
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }

    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('contactadd', 'contactadd', 'admin'), "add"))
            {
                $id = $this->insert($request);
            }
            
            $this->redirect("/".Config::get('contactadmin', 'contactadmin', 'admin'));
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('contactedit', 'contactedit', 'admin'), "edit"))
            {
                $this->update($request);
            }
            
            $this->redirect("/".Config::get('contactadmin', 'contactadmin', 'admin'), array("form_id" => $request['form_id']));
        }
        else
        {
            if($request['form_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('contactadd', 'contactadd', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $this->delete($request);
            
            $this->redirect("/".Config::get('contactadmin', 'contactadmin', 'admin'));
        }
        else
        {
            if($request['form_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('contactadd', 'contactadd', 'admin'));
            }
        }
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", $this->select());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('contactadd', 'contactadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('contactedit', 'contactedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('contactdelete', 'contactdelete', 'admin'));
    }
    
    public function action_send($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post' && isset($request['form_id']) && is_numeric($request['form_id']))
        {
            $form = $this->select(array("form_id" => $request['form_id']));
            $form = $form[0];
            
            $to         = $form->form_to;
            $bcc        = $form->form_bcc;
            $subject    = $form->form_subject;
            
            $valid = true;
            $body = '';
            
            if(empty($to) || empty($subject))
            {
                $this->redirect($form->form_thanks);
            }
            
            $e = new InvalidFormException($request, $request['url'], "Password do not match");
            
            foreach($form->fields as $field)
            {
                if($field->field_required == 1 && trim($request[$field->field_code]) == '')
                {
                    $valid = false;
                    
                    $e->addInvalidField($field->field_code, "Field was not filled in");
                    
                    continue;
                }
                
                if($body != "")
                {
                    $body .= "\n\n";
                }
                
                $body .= "{$field->field_name}: ".$request[$field->field_code];
            }
            
            if($valid)
            {
                $headers .= "Bcc: {$bcc}\r\n";

                @mail($to, $subject, $body, $headers);
                die(var_dump($body));
            }
            else
            {
                throw $e;
            }
            
            $this->redirect($form->form_thanks);
        }
    }
}
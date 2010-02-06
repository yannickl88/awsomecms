<?php
import('/core/class.Component.inc');

/**
* Contact component class, this holds all the logic for the contact component
*/
class ContactComponent extends Component
{
    /**
    * Constructor class for the contact component
    */
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
    
    /**
    * Add action, this is called when a contact form is added
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
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
    
    /**
    * Edit action, this is called when a contact form is edited
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
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
    
    /**
    * Delete action, this is called when a form is deleted
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
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
    
    /**
    * Admin action, this is called when a showing the admin overview of the forms
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", $this->select());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('contactadd', 'contactadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('contactedit', 'contactedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('contactdelete', 'contactdelete', 'admin'));
    }
    
    /**
    * Send action, this is called when a form is submitted
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
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
            }
            else
            {
                throw $e;
            }
            
            $this->redirect($form->form_thanks);
        }
    }
    
    /**
    * Function for registering menu items to the admin menu
    * 
    * @param Menu $menu
    */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("contact", "", Config::get('contactadmin', 'contactadmin', 'admin')));
        $menu->addChild(new MenuItem("forms", "", Config::get('contactadmin', 'contactadmin', 'admin'), "contact"));
        $menu->addChild(new MenuItem("formsadd", "Add", Config::get('contactadd', 'contactadd', 'admin'), "contact"));
    }
}
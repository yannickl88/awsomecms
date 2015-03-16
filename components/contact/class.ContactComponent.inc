<?php
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
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("contact.action_add", "admin");
        $registerManager->registerAuth("contact.action_edit", "admin");
        $registerManager->registerAuth("contact.action_delete", "admin");

        $registerManager->registerPage(array($this, 'admin'), '{form table="contact.forms" form="admin"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="contact.forms" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="contact.forms" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="contact.forms" form="delete"}');
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
        if($request['method'] == 'post')
        {
            $id = Table::init("contact.forms")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $fields;
            parse_str($request['form_fields'], $fields);

            if($fields["field_sort"] != null)
            {
                asort($fields["field_sort"]);

                foreach($fields["field_sort"] as $key => $value)
                {
                    $record = (object) array(
                        "field_form" => $id,
                        "field_name" => $fields["field_name"][$key],
                        "field_code" => $fields["field_code"][$key],
                        "field_type" => (int) $fields["field_type"][$key],
                        "field_required" => (int) $fields["field_required"][$key],
                    );

                    Table::init("contact.formfields")
                        ->setRecord((object) $record)
                        ->doInsert();
                }
            }

            $this->redirect(array($this, 'admin'));
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
        if($request['method'] == 'post')
        {
            Table::init("contact.forms")
                ->setRecord((object) $request)
                ->doUpdate();

            Table::init("contact.formfields")
                ->setRequest((object) array("form_id" => $request['form_id']))
                ->doDelete();

            $id = $request['form_id'];
            $fields;
            parse_str($request['form_fields'], $fields);

            if($fields["field_sort"] != null)
            {
                asort($fields["field_sort"]);

                foreach($fields["field_sort"] as $key => $value)
                {
                    $record = (object) array(
                        "field_form" => $id,
                        "field_name" => $fields["field_name"][$key],
                        "field_code" => $fields["field_code"][$key],
                        "field_type" => (int) $fields["field_type"][$key],
                        "field_required" => (int) $fields["field_required"][$key],
                    );

                    Table::init("contact.formfields")
                        ->setRecord((object) $record)
                        ->doInsert();
                }
            }

            $this->redirect(array($this, 'admin'), array("form_id" => $request['form_id']));
        }
        else
        {
            if($request['form_id'])
            {
                $request['form_fields'] = Table::init("contact.formfields")
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRows();

                Table::init("contact.forms")
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect(array($this, 'admin'));
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
        if($request['method'] == 'post')
        {
            Table::init("contact.formfields")
                ->setRequest((object) array("form_id" => $request['form_id']))
                ->doDelete();

            Table::init("contact.forms")
                ->setRequest((object) $request)
                ->doDelete();

            $this->redirect(array($this, 'admin'));
        }
        else
        {
            if($request['form_id'])
            {
                Table::init("contact.forms")
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect(array($this, 'admin'));
            }
        }
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
        if($request['method'] == 'post' && isset($request['form_id']) && is_numeric($request['form_id']))
        {
            $form = Table::init("contact.forms")
                ->setRequest((object) array("form_id" => $request['form_id']))
                ->doSelect()
                ->getRow();
            $fields = $form->fields;

            $to         = $form->form_to;
            $bcc        = $form->form_bcc;
            $subject    = $form->form_subject;

            $valid = true;
            $body = '';
            $headers = '';

            if(empty($to) || empty($subject))
            {
                $this->redirect($form->form_thanks);
            }

            $e = new InvalidFormException((object) $request, "/".$request['url'], "");
            $img = new Securimage();

            if(!$img->check($request['captcha']))
            {
                $valid = false;

                $e->addInvalidField('captcha', Language::get("error_wrongcaptcha"));
            }

            foreach($fields as $field)
            {
                if($field->field_required == 1 && trim($request[$field->field_code]) == '')
                {
                    $valid = false;

                    $e->addInvalidField($field->field_code, Language::get("error_notfilledin"));

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
        $menu->addChild(new MenuItem("contact", array($this, 'admin')));
        $menu->addChild(new MenuItem("forms", array($this, "admin"), "contact"));
    }
}
<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.contact
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("page.LocationField");
importField("contact.FieldsField");

/**
 * Form table, this is where all the forms are stored
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FormsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("forms");
        $this->assign("contact");
        
        $this->addField(new IntField("form_id", Field::AUTOKEY|Field::INDEX));
        $this->addField(new Field("form_name"), 1);
        $this->addField(new Field("form_to", Field::HIDE_VIEW), 2);
        $this->addField(new Field("form_bcc", Field::HIDE_VIEW|Field::OPTIONAL), 3);
        $this->addField(new Field("form_subject"), 4);
        $this->addField(new LocationField("form_thanks", Field::HIDE_VIEW, true), 5);
        $this->addField(new FieldsField("form_fields", Field::DUMMY), 6);
        $this->setTitle("[form_name]");
        
        $this->setURLs(
            Config::get('contactadmin', 'contactadmin', 'admin'),
            Config::get('contactadd', 'contactadd', 'admin'),
            Config::get('contactedit', 'contactedit', 'admin'),
            Config::get('contactdelete', 'contactdelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        if($data)
        {
            $query->whereWhenSet("form_name", $data->form_name);
        }
    }
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
    
    public function doSelect()
    {
        $result = parent::doSelect();
        
        $result->getRow()->fields = Table::init("contact.formfields")
            ->setRequest($result->reset()->getRow())
            ->doSelect()
            ->getRows();
            
        return $result->reset();
    }
}
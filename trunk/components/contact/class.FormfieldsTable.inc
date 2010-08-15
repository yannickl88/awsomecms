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
importField("BooleanField");

/**
 * Form table, this is where all the forms are stored
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FormfieldsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("form_fields");
        $this->assign("contact");
        
        $this->addField(new IntField("field_id", Field::AUTOKEY));
        $this->addField(new IntField("field_form"));
        $this->addField(new Field("field_name"));
        $this->addField(new Field("field_code"));
        $this->addField(new IntField("field_type"));
        $this->addField(new BooleanField("field_required"));
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null) 
    {
        $query->orderBy("field_id");
        
        if($data)
        {
            $query->whereWhenSet("field_form", $data->form_id);
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
    protected function delete($query, $data = null)
    {
        if($data)
        {
            $query->whereWhenSet("field_form", $data->form_id);
        }
    }
}
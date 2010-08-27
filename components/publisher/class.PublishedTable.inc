<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.publisher
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("DateField");
importField("TextField");
/**
 * Table to keep track of published items
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PublishedTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("published");
        $this->assign("publisher");
        
        $this->addField(new IntField("publish_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new TextField("publish_link"));
        $this->addField(new Field("publish_to"));
        $this->addField(new DateField("publish_date"));
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null) {}
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

}
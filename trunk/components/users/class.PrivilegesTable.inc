<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.users
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");

/**
 * Privileges tabel.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PrivilegesTable extends Table
{
    /**
     * @see core/Table#__construct($name)
     */
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
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
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
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null)
    {
        $rows = SQLQuery::doSelect()
            ->table('privileges')
            ->exec()
            ->getRows();
        
        $query->insert('privilege_int', pow(2, count($rows)));
    }
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
    /**
     * @see core/Table#getAction($mode)
     * 
     * Since this tabel is assigned to the users, we need a different set of actions. 
     * So we override this function to accomodate for that.
     */
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
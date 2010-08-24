<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * TablePermissionField field, with this field you can add a callback to each 
 * row which will hide icons
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TablePermissionField extends Field
{
    private $premissionCallback;
    /**
     * Constructor
     * 
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($premissionCallback)
    {
        parent::__construct("permissions", Field::HIDE|Field::DUMMY);
        
        if(is_callable($premissionCallback))
        {
            $this->premissionCallback = $premissionCallback;
        }
        else
        {
            $this->premissionCallback = array($this, "premissions");
            Debugger::getInstance()->warning("Callback is not callable");
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::postSQL()
     *
     * @param SQLRecord $record
     * @param SQLQuery $query
     */
    public function postSQL($record, $query)
    {
        $rows = $record->getRows();
        
        foreach($rows as $row)
        {
            if(!call_user_func($this->premissionCallback, $row))
            {
                $this->premissions($row);
            }
        }
    }
    /**
     * Set the premissions for this row if the callback was not valid
     * 
     * @param Object $row
     */
    private function premissions($row)
    {
        $row->permissions = array(
            "view" => true, 
            "edit" => true,
            "delete" => true
        );
    }
}
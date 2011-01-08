<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * Field for selecting the location within the site
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LocationField extends Field
{
    protected $displayPages;
    
    /**
     * Constructor
     * 
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $style=0, $displayPages = false)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
        
        $this->setDefaultValue("/");
        $this->displayPages = $displayPages;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $smarty = Controller::getInstance()->getSmarty();
        $smarty->loadPlugin("smarty_function_admintree");
        
        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => !$this->displayPages, "types" => 1), $smarty);
        $html .= "<input class='cmsfield' type='text' name='{$this->name}' id='{$this->name}' value='{$this->defaultValue}'/>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $smarty = Controller::getInstance()->getSmarty();
        $smarty->loadPlugin("smarty_function_admintree");
        
        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => !$this->displayPages, "types" => 1), $smarty);
        $html .= "<input class='cmsfield' type='text' name='{$this->name}' id='{$this->name}' value='{$this->value}'/>";
        
        return $html;
    }
}
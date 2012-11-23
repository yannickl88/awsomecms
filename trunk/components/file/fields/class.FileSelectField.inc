<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.file.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * Field for uploading files
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FileSelectField extends Field
{
    protected $types;
    protected $multiSelect = true;
    protected $autoOpen;
    
    /**
     * Constructor
     *
     * @param string $name
     * @param int $style
     */
    public function __construct($name, $types, $style=0, $autoOpen = false)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
        $this->types = $types;
        $this->autoOpen = $autoOpen;
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
        $checkboxes = Tree::RADIO;
        if($this->multiSelect)
            $checkboxes = Tree::CHECK;
        
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => false, "types" => 2, "checkboxes" => $checkboxes, "value" => array(), "filetypes" => $this->types, "noCookie" => !$this->autoOpen), $smarty);
        
        return $html;
    }
    private function sort($a, $b)
    {
        return strcmp($a->file_location.$a->file_name, $b->file_location.$b->file_name);
    }
    /**
     * Set the selection box to multiple selection or not
     * 
     * @param Boolean $multi
     */
    public function setMultiSelect($multi = true)
    {
        $this->multiSelect = $multi;
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
        $checkboxes = Tree::RADIO;
        if($this->multiSelect)
            $checkboxes = Tree::CHECK;
        
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => false, "types" => 2, "checkboxes" => $checkboxes, "value" => $this->getValue(), "filetypes" => $this->types, "noCookie" => !$this->autoOpen), $smarty);
        
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see Field::toHTML_view()
     */
    protected function toHTML_disabled()
    {
        //return print_r($this->getValue(), true);
        
        $value = $this->getValue();
        
        if(empty($value))
            $info = Language::get("nofile");
        else
        {
            $c = Component::init("file");
            
            $files = Table::init("file.files")
                ->setRequest((object) array("file_id" => $this->getValue()))
                ->doSelect()
                ->getRows();
            
            $info = "";
            foreach($files as $file)
            {
                switch ($c->getType($file->file_data->type))
                {
                    case "images":
                        $info .= "<img src='{$file->file_data->url}' alt='preview' style='max-height: 150px;'/><br />";
                        $info .= "<img src='/img/icons/image.png' alt='image'/>";
                        break;
                    case "vid":
                        $info .= "<img src='/img/icons/vid.png' alt='video'/>";
                        break;
                    case "doc":
                        $info .= "<img src='/img/icons/doc.png' alt='document'/>";
                        break;
                    default:
                        $info .= "<img src='/img/icons/other.png' alt='other'/>";
                    break;
                }
                $info .= " ".$file->file_data->url."<br />";
            }
            
            return $info;
        }
    }
    /**
     * Convert the dbvalue of the param to what the field expects
     * 
     * @param $value mixed
     * @return mixed
     */
    public function db2value($value)
    {
        if(!empty($value))
        {
            return explode(",", $value);
        }
        
        return $value;
    }
    /**
     * Convert the value of the param to that what the DB expects
     * 
     * @param $value String
     * @return String
     */
    public function value2db($value)
    {
        if(!empty($value) && is_array($value))
        {
            return implode(",", $value);
        }
        
        return $value;
    }
}
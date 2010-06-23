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
 * Tag field, this allows for quick search and assignment of tags
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TagField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#preSQL()
     */
    public function preSQL(&$record, $query)
    {
        $value = $this->getValue();
        $strings = explode(",", $value);
        $tags = array();
        
        foreach($strings as $tag)
        {
            $tag = trim($tag);
            if(!empty($tag) && !in_array($tag, $tags))
            {
                $tags[] = $tag;
            }
        }
        
        sort($tags);
        
        $this->setValue(implode(", ", $tags));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#insert()
     */
    public function select($query, $data = array())
    {
        $value = $this->getRequestValue();
        
        if(!empty($value))
        {
            $ids = array();
            $tags = $this->getAllTags();
            $requestTags = explode(",", $value);
            
            foreach($requestTags as $tag)
            {
                $tag = trim($tag);
                
                if(isset($tags[$tag]))
                {
                    $ids = array_merge($ids, $tags[$tag]);
                }
            }
            
            if(count($ids) > 0)
            {
                $query->where("file_id", $ids);
            }
        }
        
        parent::select($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#insert()
     */
    public function insert($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::insert($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#update()
     */
    public function update($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::update($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#delete()
     */
    public function delete($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::delete($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        $tags = json_encode(array_keys($this->getAllTags()));
        
        return <<<JS
            new TagField($("#{$this->getDBName()}"), {$tags});
JS;
    }
    
    private function getAllTags()
    {
        if(Cache::has($this->getDBName()."_tags"))
        {
            return Cache::get($this->getDBName()."_tags");
        }
        else
        {
            $result = Table::init($this->parent->ID, $this->getDBName()."_tags")
                ->doSelect()
                ->getRows();
            
            $fieldName = $this->getDBName();
            $keyField = array_shift($this->parent->getKeys())->getDBName();
            $tags = array();
            
            foreach($result as $row)
            {
                if(!in_array($row->$fieldName, $tags) && !empty($row->$fieldName))
                {
                    $fieldTags = explode(",",$row->$fieldName);
                    
                    foreach($fieldTags as $tag)
                    {
                        $tag = trim($tag);
                        
                        if(!isset($tags[$tag]))
                        {
                            $tags[$tag] = array();
                        }
                        
                        $tags[$tag][] = (int) $row->$keyField;
                    }
                }
            }
            
            asort($tags);
            Cache::set($this->getDBName()."_tags", $tags);
            
            return $tags;
        }
    }
}
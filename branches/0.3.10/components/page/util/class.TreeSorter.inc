<?php
class TreeSorter
{
    private $tree;
    
    public function __construct($tree)
    {
        $this->tree = $tree;
    }
    
    public function sort()
    {
        return array("items" => $this->sortItems($this->tree['items']));
    }
    
    private function sortItems($branch)
    {
        if(!is_array($branch))
        {
            return $branch;
        }
        $folders = array();
        $files = array();
        
        foreach($branch as $key => $value)
        {
            if(!is_numeric($key))
            {
                $folders[$key] = $value;
                $folders[$key]['items'] = $this->sortItems($value['items']);
            }
            else
            {
                $files[$key] = $value;
            }
        }
        
        ksort($folders);
        uasort($files, array($this, 'compare'));
        
        return array_merge($folders, $files);
    }
    
    private function compare($a, $b)
    {
        return strnatcasecmp($a->page_name, $b->page_name);
    }
}
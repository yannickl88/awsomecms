<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * The tree sorter is used to sort and manage the admin page tree
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TreeSorter
{
    private $tree;
    
    /**
     * Constructor
     * 
     * @param array $tree
     */
    public function __construct($tree)
    {
        $this->tree = $tree;
    }
    /**
     * Sort the tree, returning the new true
     * 
     * @return array
     */
    public function sort()
    {
        return array("items" => $this->sortItems($this->tree['items']));
    }
    /**
     * Algorihem for sorting the individual items
     * 
     * @param array $branch
     * @return array
     */
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
    /**
     * compare two leafes together
     * 
     * @param object $a
     * @param object $b
     * @return int
     */
    private function compare($a, $b)
    {
        return strnatcasecmp($a->page_name, $b->page_name);
    }
}
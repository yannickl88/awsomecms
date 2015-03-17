<?php 
class Collection
{
    const MACHINE     = 1;
    const MODEL       = 2;
    const TYPE        = 3;
    
    private $children;
    
    public function __construct()
    {
        $this->children = array();
    }
    
    public function load()
    {
        $cats = Table::init("collection.mcats")
            ->doSelect();
        
        while($cat = $cats->getRow())
        {
            $this->add(new CollectionElement($cat->mcat_type, $cat->mcat_name, $cat->mcat_id), $cat->mcat_parent);
        }
    }
    
    public function add($item)
    {
        if($parent == -1)
            array_push($this->children, $item);
        else
        {
            foreach($this->children as $c)
            {
                $c->add($item, $parent);
            }
        }
    }
    
    public function getType($type)
    {
        $types = array();
        
        foreach($this->children as $c)
        {
            if($c->type < $type)
            {
                $types = array_merge($types, $c->getType($type));
            }
            elseif($c->type == $type)
            {
                array_push($types, $c);
            }
        }
        
        return $types;
    }
    
    public function getChildren()
    {
        return $this->children;
    }
}

class CollectionElement
{
    private $id;
    private $name;
    private $children;
    public $type;
    
    public function __construct($type, $name, $id)
    {
        $this->name = $name;
        $this->type = $type;
        $this->id = id;
        $this->children = array();
    }
    
    public function add($item, $parent)
    {
        if($parent == $this->id)
            array_push($this->children, $item);
        else
        {
            foreach($this->children as $c)
            {
                $c->add($item, $parent);
            }
        }
    }
    
    public function getType($type)
    {
        $types = array();
        
        foreach($this->children as $c)
        {
            if($c->type < $type)
            {
                $types = array_merge($types, $c->getType($type));
            }
            elseif($c->type == $type)
            {
                array_push($types, $c);
            }
        }
        
        return $types;
    }
    
    public function getName()
    {
        return $this->name;
    }
    
    public function getID()
    {
        return $this->id;
    }
    
    public function getChildren()
    {
        return $this->children;
    }
}
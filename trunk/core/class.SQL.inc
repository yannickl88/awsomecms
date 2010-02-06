<?php
class SQLExeption extends Exception { }
class SQLConnenectionExeption extends SQLExeption { }

class SQLQuery
{
    private $select;
    private $table;
    private $join;
    private $group;
    private $order;
    private $limit;
    
    private $insert;
    
    private $update;
    
    private $values;
    private $where;
    
    private $action;
    
    private function __construct($action)
    {
        //select
        $this->select = array();
        $this->table  = array();
        $this->join   = array();
        $this->group  = array();
        $this->order  = array();
        $this->limit  = '';
        
        //insert
        $this->insert = array();
        
        //update
        $this->update = array();
        
        //common
        $this->values = array();
        $this->where  = array();
        
        $this->action = $action;
    }
    
    /**
    * return the action for this query
    * 
    * @return string    select, update, insert or delete
    */
    public function getAction()
    {
        return $this->action;
    }
    
    /**
    * Create a select query object
    * 
    * @param string $field
    * @param string $distrinct
    * @return SQLQuery
    */
    public static function doSelect($field = '', $distrinct = false)
    {
        $query = new SQLQuery('select');
        $query->select($field, $distrinct);
        
        return $query;
    }
    
    /**
    * Create a insert query object
    * 
    * @param string $field
    * @param string $value
    * @return SQLQuery
    */
    public static function doInsert($field = '', $value = '')
    {
        $query = new SQLQuery('insert');
        $query->insert($field, $value);
        
        return $query;
    }
    
    /**
    * Create a update query object
    * 
    * @param string $field
    * @param string $value
    * @return SQLQuery
    */
    public static function doUpdate($field = '', $value = '')
    {
        $query = new SQLQuery('update');
        $query->update($field, $value);
        
        return $query;
    }
    
    /**
    * Create a delete query object
    * 
    * @return SQLQuery
    */
    public static function doDelete()
    {
        $query = new SQLQuery('delete');
        return $query;
    }
    
    /**
    * add select statement
    * 
    * @param string $field
    * @param boolean $distrinct
    * @return SQLQuery
    */
    public function select($field, $distrinct = false)
    {
        if($field != '')
        {
            if($distrinct)
            {
                $field = 'DISTINCT '.$field;
            }
            
            $this->select[] = $field;
        }
        
        return $this;
    }
    
    /**
    * add insert statement
    * 
    * @param string $field
    * @param mixed $value
    * @return SQLQuery
    */
    public function insert($field, $value)
    {
        if($field != '')
        {
            $this->insert[$field] = $field;
            $this->values[$field] = $value;
        }
        
        return $this;
    }
    
    /**
    * add insert statement, only when the value is set
    * 
    * @param string $field
    * @param mixed $value
    * @return SQLQuery
    */
    public function insertWhenSet($field, $value)
    {
        if(!self::isEmpty($value))
        {
            $this->insert($field, $value);
        }
        
        return $this;
    }
    
    /**
    * add update statement
    * 
    * @param string $field
    * @param mixed $value
    * @return SQLQuery
    */
    public function update($field, $value)
    {
        if($field != '')
        {
            $this->update[$field] = $field;
            $this->values[$field] = $value;
        }
        
        return $this;
    }
    
    /**
    * add update statement, only when the value is set
    * 
    * @param string $field
    * @param mixed $value
    * @return SQLQuery
    */
    public function updateWhenSet($field, $value)
    {
        if(!self::isEmpty($value))
        {
            $this->update($field, $value);
        }
        
        return $this;
    }
    
    /**
    * add tabel
    * 
    * @param string $table
    * @return SQLQuery
    */
    public function table($table)
    {
        $this->table[] = $table;
        
        return $this;
    }
    
    /**
    * add where statement
    * 
    * @param string $field
    * @param mixed $value
    * @param string $comparison
    * @param string $orAnd  AND or OR
    * @return SQLQuery
    */
    public function where($field, $value, $comparison = "=", $orAnd = 'AND')
    {
        if(self::isEmpty($value))
        {
            throw new SQLExeption("Cannot compare '{$field}' with empty value");
        }
        
        if($orAnd != 'AND')
        {
            $orAnd = "OR";
        }
        
        if(is_array($value))
        {
            foreach($value as $key => $value2)
            {
                if(!is_numeric($value))
                {
                    $value[$key] = "'" . addSlashesConfig($value2) . "'";
                }
            }
            
            if($comparison == "!=")
            {
                $comparison = "NOT IN";
            }
            else
            {
                $comparison = "IN";
            }
            
            $value = implode(",", $value);
            
            $this->where[] = array("({$field} {$comparison}({$value}))", $orAnd);
        }
        else
        {
            if(!is_numeric($value))
            {
                $value = "'" . addSlashesConfig($value) . "'";
            }
            
            $this->where[] = array("({$field} {$comparison} {$value})", $orAnd);
        }
        
        return $this;
    }
    
    /**
    * add where statement, only when the value is set
    * 
    * @param string $field
    * @param mixed $value
    * @param string $comparison
    * @param string $orAnd  AND or OR
    * @return SQLQuery
    */
    public function whereWhenSet($field, $value, $comparison = "=", $orAnd = 'AND')
    {
        if(!self::isEmpty($value))
        {
            $this->where($field, $value, $comparison, $orAnd);
        }
        
        return $this;
    }
    
    /**
    * add group statement
    * 
    * @param string $field
    * @return SQLQuery
    */
    public function groupBy($field)
    {
        $this->group[] = $field;
        
        return $this;
    }
    
    /**
    * add order statement
    * 
    * @param string $field
    * @param string $sort   ASc or DESC
    * @return SQLQuery
    */
    public function orderBy($field, $sort = "ASC")
    {
        if($sort != 'ASC')
        {
            $sort = "DESC";
        }
        
        $this->order[] = $field.' '.$sort;
        
        return $this;
    }
    
    /**
    * add limit statement
    * 
    * @param int $amount
    * @param in $start
    * @return SQLQuery
    */
    public function limit($amount, $start = 0)
    {
        $this->limit = "LIMIT {$start}, {$amount}";
        
        return $this;
    }
    
    /**
    * add join statement
    * 
    * @param string $table
    * @param string $local
    * @param string $forign
    * @param string $type   left, right or inner
    * @return SQLQuery
    */
    public function join($table, $local, $forign, $type = 'left')
    {
        $this->join[] = strtoupper($type)." JOIN {$table} ON {$local} = {$forign}";
        
        return $this;
    }
    
    public static function isEmpty($value)
    {
        //((isset($value) && is_numeric($value)) || (isset($value) && !is_numeric($value) && !empty($value)))
        if(isset($value))
        {
            if($value == "")
            {
                return true;
            }
            else if(is_numeric($value))
            {
                return false;
            }
            else if(is_array($value))
            {
                return (count($value) == 0);
            }
            else
            {
                return empty($value);
            }
        }
        
        return true;
    }
    
    /**
    * execute the statement
    * 
    * @return SQLRecord
    */
    public function exec()
    {
        switch($this->action)
        {
            case 'select' :
                return $this->execSelect();
            break;
            case 'insert' :
                return $this->execInsert();
            break;
            case 'delete' :
                return $this->execDelete();
            break;
            case 'update' :
                return $this->execUpdate();
            break;
        }
    }
    
    /**
    * execute the statement as an select
    * 
    * @return SQLRecord
    */
    private function execSelect()
    {
        $query = '';
        
        if(count($this->select) > 0)
        {
            $query .= 'SELECT '.implode(', ', $this->select).' ';
        }
        else
        {
            $query .= 'SELECT * ';
        }
        
        $query .= ' FROM '.implode(', ', $this->table).' ';
        $query .= implode(' ', $this->join).' ';
        
        if(count($this->where) > 0)
        {
            $totalWhere = '';
            
            foreach($this->where as $where)
            {
                if($totalWhere != '')
                {
                    $totalWhere .= ' '.$where[1].' ';
                }
                
                $totalWhere .= $where[0];
            }
            
            $query .= ' WHERE '.$totalWhere.' ';
        }
        
        if(count($this->group) > 0)
        {
            $query .= ' GROUP BY '.implode(', ', $this->group).' ';
        }
        
        if(count($this->order) > 0)
        {
            $query .= ' ORDER BY '.implode(', ', $this->order).' ';
        }
        
        if($this->limit != '')
        {
            $query .= $this->limit;
        }
        
        return SQL::getInstance()->query($query);
    }
    
    /**
    * execute the statement as an insert
    * 
    * @return SQLRecord
    */
    private function execInsert()
    {
        $query = '';
        
        $query .= 'INSERT INTO '.$this->table[0].' ';
        $query .= '('.implode(', ', $this->insert).') ';
        
        $totalValues = '';
        
        foreach($this->insert as $field)
        {
            if($totalValues != '')
            {
                $totalValues .= ', ';
            }
            
            if(is_numeric($this->values[$field]))
            {
                $totalValues .= addSlashesConfig($this->values[$field]);
            }
            else
            {
                $totalValues .= "'".addSlashesConfig($this->values[$field])."'";
            }
        }
        
        $query .= ' VALUES ('.$totalValues.');';
        
        return SQL::getInstance()->query($query);
    }
    
    /**
    * execute the statment as an update
    * 
    * @return SQLRecord
    */
    private function execUpdate()
    {
        $query = '';
        
        $query .= 'UPDATE '.$this->table[0].' ';
        
        $totalValues = '';
        
        foreach($this->update as $field)
        {
            if($totalValues != '')
            {
                $totalValues .= ', ';
            }
            
            $totalValues .= $field.' = ';
            
            if(is_numeric($this->values[$field]))
            {
                $totalValues .= addSlashesConfig($this->values[$field]);
            }
            else
            {
                $totalValues .= "'".addSlashesConfig($this->values[$field])."'";
            }
        }
        
        $query .= ' SET '.$totalValues;
        
        if(count($this->where) > 0)
        {
            $totalWhere = '';
            
            foreach($this->where as $where)
            {
                if($totalWhere != '')
                {
                    $totalWhere .= ' '.$where[1].' ';
                }
                
                $totalWhere .= $where[0];
            }
            
            $query .= ' WHERE '.$totalWhere.' ';
        }
        
        return SQL::getInstance()->query($query);
    }
    
    /**
    * execute statment as an Delete
    * 
    * @return SQLRecord
    */
    private function execDelete()
    {
        $query = '';
        
        $query .= 'DELETE FROM '.$this->table[0].' ';
        
        if(count($this->where) > 0)
        {
            $totalWhere = '';
            
            foreach($this->where as $where)
            {
                if($totalWhere != '')
                {
                    $totalWhere .= ' '.$where[1].' ';
                }
                
                $totalWhere .= $where[0];
            }
            
            $query .= ' WHERE '.$totalWhere.' ';
        }
        
        return SQL::getInstance()->query($query);
    }
}

class SQLRecord
{
    public $record;
    private $count;
    private $records;
    private $insertID = false;
    
    private $index = 0;
    
    public function __construct($record = null)
    {
        $this->records = array();
        $this->record = null;
        $this->count = 0;
        
        if($record != null)
        {
            $this->records = $record->fetchAll(PDO::FETCH_OBJ);
            $this->record = $this->records[0];
            $this->count = $record->rowCount();
            $this->insertID = SQL::getInstance()->getPDOInstance()->lastInsertId();
        }
    }
    
    public function getRows()
    {
        return $this->records;
    }
    
    public function getRow()
    {
        $this->index++;
        
        if($this->records[$this->index-1])
        {
            return $this->records[$this->index-1];
        }
        else
        {
            return false;
        }   
    }
    
    public function reset()
    {
        $this->index = 0;
    }
    
    public function count()
    {
        return $this->count;
    }
    
    public function insertID()
    {
        return $this->insertID;
    }
}

class SQL
{
    private static $instance;
    
    private $host     = '';
    private $database = '';
    private $username = '';
    private $password = '';
    
    private $query;
    
    private $PDOconnection;
    
    public static function getInstance()
    {
        if(!isset(self::$instance))
        {
            self::$instance = new SQL();
        }
        
        return self::$instance;
    }
    
    private function __construct()
    {
        $this->host     = Config::get('dbhost',   'localhost', 'db');
        $this->database = Config::get('database', 'site',      'db');
        $this->username = Config::get('username', 'root',      'db');
        $this->password = Config::get('password', '',          'db');
        
        try
        {
            $this->PDOconnection = new PDO ("mysql:host={$this->host};dbname={$this->database}","{$this->username}","{$this->password}");
        }
        catch(PDOException $e)
        {
            $this->triggerSQLError("Could not connect to host '{$this->host}'", 2);
        }
    }
    
    public function reconnect()
    {
        $this->PDOconnection = new PDO ("mysql:host={$this->host};dbname={$this->database}","{$this->username}","{$this->password}");
    }
    
    public function getPDOInstance()
    {
        return $this->PDOconnection;
    }
    
    public function query($query)
    {
        if(!empty($query))
        {
            $this->query = $query;
            
            $result = $this->PDOconnection->query($query);
            
            if($this->PDOconnection->errorCode() != '00000')
            {
                $error = $this->PDOconnection->errorInfo();
                Debugger::getInstance()->log("SQL ERROR: '{$query}' {$error}");
                $this->triggerSQLError("{$error[1]} - {$error[2]}");
            }
            $result = new SQLRecord($result);
        }
        else
        {
            $result = new SQLRecord(null);
        }
        
        Debugger::getInstance()->log("Query: '{$query}' [{$result->count()} rows returned]");
        
        return $result;
    }
    
    public function multi_query($queries)
    {
        //parse the queries
        $parsedQueries = array();
        $lines = explode("\n", $queries);
        $currentQuery = '';
        
        foreach($lines as $line)
        {
            if(preg_match("/^--/", $line) === 0)
            {
                $currentQuery .= $line."\n";
                
                //check if it's the ending
                if(preg_match('/;$/', $line) == 1)
                {
                    $parsedQueries[] = trim($currentQuery);
                    $currentQuery = '';
                }
            }
        }
        
        //make sure we have the last line too, because if the last query doesn't end with a ; it doesn't reconize it
        if($currentQuery != '')
        {
            $parsedQueries[] = trim($currentQuery);
        }
        
        $results = array();
        
        foreach($parsedQueries as $query)
        {
            $results[] = $this->query($query);
        }
        
        return $results;
    }
    
    private function triggerSQLError($error, $type = 1)
    {
        switch($type)
        {
            case 1 :
                throw new SQLExeption($error."\n QUERY: '" . $this->query . "'");
                break;
            case 2 :
                throw new SQLConnenectionExeption($error."\n QUERY: '" . $this->query . "'");
                break;
        }
    }
}
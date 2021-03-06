<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package libs
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Wrapper for the table info in the info files, these can contain options
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TableInfo
{
    private $name;
    private $options;
    /**
     * Constructor
     * 
     * @param String $ID
     */
    public function __construct($ID)
    {
        if(strpos($ID, ":") !== false)
        {
            $this->name = substr($ID, 0, strpos($ID, ":"));
            $this->options = explode("|", substr($ID, strpos($ID, ":") + 1));
        }
        else
        {
            $this->name = $ID;
            $this->options = array();
        }
    }
    /**
     * Check if an option was set
     * 
     * @param String $key
     * @return Boolean
     */
    public function hasOption($key)
    {
        return in_array($key, $this->options);
    }
    /**
     * Get the name of the table
     * 
     * @return String
     */
    public function getName()
    {
        return $this->__toString();
    }
    /**
     * To String, this is called when cast to string
     * 
     * @return String
     */
    public function __toString()
    {
        return $this->name;
    }
}
/**
 * Helper class for installing and managing components
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class InstallHelper
{
    /**
     * Download and install a component. Returns True on success and False on error
     * 
     * @param string $source
     * @param string $root
     * @return boolean
     */
    public function downloadAndInstall($source, $root = "")
    {
        if($root == "")
        {
            $root = getFrameworkRoot();
        }
        
        if(!file_exists(getFrameworkRoot().'/tmp'))
        {
            mkdir(getFrameworkRoot().'/tmp', 0777, true);
        }
        
        $sourceFile = getFrameworkRoot().'/tmp/'.basename($source);
        
        file_put_contents($sourceFile, file_get_contents($source));
        
        //extract
        $zip = new ZipArchive();
        
        if ($zip->open($sourceFile) === true)
        {
            $zip->extractTo($root);
            $zip->close();
            
            //unlink($sourceFile);
            return true;
        }
        else
        {
            Debugger::getInstance()->warning('Failed extracting file "' .$sourceFile. '"');
            unlink($sourceFile);
            return false;
        }
    }
    /**
     * Install a component
     * 
     * @param string $component
     * @param boolean $installAdmin
     * @return array    (result[, errormessage])
     */
    public function installComponent($component, $installAdmin = true)
    {
        $websiteroot = Config::get("websiteroot", "");
        $frameworkroot = getFrameworkRoot();
        $dir = realpath($frameworkroot.'/components/'.$component);
        $componentInfo = parseInfoFile($dir."/{$component}.info");
        $db = SQL::getInstance();
        
        try
        {
            //run install sql
            $file = $dir.'/db/install.xml';
            if(file_exists($file) && is_readable($file))
            {
                $this->loadTable(file_get_contents($file));
            }
            
            //insert component into the database
            $version = (int) $componentInfo['atlines']['version'];
            
            $db->query(
                "INSERT INTO 
                    `components` 
                (
                    `component_name`, 
                    `component_path`, 
                    `component_auth`,
                    `component_version`
                )
                VALUES
                (
                    '{$component}',
                    '',
                    15,
                    {$version}
                );"
            );
            
            //copy the install files
            if(file_exists($dir.'/install'))
            {
                $this->rcopy($dir.'/install', $websiteroot);
            }
        }
        catch(Exception $e)
        {
            return array(false, $e->getMessage());
        }
        
        return array(true);
    }
    /**
     * Check if an array has numeric keys
     * 
     * @param array $array
     * @return boolean
     */
    public function is_numeric_keys($array)
    {
        foreach($array as $key => $value)
        {
            if(!is_numeric($key))
            {
                return false;
            }
        }
        
        return true;
    }
    /**
     * convert a array to ini format
     * 
     * @param array $array
     * @return string
     */
    public function arrayToIni($array)
    {
        $body = '';
        
        foreach($array as $section => $items)
        {
            if($body != '')
            {
                $body .= "\n";
            }
            
            if(!is_array($items))
            {
                if(is_bool($items))
                {
                    $items = ($items)? 'true' : 'false';
                }
                elseif(!is_numeric($items))
                {
                    $value = "\"{$value}\"";
                }
                
                $body .= "{$section} = {$items}\n";
            }
            elseif($this->is_numeric_keys($items))
            {
                foreach($items as $key => $value)
                {
                    if(is_bool($value))
                    {
                        $value = ($value)? 'true' : 'false';
                    }
                    elseif(!is_numeric($value))
                    {
                        $value = "\"{$value}\"";
                    }
                    
                    $body .= "{$section}[] = {$value}\n";
                }
            }
            else
            {
                $body .= "[{$section}]\n";
                
                foreach($items as $key => $value)
                {
                    if(is_bool($value))
                    {
                        $value = ($value)? 'true' : 'false';
                    }
                    elseif(!is_numeric($value))
                    {
                        $value = "\"{$value}\"";
                    }
                    
                    $body .= "{$key} = {$value}\n";
                }
            }
            
        }
        return $body;
    }
    /**
     * Recusively copy a directory
     * 
     * @param string $source
     * @param string $dest
     * @param boolean $override
     */
    public function rcopy($source, $dest, $override = true)
    {
        //is the source a file?
        if(is_file($source))
        {
            if($override || (!$override && !file_exists($dest)))
            {
                copy($source, $dest);
            }
        }
        elseif(is_dir($source))
        {
            if(!file_exists($dest))
            {
                mkdir($dest, 0777, true);
            }
            
            //loop through it and copy the content
            $content = scandir($source);

            foreach($content as $file)
            {
                if($file != '.' && $file != '..' && $file != '.svn')
                {
                    $this->rcopy($source.'/'.$file, $dest.'/'.$file, $override);
                }
            }
        }
    }
    /**
     * Uninstall a component
     * TODO: Implement
     * 
     * @param string $component
     */
    public function uninstall($component)
    {
        
    }
    /**
     * remove a directorcy and it's content
     * 
     * @param string $source
     */
    public function remove($source)
    {
        $source = realpath($source);
        
        if(is_dir($source))
        {
            $files = scandir($source);
            
            foreach($files as $file)
            {
                if($file != "." && $file != '..')
                {
                    $this->remove($source."/".$file);
                }
            }
            
            rmdir($source);
        }
        else
        {
            unlink($source);
        }
    }
    /**
     * Check if SVN is avalible through the commandline
     * 
     * @return Boolean
     */
    public static function hasSVN()
    {
        $root = getFrameworkRoot();
        $return = array();
        $status;
        
        exec("svn help", $return, $status);
        
        return ($status === 0);
    }
    /**
     * Check if a folder is up-to-date
     * 
     * NOTE: this is a very slow because it needs to access the server over 
     * HTTP. it is not recommended to do often in one call because the running
     * time will be quite a lot.
     * 
     * @param String $folder
     * @return Boolean
     */
    public function svn_up2date($folder)
    {
        $root = getFrameworkRoot();
        $return_loc = array();
        $return_rep = array();
        
        exec("svn info {$root}", $return_loc);
        exec("svn info {$root} -r HEAD", $return_rep);
        
        $locRev = 0;
        $repRev = 0;
        
        foreach($return_loca as $line)
        {
            if(strpos($line, "Last Changed Rev:") === 0)
            {
                $locRev = (int) trim(substr($line, strpos($line, "Last Changed Rev:")));
            }
        }
        
        foreach($return_rep as $line)
        {
            if(strpos($line, "Last Changed Rev:") === 0)
            {
                $repRev = (int) trim(substr($line, strpos($line, "Last Changed Rev:")));
            }
        }
        
        return ($locRev >= $repRev);
    }
    /**
     * Create an XML given a older version of the XML, tables are gathered 
     * from the XML and the $tables field
     * 
     * @param String $oldFileContent
     * @param String $tables
     */
    public function dumpTable($tables = array())
    {
        //Load the old file
        $dom = new DOMDocument('1.0', 'utf-8');
        $dom->formatOutput = true;
        $root = $dom->appendChild($dom->createElement("sqldata"));
        
        foreach($tables as $tableID)
        {
            $table = new TableInfo($tableID);
            $tableRoot = $dom->createElement("table");
            $tableRoot->setAttribute("name", $table->getName());
            
            $rows = array();
            $idField = '';
            $fields = SQL::getInstance()->query("SHOW FIELDS FROM `{$table}`;");
            
            while($row = $fields->getRow())
            {
                $rows[] = $row->Field;
                
                if($row->Key == "PRI" && $row->Extra == "auto_increment")
                {
                    $idField = $row->Field;
                }
                

                $field = $dom->createElement("field");
                $field->appendChild($dom->createElement("name", $row->Field));
                //pk?
                if($row->Key == "PRI")
                {
                    $field->setAttribute("pk", true);
                    if($row->Extra == "auto_increment")
                    {
                        $idField = $row->Field;
                    }
                }
                //type
                $type = $dom->createElement("type", $row->Type);
                $field->appendChild($type);
                //defaultvalue
                $value = $dom->createElement("value", $row->Default);
                if($row->Null == "YES")
                {
                    $value->setAttribute("null", 1);
                }
                else
                {
                    $value->setAttribute("null", 0);
                }
                $field->appendChild($value);
                $field->appendChild($dom->createElement("extra", $row->Extra));
                
                $tableRoot->appendChild($field);
            }
            
            $dataRoot = $dom->createElement("data");
            if(!$table->hasOption("nodata"))
            {
                $values = SQL::getInstance()->query("SELECT * FROM `{$table}`;");
                
                while($row = $values->getRow())
                {
                    $data = $dom->createElement("entry");
                    
                    foreach($rows as $colum)
                    {
                        if($idField != $colum)
                        {
                            $data->appendChild($dom->createElement($colum, $row->$colum));
                        }
                    }
                    
                    $dataRoot->appendChild($data);
                }
            }
            $tableRoot->appendChild($dataRoot);
            $root->appendChild($tableRoot);
        }
        
        return $dom->saveXML();
    }
    /**
     * Sync a database file with the database, returns true if there was 
     * anything to sync, else false
     * 
     * @param String $content
     * @return boolean
     */
    public function loadTable($content)
    {
        $queries = $this->makeSQL($content);
        $execQuery = false;
        
        foreach($queries as $query)
        {
            SQL::getInstance()->query($query);
            $execQuery = true;
        }
        return $execQuery;
    }
    public function needsLoad($content)
    {
        $queries = $this->makeSQL($content);
        return (count($queries) > 0);
    }
    /**
     * Create the SQL queries needed to load the database
     * 
     * @param $content
     * @return 
     */
    private function makeSQL($content)
    {
        $dom = new DOMDocument();
        $dom->loadXML($content);
        $queries = array();
        
        //check the tables
        foreach($dom->getElementsByTagName("table") as $table)
        {
            $tableName = $table->getAttribute("name");
            //table exists?
            $tables = SQL::getInstance()->query("SHOW TABLES LIKE \"{$tableName}\";");
            if($tables->count() == 0)
            {
                //create the table
                $query = "CREATE TABLE `{$tableName}` (\n";
                $fieldsSQL = array();
                $primaryKeys = array();
                $fields = $table->getElementsByTagName("field");
                foreach($fields as $count => $field)
                {
                    $fieldName = $field->getElementsByTagName("name")->item(0)->nodeValue;
                    if($field->hasAttribute("pk") && $field->getAttribute("pk") === "1")
                    {
                        $primaryKeys[] = $fieldName;
                    }
                    $fieldType = $field->getElementsByTagName("type")->item(0)->nodeValue;
                    $fieldNull = $field->getElementsByTagName("value")->item(0)->getAttribute("null") == "1";
                    $fieldValue = $field->getElementsByTagName("value")->item(0)->nodeValue;
                    $fieldExtra = $field->getElementsByTagName("extra")->item(0)->nodeValue;
                    
                    $query .= "  `{$fieldName}` {$fieldType}";
                    if(!$fieldNull) $query .= " NOT NULL";
                    if(!empty($fieldValue)) $query .= " DEFAULT \"{$fieldValue}\"";
                    if(!empty($fieldExtra)) $query .= " {$fieldExtra}";
                    if($count < $fields->length - 1)
                    {
                        $query .= ", \n";
                    }
                    
                    $fieldsSQL[] = $fieldName;
                }
                if(count($primaryKeys) > 0)
                {
                    array_walk($primaryKeys, create_function('&$i,$k', '$i = "`{$i}`";'));
                    $query .= ", \n";
                    $query .= "  PRIMARY KEY (".implode(",", $primaryKeys).")\n";
                }
                $query .= ")";
                
                $queries[] = $query;
                
                //insert data too :D
                foreach($table->getElementsByTagName("data")->item(0)->getElementsByTagName("entry") as $data)
                {
                    $query = SQLQuery::doInsert()
                        ->table($tableName);
                    foreach($fieldsSQL as $field)
                    {
                        $value = $data->getElementsByTagName($field)->item(0)->nodeValue;
                        $query->insert($field, $value);
                    }
                    $queries[] = $query;
                }
            }
            else
            {
                //check if we need to add or delete fields
                $fieldsSQL = array();
                $fieldsDB = array();
                $query = SQL::getInstance()->query("SHOW FIELDS FROM `{$tableName}`;");
                
                while($field = $query->getRow())
                {
                    $fieldsSQL[$field->Field] = $field;
                }
                
                foreach($table->getElementsByTagName("field") as $field)
                {
                    $fieldName = $field->getElementsByTagName("name")->item(0)->nodeValue;
                    
                    //do we need to add it?
                    if(key_exists($fieldName, $fieldsSQL))
                    {
                        if(!$this->isNotChanged($field, $fieldsSQL[$fieldName]))
                        {
                            $fieldType = $field->getElementsByTagName("type")->item(0)->nodeValue;
                            $fieldNull = $field->getElementsByTagName("value")->item(0)->getAttribute("null") == "1";
                            $fieldValue = $field->getElementsByTagName("value")->item(0)->nodeValue;
                            $fieldExtra = $field->getElementsByTagName("extra")->item(0)->nodeValue;
                            
                            $query = "ALTER TABLE `{$tableName}` CHANGE `{$fieldsSQL[$fieldName]->Field}` `{$fieldName}` {$fieldType}";
                            if(!$fieldNull) $query .= " NOT NULL"; else $query .= " NULL";
                            if(!empty($fieldValue)) $query .= " DEFAULT \"{$fieldValue}\"";
                            if(!empty($fieldExtra)) $query .= " {$fieldExtra}";
                            $query .= ";";
                            
                            $queries[] = $query;
                        }
                    }
                    else
                    {
                        $fieldType = $field->getElementsByTagName("type")->item(0)->nodeValue;
                        $fieldNull = $field->getElementsByTagName("value")->item(0)->getAttribute("null") == "1";
                        $fieldValue = $field->getElementsByTagName("value")->item(0)->nodeValue;
                        $fieldExtra = $field->getElementsByTagName("extra")->item(0)->nodeValue;
                        
                        $query = "ALTER TABLE `{$tableName}` ADD `{$fieldName}` {$fieldType}";
                        if(!$fieldNull) $query .= " NOT NULL";
                        if(!empty($fieldValue)) $query .= " DEFAULT \"{$fieldValue}\"";
                        if(!empty($fieldExtra)) $query .= " {$fieldExtra}";
                        $query .= ";";
                        
                        $queries[] = $query;
                    }
                    
                    $fieldsDB[$fieldName] = $field;
                }
                
                foreach(array_diff(array_keys($fieldsSQL), array_keys($fieldsDB)) as $fieldName)
                {
                    $query = "ALTER TABLE `{$tableName}` DROP `{$fieldName}`;";
                    $queries[] = $query;
                }
            }
        }
        
        return $queries;
    }
    /**
     * Check if a DOMElement and the field row from the DB have the same values
     * 
     * @param DOMElement $field
     * @param Object $row
     * @return boolean
     */
    public function isNotChanged($field, $row)
    {
        $fieldName  = $field->getElementsByTagName("name")->item(0)->textContent;
        $fieldType  = $field->getElementsByTagName("type")->item(0)->textContent;
        $fieldValue = $field->getElementsByTagName("value")->item(0)->textContent;
        $fieldNull  = "1" == $field->getElementsByTagName("value")->item(0)->attributes->getNamedItem("null")->textContent;
        $fieldExtra = $field->getElementsByTagName("extra")->item(0)->textContent;
        
        return (
            $fieldName  == $row->Field &&
            $fieldType  == $row->Type &&
            $fieldValue == $row->Default &&
            $fieldNull  == ($row->Null == "YES") &&
            $fieldExtra == $row->Extra
        );
    }
    /**
     * Recusrivly add files to a zip archive
     * 
     * @param String $source
     * @param String $dest
     * @param String $zip
     */
    public function raddFileToZip($source, $dest, $zip)
    {
        if(is_dir($source))
        {
            $files = scandir($source);
            
            if(count($files) > 2)
            {
                foreach($files as $file)
                {
                    if($file != '.' && $file != '..' && $file != '.svn')
                    {
                        $this->raddFileToZip($source.DIRECTORY_SEPARATOR.$file, $dest.DIRECTORY_SEPARATOR.$file, $zip);
                    }
                }
            }
            else
            {
                $zip->addEmptyDir($dest);
            }
        }
        else
        {
            $zip->addFile($source, str_replace(DIRECTORY_SEPARATOR, "/", $dest));
        }
    }
    /**
     * Clear a dir with files, an exlude list is optional
     * 
     * @param String $source
     * @param array $exlude
     */
    public function clearDir($source, $exlude = array())
    {
        $files = scandir($source);
        
        foreach($files as $file)
        {
            if($file != '.' && $file != '..')
            {
                if(is_dir($source.DIRECTORY_SEPARATOR.$file))
                {
                    if(!in_array(realpath($source.DIRECTORY_SEPARATOR.$file), $exlude))
                    {
                        $this->clearDir($source.DIRECTORY_SEPARATOR.$file);
                        rmdir($source.DIRECTORY_SEPARATOR.$file);
                    }
                }
                else
                {
                    if(!in_array(realpath($source.DIRECTORY_SEPARATOR.$file), $exlude))
                    {
                        unlink($source.DIRECTORY_SEPARATOR.$file);
                    }
                }
            }
        }
    }
    /**
     * Pack a directory into a zip file
     * 
     * @param String $source
     * @param String $dest      zip archive, will be created if not exists
     * @return boolean
     */
    public function dir2zip($source, $dest)
    {
        $ds = DIRECTORY_SEPARATOR;
        $zip = new ZipArchive();
        
        if(file_exists($dest))
        {
            unlink($dest);
        }
    
        if($zip->open($dest, ZIPARCHIVE::CREATE))
        {
            $files = scandir($source);
            
            foreach($files as $file)
            {
                if($file != '.' && $file != '..')
                {
                    $this->raddFileToZip($source.$ds.$file, $file, $zip);
                }
            }
            
            return true;
        }
        else
        {
            return false;
        }
        $zip->close();
    }
    /**
     * Get the highest revsion number
     * 
     * @param String $source
     * @return int
     */
    public function getHightestRevNumber($source)
    {
        if(is_array($source))
        {
            $hightest = -1;
            
            foreach($source as $file)
            {
                $rev = $this->getHightestRevNumber($file);
                
                if($rev > $hightest)
                {
                    $hightest = $rev;
                }
            }
            
            return $hightest;
        }
        else
        {
            $source = realpath($source);
            $result = array();
            
            exec("svn info {$source}", $result);
            $matches = array();
            preg_match('/Last Changed Rev: ([0-9]*)/m', implode("\n", $result), $matches);
            
            return (int) trim($matches[1]);
        }
    }
    /**
     * Generate the install XML's for each given component
     * 
     * @param String $components
     */
    public function generateInstallFiles($location)
    {
        $infoFile = parse_ini_file($location."/info.ini");
        
        if(isset($infoFile['tabels']))
        {
            $xmlFile = $location."/db/install.xml";
            file_put_contents($xmlFile, $this->dumpTable($infoFile['tabels']));
        }
    }
}
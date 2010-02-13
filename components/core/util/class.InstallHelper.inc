<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.core.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

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
            $file = $dir.'/db/install.sql';
            
            if(file_exists($file) && is_readable($file))
            {
                $sql = file_get_contents($file);
                if($sql != '')
                {
                    $db->multi_query($sql);
                }
            }
            
            //insert component into the database
            $info = parse_ini_file($dir.'/info.ini', true);
            $patchlevel = $this->getHighestPatchLevel($dir.'/db');
            
            $dir = addslashes($dir);
            
            $version = (int) $componentInfo['atlines']['version'];
            
            $db->query(
                "INSERT INTO 
                    `components` 
                (
                    `component_name`, 
                    `component_requests`, 
                    `component_path`, 
                    `component_auth`,
                    `component_patchlevel`,
                    `component_version`
                )
                VALUES
                (
                    '{$component}',
                    '{$requests}',
                    '{$dir}',
                    15,
                    {$patchlevel},
                    {$version}
                );"
            );
            
            //install the hooks
            if($info['hooks'])
            {
                foreach($info['hooks'] as $hook)
                {
                    $hook_function = explode("->", $hook);
                    $hook_target = explode(":", $hook_function[0]);
                    
                    //insert
                    $db->query(
                        "INSERT INTO 
                            `hooks` 
                        (
                            `hook_component`, 
                            `hook_target`, 
                            `hook_prepost`, 
                            `hook_action`,
                            `hook_function`
                        )
                        VALUES
                        (
                            '{$component}',
                            '{$hook_target[0]}',
                            '{$hook_target[1]}',
                            '{$hook_target[2]}',
                            '{$hook_function[1]}'
                        );"
                    );
                }
            }
            
            //copy the install files
            if(file_exists($dir.'/install'))
            {
                $this->rcopy($dir.'/install', $websiteroot);
            }
            
            //run default sql
            $file = $dir.'/db/default.sql';
            if(file_exists($file) && is_readable($file))
            {
            
                $sql = file_get_contents($file);
                if($sql != '')
                {
                    $db->multi_query($sql);
                }
            }
            
            if($installAdmin)
            {
                //run admin sql
                $file = $dir.'/db/admin.sql';
                if(file_exists($file) && is_readable($file))
                {
                
                    $sql = file_get_contents($file);
                    if($sql != '')
                    {
                        $db->multi_query($sql);
                    }
                }
            }
        }
        catch(Exception $e)
        {
            return array(false, $e->getMessage());
        }
        
        return array(true);
    }
    /**
     * Run the patches for a given component
     * 
     * @param string $component
     */
    public function runPatches($component)
    {
        $components = Config::getInstance()->getComponenets();
        $component = $components[$component];
        
        $current = $component->component_patchlevel;
        $highest = $this->getHighestPatchLevel($component->component_path."/db");
        
        while($current < $highest)
        {
            $current++;
            
            $patch = file_get_contents($component->component_path."/db/patch-{$current}.sql");
            SQL::getInstance()->multi_query($patch);
            
            //TODO: change to table object
            Request::init('core')->doUpdate(array("component_name" => $component->component_name, "component_patchlevel" => $highest));
        }
    }
    /**
     * Get the highest patch number for a given folder
     * 
     * @param string $installDir
     * @return int
     */
    public function getHighestPatchLevel($installDir)
    {
        $files = scandir($installDir);
        $patchlevel = 0;
        
        foreach($files as $file)
        {
            $matches = array();
            
            if(preg_match('/^patch-([0-9]+)\.sql$/', $file, $matches) == 1)
            {
                if($patchlevel < (int) $matches[1])
                {
                    $patchlevel = (int) $matches[1];
                }
            }
        }
        
        return $patchlevel;
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
            elseif(is_numeric_keys($items))
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
}
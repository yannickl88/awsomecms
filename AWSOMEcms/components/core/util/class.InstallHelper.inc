<?php
class InstallHelper
{
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
            
            Request::init('core')->doUpdate(array("component_name" => $component->component_name, "component_patchlevel" => $highest));
        }
    }
    
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
}
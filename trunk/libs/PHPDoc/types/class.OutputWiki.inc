<?php
class OutputWiki
{
    private $refs;

    private function formatClasses($infoXML)
    {
        $pageContent = "";

        if($infoXML->classes->class->count() > 0)
        {
            $pageContent .= "The following classes are located in this package\n";
            $pageContent .= "=== Classes: ===\n";
            foreach($infoXML->classes->class as $class)
            {
                $classHeader = "";

                if($class->abstract->__toString() != "")
                {
                    $classHeader .= "_abstract_ ";
                }
                $classHeader .= ",,class,, {$class->name}";

                if($class->extends->__toString() != "")
                {
                    $extends = $this->getRef($class->extends->__toString());
                    $classHeader .= " ,,extends,, {$extends}";
                }
                if($class->implements->interface)
                {
                    $classHeader .= " ";
                    foreach($class->implements->interface as $key => $interface)
                    {
                        if($key > 0)
                        $classHeader .= ", ";
                        $classHeader .= "`{$interface}`";
                    }
                }

                $pageContent .= "== `{$class->name}` ==\n";
                $pageContent .= "===== {$classHeader} =====\n";
                $text = htmlentities($class->header->text);
                $pageContent .= "{$text}\n";

                $pageContent .= $this->formatMethods($class);
            }
        }

        return $pageContent;
    }
    private function formatMethods($infoXML)
    {
        $pageContent = "";

        foreach($infoXML->method as $function)
        {
            $params = "";
            $id = $function['id'];
            if($function->header->see->__toString() != "")
            {
                //we need to get the description from another file
                $seeStr = $function->header->see->__toString();
                if(strpos($seeStr, "::") !== false) //new format
                {
                    $seeData = explode("::", $function->header->see->__toString());
                }
                else //old format
                {
                    $seeData = explode("#", $function->header->see->__toString());
                }
                $seeData[0] = explode("/", $seeData[0]);
                $seeData[1] = explode("(", $seeData[1]);

                $class = array_pop($seeData[0]);
                $see = $this->refs->xpath("//ref[@id='method.{$class}.{$seeData[1][0]}']");

                if(count($see) > 0)
                {
                    $file = new SimpleXMLElement(file_get_contents(PHPDocConfig::get("output")."/xml/".$this->translatePackageForeignName($see[0]->package->__toString())));
                    $item = $file->xpath("//*[@id='method.{$class}.{$seeData[1][0]}']");
                    $function = $item[0];
                }
            }

            if($function->header->param)
            {
                foreach($function->header->param as $param)
                {
                    if($params != "")
                    {
                        $params .= ", ";
                    }
                    $type = $param['type']->__toString();
                    if($type == "*") $type = "`*`";
                    
                    $type = $this->getRef($type);
                    $params .= "_{$type}_ `{$param->field}`";
                }
            }
            $return = "void";
            if($function->header->return)
            {
                $return = $function->header->return['type'];
                $return = $this->getRef($return);
                if($return == "*") $return = "`*`";
            }
            $accessor = "";
            if($function->accessor->__toString() !== "")
            {
                $accessor .= "*{$function->accessor}* ";
            }
            $static = "";
            if($function->static->__toString() === "1")
            {
                $static .= "*static* ";
            }
            $text = "";
            if($function->header->__toString() != "")
            {
                $text = htmlentities($function->header->text->__toString());
            }

            $pageContent .= " ==== {$accessor}{$static}_{$return}_ *`{$function->name}`* ( {$params} ) ====\n";
            if(!empty($text))
            {
                $pageContent .= "{$text}\n";
            }
        }

        return $pageContent;
    }
    private function formatFunctions($infoXML)
    {
        $pageContent = "";

        if($infoXML->classes->function->count() > 0)
        {
            $pageContent .= "The following functions are located in this package\n";
            $pageContent .= "=== Functions: ===\n";
            foreach($infoXML->classes->function as $function)
            {
                $params = "";
                if($function->header->param)
                {
                    foreach($function->header->param as $param)
                    {
                        if($params != "")
                        {
                            $params .= ", ";
                        }
                        $type = $param['type']->__toString();
                        if($type == "*") $type = "`*`";
                        $type = $this->getRef($type);
                        $params .= "_{$type}_ `{$param->field}`";
                    }
                }
                $return = "void";
                if($function->header->return)
                {
                    $return = $function->header->return['type'];
                    $return = $this->getRef($return);
                    if($return == "*") $return = "`*`";
                }

                $pageContent .= " ==== _{$return}_ *`{$function->name}`* ( {$params} ) ====\n";
                $pageContent .= "{$function->header->text}\n";
            }
        }

        return $pageContent;
    }
    private function formatPackage($infoXML)
    {
        if($infoXML->header->package->__toString() == "")
        {
            $packageName = "default";
        }
        else
        {
            $packageName = $infoXML->header->package->__toString();
        }
        $pageContent = "#labels Documentation\n#summary Documentation for content in the {$packageName} package.\n";
        //name
        $pageContent .= "_Package: '{$packageName}'_ \n\n";
        $pageContent .= htmlentities($infoXML->header->text->__toString())."\n";
        $pageContent .= "== Contents: ==\n";

        $pageContent .= $this->formatFunctions($infoXML);
        $pageContent .= $this->formatClasses($infoXML);

        return $pageContent;
    }
    public function process()
    {
        $dataDir = PHPDocConfig::get("output")."/xml/";
        $this->refs = new SimpleXMLElement(file_get_contents($dataDir."_refs.xml"));

        $files = scandir($dataDir);

        foreach($files as $file)
        {
            if($file != "." && $file != ".." && $file != "_refs.xml")
            {
                $packageXML = new SimpleXMLElement(file_get_contents($dataDir.$file));
                $docName = $this->translatePackageLocalName($packageXML->header->package->__toString());

                file_put_contents(PHPDocConfig::get("output")."/".$docName, $this->formatPackage($packageXML));
            }
        }
    }
    private function translatePackageLocalName($name, $withExtention = true)
    {
        if($name == "")
        {
            $packageName = "Default";
        }
        else
        {
            $packageName = ucfirst(preg_replace_callback("/(\.[a-zA-Z]{1})/", create_function('$a', 'return substr(strtoupper($a[1]), 1);'), $name));
        }
        return "DOC".$packageName.(($withExtention)?".wiki": "");
    }
    private function translatePackageForeignName($name)
    {
        if($name == "")
        {
            $packageName = "default";
        }
        else
        {
            $packageName = str_replace(".", "_", $name);
        }
        return $packageName.".xml";
    }
    private function getRef($name, $type = "class")
    {
        $see = $this->refs->xpath("//ref[@id='{$type}.{$name}']");

        if(count($see) > 0)
        {
            $loc = $this->translatePackageLocalName($see[0]->package->__toString(), false);
            return "[{$loc}#{$name} {$name}]";
        }
        return $name;
    }
}
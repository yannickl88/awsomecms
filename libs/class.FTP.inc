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
 * Helper class for uploading stuff using FTP
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FTP
{
    private static $instance;
    
    private $connection = null;
    private $dirsChecked;
    /**
     * Singelton
     * 
     * @return FTP
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            self::$instance = new self();
        }
        
        return self::$instance;
    }
    /**
     * Constructor
     */
    private function __construct()
    {
        $this->dirsChecked = array();
        $this->openConnection();
    }
    /**
     * Open connection to the FTP server, throws exception if connecton failed
     * 
     * @throws Exception
     */
    private function openConnection()
    {
        $host = Config::get("host", "", "ftp");
        $username = Config::get("username", "", "ftp");
        $password = Config::get("password", "", "ftp");
        
        $this->connection = ftp_connect($host); 
        $login_result = ftp_login($this->connection, $username, $password); 

        // check connection
        if ((!$this->connection) || (!$login_result))
        {
            throw new Exception("FTP connection has failed for {$host} for user {$username}");
        }
    }
    /**
     * Descructor, which closes the connection
     */
    public function __destruct()
    {
        ftp_close($this->connection);
        $this->connection = null;
    }
    /**
     * Upload a directory to the FTP server, throws exception if failed
     * 
     * @param String $source
     * @param String $dest
     * @return String
     * @throws Exception
     */
    public function upload($source, $dest)
    {
        $this->makeDir(dirname($dest));
        
        if($this->connection == null)
        {
            $this->openConnection();
        }
        
        $upload = @ftp_put($this->connection, $dest, $source, FTP_BINARY); 
        
        if (!$upload)
        {
            throw new Exception("FTP upload has failed! {$source} -> {$dest}");
        }
        else
        {
            return "Uploaded {$source} -> {$dest}";
        }
    }
    /**
     * Make a folder on the FTP server
     * 
     * @param unknown_type $dest
     */
    private function makeDir($dest)
    {
        if(in_array($dest, $this->dirsChecked))
        {
            return;
        }
        $dir = dirname($dest);
        $me = substr($dest, strlen($dir) + 1);
        
        if($dir != "\\")
        {
            $parentFiles = $this->makeDir($dir);
            
            if(!in_array($me, $parentFiles))
            {
                ftp_mkdir($this->connection, $dest);
            }
            
            $this->dirsChecked[] = $dest;
        }
        
        return ftp_nlist($this->connection, $dest);
    }
}
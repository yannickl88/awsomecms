<?php
use Symfony\Component\DependencyInjection\ContainerInterface;
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Configuration class, this is used to retrive all the data from seperate config files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Config
{
    private static $instance;

    /**
     * @var ContainerInterface
     */
    private $container;
    private $data;
    private $componenets;

    /**
     * singelton
     *
     * @return Config
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            throw new RuntimeException("Config not initialized, did you forget to call Config::init()?");
        }
        return self::$instance;
    }

    /**
     * @param ContainerInterface $container
     */
    public static function init(ContainerInterface $container)
    {
        self::$instance = new self($container);
    }

    /**
     * Get something from the config
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     * @return mixed
     */
    public static function get($key, $default, $section = false)
    {
        return self::getInstance()->getItem($key, $default, $section);
    }
    /**
     * constructor
     */
    private function __construct(ContainerInterface $container)
    {
        $this->container = $container;

        $this->fillData();
    }
    /**
     * Inject data into the config. This can be used to add data to the
     * config while there is no config file to read yet.
     *
     * @param Array $data
     */
    public function inject($data)
    {
        $this->data = array_merge_recursive($this->data, $data);
    }
    /**
     * Populate the config from the ini files
     */
    private function fillData()
    {
        global $websiteroot;
        $frameworkroot = getFrameworkRoot();

        $this->data = array("debug" => array("debug" => 0), "admin" => array("location" => "/"));

        if(file_exists($websiteroot.'/config.ini'))
        {
            $this->data = parse_ini_file($websiteroot.'/config.ini', true);
        }
        if(file_exists($frameworkroot.'/core/core.ini'))
        {
            $this->data = array_merge_recursive($this->data, parse_ini_file($frameworkroot.'/core/core.ini', true));
        }

        //parse extra ini's
        if(file_exists($websiteroot.'/configs/'))
        {
            $files = scandir($websiteroot.'/configs/');

            foreach($files as $file)
            {
                if($file != '.' && $file != ".." && substr($file, strrpos($file, '.')+1) == 'ini')
                {
                    $this->data = array_merge_recursive($this->data, parse_ini_file($websiteroot.'/configs/'.$file, true));
                }
            }
        }
        //is the config changed?
        if($this->data["debug"]["debug"] >= 1)
        {
            $this->data["checksum"] = md5(serialize($this->data));
        }

        $this->data['layoutmap'] = array();
        $this->data['layoutmap'][] = array(preg_quote($this->data['admin']['location'], "/")."(.*)", "admin_layout.tpl");

        //parse the layout map
        if(file_exists($frameworkroot.'/core/layoutmap.conf'))
        {
            $data = file($frameworkroot.'/core/layoutmap.conf');

            foreach($data as $line)
            {
                $this->data['layoutmap'][] = explode(" ", trim($line));
            }
        }
        //parse the layout map
        if(file_exists($frameworkroot.'/core/robots.conf'))
        {
            $data = file($frameworkroot.'/core/robots.conf');
            $this->data['robots'] = array();

            foreach($data as $line)
            {
                $this->data['robots'][] = preg_quote(trim($line), "/");
            }
        }

        $this->data['websiteroot'] = $websiteroot;
        if(isset($_SERVER['HTTP_HOST']))
            $this->data['baseurl'] = "http://".$_SERVER['HTTP_HOST'];
    }
    /**
     * Get something from the config
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     * @return mixed
     */
    public function getItem($key, $default, $section = false)
    {
        if($section === false)
        {
            if($this->hasKey($key))
            {
                return $this->passthrough($this->data[$key]);
            }
            else
            {
                if($section == "") $section = "GLOBAL";
                Debugger::getInstance()->notice("Missing Config key, '{$section}::{$key}'");
                return $default;
            }
        }
        else
        {
            if($this->hasKey($key, $section))
            {
                return $this->passthrough($this->data[$section][$key]);
            }
            else
            {
                if($section == "") $section = "GLOBAL";
                Debugger::getInstance()->notice("Missing Config key, '{$section}::{$key}'");
                return $default;
            }
        }
    }
	public function hasKey($key, $section = false)
	{
		if($section === false)
			return array_key_exists($key, $this->data);
		else
			return array_key_exists($section, $this->data) && array_key_exists($key, $this->data[$section]);
	}
    /**
     * reload the config values, this should be called when the config is changed during the execution of a page
     */
    public function reload()
    {
        $this->fillData();
    }
    /**
     * Set something for the current request.
     * Note that the value is not saved in a config.
     *
     * @param string $key
     * @param mixed $default
     * @param string $section
     */
    public function set($key, $value, $section = false)
    {
        if($section === false)
        {
            $this->data[$key] = $value;
        }
        else
        {
            $this->data[$section][$key] = $value;
        }
    }
    /**
     * Check if a value contains another value. This is done using bitwise comparison
     *
     * @param int $int
     * @param int $flag
     * @return boolean
     */
    public static function hasFlag($int, $flag)
    {
        return (($int & $flag) == $flag);
    }

    /**
     * If the value starts and ends with a %, assume it is a parameter in the parameters.yml and fetch that
     *
     * @param mixed $value
     * @return mixed
     */
    private function passthrough($value)
    {
        if (is_string($value) && preg_match('/^%.*%$/', $value)) {
            return $this->container->getParameter(substr($value, 1, -1));
        }

        return $value;
    }
}
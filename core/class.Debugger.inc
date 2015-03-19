<?php
use Psr\Log\LoggerInterface;
use Psr\Log\LogLevel;

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
 * Debugger class that handels all debug messages generated from the CMS
 */
class Debugger
{
    const LOG       = LogLevel::DEBUG;
    const NOTICE    = LogLevel::NOTICE;
    const WARNING   = LogLevel::WARNING;
    const ERROR     = LogLevel::ERROR;

    private static $instance;

    private $logger;

    /**
     * singelton
     *
     * @return Debugger      Instance of Debugger
     */
    public static function getInstance()
    {
        if (!self::$instance instanceof self) {
            throw new RuntimeException("Config not initialized, did you forget to call Debugger::init()?");
        }
        return self::$instance;
    }

    /**
     * @param LoggerInterface $logger
     */
    public static function init(LoggerInterface $logger)
    {
        self::$instance = new self($logger);
    }

    /**
     * constructor
     */
    private function __construct(LoggerInterface $logger)
    {
        $this->logger = $logger;
    }

    /**
     * Add a log message
     *
     * @param string $message
     * @param int $level
     */
    public function log($message, $level = Debugger::LOG, $stackTraceID = -1)
    {
        $this->logger->log($level, $message);
    }
    /**
     * Add a warning message
     *
     * @param string $message
     */
    public function warning($message)
    {
        $this->logger->warning($message);
    }
    /**
     * Add a error message
     *
     * @param string $message
     */
    public function error($message)
    {
        $this->logger->error($message);
    }
    /**
     * Add a notice message
     *
     * @param string $message
     */
    public function notice($message)
    {
        $this->logger->notice($message);
    }
}

<?php
namespace Bundle\AwsomecmsBundle\Controller;

use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(service="awsomecms.controller.fallback")
 */
class FallbackController
{
    /**
     * @var ContainerInterface
     */
    private $container;
    private $websiteroot;

    public function __construct(ContainerInterface $container, $websiteroot)
    {
        $this->container   = $container;
        $this->websiteroot = realpath($websiteroot);
    }

    public function fallback(Request $request, $path)
    {
        global $websiteroot, $start;

        $start = microtime(true);
        $websiteroot = $this->websiteroot;

        try {
            require_once '../core/init.inc';

            \Config::init($this->container);
            \Debugger::init($this->container->get('logger'));

            $smarty = new \Smarty();
            $smarty->assign("SCRIPT_START", $start);

            $smarty->compile_check = true;
            $smarty->debugging = false;

            $smarty->setTemplateDir($websiteroot.'/templates');
            $smarty->setConfigDir($websiteroot.'/templates_c');
            $smarty->setCacheDir($websiteroot.'/../cache');


            $smarty->compile_id = getLang();
            $smarty->assign("LANG", $smarty->compile_id);

            $smartyLoader = new \SmartyPageLoader($smarty);
            $smartyLoader->registerModulePlugins($smarty);

            // hacky hacky :D
            $_GET['url'] = $path;
            $resp        = \Controller::init($this->container, $request)
                ->dispatch($smarty, $smartyLoader);

            return new Response($resp);
        } catch (NotInstalledException $e) {
            if (file_exists('install.php') && file_exists('install')) {
                return new RedirectResponse("/install.php");
            }

            throw $e;
        }
    }
}

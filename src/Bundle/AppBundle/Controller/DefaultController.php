<?php
namespace Bundle\AppBundle\Controller;

use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Response;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(service="app.controller.default")
 */
class DefaultController
{
    /**
     * @Route("/henk", name="henk")
     */
    public function indexAction()
    {
        return new Response('HENK');
    }
}

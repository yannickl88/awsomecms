<?php
namespace Bundle\AppBundle\Controller\Admin;

use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
use Symfony\Component\Security\Http\Authentication\AuthenticationUtils;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(path="/admin", service="app.controller.admin.login")
 */
class LoginController
{
    private $auth_utils;

    public function __construct(AuthenticationUtils $auth_utils)
    {
        $this->auth_utils = $auth_utils;
    }

    /**
     * @Route("/login/", name="admin-login")
     * @Template()
     */
    public function loginAction()
    {
        $error         = $this->auth_utils->getLastAuthenticationError();
        $last_username = $this->auth_utils->getLastUsername();

        return [
            'last_username' => $last_username,
            'error'         => $error
        ];
    }

    /**
     * @Route("/login/do/", name="admin-login-check")
     * @Route("/logout/", name="admin-logout")
     */
    public function dummyAction()
    {
    }
}

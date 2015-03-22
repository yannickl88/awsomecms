<?php
namespace Bundle\AppBundle\Controller\Admin;

use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\Routing\RouterInterface;
use Symfony\Component\Security\Core\Authentication\Token\Storage\TokenStorageInterface;
use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;
use Symfony\Component\Security\Http\Authentication\AuthenticationUtils;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(path="/admin", service="app.controller.admin.login")
 */
class LoginController
{
    private $auth_utils;
    private $token_storage;
    private $router;

    public function __construct(
        AuthenticationUtils $auth_utils,
        TokenStorageInterface $token_storage,
        RouterInterface $router
    ) {
        $this->auth_utils    = $auth_utils;
        $this->token_storage = $token_storage;
        $this->router        = $router;
    }

    /**
     * @Route("/login/", name="admin-login")
     * @Template()
     */
    public function loginAction()
    {
        if (
            ($token = $this->token_storage->getToken()) instanceof UsernamePasswordToken
            && $token->isAuthenticated()
        ) {
            return new RedirectResponse($this->router->generate('admin-home'));
        }

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
        // Should never happen
        return new RedirectResponse($this->router->generate('admin-home'));
    }
}

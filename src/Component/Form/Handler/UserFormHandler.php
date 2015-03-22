<?php
namespace Component\Form\Handler;

use Hostnet\Component\Form\AbstractFormHandler;
use Hostnet\Component\Form\FormSuccesHandlerInterface;
use Symfony\Component\HttpFoundation\Request;
use Component\Form\UserData;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\Routing\RouterInterface;
use Doctrine\ORM\EntityManagerInterface;
use Bundle\AppBundle\Entity\User;
use Component\Security\UserEncoder;

class UserFormHandler extends AbstractFormHandler implements FormSuccesHandlerInterface
{
    private $router;
    private $encoder;
    private $em;
    private $data;

    public function __construct(
        RouterInterface $router,
        EntityManagerInterface $em,
        UserEncoder $encoder
    ) {
        $this->router  = $router;
        $this->encoder = $encoder;
        $this->em      = $em;
        $this->data    = new UserData();
    }

    public function getType()
    {
        return 'user';
    }

    public function getData()
    {
        return $this->data;
    }

    public function setUser(UserData $user)
    {
        $this->data = $user;
    }

    public function onSuccess(Request $request)
    {
        $user = $this->data->getOriginal()->setUsername($this->data->getUsername());
        $pass = $this->data->getPassword();

        if ($user->getId() === null || !empty($pass)) {
            $salt             = $this->getRandomString();
            $encoded_password = $this->encoder->encodePassword($pass, $salt);
            $user
                ->setPassword($encoded_password)
                ->setSalt($salt);
        }

        $this->em->persist($user);
        $this->em->flush();

        return new RedirectResponse($this->router->generate('admin-users'));
    }

    private function getRandomString($chars = 9)
    {
        $str   = '';
        $chrs  = array_merge(range('0','9'), range('a','z'), range('A','Z'));

        for ($i = 0; $i < $chars; $i++) {
            $str.= $chrs[rand(0, count($chrs) - 1)];
        }

        return $str;
    }
}

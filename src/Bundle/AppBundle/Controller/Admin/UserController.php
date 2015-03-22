<?php
namespace Bundle\AppBundle\Controller\Admin;

use Bundle\AppBundle\Entity\Repository\UserRepository;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
use Doctrine\Common\Collections\Criteria;
use Component\Form\Handler\UserFormHandler;
use Bundle\AppBundle\Entity\Bundle\AppBundle\Entity;
use Hostnet\Component\Form\FormProviderInterface;
use Symfony\Component\HttpFoundation\Request;
use Component\Form\UserData;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Bundle\AppBundle\Entity\User;
use Component\Form\Component\Form;
use Symfony\Component\HttpFoundation\Symfony\Component\HttpFoundation;
use Symfony\Component\Routing\RouterInterface;
use Doctrine\ORM\EntityManagerInterface;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(path="/admin/users", service="app.controller.admin.user")
 */
class UserController
{
    private $form_provider;
    private $user_repository;
    private $router;
    private $em;

    public function __construct(
        FormProviderInterface  $form_provider,
        UserRepository         $user_repository,
        RouterInterface        $router,
        EntityManagerInterface $em
    ) {
        $this->form_provider   = $form_provider;
        $this->user_repository = $user_repository;
        $this->router          = $router;
        $this->em              = $em;
    }

    /**
     * @Route("/admin", name="admin-users")
     * @Template()
     */
    public function indexAction()
    {
        $items = $this->user_repository->findBy([], ['username' => Criteria::ASC]);

        return [
            'items' => $items
        ];
    }

    /**
     * @Route("/add", name="admin-users-add")
     * @Template()
     */
    public function addAction(Request $request, UserFormHandler $handler)
    {
        if (($response = $this->form_provider->handle($request, $handler)) instanceof RedirectResponse) {
            return $response;
        }

        return [
            'form' => $handler->getForm()->createView()
        ];
    }

    /**
     * @Route("/edit/{user}", name="admin-users-edit")
     * @Template()
     */
    public function editAction(Request $request, User $user, UserFormHandler $handler)
    {
        $handler->setUser(new UserData($user));

        if (($response = $this->form_provider->handle($request, $handler)) instanceof RedirectResponse) {
            return $response;
        }

        return [
            'form' => $handler->getForm()->createView()
        ];
    }

    /**
     * @Route("/delete/{users}", name="admin-users-delete")
     * @Template()
     */
    public function deleteAction(Request $request, $users)
    {
        $user_ids = array_map(function ($id) { return (int) $id; }, explode(';', $users));
        $users    = $this->user_repository->findBy(['id' => $user_ids]);

        if ($request->isMethod(Request::METHOD_POST)) {
            foreach ($users as $user) {
                $this->em->remove($user);
            }

            $this->em->flush();

            return new RedirectResponse($this->router->generate('admin-users'));
        }

        return [
            'users' => $users
        ];
    }
}

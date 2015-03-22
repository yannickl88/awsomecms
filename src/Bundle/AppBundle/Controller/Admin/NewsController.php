<?php
namespace Bundle\AppBundle\Controller\Admin;

use Bundle\AppBundle\Entity\Repository\NewsRepository;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
use Doctrine\Common\Collections\Criteria;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 * @Route(path="/admin/news", service="app.controller.admin.news")
 */
class NewsController
{
    private $news_repository;

    public function __construct(NewsRepository $news_repository)
    {
        $this->news_repository = $news_repository;
    }

    /**
     * @Route("/admin", name="admin-news")
     * @Template()
     */
    public function indexAction()
    {
        $items = $this->news_repository->findBy([], ['date' => Criteria::DESC]);

        return [
            'items' => $items
        ];
    }
}

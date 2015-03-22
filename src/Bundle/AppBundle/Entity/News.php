<?php
namespace Bundle\AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity(repositoryClass="Bundle\AppBundle\Entity\Repository\NewsRepository")
 * @ORM\Table(name="news")
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class News
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer", name="news_id")
     * @var int
     */
    private $id;

    /**
     * @ORM\Column(type="mltext", name="news_title")
     * @var string
     */
    private $title;

    /**
     * @ORM\Column(type="string", name="news_titleurl", length=250)
     * @var string
     */
    private $slug;

    /**
     * @ORM\ManyToOne(targetEntity="User")
     * @ORM\JoinColumn(name="news_user", referencedColumnName="user_id")
     */
    private $user;

    /**
     * @ORM\Column(type="text", name="news_text")
     * @var string
     */
    private $text;

    /**
     * @ORM\Column(type="string", name="news_tag", length=50)
     * @var string
     */
    private $tag;

    /**
     * @ORM\Column(type="date", name="news_date")
     * @var \DateTime
     */
    private $date;

    /**
     * @ORM\Column(type="boolean", name="news_external")
     * @var bool
     */
    private $external;

    /**
     * @ORM\Column(type="string", name="news_extsite", length=50)
     * @var string
     */
    private $external_site;

    /**
     * @ORM\Column(type="string", name="news_extlink", nullable=true, length=200)
     * @var string
     */
    private $external_link;

    /**
     * @ORM\Column(type="boolean", name="news_hidden")
     * @var bool
     */
    private $hidden;

    /**
     * @return int
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @return string
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * @return string
     */
    public function getUser()
    {
        return $this->user;
    }

    /**
     * @return \DateTime
     */
    public function getDate()
    {
        return $this->date;
    }

    /**
     * @return bool
     */
    public function isHidden()
    {
        return $this->hidden;
    }
}

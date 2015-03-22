<?php

use Doctrine\DBAL\Types\Type;
use Symfony\Component\HttpKernel\Kernel;
use Symfony\Component\Config\Loader\LoaderInterface;

class AppKernel extends Kernel
{
    public function registerBundles()
    {
        $bundles = array(
            new Symfony\Bundle\FrameworkBundle\FrameworkBundle(),
            new Symfony\Bundle\SecurityBundle\SecurityBundle(),
            new Symfony\Bundle\TwigBundle\TwigBundle(),
            new Symfony\Bundle\MonologBundle\MonologBundle(),
            new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            new Doctrine\Bundle\DoctrineBundle\DoctrineBundle(),
            new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
            new Bundle\AppBundle\AppBundle(),
            new Bundle\AwsomecmsBundle\AwsomecmsBundle(),
            new Hostnet\Bundle\FormHandlerBundle\FormHandlerBundle(),
        );

        if (in_array($this->getEnvironment(), array('dev', 'test'))) {
            $bundles[] = new Symfony\Bundle\DebugBundle\DebugBundle();
            $bundles[] = new Symfony\Bundle\WebProfilerBundle\WebProfilerBundle();
        }

        return $bundles;
    }

    public function registerContainerConfiguration(LoaderInterface $loader)
    {
        $loader->load($this->getRootDir().'/config/config_'.$this->getEnvironment().'.yml');
    }

    public function boot()
    {
        parent::boot();

        Type::addType('mltext', 'Component\\MLTextType');

        $em = $this->getContainer()->get('doctrine.orm.entity_manager');
        $em->getConnection()->getDatabasePlatform()->registerDoctrineTypeMapping('mltext','mltext');
    }
}

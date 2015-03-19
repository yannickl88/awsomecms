<?php
namespace Bundle\AppBundle;

use Symfony\Component\Config\FileLocator;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\DependencyInjection\Loader\YamlFileLoader;
use Symfony\Component\HttpKernel\Bundle\Bundle;

/**
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class AppBundle extends Bundle
{
    public function build(ContainerBuilder $container)
    {
        // load default services.yml
        $loader = new YamlFileLoader($container, new FileLocator(__DIR__ . '/Resources/config'));
        $files  = ['services.yml', 'controllers.yml'];

        foreach ($files as $file) {
            $loader->load($file);
        }
    }
}

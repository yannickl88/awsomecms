<?php
namespace Component\Form;

use Symfony\Component\Form\AbstractType;
use Symfony\Component\OptionsResolver\OptionsResolverInterface;
use Symfony\Component\Form\FormBuilderInterface;

class UserType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $builder
            ->add('username', 'text', ['label' => 'Name'])
            ->add('password', 'password', ['label' => 'Password'])
            ->add('password2', 'password', ['label' => 'Again'])
            ->add('save', 'submit');
    }

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults([
            'data_class' => 'Component\Form\UserData'
        ]);
    }

    public function getName()
    {
        return 'user';
    }
}

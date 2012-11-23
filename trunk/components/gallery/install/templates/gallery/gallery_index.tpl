{foreach from=$galleries item=gallery}
    <h2>{$gallery->gallery_name}</h2>
    {$gallery->images|count} images
{/foreach}